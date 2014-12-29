#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <random_breakage_model.h>

using namespace std;
using namespace bio;
using namespace bio::week6;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open()) {
        vector<vector<int>> permutations;
        string line;
        getline(input, line);
        readPermutationString(line, permutations);

        vector<vector<int>> sorted;
        greedySorting(permutations[0], sorted);

        for (auto stage : sorted) {
            cout << permutationString(stage) << endl;
        }
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}