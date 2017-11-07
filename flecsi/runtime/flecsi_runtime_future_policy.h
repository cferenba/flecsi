/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_runtime_future_policy_h
#define flecsi_runtime_future_policy_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Aug 01, 2016
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

  #include "flecsi/execution/serial/future.h"

  namespace flecsi {
  namespace execution {

  template<typename RETURN>
  using FLECSI_RUNTIME_FUTURE_TYPE = serial_future__<RETURN>;

  } // namespace execution
  } // namespace flecsi

// Legion Policy
#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_legion
  #include "flecsi/execution/legion/future.h"

  namespace flecsi {
  namespace execution {

  template<typename RETURN>
  using FLECSI_RUNTIME_FUTURE_TYPE = legion_future__<RETURN>;

  } // namespace execution
  } // namespace flecsi

// MPI Policy
#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpi

  #include "flecsi/execution/mpi/future.h"

  namespace flecsi {
  namespace execution {

  template<typename RETURN>
  using FLECSI_RUNTIME_FUTURE_TYPE = mpi_future__<RETURN>;

  } // namespace execution
  } // namespace flecsi

#endif // FLECSI_RUNTIME_MODEL

#endif // flecsi_runtime_future_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
