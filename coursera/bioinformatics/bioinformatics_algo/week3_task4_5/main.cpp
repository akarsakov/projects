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
        
        int k, t;
        input >> k;
        input >> t;

        vector<string> DNA;

        for (int i=0; i<t; i++)
        {
            string text;
            input >> text;
            DNA.push_back(text);
        }

        LaplaceProfileGenerator* gen = new LaplaceProfileGenerator();
        vector<string> motif = greedyMotifSearch(DNA, k, gen);
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