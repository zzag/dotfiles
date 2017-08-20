#include "Parser.h"
#include "_Parser.hh"

struct yy_buffer_state;
extern yy_buffer_state* yy_scan_string(const char *);
extern void yy_delete_buffer(yy_buffer_state*);
extern int yyparse(TopLevelDecl **);

Parser::Parser()
{
}


Parser::~Parser()
{
}


TopLevelDecl*
Parser::parse(std::string& text)
{
  TopLevelDecl* decl = nullptr;

  yy_buffer_state* buffer = yy_scan_string(text.c_str());
  yyparse(&decl);
  yy_delete_buffer(buffer);
  return decl;
}
