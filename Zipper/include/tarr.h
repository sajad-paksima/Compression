#ifndef TARR_H
#define TARR_H

#include <iostream>
#include <algorithm>
#include <fstream>
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

class tarr
{
	// Class Implementation of a Program Replica of tar
	public:
		void merge(char** inputFiles, int numberOfFiles, const char archiveName[]);
		void extract(const char inputFile[]);
};

#endif // TARR_H
