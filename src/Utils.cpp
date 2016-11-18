#include "Utils.h"

bool FindAndReplaceOnce(std::string & source, std::string find, std::string replace)
{
	int location = source.find(find);
	if (location == -1)
		return false;

	source.replace(location, find.length(), replace.c_str());
	return true;
}

bool FindAndReplaceAll(std::string & source, std::string find, std::string replace)
{
	if (source.find(find) == -1)
		return false;
	while (FindAndReplaceOnce(source, find, replace)) {}
	return true;
}

std::vector<std::string> Split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}