/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_coloring_adjacency_types_h
#define flecsi_coloring_adjacency_types_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Jun 13, 2017
//----------------------------------------------------------------------------//

namespace flecsi {
namespace coloring {

//----------------------------------------------------------------------------//
//! Type for passing adjacency information from the specialization to the
//! FleCSI runtime.
//----------------------------------------------------------------------------//

struct adjacency_info_t {

  //! The index space of the adjacency information itself.
  size_t index_space;

  //! The index space of the from entities.
  size_t from_index_space;

  //! The index space of the to entities.
  size_t to_index_space;

  //! The size of adjacency array for each color.
  std::vector<size_t> color_sizes;

}; // struct adjacency_info_t

} // namespace coloring
} // namespace flecsi

#endif // flecsi_coloring_adjacency_types_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
