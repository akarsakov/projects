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

int getIndex(string str, map<string, int>& nodes, vector<string>& names, int& index)
{
	int res = 0;
	auto f = nodes.find(str);
	if (f != nodes.end())
		res = f->second;
	else
	{
		nodes[str] = index;
		names.push_back(str);
		res = index;
		index++;
	}
	return res;
}

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	const int vertex = 10650;
	Graph g(vertex);
	vector<int> income;
	vector<int> outcome;

	income.resize(vertex, 0);
	outcome.resize(vertex, 0);
	int d;
	fscanf(file, "%d", &d);
	map<string, int> nodes;
	vector<string> names;
	int index = 0;
	while(!feof(file))
	{
		char start_c[80];
		char end_c[80];
		fscanf(file, "%s | %s", start_c, end_c);
		string st(start_c);
		string en(end_c);

		string start = st.substr(0, st.size() - 1) + en.substr(0, en.size() - 1);
		string end = st.substr(1, st.size() - 1) + en.substr(1, en.size() - 1);

		int start_index = getIndex(start, nodes, names, index);
		int end_index = getIndex(end, nodes, names, index);
		g.addEdge(start_index, end_index);
		income[end_index] += 1;
		outcome[start_index] += 1;
	}

	int start = 0;
	int end = 0;
	for (int i=0; i<vertex; i++)
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

	
	int str_size = names[ordered[0]].size() / 2;
	fprintf(out, "%s", names[ordered[0]].substr(0, str_size).c_str());
	for (int i=1; i<ordered.size(); i++)
	{
		fprintf(out, "%c", names[ordered[i]][str_size - 1]);
	}

	int offset = str_size + 1 + d;
	int sz = ordered.size();
	for (int i=0; i<offset; i++)
	{
		fprintf(out, "%c", names[ordered[sz - offset + i]][2*str_size - 1]);
	}
	return 0;
}