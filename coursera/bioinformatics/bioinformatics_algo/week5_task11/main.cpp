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
        pair<int, int> start, end;

        map<pair<char, char>, int> blosum = readScoringMatrix(BLOSUM_FILE);
        alignmentMiddleEdge(v, w, 5, blosum, start, end);

        cout << "(" << start.first << ", " << start.second << ") (" << end.first << ", " << end.second << ")" << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}