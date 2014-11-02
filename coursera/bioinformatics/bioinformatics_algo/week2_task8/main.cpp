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
        int M, N;

        input >> M;
        input >> N;

        while (!input.eof())
        {
            int temp;
            input >> temp;
            spectrum.push_back(temp);
        }

        sort(spectrum.begin(), spectrum.end());
        
        vector<vector<int>> leaders = convolutionCyclopeptideSequencing(spectrum, M, N);

        for (auto it=leaders.begin(); it!=leaders.end(); it++)
            cout << massesToSpectrumString(*it) << endl;
                
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}