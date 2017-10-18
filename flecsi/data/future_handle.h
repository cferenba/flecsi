/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_future_handle_h
#define flecsi_future_handle_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Aug 01, 2016
//----------------------------------------------------------------------------//

namespace flecsi {
namespace data {

//----------------------------------------------------------------------------//
//! The future_handle_base_t type provides an empty base type for compile-time
//! identification of future handle objects.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

struct future_handle_base_t {};

//----------------------------------------------------------------------------//
//! The future_handle_base__ type captures information about permissions
//! and specifies a data policy.
//!
//! @tparam T                     The data type referenced by the handle.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

template<
  typename T,
  typename FUTURE_POLICY
>
struct future_handle_base__ : public FUTURE_POLICY, public future_handle_base_t {

  //--------------------------------------------------------------------------//
  //! Default constructor.
  //--------------------------------------------------------------------------//

  future_handle_base__()
  {}

  //--------------------------------------------------------------------------//
  //! Copy constructor.
  //--------------------------------------------------------------------------//

  future_handle_base__(const future_handle_base__& b)
  : FUTURE_POLICY(b)
  {
    future=b.future;
  }
   using typename FUTURE_POLICY::future_type__; 
   future_type__ future;
   future_id_t fid;  

};

} // namespace data
} // namespace flecsi

#include "flecsi/runtime/flecsi_runtime_future_handle_policy.h"

namespace flecsi {
  
//----------------------------------------------------------------------------//
//! The data_handle__ type is the high-level data handle type.
//!
//! @tparam T                     The data type referenced by the handle.
//! @tparam FUTURE_POLICY         The future policy for this handle type.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

template<
  typename T
>
using future_handle__ = data::future_handle_base__<
  T,
  FLECSI_RUNTIME_FUTURE_HANDLE_POLICY
>;

template<
  typename T
>
using future_handle_t = future_handle__<T>;

} // namespace flecsi

#endif // flecsi_future_handle_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
