#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <AminoAcids_processing.h>
#include <RNA_processing.h>
#include <DNA_processing.h>

using namespace std;
using namespace bio;
using namespace bio::week2;

vector<double> givenMassProblem(int m)
{
    vector<int> masses = getMassesVector();
    vector<double> numberPeptides(m+1, 0);

    int index = 0;
    numberPeptides[0] = 1;

    do
    {
        for (auto it=masses.begin(); it!=masses.end(); it++)
        {
            int newInd = *it + index;
            if (newInd < m + 1)
                numberPeptides[newInd] += numberPeptides[index];
        }

        index++;
        while (index < m)
        {
            if (numberPeptides[index] != 0)
                break;
            index++;
        }
    } while (index < m);

    return numberPeptides;
}

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);
    ofstream output("result.csv");

    int m;
    
    if (input.is_open())
    {
        input >> m;

        vector<double> numberPeptides = givenMassProblem(m);
        for (size_t i=0; i < numberPeptides.size(); i++)
            output << i << ", " << fixed << setprecision(0) << numberPeptides[i] << endl;

        output.close();
        input.close();
    }
    else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }

    return 0;
}