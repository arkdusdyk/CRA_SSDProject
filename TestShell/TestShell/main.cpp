#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"

using namespace std;
int main() {
	string input_cmd;
	CommandParser cp;
	while (1) {
		try {
			getline(cin, input_cmd);
			cp.command_parse(input_cmd);
		}
		catch (exception) {
			cout << "INVALID COMMAND\n";
		}

		if (cp.cmd == "exit")
			break;
		else if (input_cmd == "help")
			cout << "도움말 추가 예정\n";

	}
}