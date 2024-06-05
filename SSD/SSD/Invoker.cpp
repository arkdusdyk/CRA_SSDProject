#include <vector>
#include <memory>
#include "ICommand.h"

class CommandInvoker {
private:
    std::vector<std::unique_ptr<ICommand>> commands;
    SSD* ssd = nullptr;

public:
    CommandInvoker(SSD* ssd) : ssd(ssd) {}
    void addCommand(unique_ptr<ICommand> command) {
        commands.push_back(std::move(command));
    }

    int executeCommands(int argc, char* argv[]) {
        int ret = commandValidation(argc, argv);
        if (ret == ICommand::COMMAND_VALIDATION_FAIL)
            return ret;

        for (const auto& command : commands) {
            if (argv[1] == command->getCommandCode())
            {
                ret = command->execute(argc, argv, ssd);
                return ret;
            }
        }
        return 0;
    }
private:
    int commandValidation(int argc, char* argv[])
    {
        if (argc < 3 || argc > 5)
            return ICommand::COMMAND_VALIDATION_FAIL;
        return ICommand::COMMAND_VALIDATION_SUCCESS;
    }
};
