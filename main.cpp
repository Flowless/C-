#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <locale>

using namespace std;

void writeFile(vector<string> matches, const char* opath)
{
	ofstream file(opath);

	file << "MATCHES:" << endl;
	for (int i = 0; i < matches.size(); ++i)
	{
		file << matches[i] << endl;
	}
	file << endl << "****The words listed above are the ones" << endl << "that contained your search phrase.****" << endl;	

	cout << "Writing to " << opath << " was successful." << endl;

	file.close();
}

string getWord(string line, int pos) 
{
	int beg = 0;
	int end = 0;
	// Find start of word
	if (pos > 0)
	{
		for (int i = pos-1; i < line.length()-1; --i)
		{
			// Checking for characters that separates words typically
			if ( (line[i] == ' ') || (line[i] == '"') || (line[i] == '?') || (line[i] == '`') || (line[i] == ':') || (line[i] == '.') || (line[i] == '!') || (line[i] == ',') )
			{
				cout << "Start of word is index: " << i+1 << endl;
				beg = i+1;
				break;
			}
		}
	}
	// If it's the first word in string
	else {
		cout << "Start of word is index: 0" << endl;
	}
	// Find end of word 
	for (int i = pos+1; i < line.length(); ++i)
	{
		// Checking for characters that separates words typically
		if ( (line[i] == ' ') || (line[i] == '"') || (line[i] == '?') || (line[i] == '`') || (line[i] == ':') || (line[i] == '.') || (line[i] == '!') || (line[i] == ',') )
		{
			cout << "End of word is index: " << i-1 << endl;
			end = i;
			break;
		}
		// if line ends without an space-character
		else if (i == line.length()-1) 
		{
			end = i+1;
			break;
		}
	}

	cout << "Aaand the word is: " << line.substr(beg, end-beg) << endl;
	return line.substr(beg, end-beg);
}

vector<string> searchFile(string search, const char* ipath, int caseSense) 
{
	// Please do a try{} here, to check if proper connection was done
	ifstream file(ipath);

	// Vector with actual data in and temp string for transfer
	vector<string> data;
	string tmp_line;

	while (getline(file, tmp_line)) 
	{
		cout << "Pushing" << endl;
		// If the option -c was given when executing program
		if (caseSense == 0) {
			locale loc;
			// Convert all the incoming data to lowercase before continuing
			for (int i = 0; i < tmp_line.length()-1; ++i)
			{
				tmp_line[i] = tolower(tmp_line[i], loc);
			}
			// Convert the search-string to lowercase
			for (int i = 0; i < search.length()-1; ++i)
			{
				search[i] = tolower(tmp_line[i], loc);
			}

		}
		data.push_back(tmp_line);
	}

	for (int i = 0; i < data.size(); ++i)
	{
		cout << data[i] << endl;
	}
	// Gets the line, and uses find()
	int it = 0;
	vector<string> matches;
	for (int i = 0; i < data.size(); ++i)
	{
		// while - checking for multiple occurences on same line
		while (data[i].find(search, it) != ULLONG_MAX)
		{
			cout << "----------------------------------------------	" << endl;
			// Finding the word and storing in vector matches
			matches.push_back(getWord(data[i], data[i].find(search, it)));
			cout << "Str: " << data[i] << endl << "Pos: " << data[i].find(search, it) << endl;
			cout << "Found ya!" << " line nr: " << i+1 << endl;
			it = data[i].find(search, it) + 1;
			cout << "----------------------------------------------	" << endl;
		}
		it = 0;
	}

	file.close();

	return matches;

}

int main(int argc, char const *argv[]) 
{
	// Check if enough arguments were given
	if (argc <= 1) 
	{
		cout << "Usage: ./find [string] -i [input_file] -o [output_file] {-c}" << endl;
		cout << "For more help read the READ_ME file." << endl;
		return 0; 
	}

	for (int i = 0; i < argc; ++i) 
	{
		cout << "Arg nr " << i << ": " << argv[i] << endl;
	}

	// Put the CLI-arguments inside a vector for easier managing
	vector<string> args;
	for (int i = 0; i < argc-1; ++i)
	{
		args.push_back(argv[i+1]);
	}

	string search = argv[1];
	const char* opath = "";
	const char* inPath = "";

	int caseSense = 1;
	// Finding all the options
	for (int i = 0; i < args.size(); ++i)
	{
		// Find the -i option
		if (args[i] == "-i")
		{
			// add file path to variable
			inPath = argv[i+2];
			cout << "Input path: " << inPath << endl;
		}
		// Find the -o option
		if (args[i] == "-o")
		{
			// add file path to variable
			opath = argv[i+2];
			cout << "Output path: " << opath << endl;
		}
		// Find the -c option
		if (args[i] == "-c")
		{
			caseSense = 0;
			cout << "NOT case sensitive." << endl;
		}
	}
	// Checking if -i was found, if not read from stdin
	if (inPath == "")
	{
		cout << "No input file given, reading from stdin.." << endl;
		string tmp;
		cin >> tmp;
		// converting string -> const char*
		inPath = tmp.c_str();
	}
	vector<string> matches;
	
	matches = searchFile(search, inPath, caseSense);
	// Checking if -o was found, if not write to stdout.
	if (opath == "")
	{	
		cout << "No output file given, writing to stdout.." << endl;
		for (int i = 0; i < matches.size(); ++i)
		{
			cout << matches[i] << endl;
		}
	}
	else 
	{
	// Write to file
	writeFile(matches, opath);
	}

	return 0;
}