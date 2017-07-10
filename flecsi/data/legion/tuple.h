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

#ifndef flecsi_legion_tuple_h
#define flecsi_legion_tuple_h

//----------------------------------------------------------------------------//
// POLICY_NAMESPACE must be defined before including storage_type.h!!!
// Using this approach allows us to have only one storage_type__
// definintion that can be used by all data policies -> code reuse...
#define POLICY_NAMESPACE legion
#include "flecsi/data/storage_type.h"
#undef POLICY_NAMESPACE
//----------------------------------------------------------------------------//

#include "flecsi/utils/const_string.h"

///
// \file legion/tuple.h
// \authors bergen
// \date Initial file creation: Apr 17, 2016
///

namespace flecsi {
namespace data {
namespace legion {

  ///
  // FIXME: Tuple storage type.
  ///
  template<>
  struct storage_type__<tuple> {

    struct tuple_handle_t {
    }; // struct tuple_handle_t

    ///
    //
    ///
    template<
      typename T,
      size_t NS
    >
    static
    tuple_handle_t
    get_handle(
      uintptr_t runtime_namespace,
      const utils::const_string_t & key
    )
    {
      return {};
    } // get_handle

  }; // struct storage_type__

} // namespace legion
} // namespace data
} // namespace flecsi

#endif // flecsi_legion_tuple_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
