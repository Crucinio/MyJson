
#include "pch.h"
#include "CppUnitTest.h"
#include "../MyJson/Json.h"
#include "../MyJson/Json.cpp"

#include<fstream>
#include<string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const std::string test_dir = "../../UnitTestingResources/";

namespace JSONtests
{
	TEST_CLASS(Constructor_NoSeparators_OneLine)
	{
	public:
		
		// checks val after val parse
		// conatains 10 key - val pairs
		TEST_METHOD(Values)
		{	
			std::ifstream in(test_dir + "Constructor_NoSeparators_OneLine_Values.txt");
			Assert::IsTrue(in.good(), L"Your test copy does not exist, thus invalid");

			std::string line;
			std::getline(in, line);
			in.close();

			JSONObject test_object(line);
			// Check if all keys and values are here
			for (int i = 1; i < 11; i++)
			{
				std::string key = "TO1K" + std::to_string(i);
				Assert::IsTrue(test_object.is_in_values(key), (L"TO1K" + std::to_wstring(i)).c_str());
				std::string expected_val = "TO1V" + std::to_string(i);
				std::string actual_val = test_object.get_value(key);
				Assert::AreEqual(expected_val, actual_val);
			}
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
					const std::list<std::string>& actual_list = test_object.get_object(obj).get_values(key);
					int k = 1;
					for (auto kt = actual_list.begin(); kt != actual_list.end(); k++, kt++)
						Assert::AreEqual(val + std::to_string(k), *kt);
				}
			}
		}

		// checks value list after list parse
		TEST_METHOD(ListsOfValues)
		{
			std::ifstream in(test_dir + "Constructor_NoSeparators_OneLine_ListsOfValues.txt");
			Assert::IsTrue(in.good(), L"Your test copy does not exist, thus invalid");

			std::string line;
			std::getline(in, line);
			in.close();

			JSONObject test_object(line);
			// Check if all keys and values are here
			for (int i = 1; i < 11; i++)
			{
				std::string key = "TO1K" + std::to_string(i);
				Assert::IsTrue(test_object.is_in_arrays(key), (L"TO1K" + std::to_wstring(i)).c_str());
				auto& it = test_object.get_values(key).begin();
				for (int j = 1; j < 6; j++)
				{
					std::string expected_val = "TO1V" + std::to_string(i) + "_" + std::to_string(j);
					std::string actual_val = *it;
					Assert::AreEqual(expected_val, actual_val);
					it++;
				}
			}
		}

		// checks obj list after list parse
		TEST_METHOD(ListsOfObjects)
		{

		}
	};

	TEST_CLASS(Constructor_BackslashSymbols) {
		TEST_METHOD(Quotes) {

		}
	};

	TEST_CLASS(Pick_Methods) {
		TEST_METHOD(pick_value_valid) {
			std::string source = "\"value number one - basic\""
				"\"value number two - spec name = \\\"Hello to you patua\\\"\""
				"\"value number three - erase me\\b\\b\\b\\b\\b\\b\\b\\b\\b\""
				"\"value number four - I\\tam\\tpretty\\twide\\taren't\\tI\\t?\""
				"\"value number five - tricky-peaky \\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\"";

			std::vector<std::string> expected;
			expected.push_back("value number one - basic");
			expected.push_back("value number two - spec name = \"Hello to you patua\"");
			expected.push_back("value number three -");
			expected.push_back("value number four - I\tam\tpretty\twide\taren't\tI\t?");
			expected.push_back("value number five - tricky-peaky \"\"\"\"\"\"\"\"\"\"\"\"\"");


			std::vector<std::string> actual;
			int begin = 0;
			while (begin < source.size()) {
				std::string val = JSONObject::pick_val(source, begin);
				begin++;
				actual.push_back(val);
			}

			for (int i = 0; i < 5; i++)
				Assert::AreEqual(expected[i], actual[i]);
		}

		TEST_METHOD(pick_value_invalid) {
			std::string source = "\"value number one - basic";
			int begin = 0;
			auto exec1 = [&source, &begin] { JSONObject::pick_val(source, begin); };
			Assert::ExpectException<std::invalid_argument*>(exec1);

			source = "some data \"";
			begin = 0;
			auto exec2 = [&source, &begin] { JSONObject::pick_val(source, begin); };
			Assert::ExpectException<std::invalid_argument*>(exec2);
		}
		TEST_METHOD(pick_val_array_valid) {
			std::string source = "\"     [[[[\"value number one - basic\""
				"\"value number two - spec name = \\\"Hello to you patua\\\"\""
				"\"value number three [-] erase me\\b\\b\\b\\b\\b\\b\\b\\b\\b\""
				"\"]value number four - I[][][]\\tam\\tpretty\\twide\\taren't\\tI\\t?\""
				"\"value number five - tricky-peaky]]]]]] \\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"]\"     ] ]";

			std::list<std::string> expected;
			expected.push_back("value number one - basic");
			expected.push_back("value number two - spec name = \"Hello to you patua\"");
			expected.push_back("value number three [-]");
			expected.push_back("]value number four - I[][][]\tam\tpretty\twide\taren't\tI\t?");
			expected.push_back("value number five - tricky-peaky]]]]]] \"\"\"\"\"\"\"\"\"\"\"\"\"]");
			int begin = 0;
			std::list<std::string> actual = JSONObject::pick_val_array(source, begin);

			Assert::AreEqual(expected.size(), actual.size());
			for (auto it = actual.begin(), jt = expected.begin(); it != actual.end() && jt != expected.end(); it++, jt++) {
				Assert::AreEqual(*jt, *it);
			}
		}

		TEST_METHOD(pick_val_array_invalid) {
			std::string source1 = "\" hello\" \" val\"";
			int begin = 0;
			auto exec1 = [&source1, &begin] { JSONObject::pick_val_array(source1, begin); };
			Assert::ExpectException<std::invalid_argument*>(exec1);

			std::string source2 = " \" hello\" \" val\" ]";
			begin = 0;
			auto exec2 = [&source2, &begin] { JSONObject::pick_val_array(source2, begin); };
			Assert::ExpectException<std::invalid_argument*>(exec2);

			std::string source3 = "[\" hello\" \" val\"";
			begin = 0;
			auto exec3 = [&source3, &begin] { JSONObject::pick_val_array(source3, begin); };
			Assert::ExpectException<std::invalid_argument*>(exec3);


		}
	};
}
