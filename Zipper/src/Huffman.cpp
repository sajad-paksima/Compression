#include "../include/Huffman.h"

huffman::huffman()
{
	freq = new int[MAX_CHAR];
	Tree = new node[MAX_NODE_COUNT];
	root = NULL;
	for (int i = 0; i < MAX_CHAR; i++)
        alias[i].clear();
    path.clear();
	bufferLen = 0;
}
huffman::~huffman()
{
	for (int i = 0; i < MAX_CHAR; i++)
		alias[i].clear();
    path.clear();
	delete freq;
	delete Tree;
}

node* huffman::createTree(ifstream& fin)
{
	// Create the Huffman Tree from the fin Stream
	cout << "Creating Huffman Tree" << endl;

	// Initialize
	char now;
	int ind = 0;
	fin.clear();
	fin.seekg(0);

	while (!fin.eof())
	{
		fin.read(&now, sizeof(now));
		if (fin.eof())
            break;
		freq[(unsigned char)now]++;
	}

	// Initialize Min Heap
	priority_queue<huffmanItem, vector<huffmanItem>, greater<huffmanItem> > huffmanHeap;
	for (int i = 0; i < MAX_CHAR; i++)
	{
		Tree[i] = node(i, NULL, NULL);
		huffmanHeap.push(huffmanItem(i, freq[i], Tree + ind));
		ind++;
	}

	// Merge Nodes and Create the Tree
	while (huffmanHeap.size() > 1)
	{
		huffmanItem a, b;
		a = huffmanHeap.top();
		huffmanHeap.pop();
		b = huffmanHeap.top();
		huffmanHeap.pop();

		Tree[ind].leftChild = a.getNode();
		Tree[ind].rightChild = b.getNode();
		huffmanHeap.push(huffmanItem(' ', a.getCount() + b.getCount(), Tree + ind));
		ind++;
	}

	// Cleanup
	huffmanItem a = huffmanHeap.top();
	huffmanHeap.pop();
	return (root = a.getNode());
}

void huffman::printTree(node* u /*= NULL*/, int h /*= 0*/)
{
	// Print the Huffman Tree (for Debugging Purposes)
	if (u == NULL)
		u = root;
	cout << u->label << ' ' << h << endl;
	if (u->leftChild)
		printTree(u->leftChild, h + 1);
	if (u->rightChild)
		printTree(u->rightChild, h + 1);
}

void huffman::writeDFS(node* u, ofstream& fout, vector<unsigned char>& verts, vector<short>& edges)
{
	// Traverse the Huffman Tree and Calculate the set of Vertices and Edges
	static int index = 0;
	int currentVertex = index;

	// Add the Node to the Result
	verts.push_back(u->label);

	// Traverse and Add the Edges to the Result
	if (u->leftChild)
	{
		edges.push_back(currentVertex);
		edges.push_back(++index);
		writeDFS(u->leftChild, fout, verts, edges);
	}
	if (u->rightChild)
	{
		edges.push_back(currentVertex);
		edges.push_back(++index);
		writeDFS(u->rightChild, fout, verts, edges);
	}
}

void huffman::saveTree(ofstream& fout)
{
	// Save the Huffman Tree in fout to Lookup in Case of Decompression

	// Initialize
	vector<unsigned char> verts;
	vector<short> edges;

	// Calculate the Vertices and Edges
	writeDFS(root, fout, verts, edges);

	short size = verts.size();
	fout.write((char*)(&size), sizeof(size));

	// Save the Tree
	for (char i : verts)
		fout.write(&i, sizeof(i));

	for (short i : edges)
		fout.write((char*)(&i), sizeof(i));

	// Cleanup
	verts.clear();
	edges.clear();
}

void huffman::mapCharacters(node* u /*= NULL*/)
{
	// Map Characters to the Codes Generated by the Huffman Tree

	// Initialize
	if (!u)
		u = root;

	// Set the alias to the Current Code in path
	if (!u->leftChild && !u->rightChild)
		alias[u->label] = path;

	// Traverse and Maintain the Current Code Saved in path
	if (u->leftChild)
	{
		path.push_back(0);
		mapCharacters(u->leftChild);
		path.pop_back();
	}
	if (u->rightChild)
	{
		path.push_back(1);
		mapCharacters(u->rightChild);
		path.pop_back();
	}
}

void huffman::addToBuffer(bool val, ofstream& fout)
{
	// Adding bits to the Writing Buffer of the Huffman Algorithm for Outputting
	if (bufferLen == 8)
	{
		fout.write(&buffer, sizeof(buffer));
		buffer = bufferLen = 0;
	}
	buffer = (buffer << 1) + val;
	bufferLen++;
}

void huffman::emptyBuffer(ofstream& fout)
{
	// Write and Clear the Current Buffer
	if (bufferLen)
	{
		buffer <<= (8 - bufferLen);
	}
	fout.write(&buffer, sizeof(buffer));
	buffer = bufferLen = 0;
}

void huffman::compress(ifstream& fin, ofstream& fout)
{
	// Huffman Compression Algorithm

	// Initialize
	for (int i = 0; i < MAX_CHAR; i++)
		alias[i].clear();
	mapCharacters();

	fin.clear();
	fin.seekg(0);

	char c;
	unsigned char C;
	long long outLength = 0;
	while (!fin.eof())
	{
		// Saving bits
		fin.read(&c, sizeof(c));
		if (fin.eof())
            break;
		C = c;
		for (bool now : alias[C])
			addToBuffer(now, fout), outLength++;
	}
	// Cleanup and Saving
	emptyBuffer(fout);
	fout.write((char*)(&outLength), sizeof(outLength));
}

void huffman::compressDriver(const char fileName[])
{
	// Driver for Error Checking and Input-Output Handling

	// Stream Creationg and Error Checking
	ifstream fin(fileName, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << fileName << "\" could not be opened";
		exit(0);
		return;
	}

	// Naming Convention Handling
	char* outputName = new char[strlen(fileName) + 5];
	strcpy(outputName, fileName);
	strcat(outputName, ".mbz");

	cout << "Compressing " << fileName << " to " << outputName << endl;

	// Stream Creationg and Error Checking
	ofstream fout(outputName, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outputName << "\" could not be created";
		exit(0);
		return;
	}

	fout.write(fileName, sizeof(fileName[0]) * strlen(fileName));
	char c = '/';
	fout.write(&c, sizeof(c));

	// Create the Tree
	createTree(fin);

	cout << "Saving Huffman Tree" << endl;

	// Save the Tree
	saveTree(fout);

	cout << "Compressing..." << endl;

	// Compress
	compress(fin, fout);

	// Cleanup
	fin.close();
	fout.close();

	cout << "Done" << endl;

	delete outputName;
}

void huffman::readTree(ifstream& fin)
{
	// Read the Huffman Tree from fin

	// Initialize
	short size;
	fin.read((char*)(&size), sizeof(size));
	char c;

	// Read the Vertices and Edges
	for (int i = 0; i < size; i++)
	{
		fin.read(&c, sizeof(c));
		Tree[i].label = c;
	}
	for (int i = 0; i < size - 1; i++)
	{
		short u, v;
		fin.read((char*)(&u), sizeof(u));
		fin.read((char*)(&v), sizeof(v));
		if (Tree[u].leftChild)
			Tree[u].rightChild = Tree + v;
		else
			Tree[u].leftChild = Tree + v;
	}
	root = Tree;
}

void huffman::decompress(ifstream& fin, ofstream& fout)
{
	// Huffman Decompression Algorithm

	// Initialize
	long long tmp = fin.tellg();
	fin.seekg(0, fin.end);
	fin.seekg((long long)fin.tellg() - (long long)sizeof(long long), fin.beg);
	long long fileSize;
	fin.read((char*)(&fileSize), sizeof(fileSize));
	fin.seekg(tmp);

	node* now = root;
	unsigned char readBuffer = 0;
	int readBufferLen = 0;
	for (int i = 0; i < fileSize; i++)
	{
		// Read and Write bit by bit
		if (readBufferLen == 0)
			fin.read((char*)(&readBuffer), sizeof(readBuffer)), readBufferLen = 8;
		if (!now->leftChild && !now->rightChild)
			fout.write((char*)(&now->label), sizeof(now->label)), now = root;
		if ((readBuffer & (1 << 7)))
			now = now->rightChild;
		else
			now = now->leftChild;
		readBuffer <<= 1;
		readBufferLen--;
	}
	if (!now->leftChild && !now->rightChild)
		fout.write((char*)(&now->label), sizeof(now->label)), now = root;
}

void huffman::decompressDriver(const char archiveName[])
{
	// Driver for Error Checking and Input-Output Handling

	cout << "Decompressing " << archiveName << endl;

	// Stream Creationg and Error Checking
	ifstream fin(archiveName, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << archiveName << "\" could not be opened";
		exit(0);
		return;
	}

	// Read the File's Original Name
	vector<char> fileName;
	char c;
	fin.read(&c, sizeof(c));
	while (c != '/')
	{
		fileName.push_back(c);
		fin.read(&c, sizeof(c));
	}

	// Output Naming Convention Handling
	char* outputName = new char[fileName.size() + 1];
	for (int i = 0; i < (int)fileName.size(); i++)
		outputName[i] = fileName[i];
	outputName[fileName.size()] = '\0';

	// Stream Creationg and Error Checking
	ofstream fout(outputName, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outputName << "\" could not be created";
		exit(0);
		return;
	}

	cout << "Reading Huffman Tree" << endl;
	// Read the Tree
	readTree(fin);
	cout << "Decompressing..." << endl;
	// Decompress
	decompress(fin, fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outputName;

	cout << "Done" << endl;
}