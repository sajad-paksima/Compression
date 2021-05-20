#ifndef MOVETOFRONT_H
#define MOVETOFRONT_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

// Constants
#ifndef MAX_NODE_COUNT
#define MAX_NODE_COUNT 512
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 256
#endif

class moveToFront
{
	// Class implementation of the Move To Front Algorithm
	private:
		unsigned char* list;

		int search(unsigned char key, unsigned char* list);
		void MTF(int output, unsigned char* list);
		void resetList(unsigned char* list);

	public:
		moveToFront();
		~moveToFront();
		void convert(ifstream& fin, ofstream& fout);
		void inverse(ifstream& fin, ofstream& fout);
		void convertDriver(const char inputFile[]);
		void inverseDriver(const char inputFile[]);
};

#endif // MOVETOFRONT_H
