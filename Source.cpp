#include<fstream>
#include<string>
#include<cassert>
#include<iostream>
#include "Json.h"


int main()
{

	std::ifstream in("UnitTestingResources/all_effects_data.txt");

	std::string line;
	std::string test = "";
	while (std::getline(in, line)) {
		test += line;
	}
	in.close();
	JSONObject obj2 = JSONObject::read("UnitTestingResources/list_obj.txt");
	obj2.write("UnitTestingResources/list_obj2.txt");

}


