#ifndef AST_H
#define AST_H

#include <string>
#include <memory>
#include <vector>


template<typename To, typename From>
bool
isa(From const *f)
{
  return To::classof(f);
}


template<typename To, typename From>
To *
dyncast(From *f)
{
  if (To::classof(f))
    return reinterpret_cast<To *>(f);
  return nullptr;
}


namespace ops 
{
  enum BinaryOp
  {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Shl,
    Shr,
    BitAnd,
    BitOr,
    BitXor,
    Assign,
    Lt,
    Gt,
    Le,
    Ge,
    Eq,
    Ne,
    And,
    Or
  };

  enum UnaryOp
  {
    Not,
    Neg,
    PrefixDec,
    PrefixInc,
    PostfixDec,
    PostfixInc
  };
}


namespace internal
{
  enum NodeKind
  {
    TopLevelDeclId,
    VarDeclId,
    ProtoDeclId,
    FuncDeclId,
    ExternFuncDeclId,
    BlockStmtId,
    DeclStmtId,
    ExprStmtId,
    IfStmtId,
    ForStmtId,
    WhileStmtId,
    UnlessStmtId,
    BreakStmtId,
    ContinueStmtId,
    ReturnStmtId,
    LoopStmtId,
    VarExprId,
    IntegerLitExprId,
    BooleanLitExprId,
    FloatLitExprId,
    StringLitExprId,
    BinOpExprId,
    UnaryOpExprId,
    CastExprId,
    CallExprId
  };
}


class Visitor;

class ASTNode
{
public:
  ASTNode(internal::NodeKind kind) : kind(kind) {}
  virtual ~ASTNode() {}

  internal::NodeKind getKind() const { return kind; }
  virtual void accept(Visitor *v) = 0;

private:
  internal::NodeKind kind;
};


// -=====================================-
//
// AST categories
//
// -=====================================-

class Decl : public ASTNode
{
public:
  Decl(internal::NodeKind kind) : ASTNode(kind) {}
  static bool classof(Decl const *) { return true; }
};


class Stmt : public ASTNode
{
public:
  Stmt(internal::NodeKind kind) : ASTNode(kind) {}
  static bool classof(Stmt const *) { return true; }
};


class Expr : public ASTNode
{
public:
  Expr(internal::NodeKind kind) : ASTNode(kind) {}
  static bool classof(Expr const *) { return true; }
};


typedef std::vector<Decl*> DeclList;
typedef std::vector<Stmt*> StmtList;
typedef std::vector<Expr*> ExprList;


// -=====================================-
//
// DECLARATIONS
//
// -=====================================-

class TopLevelDecl : public Decl
{
public:
  TopLevelDecl() : Decl(internal::TopLevelDeclId) {}
  ~TopLevelDecl()
  {
    for (auto decl : decls)
      delete decl;
  }

  static bool classof(TopLevelDecl const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::TopLevelDeclId; }

  void clear() { decls.clear(); }
  void add(Decl* decl) { decls.push_back(decl); }
  DeclList& getDecls() { return decls; }

  void accept(Visitor *v);

private:
  DeclList decls;
};


class VarDecl : public Decl
{
public:
  VarDecl(std::string id, std::string type, Expr* value = nullptr) :
      id(id), type(type), value(value), Decl(internal::VarDeclId)
  {
  }
  ~VarDecl()
  {
    if (value) delete value;
  }

  static bool classof(VarDecl const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::VarDeclId; }

  std::string getId() { return id; }
  void setId(std::string id) { this->id = id; }
  std::string getType() { return type; }
  void setType(std::string type) { this->type = type; }
  Expr* getInitialValue() { return value; }
  void setInitialValue(Expr* value) { this->value = value; }

  void accept(Visitor *v);

private:
  std::string id;
  std::string type;
  Expr* value;
};


class ProtoDecl : public Decl
{
public:
  ProtoDecl(std::string id, std::string retType, DeclList* args) :
      id(id), retType(retType), args(args), Decl(internal::ProtoDeclId)
  {
  }
  ~ProtoDecl()
  {
    for (auto arg : *args) delete arg;
    delete args;
  }

  static bool classof(ProtoDecl const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ProtoDeclId; }

  std::string getId() { return id; }
  void setId(std::string id) { this->id = id; }
  std::string getReturnType() { return retType; }
  void setReturnType(std::string retType) { this->retType = retType; }
  DeclList& getArgs() { return *args; }
  void addArg(VarDecl* arg) { args->push_back(arg); }
  void clearArgs() { args->clear(); }

  void accept(Visitor *v);

private:
  std::string id;
  std::string retType;
  DeclList* args;
};


class FuncDecl : public Decl
{
public:
  FuncDecl(ProtoDecl* proto, Stmt* body) :
      proto(proto), body(body), Decl(internal::FuncDeclId)
  {
  }
  ~FuncDecl()
  {
    delete proto;
    delete body;
  }

  static bool classof(FuncDecl const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::FuncDeclId; }

  ProtoDecl* getProto() { return proto; }
  void setProto(ProtoDecl* proto) { this->proto = proto; }
  Stmt* getBody() { return body; }
  void setBody(Stmt* body) { this->body = body; }

  void accept(Visitor *v);

private:
  ProtoDecl* proto;
  Stmt* body;
};


class ExternFuncDecl : public Decl
{
public:
  ExternFuncDecl(ProtoDecl* proto) : proto(proto), Decl(internal::ExternFuncDeclId) {}
  ~ExternFuncDecl() { delete proto; }

  static bool classof(ExternFuncDecl const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ExternFuncDeclId; }

  ProtoDecl* getProto() { return proto; }
  void setProto(ProtoDecl* proto) { this->proto = proto; }

  void accept(Visitor *v);

private:
  ProtoDecl* proto;
};


// -=====================================-
//
// STATEMENTS
//
// -=====================================-

class BlockStmt : public Stmt
{
public:
  BlockStmt() : Stmt(internal::BlockStmtId) {}
  ~BlockStmt()
  {
    for (auto stmt : stmts) delete stmt;
  }

  static bool classof(BlockStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::BlockStmtId; }

  void add(Stmt* stmt) { stmts.push_back(stmt); }
  StmtList& getStmts() { return stmts; }

  void accept(Visitor *v);

private:
  StmtList stmts;
};


class DeclStmt : public Stmt
{
public:
  DeclStmt(Decl* decl) : decl(decl), Stmt(internal::DeclStmtId) {}
  ~DeclStmt() { delete decl; }

  static bool classof(DeclStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::DeclStmtId; }

  Decl* getDecl() { return decl; }
  void setDecl(Decl* decl) { this->decl = decl; }

  void accept(Visitor *v);

private:
  Decl* decl;
};


class ExprStmt : public Stmt
{
public:
  ExprStmt(Expr* expr) : expr(expr), Stmt(internal::ExprStmtId) {}
  ~ExprStmt() { delete expr; }

  static bool classof(ExprStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ExprStmtId; }

  Expr* getExpr() { return expr; }
  void setExpr(Expr* expr) { this->expr = expr; }

  void accept(Visitor *v);

private:
  Expr* expr;
};


class IfStmt : public Stmt
{
public:
  IfStmt(Expr* cond, Stmt* then, Stmt* otherwise = nullptr) :
      cond(cond), then(then), otherwise(otherwise),
      Stmt(internal::IfStmtId)
  {
  }
  ~IfStmt()
  {
    delete cond;
    delete then;
    if (otherwise) delete otherwise;
  }

  static bool classof(IfStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::IfStmtId; }

  Expr* getCond() { return cond; }
  void setCond(Expr* cond) { this->cond = cond; }
  Stmt* getThen() { return then; }
  void setThen(Stmt* then) { this->then = then; }
  Stmt* getOtherwise() { return otherwise; }
  void setOtherwise(Stmt* otherwise) { this->otherwise = otherwise; }

  void accept(Visitor *v);

private:
  Expr* cond;
  Stmt* then;
  Stmt* otherwise;
};


class ForStmt : public Stmt
{
public:
  ForStmt(Stmt* init, Expr* cond, Expr* step, Stmt* body) :
      init(init), cond(cond), step(step), body(body), Stmt(internal::ForStmtId)
  {
  }
  ~ForStmt()
  {
    delete init;
    delete cond;
    delete step;
    delete body;
  }

  static bool classof(ForStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ForStmtId; }

  Stmt* getInit() { return init; }
  void setInit(Stmt* init) { this->init = init; }
  Expr* getCond() { return cond; }
  void setCond(Expr* cond) { this->cond = cond; }
  Expr* getStep() { return step; }
  void setStep(Expr* step) { this->step = step; }
  Stmt* getBody() { return body; }
  void setBody(Stmt* body) { this->body = body; }

  void accept(Visitor* v);

private:
  Stmt* init;
  Expr* cond;
  Expr* step;
  Stmt* body;
};


class WhileStmt : public Stmt
{
public:
  WhileStmt(Expr* cond, Stmt* body) :
      cond(cond), body(body),
      Stmt(internal::WhileStmtId) {}
  ~WhileStmt()
  {
    delete cond;
    delete body;
  }
  
  static bool classof(WhileStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::WhileStmtId; }

  Expr* getCond() { return cond; }
  void setCond(Expr* cond) { this->cond = cond; }
  Stmt* getBody() { return body; }
  void setBody(Stmt* body) { this->body = body; }

  void accept(Visitor *v);

private:
  Expr* cond;
  Stmt* body;
};


class UnlessStmt : public Stmt
{
public:
  UnlessStmt(Expr* cond, Stmt* body) :
      cond(cond), body(body), Stmt(internal::UnlessStmtId) {}
  ~UnlessStmt()
  {
    delete cond;
    delete body;
  }

  static bool classof(UnlessStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::UnlessStmtId; }

  Expr* getCond() { return cond; }
  void setCond(Expr* cond) { this->cond = cond; }
  Stmt* getBody() { return body; }
  void setBody(Stmt* body) { this->body = body; }

  void accept(Visitor *v);

private:
  Expr* cond;
  Stmt* body;
};


class BreakStmt : public Stmt
{
public:
  BreakStmt() : Stmt(internal::BreakStmtId) {}
  ~BreakStmt() {}

  static bool classof(BreakStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::BreakStmtId; }

  void accept(Visitor *v);
};


class ContinueStmt : public Stmt
{
public:
  ContinueStmt() : Stmt(internal::ContinueStmtId) {}
  ~ContinueStmt() {}

  static bool classof(ContinueStmt const *e) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ContinueStmtId; }

  void accept(Visitor *v);
};


class ReturnStmt : public Stmt
{
public:
  ReturnStmt(Expr* value = nullptr) : value(value), Stmt(internal::ReturnStmtId) {}
  ~ReturnStmt() { delete value; }

  static bool classof(ReturnStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::ReturnStmtId; }

  Expr* getValue() { return value; }
  void setValue(Expr* value) { this->value = value; }

  void accept(Visitor *v);

private:
  Expr* value;
};


class LoopStmt : public Stmt
{
public:
  LoopStmt(Stmt* body) : body(body), Stmt(internal::LoopStmtId) {}
  ~LoopStmt() { delete body; }

  static bool classof(LoopStmt const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::LoopStmtId; }

  Stmt* getBody() { return body; }
  void setBody(Stmt* body) { this->body = body; }

  void accept(Visitor *v);

private:
  Stmt* body;
};

// -=====================================-
//
// EXPRESSIONS
//
// -=====================================-

class VarExpr : public Expr
{
public:
  VarExpr(std::string id) : id(id), Expr(internal::VarExprId) {}
  ~VarExpr() {}

  static bool classof(VarExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::VarExprId; }

  std::string getId() { return id; }
  void setId(std::string id) { this->id = id; }

  void accept(Visitor *v);

private:
  std::string id;
};


class IntegerLitExpr : public Expr
{
public:
  IntegerLitExpr(std::string value) : value(value), Expr(internal::IntegerLitExprId) {}
  ~IntegerLitExpr() {}

  static bool classof(IntegerLitExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::IntegerLitExprId; }

  std::string getValue() { return value; }
  void setValue(std::string value) { this->value = value; }

  void accept(Visitor *v);

private:
  std::string value;
};


class BooleanLitExpr : public Expr
{
public:
  BooleanLitExpr(bool value) : value(value), Expr(internal::BooleanLitExprId) {}
  ~BooleanLitExpr() {}

  static bool classof(BooleanLitExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::BooleanLitExprId; }

  bool getValue() { return value; }
  void setValue(bool value) { this->value = value; }

  void accept(Visitor* v);

private:
  bool value;
};


class FloatLitExpr : public Expr
{
public:
  FloatLitExpr(std::string value) : value(value), Expr(internal::FloatLitExprId) {}
  ~FloatLitExpr() {}

  static bool classof(FloatLitExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::FloatLitExprId; }

  std::string getValue() { return value; }
  void setValue(std::string value) { this->value = value; }

  void accept(Visitor *v);

private:
  std::string value;
};


class StringLitExpr : public Expr
{
public:
  StringLitExpr(std::string value) : value(value), Expr(internal::StringLitExprId)
  {
    unescape();
  }
  ~StringLitExpr() {}

  static bool classof(StringLitExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::StringLitExprId; }

  std::string getValue() { return value; }
  void setValue(std::string value) { this->value = value; }

  void accept(Visitor *v);

private:
  void unescape()
  {
    std::string unescaped;
    auto i = value.begin(); 
    while (i != value.end())
    {
      auto c = *i++;
      if (c == '\\' && i != value.end())
      {
        switch (*i++)
        {
          case '\\': c = '\\'; break;
          case 'n': c = '\n'; break;
          case 't': c = '\t'; break;
          default:
            break;
        }
      }

      unescaped += c;
    }

    value = unescaped;
  }

private:
  std::string value;
};


class BinOpExpr : public Expr
{
public:
  BinOpExpr(ops::BinaryOp op, Expr* lhs, Expr* rhs) :
      op(op), lhs(lhs), rhs(rhs), Expr(internal::BinOpExprId)
  {
  }
  ~BinOpExpr()
  {
    delete lhs;
    delete rhs;
  }

  static bool classof(BinOpExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::BinOpExprId; }

  ops::BinaryOp getOp() { return op; }
  void setOp(ops::BinaryOp op) { this->op = op; }
  Expr* getLHS() { return lhs; }
  void setLHS(Expr* lhs) { this->lhs = lhs; }
  Expr* getRHS() { return rhs; }
  void setRHS(Expr* rhs) { this->rhs = rhs; }

  void accept(Visitor *v);

private:
  ops::BinaryOp op;
  Expr* lhs;
  Expr* rhs;
};


class UnaryOpExpr : public Expr
{
public:
  UnaryOpExpr(ops::UnaryOp op, Expr* expr) :
      op(op), expr(expr), Expr(internal::UnaryOpExprId)
  {
  }
  ~UnaryOpExpr() { delete expr; }

  static bool classof(UnaryOpExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::UnaryOpExprId; }

  ops::UnaryOp getOp() { return op; }
  void setOp(ops::UnaryOp op) { this->op = op; }
  Expr* getExpr() { return expr; }
  void setExpr(Expr* expr) { this->expr = expr; }

  void accept(Visitor *v);

private:
  ops::UnaryOp op;
  Expr* expr;
};


class CastExpr : public Expr
{
public:
  CastExpr(Expr* expr, std::string type) :
      expr(expr), type(type), Expr(internal::CastExprId)
  {
  }
  ~CastExpr() { delete expr; }

  static bool classof(CastExpr const *) { return true;}
  static bool classof(ASTNode const *n) { return n->getKind() == internal::CastExprId; }

  Expr* getExpr() { return expr; }
  void setExpr(Expr* expr) { this->expr = expr; }
  std::string getType() { return type; }
  void setType(std::string type) { this->type = type; }

  void accept(Visitor *v);

private:
  Expr* expr;
  std::string type;
};


class CallExpr : public Expr
{
public:
  CallExpr(std::string id, ExprList* args) :
      id(id), args(args), Expr(internal::CallExprId)
  {
  }
  ~CallExpr()
  {
    for (auto arg : *args) delete arg;
    delete args;
  }

  static bool classof(CallExpr const *) { return true; }
  static bool classof(ASTNode const *n) { return n->getKind() == internal::CallExprId; }

  std::string getId() { return id; }
  void setId(std::string id) { this->id = id; }
  ExprList& getArgs() { return *args; }
  void addArg(Expr* arg) { args->push_back(arg); }
  void clearArgs() { args->clear(); }

  void accept(Visitor *v);

private:
  std::string id;
  ExprList* args;
};


// -=====================================-
//
// AST visitor
//
// -=====================================-


class Visitor
{
public:
  Visitor() {}
  virtual ~Visitor() {}

  bool visit(ASTNode *n);
  virtual void visitBeforeTopLevelDecl(TopLevelDecl*) {}
  virtual bool visitTopLevelDecl(TopLevelDecl*) { return true; }
  virtual void visitAfterTopLevelDecl(TopLevelDecl*) {}
  
  virtual void visitBeforeVarDecl(VarDecl*) {}
  virtual bool visitVarDecl(VarDecl*) { return true; }
  virtual void visitAfterVarDecl(VarDecl*) {}
  
  virtual void visitBeforeProtoDecl(ProtoDecl*) {}
  virtual bool visitProtoDecl(ProtoDecl*) { return true; }
  virtual void visitAfterProtoDecl(ProtoDecl*) {}
  
  virtual void visitBeforeFuncDecl(FuncDecl*) {}
  virtual bool visitFuncDecl(FuncDecl*) { return true; }
  virtual void visitAfterFuncDecl(FuncDecl*) {}
  
  virtual void visitBeforeExternFuncDecl(ExternFuncDecl*) {}
  virtual bool visitExternFuncDecl(ExternFuncDecl*) { return true; }
  virtual void visitAfterExternFuncDecl(ExternFuncDecl*) {}

  virtual void visitBeforeBlockStmt(BlockStmt*) {}
  virtual bool visitBlockStmt(BlockStmt*) { return true; }
  virtual void visitAfterBlockStmt(BlockStmt*) {}

  virtual void visitBeforeDeclStmt(DeclStmt*) {}
  virtual bool visitDeclStmt(DeclStmt*) { return true; }
  virtual void visitAfterDeclStmt(DeclStmt*) {}

  virtual void visitBeforeExprStmt(ExprStmt*) {}
  virtual bool visitExprStmt(ExprStmt*) { return true; }
  virtual void visitAfterExprStmt(ExprStmt*) {}

  virtual void visitBeforeIfStmt(IfStmt*) {}
  virtual bool visitIfStmt(IfStmt*) { return true; }
  virtual void visitAfterIfStmt(IfStmt*) {}

  virtual void visitBeforeForStmt(ForStmt*) {}
  virtual bool visitForStmt(ForStmt*) { return true; }
  virtual void visitAfterForStmt(ForStmt*) {}

  virtual void visitBeforeWhileStmt(WhileStmt*) {}
  virtual bool visitWhileStmt(WhileStmt*) { return true; }
  virtual void visitAfterWhileStmt(WhileStmt*) {}

  virtual void visitBeforeUnlessStmt(UnlessStmt*) {}
  virtual bool visitUnlessStmt(UnlessStmt*) { return true; }
  virtual void visitAfterUnlessStmt(UnlessStmt*) {}

  virtual void visitBeforeBreakStmt(BreakStmt*) {}
  virtual bool visitBreakStmt(BreakStmt*) { return true; }
  virtual void visitAfterBreakStmt(BreakStmt*) {}

  virtual void visitBeforeContinueStmt(ContinueStmt*) {}
  virtual bool visitContinueStmt(ContinueStmt*) { return true; }
  virtual void visitAfterContinueStmt(ContinueStmt*) {}

  virtual void visitBeforeReturnStmt(ReturnStmt*) {}
  virtual bool visitReturnStmt(ReturnStmt*) { return true; }
  virtual void visitAfterReturnStmt(ReturnStmt*) {}

  virtual void visitBeforeLoopStmt(LoopStmt*) {}
  virtual bool visitLoopStmt(LoopStmt*) { return true; }
  virtual void visitAfterLoopStmt(LoopStmt*) {}
  
  virtual void visitBeforeVarExpr(VarExpr*) {}
  virtual bool visitVarExpr(VarExpr*) { return true; }
  virtual void visitAfterVarExpr(VarExpr*) {}

  virtual void visitBeforeIntegerLitExpr(IntegerLitExpr*) {}
  virtual bool visitIntegerLitExpr(IntegerLitExpr*) { return true; }
  virtual void visitAfterIntegerLitExpr(IntegerLitExpr*) {}

  virtual void visitBeforeBooleanLitExpr(BooleanLitExpr*) {}
  virtual bool visitBooleanLitExpr(BooleanLitExpr*) { return true; }
  virtual void visitAfterBooleanLitExpr(BooleanLitExpr*) {}

  virtual void visitBeforeFloatLitExpr(FloatLitExpr*) {}
  virtual bool visitFloatLitExpr(FloatLitExpr*) { return true; }
  virtual void visitAfterFloatLitExpr(FloatLitExpr*) {}

  virtual void visitBeforeStringLitExpr(StringLitExpr*) {}
  virtual bool visitStringLitExpr(StringLitExpr*) { return true; }
  virtual void visitAfterStringLitExpr(StringLitExpr*) {}

  virtual void visitBeforeBinOpExpr(BinOpExpr*) {}
  virtual bool visitBinOpExpr(BinOpExpr*) { return true; }
  virtual void visitAfterBinOpExpr(BinOpExpr*) {}

  virtual void visitBeforeUnaryOpExpr(UnaryOpExpr*) {}
  virtual bool visitUnaryOpExpr(UnaryOpExpr*) { return true; }
  virtual void visitAfterUnaryOpExpr(UnaryOpExpr*) {}

  virtual void visitBeforeCastExpr(CastExpr*) {}
  virtual bool visitCastExpr(CastExpr*) { return true; }
  virtual void visitAfterCastExpr(CastExpr*) {}

  virtual void visitBeforeCallExpr(CallExpr*) {}
  virtual bool visitCallExpr(CallExpr*) { return true; }
  virtual void visitAfterCallExpr(CallExpr*) {}

};


// utils
inline Expr*
buildAssignOp(ops::BinaryOp op, Expr* lhs, Expr* rhs)
{
  return new BinOpExpr(
    ops::Assign,
    lhs,
    new BinOpExpr(
      op,
      lhs,
      rhs
    )
  );
}


inline Stmt*
buildPostfixCondStmt(Expr* cond, Stmt* stmt)
{
  BlockStmt* block = new BlockStmt();
  block->add(stmt);
  return new IfStmt(cond, block);
}


inline Expr*
buildNot(Expr* expr)
{
  return new UnaryOpExpr(ops::Not, expr);
}

#endif /* AST_H */
