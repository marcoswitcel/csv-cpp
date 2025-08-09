#pragma once

#include <vector>
#include <string>
#include <utility>

// @note João, considerar usar cstrings e array estáticos para o header e as linhas
using CSV_Data_Row = std::vector<std::string>;

enum Data_Cell_Type
{
  TEXT,
  // @note João, adicionar distinção entre inteiros, floats e pelo tamanho dos números?
  NUMBER,
};

std::string to_string(Data_Cell_Type &type);

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
  // @todo Suportar outros delimitadores no futuro
  char delimiter = ',';

  /**
   * @brief infere os tipos de dados para as colunas
   * 
   */
  void infer_types();
};

std::pair<bool, CSVData> parse_csv_from_file(const char* filename);

/**
 * @brief função que printa os dados em formato de tabela
 * 
 * @todo João, otimizar e ou escapar caracteres especiais para não quebrar o layout, bem como
 * calcular tamanho de colunas de forma dinâmica e escrever para um arquivo ao invés do 'std::cout'
 * 
 * @param csv 
 * @param filters colunas que não devem ser exibidas
 * @param field_size_limit 
 */
void print_as_table(CSVData &csv, std::vector<std::string> &filters, int field_size_limit = 30);

void print_infered_types(CSVData &csv);

