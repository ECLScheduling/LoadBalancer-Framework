#pragma once

@RTS_INCLUDES@
@SCHED_INCLUDES@
@CONCEPT_INCLUDES@

namespace MOGSLib {

template<typename Tuple, unsigned Index>
struct ConceptInitializer {
  static void init(Tuple &t) {
    auto ref = std::get<Index>(t);
    ref = new typename std::decay<typename std::tuple_element<Index, Tuple>::type>::type();

    ref->init();
    ConceptInitializer<Tuple, Index-1>::init(t);
  }

  static void clean(Tuple &t) {
    delete std::get<Index>(t);
    ConceptInitializer<Tuple, Index-1>::init(t); 
  }
};

template<typename Tuple>
struct ConceptInitializer<Tuple, 1> {
  static void init(Tuple &t) {
    auto ref = std::get<0>(t);
    ref = new typename std::decay<typename std::tuple_element<Index, Tuple>::type>::type();

    ref->init();
  }

  static void clean(Tuple &t) {
    delete std::get<Index>(t);
  }
};

/**
 * @brief This structure assembles MOGSLibs components into a Scheduler collection that can be use within a selected RTS.
 * @details These definitions are generated automatically when executing scripts/configure.py with proper parameters
 */
struct SchedulerCollection {
  using RTS = MOGSLib::Abstraction::RTS<MOGSLib::TargetSystem>;

  template<template<typename ... Concepts> typename Sched, typename ... Concepts>
  struct CompleteScheduler {
    using Scheduler = Sched<Concepts...>;
    using ConceptTuple = std::tuple<Concepts *...>

    std::unique_pointer<Scheduler> scheduler;
    std::unique_pointer<ConceptTuple> concepts;

    TaskMap init_and_work() {
      ConceptInitializer<ConceptTuple, std::tuple_elements<ConceptTuple>::value>::init(concepts);
      sched.init(concepts);

      return sched.work();
    }

    TaskMap work() {
      return sched.work();
    }

    void clean() {
      ConceptInitializer<ConceptTuple, std::tuple_elements<ConceptTuple>::value>::clean(concepts);
    }
  };

  using SchedulerTuple = std::tuple<$SCHEDULER_TUPLE$>;
  using ConceptTuple = std::tuple<$CONCEPT_TUPLE$>

  SchedulerTuple schedulers;
  ConceptTuple concepts;

  void TaskMap schedule(std:string &scheduler_name) {
$SCHEDULE_SNIPPET$
    return nullptr;
  }
};

}