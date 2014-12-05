#ifndef GENOME_ASSEMBLING
#define GENOME_ASSEMBLING

#include <string>
#include <vector>
#include <list>

namespace bio 
{

class DeBruijnGraph {
public:
    // construct De Bruijn graph from text string
    DeBruijnGraph(std::string text, int k);

    // construct De Bruijn graph from collection of k-mers
    DeBruijnGraph(std::vector<std::string> kMers);

    virtual ~DeBruijnGraph() { };

    void addEdge(std::string from, std::string to); 

    std::vector<std::pair<std::string, std::list<std::string>>> getAdjacencyList() {
        return adjList;
    }
private:
    std::vector<std::pair<std::string, std::list<std::string>>> adjList;
};

} /* bio */

#endif /* GENOME_ASSEMBLING */