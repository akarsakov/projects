#include <iostream>
#include <fstream>
#include <string_processing.h>

using namespace std;
using namespace bio::week1;

int main()
{
    string input_filename = "dataset_3_5.txt";
    ifstream input(input_filename);

    string text, pattern;
    
    if (input.is_open())
    {
        input >> pattern;
        input >> text;

        vector<int> occurences = AllOccurrences(text, pattern);
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