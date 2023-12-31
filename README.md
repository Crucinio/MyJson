# C++ MyJson
## My realisation of JSON files reader and manipulator.

# About
	This project is developped to make a versetile tool for reading JSON files and strings that also provides ability to add new key-val pairs via reading from other sources or just adding them through setters.
	It can easily process files that could not be processed by some other parsers due to insensitivity to some minor issues in syntax like commas, unnecessary symbols between values and keys.
	To make it work, a parser that would take into consideration only control brackets, quotes and digits, was implemented.

	Currently JSONObject can be initialised by string source or path to the file, containing data. It can process nums, bools, strings, objects and corresponding arrays, as well as nested objects;
	All values can be accessed by getters through key;
	All containers (unordered maps) can be accessed as const& via container getters;
	New fields can be added by setters (supporting either string, containing both key and val, or 2 arg set with key and val predefined);
	All keys can be checked by is_in_* methods;
	All key-val pairs can be erased via erase_* methods;
	All containers can be cleared;
	All containers have corresponding empty_* methods;
	Pick methods were mostly used to simplify constructor looks, but they can be used to pick corresponding struct from defined position in given string;
	Note that parser does not read neither NULL nor null, also it parses booleans as true or false case-sensitively;

# Install
	Just install Json.h Json.cpp and include it in files you need.

# Currently working on
	Writing in bin and txt files (write, rewrite);
	Reading from bin files (read, reread));
	to_string method;
	documentation (since 31.12.2023);
