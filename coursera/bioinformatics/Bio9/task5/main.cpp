#include <stdio.h>
#include "SuffixTree.h"
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* str_c = new char[31000];
	
	fscanf(file, "%s", str_c);
	string text1(str_c);
	fscanf(file, "%s", str_c);
	string text2(str_c);

	SuffixTree myTree;
	int max = 0;
	int max_index = 0;

	for (int i=0; i<text1.size(); i++)
	{
		string suf = text1.substr(i);
		myTree.addSuffix(suf);
	}

	for (int i=0; i<text2.size(); i++)
	{
		int cur = myTree.find(str_c + i);
		if (cur > max)
		{
			max = cur;
			max_index = i;
		}
	}

	printf("%s", text2.substr(max_index, max).c_str());

	fclose(file);
	delete[] str_c;
	return 0;
}