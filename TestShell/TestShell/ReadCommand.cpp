#include "ICommand.h"
#include "CommandParser.cpp"

class ReadCommand : public ICommand {
public:
	// ICommand을(를) 통해 상속됨
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser& cp, IProduct* product) override
	{
		checkInvalid_LBA(cp.lba);
		string result = product->Read(cp.lba);
		if (isRunningTestScenario == false)
			cout << result <<  endl;
		logger.write_Log(eLoggingOpt::ONLY_FILE, __FUNCTION__, "Read Data (LBA) : " + result + "(" + to_string(cp.lba) + ")");
	}

private:
	static constexpr char commandName[] = "READ";
};