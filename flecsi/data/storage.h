/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_data_storage_h
#define flecsi_data_storage_h

#include <cinchlog.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "flecsi/runtime/types.h"
#include "flecsi/utils/common.h"
#include "flecsi/data/future_handle.h"

//----------------------------------------------------------------------------//
//! @file
//! @date Initial file creation: Apr 17, 2016
//----------------------------------------------------------------------------//

namespace flecsi {
namespace data {

//----------------------------------------------------------------------------//
//! The storage__ type provides a high-level data model context interface that
//! is implemented by the given storage policy.
//!
//! @tparam USER_META_DATA A user-defined meta data type.
//! @tparam STORAGE_POLICY The backend storage policy.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

template<
  typename STORAGE_POLICY
>
struct storage__ : public STORAGE_POLICY {

  //--------------------------------------------------------------------------//
  // Public type definitions.
  //--------------------------------------------------------------------------//

  using registration_function_t = std::function<void(size_t)>;
  using field_registration_function_t = std::function<void(size_t, size_t)>;
  using future_registration_function_t = std::function<void(size_t, size_t)>;
  using unique_fid_t = utils::unique_id_t<field_id_t, FLECSI_GENERATED_ID_MAX>;
  using field_value_t = std::pair<field_id_t, field_registration_function_t>;
  using client_value_t = std::pair<field_id_t, registration_function_t>;
  using future_value_t = std::pair<future_id_t,
        future_registration_function_t>;

  using field_entry_t = std::unordered_map<size_t, field_value_t>;
  using client_entry_t = std::unordered_map<size_t, client_value_t>;

  //--------------------------------------------------------------------------//
  //! Register a field with the runtime.
  //!
  //! @param client_key The data client indentifier hash.
  //! @param key        The identifier hash.
  //! @param callback   The registration call back function.
  //--------------------------------------------------------------------------//

  bool
  register_field(
    size_t client_key,
    size_t key,
    const field_registration_function_t & callback
  )
  {
    if(field_registry_.find(client_key) != field_registry_.end()) {
      if(field_registry_[client_key].find(key) !=
         field_registry_[client_key].end()) {
        clog(warn) << "field key already exists" << std::endl;
      } // if
    } // if

    field_registry_[client_key][key] =
      std::make_pair(unique_fid_t::instance().next(), callback);

    return true;
  } // register_field

  auto const &
  field_registry()
  const
  {
    return field_registry_;
  } // field_registry

  auto const &
  client_registry()
  const
  {
    return client_registry_;
  } // client_registry

////////////////////////////////////////////////////////////////////////////////

  //--------------------------------------------------------------------------//
  //! Register a client with the runtime.
  //!
  //! @param client_key The data client indentifier hash.
  //! @param key        The identifier hash.
  //! @param callback   The registration call back function.
  //--------------------------------------------------------------------------//

  bool
  register_client(
    size_t client_key,
    size_t key,
    const registration_function_t & callback
  )
  {
    if(client_registry_.find(client_key) != client_registry_.end()) {
      clog_assert(client_registry_[client_key].find(key) ==
                  client_registry_[client_key].end(),
                  "client key already exists");
    } // if

    client_registry_[client_key][key] =
      std::make_pair(unique_fid_t::instance().next(), callback);

    return true;
  } // register_client

  bool
  register_client_fields(size_t client_key)
  {
    return registered_client_fields_.insert(client_key).second;
  }

  //--------------------------------------------------------------------------//
  //! Register a future with the runtime.
  //!
  //! @param client_key The data client indentifier hash.
  //! @param key        The identifier hash.
  //! @param callback   The registration call back function.
  //--------------------------------------------------------------------------//

  bool
  register_future(
    size_t key,
    const future_registration_function_t & callback
  )
  {
    if(future_registry_.find(key) != future_registry_.end()) {
        clog(warn) << "future key already exists" << std::endl;
    } // if

    future_registry_[key] =
      std::make_pair(unique_fid_t::instance().next(), callback);

    return true;
  } // register_futured

  auto const &
  future_registry()
  const
  {
    return future_registry_;
  } // field_registry


////////////////////////////////////////////////////////////////////////////////

	//--------------------------------------------------------------------------//
  //! Myer's singleton instance.
  //!
  //! @return The single instance of this type.
	//--------------------------------------------------------------------------//

  static storage__ &
  instance()
  {
    static storage__ d;
    return d;
  } // instance

	//--------------------------------------------------------------------------//
  // FIXME: What are these for?
	//--------------------------------------------------------------------------//

  void
  move(
    uintptr_t from,
    uintptr_t to
  )
  {}

  void
  reset(
    uintptr_t runtime_namespace
  )
  {}

private:

  // Default constructor
  storage__() : STORAGE_POLICY() {}

  // Destructor
  ~storage__() {}

  // We don't need any of these
  storage__(const storage__ &) = delete;
  storage__ & operator = (const storage__ &) = delete;
  storage__(storage__ &&) = delete;
  storage__ & operator = (storage__ &&) = delete;

  std::unordered_set<size_t> registered_client_fields_;
  std::unordered_map<size_t, field_entry_t> field_registry_;
  std::unordered_map<size_t, client_entry_t> client_registry_;
  std::unordered_map<size_t, future_value_t> future_registry_;
}; // class storage__

} // namespace data
} // namespace flecsi

#include "flecsi/runtime/flecsi_runtime_storage_policy.h"

namespace flecsi {
namespace data {

using storage_t = storage__<FLECSI_RUNTIME_STORAGE_POLICY>;

} // namespace data
} // namespace flecsi

#endif // flecsi_data_storage_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
