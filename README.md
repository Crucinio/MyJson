# C++ MyJson
## My realisation of JSON for reading JSON files.

# About
	This project is developped to make a versetile tool for reading JSON files and not thinking about the strict way of making a file that could be read by the reader.
	It can easily avoid process files that could not be processed by some other parsers due to the minor issues in following syntax like commas, unnecessary symbols between values and keys.
	To make it work I implemented parser, that would take into consideration only control brackets and quotes, making everything out of control zones irrelevant for reader.
	To see the rules of making a file that could be read by the lib, see documentation.

# Install
	Just install Json.h and Json.cpp and include it in files you need. After that read documentation or just click around and guess what happens

# Currently working on
	Writing in bin and txt files (write, rewrite)
	Reading from bin and txt files (read, reread))
	Inserting fields by key (insert val, arr, obj, obj_arr)
	Add object (operator +)
