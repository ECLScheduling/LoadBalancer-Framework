#pragma once

#include <abstractions/bind.h>

#include <schedulers/compact.h>

#include <system/static.h>

BEGIN_NAMESPACE(Abstraction)

/**
 * @brief This Binder specialization binds the concrete adapters T and P, required by the Compact scheduler, to their respective concepts, TaskData and PEData.
 */
template<typename T, typename P>
struct Binder<MOGSLib::Scheduler::Compact<T,P> > {
  static void bind(T* t, P *p) {
    MOGSLib::Scheduler::Compact<T,P>::TaskData::concrete = t;
    MOGSLib::Scheduler::Compact<T,P>::PEData::concrete = p;
  }
};

END_NAMESPACE