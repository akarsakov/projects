#include <iostream>
#include <fstream>
#include <vector>
#include <AminoAcids_processing.h>
#include <RNA_processing.h>
#include <DNA_processing.h>

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    string DNA_string, peptide;
    vector<string> result;
    
    if (input.is_open())
    {
        input >> DNA_string;
        input >> peptide;

        int pattern_size = 3 * peptide.size();
        for (size_t i=0; i<DNA_string.size() - pattern_size; i++)
        {
            string patternDNA = DNA_string.substr(i, pattern_size);
            
            string patternRNA = DNA2RNA(patternDNA);
            string curPeptide = RNA2AA(patternRNA);

            if (curPeptide == peptide)
                result.push_back(patternDNA);

            string rev_patternDNA = rev_complement(patternDNA);
            patternRNA = DNA2RNA(rev_patternDNA);
            curPeptide = RNA2AA(patternRNA);

            if (curPeptide == peptide)
                result.push_back(patternDNA);
        }

        for (auto it=result.begin(); it!=result.end(); it++)
            cout << *it << endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}