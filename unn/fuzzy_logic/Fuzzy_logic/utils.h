#ifndef __UTILS__
#define __UTILS__

#include <string>

#define Error( msg ) error( msg, __FUNCTION__, __FILE__, __LINE__ )

void error(const std::string& err, const char* func, const char* file, int line);

#endif /*__UTILS__*/