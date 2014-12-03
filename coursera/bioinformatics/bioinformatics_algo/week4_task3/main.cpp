#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "string_processing.h"

using namespace std;
using namespace bio::week4;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    ofstream output("result.txt");
    
    if (input.is_open())
    {
        vector<string> components;

        while (!input.eof()) {
            string line;
            input >> line;
            components.push_back(line);
        }

        int k = components[0].size() - 1;
        vector<pair<string, string>> overlapPairs = getOverlapPairs(components, k);

        for (auto op : overlapPairs) {
            output << op.first << " -> " << op.second << endl;
        }
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}