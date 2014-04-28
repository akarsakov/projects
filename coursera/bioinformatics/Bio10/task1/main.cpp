#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* c_str = new char[10000];
	fscanf(file, "%s", c_str);
	vector<string> BWT;
	int size = strlen(c_str);
	for (int i=0; i<size; i++)
	{
		string begin(c_str + size - i, i);
		string end(c_str, size - i);
		BWT.push_back(begin + end);
	}

	sort(BWT.begin(), BWT.end());

	for (int i=0; i<size; i++)
	{
		printf("%c", BWT[i][size-1]);
	}

	fclose(file);
	return 0;
}