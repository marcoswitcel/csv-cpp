#pragma once

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <utility>
#include <iomanip>

#include "./csv-parse-context.hpp"
#include "./utils.cpp"
#include "./csv.hpp"

std::string to_string(Data_Cell_Type &type) {
  return type == TEXT ? "TEXT" : "NUMBER";
}

void CSVData::infer_types()
{
  if (!this->header.size()) return;
  if (!this->dataset.size()) return;

  for (size_t i = 0; i < this->header.size(); i++)
  {
    Data_Cell_Type_Info col_info = { .type = NUMBER, .nullable = false, };
    bool all_columns_null = true;

    for (const auto &row : this->dataset)
    {
      const auto &dataField = row[i];
      
      if (dataField.size() == 0)
      {
        col_info.nullable = true;
        continue;
      }

      all_columns_null = false;
      
      try 
      {
        // @note João, fica pendente implementar diferentes tipos de números.
        // @note João, como diferenciar string vazia de null em csv? parecem ser a mesma coisa na síntaxe, 
        // não sei se vai fazer sentido TEXT (nullable), enfim, analisar...
        std::stod(dataField);
      } catch (std::invalid_argument& ex)
      {
        col_info.type = TEXT;
      } catch (std::out_of_range& ex)
      {
        col_info.type = TEXT;
      }
    }

    /**
     * @note Se todas as colunas são nullas é melhor chutar que o campo
     * é textual, isto já que TEXT é mais abrangente.
     */
    if (all_columns_null)
    {
      col_info.type = TEXT;
    }

    this->infered_types_for_columns.push_back(col_info);
  }
}

/**
 * @brief 
 * 
 * @param text 
 * @param size_limite limite positivo mínimo é de 3, por causa da ellípse, se for negativo não altera a string
 * @return std::string 
 */
static std::string limit_text(const std::string &text, int32_t size_limite)
{
  if (size_limite < 0) return text;

  size_t limit = static_cast<size_t>(size_limite);

  if (text.size() <= limit)
  {
    return text;
  }

  std::string new_string = text.substr(0, limit - 3);
  new_string.append("...");
  return new_string;
}

void print_as_table(CSVData &csv, std::vector<std::string> &filters, int field_size_limit)
{
  std::vector<size_t> index_to_show;
  const auto &header = csv.header;
  std::vector<size_t> field_widths;

  for (size_t i = 0; i < header.size(); i++)
  {
    const auto dataField = limit_text(header[i], field_size_limit);
    auto it = std::find(filters.begin(), filters.end(), dataField);
    field_widths.push_back(0);

    if (it != filters.end()) continue;

    auto field_width = field_widths.at(i);
    if (dataField.size() > field_width)
    {
      field_widths.at(i) = dataField.size();
    }

    index_to_show.push_back(i);
  }

  for (const auto &row : csv.dataset)
  {
    for (const auto i : index_to_show)
    {
      const auto dataField = limit_text(row[i], field_size_limit);

      auto field_width = field_widths.at(i);
      if (dataField.size() > field_width)
      {
        field_widths.at(i) = dataField.size();
      }
    }
  }

  // limitando tamanho
  if (field_size_limit > -1)
  {
    for (const auto i : index_to_show)
    {
      auto field_width = field_widths.at(i);
      if (field_width > static_cast<size_t>(field_size_limit))
      {
        field_widths.at(i) = static_cast<size_t>(field_size_limit);
      } 
    }
  }

  if (index_to_show.size() == 0) return;

  // header
  size_t sum_of_all_widths = 0;
  size_t field_padding = 1;

  for (auto field_width : field_widths)
  {
    sum_of_all_widths += field_width + field_padding;
  }

  /**
   * @note João, foi feita uma análise aqui, sempre que houver acentos poderá não alinhar as colunas.
   * Isto porque o std::string armazena texto utf-8 em um formato multibyte não fixo, e não dá pra saber
   * quandos bytes formam um caractere visual 'Unicode Unit' ou 'Graphene Cluster' (não tenho certeza se são sinônimos).
   * A função std::stew trabalha com bytes e tamanho binário, então os bytes não visuais estão sendo contados. Poderia
   * ser usado a função std::wcout, mas para isso teria que converter para um std::wstring antes. Também seria possível
   * converter apenas para contar os caracteres e adicionar mais "bytes" para um determinadada linha agora sabendo que
   * deles não ocuparação um espaço dedicado.
   */
  std::cout << std::setfill('-') << std::setw(sum_of_all_widths + 1 + index_to_show.size()) << "" << std::endl;
  std::cout << "|";
  for (const auto i : index_to_show)
  {
    const auto dataField = limit_text(header[i], field_size_limit);
    const auto field_width = field_widths.at(i) + field_padding;
    std::cout << std::left << std::setfill(' ') << std::setw(field_width) << dataField << "|";
  }
  std::cout << std::endl;
  std::cout << std::setfill('-') << std::setw(sum_of_all_widths + 1 + index_to_show.size()) << "" << std::endl;

  // data
  for (const auto &row : csv.dataset)
  {
    std::cout << "|";
    for (const auto i : index_to_show)
    {
      const auto dataField = limit_text(row[i], field_size_limit);
      const auto field_width = field_widths.at(i) + field_padding;
      std::cout << std::left << std::setfill(' ') << std::setw(field_width) << dataField << "|";
    }
    std::cout << std::endl;
    std::cout << std::setfill('-') << std::setw(sum_of_all_widths + 1 + index_to_show.size()) << "" << std::endl;
  }

}

void print_infered_types(CSVData &csv)
{
  if (csv.header.size() != csv.infered_types_for_columns.size()) return;

  for (size_t i = 0; i < csv.header.size(); i++)
  {
    std::cout << "Column name: '" << csv.header[i] << "'";
    std::cout << ", Type: " << to_string(csv.infered_types_for_columns[i].type);
    std::cout << ", Nullable: " << ((csv.infered_types_for_columns[i].nullable) ? "yes" : "no" )<< std::endl;
  }
}

static std::vector<std::string> parsing_data_cells(std::string source, char delimiter)
{
  static std::regex double_quote_pattern("\"\"");

  std::vector<std::string> data;
  
  CSV_Parse_Context parser(source);
  uint64_t start_current_data_cell = parser.index;
  bool quoted = false;
  bool openQuote = false;
  
  while (!parser.is_finished())
  {
    int32_t character = parser.eat_char();

    if (character == '"')
    {
      if (quoted)
      {
        if (parser.peek_char() == '"')
        {
          character = parser.eat_char();
        }
        else
        {
          openQuote = false;
        }
      }
      else if ((parser.index - 1) == start_current_data_cell)
      {
        quoted = true;
        openQuote = true;
      }
    }

    if (!openQuote && (character == delimiter || parser.is_finished()))
    {
      size_t end_index = parser.index;
      size_t start_index = start_current_data_cell;
      
      if (!parser.is_finished() || character == delimiter)
      {
        end_index--;
      }

      if (quoted)
      {
        // @todo João, dessa forma quebra com facilidade, revisar no futuro...
        start_index++;
        // evitando warp-around e tentando copiar fora dos limites da string
        if (end_index > 0) end_index--;
      }

      // @note João, quando refatora para lidar com caracteres terminadores de 'linha'
      // dentro das células acredito que posso atacar o 'problema' que esse 'if' abaixo 
      // tenta resolver...
      if (end_index > 0 && parser.source[end_index - 1] == '\r')
      {
        end_index--;
      }
      
      std::string data_cell = (end_index > start_index) ?
        parser.source.substr(start_index, end_index - start_index)
        : "";
      // @note João, lento pra compilar, pode ser lento pra executar...
      if (quoted)
      {
        data_cell = std::regex_replace(data_cell, double_quote_pattern, "\"");
      }
      data.push_back(data_cell);
      start_current_data_cell = parser.index;
      // reset quote
      quoted = false;
      openQuote = false;

      if (character == delimiter && parser.is_finished())
      {
        data.push_back(""); 
      }
    }
  }

  return data;
}

static void add_data_row(CSV_Data_Row &row, std::string line, char delimiter)
{
  for (const auto &field: parsing_data_cells(line, delimiter))
  {
    row.push_back(field);
  }
}

std::pair<bool, CSVData> parse_csv_from_file(const char* filename)
{
  CSVData csv;

  std::ifstream file_handle(filename, std::ios::in);

  if (!file_handle.good())
  {
    return std::make_pair(false, csv);
  }

  std::string line;
  /**
   * @note João, por hora delego para essa função o ato de quebrar o stream
   * em linhas do arquivo, e linhas do csv de certa forma, porém eventualmente
   * vou querer processar enter's dentro das colunas ou lidar diferente com quebras
   * de linha no geral.
   * @todo João, pendente lidar com enter's dentro de colunas...
   */
  
  for (size_t line_number = 1; std::getline(file_handle, line); line_number++)
  {
    if (csv.header.size() == 0)
    {
      add_data_row(csv.header, line, csv.delimiter);  
    }
    else
    {
      CSV_Data_Row row;
      add_data_row(row, line, csv.delimiter);

      // @todo João, ver como vai evoluir esses warns
      if (row.size() != csv.header.size())
      {
        std::cout << "WARN: data_row com número de colunas incompatível com cabeçalho. colunas: "
                  << row.size() << ", linha índice: " << csv.dataset.size() << std::endl;
        csv.parsing_errors.push_back("Linha: " + std::to_string(line_number) + ", line: " + line);
      }
      csv.dataset.push_back(row);
    }
  }

  file_handle.close();

  return std::make_pair(true, csv);
}

inline std::string output_data_field(std::string &data_field, char delimiter)
{
  static std::regex quote_pattern("\"");

  const bool contains_quote = contains(data_field, '"');
  const bool contains_delimiter = contains(data_field, delimiter);

  auto prepared_data_field = data_field;

  if (contains_delimiter && contains_quote)
  {
    prepared_data_field = std::regex_replace(prepared_data_field, quote_pattern, "\"\"");
  }

  if (contains_delimiter)
  {
    prepared_data_field = "\"" + prepared_data_field + "\"";
  }

  return prepared_data_field;
}

void emmit_row_data(std::ofstream &out, char delimiter, CSV_Data_Row &row)
{
  if (row.size() > 0)
  {
    auto &data_field = row[0];
    out << output_data_field(data_field, delimiter);

    for (size_t i = 1; i < row.size(); i++)
    {
      auto &data_field = row[i];

      out << delimiter;
      out << output_data_field(data_field, delimiter);
    }
  }

  out << std::endl; // @todo João, checar o que a RFC fala sobre a nova linha
}

bool write_csv_to_file(CSVData &data, const char* filename)
{
  std::ofstream outFile;

  outFile.open(filename);

  if (outFile.bad()) return false;

  // header
  emmit_row_data(outFile, data.delimiter, data.header);

  // dataset
  for (auto &row : data.dataset)
  {
    emmit_row_data(outFile, data.delimiter, row);
  }

  outFile.close();
  return true;
}

void emmit_sample_csv()
{
  CSVData csv;

  csv.header.push_back("id");
  csv.header.push_back("formato");
  csv.header.push_back("descrição");

  auto row1 = CSV_Data_Row();
  row1.push_back("1");
  row1.push_back("DD-mm-AAAA");
  row1.push_back("texto simples");
  csv.dataset.push_back(row1);

  auto row2 = CSV_Data_Row();
  row2.push_back("2");
  row2.push_back("DD,mm,AAAA");
  row2.push_back("texto simples com '\"' no meio");
  csv.dataset.push_back(row2);

  auto row3 = CSV_Data_Row();
  row3.push_back("3");
  row3.push_back("DD,mm,AAAA");
  row3.push_back("texto simples com , no meio");
  csv.dataset.push_back(row3);

  auto row4 = CSV_Data_Row();
  row4.push_back("4");
  row4.push_back("DD-mm-AAAA");
  row4.push_back("texto simples com ',' e '\"' no meio");
  csv.dataset.push_back(row4);

  write_csv_to_file(csv, "./emmited-sample.csv");
}

