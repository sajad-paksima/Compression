#include "../include/tarr.h"

void tarr::merge(char** inputFiles, int numberOfFiles, const char archiveName[])
{
	// Merge All of the Input Files to One Output File

	// Initialize and Error Check Output Stream Creation
	char* fullArchiveName = new char[strlen(archiveName) + 7];
	strcpy(fullArchiveName, archiveName);
	strcat(fullArchiveName, ".tarr");

	ofstream fout(fullArchiveName, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << archiveName << "\" could not be created";
		exit(0);
		return;
	}

	char c;
	long long byteCount;
	for (int i = 0; i < numberOfFiles; i++)
	{
		// Error Check Input Stream Creation
		ifstream fin(inputFiles[i], ios::in | ios::binary);
		if (!fin.is_open())
		{
			cout << "The input file \"" << inputFiles[i] << "\" could not be opened";
			exit(0);
			continue;
		}

		// Add the File to The Archive
		fin.seekg(0, fin.end);
		byteCount = fin.tellg();
		fin.seekg(0);

		c = '/';

		fout.write(inputFiles[i], sizeof(inputFiles[i][0]) * strlen(inputFiles[i]));
		fout.write(&c, sizeof(c));
		fout.write((char*)(&byteCount), sizeof(byteCount));

		cout << "Adding to " << fullArchiveName << ": " << inputFiles[i] << endl;

		while (!fin.eof())
		{
			fin.read(&c, sizeof(c));
			fout.write(&c, sizeof(c));
		}

		// Close the Current File
		fin.close();
	}

	// Cleanup
	fout.close();
	delete fullArchiveName;
}

void tarr::extract(const char inputFile[])
{
	// Extract All Files in a tarr Archive

	// Stream Creation and Error Checking
	ifstream fin(inputFile, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << inputFile << "\" could not be opened";
		exit(0);
		return;
	}

	// Initialize
	vector<char> filename;
	char c;
	char* currentName;
	long long fileLen;

	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;

		// Read the File Name
		while (c != '/')
		{
			filename.push_back(c);
			fin.read(&c, sizeof(c));
			if (fin.eof())
				break;
		}
		if (fin.eof())
			break;

		// Current File Naming Convention Handling
		currentName = new char[filename.size() + 1];
		for (int i = 0; i < (int)filename.size(); i++)
			currentName[i] = filename[i];
		currentName[filename.size()] = '\0';

		cout << "Extracting from " << inputFile << ": " << currentName << endl;

		// Read the Length of the Current File
		fin.read((char*)(&fileLen), sizeof(fileLen));
		fileLen++;
		if (fin.eof())
			break;

		// Output Creation and Error Checking
		ofstream fout(currentName, ios::out | ios::binary);
		if (!fout.is_open())
		{
			cout << "The output file \"" << currentName << "\" could not be created";
			exit(0);
			return;
		}

		// Extract the File
		for (long long i = 0; i < fileLen; i++)
		{
			fin.read(&c, sizeof(c));
			fout.write(&c, sizeof(c));
		}

		// Cleanup the Current File
		fout.close();
		delete currentName;
		filename.clear();
	}

	// Cleanup
	fin.close();
}
