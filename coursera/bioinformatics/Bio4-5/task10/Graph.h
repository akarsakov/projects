#include <list>
#include <vector>

class Graph
{
private:
	int v;
	int e;
	std::vector<std::vector<int>> mat;
public:
	Graph(int sz) {
		v = sz;
		e = 0;
		mat.resize(sz);
		for (int i=0; i<sz; i++)
		{
			mat[i].resize(sz, 0);
		}
	}

	~Graph() {};

	void addEdge(int start, int end)
	{
		mat[start][end]+=1;
		e++;
	}

	bool edge(int start, int end)
	{
		return mat[start][end] > 0;
	}

	void deleteEdge(int start, int end)
	{
		mat[start][end]-=1;
		e--;
	}

	int numEdges()
	{
		return e;
	}

	int getEdges(int vertex)
	{
		int res = -1;
		for (int i=0; i<v; i++)
		{
			if (mat[vertex][i] > 0)
			{
				res = i;
				break;
			}
		}
		return res;
	}
};