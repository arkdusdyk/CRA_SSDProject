#include <iostream>
#include "ssd.cpp"

int main(int argc, char* argv[])
{
    SSD ssd;
    CommandSet cmd;
    int ret = ssd.checkParameter(argc, argv, cmd);
    if (ret == SSD::COMMAND_VALIDATION_FAIL)
        return -1;

    if (cmd.cmdOpcode == SSD::COMMAND_WRITE)
    {
        ssd.write(cmd.address, cmd.data);
    }
    else if (cmd.cmdOpcode == SSD::COMMAND_READ)
    {
        ssd.read(cmd.address);
    }
    else
        return -1;

    return 0;
}