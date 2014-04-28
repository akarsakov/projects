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
	case '$': return 4;
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
	case 4: return '$';
	}
	return ' ';
}

struct Node
{
	char letter;
	bool isEnd;
	int degree;
	Node* subtree[5];
	Node(char l = ' '): letter(l), degree(0), isEnd(false) {
		subtree[0] = subtree[1] = subtree[2] = subtree[3] = subtree[4] = nullptr;
	}
};


class Trie
{
private:
	Node* root;
public:
	Trie() {
		root = new Node(' ');
	}

	~Trie() {
		freeNode(root);
	};

	void addString(char* str) {
		Node* cur = root;
		int size = strlen(str);
		for (int i=0; i<size; i++)
		{
			if (cur->subtree[DNA2num(str[i])] == nullptr)
			{
				cur->subtree[DNA2num(str[i])] = new Node(str[i]);
				cur->degree++;
			}
			cur = cur->subtree[DNA2num(str[i])];
		}
		cur->isEnd = true;
	}

	void printSuffixTree()
	{
		printSuf(root, "");
	}

private:
	void freeNode(Node* node)
	{
		for (int i=0; i<5; i++)
		{
			if (node->subtree[i] != nullptr)
				freeNode(node->subtree[i]);
		}
		delete node;
	}

	void printSuf(Node* node, string buffer)
	{
		if (node->isEnd)
		{
			print(buffer + node->letter);
			return;
		}

		if (node->degree > 1)
		{
			print(buffer + node->letter);
			for (int i=0; i<5; i++)
				if (node->subtree[i] != nullptr)
					printSuf(node->subtree[i], "");
			return;
		}

		for (int i=0; i<5; i++)
				if (node->subtree[i] != nullptr)
					printSuf(node->subtree[i], buffer + node->letter);
	}

	void print(string str)
	{
		if (str.size() > 0)
			printf("%s\n", str.c_str());
	}

};