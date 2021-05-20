#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <cstring>
#include <vector>
#include "./Huffman_Requirements.h"

using namespace std;

// Constants
#ifndef MAX_NODE_COUNT
#define MAX_NODE_COUNT 512
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 256
#endif

class huffman
{
	// Class Implementation of the Huffman Compression Algorithm
	private:
		node* Tree;
		node* root;
		int* freq;
		vector<bool> path, alias[MAX_CHAR];
		char buffer;
		int bufferLen;

	public:
		huffman();
		~huffman();

		node* createTree(ifstream& fin);
		void printTree(node* u = NULL, int h = 0);
		void writeDFS(node* u, ofstream& fout, vector<unsigned char>& verts, vector<short>& edges);
		void saveTree(ofstream& fout);
		void mapCharacters(node* u = NULL);
		void addToBuffer(bool val, ofstream& fout);
		void emptyBuffer(ofstream& fout);
		void compress(ifstream& fin, ofstream& fout);
		void compressDriver(const char fileName[]);
		void readTree(ifstream& fin);
		void decompress(ifstream& fin, ofstream& fout);
		void decompressDriver(const char archiveName[]);
};

#endif // HUFFMAN_H
