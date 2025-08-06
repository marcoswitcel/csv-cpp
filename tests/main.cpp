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

  csv.infer_types();
  assert(csv.infered_types_for_columns.size() == 3);
  assert(csv.infered_types_for_columns.at(0).type == TEXT);
  assert(!csv.infered_types_for_columns.at(0).nullable);
  assert(csv.infered_types_for_columns.at(1).type == TEXT);
  assert(!csv.infered_types_for_columns.at(1).nullable);
  assert(csv.infered_types_for_columns.at(2).type == NUMBER);
  assert(!csv.infered_types_for_columns.at(2).nullable);

}

int main()
{
  test_general_parse_success();

  return EXIT_SUCCESS;
}
