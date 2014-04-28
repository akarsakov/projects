#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#define sigma 1

enum DIRECTIONS { down = 0, right, diagonal };

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char str_c[1000];
	int indexes[256];
	vector<vector<int>> s;
	fscanf(file, "%s", str_c);
	string w(str_c);
	fscanf(file, "%s", str_c);
	string v(str_c);
	int m = v.size();
	int n = w.size();

	s.resize(n+1);
	for (int i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);
	
	s[0][0] = 0;
	for (int i=1; i<n+1; i++)
	{
		s[i][0] = s[i-1][0] - sigma;
	}

	for (int j=1; j<m+1; j++)
	{
		s[0][j] = s[0][j-1] - sigma;
	}

	for (int i=1; i<n+1; i++)
	{
		for (int j=1; j<m+1; j++)
		{
			int diag = v[j-1] == w[i-1] ? s[i-1][j-1] : s[i-1][j-1] - sigma;
			s[i][j] = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
		}
	}
	fprintf(out, "%d\n", -s[n][m]);
	fclose(out);
	fclose(file);
	return 0;
}