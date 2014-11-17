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
        vector<string> DNA;
        int k, d;

        input >> k;
        input >> d;

        while (!input.eof())
        {
            string temp;
            input >> temp;
            DNA.push_back(temp);
        }

        set<string> patterns = motifEnumeration(DNA, k, d);

        for (auto pattern : patterns) {
            cout << pattern << " ";
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