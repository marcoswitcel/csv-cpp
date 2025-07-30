#pragma once

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>

// @note João, considerar usar cstrings e array estáticos para o header e as linhas
using CSV_Data_Row = std::vector<std::string>;

struct CSVData
{
  CSV_Data_Row header;
  std::vector<CSV_Data_Row> dataset;
};

void print_as_table(CSVData &csv, std::vector<std::string> &filters)
{

  std::vector<size_t> filters_index;

  // header
  for (const auto &dataField : csv.header)
  {
    std::cout << dataField << "||";
  }
  std::cout << std::endl;

  for (const auto &filter: filters)
  {
    auto it = std::find(filters.begin(), filters.end(), filter);
    if (it != filters.end())
    {
      filters_index.push_back(std::distance(filters.begin(), it));
    }
  }

  // data
  for (const auto &row : csv.dataset)
  {
    for (size_t i = 0; i < row.size(); i++)
    {
      // @todo João, terminar filtro aqui...
      // if (std::find(filters_index.begin(), filters_index.end(), i) != filters_index.end()) continue;

      const auto &dataField = row[i];
      std::cout << dataField << "||";
    }
    std::cout << std::endl;
  }
}

void add_data_row(CSV_Data_Row &row, std::string line)
{
  std::stringstream stream(line);
  std::string field;
  // @todo João, terminar isso aqui, definir os tipos de dados...
  while (std::getline(stream, field, ','))
  {
    row.push_back(field);
  }
}

// @todo João, terminar isso aqui
std::pair<bool, CSVData> parse_csv_from_file(const char* filename)
{
  CSVData csv;

  std::ifstream file_handle(filename, std::ios::in);

  if (!file_handle.good())
  {
    return std::make_pair(false, csv);
  }

  std::string line;
  while (std::getline(file_handle, line))
  {
    if (csv.header.size() == 0)
    {
      add_data_row(csv.header, line);  
    }
    else
    {
      CSV_Data_Row row;
      add_data_row(row, line);

      assert(row.size() == csv.header.size());
      csv.dataset.push_back(row);
    }
  }

  file_handle.close();

  return std::make_pair(true, csv);
}

