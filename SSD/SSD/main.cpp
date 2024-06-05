#include <iostream>
#include <memory>
#include "Invoker.cpp"
#include "Write.cpp"
#include "Read.cpp"

int main(int argc, char* argv[])
{
    SSD ssd;
    CommandInvoker invoker(&ssd);

    invoker.addCommand(std::move(std::make_unique<WriteCommand>()));
    invoker.addCommand(std::move(std::make_unique<ReadCommand>()));

    return invoker.executeCommands(argc, argv);
}