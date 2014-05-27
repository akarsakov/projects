#include <iostream>
#include <string>
#include "Data.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Error("Incorrect command line parameters");
    }
    string filename(argv[1]);
    Data data(filename);
    cout << "test - " << data.getFeatureValue(100, 4) << endl;
    return 0;
}