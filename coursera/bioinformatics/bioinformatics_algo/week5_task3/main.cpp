#include <iostream>
#include <vector> 
#include <limits>
#include <fstream>
#include <string>
#include <string_processing.h>

using namespace std;
using namespace bio::week5;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open()) {
	    string v, w;
        input >> v;
        input >> w;

        string LCS = getLCS(v, w);
        cout << LCS << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}