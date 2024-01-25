#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{
	std::string source = "{\"value number one\" - \"basic\""
		"\"value number two\" -  \"spec name = \\\"Hello to you patua\\\"\""
		"\"value number three\" - \"erase me\\b\\b\\b\\b\\b\\b\\b\\b\\b\""
		"\"value number four\" - \"I\\tam\\tpretty\\twide\\taren't\\tI\\t?\""
		"\"value number five\" - \"tricky-peaky \\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\\\"\"}";

	JSONObject t(source);
	JSONObject obj2 = JSONObject::read("UnitTestingResources/to_string.txt");
	obj2.write("UnitTestingResources/list_obj3.txt");

}


