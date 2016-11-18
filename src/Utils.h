#pragma once

#include <string>
#include <vector>

bool FindAndReplaceOnce(std::string & source, std::string find, std::string replace);

bool FindAndReplaceAll(std::string & source, std::string find, std::string replace);

std::vector<std::string> Split(const std::string &text, char sep);