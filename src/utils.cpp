#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

std::vector<std::string> split_by(std::string value, char delimiter)
{
  std::vector<std::string> list;

  std::istringstream iss(value);
  std::string word;

  while (std::getline(iss, word, delimiter))
  {
    list.push_back(word);
  }

  return list;
}
