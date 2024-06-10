#include "ICommand.h"
#include "CommandParser.cpp"
#include "EraseCommand.cpp"

class EraseRangeCommand : public ICommand {
public:
	// ICommand을(를) 통해 상속됨
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser& cp, IProduct* product) override
	{
		checkInvalid_LBA(cp.lba);
		checkInvalid_LBA(cp.endLba);

		int tmp = cp.endLba - cp.lba;
		for (int i = 0; i < ((cp.endLba - cp.lba) / 10 + 1); i++) {
			if (tmp >= 10) {
				product->Erase(cp.lba + i * 10, 10);
				tmp -= 10;
			}
			else {
				product->Erase(cp.lba + i * 10, tmp);
			}
		}
	}

private:
	static constexpr char commandName[] = "ERASE_RANGE";
};