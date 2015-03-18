#ifndef __GRAPH__
#define __GRAPH__

#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <iostream>

class Graph {
public:
    Graph(size_t num_vertex) {
        adjList.resize(num_vertex);
    }

    virtual ~Graph() { };

    double getEdge(size_t s, size_t e) const {
        auto edge = std::find_if(adjList[s].begin(), adjList[s].end(), 
            [=] (const std::pair<size_t, double>& edge) {
                return edge.first == e;
            });
        if (edge == adjList[s].end()) {
            return -1;
        } else {
            return edge->second;
        }
    }

    void addEdge(size_t s, size_t e, double w) {
        if (getEdge(s, e) < 0)
            adjList[s].push_back(std::make_pair(e, w));
        
    }

    std::list<std::pair<size_t, double>> getNeighbors(size_t s) const {
        return adjList[s];
    }

    void generate(double num_edges, double min_weight, double max_weight) {
        double numV = (double) adjList.size();
        double p = num_edges / (numV*(numV-1));

        for (size_t i=0; i<numV; i++) {
            for (size_t j=0; j<numV; j++) {
                if (i==j)
                    continue;

                if (rand() < p*RAND_MAX) {
                    double weight = min_weight + (rand() / (double) RAND_MAX) * (max_weight - min_weight);
                    addEdge(i, j, weight);
                }
            }
        }
    }

    size_t getV() const {
        return (size_t) adjList.size();
    }

    void prsize_t() {
        for (size_t i=0; i<adjList.size(); i++) {
            std::cout << i << " -> ";
            for (auto e : adjList[i]) {
                std::cout << e.first << " (" << e.second << "), ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::list<std::pair<size_t, double>>> adjList;
};

#endif /* __GRAPH__ */