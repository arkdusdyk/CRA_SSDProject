#include <vector>
#include <memory>
#include "ICommand.h"

class CommandInvoker {
private:
    //std::vector<ICommand*> commands;
    std::vector<std::unique_ptr<ICommand>> commands;

public:
    CommandInvoker(SSD* ssd) : ssd(ssd) {}
    void addCommand(unique_ptr<ICommand> command) {
        commands.push_back(std::move(command));
    }

    int executeCommands(int argc, char* argv[]) {
        for (const auto& command : commands) {
            if (argv[1] == command->getCommandCode())
            {
                int ret = command->execute(argc, argv, ssd);
                commands.clear();
                return ret;
            }
        }
        commands.clear();
        return 0;
    }
private:
    SSD* ssd = nullptr;
};