#ifndef STRING_PROCESSING
#define STRING_PROCESSING

#include <string>
#include <set>
#include <vector>

int HammingDistance(std::string a, std::string b);

inline string suffix(string str)
{
    return str.substr(1);
}

namespace week1
{
    int PatternCount(std::string text, std::string pattern);
    std::set<std::string> FrequentWords(std::string text, int k);
    std::vector<int> AllOccurrences(std::string text, std::string pattern);

    // approximate methods
    int ApproximatePatternCount(std::string text, std::string pattern, int d);
    std::set<std::string> ApproximateFrequentWords(std::string text, int k, int d);
    std::set<std::string> ApproximateFrequentWordsWithReverse(std::string text, int k, int d);
    std::vector<int> AllApproximateOccurences(std::string text, std::string pattern, int d);

    std::vector<int> ComputeFrequencies(std::string text, int k);
} /* week1 */

#endif /* STRING_PROCESSING */