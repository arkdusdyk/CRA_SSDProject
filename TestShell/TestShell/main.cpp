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
			getline(cin, input_cmd);
			cp.command_parse(input_cmd);
			if (cp.cmd == "exit")
				break;
			else if (cp.cmd == "help")
				cout << "도움말 추가 예정\n";
			else if (cp.cmd == "read")
				ts.Read(cp.lba);
			else if (cp.cmd == "write")
				ts.Write(cp.lba, cp.data);
			else if (cp.cmd == "fullwrite")
				ts.FulllWrite(cp.data);
			else if (cp.cmd == "fullread")
				ts.FullRead();
			else if (cp.cmd == "testapp1") {
				string msg = "testApp1 ";
				msg.append(ts.testApp1() ? "PASS" : "FAIL");
				cout << msg << endl;
			} else if (cp.cmd == "testapp2") {
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