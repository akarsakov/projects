#include <iostream>
#include <fstream>
#include <vector>
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
        int N;

        input >> N;

        while (!input.eof())
        {
            int temp;
            input >> temp;
            spectrum.push_back(temp);
        }
        
        vector<vector<int>> leaders = leaderBoardCyclopeptideSequincingExtended(spectrum, N);

        for (auto it=leaders.begin(); it!=leaders.end(); it++)
            cout << massesToSpectrumString(*it) << " ";
                
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}