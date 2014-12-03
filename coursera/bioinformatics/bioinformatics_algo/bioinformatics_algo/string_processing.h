#ifndef STRING_PROCESSING
#define STRING_PROCESSING

#include <string>
#include <set>
#include <vector>

namespace bio
{

int HammingDistance(std::string a, std::string b);
int distanceText2Pattern(std::string text, std::string pattern);

inline std::string suffix(std::string str)
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

namespace week4
{
    std::vector<std::string> getComposition(std::string text, int k);
    std::string reconstructPath(std::vector<std::string> components);
    std::vector<std::pair<std::string, std::string>> getOverlapPairs(std::vector<std::string> patterns, int k);
} /* week4 */

} /* bio */

#endif /* STRING_PROCESSING */