#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char* c_str = new char[10000];
	int k;
	fscanf(file, "%d", &k);
	fscanf(file, "%s", c_str);
	int size = strlen(c_str);
	string str(c_str);

	vector<string> mas;
	for (int i=0; i < size - k + 1; i++)
	{
		string temp = str.substr(i, k);
		mas.push_back(temp);
	}

	sort(mas.begin(), mas.end());
	
	for (auto iter = mas.begin(); iter != mas.end(); iter++)
	{
		fprintf(out, "%s\n", iter->c_str());
	}
	delete[] c_str;
	return 0;
}