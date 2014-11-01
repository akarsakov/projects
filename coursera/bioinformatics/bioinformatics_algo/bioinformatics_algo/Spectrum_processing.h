#ifndef SPECTRUM_PROCESSING
#define SPECTRUM_PROCESSING

#include <string>
#include <vector>

namespace bio
{
bool isEqualSpectrums(const std::vector<int>& a, const std::vector<int>& b);
bool isConsistentSpectrums(const std::vector<int>& a, const std::vector<int>& b);

std::string peptideToSpectrumString(const std::string& peptide);
std::vector<int> getCyclospectrum(const std::string& peptide);
std::vector<int> getLinearSpectrum(const std::string& peptide);

int score(const std::string& peptide, const std::vector<int>& spectrum);
int linearScore(const std::string& peptide, const std::vector<int>& spectrum);

namespace week2
{
    std::vector<std::string> cyclopeptideSequencing(const std::vector<int>& spectrum);
    std::string leaderBoardCyclopeptideSequincing(const std::vector<int>& spectrum, int N);
    std::vector<std::string> leaderBoardCyclopeptideSequincing2(const std::vector<int>& spectrum, int N);
    std::vector<std::string> trimLeaderBoard(const std::vector<std::string>& leaderBoard, const std::vector<int>& spectrum, int N);

} /* week2 */

} /* bio */

#endif /* SPECTRUM_PROCESSING */