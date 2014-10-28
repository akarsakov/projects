#ifndef RNA_PROCESSING
#define RNA_PROCESSING

#include <string>

namespace bio
{

std::string DNA2RNA(std::string DNA);
bool isRNA(std::string RNA);
int rnucl2int(char rnucl);


} /* bio */


#endif /* RNA_PROCESSING */