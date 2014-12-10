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
        vector<string> kMers;

        while (!input.eof()) {
            string kMer;
            input >> kMer;
            kMers.push_back(kMer);
        }

        DeBruijnGraph g(kMers);

        list<string> contigs = g.generateContigs();

        for (auto contig : contigs) {
            output << contig << endl;
        }
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
    return 0;
}