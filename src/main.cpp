#include <iostream>
#include <string>

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
  }
  else
  {
    std::cout << "Fail!" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
