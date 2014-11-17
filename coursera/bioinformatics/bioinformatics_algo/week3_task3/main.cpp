#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "DNA_processing.h"

using namespace std;
using namespace bio;
using namespace bio::week3;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    
    if (input.is_open())
    {
        string text;
        
        int k;

        input >> text;
        input >> k;
        vector<vector<double>> profile(k, vector<double>());

        for (int i=0; i<k; i++)
            for (int j=0; j<k; j++)
            {
                double val;
                input >> val;
                profile[i].push_back(val);
            }

        string result;
        double max_prob = 0.0;
        for (int i=0; i<text.size() - k + 1; i++)
        {
            string kMer = text.substr(i, k);
            double cur_prob = kMerProbability(profile, kMer);
            if (cur_prob > max_prob)
            {
                max_prob = cur_prob;
                result = kMer;
            }
        }
        cout << "Most probable kMer in text: " << result << endl;
                
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}