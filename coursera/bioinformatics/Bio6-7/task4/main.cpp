#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
	int s;
	int e;
	int w;

	Node(int _s, int _e, int _w): s(_s), e(_e), w(_w) {};

	bool operator<(const Node& rhs)
	{
		if (s < rhs.s)
			return true;
		else if (s > rhs.s)
			return false;
		else if (e < rhs.e)
			return true;
		else 
			return false;
	}
};

void OutputPath(vector<int>& backtrack, int start, int cur, FILE* out)
{
	if (cur == start)
	{
		fprintf(out, "%d", cur);
		return;
	}

	OutputPath(backtrack, start, backtrack[cur], out);
	fprintf(out, "->%d", cur);
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	int start, end;
	fscanf(file, "%d", &start);
	fscanf(file, "%d", &end);
	vector<Node> edges;
	vector<int> s, backtrack;
	s.resize(end+1, INT_MIN);
	backtrack.resize(end+1);

	while (!feof(file))
	{
		int s, e, w;
		fscanf(file, "%d->%d:%d", &s, &e, &w);
		Node temp(s,e,w);
		edges.push_back(temp);
	}

	sort(edges.begin(), edges.end());
	s[start] = 0;
	backtrack[start] = 0;

	for (auto it=edges.begin(); it!=edges.end(); it++)
	{
		Node cur = *it;
		if (s[cur.e] < s[cur.s] + cur.w)
		{
			s[cur.e] = s[cur.s] + cur.w;
			backtrack[cur.e] = cur.s;
		}
	}
	fprintf(out, "%d\n", s[end]);
	OutputPath(backtrack, start, end, out);
	printf("\n");
	fclose(out);
	fclose(file);
	return 0;
}