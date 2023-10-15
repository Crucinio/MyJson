
#include "pch.h"
#include "CppUnitTest.h"
#include "../MyJson/Json.h"
#include "../MyJson/Json.cpp"

#include<fstream>
#include<string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JSONtests
{
	TEST_CLASS(Constructor_NoSeparators_OneLine)
	{
		std::string test_dir = "../../UnitTestingResources/";
	public:
		
		// Checks val after val parse
		TEST_METHOD(Values)
		{	
			
		}

		// checks obj after obj parse;
		// contains 5 test objects (TO) within main object, all TO contain 5 key-val pairs and 5 key-list pairs
		TEST_METHOD(Objects)
		{
			std::ifstream in(test_dir + "Constructor_NoSeparators_OneLine_Objects.txt");
			Assert::IsTrue(in.good(), L"Your test copy does not exist, thus invalid");

			std::string line;
			std::getline(in, line);
			in.close();

			JSONObject test_object(line);
			for (int i = 1; i < 6; i++)
			{
				std::string obj = "TO" + std::to_string(i);

				// Check if all nested objects are here
				Assert::IsTrue(test_object.is_in_objects("TO" + std::to_string(i)), (L"TO" + std::to_wstring(i)).c_str());

				for (int j = 1; j < 6; j++)
				{
					// Check if key exists
					std::string key = obj + "K" + std::to_string(j);
					Assert::IsTrue(test_object.get_object(obj).is_in_values(key), (std::wstring(key.begin(), key.end()) + L" is not found").c_str());

					// Compare expected with actual
					std::string expected = obj + "V" + std::to_string(j);
					std::string actual = test_object.get_object(obj).get_value(key);
					Assert::AreEqual(actual, expected);
				}

				for (int j = 6; j < 11; j++)
				{
					// Check if key exists
					std::string key = obj + "K" + std::to_string(j);
					Assert::IsTrue(test_object.get_object(obj).is_in_arrays(key), (std::wstring(key.begin(), key.end()) + L" is not found").c_str());

					// Compare expected lists with actual
					std::string val = obj + "V" + std::to_string(j) + "_";
					const std::list<std::string>& expected_list = test_object.get_object(obj).get_values(key);
					int k = 1;
					for (auto kt = expected_list.begin(); kt != expected_list.end(); k++, kt++)
						Assert::AreEqual(val + std::to_string(k), *kt);
				}
			}

			// Check all the key-val fields withing the objects
		}

		// checks value list after list parse
		TEST_METHOD(ListsOfValues)
		{

		}

		// checks obj list after list parse
		TEST_METHOD(ListsOfObjects)
		{

		}
	};
}
