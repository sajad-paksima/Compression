#ifndef PACKBITS_H
#define PACKBITS_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

#ifndef MAX_NODE_COUNT
#define MAX_NODE_COUNT 512
#endif

#ifndef MAX_CHAR
#define MAX_CHAR 256
#endif

#ifndef MAX_PACKBITS_LIMIT
#define MAX_PACKBITS_LIMIT 127
#endif

struct packBits
{
	private:
		char* tmp;

	public:
		packBits();
		~packBits();

		void decompress(ifstream& fin, ofstream& fout);
		void compress(ifstream& fin, ofstream& fout);
		void compressDriver(const char inputFile[]);
		void decompressDriver(const char inputFile[]);
};

#endif // PACKBITS_H
