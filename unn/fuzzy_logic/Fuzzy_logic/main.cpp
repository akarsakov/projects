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
    data.normalize(0,1);
    cout << "test - " << data.getFeatureValue(1, 3) << endl;
    return 0;
}