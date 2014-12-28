#include <iostream>
#include <vector> 
#include <limits>
#include <fstream>
#include <string>
#include <AminoAcids_processing.h>

using namespace std;
using namespace bio;
using namespace bio::week5;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open()) {
	    string v, w;
        input >> w;
        input >> v;

        int score = overlapAlignment(v, w, 2);

        cout << score << endl;
        cout << w << endl;
        cout << v << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}