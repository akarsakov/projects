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
        input >> v;
        input >> w;

        map<pair<char, char>, int> blosum = readScoringMatrix(BLOSUM_FILE);
        int score = alignmentWithAffineGapPenalties(v, w, blosum, 11, 1);

        cout << score << endl;
        cout << v << endl;
        cout << w << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}