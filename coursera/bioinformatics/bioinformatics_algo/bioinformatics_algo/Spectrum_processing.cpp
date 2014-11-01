#include "Spectrum_processing.h"
#include "AminoAcids_processing.h"
#include <algorithm>

using namespace std;

bool bio::isEqualSpectrums(const vector<int>& a, const vector<int>& b)
{
    if (a.size() != b.size())
        return false;

    return equal(a.begin(), a.end(), b.begin());
}

bool bio::isConsistentSpectrums(const vector<int>& big, const vector<int>& small)
{
    return includes(big.begin(), big.end(), small.begin(), small.end());
}

vector<int> bio::getCyclospectrum(const string& peptide)
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

vector<int> bio::getLinearSpectrum(const string& peptide)
{
    vector<int> linearSpectrum;
    vector<int> prefixMass(peptide.size() + 1, 0);

    for (size_t i=1; i<peptide.size()+1; i++)
    {
        prefixMass[i] = prefixMass[i-1] + getMoleculeMass(peptide[i-1]);
    }
    linearSpectrum.push_back(0);
    for (size_t i=0; i<peptide.size(); i++)
    {
        for (size_t j=i+1; j<peptide.size()+1; j++)
        {
            linearSpectrum.push_back(prefixMass[j] - prefixMass[i]);
        }
    }

    sort(linearSpectrum.begin(), linearSpectrum.end());

    return linearSpectrum;
}

string bio::peptideToSpectrumString(const string& peptide)
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

int bio::score(const string& peptide, const vector<int>& spectrum)
{
    vector<int> peptideSpectrum = getCyclospectrum(peptide);
    vector<int> intersection;
    set_intersection(peptideSpectrum.begin(), peptideSpectrum.end(), spectrum.begin(), spectrum.end(), 
                     back_inserter(intersection));

    return intersection.size();
}

int bio::linearScore(const string& peptide, const vector<int>& spectrum)
{
    vector<int> peptideSpectrum = getLinearSpectrum(peptide);
    vector<int> intersection;
    set_intersection(peptideSpectrum.begin(), peptideSpectrum.end(), spectrum.begin(), spectrum.end(), 
                     back_inserter(intersection));

    return intersection.size();
}

static vector<string> expandPeptides(const vector<string>& peptides)
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

vector<string> cyclopeptideSequencing(const vector<int>& spectrum)
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

vector<string> trimLeaderBoard(const vector<string>& leaderBoard, const vector<int>& spectrum, int N)
{
    if (leaderBoard.empty())
        return leaderBoard;

    vector<pair<int, string>> scoreBoard(leaderBoard.size());
    for (size_t i=0; i<leaderBoard.size(); i++)
    {
        scoreBoard[i] = make_pair(linearScore(leaderBoard[i], spectrum), leaderBoard[i]);
    }

    sort(scoreBoard.begin(), scoreBoard.end(), 
        [=] (pair<int, string> a, pair<int, string> b) -> bool {
            return a.first > b.first;        
    });

    size_t index = min((size_t) N, leaderBoard.size());
    for (; index<leaderBoard.size(); index++)
    {
        if (scoreBoard[index].first < scoreBoard[N-1].first)
            break;
    }

    vector<string> result;
    for (size_t i=0; i<index; i++)
    {
        result.push_back(scoreBoard[i].second);
    }
    return result;
}

string leaderBoardCyclopeptideSequincing(const vector<int>& spectrum, int N)
{
    vector<string> leaderBoard;
    leaderBoard.push_back("");
    
    string leader = "";
    int leaderScore = 0;

    int parentMass = *max_element(spectrum.begin(), spectrum.end());

    while (!leaderBoard.empty())
    {
        vector<string> newLeaderBoard = expandPeptides(leaderBoard);
        leaderBoard.clear();

        for (auto it=newLeaderBoard.begin(); it!=newLeaderBoard.end(); it++)
        {
            int curMass = getPeptideMass(*it);
            if (curMass == parentMass)
            {
                int curScore = score(*it, spectrum);
                if (curScore > leaderScore)
                {
                    leaderScore = curScore;
                    leader = *it;
                }
            } 
            else if (curMass > parentMass)
                continue;
            
            leaderBoard.push_back(*it);
        }

        leaderBoard = trimLeaderBoard(leaderBoard, spectrum, N);
    }
    return leader;
}

vector<string> leaderBoardCyclopeptideSequincing2(const vector<int>& spectrum, int N)
{
    vector<string> leaderBoard;
    leaderBoard.push_back("");
    
    vector<string> leaders;
    int leaderScore = 0;

    int parentMass = *max_element(spectrum.begin(), spectrum.end());

    while (!leaderBoard.empty())
    {
        vector<string> newLeaderBoard = expandPeptides(leaderBoard);
        leaderBoard.clear();

        for (auto it=newLeaderBoard.begin(); it!=newLeaderBoard.end(); it++)
        {
            int curMass = getPeptideMass(*it);
            if (curMass == parentMass)
            {
                int curScore = score(*it, spectrum);
                if (curScore > leaderScore)
                {
                    leaderScore = curScore;
                    leaders.clear();
                    leaders.push_back(*it);
                }
                else if (curScore == leaderScore)
                {
                    leaders.push_back(*it);
                }
            } 
            else if (curMass > parentMass)
                continue;
            
            leaderBoard.push_back(*it);
        }

        leaderBoard = trimLeaderBoard(leaderBoard, spectrum, N);
    }
    return leaders;
}

} /* week2 */

} /* bio */
