#include "ICommand.h"
#include "CommandParser.cpp"

class FullWriteCommand : public ICommand {
public:
	// ICommand��(��) ���� ��ӵ�
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser& cp, IProduct* product) override
	{
		for (int lba = 0; lba < 100; lba++) {
			product->Write(lba, cp.data);
		}
	}

private:
	static constexpr char commandName[] = "FULLWRITE";
};