#include <iostream>
#include <string>
#include <algorithm>

#include "./command-line-utils.cpp"
#include "./utils.cpp"
#include "./csv.cpp"

int main(int argc, const char* argv[])
{
  // @todo João, implementar um sistema mais robusto organizado para lidar com os parâmetros
  if (argc < 2)
  {
    std::cout << "O arquivo não foi especificado!" << std::endl;
    return EXIT_FAILURE;
  }

  bool is_verbose = is_string_present_in_argv("--verbose", argc, argv);
  bool is_infer_types = is_string_present_in_argv("--infer-types", argc, argv);
  bool is_table_print = is_string_present_in_argv("--table-print", argc, argv);
  bool is_emmit_sample_csv = is_string_present_in_argv("--emmit-sample-csv", argc, argv);

  if (!is_verbose && !is_infer_types && !is_table_print && !is_emmit_sample_csv)
  {
    std::cout << "Nenhum parâmetro que gere output especificado!" << std::endl;
    return EXIT_FAILURE;
  }

  // @note João, permite filtrar apenas uma coluna por vez... pode ser melhorado no futuro.
  auto filter = get_value_for_in_argv("--filter", argc, argv);

  if (filter.found && !filter.value)
  {
    std::cout << "Filtro não especificado!" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename = std::string(argv[1]);

  if (is_verbose) std::cout << "Filename: " << filename << std::endl;

  auto result = parse_csv_from_file(filename.c_str());

  if (result.first)
  {
    auto csv = result.second;
    if (is_verbose)
    {
      std::cout << "Success!" << std::endl;
      std::cout << "Header Size: " << csv.header.size() << std::endl;
      std::cout << "Dataset Size: " << csv.dataset.size() << std::endl;
    }

    std::vector<std::string> filters;
    if (filter.found && filter.value)
    {
      // @note João, não suporta nomes de colunas que contenham o separador
      filters = split_by(std::string(filter.value), ',');
    }

    if (is_table_print) print_as_table(csv, filters);

    if (is_infer_types)
    {
      csv.infer_types();
      print_infered_types(csv);
    }
    
  }
  else
  {
    if (is_verbose) std::cout << "Fail!" << std::endl;
  }

  if (is_emmit_sample_csv)
  {
    emmit_sample_csv();
    if (is_verbose) std::cout << "Arquivo CSV criado: ./emmited-sample.csv" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
