#include <iostream>
#include <fstream>
#include <string_processing.h>

using namespace std;
using namespace week1;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    string text, pattern;
    int d;
    
    if (input.is_open())
    {
        input >> pattern;
        input >> text;
        input >> d;

        vector<int> occurences = AllApproximateOccurences(text, pattern, d);
        if (occurences.empty())
            cout << "No occurences found" << endl;
        else
        {
            for (auto it=occurences.begin(); it!=occurences.end(); it++)
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