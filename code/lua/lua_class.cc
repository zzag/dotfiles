#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
}
#endif

using std::cout;
using std::endl;

class Foo {
public:
  Foo() {
    cout << "Constructor" << endl;
  }

  ~Foo() {
    cout << "Destructor" << endl;
  }

  void test() {
    cout << "Hello world!" << endl;
  }
};

int luaFooConstructor (lua_State *L) {

  cout << "Stack size(constructor) = " << lua_gettop(L) << endl;
  cout << "nil: " << lua_isnil(L, -1) << endl;
  Foo **udata = (Foo **)lua_newuserdata(L, sizeof(Foo *));
  *udata = new Foo;

  luaL_getmetatable(L, "luaFoo");
  lua_setmetatable(L, -2);

  cout << "Stack size = " << lua_gettop(L) << endl;

  return 1;
}

int luaFooDestructor(lua_State *L) {

  cout << "Stack size(destructor) = " << lua_gettop(L) << endl;
  Foo *foo = *(Foo **)luaL_checkudata(L, 1, "luaFoo");
  delete foo;

  return 0;
}

int luaFooTest(lua_State *L) {

  cout << "Stack size(test) = " << lua_gettop(L) << endl;
  Foo *foo = *(Foo **)luaL_checkudata(L, 1, "luaFoo");
  foo->test();

  return 0;
}

void registerFoo(lua_State *L) {

  luaL_Reg regs[] = {
    {"new", luaFooConstructor},
    {"test", luaFooTest},
    {"__gc", luaFooDestructor},
    {0, 0}
  };

  cout << "Stack size = " << lua_gettop(L) << endl;
  luaL_newmetatable(L, "luaFoo");
  cout << "Stack size(luaL_newmetatable) = " << lua_gettop(L) << endl;  

  luaL_register(L, 0, regs);
  cout << "Stack size(luaL_register) = " << lua_gettop(L) << endl;

  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
  cout << "Stack size(lua_setfield) = " << lua_gettop(L) << endl;
  lua_setglobal(L, "Foo");

  cout << "Stack size = " << lua_gettop(L) << endl;
}

int main (int argc, char **argv) {


  for(int i = 1; i < argc; i++) {
    const char *filename = argv[i];
    lua_State *L = lua_open();
    luaL_openlibs(L);

    if(luaL_loadfile(L, filename)) {
      cout << "Can\'t load: " << filename << endl;
      continue;
    }

    registerFoo(L);

    if(lua_pcall(L, 0, LUA_MULTRET, 0)) {
      cout << "Error: " << lua_tostring(L, -1) << endl;
      lua_pop(L, 1);
    }

    lua_getglobal(L, "main");
    if(lua_pcall(L, 0, 0, 0)) {
      cout << "Error: " << lua_tostring(L, -1) << endl;
      lua_pop(L, 1);
    }


    cout << "--- END EXEC ---" << endl;

    lua_close(L);

    cout << "--- END PROGRAM ---" << endl;
  }

  return 0;
}
