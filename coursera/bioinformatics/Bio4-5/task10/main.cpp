#include <stdio.h>
#include <string>
#include <stack>
#include <sstream>
#include <map>
#include "Graph.h"
using namespace std;

class Result
{
private:
	vector<int> res;
public:
	Result() {};

	void insert(int pos, int val)
	{
		if (pos < res.size())
		{
			res.push_back(res[res.size() - 1]);
			for (int i=res.size()-1; i > pos; i--)
			{
				res[i] = res[i-1];
			}
			res[pos] = val;
		} else
			res.push_back(val);
	}

	void push(int val)
	{
		res.push_back(val);
	}

	vector<int> getVector()
	{
		return res;
	}

};

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	const int vertex = 3250;
	Graph g(vertex);
	vector<int> income;
	vector<int> outcome;

	income.resize(vertex, 0);
	outcome.resize(vertex, 0);

	map<string, int> nodes;
	vector<string> names;
	int index = 0;
	while(!feof(file))
	{
		char str_c[80];
		fscanf(file, "%s", str_c);
		string str(str_c);
		string start = str.substr(0, str.size()-1);
		string end = str.substr(1, str.size()-1);
		int st = 0;
		auto f = nodes.find(start);
		if (f != nodes.end())
			st = f->second;
		else
		{
			nodes[start] = index;
			names.push_back(start);
			st = index;
			index++;
		}
		int en = 0;
		f = nodes.find(end);
		if (f != nodes.end())
			en = f->second;
		else
		{
			nodes[end] = index;
			names.push_back(end);
			en = index;
			index++;
		}

		g.addEdge(st, en);
		income[en] += 1;
		outcome[st] += 1;
	}
	vector<string> contigs;
	int str_size = names[0].size();

	for (int i=0; i<names.size(); i++)
	{
		if (((income[i] == 1) && (outcome[i] == 1)) || (outcome[i] == 0))
		{
			printf("skip ");
		}
		else
		{
			for (int j=0; j<outcome[i]; j++)
			{
				string current = names[i];
				int next = g.getEdges(i);
				current += names[next][str_size - 1];
				g.deleteEdge(i, next);

				while ((outcome[next] == 1) && (income[next] == 1))
				{
					int new_next = g.getEdges(next);

					if (new_next < 0)
						break;
					g.deleteEdge(next, new_next);
					next = new_next;
					current += names[next][str_size - 1];
				}

				contigs.push_back(current);
			}
		}
	}

	for (auto it=contigs.begin(); it!=contigs.end(); it++)
	{
		fprintf(out, "%s\n", it->c_str());
	}

	return 0;
}