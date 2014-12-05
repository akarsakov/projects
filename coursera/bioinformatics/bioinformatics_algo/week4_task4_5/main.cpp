#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "genome_assembling.h"

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "input4.txt";
    ifstream input(input_filename);
    ofstream output("result4.txt");
    
    if (input.is_open())
    {
        int k;
        string text;

        input >> k;
        input >> text;

        DeBruijnGraph g(text, k);
        vector<pair<string, list<string>>> adjList = g.getAdjacencyList();

        for (auto node : adjList) {
            output << node.first << " -> ";
            int index = 0;

            for (auto elem : node.second) {
                if (index != 0)
                    output << "," << elem;
                else
                    output << elem;

                index++;
            }
            output << endl;
        }
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    input.close();
    output.close();
    input.open("input5.txt");
    output.open("result5.txt");

    if (input.is_open())
    {
        vector<string> kMers;

        while (!input.eof()) {
            string kMer;
            input >> kMer;
            kMers.push_back(kMer);
        }

        DeBruijnGraph g(kMers);
        vector<pair<string, list<string>>> adjList = g.getAdjacencyList();

        for (auto node : adjList) {
            output << node.first << " -> ";
            int index = 0;

            for (auto elem : node.second) {
                if (index != 0)
                    output << "," << elem;
                else
                    output << elem;

                index++;
            }
            output << endl;
        }
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}