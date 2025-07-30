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
  std::vector<size_t> index_to_show;
  const auto &header = csv.header;

  for (size_t i = 0; i < header.size(); i++)
  {
    const auto &dataField = header[i];
    auto it = std::find(filters.begin(), filters.end(), dataField);
    
    if (it != filters.end()) continue;

    index_to_show.push_back(i);
  }

  // header
  for (const auto i : index_to_show)
  {
    const auto &dataField = header[i];
    std::cout << dataField << "||";
  }
  std::cout << std::endl;

  // data
  for (const auto &row : csv.dataset)
  {
    for (const auto i : index_to_show)
    {
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

