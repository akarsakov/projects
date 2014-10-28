#include <iostream>
#include <fstream>
#include <AminoAcids_processing.h>

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    string RNA_string;
    
    if (input.is_open())
    {
        input >> RNA_string;

        cout << "Result = " << RNA2AA(RNA_string) << endl;
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}