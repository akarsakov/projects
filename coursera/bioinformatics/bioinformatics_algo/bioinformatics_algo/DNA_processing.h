#ifndef DNA_PROCESSING
#define DNA_PROCESSING

#include <string>
#include <set>
#include <vector>

namespace bio
{

inline bool isDNA(std::string DNA_string)
{
    std::size_t pos = DNA_string.find_first_not_of("ACGT");
    return pos == std::string::npos;
}

inline char complement_nucleotide(char nucleotide)
{
    switch (nucleotide)
    {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    }
    return 0;
}

inline int nucl2int(char nucl)
{
    switch (nucl)
    {
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
    }
    return 0;
}

char int2nucl(int n);

std::string rev_complement(std::string DNA);
std::vector<std::string> generateAllKMers(int k);

namespace week1
{
    std::set<std::string> FindClumps(std::string DNA, int k, int L, int t);
    std::vector<int> GetSkew(std::string DNA);

    std::set<std::string> ImmediateNeighbors(std::string pattern);
    std::set<std::string> Neighbors(std::string pattern, int d);
} /* week1 */

} /* bio */

#endif /* DNA_PROCESSING */