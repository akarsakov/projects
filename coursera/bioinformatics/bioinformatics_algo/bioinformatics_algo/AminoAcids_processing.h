#ifndef AMINO_ACIDS_PROCESSING
#define AMINO_ACIDS_PROCESSING

#include <string>

namespace bio
{

char codon2AA(std::string codon);
std::string RNA2AA(std::string RNA);


} /* bio */

#endif /* AMINO_ACIDS_PROCESSING */