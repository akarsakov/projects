#ifndef AMINO_ACIDS_PROCESSING
#define AMINO_ACIDS_PROCESSING

#include <string>
#include <vector>
#include <map>

#define BLOSUM_FILE "..\\extra_data\\BLOSUM62.txt"
#define PAM250_FILE "..\\extra_data\\PAM250_1.txt"

namespace bio
{

char codon2AA(std::string codon);
std::string RNA2AA(std::string RNA);
int getMoleculeMass(char molecule);
int getPeptideMass(std::string);

std::map<std::pair<char, char>, int> readScoringMatrix(const std::string filename);
std::map<std::pair<char, char>, int> generateScoringMatrix(const int sigma);

namespace week2
{
    std::vector<int> getMassesVector();
} /* week2 */

namespace week5 
{
    int globalAlignment(std::string& v, std::string& w, const std::map<std::pair<char, char>, int>& blosum_map, const int sigma);
    int localAlignment(std::string& v, std::string& w, const std::map<std::pair<char, char>, int>& pam250_map, const int sigma);
    int fittingAlignment(std::string& v, std::string& w, const int sigma);
    int overlapAlignment(std::string& v, std::string& w, const int sigma);
    int alignmentWithAffineGapPenalties(std::string& v, std::string& w, const std::map<std::pair<char, char>, int>& blosum_map, 
                                        const int gap_open, const int gap_extension);
    void alignmentMiddleEdge(const std::string& v, const std::string& w, const int sigma, const std::map<std::pair<char, char>, int>& blosum_map,
                             std::pair<int, int>& start, std::pair<int, int>& end);
} /* week5 */

} /* bio */

#endif /* AMINO_ACIDS_PROCESSING */