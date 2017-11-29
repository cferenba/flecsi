/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_runtime_future_handle_policy_h
#define flecsi_runtime_future_handle_policy_h

//----------------------------------------------------------------------------//
// @file
// @date Initial file creation: Aug 01, 2016
//----------------------------------------------------------------------------//

#include <flecsi.h>

//----------------------------------------------------------------------------//
// This section works with the build system to select the correct runtime
// implemenation for the task model. If you add to the possible runtimes,
// remember to edit config/packages.cmake to include a definition using
// the same convention, e.g., -DFLECSI_RUNTIME_MODEL_new_runtime.
//----------------------------------------------------------------------------//

// Serial Policy
#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_serial

  #include "flecsi/data/serial/future_policy.h"

  namespace flecsi {

  using FLECSI_RUNTIME_FUTURE_HANDLE_POLICY = serial_future_handle_policy_t;

  }

// Legion, MPI+Legion Policy
#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_legion

  #include "flecsi/data/legion/future_policy.h"
  //#include "flecsi/execution/legion/future.h"

  namespace flecsi {

  using FLECSI_RUNTIME_FUTURE_HANDLE_POLICY =
      data::legion_future_handle_policy_t;

  template <typename T>
  using FLECSI_FUTURE_TYPE=flecsi::execution::legion_future__<T>; 
//    template <typename T>
//    using FLECSI_FUTURE_TYPE=flecsi::execution::future_base_t; 

  }

// MPI Policy
#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpi

  #include "flecsi/data/mpi/future_policy.h"

  namespace flecsi {

  using FLECSI_RUNTIME_FUTURE_HANDLE_POLICY = mpi_future_handle_policy_t;
  
  }

#endif // FLECSI_RUNTIME_MODEL

#endif // flecsi_runtime_future_handle_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
