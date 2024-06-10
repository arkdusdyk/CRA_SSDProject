#pragma once
#include <iostream>
#include <vector>

using namespace std;
class TestCase
{
public:
	TestCase(string fileName);
	~TestCase();
	bool execute(CommandParser &cp, CommandInvoker &invoker);
private:
	string mId;
	vector<string> mSteps;
	vector<string> mExpectedResults;
	string mFileName;
};

