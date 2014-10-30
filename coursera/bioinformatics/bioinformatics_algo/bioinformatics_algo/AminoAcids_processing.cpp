#include "AminoAcids_processing.h"
#include "RNA_processing.h"
#include <algorithm>

using namespace std;

static char int2AA[] = { 'K', 'N', 'K', 'N', 'T', 'T', 'T', 'T', 'R', 'S', 'R', 'S', 'I', 'I', 'M', 'I',
                         'Q', 'H', 'Q', 'H', 'P', 'P', 'P', 'P', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'L',
                         'E', 'D', 'E', 'D', 'A', 'A', 'A', 'A', 'G', 'G', 'G', 'G', 'V', 'V', 'V', 'V',
                         '*', 'Y', '*', 'Y', 'S', 'S', 'S', 'S', '*', 'C', 'W', 'C', 'L', 'F', 'L', 'F' };

static int moleculeMasses[] = { 71, 0, 103, 115, 129, 147, 57, 137, 113, 0, 128, 113, 131, 114, 0, 97, 128,
                                156, 87, 101, 0, 99, 186, 0, 163, 0};

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

int bio::getMoleculeMass(char molecule)
{
    int index = molecule - 'A';
    if (index < 0 || index > 25)
        throw exception("invalid molecule");

    int mass = moleculeMasses[index];
    if (mass <= 0)
        throw exception("invalid molecule");

    return mass;
}

int bio::getPeptideMass(string peptide)
{
    int mass = 0;
    for (size_t i=0; i<peptide.size(); i++)
        mass += bio::getMoleculeMass(peptide[i]);

    return mass;
}

namespace bio
{
namespace week2
{

vector<int> getCyclospectrum(string peptide)
{
    string cyclopeptide = peptide + peptide;
    vector<int> result;
    
    result.push_back(0);
    result.push_back(getPeptideMass(peptide));

    for (size_t subpeptideSize=1; subpeptideSize<peptide.size(); subpeptideSize++)
    {
        for (size_t i=0; i<peptide.size(); i++)
        {
            string subpeptide = cyclopeptide.substr(i, subpeptideSize);
            int subpeptideMass = getPeptideMass(subpeptide);

            result.push_back(subpeptideMass);
        }
    }

    sort(result.begin(), result.end());

    return result;
}

} /* week2 */

} /* bio */