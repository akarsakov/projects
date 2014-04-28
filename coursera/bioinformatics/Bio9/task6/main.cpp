#include <stdio.h>
#include "SuffixTree.h"
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* str_c = new char[31000];
	char* str_c1 = new char[31000];

	fscanf(file, "%s", str_c);
	string text1(str_c);
	fscanf(file, "%s", str_c1);
	string text2(str_c1);

	SuffixTree myTree;
	int min = INT_MAX;
	int min_index = -1;

	for (int i=0; i<text2.size(); i++)
	{
		string suf = text2.substr(i);
		myTree.addSuffix(suf);
	}

	for (int i=0; i<text1.size(); i++)
	{
		int cur = myTree.find(str_c + i);
		if (cur == text1.size() - i)
			continue;
		if (cur < min)
		{
			min = cur;
			min_index = i;
		}
	}

	printf("%s", text1.substr(min_index, min+1).c_str());

	fclose(file);
	delete[] str_c;
	delete[] str_c1;
	return 0;
}