#include <sstream>
#include <list>
#include <algorithm>

#include "random_breakage_model.h"

using namespace std;

class UndirectedGraph {
private:
    vector<list<int>> adjList;
public:
    UndirectedGraph(int size) {
        adjList.resize(size, list<int>());
    }

    void addEdge(int start, int end) {
        adjList[start].push_back(end);
        adjList[end].push_back(start);
    }

    int getNeighbor(int v) {
        return adjList[v].front();
    }

    void removeEdge(int start, int end) {
        auto f = find(adjList[start].begin(), adjList[start].end(), end);
        adjList[start].erase(f);
             f = find(adjList[end].begin(), adjList[end].end(), start);
        adjList[end].erase(f);
    }

    vector<vector<int>> getCycles() {
        vector<vector<int>> cycles;
        vector<bool> visited(adjList.size(), false);
        for (size_t i=0; i<adjList.size(); i++) {
            if (visited[i])
                continue;

            visited[i] = true;
            cycles.push_back(vector<int>());
            cycles.back().push_back(i);

            int next = getNeighbor(i);
            removeEdge(i, next);

            while (next != i) {
                visited[next] = true;
                cycles.back().push_back(next);
                int prev = next;
                next = getNeighbor(prev);
                removeEdge(prev, next);
            }
        }
        return cycles;
    }

};

namespace bio
{

void reversal(vector<int>& perm, int start, int end)
{
	while (start < end)
	{
		int st = -perm[start];
		int en = -perm[end];
		perm[start] = en;
		perm[end] = st;
		start++;
		end--;
	}
	if (start == end)
		perm[start] = -perm[start];
}

void readPermutationString(string str, vector<vector<int>>& permutation) {
    size_t s_pos = str.find_first_of('(');
	while (s_pos != string::npos)
	{
		size_t e_pos = str.find_first_of(')', s_pos);
		string buffer = str.substr(s_pos+1, e_pos-s_pos-1);
		stringstream cycle(buffer);
        permutation.push_back(vector<int>());
		while (!cycle.eof())
		{
            int cur;
			cycle >> cur;
            permutation.back().push_back(cur);
		}
		s_pos = str.find_first_of('(', e_pos);
	}
}

string permutationString(const vector<int>& perm) {
    stringstream result;
    result << "(";
	for (auto it = perm.begin(); it!=perm.end(); it++)
	{
		if (it != perm.begin())
			result << " ";
		if (*it > 0)
			result << "+" << *it;
		else
            result << *it;
	}
	result << ")";
    return result.str();
}

int getNumBreakpoints(const vector<int>& perm) {
    vector<int> permutation = perm;
    permutation.push_back(permutation.size());
	int breakpoints = 0;
	for (size_t i=0; i<permutation.size()-1; i++)
	{
		if (permutation[i+1] != permutation[i] + 1)
			breakpoints++;
	}
    return breakpoints;
}

vector<int> chromosomeToCycle(const vector<int>& chromosome) {
    vector<int> cycle(2*chromosome.size());
    for (size_t j=0; j<chromosome.size(); j++) {
        int i = chromosome[j];
        if (i > 0) {
            cycle[2*j] = 2*i - 1;
            cycle[2*j+1] = 2*i;
        } else {
            cycle[2*j] = -2*i;
            cycle[2*j+1] = -2*i-1;
        }
    }
    return cycle;
}

vector<int> cycleToChromosome(const vector<int>& cycle) {
    vector<int> chromosome(cycle.size() / 2);
    for (size_t j=0; j<cycle.size() / 2; j++) {
        if (cycle[2*j] < cycle[2*j+1])
            chromosome[j] = cycle[2*j+1]/2;
        else
            chromosome[j] = -cycle[2*j]/2;
    }
    return chromosome;
}

vector<pair<int, int>> getColoredEdges(const vector<vector<int>>& permutations) {
    vector<pair<int, int>> edges;
    for (auto chromosome : permutations) {
        vector<int> cycle = chromosomeToCycle(chromosome);
        for (size_t i=0; i<chromosome.size()-1; i++) {
            edges.push_back(make_pair(cycle[2*i+1], cycle[2*i+2]));
        }
        edges.push_back(make_pair(cycle[cycle.size()-1], cycle[0]));
    }
    return edges;
}

vector<vector<int>> edgesToGenome(const vector<pair<int, int>>& edges) {
    vector<vector<int>> genome;
    
    vector<int> graph(2*edges.size() + 1);
    for (auto edge : edges) {
        graph[edge.first] = edge.second;
        int in = edge.first % 2 == 0 ? edge.first-1 : edge.first+1;
        graph[in] = edge.first;
    }

    vector<int> visited(edges.size()+1, false);
    for (size_t i=1; i<visited.size(); i++) {
        if (visited[i])
            continue;

        vector<int> cycle;
        int next = i;
        do {
            visited[next] = true;
            int cur;
            if (graph[2*next-1] == 2*next) {
                cycle.push_back(2*next-1);
                cycle.push_back(2*next);
                cur = 2*next;
            } else {
                cycle.push_back(2*next);
                cycle.push_back(2*next-1);
                cur = 2*next-1;
            }
            next = (graph[cur]+1)/2;
        } while (next != i);

        genome.push_back(cycleToChromosome(cycle));
    }
    return genome;
}

int getNumCycles(const vector<pair<int, int>>& edges) {
    int max_index = INT_MIN;
    for (auto edge : edges) {
        max_index = max(max_index, edge.first);
        max_index = max(max_index, edge.second);
    }
    UndirectedGraph g(max_index);
    for (auto edge : edges) {
        g.addEdge(edge.first-1, edge.second-1);
    }
    return g.getCycles().size();
}

void twoBrakeOnGenomeGraph(vector<pair<int, int>>& edges, int i1, int i2, int j1, int j2) {
    int i_index, j_index;
    
    for (size_t i=0; i<edges.size(); i++) {
        if ((edges[i].first == i1 && edges[i].second == i2) ||
            (edges[i].first == i2 && edges[i].second == i1)) {
            i_index = i;
        }
        
        if ((edges[i].first == j1 && edges[i].second == j2) ||
            (edges[i].first == j2 && edges[i].second == j1)) {
            j_index = i;
        }
    }

    swap(edges[i_index].second, edges[j_index].second);
}

void twoBrakeOnGenome(vector<vector<int>>& genome, int i1, int i2, int j1, int j2) {
    vector<pair<int, int>> edges = getColoredEdges(genome);
    twoBrakeOnGenomeGraph(edges, i1, i2, j1, j2);
    genome = edgesToGenome(edges);
}

namespace week6
{
void twoBreakSorting(const vector<int>& P, const vector<int>& Q, vector<vector<int>>& seq_permutations) {
    seq_permutations.push_back(P);
    vector<pair<int, int>> edges

}

void greedySorting(const vector<int>& perm, vector<vector<int>>& seq_permutations) {
    vector<int> permutation = perm;
    int distance = 0;

	for (int k=1; k<(int) permutation.size()+1; k++) {
		if (abs(permutation[k-1]) != k)
		{
			int index = 0;
			for (int i=k-1; i<(int) permutation.size(); i++)
				if (abs(permutation[i]) == k) 
				{
					index = i;
					break;
				}
			reversal(permutation, k-1, index);
            seq_permutations.push_back(permutation);
			distance++;
		}
		if (permutation[k-1] == -k)
		{
			permutation[k-1] = k;
			seq_permutations.push_back(permutation);
			distance++;
		}
	}
}

} /* week6 */
} /* bio */