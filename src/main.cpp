#include <iostream>
#include <string>
#include <algorithm>

#include "./command-line-utils.cpp"
#include "./csv.cpp"

int main(int argc, const char* argv[])
{
  // @todo João, implementar um sistema mais robusto organizado para lidar com os parâmetros
  if (argc < 2)
  {
    std::cout << "O arquivo não foi especificado!" << std::endl;
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

  std::cout << "Filename: " << filename << std::endl;
  auto result = parse_csv_from_file(filename.c_str());

  if (result.first)
  {
    auto csv = result.second;
    std::cout << "Success!" << std::endl;
    std::cout << "Header Size: " << csv.header.size() << std::endl;
    std::cout << "Dataset Size: " << csv.dataset.size() << std::endl;

    std::vector<std::string> filters;
    if (filter.found && filter.value)
    {
      filters.push_back(filter.value);
    }

    print_as_table(csv, filters);

    csv.infer_types();
    print_infered_types(csv);
    
  }
  else
  {
    std::cout << "Fail!" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
