#include <iostream>
#include <string>

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
  
  return EXIT_SUCCESS;
}
