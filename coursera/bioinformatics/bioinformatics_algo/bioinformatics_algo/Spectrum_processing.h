#ifndef SPECTRUM_PROCESSING
#define SPECTRUM_PROCESSING

#include <string>
#include <vector>

namespace bio
{
bool isEqualSpectrums(std::vector<int> a, std::vector<int> b);
bool isConsistentSpectrums(std::vector<int> a, std::vector<int> b);

std::string peptideToSpectrumString(std::string peptide);
std::vector<int> getCyclospectrum(std::string peptide);

int score(std::string peptide, std::vector<int> spectrum);

namespace week2
{
    std::vector<std::string> cyclopeptideSequencing(std::vector<int> spectrum);

} /* week2 */

} /* bio */

#endif /* SPECTRUM_PROCESSING */