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
	for (int i=0; i<text.size(); i++)
	{
		char* suffix = str_c + i;
		myTie.addString(suffix);
	}

	myTie.printSuffixTree();
	
	fclose(file);
	delete[] str_c;
	return 0;
}