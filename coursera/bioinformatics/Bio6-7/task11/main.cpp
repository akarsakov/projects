#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#define sigma 5
#define NUM_AMINS 20

enum DIRECTIONS { down = 0, right, diagonal };

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	FILE* blosum_file = fopen("BLOSUM62.txt", "r");
	char str_c[2010];
	int indexes[256];
	vector<vector<int>> weights;
	vector<vector<int>> s;
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
	
	s.resize(m+1);
	for (int i=0; i<m+1; i++)
		s[i].resize(n+1, INT_MAX);

	backtracks.resize(m+1);
	for (int i=0; i<m+1; i++)
		backtracks[i].resize(n+1);
	
	int middle = n/2;

	// from source
	s[0][0] = 0;
	for (int i=1; i<m+1; i++)
	{
		s[i][0] = s[i-1][0] - sigma;
		backtracks[i][0] = DIRECTIONS::down;
	}

	for (int j=1; j<n+1; j++)
	{
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::right;
	}
	
	for (int j=1; j<=middle; j++)
	{
		for (int i=1; i<m+1; i++)
		{
			int diag = s[i-1][j-1] + weights[indexes[v[i-1]]][indexes[w[j-1]]];
			
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
	vector<int> mid;
	mid.resize(m+1);
	for (int i=0; i<m+1; i++)
		mid[i] = s[i][middle];

	// to sink
	s[m][n] = 0;
	for (int i=m-1; i>=0; i--)
	{
		s[i][n] = s[i+1][n] - sigma;
		backtracks[i+1][n] = DIRECTIONS::down;
	}

	for (int j=n-1; j>=0; j--)
	{
		s[m][j] = s[m][j+1] - sigma;
		backtracks[m][j] = DIRECTIONS::right;
	}

	for (int j=n-1; j>=middle; j--)
	{
		for (int i=m-1; i>=0; i--)
		{
			int diag = s[i+1][j+1] + weights[indexes[v[i]]][indexes[w[j]]];
			
			int maximum = max(diag, max(s[i+1][j] - sigma, s[i][j+1] - sigma));
			if (maximum == s[i][j+1] - sigma)
				backtracks[i][j] = DIRECTIONS::right;
			else if (maximum == s[i+1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::down;
			else
				backtracks[i][j] = DIRECTIONS::diagonal;
			s[i][j] = maximum;
		}
	}
	int max_s = INT_MIN;
	int max_i = 0;
	for (int i=0; i<m+1; i++) 
	{
		if (mid[i] + s[i][middle] > max_s)
		{
			max_s = mid[i] + s[i][middle];
			max_i = i;
		}
	}
	fprintf(out, "(%d,%d) ", max_i, middle);
	if (backtracks[max_i][middle] == DIRECTIONS::right)
		fprintf(out, "(%d,%d)", max_i, middle+1);
	else if (backtracks[max_i][middle] == DIRECTIONS::down)
		fprintf(out, "(%d,%d)", max_i+1, middle);
	else
		fprintf(out, "(%d,%d)", max_i+1, middle+1);
	fclose(blosum_file);
	fclose(out);
	fclose(file);
	return 0;
}