#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <Spectrum_processing.h>

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
        string peptide;

        input >> peptide;

        while (!input.eof())
        {
            int num;
            input >> num;
            spectrum.push_back(num);
        }

        cout << "Score: " << score(peptide, spectrum) << endl;       
        
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}