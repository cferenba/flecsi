/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_topology_mpi_topology_storage_policy_h
#define flecsi_topology_mpi_topology_storage_policy_h

#include "flecsi/topology/mesh_storage.h"

#include <array>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>

#include "flecsi/data/data_client.h"
#include "flecsi/topology/mesh_utils.h"
#include "flecsi/utils/array_ref.h"
#include "flecsi/utils/reorder.h"
#include "flecsi/topology/index_space.h"
#include "flecsi/topology/entity_storage.h"

///
/// \file
/// \date Initial file creation: Apr 04, 2017
///

namespace flecsi {
namespace topology {

///
/// \class mpi_data_handle_policy_t data_handle_policy.h
/// \brief mpi_data_handle_policy_t provides...
///

template <size_t ND, size_t NM>
struct mpi_topology_storage_policy_t
{
  using id_t = utils::id_t;

  using index_spaces_t = 
    std::array<index_space<mesh_entity_base_*, true, true, true, 
    entity_storage_t>, ND + 1>;

  // array of array of domain_connectivity
  std::array<std::array<domain_connectivity<ND>, NM>, NM> topology;

  std::array<index_spaces_t, NM> index_spaces;

  template<size_t D, size_t M, typename ET>
  void
  add_entity(
    ET* ent,
    size_t partition_id = 0
  )
  {
    using dtype = domain_entity<M, ET>;

    auto & is = index_spaces[M][D].template cast<dtype>();

    id_t global_id = id_t::make<D, M>(is.size(), partition_id);

    auto typed_ent = static_cast<mesh_entity_base_t<NM>*>(ent);

    typed_ent->template set_global_id<M>(global_id);
    is.push_back(ent);
  }
  
}; // class mpi_topology_storage_policy_t

} // namespace topology
} // namespace flecsi

#endif // flecsi_mpi_legion_topology_storage_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/

