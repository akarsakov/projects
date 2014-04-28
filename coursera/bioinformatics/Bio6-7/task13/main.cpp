#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

enum DIRECTIONS { ibb = 0, bjb, bbk, ijb, ibk, bjk, diagonal };

void OutputLCS_v(vector<vector<vector<DIRECTIONS>>>& backrack, string v, int i, int j, int t, FILE* out)
{
	if ((i==0) && (j==0) && (t==0))
	{
		return;
	}
	DIRECTIONS dir = backrack[i][j][t];
	if (dir == DIRECTIONS::ibb) {
		OutputLCS_v(backrack, v, i-1, j, t, out);
		fprintf(out, "%c", v[i-1]);
	} else if (dir == DIRECTIONS::bjb) {
		OutputLCS_v(backrack, v, i, j-1, t, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::bbk) {
		OutputLCS_v(backrack, v, i, j, t-1, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::ijb) {
		OutputLCS_v(backrack, v, i-1, j-1, t, out);
		fprintf(out, "%c", v[i-1]);
	} else if (dir == DIRECTIONS::ibk) {
		OutputLCS_v(backrack, v, i-1, j, t-1, out);
		fprintf(out, "%c", v[i-1]);
	} else if (dir == DIRECTIONS::bjk) {
		OutputLCS_v(backrack, v, i, j-1, t-1, out);
		fprintf(out, "%c", '-');
	} else {
		OutputLCS_v(backrack, v, i-1, j-1, t-1, out);
		fprintf(out, "%c", v[i-1]);
	}
}

void OutputLCS_w(vector<vector<vector<DIRECTIONS>>>& backrack, string v, int i, int j, int t, FILE* out)
{
	if ((i==0) && (j==0) && (t==0))
	{
		return;
	}
	DIRECTIONS dir = backrack[i][j][t];
	if (dir == DIRECTIONS::ibb) {
		OutputLCS_w(backrack, v, i-1, j, t, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::bjb) {
		OutputLCS_w(backrack, v, i, j-1, t, out);
		fprintf(out, "%c", v[j-1]);
	} else if (dir == DIRECTIONS::bbk) {
		OutputLCS_w(backrack, v, i, j, t-1, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::ijb) {
		OutputLCS_w(backrack, v, i-1, j-1, t, out);
		fprintf(out, "%c", v[j-1]);
	} else if (dir == DIRECTIONS::ibk) {
		OutputLCS_w(backrack, v, i-1, j, t-1, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::bjk) {
		OutputLCS_w(backrack, v, i, j-1, t-1, out);
		fprintf(out, "%c", v[j-1]);
	} else {
		OutputLCS_w(backrack, v, i-1, j-1, t-1, out);
		fprintf(out, "%c", v[j-1]);
	}
}

void OutputLCS_u(vector<vector<vector<DIRECTIONS>>>& backrack, string v, int i, int j, int t, FILE* out)
{
	if ((i==0) && (j==0) && (t==0))
	{
		return;
	}
	DIRECTIONS dir = backrack[i][j][t];
	if (dir == DIRECTIONS::ibb) {
		OutputLCS_u(backrack, v, i-1, j, t, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::bjb) {
		OutputLCS_u(backrack, v, i, j-1, t, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::bbk) {
		OutputLCS_u(backrack, v, i, j, t-1, out);
		fprintf(out, "%c", v[t-1]);
	} else if (dir == DIRECTIONS::ijb) {
		OutputLCS_u(backrack, v, i-1, j-1, t, out);
		fprintf(out, "%c", '-');
	} else if (dir == DIRECTIONS::ibk) {
		OutputLCS_u(backrack, v, i-1, j, t-1, out);
		fprintf(out, "%c", v[t-1]);
	} else if (dir == DIRECTIONS::bjk) {
		OutputLCS_u(backrack, v, i, j-1, t-1, out);
		fprintf(out, "%c", v[t-1]);
	} else {
		OutputLCS_u(backrack, v, i-1, j-1, t-1, out);
		fprintf(out, "%c", v[t-1]);
	}
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char str_c[4100];
	int indexes[256];
	vector<vector<vector<int>>> s;
	vector<vector<vector<DIRECTIONS>>> backtracks;
	fscanf(file, "%s", str_c);
	string v(str_c);
	fscanf(file, "%s", str_c);
	string w(str_c);
	fscanf(file, "%s", str_c);
	string u(str_c);
	int m = v.size();
	int n = w.size();
	int k = u.size();

	s.resize(m+1);
	for (int i=0; i<m+1; i++)
	{
		s[i].resize(n+1);
		for (int j=0; j<n+1; j++)
			s[i][j].resize(k+1, INT_MIN);
	}

	backtracks.resize(m+1);
	for (int i=0; i<m+1; i++)
	{
		backtracks[i].resize(n+1);
		for (int j=0; j<n+1; j++)
			backtracks[i][j].resize(k+1);
	}
	
	s[0][0][0] = 0;
	for (int i=0; i<m+1; i++)
		for (int j=0; j<n+1; j++)
		{
			s[i][j][0] = 0;
			backtracks[i][j][0] = DIRECTIONS::ijb;
		}

	for (int i=0; i<m+1; i++)
		for (int t=0; t<k+1; t++)
		{
			s[i][0][t] = 0;
			backtracks[i][0][t] = DIRECTIONS::ibk;
		}
	for (int j=0; j<n+1; j++)
		for (int t=0; t<k+1; t++)
		{
			s[0][j][t] = 0;
			backtracks[0][j][t] = DIRECTIONS::bjk;
		}
	for (int i=0; i<m+1; i++)
	{
		s[i][0][0] = 0;
		backtracks[i][0][0] = DIRECTIONS::ibb;
	}
	for (int j=0; j<n+1; j++)
	{
		s[0][j][0] = 0;
		backtracks[0][j][0] = DIRECTIONS::bjb;
	}
	for (int t=0; t<k+1; t++)
	{
		s[0][0][t] = 0;
		backtracks[0][0][t] = DIRECTIONS::bbk;
	}

	for (int i=1; i<m+1; i++)
	{
		for (int j=1; j<n+1; j++)
		{
			for (int t=1; t<k+1; t++)
			{
				int diag = s[i-1][j-1][t-1];
				if ((v[i-1] == w[j-1]) && (w[j-1] == u[t-1]))
					diag = s[i-1][j-1][t-1] + 1;
			
				int maximum = max(diag, max(s[i-1][j][t], s[i][j-1][t]));
				maximum = max(maximum, max(s[i][j][t-1], s[i-1][j-1][t]));
				maximum = max(maximum, max(s[i][j-1][t-1], s[i-1][j][t-1]));
				if (maximum == s[i-1][j][t])
					backtracks[i][j][t] = DIRECTIONS::ibb;
				else if (maximum == s[i][j-1][t])
					backtracks[i][j][t] = DIRECTIONS::bjb;
				else if (maximum == s[i][j][t-1])
					backtracks[i][j][t] = DIRECTIONS::bbk;
				else if (maximum == s[i-1][j-1][t])
					backtracks[i][j][t] = DIRECTIONS::ijb;
				else if (maximum == s[i][j-1][t-1])
					backtracks[i][j][t] = DIRECTIONS::bjk;
				else if (maximum == s[i-1][j][t-1])
					backtracks[i][j][t] = DIRECTIONS::ibk;
				else
					backtracks[i][j][t] = DIRECTIONS::diagonal;

				s[i][j][t] = maximum;
			}
		}
	}
	fprintf(out, "%d\n", s[m][n][k]);
	OutputLCS_v(backtracks, v, m, n, k, out);
	fprintf(out, "\n");
	OutputLCS_w(backtracks, w, m, n, k, out);
	fprintf(out, "\n");
	OutputLCS_u(backtracks, u, m, n, k, out);
	fclose(out);
	fclose(file);
	return 0;
}