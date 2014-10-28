#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_processing.h>

using namespace std;
using namespace bio;

int main()
{
    string input_filename = "dataset_9_3.txt";
    ifstream input(input_filename);

    string str1, str2;
    
    if (input.is_open())
    {
        input >> str1;
        input >> str2;

        int distance = HammingDistance(str1, str2);
        if (distance >= 0)
            cout << "Hamming distance: " << distance << endl;
        else
            cout << "Wrong strings!" << endl;

        input.close();
    }
    else
    {
        cout << "Couldn't open input file: \"" << input_filename << "\"" << endl;
        return 1;
    }
    return 0;
}