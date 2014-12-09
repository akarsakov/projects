#include <algorithm>
#include <stack>

#include "genome_assembling.h"

using namespace std;

namespace bio
{

Graph::Graph(int v) {
    numV = v;
    adjList.resize(numV, list<int>());
}

Graph::Graph(const Graph& g) {
    numV = g.numV;
    adjList = g.adjList;
}

bool Graph::getEdge(int s, int e) {
    auto f = find(adjList[s].begin(), adjList[s].end(), e);
    return f != adjList[s].end();
}

void Graph::addEdge(int s, int e) {
    adjList[s].push_back(e);
}

void Graph::removeEdge(int s, int e) {
    for (auto v=adjList[s].begin(); v!=adjList[s].end(); v++) {
        if (*v == e) {
            auto e = v;
            std::advance(e,1);
            adjList[s].erase(v, e);
            break;
        }
    }
}

list<int> Graph::getNeighbors(int s) {
    return adjList[s];
}

list<int> Graph::getEulerianCycle(int start_vertex) {
    list<int> result;
    Graph tempGraph = *this;
    
    stack<int> st;
    st.push(start_vertex);

    while (!st.empty()) {
        int cur = st.top();
        
        list<int> neighbors = tempGraph.getNeighbors(cur);

        if (neighbors.empty()) {
            result.push_front(cur);
            st.pop();
        } else {
            int n = neighbors.back();
            tempGraph.removeEdge(cur, n);
            st.push(n);
        }
    }
    return result;
}

list<int> Graph::getEulerianPath() {
    vector<int> income(numV, 0);
    vector<int> outcome(numV);

    for (size_t i=0; i<adjList.size(); i++) {
        outcome[i] = adjList[i].size();

        for (auto v : adjList[i]) {
            income[v]++;
        }
    }

    int start, end;
    for (int i=0; i<numV; i++) {
        if (income[i] < outcome[i])
            start = i;
        if (income[i] > outcome[i])
            end = i;
    }

    addEdge(end, start);
    list<int> cycle = getEulerianCycle(start);
    removeEdge(end, start);

    vector<int> path(cycle.begin(), cycle.end());
    int start_index;
    for (size_t i=0; i<path.size()-1; i++) {
        if (path[i] == end && path[i+1] == start)
            start_index = i+1;
    }

    list<int> result;
    for (size_t i=0; i<path.size()-1; i++) {
        result.push_back(path[(start_index + i + 1) % path.size()]);
    }
    return result;
}

DeBruijnGraph::DeBruijnGraph(string text, int k) {
    int numV = 0;
    for (size_t i=0; i<text.size()-k+2; i++) {
        string v = text.substr(i, k-1);
        auto f = vertexes.find(v);
        if (f == vertexes.end())
            vertexes.insert(make_pair(v, numV++));
    }
    g = Graph(numV);
    string from = text.substr(0, k-1);
    for (size_t i=1; i<text.size()-k+2; i++) {
        string to = text.substr(i, k-1);
        addEdge(from, to);
        from = to;
    }
}

DeBruijnGraph::DeBruijnGraph(vector<string> kMers) {
    int numV = 0;
    for (auto kMer : kMers) {
        string from = kMer.substr(0, kMer.size()-1);
        string to = kMer.substr(1, kMer.size()-1);
        auto f = vertexes.find(from);
        if (f == vertexes.end())
            vertexes.insert(make_pair(from, numV++));
        f = vertexes.find(to);
        if (f == vertexes.end())
            vertexes.insert(make_pair(to, numV++));
    }
    g = Graph(numV);
    for (auto kMer : kMers) {
        string from = kMer.substr(0, kMer.size()-1);
        string to = kMer.substr(1, kMer.size()-1);
        addEdge(from, to);
    }
}

DeBruijnGraph::DeBruijnGraph(vector<pair<string, string>> kMers) {
    int numV = 0;
    for (auto kMer : kMers) {
        string from = kMer.first.substr(0, kMer.first.size()-1) + '|' + kMer.second.substr(0, kMer.second.size()-1);
        string to = kMer.first.substr(1, kMer.first.size()-1) + '|' + kMer.second.substr(1, kMer.second.size()-1);
        auto f = vertexes.find(from);
        if (f == vertexes.end())
            vertexes.insert(make_pair(from, numV++));
        f = vertexes.find(to);
        if (f == vertexes.end())
            vertexes.insert(make_pair(to, numV++));
    }
    g = Graph(numV);
    for (auto kMer : kMers) {
        string from = kMer.first.substr(0, kMer.first.size()-1) + '|' + kMer.second.substr(0, kMer.second.size()-1);
        string to = kMer.first.substr(1, kMer.first.size()-1) + '|' + kMer.second.substr(1, kMer.second.size()-1);
        addEdge(from, to);
    }
}

void DeBruijnGraph::addEdge(string from, string to) {
    int s = vertexes.find(from)->second;
    int e = vertexes.find(to)->second;
    g.addEdge(s, e);
}

vector<pair<string, list<string>>> DeBruijnGraph::getAdjacencyList() {
    vector<pair<string, list<string>>> result;
    for (auto st : vertexes) {
        int s = st.second;
        list<int> neighbors = g.getNeighbors(s);
        list<string> res;
        for (auto n : neighbors) {
            auto f = find_if(vertexes.begin(), vertexes.end(), 
                [=] (const pair<string, int>& k) {
                    return k.second == n;
                });
            res.push_back(f->first);
        }
        result.push_back(make_pair(st.first, res));
    }
    return result;
}

string DeBruijnGraph::reconstructPath() {
    string genome;
    list<int> path = g.getEulerianPath();
    auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == path.front();
            });
    genome = f->first;
    auto it = path.begin();
    advance(it, 1);
    for (; it!=path.end(); it++) {
        auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == *it;
            });
        string cur = f->first;
        genome += cur[cur.size()-1];
    }

    return genome;
}

string DeBruijnGraph::reconstructPathFromPairedReads(int k, int d) {
    string genome;
    list<int> path = g.getEulerianPath();
    auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == path.front();
            });
    genome = f->first.substr(0, k-1);
    auto it = path.begin();
    advance(it, 1);
    for (; it!=path.end(); it++) {
        auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == *it;
            });
        string cur = f->first;
        genome += cur[k-2];
    }

    it = path.begin();
    advance(it, path.size() - (d + k));
    for (; it!=path.end(); it++) {
        auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == *it;
            });
        string cur = f->first;
        genome += cur[2*k-2];
    }
    return genome;
}

string DeBruijnGraph::reconstructCycle() {
    string genome;
    list<int> cycle = g.getEulerianCycle();
    auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == cycle.front();
            });
    genome = f->first;
    auto it = cycle.begin();
    advance(it, 1);
    for (; it!=cycle.end(); it++) {
        auto f = find_if(vertexes.begin(), vertexes.end(),
            [=] (const pair<string, int>& k) {
                return k.second == *it;
            });
        string cur = f->first;
        genome += cur[cur.size()-1];
    }

    return genome;
}

};