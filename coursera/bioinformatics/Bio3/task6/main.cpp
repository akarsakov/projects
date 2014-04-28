#include <math.h>
#include <stdio.h>
#include <numeric>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>

using namespace std;

vector<string>* bestMotifs;
int* bestScores;

vector<string> DNA;



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

double getProbability(vector<vector<float>>& profile, string& a)
{
    float p = 1.0;
	for (int i=0; i<a.size(); i++)
	{
		p *= profile[i][str2int(a[i])];
	}
    return p;
}

vector<vector<float>> generateProfile(vector<string>& motif)
{
	vector<vector<float>> profile;
	int k = motif[0].size();
	profile.resize(k);
	
	for (int i=0; i<k; i++)
	{
        profile[i].resize(4, 1.0/ motif.size());
		for (int j=0; j<motif.size(); j++)
		{
			profile[i][str2int(motif[j][i])] += 1.0/((int) motif.size());
		}
	}
	return profile;
}

int score(const vector<string>& motif)
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

vector<string> Motifs(vector<vector<float>>& profile)
{
    int k = profile.size();
    vector<string> motif;
    for (int i=0; i<DNA.size(); i++)
    {
        float maximum = 0;
	    string best = DNA[i].substr(0,k);
	    for (int s = 0; s < DNA[i].size() - k + 1; s++)
	    {
            string cur = DNA[i].substr(s, k);
            float p = getProbability(profile, cur);
		    if (p > maximum)
		    {
			    best = cur;
			    maximum = p;
		    }
	    }
        motif.push_back(best);
    }
    return motif;
}


vector<string> RandomizedMotifSearch(int k, int t)
{
    int size = DNA[0].size();
    vector<string> motif;
    for (int j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        motif.push_back(DNA[j].substr(start, k));
    }
    vector<string> bestMotif = motif;
    int bestScore = score(bestMotif);

    while (true)
    {
        vector<vector<float>> profile = generateProfile(motif);
        motif = Motifs(profile);
        int current_score = score(motif);
        if (current_score < bestScore)
        {
            bestMotif = motif;
            bestScore = current_score;
        }
        else
        {
            return bestMotif;
        }
    }
}

void thread_func(int num, int num_iterations, int k, int t)
{
    int size = DNA[0].size();
    vector<string> bestMotif;
    for (int j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        bestMotif.push_back(DNA[j].substr(start, k));
    }
    int bestScore = score(bestMotif);

    vector<string> motif;

    for (int i=0; i<num_iterations; i++)
    {
        motif = RandomizedMotifSearch(k, t);
        if (score(motif) < bestScore)
        {
            bestMotif = motif;
            bestScore = score(motif);
        }
    }

    bestMotifs[num] = bestMotif;
    bestScores[num] = bestScore;
}

int main()
{
    srand(time(NULL));
	FILE* file = fopen("stepic_dataset.txt", "r");
    FILE* out = fopen("result.txt", "w");
	int k, t;
	char* c_str = new char[1000];
	fscanf(file, "%d %d", &k, &t);

	for (int i=0; i<t; i++)
	{
		fscanf(file, "%s", c_str);
		DNA.push_back(string(c_str));
	}
    int num_threads = 8;

    bestMotifs = new vector<string>[num_threads];
    bestScores = new int[num_threads];

    thread* threads = new thread[num_threads];
    for (int i=0; i<num_threads; i++)
    {
        threads[i] = thread(thread_func, i, 100000/num_threads, k, t);
    }

    for (int i=0; i<num_threads; i++)
    {
        threads[i].join();
    }

    int minScore = INT_MAX;
    int index = 0;
    for (int i=0; i<num_threads; i++)
    {
        if (bestScores[i] < minScore)
        {
            minScore = bestScores[i];
            index = i;
        }
    }

	for (auto i=0; i<bestMotifs[index].size(); i++)
	{
		fprintf(out, "%s\n", bestMotifs[index][i].c_str());
	}
	return 0;
}