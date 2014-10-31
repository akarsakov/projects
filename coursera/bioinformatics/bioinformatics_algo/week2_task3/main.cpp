#include <iostream>
#include <fstream>
#include <vector>
#include <AminoAcids_processing.h>
#include <Spectrum_processing.h>


using namespace std;
using namespace bio;
using namespace bio::week2;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    string peptide;
    vector<int> cyclospectrum;
    
    if (input.is_open())
    {
        input >> peptide;

        cyclospectrum = getCyclospectrum(peptide);

        for (auto it=cyclospectrum.begin(); it!=cyclospectrum.end(); it++)
            cout << *it << " ";
        cout << endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}