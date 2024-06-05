#include <stdexcept>
#include <iostream>
#include <string>
#include "TestShell.cpp"
#include "CommandParser.cpp"

using namespace std;
int main() {
	string input_cmd;
	TestShell ts;
	ts.createProduct("SSD");
	CommandParser cp;
	while (1) {
		try {
			cout << "> ";
			getline(cin, input_cmd);
			if (input_cmd == "")
				continue;
			cp.command_parse(input_cmd);
			if (cp.cmd == "exit")
				break;
			else if (cp.cmd == "help")
				cout << "�ֿ� ��ɾ�\n"
				<< "1. write (��) write LBA data : LBA �� data�� ���)\n"
				<< "2. read (��) read LBA : LBA �� ����)\n"
				<< "3. exit (��) exit : shell �� ����)\n"
				<< "4. help (��) help : �ֿ� ��ɾ��� ����� ���)\n"
				<< "5. fullwrite (��) fullwrite data : ��� LBA(0~99)���� data �� write ����)\n"
				<< "6. fullread (��) fullread : ��� LBA(0~99) ���� read ����)\n";
			else if (cp.cmd == "read")
				ts.Read(cp.lba);
			else if (cp.cmd == "write")
				ts.Write(cp.lba, cp.data);
			else if (cp.cmd == "fullwrite")
				ts.FulllWrite(cp.data);
			else if (cp.cmd == "fullread")
				ts.FullRead();
			else if (cp.cmd == "testapp1")
				ts.testApp1();
			else if (cp.cmd == "testapp2")
				ts.testApp2();
		}
		catch (exception) {
			cout << "INVALID COMMAND\n";
		}
	}
}