#include <list>
#include <vector>

class Graph
{
private:
	int v;
	int e;
	std::vector<std::vector<bool>> mat;
public:
	Graph(int sz) {
		v = sz;
		mat.resize(sz);
		for (int i=0; i<sz; i++)
		{
			mat[i].resize(sz, false);
		}
	}

	~Graph() {};

	void addEdge(int start, int end)
	{
		mat[start][end] = true;
	}

	bool edge(int start, int end)
	{
		return mat[start][end];
	}

	void deleteEdge(int start, int end)
	{
		mat[start][end] = false;
	}

	int getEdges(int vertex)
	{
		int res = -1;
		for (int i=0; i<v; i++)
		{
			if (mat[vertex][i])
			{
				res = i;
				break;
			}
		}
		return res;
	}

};