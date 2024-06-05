#include "ICommand.h"

class WriteCommand : public ICommand {
public:
	virtual string getCommandCode() const override {
		return COMMAND_CODE;
	}
	virtual int execute(int argc, char* argv[], SSD* ssd)
	{
		CommandSet cmd;
		int ret = checkParameter(argc, argv, cmd);
		if (ret == ICommand::COMMAND_VALIDATION_SUCCESS)
			ssd->write(cmd.address, cmd.data);
		return ret;
	}

private:
	static constexpr char COMMAND_CODE[] = "W";
};