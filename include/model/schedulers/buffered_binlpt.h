#pragma once

#include <model/schedulers/binlpt.h>
namespace MOGSLib { namespace Scheduler {

/**
 *  @class BufferedBinLPT
 *  @tparam Ctx The context where the scheduler will be applied to.
 *  @brief A scheduler which keeps the schedule saved in a buffer to avoid calculating it multiple times.
 **/
template<typename C>
class BufferedBinLPT {
public:
  using Ctx = C;
  using Id = typename Ctx::Id;
  using Load = typename Ctx::Load;
  using Scheduler = BinLPT<Ctx>;
  using Schedule = typename Scheduler::Schedule;
  
  Scheduler scheduler;

  /// @brief The method to obtain a schedule based on a binlpt policy.
  void work(Ctx &ctx) {
    if(ctx.recalculate())
      scheduler.work(ctx);
  }

};

}}