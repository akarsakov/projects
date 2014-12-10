#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include "genome_assembling.h"

using namespace std;
using namespace bio;

#define N 8

int main()
{
    ofstream output("result.txt");

    int numkMers = (1 << N);
    vector<string> kMers;
    kMers.reserve(numkMers);

    for (int i=0; i<numkMers; i++) {
        bitset<N> current(i);
        string kMer = current.to_string();
        kMers.push_back(kMer);
    }

    DeBruijnGraph g(kMers);

    output << g.reconstructCycle().substr(0, numkMers) << endl;

    return 0;
}