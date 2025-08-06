#include <iostream>
#include <string>
#include <algorithm>
#include <assert.h>

#include "../src/csv.cpp"

void test_general_parse_success()
{
  auto result = parse_csv_from_file("./samples/books.csv");

  assert(result.first);

  auto csv = result.second;

  assert(csv.header.size() == 3);
  assert(csv.dataset.size() == 4);

  assert(csv.header.at(0).compare("nome") == 0);
  assert(csv.header.at(1).compare("descrição") == 0);
  assert(csv.header.at(2).compare("pontos") == 0);

  assert(csv.dataset.at(1).at(0).compare("Batman: vol 14") == 0);
  assert(csv.dataset.at(1).at(2).compare("10") == 0);

  csv.infer_types();
  assert(csv.infered_types_for_columns.size() == 3);
  assert(csv.infered_types_for_columns.at(0).type == TEXT);
  assert(!csv.infered_types_for_columns.at(0).nullable);
  assert(csv.infered_types_for_columns.at(1).type == TEXT);
  assert(!csv.infered_types_for_columns.at(1).nullable);
  assert(csv.infered_types_for_columns.at(2).type == NUMBER);
  assert(!csv.infered_types_for_columns.at(2).nullable);

}

// @todo João, adicionar testes com linhas e colunas vazias

int main()
{
  std::cout << "Iniciando testes" << std::endl;

  test_general_parse_success();

  std::cout << "Fim testes" << std::endl;

  return EXIT_SUCCESS;
}
