#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TestShell {
public:
	void Write(int arr, string value) {
	}
	string Read(int arr) {
		return "";
	}

	void FulllWrite( string value) {

	}

	string FullRead(string value) {

	}

private:
	string cmd;
	int lba;
	string data;
	void command_parse(string cmd_line){
		int prev = 0;
		int cur = 0;
		vector<string> tokens;
		tokens.clear();
		cur = cmd_line.find(' ');
		while (cur != string::npos) {
			string substring = cmd_line.substr(prev, cur - prev);
			tokens.push_back(substring);
			prev = cur + 1;
			cur = cmd_line.find(' ', prev);
		}
		if (tokens.empty())
			throw exception("No Command\n");
		cmd = tokens[0];

		if (cmd == "write" || cmd == "read" || cmd == "fullwrite" || cmd == "fullread") {
			if (cmd == "write") {
			}
			else if (cmd == "read") {

			}
			else if (cmd == "fullwrite") {

			}
			else if (cmd == "fullread") {

			}
		}
		else if (cmd == "exit" || cmd == "help") {
		}
		else
			throw exception("Invalid Command");
	}
};