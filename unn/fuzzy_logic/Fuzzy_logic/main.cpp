#include <iostream>
#include <string>
#include "Data.h"
#include "KohonenNetwork.h"
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
	KohonenNetwork identification(data, 7, 0.1f, 0.1f, 0.00001f, 5);
	identification.trainNetwork();
	vector<vector<float>> centers = identification.getCenters();
	for (auto it=centers.begin(); it!=centers.end(); it++)
	{
		cout << "[";
		for (auto v=it->begin(); v!=it->end(); v++)
			cout << *v << " ";
		cout << "]" << endl;
	}

    return 0;
}