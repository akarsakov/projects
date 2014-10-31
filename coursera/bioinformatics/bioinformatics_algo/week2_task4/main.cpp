#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <AminoAcids_processing.h>
#include <RNA_processing.h>
#include <DNA_processing.h>

using namespace std;
using namespace bio;
using namespace bio::week2;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    
    if (input.is_open())
    {
        vector<int> spectrum;
        while (!input.eof())
        {
            int num;
            input >> num;
            spectrum.push_back(num);
        }

        vector<string> result = cyclopeptideSequencing(spectrum);
        for (auto it=result.begin(); it!=result.end(); it++)
        {
            cout << peptideToSpectrumString(*it) << " ";
        }
        cout << endl;
        
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}