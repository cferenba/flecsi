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

#ifndef flecsi_execution_future_h
#define flecsi_execution_future_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Oct 19, 2015
//----------------------------------------------------------------------------//


#include "cinchlog.h"
#include "flecsi/runtime/types.h"

clog_register_tag(future);

namespace flecsi {
namespace execution {

//----------------------------------------------------------------------------//
//! The future__ type provides a high-level runtime future interface that
//! is implemented by the given future policy.
//!
//! @tparam FUTURE_POLICY The backend future policy.
//!
//! @ingroup execution
//----------------------------------------------------------------------------//

template<typename T, class FUTURE_POLICY>
struct future__ : public FUTURE_POLICY<T>
{

  // Default constructor
  future__() : FUTURE_POLICY() {}

  // Destructor
  ~future__() {}

  //copy constructor
  future__(const future__ &f)
  {
   future_ = f.future_;
  }

  // operator =
  future__ & operator = (const future__ &f)
  {
   future_ = f.future_;
  //FIXME
  }

  future__(future__ &&) = delete;
  future__ & operator = (future__ &&) = delete;

  private

  FUTURE_POLICY::future_type future_;

}; // class future__

} // namespace execution
} // namespace flecsi

//----------------------------------------------------------------------------//
// This include file defines the FLECSI_RUNTIME_FUTURE_POLICY used below.
//----------------------------------------------------------------------------//

#include "flecsi/runtime/flecsi_runtime_future_policy.h"

namespace flecsi {
namespace execution {

//----------------------------------------------------------------------------//
//! The future_t type is the high-level interface to the FleCSI runtime
//! context.
//!
//! @ingroup execution
//----------------------------------------------------------------------------//

template<typename T>
using future_t = future__< T, FLECSI_RUNTIME_FUTURE_POLICY >;

} // namespace execution
} // namespace flecsi

#endif // flecsi_execution_future_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
