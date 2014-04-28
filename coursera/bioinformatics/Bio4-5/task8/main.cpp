#include <stdio.h>
#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <math.h>
#include <bitset>
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

string num2bin(int num, int size)
{
	char buffer[80];
	itoa(num, buffer, 2);
	string base(buffer);
	string res(size - base.size(), '0');
	res += base;
	return res;
}

#define N 16

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	int n;
	fscanf(file, "%d", &n);
	int vertex = pow(2, n);
	Graph g(pow(2, n-1));
	vector<string> names;

	int index = 0;
	for (int i=0; i<vertex; i++)
	{
		string current = num2bin(i, n);
		string start = current.substr(0, current.size() - 1);
		string end = current.substr(1, current.size() - 1);
		bitset<N> st(start);
		bitset<N> en(end);
		g.addEdge(st.to_ulong(), en.to_ulong());
	}

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
	fprintf(out, "%s", num2bin(res[0], n-1).c_str());
	for (int i=1; i<res.size() - n + 1; i++)
	{
		string cur = num2bin(res[i], n-1);
		fprintf(out, "%c", cur[cur.size() - 1]);
	}
	return 0;
}