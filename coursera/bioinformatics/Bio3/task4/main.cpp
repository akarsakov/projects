#include <math.h>
#include <stdio.h>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int str2int(char a)
{
	switch (a)
	{
	case 'A': return 0;
	case 'C': return 1;
	case 'G': return 2;
	case 'T': return 3;
	}
	return 5;
}

vector<vector<float>> generateProfile(vector<string> motif)
{
	vector<vector<float>> profile;
	int k = motif[0].size();
	profile.resize(k);
	
	for (int i=0; i<k; i++)
	{
		profile[i].resize(4, 0.0);
		for (int j=0; j<motif.size(); j++)
		{
			profile[i][str2int(motif[j][i])] += 1.0/((int) motif.size());
		}
	}
	return profile;
}

int score(vector<string> motif)
{
	int sc = 0;
	for (int i=0; i<motif[0].size(); i++)
	{
		vector<int> nucl;
		nucl.resize(4, 0);
		for (int j=0; j<motif.size(); j++)
		{
			nucl[str2int(motif[j][i])] += 1;
		}
		int max = 0;
		for (int i=0; i<4; i++)
			if (nucl[i] > max)
				max = nucl[i];
		sc += motif.size() - max;
	}
	return sc;
}

double getProbability(vector<vector<float>> profile, string a)
{
    float p = 1.0;
	for (int i=0; i<a.size(); i++)
	{
		p *= profile[i][str2int(a[i])];
	}
    return p;
}

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
    FILE* out = fopen("result.txt", "w");
	int k, t;
	char* c_str = new char[1000];
	fscanf(file, "%d %d", &k, &t);
	vector<string> DNA;
	for (int i=0; i<t; i++)
	{
		fscanf(file, "%s", c_str);
		DNA.push_back(string(c_str));
	}

	vector<string> bestMotif;
	for (int i=0; i<t; i++)
	{
		bestMotif.push_back(DNA[i].substr(0,k));
	}

	

	for (int i=0; i<DNA[0].size() - k + 1; i++)
	{
		vector<string> motif;
		motif.push_back(DNA[0].substr(i, k));
		for (int j=1; j<t; j++)
		{
			vector<vector<float>> profile = generateProfile(motif);
			float maximum = 0;
			string best = DNA[j].substr(0,k);
			for (int s = 0; s < DNA[j].size() - k + 1; s++)
			{
                string cur = DNA[j].substr(s, k);
                float p = getProbability(profile, cur);
				if (p > maximum)
				{
					best = cur;
					maximum = p;
				}
			}
			motif.push_back(best);
		}
		if (score(motif) < score(bestMotif))
		{
			bestMotif = motif;
		}
	}

	for (auto i=0; i<bestMotif.size(); i++)
	{
		fprintf(out, "%s\n", bestMotif[i].c_str());
	}
	return 0;
}