#include "Trie.h"
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* str_c = new char[10000];
	
	fscanf(file, "%s", str_c);
	string text(str_c);

	Trie myTie;
	while (!feof(file))
	{
		fscanf(file, "%s", str_c);
		string pattern(str_c);
		myTie.addString(pattern);
	}
	
	myTie.matchText(text);
	
	fclose(file);
	delete[] str_c;
	return 0;
}