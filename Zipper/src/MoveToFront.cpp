#include "../include/MoveToFront.h"

int moveToFront::search(unsigned char key, unsigned char* list)
{
	for (int i = 0; i < MAX_CHAR; i++)
		if (key == list[i])
			return i;
    return -1;
}

void moveToFront::MTF(int output, unsigned char* list)
{
	for (int i = output - 1; i >= 0; i--)
		swap(list[i + 1], list[i]);
}

void moveToFront::resetList(unsigned char* list)
{
	for (int i = 0; i < MAX_CHAR; i++)
		list[i] = i;
}

moveToFront::moveToFront()
{
	list = new unsigned char[MAX_CHAR + 1];
	resetList(list);
}
moveToFront::~moveToFront()
{
	delete list;
}

void moveToFront::convert(ifstream& fin, ofstream& fout)
{
	// Converting the fin stream to the "Move To Front Form"

	// Initialize
	resetList(list);
	char c;
	unsigned char out;

	// Process
	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;
		out = search(c, list);
		fout.write((char*)(&out), sizeof(out));
		MTF(out, list);
	}
}

void moveToFront::inverse(ifstream& fin, ofstream& fout)
{
	// Converting the fin stream back to it's original form

	// Initialize
	resetList(list);
	char c;

	// Process
	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;
		fout.write((char*)(list + (unsigned char)c), sizeof(list[0]));
		MTF((unsigned char)c, list);
	}
}

void moveToFront::convertDriver(const char inputFile[])
{
	// Dirver written to handle errors and file input-output

	cout << "Applying Move to Front Algorithm..." << endl;

	// Creating the name of the output file based on the input
	char* outputFile = new char[strlen(inputFile) + 5];
	strcpy(outputFile, inputFile);
	strcat(outputFile, ".mtf");

	// Stream creation and error handling
	ifstream fin(inputFile, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << inputFile << "\" could not be opened";
		exit(0);
		return;
	}

	ofstream fout(outputFile, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outputFile << "\" could not be created";
		exit(0);
		return;
	}

	// Convert
	convert(fin, fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outputFile;
}

void moveToFront::inverseDriver(const char inputFile[])
{
	// Dirver written to handle errors and file input-output

	cout << "Inverting Move to Front Algorithm..." << endl;

	// Creating the name of the output file based on the input
	char* outputFile = new char[strlen(inputFile)];
	strncpy(outputFile, inputFile, strlen(inputFile) - 4);
	outputFile[strlen(inputFile) - 4] = 0;

	// Stream creation and error handling
	ifstream fin(inputFile, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << inputFile << "\" could not be opened";
		exit(0);
		return;
	}

	ofstream fout(outputFile, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outputFile << "\" could not be created";
		exit(0);
		return;
	}

	// Invert
	inverse(fin, fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outputFile;
}
