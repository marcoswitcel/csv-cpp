#pragma once

#include <string.h>
#include <utility>

/**
 * @brief permite encontrar o índice de um dado parâmetro caso ele exista, caso não,
 * retorna -1, o que permite identificar a presença ou não do argumento.
 * 
 * @param argument 
 * @param argc 
 * @param argv 
 * @return int 
 */
int index_of_in_argv(const char *argument, int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], argument))
    {
      return i;
    }
  }
  return -1;
}

bool is_string_present_in_argv(const char *switch_name, int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], switch_name))
    {
      return true;
    }
  }
  return false;
}

std::pair<bool, const char*> get_value_for_in_argv(const char *argc_name, int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    if (!strcmp(argv[i], argc_name))
    {
      int next_index = i + 1;
      const char* value = NULL;
      if (next_index < argc)
      {
        value = argv[next_index];
      }

      return std::make_pair(true, value);
    }
  }
  return std::make_pair(false, static_cast<const char*>(NULL));
}
