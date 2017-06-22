#pragma once

#include "penalizedGraphAlgorithm.h"
#include <system/traits.h>

#include <strategies/input/naiveBasicInput.h>

#include <strategies/abstractStrategy.h>
#include <strategies/impl/greedy/greedyAlgorithm.h>

#include <iostream>

class GreedyPenalizedGraphStrategy : public AbstractStrategy<NaiveBasicInput> {

public:
  typedef PenalizedGraphAlgorithm<> PGAlgorithm;
  typedef PenalizedGraphAlgorithmTraits::Graph Graph;
  typedef Graph::Vertex Vertex;
  typedef NaiveBasicInput::Id Id;


  /**
   * The penalized graph algorithm that will be used in the strategy.
   */
  PGAlgorithm penalizedGraphAlgorithm;

  /**
   * Auxiliary PE structure to wrap the graph, adding the mapTask functionallity needed by GreedyAlgorithm.
   */
  struct PE {
    Graph graph;
    Id id;
    PGAlgorithm * pgAlgorithm;

    PE(const Id &anId, PGAlgorithm *pgAlgorithmRef) : id(anId), pgAlgorithm(pgAlgorithmRef) {
      graph = Graph(id);
    }

    void mapTask(const Vertex &task) {
      graph.addVertex(task);
      graph.setWeight(pgAlgorithm->weightIncrementalGainAVertex(graph, task.weight()));
    }

    /**
     * Proxy function to get the vertices of the graph, which are the tasks of this PE wrapper structure.
     */
    inline Vertex * tasks() {
      return graph.vertices();
    }

    /**
     * Proxy function to get the vertices count of the graph, which are the tasks count of this PE wrapper structure.
     */
    inline const int taskCount() {
      return graph.verticesSize();
    }

    const bool operator>(const PE &o) const {
      return graph > o.graph;
    }
  };
  
  typedef GreedyStrategyAlgorithm<Vertex,PE> GreedyAlgorithm;
  typedef GreedyAlgorithm::MaxHeap MaxHeap;
  typedef GreedyAlgorithm::MinHeap MinHeap;

  /**
   * The greedy algorithm that will be used in the strategy.
   */
  GreedyAlgorithm greedyAlgorithm;

  /**
   * Method to create this strategy with a said penality function
   */
  GreedyPenalizedGraphStrategy(const PGAlgorithm::PenalityFunction &penalityFunction) : penalizedGraphAlgorithm(PenalizedGraphAlgorithmTraits::zeroRef, penalityFunction) {}

  /**
   * The strategy specific code for every strategy implementation. This method must be implemented for each strategy and inside it's code it must modify the lbOutput variable.
   * @param input The Load Balancer's input
   */
  virtual void doTaskMapping(const NaiveBasicInput &input) {
    MaxHeap tasks;
    MinHeap PEs;

    for(auto id : input.getPEsIds()) {
      PEs.push(PE(id, &penalizedGraphAlgorithm));
    }

    for(auto taskId : input.getTasksIds()) {
      tasks.push(Vertex(taskId, input.getTaskLoad(taskId)));
    }

    greedyAlgorithm.map(tasks, PEs);
    populateOutput(PEs);
  }

protected:

  /**
   * This method is called to populate the output variable lbOutput.
   * @param PEs The heap of PEs modified to contain the tasks by the greedy algorithm.
   */
  void populateOutput(MinHeap &PEs) {
    while(!PEs.empty()) {
      auto _PE = PEs.top();
      auto tasks = _PE.tasks();

      for(auto i = 0; i < _PE.taskCount(); ++i) {
        lbOutput.set(tasks[i].id, _PE.id);
      }

      std::cout << "PE " << _PE.id << " load: " << _PE.graph.weight() << ". Task count: " << _PE.taskCount() << std::endl;
      PEs.pop();
    }
  }

};