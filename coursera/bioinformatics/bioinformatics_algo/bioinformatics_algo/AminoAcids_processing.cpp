#include "AminoAcids_processing.h"
#include "RNA_processing.h"
#include <algorithm>

using namespace std;

static char int2AA[] = { 'K', 'N', 'K', 'N', 'T', 'T', 'T', 'T', 'R', 'S', 'R', 'S', 'I', 'I', 'M', 'I',
                         'Q', 'H', 'Q', 'H', 'P', 'P', 'P', 'P', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'L',
                         'E', 'D', 'E', 'D', 'A', 'A', 'A', 'A', 'G', 'G', 'G', 'G', 'V', 'V', 'V', 'V',
                         '*', 'Y', '*', 'Y', 'S', 'S', 'S', 'S', '*', 'C', 'W', 'C', 'L', 'F', 'L', 'F' };

static int moleculeMasses[] = { 71, 0, 103, 115, 129, 147, 57, 137, 113, 0, 128, 113, 131, 114, 0, 97, 128,
                                156, 87, 101, 0, 99, 186, 0, 163, 0 };

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

bool bio::isEqualSpectrums(vector<int> a, vector<int> b)
{
    if (a.size() != b.size())
        return false;

    return equal(a.begin(), a.end(), b.begin());
}

bool bio::isConsistentSpectrums(vector<int> big, vector<int> small)
{
    return includes(big.begin(), big.end(), small.begin(), small.end());
}

string bio::peptideToSpectrumString(string peptide)
{
    string spectrumString;
    for (size_t i=0; i<peptide.size(); i++)
    {
        if (i == 0)
            spectrumString += to_string(getMoleculeMass(peptide[i]));
        else
            spectrumString += "-" + to_string(getMoleculeMass(peptide[i]));
    }
    return spectrumString;
}

static vector<string> expandPeptides(vector<string> peptides)
{
    static char acidsToExpand[] = { 'G', 'A', 'S', 'P', 'V', 'T', 'C', 'I', 'N', 'D', 'K', 'E', 'M', 'H', 'F', 'R', 'Y', 'W' };
    static int acidsSize = sizeof(acidsToExpand) / sizeof(char);
    vector<string> expanded;

    for (auto it=peptides.begin(); it!=peptides.end(); it++)
    {
        for (int i=0; i<acidsSize; i++)
        {
            expanded.push_back(*it + acidsToExpand[i]);
        }
    }

    return expanded;
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

vector<int> getMassesVector()
{
    vector<int> masses(moleculeMasses, moleculeMasses + sizeof(moleculeMasses) / sizeof(int));
    auto end = remove_if(masses.begin(), masses.end(), 
                    [](const int& el) -> bool 
                    {
                        return el == 0;
                    });
    masses.erase(end, masses.end());
    sort(masses.begin(), masses.end());
    masses.erase( unique( masses.begin(), masses.end() ), masses.end() );
    return masses;
}

vector<string> cyclopeptideSequencing(vector<int> spectrum)
{
    vector<string> result;

    vector<string> peptides;
    peptides.push_back("");

    int parentMass = *max_element(spectrum.begin(), spectrum.end());

    while (!peptides.empty())
    {
        vector<string> exPeptides = expandPeptides(peptides);
        peptides.clear();

        for (auto it=exPeptides.begin(); it!=exPeptides.end(); it++)
        {
            if (getPeptideMass(*it) == parentMass)
            {
                if (isEqualSpectrums(getCyclospectrum(*it), spectrum))
                {
                    result.push_back(*it);
                }
                continue;
            }
            else 
            {
                vector<int> curSpectrum = getCyclospectrum(*it);
                if (!isConsistentSpectrums(spectrum, curSpectrum))
                    continue;
            }

            peptides.push_back(*it);
        }
    }

    return result;
}

} /* week2 */

} /* bio */