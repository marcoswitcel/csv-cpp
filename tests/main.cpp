#include <iostream>
#include <string>
#include <algorithm>
#include <assert.h>

#include "../src/csv.cpp"
#include "../src/utils.cpp"


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

void test_general_parse_success_with_empty_columns()
{
  auto result = parse_csv_from_file("./samples/test-empty-columns.csv");

  assert(result.first);

  auto csv = result.second;

  assert(csv.header.size() == 3);
  assert(csv.dataset.size() == 4);

  assert(csv.header.at(0).compare("nome") == 0);
  assert(csv.header.at(1).compare("descrição") == 0);
  assert(csv.header.at(2).compare("pontos") == 0);

  assert(csv.dataset.at(1).at(0).compare("Teste com strings vazias 2") == 0);
  
  assert(csv.dataset.at(0).at(1).compare("") == 0);
  assert(csv.dataset.at(0).at(2).compare("") == 0);
  assert(csv.dataset.at(1).at(1).compare("") == 0);
  assert(csv.dataset.at(1).at(2).compare("") == 0);
  assert(csv.dataset.at(2).at(1).compare("") == 0);
  assert(csv.dataset.at(2).at(2).compare("") == 0);
  assert(csv.dataset.at(3).at(1).compare("") == 0);
  assert(csv.dataset.at(3).at(2).compare("") == 0);

  csv.infer_types();
  assert(csv.infered_types_for_columns.size() == 3);
  
  assert(csv.infered_types_for_columns.at(0).type == TEXT);
  assert(!csv.infered_types_for_columns.at(0).nullable);
  
  assert(csv.infered_types_for_columns.at(1).type == TEXT);
  assert(csv.infered_types_for_columns.at(1).nullable);

  assert(csv.infered_types_for_columns.at(2).type == TEXT);
  assert(csv.infered_types_for_columns.at(2).nullable);

}

void test_general_parse_success_with_quoted_columns()
{
  auto result = parse_csv_from_file("./samples/test-quoted-columns.csv");

  assert(result.first);

  auto csv = result.second;

  assert(csv.header.size() == 3);
  assert(csv.dataset.size() == 4);

  assert(csv.header.at(0).compare("nome") == 0);
  assert(csv.header.at(1).compare("descrição") == 0);
  assert(csv.header.at(2).compare("pontos") == 0);

  assert(csv.dataset.at(0).size() == 3);
  assert(csv.dataset.at(0).at(0).compare("teste correto 01") == 0);
  assert(csv.dataset.at(0).at(1).compare("texto \" texto") == 0);
  assert(csv.dataset.at(0).at(2).compare("01") == 0);

  assert(csv.dataset.at(1).size() == 3);
  assert(csv.dataset.at(1).at(0).compare("teste correto 02") == 0);
  assert(csv.dataset.at(1).at(1).compare("texto \" texto") == 0);
  assert(csv.dataset.at(1).at(2).compare("02") == 0);

  assert(csv.dataset.at(2).size() == 3);
  assert(csv.dataset.at(2).at(0).compare("teste correto 03") == 0);
  assert(csv.dataset.at(2).at(1).compare("texto \"\" texto") == 0);
  assert(csv.dataset.at(2).at(2).compare("03") == 0);

  assert(csv.dataset.at(3).size() == 3);
  assert(csv.dataset.at(3).at(0).compare("teste correto 04") == 0);
  assert(csv.dataset.at(3).at(1).compare("texto \"\" texto") == 0);
  assert(csv.dataset.at(3).at(2).compare("04") == 0);

}

void test_split_by()
{
  {
    const char* sample_text = "nome,idade,descrição";
  
    auto list = split_by(sample_text, ',');
  
    assert(list.size() == 3);
    assert(list.at(0).compare("nome") == 0);
    assert(list.at(1).compare("idade") == 0);
    assert(list.at(2).compare("descrição") == 0);
  }

  {
    const char* sample_text = "nome-idade-descrição";
  
    auto list = split_by(sample_text, '-');
  
    assert(list.size() == 3);
    assert(list.at(0).compare("nome") == 0);
    assert(list.at(1).compare("idade") == 0);
    assert(list.at(2).compare("descrição") == 0);
  }

  {
    const char* sample_text = "nome-idade-descrição";
  
    auto list = split_by(sample_text, ',');
  
    assert(list.size() == 1);
    assert(list.at(0).compare("nome-idade-descrição") == 0);
  }

  {
    const char* sample_text = "nome,idade,";
  
    auto list = split_by(sample_text, ',');
  
    assert(list.size() == 3);
    assert(list.at(0).compare("nome") == 0);
    assert(list.at(1).compare("idade") == 0);
    assert(list.at(2).compare("") == 0);
  }

  {
    const char* sample_text = ",";
  
    auto list = split_by(sample_text, ',');
  
    assert(list.size() == 2);
    assert(list.at(0).compare("") == 0);
    assert(list.at(1).compare("") == 0);
  }

  {
    const char* sample_text = "";
  
    auto list = split_by(sample_text, ',');
  
    assert(list.size() == 1);
    assert(list.at(0).compare("") == 0);
  }
}

int main()
{
  std::cout << "Iniciando testes" << std::endl << std::endl;

  test_general_parse_success();
  std::cout << "test_general_parse_success......................................OK" << std::endl;
  test_general_parse_success_with_empty_columns();
  std::cout << "test_general_parse_success_with_empty_columns...................OK" << std::endl;
  test_general_parse_success_with_quoted_columns();
  std::cout << "test_general_parse_success_with_quoted_columns..................OK" << std::endl;
  test_split_by();
  std::cout << "test_split_by...................................................OK" << std::endl;

  std::cout << std::endl << "Fim testes" << std::endl;

  return EXIT_SUCCESS;
}
