#include "Spectrum_processing.h"
#include "AminoAcids_processing.h"
#include <algorithm>

using namespace std;

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

vector<int> bio::getCyclospectrum(string peptide)
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

vector<int> bio::getLinearSpectrum(string peptide)
{
    vector<int> linearSpectrum;
    vector<int> prefixMass(peptide.size() + 1, 0);

    for (size_t i=0; i<peptide.size(); i++)
    {
        prefixMass[i
    }

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

int bio::score(string peptide, vector<int> spectrum)
{
    vector<int> peptideSpectrum = getCyclospectrum(peptide);
    vector<int> intersection;
    set_intersection(peptideSpectrum.begin(), peptideSpectrum.end(), spectrum.begin(), spectrum.end(), 
                     back_inserter(intersection));

    return intersection.size();
}

int bio::linearScore(string peptide, vector<int> spectrum)
{

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
