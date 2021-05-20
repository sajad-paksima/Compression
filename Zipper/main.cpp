#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "./include/MoveToFront.h"
#include "./include/PackBits.h"
#include "./include/BurrowsWheelerTransform.h"
#include "./include/tarr.h"
#include "./include/Huffman.h"

using namespace std;
using namespace std::chrono;

void printUsageAndExit()
{
	// In-App Usage and Help
	cout << "Usage: zipper [Options] [Args]" << endl << endl;
	cout << "Where Options are of the form \"mabcd\", and can have a length between 2 and 5, inclusive." << endl;
	cout << "The m option is required, and can have the values c and x, standing for create and eextract, respectively." << endl << endl;
	cout << "The a and b options are optional, and can either have the values of m or b, each standing for" << endl;
	cout << "\"Move to Front \" and \"Burrows-Wheeler Transform\", respectively." << endl;
	cout << "The a and b options are executed in the order inputted, and cannot have the save value, but can be omitted." << endl << endl;
	cout << "The c option is a requirement and can have the values of p or h, standing for \"PackBits\" and \"Huffman\"" << endl;
	cout << "respectively" << endl << endl;
	cout << "The d option is optional, and can only have the o value. If included, the last string in the input will be" << endl;
	cout << "the name of the result of the program. Cannot be used with the extraction command." << endl << endl;
	cout << "Note: In case of extraction, just input the same abc options used when compression was performed." << endl;

	exit(1);
}

long long getFileSize(const char fileName[])
{
	// Getting File Sizes
	ifstream fin(fileName, ios::in | ios::binary);
	fin.seekg(0, fin.end);
	long long res = fin.tellg();
	fin.seekg(0);
	res -= fin.tellg();
	fin.close();
	return res;
}

bool fileExists(const char fileName[])
{
    ifstream fin(fileName, ios::in | ios::binary);
    bool res = fin.is_open();
    fin.close();
    return res;
}

void programDriver(int fileCount, char** fileList, bool command, int pMTF, int pBWT, bool bPackBits, bool bHuffman, bool bOutputName, string& outputName)
{
	// Drive All Functions and Classes to Work Together

	// Initialize
	tarr tarrAgent;
	moveToFront mtf;
	burrowsWheelerTransform bwt;
	static huffman hCompression;
	packBits pCompression;
	auto timeStart = high_resolution_clock::now(); // Starting Time
	long long iBytes = 0, fBytes = 0;
	if (command)
	{
		// Getting Input Size
		iBytes = getFileSize(outputName.c_str());

		if (bHuffman)
		{
			hCompression.decompressDriver(outputName.c_str());
			outputName = outputName.substr(0, outputName.size() - 4);
		}
		else
		{
			pCompression.decompressDriver(outputName.c_str());
			outputName = outputName.substr(0, outputName.size() - 4);
		}


		if (pMTF != -1 && pBWT != -1)
		{
			if (pMTF < pBWT)
			{
				bwt.inverseDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName = outputName.substr(0, outputName.size() - 4);

				mtf.inverseDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName = outputName.substr(0, outputName.size() - 4);
			}
			else
			{
				mtf.inverseDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName = outputName.substr(0, outputName.size() - 4);

				bwt.inverseDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName = outputName.substr(0, outputName.size() - 4);
			}
		}
		else if (pMTF != -1)
		{
			mtf.inverseDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName = outputName.substr(0, outputName.size() - 4);
		}
		else if (pBWT != -1)
		{
			bwt.inverseDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName = outputName.substr(0, outputName.size() - 4);
		}

		// Getting Output Size
		fBytes = getFileSize(outputName.c_str());

		tarrAgent.extract(outputName.c_str());
		remove(outputName.c_str());
		outputName = outputName.substr(0, outputName.size() - 5);
	}
	else
	{
		tarrAgent.merge(fileList, fileCount, outputName.c_str());
		outputName += ".tarr";

		// Getting Input Size
		iBytes = getFileSize(outputName.c_str());

		if (pMTF != -1 && pBWT != -1)
		{
			if (pMTF < pBWT)
			{
				mtf.convertDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName += ".mtf";
				bwt.transformDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName += ".bwt";
			}
			else
			{
				bwt.transformDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName += ".bwt";
				mtf.convertDriver(outputName.c_str());
				remove(outputName.c_str());
				outputName += ".mtf";
			}
		}
		else if (pMTF != -1)
		{
			mtf.convertDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName += ".mtf";
		}
		else if (pBWT != -1)
		{
			bwt.transformDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName += ".bwt";
		}

		if (bHuffman)
		{
			hCompression.compressDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName += ".mbz";
		}
		else
		{
			pCompression.compressDriver(outputName.c_str());
			remove(outputName.c_str());
			outputName += ".pkb";
		}

		// Getting Output Size
		fBytes = getFileSize(outputName.c_str());
	}
	// Stopping Time and Calculating Duration
	auto timeStop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(timeStop - timeStart);

	// Sitrep
	cout << endl << "Status:" << endl;
	cout << "\tTruned " << iBytes << " Bytes to " << fBytes << " Bytes" << endl;
	cout << "\tThe Operation took " << duration.count() << " microseconds." << endl;
}

void handleCommandLineArguments(int argc, char** argv)
{
	// Handling Command-Line Input
	bool command = false, bPackBits = false, bHuffman = false, bOutputName = false;
	int pMTF = -1, pBWT = -1;
	string outputName = "";

	if (strlen(argv[1]) > 5 || strlen(argv[1]) < 2)
		printUsageAndExit();
	for (int i = 0; i < (int)strlen(argv[1]); i++)
	{
		if (argv[1][i] == 'c')
		{
			if (i)
				printUsageAndExit();
			command = false;
		}
		else if (argv[1][i] == 'x')
		{
			if (i)
				printUsageAndExit();
			command = true;
		}
		else if (argv[1][i] == 'm')
		{
			if (bOutputName || pMTF != -1)
				printUsageAndExit();
			if (bHuffman || bPackBits)
				printUsageAndExit();
			if (!i)
				printUsageAndExit();
			pMTF = i;
		}
		else if (argv[1][i] == 'b')
		{
			if (bOutputName || pBWT != -1)
				printUsageAndExit();
			if (bHuffman || bPackBits)
				printUsageAndExit();
			if (!i)
				printUsageAndExit();
			pBWT = i;
		}
		else if (argv[1][i] == 'p')
		{
			if ((bPackBits || bHuffman) || bOutputName)
				printUsageAndExit();
			if (!i)
				printUsageAndExit();
			bPackBits = true;
		}
		else if (argv[1][i] == 'h')
		{
			if ((bPackBits || bHuffman) || bOutputName)
				printUsageAndExit();
			if (!i)
				printUsageAndExit();
			bHuffman = true;
		}
		else if (argv[1][i] == 'o')
		{
			if (bOutputName)
				printUsageAndExit();
			if (!bHuffman && !bPackBits)
				printUsageAndExit();
			if (!i)
				printUsageAndExit();
			bOutputName = true;
		}
		else
			printUsageAndExit();
	}
	if (command && bOutputName)
		printUsageAndExit();
	if (bOutputName)
		for (int i = 0; i < (int)strlen(argv[argc - 1]); i++)
			outputName += argv[argc - 1][i];
	if (bOutputName && argc == 3)
	{
		cout << "Please select a file, if you want to name the output" << endl;
		exit(1);
	}

	if (!bOutputName && argc > 3)
	{
		cout << "Please enter an output name in case you want to compress multiple files" << endl;
		exit(1);
	}

	if (!bOutputName && argc == 3)
		for (int i = 0; i < (int)strlen(argv[argc - 1]); i++)
			outputName += argv[argc - 1][i];

	programDriver(argc - 2 - bOutputName, (argv + 2), command, pMTF, pBWT, bPackBits, bHuffman, bOutputName, outputName);
}

void interactiveMenu()
{
	// Handling the Interactive Menu
	bool LOOP = true;
	while (LOOP)
	{
		LOOP = false;
		bool command = false, bPackBits = false, bHuffman = false, bOutputName = false;
		int pMTF = -1, pBWT = -1, mode;
		string outputName, inp;
		vector<string> files;
		char** fileList;

		cout << "Please select your command: (0: Compress, 1: Decompress)" << endl;
		do
		{
			cin >> mode;
			if (mode < 0 || mode > 1)
				cout << endl << "Please enter a valid value" << endl;
		} while (mode < 0 || mode > 1);
		command = mode;

		cout << "Enhancement Codes: (0: None) (1: Move to Front) (2: Burrows-Wheeler Transform)" << endl;
		cout << "                   (3: Move to Front, then Burrows-Wheeler Transform)" << endl;
		cout << "                   (4: Burrows-Wheeler Transform, then Move to Front)" << endl;
		cout << "Please Enter Your Desired Enhancement Code: " << endl;
		do
		{
			cin >> mode;
			if (mode < 0 || mode > 4)
				cout << endl << "Please enter a valid value" << endl;
		} while (mode < 0 || mode > 4);

		if (mode == 1)
			pMTF = 1;
		else if (mode == 2)
			pBWT = 1;
		else if (mode == 3)
			pMTF = 1, pBWT = 2;
		else if (mode == 4)
			pBWT = 1, pMTF = 2;

		cout << "Please Select Your Desired Algorithm: (0: PackBits, 1: Huffman)" << endl;
		do
		{
			cin >> mode;
			if (mode < 0 || mode > 1)
				cout << endl << "Please enter a valid value" << endl;
		} while (mode < 0 || mode > 1);

		if (mode)
			bHuffman = true;
		else
			bPackBits = true;

		if (!command)
		{
			cout << "Do you want to compress multiple files or name the output archive? (y/n)" << endl;
			char C;
			do
			{
				cin >> C;
				if ('A' <= C && C <= 'Z')
					C += (int)('a' - 'A');
				if (C != 'y' && C != 'n')
					cout << "Please enter a valid input" << endl;
			} while (C != 'y' && C != 'n');

			mode = (C == 'y');

			if (mode)
			{
				bOutputName = true;
				cout << "Please enter your desired archive name: " << endl;
				getline(cin, outputName);
				if (outputName == "" || outputName == "\n")
					getline(cin, outputName);
			}
		}

		if (!command)
		{
			if (bOutputName)
			{
				cout << "Please enter the names of the files you want to compress, type -1 at the end" << endl;
				do
				{
					getline(cin, inp);
					if (inp == "" || inp == "\n")
						continue;
                    if (inp != "-1" && !fileExists(inp.c_str()))
                    {
                        cout << "The file that you entered does not exist, please enter a valid input" << endl;
                        continue;
                    }
					if (inp != "-1")
						files.push_back(inp);
				} while (inp != "-1");
			}
			else
			{
				cout << "Please enter the name of the file you want to compress: " << endl;
				do
				{
                    getline(cin, inp);
                    if (inp == "" || inp == "\n")
                        getline(cin, inp);
                    if (!fileExists(inp.c_str()))
                    {
                        cout << "The file that you entered does not exist, please enter a valid input" << endl;
                        continue;
                    }
				} while (!fileExists(inp.c_str()));
				files.push_back(inp);
			}
		}
		else
		{
			cout << "Please enter the name of the archive you want to decomperess: " << endl;
			do
			{
                getline(cin, inp);
                if (inp == "" || inp == "\n")
                    getline(cin, inp);
                if (!fileExists(inp.c_str()))
                {
                    cout << "The file that you entered does not exist, please enter a valid input" << endl;
                    continue;
                }
			} while (!fileExists(inp.c_str()));
			files.push_back(inp);
		}
		fileList = new char*[files.size()];
		for (int i = 0; i < (int)files.size(); i++)
		{
			fileList[i] = new char[files[i].size() + 3];
			for (int j = 0; j < (int)files[i].size(); j++)
				fileList[i][j] = files[i][j];
		}

		if (files.size() > 1 && !bOutputName)
		{
			cout << "Please enter an output name in case you want to compress multiple files" << endl;
			exit(1);
		}

		if (!bOutputName && files.size() == 1)
			outputName = files[0];

		programDriver(files.size(), fileList, command, pMTF, pBWT, bPackBits, bHuffman, bOutputName, outputName);

		// Cleanup
		files.clear();
		for (int i = 0; i < (int)files.size(); i++)
			delete fileList[i];
		delete fileList;

		// Handling the Looping of the Menu
		cout << "Do you still want to use this program? (y/n)" << endl;
		char C;
		do
		{
			cin >> C;
			if ('A' <= C && C <= 'Z')
				C += (int)('a' - 'A');
			if (C != 'y' && C != 'n')
				cout << "Please enter a valid input" << endl;
		} while (C != 'y' && C != 'n');
		LOOP = (C == 'y');
	}
}

int main(int argc, char** argv) // Command-Line Arguments
{
	if (argc == 2)
		printUsageAndExit();
	if (argc > 2)
		handleCommandLineArguments(argc, argv);
	else
		interactiveMenu();
	return 0;
}
