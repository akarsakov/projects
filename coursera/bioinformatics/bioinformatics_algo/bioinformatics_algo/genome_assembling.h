#ifndef GENOME_ASSEMBLING
#define GENOME_ASSEMBLING

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

namespace bio 
{

class Graph {
public:
    Graph(int v = 0);
    virtual ~Graph() { };
    Graph(const Graph& g);

    void addEdge(int s, int e);
    bool getEdge(int s, int e);
    void removeEdge(int s, int e);

    std::list<int> getNeighbors(int s);
    std::list<int> getEulerianCycle(int start_vertex = 0);
    std::list<int> getEulerianPath();

private:
    int numV;
    std::vector<std::list<int>> adjList;
};

class DeBruijnGraph {
public:
    // construct De Bruijn graph from text string
    DeBruijnGraph(std::string text, int k);

    // construct De Bruijn graph from collection of k-mers
    DeBruijnGraph(std::vector<std::string> kMers);

    // construct De Bruijn graph from collection of k-mers
    DeBruijnGraph(std::vector<std::pair<std::string, std::string>> kMers);

    virtual ~DeBruijnGraph() { };

    void addEdge(std::string from, std::string to); 

    std::vector<std::pair<std::string, std::list<std::string>>> getAdjacencyList();
    std::string reconstructPath();
    std::string reconstructPathFromPairedReads(int k, int d);
    std::string reconstructCycle();
private:
    Graph g;
    std::unordered_map<std::string, int> vertexes;
};

} /* bio */

#endif /* GENOME_ASSEMBLING */