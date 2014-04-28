#include "Trie.h"
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* str_c = new char[11000];
	
	fscanf(file, "%s", str_c);
	string text(str_c);

	Trie myTie;
	int max = 0;
	int max_index = 0;
	for (int i=0; i<text.size()-1; i++)
	{
		char* suffix = str_c + i;
		int cur = myTie.addString(suffix);
		if (cur > max)
		{
			max = cur;
			max_index = i;
		}
	}

	string longest_repeated = text.substr(max_index, max);
	printf("%s", longest_repeated.c_str());
	
	fclose(file);
	delete[] str_c;
	return 0;
}