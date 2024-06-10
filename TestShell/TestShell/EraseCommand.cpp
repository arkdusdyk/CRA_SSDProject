#pragma once
#include "ICommand.h"
#include "CommandParser.cpp"

class EraseCommand : public ICommand {
public:
	// ICommand을(를) 통해 상속됨
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser& cp, IProduct* product) override
	{
		checkInvalid_LBA(cp.lba);

		int tmp = cp.size;
		for (int i = 0; i < (cp.size / 10 + 1); i++) {
			if (tmp > 10) {
				product->Erase(cp.lba + i * 10, 10);
				tmp -= 10;
			}
			else {
				product->Erase(cp.lba + i * 10, tmp);
			}
		}
	}

private:
	static constexpr char commandName[] = "ERASE";
};