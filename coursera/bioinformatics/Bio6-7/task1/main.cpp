#include <stdio.h>
#include <vector> 
#include <limits>
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	int money;
	vector<int> coins;
	vector<int> minNumCoins;
	fscanf(file, "%d", &money);
	while (!feof(file))
	{
		int coin;
		fscanf(file, "%d", &coin);
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

	printf("result - %d\n", minNumCoins[money]);
	fclose(file);
	return 0;
}