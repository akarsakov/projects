#include <vector>
#include <string>
using namespace std;

#define ST_SIZE 5 // max number of subtrees

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
	string suffix;
	Node* subtree[ST_SIZE];
	Node* parent;
	int offset;

	Node(string suf): suffix(suf), parent(nullptr), offset(0) {
		for (int i=0; i<ST_SIZE; i++)
			subtree[i] = nullptr;
	};

	void addSubTree(string suf, int offset) {
		int index = DNA2num(suf[0]);
		subtree[index] = new Node(suf);
		subtree[index]->parent = this;
		subtree[index]->offset = offset;
	}

	void addSubTree(Node* node) {
		int index = DNA2num(node->suffix[0]);
		subtree[index] = node;
		subtree[index]->parent = this;
	}
};


class SuffixTree
{
private:
	Node* root;
	int count;

	int max_length;
	string max_common_substring;
public:
	SuffixTree() {
		root = new Node("");
		count = 1;
		max_length = 0;
		max_common_substring = "";
	}

	~SuffixTree() {};

	void addSuffix(string suf) {
		Node* cur = root;
		string suffix = suf;
		while (true)
		{
			int prefix_len = 0;
			Node* subNode = nullptr;
			for (int i=0; i<ST_SIZE; i++)
			{
				if (cur->subtree[i] != nullptr)
				{
					string temp = cur->subtree[i]->suffix;
					prefix_len = commonPrefix(suffix, temp);
					if (prefix_len > 0)
					{
						subNode = cur->subtree[i];
						break;
					}
				}
			}

			if (subNode != nullptr)
			{
				string temp = subNode->suffix;
				string prefix = temp.substr(0, prefix_len);
				string postfix = temp.substr(prefix_len);
				string postfix2 = suffix.substr(prefix_len);

				if (postfix.size() == 0)
				{
					cur = subNode;
					suffix = postfix2;
				}
				else
				{
					Node* temp = new Node(prefix);
					subNode->parent->addSubTree(temp);
					subNode->suffix = postfix;
					temp->addSubTree(subNode);
					temp->addSubTree(postfix2, count++);
					break;
				}
			} 
			else 
			{
				cur->addSubTree(suffix, count++);
				break;
			}
		}
	}

	int find(char* str)
	{
		int index = 0;
		Node* prev = root;
		while (true) 
		{
			Node* current = prev->subtree[DNA2num(str[index])];

			if (current != nullptr)
			{
				int prefix_len = commonPrefix(current->suffix, str + index);

				if (prefix_len < current->suffix.size())
				{
					return index + prefix_len;
				}
				else if (prefix_len == current->suffix.size())
				{
					index += prefix_len;
					prev = current;
				}
			}
			else
				return index;
		}
	}

	void printTree()
	{
		printNode(root);
	}

private:

	int commonPrefix(string first, string second)
	{
		int res = 0;
		int i = 0;
		while ((i < first.size()) && (i < second.size()))
		{
			if (first[i] == second[i])
				res++;
			else
				break;
			i++;
		}
		return res;
	}

	int commonPrefix(string first, char* second)
	{
		int res = 0;
		int i = 0;
		int second_size = strlen(second);
		while ((i < first.size()) && (i < second_size))
		{
			if (first[i] == second[i])
				res++;
			else
				break;
			i++;
		}
		return res;
	}

	int countSpecialSymbols(string str) {
		int count = 0;
		auto f = str.find('$');
		if (f != string::npos)
			count++;
		f = str.find('#');
		if (f != string::npos)
			count++;
		return count;
	}

	void search(Node* node, string buffer)
	{
		for (int i=0; i<ST_SIZE; i++)
			if (node->subtree[i] != nullptr)
			{
				string current = buffer + node->subtree[i]->suffix;
				if (countSpecialSymbols(current) == 2)
				{
					int i = current.find('$');
					int j = current.find('#');
					int length = min(i, j);
					if (length > max_length)
					{
						max_length = length;
						max_common_substring = current.substr(0, length);
					}
				}
				else
				{
					search(node->subtree[i], current);
				}
			}
	}

	void printNode(Node* node)
	{
		for (int i=0; i<ST_SIZE; i++)
			if (node->subtree[i] != nullptr)
				printNode(node->subtree[i]);
		printf("%s\n", node->suffix.c_str());
	}

};