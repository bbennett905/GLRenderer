#pragma once

#include <string>

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
	while (FindAndReplaceOnce(source, find, replace)) { }
	return true;
}