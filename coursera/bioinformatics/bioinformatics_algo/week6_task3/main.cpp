#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <algorithm>
#include <random_breakage_model.h>

using namespace std;
using namespace bio;
using namespace bio::week6;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open()) {
        vector<pair<int, int>> edges;
        
        string line;
        int max_index = INT_MIN;
        while (getline(input, line)) {
            vector<vector<int>> perm;
            readPermutationString(line, perm);
            for (size_t i=0; i<perm.size(); i++) {
                max_index = max(max_index, *max_element(perm[i].begin(), perm[i].end()));
            }
            vector<pair<int, int>> curEdges = getColoredEdges(perm);
            edges.insert(edges.end(), curEdges.begin(), curEdges.end());
        }


        cout << max_index - getNumCycles(edges) << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}