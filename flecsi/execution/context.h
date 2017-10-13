/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
 * /@@/////  /@@          @@////@@ @@////// /@@
 * /@@       /@@  @@@@@  @@    // /@@       /@@
 * /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
 * /@@////   /@@/@@@@@@@/@@       ////////@@/@@
 * /@@       /@@/@@//// //@@    @@       /@@/@@
 * /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
 * //       ///  //////   //////  ////////  //
 *
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_execution_context_h
#define flecsi_execution_context_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Oct 19, 2015
//----------------------------------------------------------------------------//

#include <algorithm>
#include <cstddef>
#include <map>
#include <unordered_map>

#include "cinchlog.h"
#include "flecsi/execution/common/execution_state.h"
#include "flecsi/utils/const_string.h"
#include "flecsi/coloring/adjacency_types.h"
#include "flecsi/coloring/coloring_types.h"
#include "flecsi/coloring/index_coloring.h"
#include "flecsi/runtime/types.h"

clog_register_tag(context);

namespace flecsi {
namespace execution {

//----------------------------------------------------------------------------//
//! The context__ type provides a high-level runtime context interface that
//! is implemented by the given context policy.
//!
//! @tparam CONTEXT_POLICY The backend context policy.
//!
//! @ingroup execution
//----------------------------------------------------------------------------//

template<class CONTEXT_POLICY>
struct context__ : public CONTEXT_POLICY
{
  using index_coloring_t = flecsi::coloring::index_coloring_t;
  using coloring_info_t = flecsi::coloring::coloring_info_t;
  using set_coloring_info_t = flecsi::coloring::set_coloring_info_t;
  using adjacency_info_t = flecsi::coloring::adjacency_info_t;

  //--------------------------------------------------------------------------//
  //! The unique_tid_t type create a unique id generator for registering
  //! tasks.
  //--------------------------------------------------------------------------//

  //using unique_tid_t = utils::unique_id_t<task_id_t, FLECSI_GENERATED_ID_MAX>;

  //--------------------------------------------------------------------------//
  //! Adjacency triple: index space, from index space, to index space
  //--------------------------------------------------------------------------//

  using adjacency_triple_t = std::tuple<size_t, size_t, size_t>;

  //--------------------------------------------------------------------------//
  // Gathers info about registered data fields.
  //--------------------------------------------------------------------------//

  struct field_info_t{
    size_t data_client_hash;
    size_t storage_type;
    size_t size;
    size_t namespace_hash;
    size_t name_hash;
    size_t versions;
    field_id_t fid;
    size_t index_space;
    size_t key;
  }; // struct field_info_t

  //--------------------------------------------------------------------------//
  // Gathers info about registered futures.
  //--------------------------------------------------------------------------//

  struct future_info_t{
    size_t size;
    size_t namespace_hash;
    size_t name_hash;
    size_t versions;
    future_id_t fid;
  }; // struct future_info_t

  //--------------------------------------------------------------------------//
  // Field info map for fields in SPMD task, key1 =
  // (data client hash, index space), key2 = fid
  //--------------------------------------------------------------------------//

  using field_info_map_t =
    std::map<std::pair<size_t, size_t>, std::map<field_id_t, field_info_t>>;

  //--------------------------------------------------------------------------//
  // Function interface.
  //--------------------------------------------------------------------------//

  //--------------------------------------------------------------------------//
  //! FIXME: This interface needs to be updated.
  //--------------------------------------------------------------------------//

  template<
    typename RETURN,
    typename ARG_TUPLE,
    RETURN (*FUNCTION)(ARG_TUPLE),
    size_t KEY
  >
  bool
  register_function(
  )
  {
    clog_assert(function_registry_.find(KEY) == function_registry_.end(),
                "function has already been registered");

    const std::size_t addr = reinterpret_cast<std::size_t>(FUNCTION);
    clog(info) << "Registering function: " << addr << std::endl;

    function_registry_[KEY] =
      reinterpret_cast<void *>(FUNCTION);
    return true;
  } // register_function

  //--------------------------------------------------------------------------//
  //! FIXME: Add description.
  //--------------------------------------------------------------------------//

  void *
  function(
    size_t key
  )
  {
    return function_registry_[key];
  } // function


  //--------------------------------------------------------------------------//
  //! Meyer's singleton instance.
  //!
  //! @return The single instance of this type.
  //--------------------------------------------------------------------------//

  static
  context__ &
  instance()
  {
    static context__ context;
    return context;
  } // instance

  //--------------------------------------------------------------------------//
  //! Return the color for which the context was initialized.
  //--------------------------------------------------------------------------//

  size_t
  color()
  const
  {
    return CONTEXT_POLICY::color();
  } // color

  //--------------------------------------------------------------------------//
  //! Return the number of colors.
  //--------------------------------------------------------------------------//

  size_t
  colors()
  const
  {
    return CONTEXT_POLICY::colors();
  } // color

  //--------------------------------------------------------------------------//
  //! Add an index map. This map can be used to go between mesh and locally
  //! compacted index spaces.
  //!
  //! @param index_space The map key.
  //! @param index_map   The map to add.
  //--------------------------------------------------------------------------//

  void
  add_index_map(
    size_t index_space,
    std::map<size_t, size_t> & index_map
  )
  {
    index_map_[index_space] = index_map;

    for(auto i: index_map) {
      reverse_index_map_[index_space][i.second] = i.first;
    } // for
  } // add_index_map

  //--------------------------------------------------------------------------//
  //! Return the index map associated with the given index space.
  //!
  //! @param index_space The map key.
  //--------------------------------------------------------------------------//

  auto &
  index_map(
    size_t index_space
  )
  {
    clog_assert(index_map_.find(index_space) != index_map_.end(),
      "invalid index space");

    return index_map_[index_space];
  } // index_map

  const auto &
  index_map(
    size_t index_space
  ) const
  {
    clog_assert(index_map_.find(index_space) != index_map_.end(),
      "invalid index space");

    return index_map_.at(index_space);
  } // index_map

  const auto &
  cis_to_gis_map(
    size_t index_space
  ) const
  {
    return cis_to_gis_map_.at(index_space);
  }

  auto &
  cis_to_gis_map(
    size_t index_space
  )
  {
    return cis_to_gis_map_[index_space];
  }

  const auto &
  gis_to_cis_map(
    size_t index_space
  ) const
  {
    return gis_to_cis_map_.at(index_space);
  }

  auto &
  gis_to_cis_map(
    size_t index_space
  )
  {
    return gis_to_cis_map_[index_space];
  }

  //--------------------------------------------------------------------------//
  //! Return the index map associated with the given index space.
  //!
  //! @param index_space The map key.
  //--------------------------------------------------------------------------//

  auto &
  reverse_index_map(
    size_t index_space
  )
  {
    clog_assert(reverse_index_map_.find(index_space) !=
      reverse_index_map_.end(), "invalid index space");

    return reverse_index_map_[index_space];
  } // reverse_index_map

  const auto &
  reverse_index_map(
    size_t index_space
  ) const
  {
    clog_assert(reverse_index_map_.find(index_space) !=
      reverse_index_map_.end(), "invalid index space");

    return reverse_index_map_.at(index_space);
  } // reverse_index_map

  //--------------------------------------------------------------------------//
  //! Add an intermediate map. This map can be used to go between mesh and
  //! locally compacted index spaces for intermediate entities.
  //!
  //! @param dimension        The entity dimension.
  //! @param domain           The entity domain.
  //! @param intermediate_map The map to add.
  //--------------------------------------------------------------------------//

  void
  add_intermediate_map(
    size_t dimension,
    size_t domain,
    std::unordered_map<size_t, std::vector<size_t>> & intermediate_map
  )
  {
    const size_t key = utils::hash::intermediate_hash(dimension, domain);
    intermediate_map_[key] = intermediate_map;

    for(auto i: intermediate_map) {
      reverse_intermediate_map_[key][i.second] = i.first;
    } // for
  } // add_intermediate_map

  //--------------------------------------------------------------------------//
  //! Return the intermediate map associated with the given dimension and
  //! domain.
  //!
  //! @param dimension The entity dimension.
  //! @param domain    The entity domain.
  //--------------------------------------------------------------------------//

  auto const &
  intermediate_map(
    size_t dimension,
    size_t domain
  )
  const
  {
    const size_t key = utils::hash::intermediate_hash(dimension, domain);

    clog_assert(intermediate_map_.find(key) != intermediate_map_.end(),
      "invalid index space");

    return intermediate_map_.at(key);
  } // intermediate_map

  //--------------------------------------------------------------------------//
  //! Return the index map associated with the given index space.
  //!
  //! @param dimension The entity dimension.
  //! @param domain    The entity domain.
  //--------------------------------------------------------------------------//

  auto const &
  reverse_intermediate_map(
    size_t dimension,
    size_t domain
  )
  const
  {
    const size_t key = utils::hash::intermediate_hash(dimension, domain);

    clog_assert(reverse_intermediate_map_.find(key) !=
      reverse_intermediate_map_.end(), "invalid index space");

    return reverse_intermediate_map_.at(key);
  } // reverse_intermediate_map

  //--------------------------------------------------------------------------//
  //! Add an index coloring.
  //!
  //! @param index_space The map key.
  //! @param coloring The index coloring to add.
  //! @param coloring The index coloring information to add.
  //--------------------------------------------------------------------------//

  void
  add_coloring(
    size_t index_space,
    index_coloring_t & coloring,
    std::unordered_map<size_t, coloring_info_t> & coloring_info
  )
  {
    clog_assert(colorings_.find(index_space) == colorings_.end(),
      "color index already exists");

    colorings_[index_space] = coloring;
    coloring_info_[index_space] = coloring_info;
  } // add_coloring

  void
  add_set_coloring(
    size_t index_space,
    std::unordered_map<size_t, coloring_info_t> & coloring_info
  )
  {
    coloring_info_[index_space] = coloring_info;
  }

  //--------------------------------------------------------------------------//
  //! Return the index coloring referenced by key.
  //!
  //! @param index_space The key associated with the coloring to be returned.
  //--------------------------------------------------------------------------//

  index_coloring_t &
  coloring(
    size_t index_space
  )
  {
    if(colorings_.find(index_space) == colorings_.end()) {
      clog(fatal) << "invalid index_space " << index_space << std::endl;
    } // if

    return colorings_[index_space];
  } // coloring

  //--------------------------------------------------------------------------//
  //! Return the index coloring information referenced by key.
  //!
  //! @param index_space The key associated with the coloring information
  //!                    to be returned.
  //--------------------------------------------------------------------------//

  const std::unordered_map<size_t, coloring_info_t> &
  coloring_info(
    size_t index_space
  )
  {
    if(coloring_info_.find(index_space) == coloring_info_.end()) {
      clog(fatal) << "invalid index space " << index_space << std::endl;
    } // if

    return coloring_info_[index_space];
  } // coloring_info

  //--------------------------------------------------------------------------//
  //! Return the coloring map (convenient for iterating through all
  //! of the colorings.
  //!
  //! @return The map of index colorings.
  //--------------------------------------------------------------------------//

  const std::map<size_t, index_coloring_t> &
  coloring_map()
  const
  {
    return colorings_;
  } // colorings

  //--------------------------------------------------------------------------//
  //! Return the coloring info map (convenient for iterating through all
  //! of the colorings.
  //!
  //! @return The map of index coloring information.
  //--------------------------------------------------------------------------//

  const std::map<
    size_t,
    std::unordered_map<size_t, coloring_info_t>
  > &
  coloring_info_map()
  const
  {
    return coloring_info_;
  } // colorings

  //--------------------------------------------------------------------------//
  //! Add an adjacency/connectivity from one index space to another.
  //!
  //! @param from_index_space The index space id of the from side
  //! @param to_index_space The index space id of the to side
  //--------------------------------------------------------------------------//

  void
  add_adjacency(
    adjacency_info_t & adjacency_info
  )
  {
    clog_assert(adjacency_info_.find(adjacency_info.index_space) ==
      adjacency_info_.end(),
      "adjacency exists");

    adjacency_info_.emplace(adjacency_info.index_space,
      std::move(adjacency_info));
  } // add_adjacency

  //--------------------------------------------------------------------------//
  //! Return the set of registered adjacencies.
  //!
  //! @return The set of registered adjacencies
  //--------------------------------------------------------------------------//

  const std::map<size_t, adjacency_info_t> &
  adjacency_info()
  const
  {
    return adjacency_info_;
  } // adjacencies

  //--------------------------------------------------------------------------//
  //! Register field info for index space and field id.
  //!
  //! @param index_space virtual index space
  //! @param field allocated field id
  //! @param field_info field info as registered
  //--------------------------------------------------------------------------//

  void register_field_info(field_info_t& field_info){
    field_info_vec_.emplace_back(std::move(field_info));
  }

  /--------------------------------------------------------------------------//
  //! Register future info for future id.
  //!
  //! @param future allocated field id
  //! @param future_info field info as registered
  //--------------------------------------------------------------------------//

  void register_future_info(future_info_t& future_info){
    future_info_vec_.emplace_back(std::move(future_info));
  }


  //--------------------------------------------------------------------------//
  //! Return registered fields
  //--------------------------------------------------------------------------//

  const std::vector<field_info_t>&
  registered_fields()
  const
  {
    return field_info_vec_;
  }

  //--------------------------------------------------------------------------//
  //! Return registered futures
  //--------------------------------------------------------------------------//

  const std::vector<future_info_t>&
  registered_futures()
  const
  {
    return future_info_vec_;
  }
  //--------------------------------------------------------------------------//
  //! Add an adjacency index space.
  //!
  //! @param index_space index space to add.
  //--------------------------------------------------------------------------//

  void
  add_adjacency_triple(const adjacency_triple_t& triple)
  {
    adjacencies_.emplace(std::get<0>(triple), triple);
  }

  //--------------------------------------------------------------------------//
  //! Return set of all adjacency index spaces.
  //--------------------------------------------------------------------------//

  auto&
  adjacencies()
  const
  {
    return adjacencies_;
  }

  //--------------------------------------------------------------------------//
  //! Put field info for index space and field id.
  //!
  //! @param field_info field info as registered
  //--------------------------------------------------------------------------//

  void
  put_field_info(
    const field_info_t& field_info
  )
  {
    size_t index_space = field_info.index_space;
    size_t data_client_hash = field_info.data_client_hash;
    field_id_t fid = field_info.fid;

    field_info_map_[{data_client_hash, index_space}].emplace(fid, field_info);

    field_map_.insert({{field_info.data_client_hash, field_info.key},
                       {index_space, fid}});
  } // put_field_info

  //--------------------------------------------------------------------------//
  //! Get registered field info map for read access.
  //--------------------------------------------------------------------------//

  const field_info_map_t&
  field_info_map()
  const
  {
    return field_info_map_;
  } // field_info_map

  //--------------------------------------------------------------------------//
  //! Get field map for read access.
  //--------------------------------------------------------------------------//
  const std::map<std::pair<size_t, size_t>, std::pair<size_t, field_id_t>>
  field_map()
  const
  {
    return field_map_;
  } // field_info_map
  //--------------------------------------------------------------------------//
  //! Lookup registered field info from data client and namespace hash.
  //! @param data_client_hash data client type hash
  //! @param namespace_hash namespace/field name hash
  //!-------------------------------------------------------------------------//

  const field_info_t&
  get_field_info(
    size_t data_client_hash,
    size_t namespace_hash)
  const
  {
    auto itr = field_map_.find({data_client_hash, namespace_hash});
    clog_assert(itr != field_map_.end(), "invalid field");

    auto iitr = field_info_map_.find({data_client_hash, itr->second.first});
    clog_assert(iitr != field_info_map_.end(), "invalid index_space");

    auto fitr = iitr->second.find(itr->second.second);
    clog_assert(fitr != iitr->second.end(), "invalid fid");

    return fitr->second;
  }

  //--------------------------------------------------------------------------//
  //! Lookup registered future info from  namespace hash.
  //! @param namespace_hash namespace/field name hash
  //!-------------------------------------------------------------------------//

  const field_info_t&
  get_field_info(
    size_t namespace_hash)
  const
  {
//FIXME
    return fitr->second;
  }


  //------------------------------------------------------------------------//
  //! advance state of the execution flow
  //------------------------------------------------------------------------//

  void
  advance_state()
  {
    execution_state_ ++;
  }

  //------------------------------------------------------------------------//
  //!  state of the execution flow
  //------------------------------------------------------------------------//

  void
  lower_state()
  {
    execution_state_ --;
  }


  //------------------------------------------------------------------------//
  //! return current execution state
  //------------------------------------------------------------------------//
  size_t
  execution_state()
  {
    return execution_state_;
  }
 

private:

  // Default constructor
  context__() : CONTEXT_POLICY() {}

  // Destructor
  ~context__() {}

  // We don't need any of these
  context__(const context__ &) = delete;
  context__ & operator = (const context__ &) = delete;
  context__(context__ &&) = delete;
  context__ & operator = (context__ &&) = delete;

  //--------------------------------------------------------------------------//
  // Function data members.
  //--------------------------------------------------------------------------//

  std::unordered_map<size_t, void *>
    function_registry_;
  //--------------------------------------------------------------------------//
  // Field info vector for registered fields in TLT
  //--------------------------------------------------------------------------//

  std::vector<field_info_t> field_info_vec_;

  //--------------------------------------------------------------------------//
  // Future info vector for registered fields in TLT
  //--------------------------------------------------------------------------//

  std::vector<future_info_t> future_info_vec_;

  //--------------------------------------------------------------------------//
  // Field info map for fields in SPMD task, key1 = (data client hash, index space), key2 = fid
  //--------------------------------------------------------------------------//

  field_info_map_t field_info_map_;

  //--------------------------------------------------------------------------//
  // Future info map for fields in SPMD task, key1 = future id
  //--------------------------------------------------------------------------//

  std::map<future_id_t, future_info_t> future_info_map_;

  //--------------------------------------------------------------------------//
  // Map of adjacency triples. key: adjacency index space
  //--------------------------------------------------------------------------//

  std::map<size_t, adjacency_triple_t> adjacencies_;

  //--------------------------------------------------------------------------//
  // Field map, key1 = (data client hash, name/namespace hash)
  // value = (index space, fid)
  //--------------------------------------------------------------------------//

  std::map<std::pair<size_t, size_t>, std::pair<size_t, field_id_t>>
    field_map_;

  // key: virtual index space id
  // value: coloring indices (exclusive, shared, ghost)
  std::map<size_t, index_coloring_t> colorings_;

  // key: mesh index space entity id
  std::map<size_t, std::map<size_t, size_t>> index_map_;
  std::map<size_t, std::map<size_t, size_t>> reverse_index_map_;

#if 0
  std::map<size_t, std::map<size_t, size_t>> cis_to_mis_map_;
  std::map<size_t, std::map<size_t, size_t>> mis_to_cis_map_;
#endif

  // key: mesh index space entity id
  std::map<size_t, std::map<size_t, size_t>> cis_to_gis_map_;
  std::map<size_t, std::map<size_t, size_t>> gis_to_cis_map_;

  // key: intermediate entity to vertex ids
  std::map<size_t, std::unordered_map<size_t, std::vector<size_t>>>
    intermediate_map_;

  struct vector_hash_t
  {
    size_t
    operator () (
      std::vector<size_t> const & v
    )
    const
    {
      size_t h{0};
      for(auto i: v) {
        h |= i;
      } // for

      return h;
    } // operator ()
  }; // struct vector_hash_t

  struct vector_equal_t
  {
    bool
    operator () (
      std::vector<size_t> a,
      std::vector<size_t> b
    )
    const
    {
      std::sort( a.begin(), a.end() );
      std::sort( b.begin(), b.end() );
      return (a == b );
    } // operator ()
  }; // struct vector_hash_t

  std::map<size_t, std::unordered_map<std::vector<size_t>, size_t,
    vector_hash_t, vector_equal_t>> reverse_intermediate_map_;

  // key: virtual index space.
  // value: map of color to coloring info
  std::map<size_t,
    std::unordered_map<size_t, coloring_info_t>> coloring_info_;

  // key is index space
  std::map<size_t, adjacency_info_t> adjacency_info_;

  //-------------------------------------------------------------------------//
  // Execution state 
  //-------------------------------------------------------------------------//

   size_t execution_state_ = SPECIALIZATION_TLT_INIT;

}; // class context__

} // namespace execution
} // namespace flecsi

//----------------------------------------------------------------------------//
// This include file defines the FLECSI_RUNTIME_CONTEXT_POLICY used below.
//----------------------------------------------------------------------------//

#include "flecsi/runtime/flecsi_runtime_context_policy.h"

namespace flecsi {
namespace execution {

//----------------------------------------------------------------------------//
//! The context_t type is the high-level interface to the FleCSI runtime
//! context.
//!
//! @ingroup execution
//----------------------------------------------------------------------------//

using context_t = context__<FLECSI_RUNTIME_CONTEXT_POLICY>;

} // namespace execution
} // namespace flecsi

#endif // flecsi_execution_context_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
