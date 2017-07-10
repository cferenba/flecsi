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

#ifndef flecsi_data_storage_type_h
#define flecsi_data_storage_type_h

///
/// \file
/// \date Initial file creation: Apr 15, 2016
///

#ifndef POLICY_NAMESPACE
  #error "You must define a data policy namespace before including this file."
#endif

#include "flecsi/data/data_constants.h"

namespace flecsi {
namespace data {
namespace POLICY_NAMESPACE {

  ///
  /// \struct storage_type__
  ///
  /// \tparam T Specialization parameter.
  /// \tparam ST Data store type.
  /// \tparam MD Metadata type.
  ///
  template<size_t STORAGE_TYPE>
  struct storage_type__ {};

} // namespace POLICY_NAMESPACE
} // namespace data
} // namespace flecsi

#endif // flecsi_data_storage_type_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
