#include <iostream>
#include <string>
#include <stdlib.h>
#include "Data.h"
#include "CrossValidation.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    char c_input[80];
    cout << "Enter dataset filename (iris.csv or hd.csv) [iris.csv]: ";
    cin.getline(c_input, 80);
    string filename(c_input);
    if (filename.empty())
        filename = "data\\iris.csv";
    else
        filename = "data\\" + filename;

    KohonenNetworkParameters kohonenParameters(10, 10, 0.1f, 0.05f, 0.000001f);
    ModelTSK0Parameters tskParameters(15, 0.1f, 0.001f);
    cout << "Use default parameters (y/n) [y]: ";
    cin.getline(c_input, 80);
    string answer(c_input);
    if (answer == "No" || answer == "no" || answer == "n")
    {
        cout << "Enter Kohonen network parameters" << endl;
        cout << "Enter number of clusters [10]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            kohonenParameters.num_clusters = atoi(c_input);
        cout << "Enter max epoch number [10]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            kohonenParameters.maxEpoch = atoi(c_input);
        cout << "Enter alpha for winner [0.1]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            kohonenParameters.alpha_winner = (float) atof(c_input);
        cout << "Enter alpha for loser [0.05]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            kohonenParameters.alpha_loser = (float) atof(c_input);
        cout << "Enter epsilon [0.000001]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            kohonenParameters.alpha_loser = (float) atof(c_input);

        cout << "Enter parameters for training TSK0 model" << endl;
        cout << "Enter max epoch number [15]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            tskParameters.maxEpoch = atoi(c_input);
        cout << "Enter learning rate [0.1]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            tskParameters.learningSpeed = (float) atof(c_input);
        cout << "Enter epsilon [0.001]: ";
        cin.getline(c_input, 80);
        if (strlen(c_input) != 0)
            tskParameters.epsilon = (float) atof(c_input);
    }
    
    Data data(filename);
    CrossValidation valid(data, 3);
    valid.validate(kohonenParameters, tskParameters);
    return 0;
}