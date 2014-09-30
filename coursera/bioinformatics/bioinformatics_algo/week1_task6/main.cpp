#include <iostream>
#include <fstream>
#include <algorithm>
#include <DNA_processing.h>

using namespace std;
using namespace week1;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    string text;
    
    if (input.is_open())
    {
        input >> text;

        vector<int> skew = GetSkew(text);

        if (skew.empty())
            cout << "Skew is empty" << endl;
        else
        {
            int min_skew = *min_element(skew.begin(), skew.end());

            for (int i=0; i<skew.size(); i++)
                if (skew[i] == min_skew)
                    cout << i << " ";
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