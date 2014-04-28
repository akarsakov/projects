#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Comparator {
	char* text;
	Comparator(char* t): text(t) {}; 
	bool operator() (int i,int j) { 
		return strcmp(text+i, text+j) < 0;
	}
};

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* str_c = new char[200000];

	fscanf(file, "%s", str_c);
	int size = strlen(str_c);

	vector<int> arr;

	for (int i=0; i<size; i++)
	{
		arr.push_back(i);
	}
	Comparator comp(str_c);
	sort(arr.begin(), arr.end(), comp);

	printf("%d", size);
	for (auto it=arr.begin(); it!=arr.end(); it++)
	{
		printf(", %d", *it);
	}

	fclose(file);
	delete[] str_c;
	return 0;
}