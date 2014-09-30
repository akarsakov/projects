#include "DNA_processing.h"
#include "string_processing.h"

using namespace std;

static char nucl_map[4] = { 'A', 'C', 'G', 'T' };
inline char int2nucl(int n)
{
    return nucl_map[n];
}

string rev_complement(string DNA)
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

vector<string> generateAllKMers(int k)
{
    string init;
    vector<string> kmers;
    recursiveGenerate(kmers, init, k);
    return kmers;
}

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

    for (int i=0; i<pattern.size(); i++)
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