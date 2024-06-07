#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"

using namespace std;
int main() {
	string input_cmd;
	TestShell ts;
	ts.createProduct("SSD");
	CommandParser cp;
	while (1) {
		try {
			cout << "> ";
			getline(cin, input_cmd);
			if (input_cmd == "")
				continue;
			cp.command_parse(input_cmd);
			transform(cp.cmd.begin(), cp.cmd.end(), cp.cmd.begin(), ::toupper);
			if (cp.cmd == "EXIT")
				break;
			else if (cp.cmd == "HELP")
				ts.Help();
			else if (cp.cmd == "READ")
				ts.Read(cp.lba);
			else if (cp.cmd == "WRITE")
				ts.Write(cp.lba, cp.data);
			else if (cp.cmd == "ERASE")
				ts.Erase(cp.lba, cp.size);
			else if(cp.cmd=="ERASE_RANGE")
				ts.EraseRange(cp.lba, cp.endLba);
			else if (cp.cmd == "FULLWRITE")
				ts.FulllWrite(cp.data);
			else if (cp.cmd == "FULLREAD")
				ts.FullRead();
			else if (cp.cmd == "TESTAPP1") {
				string msg = "testApp1 ";
				msg.append(ts.testApp1() ? "PASS" : "FAIL");
				cout << msg << endl;
			} else if (cp.cmd == "TESTAPP2") {
				string msg = "testApp2 ";
				msg.append(ts.testApp2() ? "PASS" : "FAIL");
				cout << msg << endl;
			}
		}
		catch (exception) {
			cout << "INVALID COMMAND\n";
		}
	}
}
