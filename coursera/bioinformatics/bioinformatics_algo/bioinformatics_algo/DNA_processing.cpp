#include <algorithm>

#include "DNA_processing.h"
#include "string_processing.h"
#include "GibbsSampler.h"

using namespace std;
using namespace bio;

static char nucl_map[4] = { 'A', 'C', 'G', 'T' };

inline char bio::int2nucl(int n)
{
    return nucl_map[n];
}

string bio::rev_complement(string DNA)
{
    string result;
    for (auto rit=DNA.rbegin(); rit!=DNA.rend(); rit++)
        result.append(1, complement_nucleotide(*rit));
    return result;
}

static void recursiveGenerate(vector<string>& kmers, string current, int k)
{
    if (current.size() == k)
        kmers.push_back(current);
    else
    {
        for (int i=0; i<4; i++)
            recursiveGenerate(kmers, current + int2nucl(i), k);
    }
}

vector<string> bio::generateAllKMers(int k)
{
    string init;
    vector<string> kmers;
    recursiveGenerate(kmers, init, k);
    return kmers;
}

static bool findWithMismatches(string text, string pattern, int d)
{
    size_t k = pattern.size();
    for (size_t i=0; i<text.size() - k + 1; i++) {
        int dist = HammingDistance(text.substr(i, k), pattern);
        if (dist <= d)
            return true;
    }
    return false;
}

double bio::kMerProbability(const vector<vector<double>>& profile, string pattern)
{
    double probability = 1.0;

    for (size_t i=0; i<pattern.size(); i++)
    {
        probability *= profile[nucl2int(pattern[i])][i];
    }
    return probability;
}

vector<vector<double>> bio::ProfileGenerator::generateProfile(const vector<string>& motif)
{
    vector<vector<double>> profile;
	size_t k = motif[0].size();
	profile.resize(4);

    for (int i=0; i<4; i++)
        profile[i].resize(k, 0.0);
	
	for (size_t i=0; i<k; i++)
	{
		for (size_t j=0; j<motif.size(); j++)
		{
            profile[nucl2int(motif[j][i])][i] += 1.0/((int) motif.size());
		}
	}
	return profile;
}

vector<vector<double>> bio::LaplaceProfileGenerator::generateProfile(const vector<string>& motif)
{
    vector<vector<double>> profile;
	size_t k = motif[0].size();
	profile.resize(4);

    for (int i=0; i<4; i++)
        profile[i].resize(k, 1.0/(2 * motif.size()));
	
	for (size_t i=0; i<k; i++)
	{
		for (size_t j=0; j<motif.size(); j++)
		{
            profile[nucl2int(motif[j][i])][i] += 1.0/(2 * motif.size());
		}
	}
	return profile;
}

string bio::getConsensus(const vector<vector<double>>& profile)
{
    size_t k = profile[0].size();
    string consensus;

    for (size_t i=0; i<k; i++)
    {
        int max_nuc_index;
        double max_prob = 0.;

        for (int j=0; j<4; j++)
        {
            if (profile[j][i] > max_prob) 
            {
                max_prob = profile[j][i];
                max_nuc_index = j;
            }
        }
        consensus += int2nucl(max_nuc_index);
    }
    return consensus;
}

int bio::motifScore(const vector<string>& motif)
{
    int score = 0;
	for (size_t i=0; i<motif[0].size(); i++)
	{
		vector<int> nucl_counter;
		nucl_counter.resize(4, 0);
		for (size_t j=0; j<motif.size(); j++)
		{
            nucl_counter[nucl2int(motif[j][i])] += 1;
		}
        int max_nucl = *max_element(nucl_counter.begin(), nucl_counter.end()); 
		score += motif.size() - max_nucl;
	}
	return score;
}

vector<string> bio::generateMotif(const vector<string>& DNA, const vector<vector<double>>& profile)
{
    size_t k = profile[0].size();
    size_t t = DNA.size();
    vector<string> motif;

    for (size_t i=0; i<t; i++)
    {
        double max_prob = 0.0;
	    string best_kMer = DNA[i].substr(0,k);

	    for (size_t s = 0; s < DNA[i].size() - k + 1; s++)
	    {
            string cur_kMer = DNA[i].substr(s, k);
            double cur_prob = kMerProbability(profile, cur_kMer);
		    if (cur_prob > max_prob)
		    {
			    max_prob = cur_prob;
			    best_kMer = cur_kMer;
		    }
	    }
        motif.push_back(best_kMer);
    }
    return motif;
}

static vector<string> randomGenerateMotif(const vector<string>& DNA, int k, ProfileGenerator* generator)
{
    size_t size = DNA[0].size();
    size_t t = DNA.size();
    vector<string> motif;

    for (size_t j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        motif.push_back(DNA[j].substr(start, k));
    }
    vector<string> bestMotif = motif;
    int bestScore = bio::motifScore(bestMotif);

    while (true)
    {
        vector<vector<double>> profile = generator->generateProfile(motif);
        motif = bio::generateMotif(DNA, profile);
        int current_score = bio::motifScore(motif);
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

static vector<string> GibbsGenerateMotif(const vector<string>& DNA, int k, int N, ProfileGenerator* generator)
{
    int size = DNA[0].size();
    const size_t t = DNA.size();
    vector<string> bestMotif;
    for (size_t j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        bestMotif.push_back(DNA[j].substr(start, k));
    }
    int bestScore = bio::motifScore(bestMotif);

    for (int j=0; j<N; j++)
    {
        int motif_index = rand() % t;
        vector<string> curMotif = bestMotif;
        curMotif.erase(curMotif.begin() + motif_index);

        vector<vector<double>> curProfile = generator->generateProfile(curMotif);

        vector<double> probs;
        for (size_t s=0; s<DNA[motif_index].size() - k + 1; s++)
        {
            probs.push_back(bio::kMerProbability(curProfile, DNA[motif_index].substr(s, k)));
        }

        GibbsSampler sampler(probs);
        int kMerIndex = sampler.sample();
        string kMer = DNA[motif_index].substr(kMerIndex, k);

        curMotif.insert(curMotif.begin() + motif_index, kMer);
        int curScore = bio::motifScore(curMotif);

        if (curScore < bestScore)
        {
            bestScore = curScore;
            bestMotif = curMotif;
        }
    }
    return bestMotif;
}

namespace bio
{
namespace week1
{

set<string> FindClumps(string DNA, int k, int L, int t)
{
    set<string> clumps;
    int DNA_length = DNA.size();

    for (int i=0; i<DNA_length-k; i++)
    {
        string clump = DNA.substr(i, k);

        if (clumps.find(clump) == clumps.end())
        {
            size_t pos = i + 1;
            int count = 1;

            while (true)
            {
                pos = DNA.find(clump, pos+1);
                if (pos != string::npos && (int) pos < i+L-k)
                    count++;
                else
                    break;
            }

            if (count >= t)
                clumps.insert(clump);
        }
    }
    return clumps;
}

vector<int> GetSkew(string DNA)
{
    vector<int> skew;
    int cur_skew = 0;

    for (auto it=DNA.begin(); it!=DNA.end(); it++)
    {
        skew.push_back(cur_skew);
        if (*it == 'C') cur_skew--;
        if (*it == 'G') cur_skew++;
    }
    skew.push_back(cur_skew);

    return skew;
}

set<string> ImmediateNeighbors(string pattern)
{
    set<string> neighborhood;
    neighborhood.insert(pattern);

    for (size_t i=0; i<pattern.size(); i++)
    {
        int cur_nucl = nucl2int(pattern[i]);
        for (int j=0; j<3; j++)
        {
            string new_neighbor = pattern;
            new_neighbor[i] = int2nucl((cur_nucl + j + 1) % 4);
            neighborhood.insert(new_neighbor);
        }
    }
    return neighborhood;
}

set<string> Neighbors(string pattern, int d)
{
    set<string> neighborhood;
    if (d == 0)
    {
        neighborhood.insert(pattern);
    } else if (pattern.size() == 1)
    {
        string result[4] = { "A", "C", "G", "T" };
        neighborhood.insert(result, result+4);
    } else
    {
        string suf_pattern = suffix(pattern);
        set<string> suf_neighbors = Neighbors(suf_pattern, d);
        for (auto it=suf_neighbors.begin(); it!=suf_neighbors.end(); it++)
        {
            if (HammingDistance(suf_pattern, *it) < d)
            {
                for (int j=0; j<4; j++)
                    neighborhood.insert(int2nucl(j) + *it);
            }
            else
            {
                neighborhood.insert(pattern[0] + *it);
            }
        }
    }
    return neighborhood;
}

} /* week1 */

namespace week3
{
set<string> motifEnumeration(const vector<string>& DNA, int k, int d)
{
    set<string> patterns;
    vector<string> kMers = generateAllKMers(k);

    for (auto kMer : kMers)
    {
        bool allFound = true;
        for (size_t j=0; j<DNA.size(); j++) {
            if (!findWithMismatches(DNA[j], kMer, d)) {
                allFound = false;
                break;
            }
        }

        if (allFound)
            patterns.insert(kMer);
    }
    
    return patterns;
}

int distanceDNA2Pattern(const vector<string>& DNA, string pattern)
{
    int dist = 0;
    for (auto text : DNA)
    {
        dist += distanceText2Pattern(text, pattern);
    }
    return dist;
}

string medianString(const vector<string>& DNA, int k)
{
    int dist = INT_MAX;
    string median;
    vector<string> kMers = generateAllKMers(k);

    for (auto kMer : kMers) 
    {
        int curDist = distanceDNA2Pattern(DNA, kMer);
        if (dist > curDist)
        {
            dist = curDist;
            median = kMer;
        }
    }
    return median;
}

vector<string> greedyMotifSearch(const vector<string>& DNA, int k, ProfileGenerator* generator)
{
    vector<string> bestMotif;
    size_t t = DNA.size();
    for (auto text : DNA) {
        bestMotif.push_back(text.substr(0, k));
    }

    for (size_t i=0; i<DNA[0].size() - k + 1; i++)
    {
        vector<string> motif;
        motif.push_back(DNA[0].substr(i, k));

        for (size_t j=1; j<t; j++)
        {
            vector<vector<double>> profile = generator->generateProfile(motif);
            double maxProb = 0.;
            string bestkMer = DNA[j].substr(0, k);

            for (size_t s=0; s < DNA[j].size() - k + 1; s++)
            {
                string curkMer = DNA[j].substr(s, k);
                double curProb = kMerProbability(profile, curkMer);

                if (curProb > maxProb)
                {
                    maxProb = curProb;
                    bestkMer = curkMer;
                }
            }

            motif.push_back(bestkMer);
        }

        if (motifScore(bestMotif) > motifScore(motif))
        {
            bestMotif = motif;
        }
    }
    return bestMotif;
}

vector<string> randomizedMotifSearch(const vector<string>& DNA, int k, int num_iterations, ProfileGenerator* generator)
{
    int size = DNA[0].size();
    const size_t t = DNA.size();
    vector<string> bestMotif;
    for (size_t j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        bestMotif.push_back(DNA[j].substr(start, k));
    }
    int bestScore = motifScore(bestMotif);

    vector<string> motif;
    for (int i=0; i<num_iterations; i++)
    {
        motif = randomGenerateMotif(DNA, k, generator);
        if (motifScore(motif) < bestScore)
        {
            bestMotif = motif;
            bestScore = motifScore(motif);
        }
    }
    return bestMotif;
}

vector<string> GibbsMotifSearch(const vector<string>& DNA, int k, int N, int num_iterations, ProfileGenerator* generator)
{
    int size = DNA[0].size();
    const size_t t = DNA.size();
    vector<string> bestMotif;
    for (size_t j=0; j<t; j++)
    {
        int start = rand() % (size - k + 1);
        bestMotif.push_back(DNA[j].substr(start, k));
    }
    int bestScore = motifScore(bestMotif);

    vector<string> motif;
    for (int i=0; i<num_iterations; i++)
    {
        motif = GibbsGenerateMotif(DNA, k, N, generator);
        if (motifScore(motif) < bestScore)
        {
            bestMotif = motif;
            bestScore = motifScore(motif);
        }
    }
    return bestMotif;
}

} /* week3 */

} /* bio */