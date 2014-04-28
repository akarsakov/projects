#include <list>
#include <vector>
using namespace std;

class Graph
{
private:
	int v;
	int e;
	vector<list<int>> mat;
public:
	Graph(int sz) {
		v = sz;
		mat.resize(sz);
	}

	~Graph() {};

	void addEdge(int start, int end)
	{
		mat[start].push_back(end);
	}

	bool edge(int start, int end)
	{
		auto f = find(mat[start].begin(), mat[start].end(), end);
		if (f != mat[start].end())
			return true;
		else
			return false;
	}

	void deleteEdge(int start, int end)
	{
		mat[start].remove(end);
	}

	int getEdges(int vertex)
	{
		int res = -1;
		if (!mat[vertex].empty())
		{
			res = mat[vertex].front();
		}
		return res;
	}

};