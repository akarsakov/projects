#include <iostream>
#include <fstream>
#include <string_processing.h>

using namespace std;
using namespace bio::week1;

int main()
{
    string input_filename = "dataset_9_7.txt";
    ifstream input(input_filename);
    string text;
    int k, d;
    
    if (input.is_open())
    {
        input >> text;
        input >> k;
        input >> d;

        set<string> mostFrequentKMers = ApproximateFrequentWords(text, k, d);
        if (!mostFrequentKMers.empty())
        {
            cout << "Most frequent " << k << "-mer(s): ";
            for (auto it=mostFrequentKMers.begin(); it!=mostFrequentKMers.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
        else
        {
            cout << "Most frequent " << k << "-mer(s) not found!" << endl;
        }
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
    return 0;
}