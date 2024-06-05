#include <vector>
#include "ICommand.h"

class CommandInvoker {
private:
    std::vector<ICommand*> commands;

public:
    CommandInvoker(SSD* ssd) : ssd(ssd) {}
    void addCommand(ICommand* command) {
        commands.push_back(command);
    }

    int executeCommands(int argc, char* argv[]) {
        for (ICommand* command : commands) {
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