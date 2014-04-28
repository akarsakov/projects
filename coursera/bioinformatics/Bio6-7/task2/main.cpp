#include <stdio.h>
#include <vector> 
#include <limits>
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	int n,m;
	vector<vector<int>> down, right, s;
	fscanf(file, "%d %d", &n, &m);
	
	down.resize(n);
	for (int i=0; i<n; i++)
	{
		down[i].resize(m+1, 0);
		for (int j=0; j<m+1; j++)
		{
			int temp;
			fscanf(file, "%d", &temp);
			down[i][j] = temp;
		}
	}
	char temp[80];
	fscanf(file, "%s", temp);

	right.resize(n+1);
	for (int i=0; i<n+1; i++)
	{
		right[i].resize(m, 0);
		for (int j=0; j<m; j++)
		{
			int temp;
			fscanf(file, "%d", &temp);
			right[i][j] = temp;
		}
	}

	s.resize(n+1);
	for (int i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);
	
	s[0][0] = 0;
	for (int i=1; i<n+1; i++)
	{
		s[i][0] = s[i-1][0] + down[i-1][0];
	}

	for (int j=1; j<m+1; j++)
	{
		s[0][j] = s[0][j-1] + right[0][j-1];
	}

	for (int i=1; i<n+1; i++)
	{
		for (int j=1; j<m+1; j++)
		{
			s[i][j] = max(s[i-1][j] + down[i-1][j], s[i][j-1] + right[i][j-1]);
		}
	}

	printf("result - %d\n", s[n][m]);
	fclose(file);
	return 0;
}