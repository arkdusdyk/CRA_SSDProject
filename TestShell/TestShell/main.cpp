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
				cout << "주요 명령어\n"
				<< "1. write (예) write LBA data : LBA 에 data를 기록)\n"
				<< "2. read (예) read LBA : LBA 를 읽음)\n"
				<< "3. exit (예) exit : shell 을 종료)\n"
				<< "4. help (예) help : 주요 명령어의 사용방법 출력)\n"
				<< "5. fullwrite (예) fullwrite data : 모든 LBA(0~99)까지 data 를 write 수행)\n"
				<< "6. fullread (예) fullread : 모든 LBA(0~99) 까지 read 수행)\n";
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