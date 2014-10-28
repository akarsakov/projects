#include <iostream>
#include <fstream>
#include <string_processing.h>
#include <kMers_generator.h>
#include <DNA_processing.h>

using namespace std;
using namespace bio::week1;

int main()
{
    string input_filename = "dataset_3014_3.txt";
    ifstream input(input_filename);
    ofstream output("result.txt");
    string pattern;
    int d;

    if (input.is_open() && output.is_open())
    {
        input >> pattern;
        input >> d;

        set<string> res = Neighbors(pattern, d);

        for (auto it=res.begin(); it!=res.end(); it++)
            output << *it << endl;
    }
    else
    {
        cout << "Couldn't input/output file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}