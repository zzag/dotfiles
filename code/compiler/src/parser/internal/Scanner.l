%{
#include <stdio.h>
#include "_Parser.hh"

void yyerror(TopLevelDecl**, const char* message)
{
  fprintf(stderr, "error: %s\n", message);
}

%}

%x line_comment
%x block_comment
%x lit_char
%x lit_string

%%

\xef\xbb\xbf                                { }

[ \t\r\n]                                   { }

"//"                                        { BEGIN(line_comment); }
<line_comment>\n                            { BEGIN(INITIAL); }
<line_comment>.                             { }

"/*"                                        { BEGIN(block_comment); }
<block_comment>"*/"                         { BEGIN(INITIAL); }
<block_comment>.                            { }

"+"                                         { return PLUS; }
"-"                                         { return MINUS; }
"*"                                         { return STAR; }
"/"                                         { return SLASH; }
"%"                                         { return PERCENT; }
"<<"                                        { return SHL; }
">>"                                        { return SHR; }
"&"                                         { return AND; }
"|"                                         { return OR; }
"^"                                         { return CARET; }
"="                                         { return EQ; }
"<"                                         { return LT; }
">"                                         { return GT; }
"<="                                        { return LE; }
">="                                        { return GE; }
"=="                                        { return EQEQ; }
"!="                                        { return NE; }
"&&"                                        { return ANDAND; }
"||"                                        { return OROR; }
"+="                                        { return PLUSEQ; }
"-="                                        { return MINUSEQ; }
"*="                                        { return STAREQ; }
"/="                                        { return SLASHEQ; }
"%="                                        { return PERCENTEQ; }
"<<="                                       { return SHLEQ; }
">>="                                       { return SHREQ; }
"&="                                        { return ANDEQ; }
"|="                                        { return OREQ; }
"^="                                        { return CARETEQ; }
"!"                                         { return NOT; }
"++"                                        { return PLUSPLUS; }
"--"                                        { return MINUSMINUS; }

"("                                         { return LPAREN; }
")"                                         { return RPAREN; }
"{"                                         { return LBRACE; }
"}"                                         { return RBRACE; }
":"                                         { return COLON; }
";"                                         { return SEMI; }
","                                         { return COMMA; }
"."                                         { return DOT; }

"extern"                                    { return EXTERN; }
"as"                                        { return AS; }
"func"                                      { return FUNC; }
"if"                                        { return IF; }
"else"                                      { return ELSE; }
"for"                                       { return FOR; }
"while"                                     { return WHILE; }
"unless"                                    { return UNLESS; }
"loop"                                      { return LOOP; }
"break"                                     { return BREAK; }
"continue"                                  { return CONTINUE; }
"return"                                    { return RETURN; }
"var"                                       { return VAR; }
"true"                                      { return TRUE; }
"false"                                     { return FALSE; }

[a-zA-Z\x80-\xff_][a-zA-Z0-9\x80-\xff_]*    { return IDENTIFIER; }

0x[0-9a-fA-F_]+                             { return LIT_INTEGER; }
0o[0-8_]+                                   { return LIT_INTEGER; }
0b[01_]+                                    { return LIT_INTEGER; }
[0-9][0-9_]*                                { return LIT_INTEGER; }

[0-9][0-9_]*\.[0-9_]*([eE][-\+]?[0-9_]+)?   { return LIT_FLOAT; }
[0-9][0-9_]*(\.[0-9_]*)?[eE][-\+]?[0-9_]+   { return LIT_FLOAT; }

\x22                                        { BEGIN(lit_string); }
<lit_string>\x22                            { BEGIN(INITIAL); yytext[yyleng-1] = '\0'; return LIT_STRING; }
<lit_string><<EOF>>                         { return -1; }
<lit_string>\\[n\nr\rt\\\x27\x220]          { yymore(); }
<lit_string>\\x[0-9a-fA-F]{2}               { yymore(); }
<lit_string>\\u\{[0-9a-fA-F]?{6}\}          { yymore(); }
<lit_string>\\[^n\nrt\\\x27\x220]           { return -1; }
<lit_string>(.|\n)                          { yymore(); }

.                                           { }

%%

int yywrap()
{
  return 1;
}
