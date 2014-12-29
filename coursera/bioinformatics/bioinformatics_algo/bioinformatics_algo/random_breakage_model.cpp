#include <sstream>
#include <list>

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

    int getNumCycles() {
        int cycles = 0;
        vector<bool> visited(adjList.size(), false);
        for (size_t i=0; i<adjList.size(); i++) {
            if (visited[i])
                continue;

            visited[i] = true;
            cycles++;
            int next = getNeighbor(i);
            removeEdge(i, next);

            while (next != i) {
                visited[next] = true;
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
    size_t i=0;
    for ( ; i<edges.size(); i++) {
        vector<int> cycle;
        int start = edges[i].first % 2 == 0 ? edges[i].first-1 : edges[i].first+1;
        cycle.push_back(start);

        while (edges[i].second != start) {
            cycle.push_back(edges[i].first);
            cycle.push_back(edges[i].second);
            i++;
        }
        cycle.push_back(edges[i].first);
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
    return g.getNumCycles();
}

namespace week6
{

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