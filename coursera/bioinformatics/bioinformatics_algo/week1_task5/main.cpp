#include <iostream>
#include <fstream>
#include <DNA_processing.h>

using namespace std;
using namespace week1;

int main()
{
    string input_filename = "dataset_4_4.txt";
    ifstream input(input_filename);

    string text;
    int k, L, t;
    
    if (input.is_open())
    {
        input >> text;
        input >> k;
        input >> L;
        input >> t;

        set<string> clumps = FindClumps(text, k, L, t);

        if (clumps.empty())
            cout << "No clumps found" << endl;
        else
        {
            for (auto it=clumps.begin(); it!=clumps.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
        input.close();
    }
    else
    {
        cout << "Couldn't open input file: \"" << input_filename << "\"" << endl;
        return 1;
    }
    return 0;
}