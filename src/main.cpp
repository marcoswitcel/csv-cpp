#include <iostream>
#include <string>
#include <algorithm>

#include "./csv.cpp"

int main(int argc, const char* argv[])
{
  std::cout << "Olá mundo do CSV!" << std::endl;

  // @todo João, implementar um sistema mais robusto organizado para lidar com os parâmetros
  if (argc < 2)
  {
    std::cout << "O arquivo não foi especificado!" << std::endl;
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

    // @note apenas para testes...
    std::vector<std::string> filters;
    filters.push_back("score");
    std::vector<size_t> filters_index;

    // header
    for (const auto &dataField : csv.header)
    {
      std::cout << dataField << "||";
    }
    std::cout << std::endl;

    for (const auto &filter: filters)
    {
      auto it = std::find(filters.begin(), filters.end(), filter);
      if (it != filters.end())
      {
        filters_index.push_back(std::distance(filters.begin(), it));
      }
    }

    // data
    for (const auto &row : csv.dataset)
    {
      for (const auto &dataField : row)
      {
        std::cout << dataField << "||";
      }
      std::cout << std::endl;
    }
  }
  else
  {
    std::cout << "Fail!" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
