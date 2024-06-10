#include <stdexcept>
#include <iostream>
#include <string>
#include "CommandParser.cpp"
#include "Logger.h"
#include "invoker.cpp"
#include "Runner.cpp"
#include "ReadCommand.cpp"
#include "WriteCommand.cpp"
#include "HelpCommand.cpp"
#include "EraseCommand.cpp"
#include "EraseRangeCommand.cpp"
#include "FullReadCommand.cpp"
#include "FullWriteCommand.cpp"
#include "TestApp1Command.cpp"
#include "TestApp2Command.cpp"

using namespace std;

int main(int argc, char* argv[]) {
	string input_cmd;

	CommandInvoker invoker;
	invoker.createProduct("SSD");
	invoker.addCommand(move(make_unique<ReadCommand>()));
	invoker.addCommand(move(make_unique<WriteCommand>()));
	invoker.addCommand(move(make_unique<HelpCommand>()));
	invoker.addCommand(move(make_unique<EraseCommand>()));
	invoker.addCommand(move(make_unique<EraseRangeCommand>()));
	invoker.addCommand(move(make_unique<FullReadCommand>()));
	invoker.addCommand(move(make_unique<FullWriteCommand>()));
	invoker.addCommand(move(make_unique<TestApp1Command>()));
	invoker.addCommand(move(make_unique<TestApp2Command>()));


	CommandParser cp;
	Logger& logger = Logger::GetInstance();
	if (argc == 1) {
		while (1) {
			try {
				cout << "> ";
				getline(cin, input_cmd);
				if (input_cmd == "")
					continue;
				cp.command_parse(input_cmd);
				transform(cp.cmd.begin(), cp.cmd.end(), cp.cmd.begin(), ::toupper);
				invoker.execute(cp);
				if (cp.cmd == "EXIT")
					break;
			}
			catch (exception) {
				cout << "INVALID COMMAND\n";
			}
		}
	}
	else {
		Runner runner;
		bool run_flag = runner.readScenario(argv[1]);
		invoker.setRun(run_flag);
		if (run_flag == false)
			cout << "File Open Error\n";
		else {
			for (auto script : runner.scripts) {
				try {
					cout << script << " --- Run...";
					cp.command_parse(script);
					transform(script.begin(), script.end(), script.begin(), ::toupper);
					invoker.execute(cp);
					cout << "Pass" << "\n";
				}
				catch (exception) {
					cout << "FAIL!" << "\n";
					break;
				}
			}
		}
	}
}
