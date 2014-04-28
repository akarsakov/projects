#include <list>
#include <string>
#include <stdio.h>
using namespace std;

int DNA2num(char DNA)
{
	switch (DNA)
	{
	case 'A': return 0;
	case 'T': return 1;
	case 'C': return 2;
	case 'G': return 3;
	}
	return 5;
}

char num2DNA(int num)
{
	switch (num)
	{
	case 0: return 'A';
	case 1: return 'T';
	case 2: return 'C';
	case 3: return 'G';
	}
	return ' ';
}

struct Node
{
	char letter;
	bool isEnd;
	int index;
	Node* subtree[4];
	Node(char l = ' ', int n = 0, bool is_end = false): letter(l), index(n), isEnd(is_end) {
		subtree[0] = subtree[1] = subtree[2] = subtree[3] = nullptr;
	}
};


class Trie
{
private:
	Node* root;
	int index;
public:
	Trie() {
		root = new Node(' ', 1);
		index = 2;
	}

	~Trie() {};

	void addString(string str) {
		Node* cur = root;
		for (int i=0; i<str.size(); i++)
		{
			if (cur->subtree[DNA2num(str[i])] == nullptr)
			{
				cur->subtree[DNA2num(str[i])] = new Node(str[i], index++);
			}
			cur = cur->subtree[DNA2num(str[i])];
		}
		cur->isEnd = true;
	}

	void print()
	{
		printNode(root);
	}

private:
	void printNode(Node* node)
	{
		for (int i=0; i<4; i++)
		{
			if (node->subtree[i] != nullptr)
			{
				printf("%d %d %c\n", node->index, node->subtree[i]->index, num2DNA(i));
				printNode(node->subtree[i]);
			}
		}
	}

};