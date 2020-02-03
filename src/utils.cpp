#include "utils.h"
#include <ostream>
#include <sstream>
#include <fstream>
#include <iostream>

std::string Utils::TextFileToString(const std::string& file_path)
{
	return TextFileToStream(file_path).str();
}

std::stringstream Utils::TextFileToStream(const std::string& file_path)
{
	std::stringstream ss;
	std::ifstream fs;
	fs.open(file_path, std::ios::in);

	if (!fs.fail())
	{
		ss << fs.rdbuf();
	}
	else
	{
		std::cerr << "Error reading text file: " << file_path << std::endl;
	}

	fs.close();
	
	return ss;
}