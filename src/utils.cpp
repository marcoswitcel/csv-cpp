#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "./trace.hpp"

/**
 * @brief 
 * @todo João, teminar testes
 * 
 * @param value 
 * @param delimiter 
 * @return std::vector<std::string> 
 */
std::vector<std::string> split_by(std::string value, char delimiter)
{
  std::vector<std::string> list;

  size_t index = 0;
  size_t start_index = 0;
  std::cout << value << std::endl;
  for (; index < value.length(); index++)
  {
    if (value.at(index) == delimiter || index + 1 == value.length())
    {
      list.push_back(value.substr(start_index, index - start_index));
      start_index = index + 1;
    }
  }

  return list;
}
