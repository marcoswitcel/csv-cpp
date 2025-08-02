#pragma once

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <iomanip>

// @note João, considerar usar cstrings e array estáticos para o header e as linhas
using CSV_Data_Row = std::vector<std::string>;

enum Data_Cell_Type
{
  TEXT,
  // @note João, adicionar distinção entre inteiros, floats e pelo tamanho dos números?
  NUMBER,
};

struct Data_Cell_Type_Info
{
  Data_Cell_Type type;
  bool nullable;
};

struct CSVData
{
  // @todo João, modelar uma coluna dedicada a metadados sobre o parsing e os tipos inferidos
  // das colunas. Armazenar se houve algum erro em alguma linha e talvez traquear as linhas problemáticas
  // para remoção ou tratamento
  CSV_Data_Row header;
  std::vector<Data_Cell_Type_Info> infered_types_for_columns;
  std::vector<CSV_Data_Row> dataset;

  void infer_types()
  {
    if (!this->header.size()) return;
    if (!this->dataset.size()) return;

    for (size_t i = 0; i < this->header.size(); i++)
    {
      Data_Cell_Type_Info col_info = { .type = NUMBER, .nullable = false, };

      for (const auto &row : this->dataset)
      {
        const auto &dataField = row[i];
        // @todo João, terminar aqui...


      }
    }
  }
};

constexpr int32_t END_OF_SOURCE = -9999;

struct CSV_Parse_Context
{
  std::string source;
  uint64_t index = 0;

  CSV_Parse_Context(std::string source)
  {
    this->source = source;
    this->index = 0;
  }

  int32_t peek_char()
  {
    if (this->is_finished()) return END_OF_SOURCE;

    return static_cast<int32_t>(this->source[this->index++]);
  }

  int32_t eat_char()
  {
    if (this->is_finished()) return END_OF_SOURCE;

    return static_cast<int32_t>(this->source[this->index++]);
  }

  inline bool is_finished()
  {
    return this->index >= this->source.length();
  }
};

/**
 * @brief função que printa os dados em formato de tabela
 * 
 * @todo João, otimizar e ou escapar caracteres especiais para não quebrar o layout, bem como
 * calcular tamanho de colunas de forma dinâmica e escrever para um arquivo ao invés do 'std::cout'
 * 
 * @param csv 
 * @param filters colunas que não devem ser exibidas
 */
void print_as_table(CSVData &csv, std::vector<std::string> &filters)
{
  std::vector<size_t> index_to_show;
  const auto &header = csv.header;
  size_t field_width = 0;

  for (size_t i = 0; i < header.size(); i++)
  {
    const auto &dataField = header[i];
    auto it = std::find(filters.begin(), filters.end(), dataField);

    if (it != filters.end()) continue;

    field_width = field_width > dataField.size() ? field_width : dataField.size();

    index_to_show.push_back(i);
  }

  for (const auto &row : csv.dataset)
  {
    for (const auto i : index_to_show)
    {
      const auto &dataField = row[i];
      field_width = field_width > dataField.size() ? field_width : dataField.size();
    }
  }

  // @todo João, eventualmente printar com reticências
  field_width += 4; // padding arbitrário

  if (index_to_show.size() == 0) return;

  // header
  std::cout << std::setfill('-') << std::setw(field_width * index_to_show.size() + 1 + index_to_show.size()) << "" << std::endl;
  std::cout << "|";
  for (const auto i : index_to_show)
  {
    const auto &dataField = header[i];
    std::cout << std::left << std::setfill(' ') << std::setw(field_width) << dataField << "|";
  }
  std::cout << std::endl;
  std::cout << std::setfill('-') << std::setw(field_width * index_to_show.size() + 1 + index_to_show.size()) << "" << std::endl;

  // data
  for (const auto &row : csv.dataset)
  {
    std::cout << "|";
    for (const auto i : index_to_show)
    {
      const auto &dataField = row[i];
      std::cout << std::left << std::setfill(' ') << std::setw(field_width) << dataField << "|";
    }
    std::cout << std::endl;
    std::cout << std::setfill('-') << std::setw(field_width * index_to_show.size() + 1 + index_to_show.size()) << "" << std::endl;
  }

}

std::vector<std::string> parsing_data_cells(std::string source)
{
  char delimiter = ',';
  std::vector<std::string> data;
  CSV_Parse_Context parser(source);
  int64_t start_current_data_cell = parser.index;
  bool quoted = false;
  bool openQuote = false;
  
  while (!parser.is_finished())
  {
    int32_t character = parser.eat_char();

    // @todo João, validar a função substr, validar o porque de os caracteres serem negativos...
    // @todo João, também ignorar aspas seguidas de aspas, checar na RFC do CSV
    // @wip vários bugs... terminar
    if (character == '"')
    {
      character = parser.eat_char();
      if (quoted)
      {
        openQuote = false;
      }
      else
      {
        quoted = true;
        openQuote = true;
      }
    }

    if (!openQuote && (character == delimiter || parser.is_finished()))
    {
      size_t end_index = parser.index; // remove um índice para compensar o caractere de marcação de separação
      size_t start_index = start_current_data_cell;
      if (!parser.is_finished())
      {
        end_index--;
      }
      if (quoted)
      {
        // @todo João, dessa forma quebra com facilidade, revisar no futuro...
        start_index++;
        end_index--;
      }
      std::string data_cell = parser.source.substr(start_index, end_index - start_index);
      data.push_back(data_cell);
      start_current_data_cell = parser.index;
      // reset quote
      quoted = false;
      openQuote = false;
    }
  }

  return data;
}

void add_data_row(CSV_Data_Row &row, std::string line)
{
  // @todo João, aqui na verdade será necessário ajustar para parsear as linhas
  // tanto pela questão das colunas com àspas como pelos possíveis enter's dentro dessas colunas
  for (const auto &field: parsing_data_cells(line))
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

      // @todo João, ver como vai evoluir esses warns
      if (row.size() != csv.header.size())
      {
        std::cout << "WARN: data_row com número de colunas incompatível com cabeçalho. colunas: "
                  << row.size() << ", linha índice: " << csv.dataset.size() << std::endl;
      }
      csv.dataset.push_back(row);
    }
  }

  file_handle.close();

  return std::make_pair(true, csv);
}

