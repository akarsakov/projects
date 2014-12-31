#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <sstream>
#include <random_breakage_model.h>

using namespace std;
using namespace bio;
using namespace bio::week6;

string permutationString1(const vector<int>& perm) {
    stringstream result;
    result << "(";
    if (perm.back() > 0)
			result << "+" << perm.back();
		else
            result << perm.back();

	for (size_t i=0; i<perm.size()-1; i++)
	{
        result << " ";
		if (perm[i] > 0)
			result << "+" << perm[i];
		else
            result << perm[i];
	}
	result << ")";
    return result.str();
}

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open()) {
        vector<vector<int>> genome;
        string line;

        getline(input, line);
        readPermutationString(line, genome);
        int i1, i2, j1, j2;
        input >> i1;
        input >> i2;
        input >> j1;
        input >> j2;

        twoBrakeOnGenome(genome, i1, i2, j1, j2);

        for (auto perm : genome) {
            cout << permutationString1(perm);
        }
        cout << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}