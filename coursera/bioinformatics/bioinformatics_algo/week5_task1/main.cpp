#include <iostream>
#include <vector> 
#include <limits>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open())
    {
	    int money;
	    vector<int> coins;
	    vector<int> minNumCoins;
	    input >> money;

        string coin_str;
        while (getline(input, coin_str, ','))
	    {
		    int coin;
            stringstream ss(coin_str);
		    ss >> coin;
		    coins.push_back(coin);
	    }

	    minNumCoins.resize(money+1, INT_MAX);
	    minNumCoins[0] = 0;
	    for (int i=1; i<=money; i++)
	    {
		    for (auto it=coins.begin(); it!=coins.end(); it++)
		    {
			    if (i >= *it)
			    {
				    if (minNumCoins[i - *it] + 1 < minNumCoins[i])
				    {
					    minNumCoins[i] = minNumCoins[i - *it] + 1;
				    }
			    }
		    }
	    }

	    cout << "Result: " << minNumCoins[money] << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}