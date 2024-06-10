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
#include "TestCase.h"

using namespace std;

int main(int argc, char* argv[]) {
;	string input_cmd;

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
	Logger& logger = Logger::GetInstance();
	if (argc == 1) {
		logger.write_Log(eLoggingOpt::ONLY_FILE, "main", "Console mode running...");
		while (1) {
			try {
				cout << "> ";
				getline(cin, input_cmd);
				if (input_cmd == "")
					continue;
				cp.command_parse(input_cmd);
				transform(cp.cmd.begin(), cp.cmd.end(), cp.cmd.begin(), ::toupper);
				invoker.execute(cp);
				if (cp.cmd == "EXIT") {
					logger.write_Log(eLoggingOpt::ONLY_FILE, "main", "Program Exit");
					break;
				}
			}
			catch (exception) {
				logger.write_Log(eLoggingOpt::ALL_PRINT, __FUNCTION__, "INVALID COMMA");
			}
		}
	}
	else {
		logger.write_Log(eLoggingOpt::ONLY_FILE, "main", "Script mode running...");
		Runner runner;
		bool run_flag = runner.readScenario(argv[1]);
		invoker.setRun(run_flag);
		if (run_flag == false)
			logger.write_Log(eLoggingOpt::ALL_PRINT, __FUNCTION__, "File Open Error");
		else {
			for (auto script : runner.scripts) {
				TestCase tc(script);
				logger.write_Log(eLoggingOpt::ALL_PRINT, __FUNCTION__, script + " --- Run...");
				if (!tc.execute(cp, invoker)) return 0;
			}
		}
	}
}
