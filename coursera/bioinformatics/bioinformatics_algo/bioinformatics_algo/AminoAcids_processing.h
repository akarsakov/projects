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

namespace week2
{
    std::vector<int> getCyclospectrum(std::string peptide);
    std::vector<int> getMassesVector();

} /* week2 */


} /* bio */

#endif /* AMINO_ACIDS_PROCESSING */