#include "RNA_processing.h"
#include "DNA_processing.h"

#include <algorithm>

using namespace std;

string bio::DNA2RNA(string DNA)
{
    string RNA = DNA;
    replace(RNA.begin(), RNA.end(), 'T', 'U');
    return RNA;
}

bool bio::isRNA(string RNA)
{
    size_t pos = RNA.find_first_not_of("ACGU");
    return pos == std::string::npos;
}

int bio::rnucl2int(char rnucl)
{
    switch (rnucl)
    {
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'U': return 3;
    }

    throw exception("invalid ribonucleotide");
}

