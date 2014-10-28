#include <iostream>
#include <fstream>
#include <string_processing.h>

using namespace std;
using namespace bio::week1;

int main()
{
    string input_filename = "dataset_9_6.txt";
    ifstream input(input_filename);

    string text, pattern;
    int d;
    
    if (input.is_open())
    {
        input >> text;
        input >> pattern;
        input >> d;

        cout << "Pattern count: " << ApproximatePatternCount(text, pattern, d) << endl;

        input.close();
    }
    else
    {
        cout << "Couldn't open input file: \"" << input_filename << "\"" << endl;
        return 1;
    }
    return 0;
}