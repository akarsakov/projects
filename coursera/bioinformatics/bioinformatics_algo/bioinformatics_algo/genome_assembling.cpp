#include <algorithm>

#include "genome_assembling.h"

using namespace std;

namespace bio
{

DeBruijnGraph::DeBruijnGraph(string text, int k) {
    string from = text.substr(0, k-1); 
    for (size_t i=1; i<text.size()-k+2; i++) {
        string to = text.substr(i, k-1);
        addEdge(from, to);
        from = to;
    }
}

DeBruijnGraph::DeBruijnGraph(vector<string> kMers) {
    for (auto kMer : kMers) {
        string from = kMer.substr(0, kMer.size()-1);
        string to = kMer.substr(1, kMer.size()-1);
        addEdge(from, to);
    }
}

void DeBruijnGraph::addEdge(string from, string to) {
    auto f = find_if(adjList.begin(), adjList.end(), 
        [=] (const pair<string, list<string>>& elem) {
            return elem.first == from;
        });

    if (f != adjList.end()) {
        f->second.push_back(to);
    } else {
        adjList.push_back(make_pair(from, list<string>(1, to)));
    }
}

};