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
  /**
   * @brief aceita null (string vazia)
   * 
   */
  bool nullable;
};

struct CSVData
{
  char delimiter = ',';
  // @todo João, armazenar se houve algum erro em alguma linha e talvez traquear as linhas problemáticas
  // para remoção ou tratamento
  CSV_Data_Row header;
  std::vector<CSV_Data_Row> dataset;
  std::vector<Data_Cell_Type_Info> infered_types_for_columns;

  /**
   * @brief infere os tipos de dados para as colunas carregadas
   */
  void infer_types();
};

/**
 * @brief carrega e parseia um arquivo 'CSV'
 * 
 * @param filename nome/caminho do arquivo
 * @return std::pair<bool, CSVData> .first para saber o sucesso ou falha da
 * operação e .second para o valor em caso de sucesso.
 */
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

/**
 * @brief função que printa os dados de tipos inferidos para o 'CSV'
 * 
 * @todo João, ainda falta avaliar a RFC do CSV para comparar minha
 * implementação com as expectativas definidas lá.
 * 
 * @param csv 
 */
void print_infered_types(CSVData &csv);

