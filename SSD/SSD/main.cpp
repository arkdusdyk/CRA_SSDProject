#include <iostream>
#include <memory>
#include <stdexcept>
#include "Invoker.cpp"
#include "Write.cpp"
#include "Read.cpp"

#define DEBUG (0)

int main(int argc, char* argv[])
{
    SSD ssd;
    CommandInvoker invoker(&ssd);

    invoker.addCommand(std::move(std::make_unique<WriteCommand>()));
    invoker.addCommand(std::move(std::make_unique<ReadCommand>()));
 
    if (argc < 2) {
        invoker.printHelp();
        return 0;
    }

    try {
        return invoker.executeCommands(argc, argv);
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        return -EPERM;
    }
}