#include <iostream>
#include <string>
#include "Data.h"
#include "KohonenNetwork.h"
#include "CrossValidation.h"
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
    CrossValidation valid(data, 5);
    cout << "Total error: " << valid.getError() << endl;
    return 0;
}