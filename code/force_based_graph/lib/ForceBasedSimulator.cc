#include "ForceBasedSimulator.h"
#include <cmath>


ForceBasedSimulator::ForceBasedSimulator(Graph *graph,
                      float repulsiveForce,
                      float attractiveForce,
                      float convergeRatio) :
    m_graph(graph),
    m_repulsiveForce(repulsiveForce),
    m_attractiveForce(attractiveForce),
    m_convergenceRatio(convergeRatio)
{
}


void
ForceBasedSimulator::update(int maxWidth, int maxHeight)
{
  int n = m_graph->size();

  for (int i = 0; i < n; i++)
  {
    float repulsiveX = 0, repulsiveY = 0;
    float attractiveX = 0, attractiveY = 0;

    std::vector<int> &adjacency = m_graph->adjacency(i);
    Vertex &u = m_graph->node(i);

    // repulsive force
    for (int j = 0; j < n; j++)
    {
      if (i == j) continue;

      Vertex &v = m_graph->node(j);
      float dx = u.posX - v.posX,
            dy = u.posY - v.posY;
      float norm = sqrt(dx*dx + dy*dy);
      if (norm > 0)
      {
        repulsiveX += m_repulsiveForce * dx / norm;
        repulsiveY += m_repulsiveForce * dy / norm;
      }
    }

    // attractive force
    for (int j : adjacency)
    {
      Vertex &v = m_graph->node(j);
      attractiveX += m_attractiveForce * (v.posX - u.posX);
      attractiveY += m_attractiveForce * (v.posY - u.posY);
    }

    // update velocity
    u.velX = (u.velX + repulsiveX + attractiveX) * m_convergenceRatio;
    u.velY = (u.velY + repulsiveY + attractiveY) * m_convergenceRatio;
  }

  for (int i = 0; i < n; i++)
  {
    Vertex &v = m_graph->node(i);
    v.posX += v.velX;
    v.posY += v.velY;
    // if (v.posX < 0) v.posX = 0;
    // else if (v.posX >= maxWidth) v.posX = maxWidth - 1;
    // if (v.posY < 0) v.posY = 0;
    // else if (v.posY >= maxHeight) v.posY = maxHeight - 1;
  }
}