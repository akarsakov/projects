#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#define gap_open 11
#define gap_extension 1
#define NUM_AMINS 20

enum DIRECTIONS { down = 0, right, diagonal, ext, open };

void OutputLCS_v(vector<vector<DIRECTIONS>>& backrack, vector<vector<DIRECTIONS>>& l_backrack, vector<vector<DIRECTIONS>>& u_backrack, string v, int i, int j, int level, FILE* out)
{
	if ((i==0) && (j==0))
	{
		return;
	}

	if (level == 0) //middle
	{
		if (backrack[i][j] == DIRECTIONS::down) {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i-1, j, 2, out);
			fprintf(out, "%c", '-');
		} else if (backrack[i][j] == DIRECTIONS::right) {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i, j-1, 1, out);
			fprintf(out, "%c", v[j-1]);
		} else {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i-1, j-1, 0, out);
			fprintf(out, "%c", v[j-1]);
		}
	} else if (level == 1) //upper
	{
		if (u_backrack[i][j] == DIRECTIONS::ext) {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i, j-1, 1, out);
			fprintf(out, "%c", v[j-1]);
		} else {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i, j-1, 0, out);
			fprintf(out, "%c", v[j-1]);
		}
	} else //lower 
	{
		if (l_backrack[i][j] == DIRECTIONS::ext) {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i-1, j, 2, out);
			fprintf(out, "%c", '-');
		} else {
			OutputLCS_v(backrack, l_backrack, u_backrack, v, i-1, j, 0, out);
			fprintf(out, "%c", '-');
		}
	}
}

void OutputLCS_w(vector<vector<DIRECTIONS>>& backrack, vector<vector<DIRECTIONS>>& l_backrack, vector<vector<DIRECTIONS>>& u_backrack, string v, int i, int j, int level, FILE* out)
{
	if ((i==0) && (j==0))
	{
		return;
	}

	if (level == 0) //middle
	{
		if (backrack[i][j] == DIRECTIONS::down) {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i-1, j, 2, out);
			fprintf(out, "%c", v[i-1]);
		} else if (backrack[i][j] == DIRECTIONS::right) {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i, j-1, 1, out);
			fprintf(out, "%c", '-');
		} else {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i-1, j-1, 0, out);
			fprintf(out, "%c", v[i-1]);
		}
	} else if (level == 1) //upper
	{
		if (u_backrack[i][j] == DIRECTIONS::ext) {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i, j-1, 1, out);
			fprintf(out, "%c", '-');
		} else {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i, j-1, 0, out);
			fprintf(out, "%c", '-');
		}
	} else //lower 
	{
		if (l_backrack[i][j] == DIRECTIONS::ext) {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i-1, j, 2, out);
			fprintf(out, "%c", v[i-1]);
		} else {
			OutputLCS_w(backrack, l_backrack, u_backrack, v, i-1, j, 0, out);
			fprintf(out, "%c", v[i-1]);
		}
	}
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	FILE* blosum_file = fopen("BLOSUM62.txt", "r");
	char str_c[1000];
	int indexes[256];
	vector<vector<int>> weights;
	vector<vector<int>> middle, lower, upper;
	vector<vector<DIRECTIONS>> m_back, l_back, u_back;
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

	middle.resize(n+1);
	upper.resize(n+1);
	lower.resize(n+1);
	for (int i=0; i<n+1; i++)
	{
		middle[i].resize(m+1, INT_MIN);
		upper[i].resize(m+1, INT_MIN);
		lower[i].resize(m+1, INT_MIN);
	}

	m_back.resize(n+1);
	l_back.resize(n+1);
	u_back.resize(n+1);
	for (int i=0; i<n+1; i++)
	{
		m_back[i].resize(m+1);
		l_back[i].resize(m+1);
		u_back[i].resize(m+1);
	}
	
	middle[0][0] = 0;
	upper[0][0] = -gap_open;
	lower[0][0] = -gap_open;
	for (int i=1; i<n+1; i++)
	{
		upper[i][0] = upper[i-1][0] - gap_extension;
		middle[i][0] = upper[i][0];
		m_back[i][0] = DIRECTIONS::down;
		u_back[i][0] = DIRECTIONS::open;
	}

	for (int j=1; j<m+1; j++)
	{
		lower[0][j] = lower[0][j-1] - gap_extension;
		middle[0][j] = lower[0][j];
		m_back[0][j] = DIRECTIONS::right;
		l_back[0][j] = DIRECTIONS::open;
	}

	for (int i=1; i<n+1; i++)
	{
		for (int j=1; j<m+1; j++)
		{
			int maximum = max(lower[i-1][j] - gap_extension, middle[i-1][j] - gap_open);
			if (maximum == lower[i-1][j] - gap_extension)
				l_back[i][j] = DIRECTIONS::ext;
			else
				l_back[i][j] = DIRECTIONS::open;
			lower[i][j] = maximum;

			maximum = max(upper[i][j-1] - gap_extension, middle[i][j-1] - gap_open);
			if (maximum == upper[i][j-1] - gap_extension)
				u_back[i][j] = DIRECTIONS::ext;
			else
				u_back[i][j] = DIRECTIONS::open;
			upper[i][j] = maximum;

			int diag = middle[i-1][j-1] + weights[indexes[v[j-1]]][indexes[w[i-1]]];
			maximum = max(diag, max(lower[i][j], upper[i][j]));
			if (maximum == upper[i][j]) {
				m_back[i][j] = DIRECTIONS::right;
			}
			else if (maximum == lower[i][j]) {
				m_back[i][j] = DIRECTIONS::down;
			}
			else
				m_back[i][j] = DIRECTIONS::diagonal;

			middle[i][j] = maximum;
		}
	}
	fprintf(out, "%d\n", middle[n][m]);
	printf("%d\n", upper[n][m]);
	printf("%d\n", lower[n][m]);

	OutputLCS_v(m_back, l_back, u_back, v, n, m, 0, out);
	fprintf(out, "\n");
	OutputLCS_w(m_back, l_back, u_back, w, n, m, 0, out);
	fclose(blosum_file);
	fclose(out);
	fclose(file);
	return 0;
}