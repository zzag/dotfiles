#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <list>
#include <string>


template<typename Key, typename Value>
class Scope
{
public:
  Scope() {}
  ~Scope() {}

  bool exists(Key name)
  {
    auto i = symbols.find(name);
    return i != symbols.end();
  }

  Value& get(Key name)
  {
    return symbols[name];
  }

  void put(Key name, Value value)
  {
    symbols[name] = value;
  }

private:
  std::map<Key, Value> symbols;
};


template<typename Key, typename Value>
class SymbolTable
{
public:
  SymbolTable() {}
  ~SymbolTable() {}

  void pushBlock() { scopes.push_front(Scope<Key, Value>()); }
  void popBlock() { scopes.pop_front(); }
  bool empty() { return scopes.size() == 0; }
  Scope<Key, Value>& currentBlock() { return scopes.front(); }

  bool exists(Key key)
  {
    for (auto scope : scopes)
      if (scope.exists(key)) return true;
    return false;
  }
  
  Value& get(Key key)
  {
    for (auto scope : scopes)
      if (scope.exists(key)) return scope.get(key);
    throw std::string("invalid key");
  }

  void put(Key key, Value value)
  {
    for (auto scope : scopes)
    {
      if (! scope.exists(key)) continue;
      scope.put(key, value);
      break;
    }
  }

  void declare(Key key, Value value)
  {
    currentBlock().put(key, value);
  }

private:
  std::list<Scope<Key, Value>> scopes;
};

#endif /* SYMBOL_TABLE_H */
