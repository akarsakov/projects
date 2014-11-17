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
        int k;

        input >> k;

        while (!input.eof())
        {
            string temp;
            input >> temp;
            DNA.push_back(temp);
        }

        string median = medianString(DNA, k);
        cout << "Median: " << median << endl;
                
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}