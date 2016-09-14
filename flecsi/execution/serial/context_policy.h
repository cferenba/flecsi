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

#ifndef flecsi_execution_serial_context_policy_h
#define flecsi_execution_serial_context_policy_h

#include <unordered_map>
#include <functional>

#include "flecsi/utils/common.h"
#include "flecsi/utils/const_string.h"
#include "flecsi/execution/serial/runtime_driver.h"
#include "flecsi/execution/common/task_hash.h"

///
// \file serial/execution_policy.h
// \authors bergen
// \date Initial file creation: Nov 15, 2015
///

namespace flecsi {
namespace execution {

struct serial_context_policy_t
{
  //--------------------------------------------------------------------------//
  // Initialization.
  //--------------------------------------------------------------------------//

  ///
  // Initialize the context runtime. The arguments to this method should
  // be passed from the main function.
  //
  // \param argc The number of command-line arguments.
  // \param argv The array of command-line arguments.
  //
  // \return Zero upon clean initialization, non-zero otherwise.
  ///
  int
  initialize(
    int argc,
    char ** argv
  )
  {
    serial_runtime_driver(argc, argv);
    return 0;
  } // initialize

  //--------------------------------------------------------------------------//
  // Task registration.
  //--------------------------------------------------------------------------//

  using task_id_t = size_t;
  using register_function_t = std::function<void(size_t)>;
	using unique_fid_t = unique_id_t<task_id_t>;

  bool
  register_task(
    task_hash_key_t key,
    const register_function_t & f
  )
  {
    if(task_registry_.find(key) == task_registry_.end()) {
      task_registry_[key] = { unique_fid_t::instance().next(), f };
      return true;
    } // if

    return false;  
  } // register_task

  //--------------------------------------------------------------------------//
  // Function registration.
  //--------------------------------------------------------------------------//

  ///
  // \tparam T The type of the function being registered.
  //
  // \param key A unique function identifier.
  //
  // \return A boolean value that is true if the registration succeeded,
  //         false otherwise.
  ///
  template<typename T>
  bool
  register_function(
    const const_string_t & key,
    T & function
  )
  {
    const size_t h = key.hash();
    if(function_registry_.find(h) == function_registry_.end()) {
      function_registry_[h] =
        reinterpret_cast<std::function<void(void)> *>(&function);
      return true;
    } // if

    return false;
  } // register_function
  
  ///
  // Return the function assocaited with \e key.
  //
  // \param key The unique function identifier.
  //
  // \return A pointer to a std::function<void(void)> that may be cast
  //         back to the oringinal function type using reinterpret_cast.
  ///
  std::function<void(void)> *
  function(
    size_t key
  )
  {
    return function_registry_[key];
  } // function

private:

  //--------------------------------------------------------------------------//
  // Task registry
  //--------------------------------------------------------------------------//

  std::unordered_map<task_hash_t::key_t,
    std::pair<task_id_t, register_function_t>,
    task_hash_t> task_registry_;

  //--------------------------------------------------------------------------//
  // Function registry
  //--------------------------------------------------------------------------//

  std::unordered_map<size_t, std::function<void(void)> *>
    function_registry_;

}; // struct serial_context_policy_t

} // namespace execution 
} // namespace flecsi

#endif // flecsi_execution_serial_context_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
