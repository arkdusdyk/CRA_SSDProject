#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"
#include "Runner.cpp"

using namespace std;
int main(int argc, char* argv[]) {
	TestShell ts;
	ts.createProduct("SSD");
	if (argc == 1) {
		string input_cmd;
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
				else if (cp.cmd == "FULLWRITE")
					ts.FulllWrite(cp.data);
				else if (cp.cmd == "FULLREAD")
					ts.FullRead();
				else if (cp.cmd == "TESTAPP1") {
					string msg = "testApp1 ";
					msg.append(ts.testApp1() ? "PASS" : "FAIL");
					cout << msg << endl;
				}
				else if (cp.cmd == "TESTAPP2") {
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
	else {
		Runner runner;
		bool run_flag = runner.readScenario(argv[1]);
		ts.setRunMode(run_flag);
		if (run_flag == false)
			cout << "File Open Error\n";
		else {
			for (auto script : runner.scripts) {
				bool result = false;
				cout << script << " --- Run...";
				transform(script.begin(), script.end(), script.begin(), ::toupper);
				if (script == "TESTAPP1") {
					result = ts.testApp1();
				}
				else if (script == "TESTAPP2") {
					result = ts.testApp2();
				}
				cout << (result ? "Pass" : "FAIL!") << "\n";
				if (result == false)
					break;
			}
		}
	}
}
