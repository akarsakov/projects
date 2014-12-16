#ifndef __GRAPH__
#define __GRAPH__

#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <iostream>

class Graph {
public:
    Graph(int num_vertex) {
        adjList.resize(num_vertex);
    }

    virtual ~Graph() { };

    double getEdge(int s, int e) const {
        auto edge = std::find_if(adjList[s].begin(), adjList[s].end(), 
            [=] (const std::pair<int, double>& edge) {
                return edge.first == e;
            });
        if (edge == adjList[s].end()) {
            return -1;
        } else {
            return edge->second;
        }
    }

    void addEdge(int s, int e, double w) {
        if (getEdge(s, e) < 0)
            adjList[s].push_back(std::make_pair(e, w));
        
    }

    std::list<std::pair<int, double>> getNeighbors(int s) const {
        return adjList[s];
    }

    void generate(double num_edges, double min_weight, double max_weight) {
        double numV = (double) adjList.size();
        double p = num_edges / (numV*(numV-1));

        for (int i=0; i<numV; i++) {
            for (int j=0; j<numV; j++) {
                if (i==j)
                    continue;

                if (rand() < p*RAND_MAX) {
                    double weight = min_weight + (rand() / (double) RAND_MAX) * (max_weight - min_weight);
                    addEdge(i, j, weight);
                }
            }
        }
    }

    int getV() const {
        return (int) adjList.size();
    }

    void print() {
        for (size_t i=0; i<adjList.size(); i++) {
            std::cout << i << " -> ";
            for (auto e : adjList[i]) {
                std::cout << e.first << " (" << e.second << "), ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::list<std::pair<int, double>>> adjList;
};

#endif /* __GRAPH__ */