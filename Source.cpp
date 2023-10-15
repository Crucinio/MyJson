#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{
	std::ifstream in;
	in.open("UnitTestingResources/AllDataInOneLine_NestedObjects.txt");
	//Assert::IsTrue(!in.good(), L"Your test copy is empty, thus invalid");

	std::string line;
	std::getline(in, line);

	JSONObject test_object(line);
	// Check if all nested objects are here
	for (int i = 1; i < 10; i++)
	{
		std::string str_msg = "T" + std::to_string(i);
		std::wstring wstr_msg = std::wstring(str_msg.begin(), str_msg.end());
		const wchar_t* msg = wstr_msg.c_str();
		//Assert::IsTrue(!test_object.is_in_objects("T1" + std::to_string(i)), msg);
		if (!test_object.is_in_objects("T" + std::to_string(i)))
			std::cout << "T" << std::to_string(i) << " was not found" << std::endl;
	}
}