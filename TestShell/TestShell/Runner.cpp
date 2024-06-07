#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Runner {
public:
	void readScenario(string file_name) {
		filename = file_name;
		ifstream inFile(filename);
		string script_line;
		while (getline(inFile, script_line)) {
			scripts.push_back(script_line);
		}
		inFile.close();
	}
	void runScenario() {
		for (auto script : scripts) {
			cout << script << "\n";
		}
	}
private:
	string filename;
	vector<string> scripts;
};