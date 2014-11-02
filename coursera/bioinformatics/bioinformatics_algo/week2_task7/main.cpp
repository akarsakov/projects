#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

        while (!input.eof())
        {
            int temp;
            input >> temp;
            spectrum.push_back(temp);
        }

        sort(spectrum.begin(), spectrum.end());
        
        vector<int> convolution = spectrumConvolution(spectrum);

        for (auto it=convolution.begin(); it!=convolution.end(); it++)
            cout << *it << " ";
                
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}