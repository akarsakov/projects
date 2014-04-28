#include <vector>
using namespace std;

class Graph
{
private:
	int sz;
	vector<vector<int>> mas;
public:
	Graph(int size)
	{
		sz = size;
		mas.resize(2*size + 1);
		for (int i=0; i<2*size+1; i++)
		{
			mas[i].resize(2*size+1, 0);
		}
	}

	void add(int start, int end)
	{
		mas[start+sz][end+sz] += 1;
		mas[end+sz][start+sz] += 1;
	}

	int get(int start)
	{
		int res = sz;
		for (int i=0; i<2*sz+1; i++)
		{
			if (mas[start+sz][i] > 0)
			{
				res = i;
				break;
			}
		}
		return res-sz;
	}

	void del(int start, int end)
	{
		mas[start+sz][end+sz] -= 1;
		mas[end+sz][start+sz] -= 1;
	}

};