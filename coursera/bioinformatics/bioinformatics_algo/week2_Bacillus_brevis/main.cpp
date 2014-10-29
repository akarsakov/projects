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
    string input_filename = "B_brevis.txt";
    ifstream input(input_filename);
    string DNA_string, peptide;
    
    int start_positions = 0;
    peptide = "VKLFPWFNQY";
    
    if (input.is_open())
    {
        input >> DNA_string;

        int pattern_size = 3 * peptide.size();
        for (size_t i=0; i<DNA_string.size() - pattern_size; i++)
        {
            string patternDNA = DNA_string.substr(i, pattern_size);
            
            string patternRNA = DNA2RNA(patternDNA);
            string curPeptide = RNA2AA(patternRNA);

            if (curPeptide == peptide)
            {
                start_positions++;
                continue;
            }

            string rev_patternDNA = rev_complement(patternDNA);
            patternRNA = DNA2RNA(rev_patternDNA);
            curPeptide = RNA2AA(patternRNA);

            if (curPeptide == peptide)
                start_positions++;
        }

        cout << "Number of starting positions: " << start_positions << std::endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}