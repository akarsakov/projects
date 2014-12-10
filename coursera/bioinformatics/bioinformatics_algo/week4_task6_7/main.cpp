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
    char buffer[1000];
    if (input.is_open())
    {
        int numV = 0;
        string line;
        while (getline(input, line)) {
            int s;
            sscanf(line.c_str(), "%d -> %s", &s, buffer);
            numV = max(s, numV);
        }

        Graph g(3000);

        input.clear();
        input.seekg(0, ios::beg);

        while (!input.eof()) {
            int s;
            
            getline(input, line);
            sscanf(line.c_str(), "%d -> %s", &s, buffer);
            stringstream ss(buffer);

            while (getline(ss, line, ',')) {
                int e = atoi(line.c_str());
                g.addEdge(s, e);
            }
        }

        list<int> eulerianCycle = g.getEulerianPath();

        int index = 0;
        for (auto v : eulerianCycle) {
            if (index == 0)
                output << v;
            else
                output << "->" << v;
            index++;
        }
        output << endl;
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
    return 0;
}