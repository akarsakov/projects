#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

enum DIRECTIONS { down = 0, right = 1, diagonal };

void OutputLCS(vector<vector<DIRECTIONS>>& backrack, string v,int i, int j, FILE* out)
{
	if ((i==0) || (j==0))
		return;

	if (backrack[i][j] == DIRECTIONS::down)
		OutputLCS(backrack, v, i-1, j, out);
	else if (backrack[i][j] == DIRECTIONS::right)
		OutputLCS(backrack, v, i, j-1, out);
	else
	{
		OutputLCS(backrack, v, i-1, j-1, out);
		fprintf(out, "%c", v[j-1]);
	}
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char str_c[1000];
	vector<vector<int>> s;
	vector<vector<DIRECTIONS>> backtracks;
	fscanf(file, "%s", str_c);
	string v(str_c);
	fscanf(file, "%s", str_c);
	string w(str_c);
	int m = v.size();
	int n = w.size();

	s.resize(n+1);
	for (int i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (int i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (int i=0; i<n+1; i++)
	{
		s[i][0] = 0;
		backtracks[i][0] = DIRECTIONS::down;
	}

	for (int j=0; j<m+1; j++)
	{
		s[0][j] = 0;
		backtracks[0][j] = DIRECTIONS::right;
	}

	for (int i=1; i<n+1; i++)
	{
		for (int j=1; j<m+1; j++)
		{
			int diag = s[i-1][j-1];
			if (v[j-1] == w[i-1])
				diag = s[i-1][j-1] + 1;
			
			int maximum = max(diag, max(s[i-1][j], s[i][j-1]));
			if (maximum == s[i][j-1])
				backtracks[i][j] = DIRECTIONS::right;
			else if (maximum == s[i-1][j])
				backtracks[i][j] = DIRECTIONS::down;
			else
				backtracks[i][j] = DIRECTIONS::diagonal;

			s[i][j] = maximum;
		}
	}
	printf("result - %d\n", s[n][m]);
	OutputLCS(backtracks, v, n, m, out);
	printf("\n");
	fclose(out);
	fclose(file);
	return 0;
}