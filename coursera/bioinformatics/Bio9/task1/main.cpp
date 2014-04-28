#include <vector>
#include "Trie.h"
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char str_c[1000];
	vector<string> patterns;

	while (!feof(file))
	{
		fscanf(file, "%s", str_c);
		string pattern(str_c);
		patterns.push_back(pattern);
	}

	Tie myTie;

	for (auto it=patterns.begin(); it!=patterns.end(); it++)
	{
		string pattern = *it;
		myTie.addString(pattern);
	}

	myTie.print();
	fclose(file);
	return 0;
}