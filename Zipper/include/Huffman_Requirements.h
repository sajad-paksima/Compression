#ifndef HUFFMAN_REQUIREMENTS_H
#define HUFFMAN_REQUIREMENTS_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <cstring>
#include <vector>

using namespace std;

// Constants
#ifndef MAX_NODE_COUNT
#define MAX_NODE_COUNT 512
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 256
#endif

struct node
{
	// Node Structure for Creating the Huffman Tree
	unsigned char label;
	node* leftChild;
	node* rightChild;

	node(unsigned char LABEL = ' ', node* LEFT = NULL, node* RIGHT = NULL);
	void operator=(const node& other);
};

class huffmanItem
{
	// Item Class for Use in Huffman's Min-Heap
	private:
		unsigned char c;
		int count;
		node* ptr;

	public:
		huffmanItem(unsigned char C = ' ', int COUNT = 0, node* PTR = NULL);
		huffmanItem(const huffmanItem& other);

		char getc();
		int getCount();
		node* getNode();
		void addToCount(int val = 1);
		void initNode(node* PTR = NULL);
		void operator=(const huffmanItem& other);
		bool operator<(const huffmanItem& other);
		bool operator>(const huffmanItem& other);

		friend bool operator<(const huffmanItem& a, const huffmanItem& b)
		{
			if (a.count == b.count)
				return (a.c < b.c);
			return (a.count < b.count);
		}

		friend bool operator>(const huffmanItem& a, const huffmanItem& b)
		{
			if (a.count == b.count)
				return (a.c > b.c);
			return (a.count > b.count);
		}
};

#endif // HUFFMAN_REQUIREMENTS_H
