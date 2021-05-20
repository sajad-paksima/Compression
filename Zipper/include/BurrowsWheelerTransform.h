#ifndef BURROWSWHEELERTRANSFORM_H
#define BURROWSWHEELERTRANSFORM_H


#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>

using namespace std;

// Constants
#ifndef MAX_NODE_COUNT
#define MAX_NODE_COUNT 512
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 256
#endif

class burrowsWheelerTransform
{
	// Class Implementation of the Burrows-Wheeler Transform Algorithm
	private:
		vector<int> p, c, pn, cn, cnt;
		char* s;
		int index, slen;

	public:
		burrowsWheelerTransform();
		~burrowsWheelerTransform();

		void readFile(ifstream& fin);
		void sortCyclicShifts();
		void writeOutTransform(ofstream& fout);
		void inverseReadFile(ifstream& fin);
		void inverse();
		void writeOutInverse(ofstream& fout);
		void transformDriver(const char inputFile[]);
		void inverseDriver(const char inputFile[]);
};

#endif // BURROWSWHEELERTRANSFORM_H
