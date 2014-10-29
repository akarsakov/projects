#include "AminoAcids_processing.h"
#include "RNA_processing.h"

using namespace std;

static char int2AA[] = { 'K', 'N', 'K', 'N', 'T', 'T', 'T', 'T', 'R', 'S', 'R', 'S', 'I', 'I', 'M', 'I',
                         'Q', 'H', 'Q', 'H', 'P', 'P', 'P', 'P', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'L',
                         'E', 'D', 'E', 'D', 'A', 'A', 'A', 'A', 'G', 'G', 'G', 'G', 'V', 'V', 'V', 'V',
                         '*', 'Y', '*', 'Y', 'S', 'S', 'S', 'S', '*', 'C', 'W', 'C', 'L', 'F', 'L', 'F' };

char bio::codon2AA(string codon)
{
    if (codon.size() != 3 || !isRNA(codon))
        throw exception("invalid codon");

    int index = rnucl2int(codon[0]) * 16 + rnucl2int(codon[1]) * 4 + rnucl2int(codon[2]);
    return int2AA[index];
}

string bio::RNA2AA(string RNA)
{
    string AA_string = "";
    for (size_t i=0; i<RNA.size() - 2; i+=3)
    {
        string codon = RNA.substr(i, 3);
        char AA = codon2AA(codon);

        if (AA != '*')
            AA_string += AA;
        else
            break;
    }
    return AA_string;
}