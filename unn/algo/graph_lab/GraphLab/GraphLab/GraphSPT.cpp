
#include "GraphSPT.h"
#include "BinaryHeap.h"

using namespace std;

double Deijkstra_SPT(const Graph& g, int s, int e) {
    BinaryHeap heap(g.getV());
    vector<bool> visited(g.getV(), 0);
    vector<double> distances(g.getV(), DBL_MAX);
    
    heap.insert(make_pair(s, 0));
    distances[s] = 0;


    while (!heap.isEmpty()) {
        pair<int, double> current = heap.top();
        heap.pop();

        if (visited[current.first])
            continue;
        visited[current.first] = true;

        //if (current.first == e)
        //    break;

        list<pair<int, double>> neighbors = g.getNeighbors(current.first);
        for (auto n : neighbors) {
            double newDist = current.second + n.second;
            if (distances[n.first] < DBL_MAX) {
                if (newDist < distances[n.first]) {
                    heap.increaseKey(n.first, newDist);
                    distances[n.first] = newDist;
                }
            } else {
                distances[n.first] = newDist;
                heap.insert(make_pair(n.first, newDist)); 
            }
        }
    }
    return distances[e];
}

double FordBellman_SPT(const Graph& g, int s, int e) {
    vector<double> distances(g.getV(), DBL_MAX);
    distances[s] = 0;

    for (int i=0; i<g.getV()-1; i++) {
        bool changed = false;
        for (int j=0; j<g.getV(); j++) {
            if (distances[j] == DBL_MAX)
                continue;

            list<pair<int, double>> neighbors = g.getNeighbors(j);
            for (auto n : neighbors) {
                if (distances[j] + n.second < distances[n.first]) {
                    distances[n.first] = distances[j] + n.second;
                    changed = true;
                }
            }
        }

        if (!changed)
            break;
    }

    return distances[e];
}
