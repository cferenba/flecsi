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

#ifndef flecsi_execution_legion_future_h
#define flecsi_execution_legion_future_h

//----------------------------------------------------------------------------//
//! \file
//! \date Initial file creation: Nov 15, 2015
//----------------------------------------------------------------------------//

#include <functional>
#include <legion.h>
#include <memory>

namespace flecsi {
namespace execution {

struct future_base_t{
  public:
//    virtual ~future_base_t() = 0;
  virtual void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)=0;

  virtual void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)=0;

};

//----------------------------------------------------------------------------//
// Future concept.
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//! Abstract interface type for Legion futures.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<
  typename RETURN
>
struct legion_future_concept__ : public future_base_t
{

  virtual ~legion_future_concept__() {}

  //--------------------------------------------------------------------------//
  //! Abstract interface to wait on a task result.
  //--------------------------------------------------------------------------//

  virtual void wait(bool silence_warnings = false) = 0;

  //--------------------------------------------------------------------------//
  //! Abstract interface to get a task result.
  //--------------------------------------------------------------------------//

  virtual RETURN get(size_t index = 0, bool silence_warnings = false) = 0;

  //--------------------------------------------------------------------------//
  //! Abstract interface for reduction step.
  //--------------------------------------------------------------------------//

  virtual void
  defer_dynamic_collective_arrival(
    Legion::Runtime* runtime,
    Legion::Context ctx,
    Legion::DynamicCollective& dc_reduction) = 0;

  //-------------------------------------------------------------------------//
  //! Add Legion::Future to the task
  //-------------------------------------------------------------------------//
  virtual void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)=0;

  virtual void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)=0;

}; // struct legion_future_concept__

//----------------------------------------------------------------------------//
//! Explicit specialization for void.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<>
struct legion_future_concept__<void>
{

  virtual ~legion_future_concept__() {}

  //--------------------------------------------------------------------------//
  //! Abstract interface to wait on a task result.
  //--------------------------------------------------------------------------//

  virtual void wait(bool silence_warnings = false) = 0;

}; // struct legion_future_concept__

//----------------------------------------------------------------------------//
// Future model.
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//! Base future model type.
//!
//! @tparam RETURN The return type of the task.
//! @tparam FUTURE The Legion runtime future type.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<
  typename RETURN,
  typename FUTURE
>
struct legion_future_model__ : public legion_future_concept__<RETURN>
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future.
  //!
  //! @param legion_future The Legion future instance.
  //--------------------------------------------------------------------------//

  legion_future_model__(const FUTURE & legion_future)
    : legion_future_(legion_future) {}

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void
  wait(bool silence_warnings = false)
  {
    legion_future_.wait();
  } // wait

  //--------------------------------------------------------------------------//
  //! Get a task result.
  //!
  //! @param index The index of the task.
  //!
  //! @remark This method only applies to indexed task invocations.
  //--------------------------------------------------------------------------//

  RETURN
  get(
    size_t index = 0,
    bool silence_warnings = false
  )
  {
    return legion_future_.template get_result<RETURN>(silence_warnings);
  } // get

  void
  defer_dynamic_collective_arrival(
    Legion::Runtime* runtime,
    Legion::Context ctx,
    Legion::DynamicCollective& dc_reduction)
  {
    runtime->defer_dynamic_collective_arrival(ctx, dc_reduction,
      legion_future_);
  } // defer_dynamic_collective_arrival

  //-------------------------------------------------------------------------//
  //! Add Legion Future to the task launcher
  //------------------------------------------------------------------------//
  void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)
  {
std::cout<<"IRINA DEBUG inside add_future"<<std::endl;
    launcher.add_future(legion_future_);
  }

  void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)
  {
    launcher.add_future(legion_future_);
  }


private:

  FUTURE legion_future_;

}; // struct legion_future_model__

//----------------------------------------------------------------------------//
//! Partial specialization for void.
//!
//! @tparam FUTURE The Legion runtime future type.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<typename FUTURE>
struct legion_future_model__<void, FUTURE>
  : public legion_future_concept__<void>
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future.
  //!
  //! @param legion_future The Legion future instance.
  //--------------------------------------------------------------------------//

  legion_future_model__(const FUTURE & legion_future)
    : legion_future_(legion_future) {}

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void
  wait(bool silence_warnings = false)
  {
    legion_future_.get_void_result(silence_warnings);
  } // wait

  void
  defer_dynamic_collective_arrival(
    Legion::Runtime* runtime,
    Legion::Context ctx,
    Legion::DynamicCollective& dc_reduction)
  {
    // reduction of a void is still void
  }

  void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)
  {
    launcher.add_future(legion_future_);
  }

  void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)
  {
    launcher.add_future(legion_future_);
  }

private:

  FUTURE legion_future_;

}; // struct legion_future_model__

//----------------------------------------------------------------------------//
//! Partial specialization for index launch FutureMap.
//----------------------------------------------------------------------------//

template<typename RETURN>
struct legion_future_model__<RETURN, Legion::FutureMap>
  : public legion_future_concept__<RETURN>
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future map.
  //!
  //! @param legion_future The Legion future instance.
  //--------------------------------------------------------------------------//

  legion_future_model__(
    const Legion::FutureMap & legion_future
  )
    : legion_future_(legion_future) {}

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void
  wait(bool silence_warnings = false)
  {
    legion_future_.wait_all_results(silence_warnings);
  } // wait

  //--------------------------------------------------------------------------//
  //! Get a task result.
  //!
  //! @param index The index of the task.
  //!
  //! @remark This method only applies to indexed task invocations.
  //--------------------------------------------------------------------------//

  RETURN
  get(
    size_t index = 0,
    bool silence_warnings = false
  )
  {
    return legion_future_.get_result<RETURN>(
      Legion::DomainPoint::from_point<1>(
        LegionRuntime::Arrays::Point<1>(index)
      ),
      silence_warnings
    );
  } // get

  void
  defer_dynamic_collective_arrival(
    Legion::Runtime* runtime,
    Legion::Context ctx,
    Legion::DynamicCollective& dc_reduction)
  {
    // Not sure what reducing a map with other maps would mean
  }

  void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)
  {
    assert( false &&"you can't pass future handle from index task to any task");
  }

  void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)
  {
    assert( false &&"you can't pass future handle from index task to any task");
  }


private:

  Legion::FutureMap legion_future_;

}; // struct legion_future_model__

//----------------------------------------------------------------------------//
//! Explicit specialization for index launch FutureMap and void.
//----------------------------------------------------------------------------//

template<>
struct legion_future_model__<void, Legion::FutureMap>
  : public legion_future_concept__<void>
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future map.
  //!
  //! @param legion_future The Legion future instance.
  //--------------------------------------------------------------------------//

  legion_future_model__(
    const Legion::FutureMap & legion_future
  )
    : legion_future_(legion_future) {}

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void
  wait(bool silence_warnings = false)
  {
    legion_future_.wait_all_results(silence_warnings);
  } // wait

  void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)
  {
    assert( false &&"you can't pass future handle from index task to any task");
  }

  void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)
  {
    assert( false &&"you can't pass future handle from index task to any task");
  }


private:

  Legion::FutureMap legion_future_;

}; // struct legion_future_model__

//----------------------------------------------------------------------------//
// Future.
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//! Legion future type.
//!
//! @tparam RETURN The return type of the task.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<
  typename RETURN
>
struct legion_future__
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future map.
  //!
  //! @tparam FUTURE
  //!
  //! @param future
  //--------------------------------------------------------------------------//

  template<
    typename FUTURE
  >
  legion_future__(
    const FUTURE & future
  )
  :
    state_(new legion_future_model__<RETURN, FUTURE>(future))
  {
  } // legion_future__

  //--------------------------------------------------------------------------//
  //! Copy constructor.
  //!
  //! @param lf The legion_future__ to use to set our state.
  //--------------------------------------------------------------------------//

  legion_future__(const legion_future__ & lf)
    : state_(lf.state_) {}

  //--------------------------------------------------------------------------//
  //! Assignment operator.
  //!
  //! @param lf The legion_future__ to use to set our state.
  //--------------------------------------------------------------------------//

  legion_future__ &
  operator = (
    const legion_future__ & lf
  )
  {
    state_ = lf.state_;
  } // operator =

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void
  wait(bool silence_warnings=false)
  {
    state_->wait(silence_warnings);
  } // wait

  //--------------------------------------------------------------------------//
  //! Get a task result.
  //!
  //! @param index The index of the task.
  //!
  //! @remark This method only applies to indexed task invocations.
  //--------------------------------------------------------------------------//

  RETURN
  get(
    size_t index = 0,
    bool silence_warnings = false
  )
  {
    return state_->get(index, silence_warnings);
  } // get

  void
  defer_dynamic_collective_arrival(
    Legion::Runtime* runtime,
    Legion::Context ctx,
    Legion::DynamicCollective& dc_reduction)
  {
    state_->defer_dynamic_collective_arrival(runtime, ctx, dc_reduction);
  } // defer_dynamic_collective_arrival

  void
  add_future_to_single_task_launcher(
    Legion::TaskLauncher& launcher)
  {
    state_->add_future_to_single_task_launcher(launcher);
  }

  void
  add_future_to_index_task_launcher(
    Legion::IndexLauncher& launcher)
  {
    state_->add_future_to_index_task_launcher(launcher);
  }


private:

  // Needed to satisfy static check.
  void set() {}

  std::shared_ptr<legion_future_concept__<RETURN>> state_;

}; // struct legion_future__

//----------------------------------------------------------------------------//
//! Legion future type. Explicit specialization for void.
//!
//! @ingroup legion-execution
//----------------------------------------------------------------------------//

template<>
struct legion_future__<void>
{

  //--------------------------------------------------------------------------//
  //! Construct a future from a Legion future map.
  //!
  //! @tparam FUTURE
  //!
  //! @param future
  //--------------------------------------------------------------------------//

  template<typename FUTURE>
  legion_future__(const FUTURE & future)
    : state_(new legion_future_model__<void, FUTURE>(future)) {}

  //--------------------------------------------------------------------------//
  //! Wait on a task result.
  //--------------------------------------------------------------------------//

  void wait(bool silence_warnings=false)
  {
    state_->wait(silence_warnings);
  } // wait

  std::shared_ptr<legion_future_concept__<void>> state_;

}; // struct legion_future__

} // namespace execution 
} // namespace flecsi

#endif // flecsi_execution_legion_future_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
