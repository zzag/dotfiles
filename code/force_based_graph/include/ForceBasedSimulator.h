#ifndef FORCE_BASED_SIMULATOR_H
#define FORCE_BASED_SIMULATOR_H

#include "Graph.h"


class ForceBasedSimulator
{
public:
  ForceBasedSimulator(Graph *graph,
                      float repulsiveForce = 200.0f,
                      float attractiveForce = 0.05f,
                      float convergeRatio = 0.7f);

  void update(int maxWidth, int maxHeight);
  Graph *getGraph() { return m_graph; }

private:
  Graph *m_graph;
  float m_repulsiveForce;
  float m_attractiveForce;
  float m_convergenceRatio;
};

#endif /* FORCE_BASED_SIMULATOR_H */