#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include "Graph.h"
using namespace std;

#define SIZE 10000

int max_index = INT_MIN;

void parse(Graph& g, string line)
{
	size_t s_pos = line.find_first_of('(');
	while (s_pos != string::npos)
	{
		size_t e_pos = line.find_first_of(')', s_pos);
		string buffer = line.substr(s_pos+1, e_pos-s_pos-1);
		stringstream cycle(buffer);
		int first;
		cycle >> first;
		int prev = first;
		int cur;
		while (!cycle.eof())
		{
			cycle >> cur;
			g.add(prev, -cur);
			prev = cur;
			if (abs(cur) > max_index)
			{
				max_index = cur;
			}
		}
		g.add(prev, -first);
		s_pos = line.find_first_of('(', e_pos);
	}
}

int main()
{
	std::ifstream in("input.txt");
	Graph g(SIZE);
	string line;
	getline(in, line);
	parse(g, line);
	getline(in, line);
	parse(g, line);

	vector<bool> visited;
	visited.resize(2 * max_index + 1, false);
	int blocks = 0;
	int cycles = 0;

	for (int i=-max_index; i<=max_index; i++)
	{
		if (!visited[i + max_index])
		{
			cycles++;
			int next = g.get(i);
			if (next != 0)
			{
				int prev = i;
				g.del(prev, next);
				while(next != i)
				{
					if ((next < -max_index) || (next > max_index))
					{
						printf("fuck - %d", next);
					}
					visited[next + max_index] = true;
					prev = next;
					next = g.get(prev);
					g.del(prev, next);
				}
			}
		}
	}
	printf("result - %d\n", max_index - cycles + 1); 
	return 0;
}