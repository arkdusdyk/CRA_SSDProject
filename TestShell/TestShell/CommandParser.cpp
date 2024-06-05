#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

class CommandParser {
public:
	string cmd;
	int lba;
	string data;
	
	void command_parse(string cmd_line) {
		if (cmd_line == "")
			throw exception("No Command");
		vector<string> tokens;
		tokens.clear();
		int prev = 0;
		int cur = cmd_line.find(' ');
		while (cur != string::npos) {
			string substring = cmd_line.substr(prev, cur - prev);
			tokens.push_back(substring);
			prev = cur + 1;
			cur = cmd_line.find(' ', prev);
		}
		tokens.push_back(cmd_line.substr(prev, cur - prev));

		if (tokens.size() == 1)				//	exit, help, fullwrite, fullread
			cmd = tokens[0];
		else if (tokens.size() == 2) {		// read LBA
			cmd = tokens[0];
			if (isLBAint(tokens[1]) == false)
				throw exception("LBA out of range");
			if (isLBAinRange(tokens[1]) == false)
				throw exception("LBA out of range");
			lba = stoi(tokens[1]);
		}
		else if (tokens.size() == 3) {		// write LBA data
			cmd = tokens[0];
			if (isLBAint(tokens[1]) == false)
				throw exception("LBA out of range");
			if (isLBAinRange(tokens[1]) == false)
				throw exception("LBA out of range");
			lba = stoi(tokens[1]);
		}
		else if(tokens.size() > 3)
			throw exception("INVALID COMMAND");
	}

	bool isLBAint(string LBA_string) {
		if (LBA_string.size() > 2)
			return false;
		bool flag = true;
		for (int i = 0; i < LBA_string.size(); i++) {
			if (LBA_string[i] < '0' || LBA_string[i] > '9') {
				flag = false;
				break;
			}
		}
		return flag;
	}

	bool isLBAinRange(string check_lba) {
		if ((0 <= stoi(check_lba)) && (stoi(check_lba) < 100))
			return true;
		return false;
	}
};