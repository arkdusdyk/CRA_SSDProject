#include "ICommand.h"
#include "CommandParser.cpp"

class FullReadCommand : public ICommand {
public:
	// ICommand을(를) 통해 상속됨
	string getCommand() override
	{
		return commandName;
	}

	void execute(CommandParser cp, IProduct* product) override
	{
		vector<string> results;
		for (int lba = 0; lba < MAX_LBA; lba++) {
			results.push_back(product->Read(lba));
			cout << results[lba] << endl;
		}
		//return results;
	}

private:
	static constexpr char commandName[] = "FULLREAD";
};