#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <fstream>
#include <string>
#include <sstream>

class FileLoader
{
public:
  static bool load(std::string filename, std::string& buffer)
  {
    std::ifstream fin;
    fin.open(filename);

    if (! fin.is_open()) return false;

    std::stringstream stream;
    stream << fin.rdbuf();
    buffer = stream.str();

    return true;
  }
};

#endif /* FILE_LOADER_H */
