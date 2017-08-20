%{
extern char* yytext;
extern int yylex();
%}

%code requires {
#include "parser/AST.h"

extern void yyerror(TopLevelDecl**, const char *);

}

%union {
  TopLevelDecl *top_level_decl;
  Decl* decl;
  Stmt* stmt;
  Expr* expr;
  DeclList* decl_list;
  StmtList* stmt_list;
  ExprList* expr_list;
  std::string* string;
  int token;
}

%parse-param { TopLevelDecl **topLevelDecl }

/* expressions */
%token PLUS
%token MINUS
%token STAR
%token SLASH
%token PERCENT
%token SHL
%token SHR
%token AND
%token OR
%token CARET
%token EQ
%token LT
%token GT
%token LE
%token GE
%token EQEQ
%token NE
%token ANDAND
%token OROR
%token PLUSEQ
%token MINUSEQ
%token STAREQ
%token SLASHEQ
%token PERCENTEQ
%token SHLEQ
%token SHREQ
%token ANDEQ
%token OREQ
%token CARETEQ
%token NOT
%token PLUSPLUS
%token MINUSMINUS

/* structure */
%token LPAREN
%token RPAREN
%token LBRACE
%token RBRACE
%token COLON
%token SEMI
%token COMMA
%token DOT

/* keywords */
%token EXTERN
%token AS
%token FUNC
%token IF
%token ELSE
%token FOR
%token WHILE
%token UNLESS
%token LOOP
%token BREAK
%token CONTINUE
%token RETURN
%token VAR
%token TRUE
%token FALSE

/* other */
%token IDENTIFIER
%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_STRING

%nonassoc NONBLOCK_STMT
%nonassoc IF UNLESS

%precedence RETURN
%precedence IDENTIFIER SELF LIT_INTEGER LIT_FLOAT LIT_STRING TRUE FALSE PLUSPLUS MINUSMINUS
%right EQ SHLEQ SHREQ PLUSEQ MINUSEQ STAREQ SLASHEQ PERCENTEQ CARETEQ ANDEQ OREQ
%left OROR
%left ANDAND
%left EQEQ NE
%left LE GE LT GT
%left OR
%left CARET
%left AND
%left SHL SHR
%left PLUS MINUS
%left STAR SLASH PERCENT
%precedence AS
%precedence LPAREN
%nonassoc NOT
%nonassoc UNARY_MINUS
%nonassoc PREFIX_OP
%nonassoc POSTFIX_OP

%type <top_level_decl> top_level_decl
%type <decl> decl var_decl func_decl func_arg proto_decl
%type <decl_list> func_args maybe_func_args func_args_with_paren
%type <stmt> stmt if_stmt while_stmt unless_stmt loop_stmt return_stmt break_stmt continue_stmt expr_stmt decl_stmt
%type <stmt> block stmts maybe_stmts for_stmt for_init cfg_stmts loop_stmts cond_stmts
%type <expr> expr value variable lit_integer lit_float lit_boolean lit_string
%type <expr_list> maybe_exprs exprs
%type <string> ident type ret_type maybe_var_type var_type

%expect 0

%%

top_level_decl : decl { *topLevelDecl = new TopLevelDecl(); (*topLevelDecl)->add($<decl>1); }
               | top_level_decl decl { (*topLevelDecl)->add($<decl>2); }
               ;

decl : func_decl { $$ = $1; }
     | var_decl { $$ = $1; }
     ;

// var decl
var_decl : VAR ident maybe_var_type EQ expr { $$ = new VarDecl(*$2, *$3, $5);
                                              delete $2;
                                              delete $3;
                                            }
         ;

maybe_var_type : %empty { $$ = new std::string(); }
               | var_type { $$ = $1; }
               ;

var_type : COLON type { $$ = $2; }
         ;

// function decl
func_decl : proto_decl block { $$ = new FuncDecl((ProtoDecl*)$1, $2); }
          | EXTERN proto_decl { $$ = new ExternFuncDecl((ProtoDecl*)$2); }
          ;

proto_decl : FUNC ident func_args_with_paren ret_type {
                                                        $$ = new ProtoDecl(*$2, *$4, $3);
                                                        delete $2;
                                                        delete $4;
                                                      }
           ;

func_args_with_paren : LPAREN maybe_func_args RPAREN { $$ = $2; }
                     ;

maybe_func_args : %empty { $$ = new DeclList(); }
                | func_args { $$ = $1; }
                | func_args COMMA { $$ = $1; }
                ;

func_args : func_arg { $$ = new DeclList(); $$->push_back($1); }
          | func_args COMMA func_arg { $1->push_back($3); }
          ;

func_arg : ident COLON type { $$ = new VarDecl(*$1, *$3); delete $1; delete $3; }
         ;

ret_type : %empty { $$ = new std::string(); }
         | COLON type { $$ = $2; }
         ;

// statements
stmts : stmt { $$ = new BlockStmt(); ((BlockStmt*)$$)->add($1); }
      | stmts stmt { ((BlockStmt*)$1)->add($2); }
      ;

stmt : cond_stmts { $$ = $1; }
     | loop_stmts %prec NONBLOCK_STMT { $$ = $1; }
     | cfg_stmts SEMI { $$ = $1; }
     | expr_stmt SEMI { $$ = $1; }
     | decl_stmt SEMI { $$ = $1; }
     ;

cond_stmts : if_stmt { $$ = $1; }
           | loop_stmts IF expr SEMI { $$ = buildPostfixCondStmt($3, $1); }
           | cfg_stmts IF expr SEMI { $$ = buildPostfixCondStmt($3, $1); }
           | expr_stmt IF expr SEMI { $$ = buildPostfixCondStmt($3, $1); }
           | loop_stmts UNLESS expr SEMI { $$ = buildPostfixCondStmt(buildNot($3), $1); }
           | cfg_stmts UNLESS expr SEMI { $$ = buildPostfixCondStmt(buildNot($3), $1); }
           | expr_stmt UNLESS expr SEMI { $$ = buildPostfixCondStmt(buildNot($3), $1); }
           ;

loop_stmts : for_stmt { $$ = $1; }
           | while_stmt { $$ = $1; }
           | unless_stmt { $$ = $1; }
           | loop_stmt { $$ = $1; }
           ;

cfg_stmts : return_stmt { $$ = $1; }
          | break_stmt { $$ = $1; }
          | continue_stmt { $$ = $1; }
          ;

block : LBRACE maybe_stmts RBRACE { $$ = $2; };
      ;

maybe_stmts : %empty { $$ = new BlockStmt(); }
            | stmts { $$ = $1; }
            ;

if_stmt : IF expr block { $$ = new IfStmt($2, $3); }
        | IF expr block ELSE block { $$ = new IfStmt($2, $3, $5); }
        | IF expr block ELSE if_stmt { $$ = new IfStmt($2, $3, $5); }
        ;

for_stmt : FOR for_init COMMA expr COMMA expr block { $$ = new ForStmt($2, $4, $6, $7); }
         ;

for_init : var_decl { $$ = new DeclStmt($1); }
         | expr { $$ = new ExprStmt($1); }
         ;

while_stmt : WHILE expr block { $$ = new WhileStmt($2, $3); }
           ;

unless_stmt : UNLESS expr block { $$ = new UnlessStmt($2, $3); }
            ;

loop_stmt : LOOP block { $$ = new LoopStmt($2); }
          ;

return_stmt : RETURN expr { $$ = new ReturnStmt($2); }
            | RETURN { $$ = new ReturnStmt(); }
            ;

break_stmt : BREAK { $$ = new BreakStmt(); }
           ;

continue_stmt : CONTINUE { $$ = new ContinueStmt(); }
              ;

expr_stmt : expr { $$ = new ExprStmt($1); }
          ;

decl_stmt : decl { $$ = new DeclStmt($1); }
          ;

// expressions
expr : value { $$ = $1; }
     | NOT expr { $$ = new UnaryOpExpr(ops::Not, $2); }
     | ident LPAREN maybe_exprs RPAREN { $$ = new CallExpr(*$1, $3); delete $1; }
     | MINUS expr %prec UNARY_MINUS { $$ = new UnaryOpExpr(ops::Neg, $2); }
     | MINUSMINUS expr %prec PREFIX_OP { $$ = new UnaryOpExpr(ops::PrefixDec, $2); }
     | PLUSPLUS expr %prec PREFIX_OP { $$ = new UnaryOpExpr(ops::PrefixInc, $2); }
     | expr MINUSMINUS %prec POSTFIX_OP { $$ = new UnaryOpExpr(ops::PostfixDec, $1); }
     | expr PLUSPLUS %prec POSTFIX_OP { $$ = new UnaryOpExpr(ops::PostfixInc, $1); }
     | LPAREN expr RPAREN { $$ = $2; }
     | expr PLUS expr { $$ = new BinOpExpr(ops::Add, $1, $3); }
     | expr MINUS expr { $$ = new BinOpExpr(ops::Sub, $1, $3); }
     | expr STAR expr { $$ = new BinOpExpr(ops::Mul, $1, $3); }
     | expr SLASH expr { $$ = new BinOpExpr(ops::Div, $1, $3); }
     | expr PERCENT expr { $$ = new BinOpExpr(ops::Mod, $1, $3); }
     | expr SHL expr { $$ = new BinOpExpr(ops::Shl, $1, $3); }
     | expr SHR expr { $$ = new BinOpExpr(ops::Shr, $1, $3); }
     | expr AND expr { $$ = new BinOpExpr(ops::BitAnd, $1, $3); }
     | expr OR expr { $$ = new BinOpExpr(ops::BitOr, $1, $3); }
     | expr CARET expr { $$ = new BinOpExpr(ops::BitXor, $1, $3); }
     | expr EQ expr { $$ = new BinOpExpr(ops::Assign, $1, $3); }
     | expr LT expr { $$ = new BinOpExpr(ops::Lt, $1, $3); }
     | expr GT expr { $$ = new BinOpExpr(ops::Gt, $1, $3); }
     | expr LE expr { $$ = new BinOpExpr(ops::Le, $1, $3); }
     | expr GE expr { $$ = new BinOpExpr(ops::Ge, $1, $3); }
     | expr EQEQ expr { $$ = new BinOpExpr(ops::Eq, $1, $3); }
     | expr NE expr { $$ = new BinOpExpr(ops::Ne, $1, $3); }
     | expr ANDAND expr { $$ = new BinOpExpr(ops::And, $1, $3); }
     | expr OROR expr { $$ = new BinOpExpr(ops::Or, $1, $3); }
     | expr PLUSEQ expr { $$ = buildAssignOp(ops::Add, $1, $3); }
     | expr MINUSEQ expr { $$ = buildAssignOp(ops::Sub, $1, $3); }
     | expr STAREQ expr { $$ = buildAssignOp(ops::Mul, $1, $3); }
     | expr SLASHEQ expr { $$ = buildAssignOp(ops::Div, $1, $3); }
     | expr PERCENTEQ expr { $$ = buildAssignOp(ops::Mod, $1, $3); }
     | expr SHLEQ expr { $$ = buildAssignOp(ops::Shl, $1, $3); }
     | expr SHREQ expr { $$ = buildAssignOp(ops::Shr, $1, $3); }
     | expr ANDEQ expr { $$ = buildAssignOp(ops::BitAnd, $1, $3); }
     | expr OREQ expr { $$ = buildAssignOp(ops::BitOr, $1, $3); }
     | expr CARETEQ expr { $$ = buildAssignOp(ops::BitXor, $1, $3); }
     | expr AS type { $$ = new CastExpr($1, *$3); delete $3; }
     ;

value : variable { $$ = $1; }
      | lit_integer { $$ = $1; }
      | lit_float { $$ = $1; }
      | lit_boolean { $$ = $1; }
      | lit_string { $$ = $1; }
      ;

maybe_exprs : %empty { $$ = new ExprList(); }
            | exprs { $$ = $1; }
            | exprs COMMA { $$ = $1; }
            ;

exprs : expr { $$ = new ExprList(); $$->push_back($1); }
      | exprs COMMA expr { $1->push_back($3); }
      ;

// common
ident : IDENTIFIER { $$ = new std::string(yytext); };
variable : ident { $$ = new VarExpr(*$1); delete $1; };
lit_integer : LIT_INTEGER { $$ = new IntegerLitExpr(std::string(yytext)); };
lit_float : LIT_FLOAT { $$ = new FloatLitExpr(std::string(yytext)); };
lit_boolean : TRUE { $$ = new BooleanLitExpr(true); }
            | FALSE { $$ = new BooleanLitExpr(false); }
            ;
lit_string : LIT_STRING { $$ = new StringLitExpr(std::string(yytext)); };
type : IDENTIFIER { $$ = new std::string(yytext); };

%%

