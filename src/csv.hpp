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

std::string to_string(Data_Cell_Type &type) {
  return type == TEXT ? "TEXT" : "NUMBER";
}

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
        
        if (dataField.size() == 0)
        {
          col_info.nullable = true;
          continue;
        }
        
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

      this->infered_types_for_columns.push_back(col_info);
    }
  }
};
