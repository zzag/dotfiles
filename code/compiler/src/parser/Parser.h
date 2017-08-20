#ifndef _PARSER_H
#define _PARSER_H

#include "AST.h"
#include <string>

class Parser
{
public:
  Parser();
  ~Parser();

  TopLevelDecl* parse(std::string& text);
};

#endif /* PARSER_H */
