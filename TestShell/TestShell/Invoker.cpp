#include <vector>

#include "ProductInterface.h"
#include "ICommand.h"
#include "CommandParser.cpp"

class CommandInvoker {
public:
	void setProduct(IProduct* iproduct) {
		product = iproduct;
	}

	void createProduct(string device) {
		createProductFactory cPF(device);
		setProduct(cPF.getProduct());
	}

	void addCommand(unique_ptr<ICommand> command) {
		commands.push_back(std::move(command));
	}

	void execute(CommandParser& cp) {
		for (const auto& command : commands) {
			if(command->getCommand() == cp.cmd)
				command->execute(cp, product);
		}
	}

	void setRun(bool runflag) {
		for (const auto& command : commands) {
			command->setRunMode(runflag);
		}
	}

private:
	IProduct* product;
	std::vector<std::unique_ptr<ICommand>> commands;
};