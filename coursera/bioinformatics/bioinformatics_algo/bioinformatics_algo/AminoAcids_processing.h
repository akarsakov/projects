#ifndef AMINO_ACIDS_PROCESSING
#define AMINO_ACIDS_PROCESSING

#include <string>
#include <vector>

namespace bio
{

char codon2AA(std::string codon);
std::string RNA2AA(std::string RNA);
int getMoleculeMass(char molecule);
int getPeptideMass(std::string);
bool isEqualSpectrums(std::vector<int> a, std::vector<int> b);
bool isConsistentSpectrums(std::vector<int> a, std::vector<int> b);
std::string peptideToSpectrumString(std::string peptide);

namespace week2
{
    std::vector<int> getCyclospectrum(std::string peptide);
    std::vector<int> getMassesVector();
    std::vector<std::string> cyclopeptideSequencing(std::vector<int> spectrum);

} /* week2 */


} /* bio */

#endif /* AMINO_ACIDS_PROCESSING */