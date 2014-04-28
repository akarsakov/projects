#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#define sigma 5
#define NUM_AMINS 20

enum DIRECTIONS { down = 0, right, diagonal };

void OutputLCS_v(vector<vector<DIRECTIONS>>& backrack, string v, int i, int j, FILE* out)
{
	if ((i==0) && (j==0))
	{
		return;
	}

	if (backrack[i][j] == DIRECTIONS::down)
	{
		OutputLCS_v(backrack, v, i-1, j, out);
		fprintf(out, "%c", '-');
	}
	else if (backrack[i][j] == DIRECTIONS::right)
	{
		OutputLCS_v(backrack, v, i, j-1, out);
		fprintf(out, "%c", v[j-1]);
	}
	else
	{
		OutputLCS_v(backrack, v, i-1, j-1, out);
		fprintf(out, "%c", v[j-1]);
	}
}

void OutputLCS_w(vector<vector<DIRECTIONS>>& backrack, string v, int i, int j, FILE* out)
{
	if ((i==0) && (j==0))
	{
		return;
	}

	if (backrack[i][j] == DIRECTIONS::down)
	{
		OutputLCS_w(backrack, v, i-1, j, out);
		fprintf(out, "%c", v[i-1]);
	}
	else if (backrack[i][j] == DIRECTIONS::right)
	{
		OutputLCS_w(backrack, v, i, j-1, out);
		fprintf(out, "%c", '-');
	}
	else
	{
		OutputLCS_w(backrack, v, i-1, j-1, out);
		fprintf(out, "%c", v[i-1]);
	}
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	FILE* blosum_file = fopen("BLOSUM62.txt", "r");
	char str_c[4100];
	int indexes[256];
	vector<vector<int>> s, weights;
	vector<vector<DIRECTIONS>> backtracks;
	fscanf(file, "%s", str_c);
	string v(str_c);
	fscanf(file, "%s", str_c);
	string w(str_c);
	int m = v.size();
	int n = w.size();

	for (int i=0; i<NUM_AMINS; i++)
	{
		char acid;
		fscanf(blosum_file, "%c", &acid);
		indexes[acid] = i;
	}

	weights.resize(NUM_AMINS);
	for (int i=0; i<NUM_AMINS; i++)
	{
		weights[i].resize(NUM_AMINS);
		for (int j=0; j<NUM_AMINS; j++)
		{
			int weight;
			fscanf(blosum_file, "%d", &weight);
			weights[i][j] = weight;
		}
	}

	s.resize(n+1);
	for (int i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (int i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (int i=1; i<n+1; i++)
	{
		s[i][0] = s[i-1][0] - sigma;
		backtracks[i][0] = DIRECTIONS::down;
	}

	for (int j=1; j<m+1; j++)
	{
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::right;
	}

	for (int i=1; i<n+1; i++)
	{
		for (int j=1; j<m+1; j++)
		{
			int diag = s[i-1][j-1] + weights[indexes[v[j-1]]][indexes[w[i-1]]];
			
			int maximum = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::right;
			else if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::down;
			else
				backtracks[i][j] = DIRECTIONS::diagonal;

			s[i][j] = maximum;
		}
	}
	fprintf(out, "%d\n", s[n][m]);
	OutputLCS_v(backtracks, v, n, m, out);
	fprintf(out, "\n");
	OutputLCS_w(backtracks, w, n, m, out);
	fclose(blosum_file);
	fclose(out);
	fclose(file);
	return 0;
}