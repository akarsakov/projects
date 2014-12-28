#include "string_processing.h"

#include <vector>
#include <algorithm>

#include "DNA_processing.h"
#include "kMers_generator.h"

using namespace std;
using namespace bio;

int bio::HammingDistance(string a, string b)
{
    if (a.size() != b.size())
        return -1;

    int distance = 0;
    for (size_t i=0; i<a.size(); i++)
        if (a[i] != b[i])
            distance++;
    return distance;
}

namespace bio
{

int distanceText2Pattern(string text, string pattern)
{
    int distance = INT_MAX;
    size_t k = pattern.size();
    for (size_t i = 0; i<text.size() - k + 1; i++)
    {
        distance = std::min(distance, HammingDistance(pattern, text.substr(i, k)));
    }
    return distance;
}

namespace week1
{

int PatternCount(string text, string pattern)
{
    size_t pos = 0;
    int count = 0;
    do
    {
        pos = text.find(pattern, pos);
        if (pos != string::npos)
        {
            count++;
            pos++;
        }
    } while (pos != string::npos);
    return count;
}

int ApproximatePatternCount(string text, string pattern, int d)
{
    int count = 0;
    int text_length = text.size();
    int pattern_length = pattern.size();

    for (int i=0; i<text_length-pattern_length+1; i++)
    {
        string cur = text.substr(i, pattern_length);
        if (HammingDistance(cur, pattern) <= d)
            count++;
    }
    return count;
}

set<string> FrequentWords(string text, int k)
{
    set<string> result;
    int text_length = text.size();
    vector<int> count(text_length - k + 1, 0);
    string pattern;
    int max_count = 0;

    for (int i=0; i<text_length-k+1; i++)
    {
        pattern = text.substr(i, k);
        count[i] = PatternCount(text, pattern);
        max_count = max(max_count, count[i]);
    }

    for (int i=0; i<text_length-k; i++)
    {
        if (count[i] == max_count)
            result.insert(text.substr(i, k));
    }
    return result;
}

set<string> ApproximateFrequentWords(string text, int k, int d)
{
    set<string> result;
    int text_length = text.size();
    vector<int> count(text_length - k + 1, 0);
    string pattern;
    int max_count = 0;

    for (int i=0; i<text_length-k+1; i++)
    {
        pattern = text.substr(i, k);
        count[i] = ApproximatePatternCount(text, pattern, d);
        max_count = max(max_count, count[i]);
    }

    for (int i=0; i<text_length-k; i++)
    {
        if (count[i] == max_count)
            result.insert(text.substr(i, k));
    }
    return result;
}

set<string> ApproximateFrequentWordsWithReverse(string text, int k, int d)
{
    set<string> result;
    kMersGenerator generator(k);
    string pattern, rev_pattern;
    int count, max_count = 0;

    for (auto it=generator.begin(); it!=generator.end(); it++)
    {
        pattern = *it;
        rev_pattern = rev_complement(pattern);
        count = ApproximatePatternCount(text, pattern, d) + ApproximatePatternCount(text, rev_pattern, d);
        if (count > max_count)
        {
            max_count = count;
            result.clear();
            result.insert(pattern);
        } else if (count == max_count)
        {
            result.insert(pattern);
        }
    }
    return result;
}

vector<int> AllOccurrences(string text, string pattern)
{
    size_t pos = 0;
    vector<int> occurrences;

    while (true)
    {
        pos = text.find(pattern, pos+1);
        if (pos != string::npos)
            occurrences.push_back((int) pos);
        else
            break;
    }
    return occurrences;
}

vector<int> AllApproximateOccurences(string text, string pattern, int d)
{
    int text_length = text.size();
    int pattern_length = pattern.size();
    vector<int> occurrences;

    for (int i=0; i<text_length-pattern_length+1; i++)
    {
        string cur = text.substr(i, pattern_length);
        if (HammingDistance(cur, pattern) <= d)
            occurrences.push_back(i);
    }
    return occurrences;
}

vector<int> ComputeFrequencies(string text, int k)
{
    int array_size = (int) pow(4, k);
    int text_size = text.size();
    vector<int> freq_array(array_size, 0);

    for (int i=0; i<text_size - k + 1; i++)
    {
        string pattern = text.substr(i, k);
        int ind = pattern2number(pattern);
        freq_array[ind]++;
    }
    return freq_array;
}

} /* week1 */

namespace week4 
{

vector<string> getComposition(string text, int k)
{
    vector<string> kMers;
	for (size_t i=0; i < text.size() - k + 1; i++)
	{
		string temp = text.substr(i, k);
		kMers.push_back(temp);
	}

	sort(kMers.begin(), kMers.end());

    return kMers;
}

string reconstructPath(vector<string> components)
{
    string path = components[0];
    int k = components[0].size();

    for (size_t i=1; i<components.size(); i++) {
        path += components[i][k-1];
    }

    return path;
}

vector<pair<string, string>> getOverlapPairs(vector<string> patterns, int k) 
{
    vector<pair<string, string>> result;
    for (size_t i=0; i<patterns.size(); i++)
        for (size_t j=0; j<patterns.size(); j++) {
            if (i == j)
                continue;

            string start = patterns[i].substr(patterns[i].size() - k, k);
            string end = patterns[j].substr(0, k);

            if (start == end)
                result.push_back(make_pair(patterns[i], patterns[j]));
        }
    return result;
}

} /* week 4 */

namespace week5 
{
 
static void constructLCS(vector<vector<DIRECTIONS>>& backtrack, string v, int i, int j, string& out)
{
	if ((i==0) || (j==0))
		return;

	if (backtrack[i][j] == DIRECTIONS::DOWN)
		constructLCS(backtrack, v, i-1, j, out);
	else if (backtrack[i][j] == DIRECTIONS::RIGHT)
		constructLCS(backtrack, v, i, j-1, out);
	else {
		constructLCS(backtrack, v, i-1, j-1, out);
		out += v[j-1];
	}
}

string getLCS(const string& v, const string& w) {
    size_t m = v.size();
	size_t n = w.size();
    vector<vector<int>> s;
    vector<vector<DIRECTIONS>> backtracks;

	s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
    for (size_t i=0; i<n+1; i++) {
		s[i][0] = 0;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=0; j<m+1; j++) {
		s[0][j] = 0;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int diag = s[i-1][j-1];
			if (v[j-1] == w[i-1])
				diag = s[i-1][j-1] + 1;
			
			int maximum = max(diag, max(s[i-1][j], s[i][j-1]));
			if (maximum == s[i][j-1])
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == s[i-1][j])
				backtracks[i][j] = DIRECTIONS::DOWN;
			else
                backtracks[i][j] = DIRECTIONS::DIAGONAL;

			s[i][j] = maximum;
		}
	}

    string LCS;
    constructLCS(backtracks, v, n, m, LCS);
    return LCS;
}

int getEditDistance(const string& v, const string& w, const int sigma) {
    vector<vector<int>> s;
    size_t m = v.size();
	size_t n = w.size();

    s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);
	
	s[0][0] = 0;
	for (size_t i=1; i<n+1; i++) {
		s[i][0] = s[i-1][0] - sigma;
	}

	for (size_t j=1; j<m+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int diag = v[j-1] == w[i-1] ? s[i-1][j-1] : s[i-1][j-1] - sigma;
			s[i][j] = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
		}
	}

    return -s[n][m];
}

} /* week5 */

} /* bio */