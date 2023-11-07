#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{
	std::ifstream in("UnitTestingResources/Constructor_NoSeparators_OneLine_Objects.txt");

	std::string line;
	std::getline(in, line);
	in.close();

	JSONObject test_object(line);
	for (int i = 1; i < 6; i++)
	{
		std::string obj = "TO" + std::to_string(i);

		// Check if all nested objects are here

		for (int j = 1; j < 6; j++)
		{
			// Check if key exists
			std::string key = obj + "K" + std::to_string(j);

			// Compare expected with actual
			std::string expected = obj + "V" + std::to_string(j);
			std::string actual = test_object.get_object(obj).get_value(key);
		}

		for (int j = 6; j < 11; j++)
		{
			// Check if key exists
			std::string key = obj + "K" + std::to_string(j);

			// Compare expected lists with actual
			std::string val = obj + "V" + std::to_string(j) + "_";
			const std::list<std::string>& actual_list = test_object.get_object(obj).get_values(key);
			int k = 1;
		}
	}

	// Check all the key-val fields withing the objects
}