/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_data_future_h
#define flecsi_data_future_h

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Jun 21, 2017
//----------------------------------------------------------------------------//

#include "flecsi/data/common/registration_wrapper.h"
#include "flecsi/runtime/flecsi_runtime_future_handle_policy.h"
#include "flecsi/data/storage.h"
#include "flecsi/utils/hash.h"

namespace flecsi {
namespace data {

//----------------------------------------------------------------------------//
//! FIXME: Description of class
//----------------------------------------------------------------------------//

template<
  typename FUTURE_POLICY
>
struct future_data__
{

  //--------------------------------------------------------------------------//
  //! Register a future with the FleCSI runtime. 
  //!
  //! @tparam DATA_TYPE        The data type, e.g., double. This may be
  //!                          P.O.D. or a user-defined type that is
  //!                          trivially-copyable.
  //! @tparam NAMESPACE_HASH   The namespace key. Namespaces allow separation
  //!                          of attribute names to avoid collisions.
  //! @tparam NAME_HASH        The attribute name.
  //! @tparam VERSIONS         The number of versions that shall be associated
  //!                          with this attribute.
  //!
  //! @param name The string version of the field name.
  //!
  //! @ingroup data
  //--------------------------------------------------------------------------//

  template<
    typename DATA_TYPE,
    size_t NAMESPACE_HASH,
    size_t NAME_HASH,
    size_t VERSIONS
  >
  static
  bool
  register_future(
    std::string const & name
  )
  {
    static_assert(VERSIONS <= utils::hash::future_max_versions,
      "max field versions exceeded");

    using wrapper_t = future_registration_wrapper__<
      DATA_TYPE,
      NAMESPACE_HASH,
      NAME_HASH,
      VERSIONS
    >;


    for(size_t version(0); version<VERSIONS; ++version) {
      const size_t key =
        utils::hash::field_hash<NAMESPACE_HASH, NAME_HASH>(version);

      if(!storage_t::instance().register_future( key,
        wrapper_t::register_callback)) {
        return false;
      } // if
    } // for

    return true;
  } // register_future

  //--------------------------------------------------------------------------//
  //! Return the future associated with the given parameters.
  //!
  //! @tparam DATA_TYPE        The data type, e.g., double. This may be
  //!                          P.O.D. or a user-defined type that is
  //!                          trivially-copyable.
  //! @tparam NAMESPACE_HASH   The namespace key. Namespaces allow separation
  //!                          of attribute names to avoid collisions.
  //! @tparam NAME_HASH        The attribute name.
  //! @tparam VERSION          The data version.
  //!
  //! @ingroup data
  //--------------------------------------------------------------------------//

  template<
    typename DATA_TYPE,
    size_t NAMESPACE_HASH,
    size_t NAME_HASH,
    size_t VERSION = 0
  >
  static
  decltype(auto)
  get_future()
  {
    static_assert(VERSION < utils::hash::field_max_versions,
      "max field version exceeded");

    const size_t key =
        utils::hash::field_hash<NAMESPACE_HASH, NAME_HASH>(VERSION);    
 
    future_handle_t<DATA_TYPE> f;

    auto& context = execution::context_t::instance();
 
    auto& future_info =  context.get_future_info(key);

    f.fid = future_info.fid;

std::cout <<"IRINA DEBUG inside get_future" <<std::endl;
  return f;
  } // get_future

}; // struct future_data__

} // namespace data
} // namespace flecsi

//----------------------------------------------------------------------------//
// This include file defines the FLECSI_RUNTIME_DATA_POLICY used below.
//----------------------------------------------------------------------------//

#include "flecsi/runtime/flecsi_runtime_future_handle_policy.h"

namespace flecsi {
namespace data {

using future_data_t = future_data__<FLECSI_RUNTIME_FUTURE_HANDLE_POLICY>;

} // namespace data
} // namespace flecsi

#endif // flecsi_data_future_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
