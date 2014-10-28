#include <iostream>
#include <fstream>
#include <DNA_processing.h>

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "dataset_3_2.txt";
    ifstream input(input_filename);
    ofstream output("result.txt");
    string text;
    
    if (input.is_open() && output.is_open())
    {
        input >> text;
        if (!isDNA(text))
        {
            cout << "Input string is not DNA!" << endl;
            return 1;
        }
        output << rev_complement(text);
        input.close();
    }
    else
    {
        cout << "Couldn't open input/output file: \"" << input_filename << "\"" << endl;
        return 1;
    }
    return 0;
}