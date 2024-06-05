#include <iostream>
#include <string>
#include "TestShell.cpp"

using namespace std;
int main() {
	string input_cmd;
	while (1) {
		cin >> input_cmd;
		if (input_cmd == "exit")
			break;
		else if (input_cmd == "help")
			cout << "도움말 추가 예정";

	}
}