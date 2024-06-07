#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Runner {
public:
	string filename;
	vector<string> scripts;

	bool readScenario(string file_name) {
		filename = file_name;
		ifstream inFile(filename);
		
		if (inFile.fail())
			return false;

		string script_line;
		while (getline(inFile, script_line)) {
			scripts.push_back(script_line);
		}
		inFile.close();
		return true;
	}
};