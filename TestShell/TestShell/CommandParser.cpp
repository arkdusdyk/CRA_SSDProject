#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

class CommandParser {
public:
	void command_parse(string cmd_line) {
		vector<string> tokens;
		tokens.clear();
		int prev;
		int cur = cmd_line.find(' ');
		while (cur != string::npos) {
			string substring = cmd_line.substr(prev, cur - prev);
			tokens.push_back(substring);
			prev = cur + 1;
			cur = cmd_line.find(' ', prev);
		}
		if (tokens.empty())
			throw exception("No Command");
		else if (tokens.size() == 1)
			cmd = tokens[0];
		else if (tokens.size() == 2)
			cmd = tokens[1];
		else if (tokens.size() == 3)
			cmd = tokens[2];
		else
			throw exception("Check Argument");
	}
private:
	string cmd;
	int lba;
	string data;
};