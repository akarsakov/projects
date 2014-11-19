#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "DNA_processing.h"

using namespace std;
using namespace bio;
using namespace bio::week3;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    
    if (input.is_open())
    {
        string text;
        
        int k, t, N;
        input >> k;
        input >> t;
        input >> N;

        vector<string> DNA;

        for (int i=0; i<t; i++)
        {
            string text;
            input >> text;
            DNA.push_back(text);
        }

        LaplaceProfileGenerator* gen = new LaplaceProfileGenerator();
        vector<string> motif = GibbsMotifSearch(DNA, k, N, 1000, gen);
        for (auto text : motif)
        {
            cout << text << endl;
        }
        delete gen;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}