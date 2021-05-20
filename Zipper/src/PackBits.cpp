#include "../include/PackBits.h"

packBits::packBits()
{
	tmp = new char[MAX_CHAR];
}
packBits::~packBits()
{
	delete tmp;
}

void packBits::decompress(ifstream& fin, ofstream& fout)
{
	// Packbits Decompression Algorithm

	char c;
	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;
		if (c < 0)
		{
			char harf;
			fin.read(&harf, sizeof(harf));
			for (int i = 0; i < (-1) * c; i++)
				fout.write(&harf, sizeof(harf));
		}
		else if (c > 0)
		{
			char harf;
			for (int i = 0; i < c; i++)
			{
				fin.read(&harf, sizeof(harf));
				fout.write(&harf, sizeof(harf));
			}
		}
	}
}

void packBits::compress(ifstream& fin, ofstream& fout)
{
	// Packbits Compression Algorithm

	unsigned char counter = 1, j = 0;
	char c, harf, buffer;
	fin.read(&harf, sizeof(harf));
	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;

		if (c == harf) // Equality With the Last Character
		{
			counter++;
			if (counter == MAX_PACKBITS_LIMIT + 1) // Max Equality Count
			{
				counter--;
				buffer = counter;
				buffer *= -1;
				fout.write(&buffer, sizeof(buffer));
				fout.write(&harf, sizeof(harf));
				counter = 1;
			}
			if (j != 0) // Writing Out the Inequality String
			{
				buffer = j;
				fout.write(&buffer, sizeof(buffer));
				fout.write(tmp, sizeof(tmp[0]) * j);
				for (int i = 0; i < j; i++)
					tmp[i] = 0;
				j = 0;
			}

		}
		else // Inequality With the Last Character
		{
			if (counter != 1 && counter <= MAX_PACKBITS_LIMIT) // Writing Out the Equal Characters
			{
				buffer = counter;
				buffer *= -1;
				fout.write(&buffer, sizeof(buffer));
				fout.write(&harf, sizeof(harf));
				harf = c;
				counter = 1;
			}
			else
			{
				if (j == MAX_PACKBITS_LIMIT) // Max Inequality Count
				{
					buffer = MAX_PACKBITS_LIMIT;
					fout.write(&buffer, sizeof(buffer));
					fout.write(tmp, sizeof(tmp[0]) * j);
					for (int i = 0; i < j; i++)
						tmp[i] = 0;
					j = 0;
				}
				tmp[j++] = harf;
				harf = c;
			}
		}
	}

	// Last Output Left and Cleanup
	if (j == 0)
	{
		buffer = counter;
		buffer *= -1;
		fout.write(&buffer, sizeof(buffer));
		fout.write(&harf, sizeof(harf));
	}
	else if (j != 0)
	{
		tmp[j++] = harf;
		buffer = j;
		fout.write(&buffer, sizeof(buffer));
		fout.write(tmp, sizeof(tmp[0]) * j);
	}
}

void packBits::compressDriver(const char inputFile[])
{
	// Driver Written for Error Handling and File Input-Output

	// Initialize
	char* outputFile = new char[strlen(inputFile) + 5];
	strcpy(outputFile, inputFile);
	strcat(outputFile, ".pkb");

	// Creating Streams
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

	// Compress
	cout << "Compressing " << inputFile << " to " << outputFile << endl;
	compress(fin, fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outputFile;
}

void packBits::decompressDriver(const char inputFile[])
{
	// Driver Written for Error Handling and File Input-Output

	// Initialize
	char* outputFile = new char[strlen(inputFile)];
	strncpy(outputFile, inputFile, strlen(inputFile) - 4);
	outputFile[strlen(inputFile) - 4] = 0;

	// Creating Streams
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

	// Decompress
	cout << "Decompressing from " << inputFile << " to " << outputFile << endl;
	decompress(fin, fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outputFile;
}
