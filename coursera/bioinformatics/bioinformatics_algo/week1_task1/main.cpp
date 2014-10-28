#include <iostream>
#include <fstream>
#include <string_processing.h>

using namespace std;
using namespace bio::week1;

int main()
{
    string input_filename = "dataset_2_6.txt";
    ifstream input(input_filename);
    string text, pattern;
    
    if (input.is_open())
    {
        input >> pattern;
        input >> text;

        cout << "Count = " << PatternCount(text, pattern) << endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}