#include <stdexcept>
#include <iostream>
#include <string>
#include "CommandParser.cpp"
#include "Logger.h"
#include "invoker.cpp"
#include "ReadCommand.cpp"
#include "WriteCommand.cpp"
#include "HelpCommand.cpp"
#include "EraseCommand.cpp"
#include "EraseRangeCommand.cpp"
#include "FullReadCommand.cpp"
#include "FullWriteCommand.cpp"

using namespace std;

int main() {
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

	CommandParser cp;
	Logger &logger = Logger::GetInstance();
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
