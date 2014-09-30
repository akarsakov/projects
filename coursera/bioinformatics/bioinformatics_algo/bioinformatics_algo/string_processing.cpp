#include "string_processing.h"

#include <vector>
#include "DNA_processing.h"
#include "kMers_generator.h"
using namespace std;

int HammingDistance(string a, string b)
{
    if (a.size() != b.size())
        return -1;

    int distance = 0;
    for (size_t i=0; i<a.size(); i++)
        if (a[i] != b[i])
            distance++;
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
        if (pattern == "AGCGCCGCT")
        {
            printf("count = %d\n", count);
        }
    }
    printf("max count = %d\n", max_count);
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