#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "genome_assembling.h"

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    ofstream output("result.txt");

    if (input.is_open())
    {
        int k, d;
        input >> k;
        input >> d;
        vector<pair<string, string>> pairkMers;
        
        string line;
        while (getline(input, line)) {                      
            auto sep = line.find_first_of('|');
            if (sep != string::npos) {
                string first = line.substr(0, sep);
                string second = line.substr(sep+1, line.size()-(sep+1));
                pairkMers.push_back(make_pair(first, second));
            }
        }

        DeBruijnGraph g(pairkMers);
        output << g.reconstructPathFromPairedReads(k, d) << endl;
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
    return 0;
}