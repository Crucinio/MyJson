#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{
	std::string source = "\"value number one - basic";
	int begin = 0;
	auto exec1 = [&source, &begin] { JSONObject::pick_val(source, begin); };
	//Assert::ExpectException<std::exception>(exec1);
	//exec1();

	source = "some data \"";
	begin = 0;
	auto exec2 = [&source, &begin] { JSONObject::pick_val(source, begin); };
	//Assert::ExpectException<std::exception>(exec2);
}