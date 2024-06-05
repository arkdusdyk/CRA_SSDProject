#include <iostream>
#include "Invoker.cpp"
#include "Write.cpp"
#include "Read.cpp""

int main(int argc, char* argv[])
{
    SSD ssd;
    CommandInvoker invoker(&ssd);
   
    invoker.addCommand(new WriteCommand);
    invoker.addCommand(new ReadCommand);

    return invoker.executeCommands(argc, argv);
}