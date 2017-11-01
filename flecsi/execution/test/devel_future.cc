/*~-------------------------------------------------------------------------~~*
 * Copyright (c) 2014 Los Alamos National Security, LLC
 * All rights reserved.
 *~-------------------------------------------------------------------------~~*/

#include <cinchdevel.h>

#include "flecsi/execution/context.h"
#include "flecsi/execution/execution.h"

clog_register_tag(devel_future);

namespace flecsi {
namespace execution {


using future_t=

//----------------------------------------------------------------------------//
// task that returns future
//----------------------------------------------------------------------------//

double task1(double a) {
  return 3.14;
} // initialize_pressure

flecsi_register_task(task1, loc, single);

//----------------------------------------------------------------------------//
// Task that gets future as an argument
//----------------------------------------------------------------------------//

void task2(future_t f) {
  std::cout <<f<<std::endl;
} // initialize_pressure

flecsi_register_task(task2, loc, single);

//----------------------------------------------------------------------------//
// Top-Level Specialization Initialization
//----------------------------------------------------------------------------//

void specialization_tlt_init(int argc, char ** argv) {

  {
  clog_tag_guard(devel_handle);
  clog(info) << "specialization_tlt_init function" << std::endl;
  } // scope


} // specialization_tlt_init

//----------------------------------------------------------------------------//
// SPMD Specialization Initialization
//----------------------------------------------------------------------------//

void specialization_spmd_init(int argc, char ** argv) {

  {
  clog_tag_guard(devel_handle);
  clog(info) << "specialization_spmd_init function" << std::endl;
  } // scope
}
//----------------------------------------------------------------------------//
// User driver.
//----------------------------------------------------------------------------//

void driver(int argc, char ** argv) {

  auto future = flecsi_execute_task(task1, single, 0.0);
  flecsi_execute_task(task2, future);

} // driver

} // namespace execution
} // namespace flecsi

DEVEL(devel_handle) {}

/*~------------------------------------------------------------------------~--*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~------------------------------------------------------------------------~--*/
