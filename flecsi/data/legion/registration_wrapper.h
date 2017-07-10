/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_data_legion_registration_wrapper_h
#define flecsi_data_legion_registration_wrapper_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Apr 10, 2017
//----------------------------------------------------------------------------//

#include <cinchlog.h>
#include <string>

#include "flecsi/execution/context.h"
#include "flecsi/data/data_constants.h"
#include "flecsi/data/storage.h"
#include "flecsi/topology/mesh_topology.h"
#include "flecsi/utils/hash.h"
#include "flecsi/utils/tuple_walker.h"

clog_register_tag(registration);

namespace flecsi {
namespace data {

//----------------------------------------------------------------------------//
//!
//----------------------------------------------------------------------------//

template<
  typename DATA_CLIENT_TYPE,
  size_t STORAGE_TYPE,
  typename DATA_TYPE,
  size_t NAMESPACE_HASH,
  size_t NAME_HASH,
  size_t INDEX_SPACE,
  size_t VERSIONS
>
struct legion_field_registration_wrapper__
{
  using field_id_t = Legion::FieldID;

  //--------------------------------------------------------------------------//
  //!
  //--------------------------------------------------------------------------//

  static
  void
  register_callback(
    field_id_t fid
  )
  {
    execution::context_t::field_info_t fi;

    fi.data_client_hash = typeid(DATA_CLIENT_TYPE).hash_code();
    fi.storage_type = STORAGE_TYPE;
    fi.size = sizeof(DATA_TYPE);
    fi.namespace_hash = NAMESPACE_HASH;
    fi.name_hash = NAME_HASH;
    fi.versions = VERSIONS;
    fi.index_space = INDEX_SPACE;
    fi.fid = fid;

    execution::context_t::instance().register_field_info(fi);
  } // register_callback

}; // class legion_field_registration_wrapper__

//----------------------------------------------------------------------------//
//!
//----------------------------------------------------------------------------//

template<
  typename DATA_CLIENT_TYPE,
  size_t NAMESPACE_HASH,
  size_t NAME_HASH
>
struct legion_client_registration_wrapper__
{
}; // class legion_client_registration_wrapper__

//----------------------------------------------------------------------------//
//!
//----------------------------------------------------------------------------//

template<
  typename POLICY_TYPE,
  size_t NAMESPACE_HASH,
  size_t NAME_HASH
>
struct legion_client_registration_wrapper__<
  flecsi::topology::mesh_topology_t<POLICY_TYPE>,
  NAMESPACE_HASH,
  NAME_HASH
>
{
  using field_id_t = Legion::FieldID;

  using CLIENT_TYPE =
    typename flecsi::topology::mesh_topology_t<POLICY_TYPE>;

  //--------------------------------------------------------------------------//
  //!
  //--------------------------------------------------------------------------//

  struct entity_walker_t :
    public flecsi::utils::tuple_walker__<entity_walker_t>
  {

    template<
      typename TUPLE_ENTRY_TYPE
    >
    void
    handle_type()
    {
      using INDEX_TYPE =
        typename std::tuple_element<0, TUPLE_ENTRY_TYPE>::type;
      using DOMAIN_TYPE =
        typename std::tuple_element<1, TUPLE_ENTRY_TYPE>::type;
      using ENTITY_TYPE =
        typename std::tuple_element<2, TUPLE_ENTRY_TYPE>::type;

      constexpr size_t entity_hash = utils::hash::client_entity_hash<
        NAMESPACE_HASH,
        NAME_HASH,
        INDEX_TYPE::value,
        DOMAIN_TYPE::value,
        ENTITY_TYPE::dimension
        >
        ();

      using wrapper_t = legion_field_registration_wrapper__<
        CLIENT_TYPE,
        flecsi::data::dense,
        ENTITY_TYPE,
        entity_hash,
        0,
        INDEX_TYPE::value,
        1
      >;

      const size_t client_key = typeid(CLIENT_TYPE).hash_code();
      const size_t key = utils::hash::client_internal_field_hash<
        utils::const_string_t("__flecsi_internal_field_bash_base__").hash(),
        INDEX_TYPE::value
      >();

      storage_t::instance().register_field(client_key,
        key, wrapper_t::register_callback);
    } // handle_type

  }; // struct entity_walker_t

  struct connectivity_walker_t :
    public flecsi::utils::tuple_walker__<connectivity_walker_t>
  {

    template<
      typename TUPLE_ENTRY_TYPE
    >
    void
    handle_type()
    {
      using INDEX_TYPE =
        typename std::tuple_element<0, TUPLE_ENTRY_TYPE>::type;
      using DOMAIN_TYPE =
        typename std::tuple_element<1, TUPLE_ENTRY_TYPE>::type;
      using FROM_ENTITY_TYPE =
        typename std::tuple_element<2, TUPLE_ENTRY_TYPE>::type;
      using TO_ENTITY_TYPE =
        typename std::tuple_element<3, TUPLE_ENTRY_TYPE>::type;

      constexpr size_t adjacency_hash =
        utils::hash::client_adjacency_hash<
          NAMESPACE_HASH,
          NAME_HASH,
          INDEX_TYPE::value,
          DOMAIN_TYPE::value, // from and to domains are the same
          DOMAIN_TYPE::value, // for connectivity information
          FROM_ENTITY_TYPE::dimension,
          TO_ENTITY_TYPE::dimension
          >
          ();

      using wrapper_t = legion_field_registration_wrapper__<
        CLIENT_TYPE,
        flecsi::data::dense,
        size_t,
        adjacency_hash,
        0,
        INDEX_TYPE::value,
        1
      >;

      const size_t client_key = typeid(CLIENT_TYPE).hash_code();
      const size_t key = utils::hash::client_internal_field_hash<
        utils::const_string_t("__flecsi_internal_field_bash_base__").hash(),
        INDEX_TYPE::value
      >();

      storage_t::instance().register_field(client_key,
        key, wrapper_t::register_callback);
    } // handle_type

  }; // struct connectivity_walker_t

  struct binding_walker_t :
    public flecsi::utils::tuple_walker__<binding_walker_t>
  {

    template<
      typename TUPLE_ENTRY_TYPE
    >
    void
    handle_type()
    {
      using INDEX_TYPE =
        typename std::tuple_element<0, TUPLE_ENTRY_TYPE>::type;
      using FROM_DOMAIN_TYPE =
        typename std::tuple_element<1, TUPLE_ENTRY_TYPE>::type;
      using TO_DOMAIN_TYPE =
        typename std::tuple_element<2, TUPLE_ENTRY_TYPE>::type;
      using FROM_ENTITY_TYPE =
        typename std::tuple_element<3, TUPLE_ENTRY_TYPE>::type;
      using TO_ENTITY_TYPE =
        typename std::tuple_element<4, TUPLE_ENTRY_TYPE>::type;

      constexpr size_t adjacency_hash =
        utils::hash::client_adjacency_hash<
          NAMESPACE_HASH,
          NAME_HASH,
          INDEX_TYPE::value,
          FROM_DOMAIN_TYPE::value,
          TO_DOMAIN_TYPE::value,
          FROM_ENTITY_TYPE::dimension,
          TO_ENTITY_TYPE::dimension
          >
          ();

      using wrapper_t = legion_field_registration_wrapper__<
        CLIENT_TYPE,
        flecsi::data::dense,
        size_t,
        adjacency_hash,
        0,
        INDEX_TYPE::value,
        1
      >;

      const size_t client_key = typeid(CLIENT_TYPE).hash_code();
      const size_t key = utils::hash::client_internal_field_hash<
        utils::const_string_t("__flecsi_internal_field_bash_base__").hash(),
        INDEX_TYPE::value
      >();

      storage_t::instance().register_field(client_key,
        key, wrapper_t::register_callback);
    } // handle_type

  }; // struct binding_walker_t

  //--------------------------------------------------------------------------//
  //!
  //--------------------------------------------------------------------------//

  static
  void
  register_callback(
    field_id_t fid
  )
  {
    using entity_types_t = typename POLICY_TYPE::entity_types;
    using connectivities = typename POLICY_TYPE::connectivities;
    using bindings = typename POLICY_TYPE::bindings;

    const size_t client_key = typeid(CLIENT_TYPE).hash_code();
    auto const & field_registry = storage_t::instance().field_registry();

    // Only register field attributes if this is the first time
    // that we have seen this type.
    if(field_registry.find(client_key) == field_registry.end()) {
      entity_walker_t entity_walker;
      entity_walker.template walk_types<entity_types_t>();

      connectivity_walker_t connectivity_walker;
      connectivity_walker.template walk_types<connectivities>();

      binding_walker_t binding_walker;
      binding_walker.template walk_types<bindings>();
    } // if

  } // register_callback

}; // class legion_client_registration_wrapper__

} // namespace data
} // namespace flecsi

#endif // flecsi_data_legion_registration_wrapper_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
