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
	char* c_str = new char[1000];
	const int vertex = 2000;
	Graph g(vertex+1);
	vector<int> income;
	vector<int> outcome;

	income.resize(vertex+1, 0);
	outcome.resize(vertex+1, 0);

	map<string, int> nodes;
	vector<string> names;
	int index = 0;
	while(!feof(file))
	{
		char start[80];
		char end[80];
		fscanf(file, "%s -> %s", start, end);
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

	int start = 0;
	int end = 0;
	for (int i=0; i<vertex+1; i++)
	{
		if (income[i] < outcome[i])
			end = i;
		if (income[i] > outcome[i])
			start = i;
	}
	g.addEdge(start, end);
	printf("start - %d, end - %d\n", start, end);
	printf("start - %s\n", names[start].c_str());
	printf("end - %s\n", names[end].c_str());
	printf("%d - %d\n", income[end], outcome[end]);

	stack<int> st;
	Result result;
	st.push(0);
	result.push(0);
	index = 1;
	while(!st.empty())
	{
		int current = st.top();
		int next = g.getEdges(current);

		if (next < 0)
		{
			index--;
			st.pop();
		}
		else
		{
			g.deleteEdge(current, next);
			st.push(next);
			result.insert(index, next);
			index++;
		}
	}
	
	vector<int> res = result.getVector();
	int size = res.size() - 1;
	int begin = 0;
	for (int i=1; i<res.size(); i++)
	{
		if ((res[i] == end) && (res[i-1] == start))
		{
			begin = i;
			break;
		}
	}

	vector<int> ordered;
	for (int i=0; i<res.size()-1; i++)
	{
		ordered.push_back(res[(i + begin) % size]);
	}

	
	int str_size = names[ordered[0]].size();
	fprintf(out, "%s", names[ordered[0]].c_str());
	for (int i=1; i<ordered.size(); i++)
	{
		fprintf(out, "%c", names[ordered[i]][str_size - 1]);
	}

	delete[] c_str;
	return 0;
}