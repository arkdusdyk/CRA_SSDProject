#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"

using namespace std;
int main() {
	string input_cmd;
	TestShell ts;
	CommandParser cp;
	while (1) {
		try {
			getline(cin, input_cmd);
			cp.command_parse(input_cmd);
			if (cp.cmd == "exit")
				break;
			else if (cp.cmd == "help")
				cout << "���� �߰� ����\n";
			else if (cp.cmd == "read")
				ts.Read(cp.lba);
			else if (cp.cmd == "write")
				ts.Write(cp.lba, cp.data);
			else if (cp.cmd == "fullwrite")
				ts.FulllWrite(cp.data);
			else if (cp.cmd == "fullread")
				ts.FullRead();
		}
		catch (exception) {
			cout << "INVALID COMMAND\n";
		}
	}
}