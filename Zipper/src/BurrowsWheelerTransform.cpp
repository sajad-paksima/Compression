#include "../include/BurrowsWheelerTransform.h"


burrowsWheelerTransform::burrowsWheelerTransform()
{
	s = NULL;
}
burrowsWheelerTransform::~burrowsWheelerTransform()
{
	if (s)
		delete s;
	p.clear();
	c.clear();
	pn.clear();
	cn.clear();
	cnt.clear();
}

void burrowsWheelerTransform::readFile(ifstream& fin)
{
	// Read the Stream and Handle Neccesary Allocations
	// For the Suffix-Array Algorithm

	// Initialize
	if (s)
		delete s;
	fin.seekg(0, fin.end);
	int len = (int)fin.tellg();
	s = new char[len + 3];
	fin.seekg(0);

	char c;
	int ind = 0;
	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;
		s[ind++] = c;
	}
	// Save the Length of the File
	slen = ind;
}

void burrowsWheelerTransform::sortCyclicShifts()
{
	// Sorting the Cyclic Shifts O(nlgn)

	// Initialize
	int len = slen;
	cnt.resize(max(len, MAX_CHAR));
	fill(cnt.begin(), cnt.end(), 0);

	p.resize(len);
	pn.resize(len);
	c.resize(len);
	cn.resize(len);

	// First Step ans Initialization of the Radix-Sort
	//
	// Calculate the First p for Sorting
	for (int i = 0; i < len; i++)
		cnt[(unsigned char)s[i]]++;
	for (int i = 1; i < (int)cnt.size(); i++)
		cnt[i] += cnt[i - 1];
	for (int i = 0; i < len; i++)
		p[--cnt[(unsigned char)s[i]]] = i;

	// Calculate Classes
	c[p[0]] = 0;
	int classes = 1;
	for (int i = 1; i < len; i++)
	{
		if (s[p[i]] != s[p[i - 1]])
			classes++;
		c[p[i]] = classes - 1;
	}

	// Use the Radix-Sort Method lgn Times More to Sort the
	// Cyclic Shifts
	for (int h = 0; (1 << h) < len; h++)
	{
		// Place p into pn for Further Sorting
		for (int i = 0; i < len; i++)
			pn[i] = (p[i] - (1 << h) + len) % len;
		fill(cnt.begin(), cnt.begin() + classes, 0);
		for (int i = 0; i < len; i++)
			cnt[c[pn[i]]]++;
		for (int i = 1; i < classes; i++)
			cnt[i] += cnt[i - 1];

		// Stable Radix-Sort
		for (int i = len - 1; i >= 0; i--)
			p[--cnt[c[pn[i]]]] = pn[i];

		// Calculate New Classes
		cn[p[0]] = 0;
		classes = 1;
		for (int i = 1; i < len; i++)
		{
			pair<int, int> cur = make_pair(c[p[i]], c[(p[i] + (1 << h)) % len]);
			pair<int, int> prev = make_pair(c[p[i - 1]], c[(p[i - 1] + (1 << h)) % len]);
			if (cur != prev)
				classes++;
			cn[p[i]] = classes - 1;
		}
		c.swap(cn);
	}

	// Cleanup
	c.clear();
	cn.clear();
	pn.clear();
}

void burrowsWheelerTransform::writeOutTransform(ofstream& fout)
{
	// Saving the Transformed Form using s and the Calculated p
	// to the fout Stream
	int pos = -1;
	for (int i = 0; i < (int)p.size(); i++)
		if (p[i] == 0)
			pos = i;
	fout.write((char*)(&pos), sizeof(pos));
	for (int i = 0; i < (int)p.size(); i++)
		fout.write(&(s[(p[i] + p.size() - 1) % p.size()]), sizeof(s[0]));
}

void burrowsWheelerTransform::inverseReadFile(ifstream& fin)
{
	// Reading the Stream fin to Inverse it

	// Initialize
	if (s)
		delete s;
	fin.read((char*)(&index), sizeof(index));
	fin.seekg(0, fin.end);
	int len = fin.tellg();
	len -= sizeof(index);
	slen = len;
	fin.seekg(sizeof(index));

	s = new char[len + 3];
	char c;
	int ind = 0;

	while (!fin.eof())
	{
		fin.read(&c, sizeof(c));
		if (fin.eof())
			break;
		s[ind++] = c;
	}

	// Saving the File Length
	slen = ind;
}

void burrowsWheelerTransform::inverse()
{
	// Invert the Burrows-Wheeler Transform

	// Initialize
	int len = slen;
	char* t = new char[len];
	int* leftShift = new int[len];
	vector<stack<int> > occ(MAX_CHAR + 2);
	cnt.resize(MAX_CHAR + 2);
	fill(cnt.begin(), cnt.end(), 0);
	for (int i = 0; i < len; i++)
		cnt[(unsigned char)s[i]]++;

	// Sort s for Finding the First Column
	int ind = 0;
	for (int i = 0; i < (int)cnt.size(); i++)
		while (cnt[i]--)
			t[ind++] = i;

	// Initializing the Stacks in Order to Calculate the Answer Swiftly
	for (int i = len - 1; i >= 0; i--)
		occ[(unsigned char)s[i]].push(i);

	// Calculate the leftShift Array
	for (int i = 0; i < len; i++)
	{
		leftShift[i] = occ[(unsigned char)t[i]].top();
		occ[(unsigned char)t[i]].pop();
	}

	// Invert s
	s[0] = t[index];
	int now = leftShift[index];
	for (int i = 1; i < len; i++)
		s[i] = t[now], now = leftShift[now];

	// Cleanup
	delete t;
	delete leftShift;
	occ.clear();
}

void burrowsWheelerTransform::writeOutInverse(ofstream& fout)
{
	// Save the Inverse
	fout.write(s, sizeof(s[0]) * slen);
}

void burrowsWheelerTransform::transformDriver(const char inputFile[])
{
	// Driver Written to Handle Errors ans File Input-Output

	cout << "Applying the Burrows-Wheeler Transform..." << endl;

	// Creating Streams
	ifstream fin(inputFile, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << inputFile << "\" could not be opened";
		exit(0);
		return;
	}

	// Read the File and Sort the Cyclic Shifts
	readFile(fin);
	sortCyclicShifts();

	// Handle Output Naming Conventions and Create fout
	char* outName = new char[strlen(inputFile) + 5];
	strcpy(outName, inputFile);
	strcat(outName, ".bwt");

	ofstream fout(outName, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outName << "\" could not be created";
		exit(0);
		return;
	}

	// Save the Transform
	writeOutTransform(fout);

	// Cleanup
	fin.close();
	fout.close();
	delete outName;
}

void burrowsWheelerTransform::inverseDriver(const char inputFile[])
{
	// Driver Written to Handle Errors ans File Input-Output

	cout << "Inverting the Burrows-Wheeler Transform..." << endl;

	// Creating Streams
	ifstream fin(inputFile, ios::in | ios::binary);
	if (!fin.is_open())
	{
		cout << "The input file \"" << inputFile << "\" could not be opened";
		exit(0);
		return;
	}

	// Read fin and Invert it
	inverseReadFile(fin);
	inverse();

	// Handle Output Naming Conventions and Create fout
	char* outName = new char[strlen(inputFile)];
	strncpy(outName, inputFile, strlen(inputFile) - 4);
	outName[strlen(inputFile) - 4] = 0;

	ofstream fout(outName, ios::out | ios::binary);
	if (!fout.is_open())
	{
		cout << "The output file \"" << outName << "\" could not be created";
		exit(0);
		return;
	}

	// Save the Inverse
	writeOutInverse(fout);

	// Cleanup
	fout.close();
	fin.close();
	delete outName;
}
