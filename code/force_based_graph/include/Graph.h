#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cstdlib>


struct Vertex
{
  Vertex()
  {
    posX = 400;
    posY = 400;
    velX = (static_cast<float>(rand()) / RAND_MAX)*2 - 1;
    velY = (static_cast<float>(rand()) / RAND_MAX)*2 - 1;
  }

  float posX, posY;
  float velX, velY;
};


class Graph
{
public:
  int addNode() {
    int index = m_nodes.size();
    Vertex v;
    std::vector<int> a;
    m_nodes.push_back(v);
    m_adjacency.push_back(a);
    return index;
  }

  void addEdge(int source, int target)
  { 
    m_adjacency[source].push_back(target);
    m_adjacency[target].push_back(source);
  }
  Vertex &node(int i) { return m_nodes[i]; }
  std::vector<int> &adjacency(int i) { return m_adjacency[i]; }
  size_t size() { return m_nodes.size(); }

private:
  std::vector<Vertex> m_nodes;
  std::vector< std::vector<int> > m_adjacency;
};

#endif /* GRAPH_H */