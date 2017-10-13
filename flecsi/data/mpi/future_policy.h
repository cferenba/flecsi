/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_data_mpi_future_policy_h
#define flecsi_data_mpi_future_policy_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Jun 21, 2017
//----------------------------------------------------------------------------//

#include "flecsi/execution/legion/future.h "

namespace flecsi {
namespace data {

//----------------------------------------------------------------------------//
//! The legion future policy defines types that are specific to the Legion
//! runtime.
//----------------------------------------------------------------------------//

struct legion_future_policy_t
{

  //--------------------------------------------------------------------------//
  //! The storage_type__ type determines the underlying storage mechanism
  //! for the backend runtime.
  //--------------------------------------------------------------------------//

  template<
    typename T
  >
  using future_type__ = T;

}; // class legion_data_policy_t

} // namespace data
} // namespace flecsi

#endif // flecsi_data_legion_future_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
