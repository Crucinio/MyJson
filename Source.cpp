#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{
	std::ifstream in("UnitTestingResources/Constructor_NoSeparators_OneLine_Values.txt");

	std::string line;
	std::getline(in, line);
	in.close();

	JSONObject test_object(line);
}