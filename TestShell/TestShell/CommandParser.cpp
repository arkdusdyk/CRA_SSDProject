#include <stdexcept>
#include <iostream>
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

		if (tokens.size() == 1) {			//	exit, help, fullread
			cmd = tokens[0];
			if ((cmd == "exit") || (cmd == "help") || (cmd == "fullread") || (cmd == "testapp1")||(cmd == "testapp2")) {
				//cout << "cmd : " << cmd << "\n";
			}
			else
				throw exception("INVALID COMMAND");
		}
		else if (tokens.size() == 2) {		// read LBA, fullwrite data
			cmd = tokens[0];
			if (cmd == "read") {
				if (isLBAint(tokens[1]) == false)
					throw exception("LBA out of range");
				if (isLBAinRange(tokens[1]) == false)
					throw exception("LBA out of range");
				lba = stoi(tokens[1]);
				//cout << "cmd : " << cmd << " lba : " << lba << "\n";
			}
			else if (cmd == "fullwrite") {
				if (isDataValid(tokens[2]) == false)
					throw exception("Data out of range");
				data = tokens[2];
				//cout << "cmd : " << cmd << " data : " << lba << "\n";
			}
			else
				throw exception("INVALID COMMAND");
		}
		else if (tokens.size() == 3) {		// write LBA data
			cmd = tokens[0];
			if (cmd == "write") {
				if (isLBAint(tokens[1]) == false)
					throw exception("LBA out of range");
				if (isLBAinRange(tokens[1]) == false)
					throw exception("LBA out of range");
				lba = stoi(tokens[1]);
				if (isDataValid(tokens[2]) == false)
					throw exception("Data out of range");
				data = tokens[2];
				//cout << "cmd : " << cmd << " lba : " << lba << " data : " << data << "\n";
			}
			else
				throw exception("INVALID COMMAND");
		}
		else if(tokens.size() > 3)
			throw exception("INVALID COMMAND");
	}

	bool isLBAint(string LBA_string) {
		if (LBA_string.length() > 2)
			return false;
		bool flag = true;
		for (int i = 0; i < LBA_string.length(); i++) {
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

	bool isDataValid(string check_data) {
		if (check_data.length() != 10)
			return false;
		if (check_data[0] == '0' && check_data[1] == 'x') {
			bool flag = true;
			for (int i = 2; i < 10; i++) {
				if ((('A' <= check_data[i]) && (check_data[i] <= 'F')) || (('0' <= check_data[i]) && (check_data[i] <= '9')))	continue;
				else {
					flag = false;
					break;
				}
			}
			if (flag == true)
				return true;
		}
		return false;
	}
};