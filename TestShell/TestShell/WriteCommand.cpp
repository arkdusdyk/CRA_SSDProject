#include "ICommand.h"

class WriteCommand : public ICommand {
public:
	// ICommand을(를) 통해 상속됨
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser cp, IProduct* product) override
	{
		checkInvalid_LBA(cp.lba);
		checkInvalid_Value(cp.data);
		product->Write(cp.lba, cp.data);
	}

private:
	static constexpr char commandName[] = "WRITE";
};