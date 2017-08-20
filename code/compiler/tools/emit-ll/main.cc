#include "frontend/CompilerFrontend.h"


int main(int argc, char** argv)
{
  std::string filename = argv[1];
  CompilerFrontend frontend(filename);
  frontend.emitIR();
  return 0;
}
