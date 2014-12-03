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

        string path = reconstructPath(components);
        output << path << endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}