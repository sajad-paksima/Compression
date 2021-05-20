#include "../include/Huffman_Requirements.h"


node::node(unsigned char LABEL /*= ' '*/, node* LEFT /*= NULL*/, node* RIGHT /*= NULL*/)
{
	label = LABEL;
	leftChild = LEFT;
	rightChild = RIGHT;
}

// Set Operator
void node::operator=(const node& other)
{
	label = other.label;
	leftChild = other.leftChild;
	rightChild = other.rightChild;
}

huffmanItem::huffmanItem(unsigned char C /*= ' '*/, int COUNT /*= 0*/, node* PTR /*= NULL*/)
{
	c = C;
	count = COUNT;
	ptr = PTR;
}
huffmanItem::huffmanItem(const huffmanItem& other)
{
	c = other.c;
	count = other.count;
	ptr = other.ptr;
}

// Getters
char huffmanItem::getc()
{
	return c;
}
int huffmanItem::getCount()
{
	return count;
}
node* huffmanItem::getNode()
{
	return ptr;
}

// Modifiers
void huffmanItem::addToCount(int val /*= 1*/)
{
	count += val;
}
void huffmanItem::initNode(node* PTR /*= NULL*/)
{
	ptr = PTR;
}

// Set Operator
void huffmanItem::operator=(const huffmanItem& other)
{
	c = other.c;
	count = other.count;
	ptr = other.ptr;
}

// Comparison Operator
bool huffmanItem::operator<(const huffmanItem& other)
{
	if (count == other.count)
		return (c < other.c);
	return (count < other.count);
}
bool huffmanItem::operator>(const huffmanItem& other)
{
	if (count == other.count)
		return (c > other.c);
	return (count > other.count);
}
