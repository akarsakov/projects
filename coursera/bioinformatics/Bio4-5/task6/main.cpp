#include <stdio.h>
#include <string>
#include <stack>
#include <sstream>
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
	const int vertex = 3001;
	Graph g(vertex+1);
	vector<int> income;
	vector<int> outcome;

	income.resize(vertex+1, 0);
	outcome.resize(vertex+1, 0);

	while(!feof(file))
	{
		int start, end;
		fscanf(file, "%d -> %s", &start, c_str);
		std::stringstream lineStream(c_str);
		std::string num;

		while(std::getline(lineStream,num, ','))
		{
			end = atoi(num.c_str());
			g.addEdge(start, end);
			income[end] += 1;
			outcome[start] += 1;
		}
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
	
	stack<int> st;
	Result result;
	st.push(0);
	result.push(0);
	int index = 1;
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

	for (int i=0; i<res.size()-1; i++)
	{
		if (i == 0)
			fprintf(out, "%d", res[(i + begin) % size]);
		else
			fprintf(out, "->%d", res[(i + begin) % size]);
	}

	delete[] c_str;
	return 0;
}