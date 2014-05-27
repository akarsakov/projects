#include <cstdlib>
#include <iostream>
#include "utils.h"

using namespace std;

void error(const string& err, const char* func, const char* file, int line)
{
    cout << "Error: " << err << " in " << func << " (" << file << ", line: " << line << ")" << endl;
    exit(EXIT_FAILURE);
}