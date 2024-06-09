#include "ICommand.h"
#include "CommandParser.cpp"

class ReadCommand : public ICommand {
public:
	// ICommand��(��) ���� ��ӵ�
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser cp, IProduct* product) override
	{
		checkInvalid_LBA(cp.lba);
		string result = product->Read(cp.lba);
		cout << result <<  endl;
	}

private:
	static constexpr char commandName[] = "READ";
};