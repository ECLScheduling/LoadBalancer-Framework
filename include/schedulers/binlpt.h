#pragma once

#include <policies/binlpt.h>

#include <iostream>

namespace MOGSLib { namespace Scheduler {

/**
 *  @class BinLPT
 *  @tparam Ctx The context where the scheduler will be applied to.
 *  @brief A scheduler which utilizes the binlpt policy to output a workload-aware task map.
 **/
template<typename C>
class BinLPT {
public:
  using Ctx = C;
  using Id = typename Ctx::Id;
  using Load = typename Ctx::Load;
  using Policy = MOGSLib::Policy::BinLPT<MOGSLib::Dependency::WorkloadAware<Id,Load>>;
  using Schedule = typename Policy::Schedule;
  
  int i = 0;

  /// @brief The method to obtain a schedule based on a binlpt policy.
  auto work(Ctx &ctx) {
    auto &data = ctx.input();
    auto chunks = ctx.k();
    auto schedule = MOGSLib::Output<Schedule>::alloc(data.ntasks());

    Policy::map(schedule, data.task_workloads(), data.pu_workloads(), chunks);
    
    return schedule;
  }

};

}}