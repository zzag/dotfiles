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

int main (int argc, char **argv) {


  for(int i = 1; i < argc; i++) {
    const char *filename = argv[i];
    lua_State *L = lua_open();
    luaL_openlibs(L);

    if(luaL_loadfile(L, filename)) {
      cout << "Can\'t load: " << filename << endl;
      continue;
    }

    if(lua_pcall(L, 0, LUA_MULTRET, 0)) {
      cout << "Error: " << lua_tostring(L, -1) << endl;
      lua_pop(L, 1);
    }

    // lua_newtable
    lua_newtable(L);
    cout << "Stack size(lua_newtable) = " << lua_gettop(L) << endl;

    // set fields
    {
      // abc = 123
      const char *key = "abc";
      const int value = 1231;
      lua_pushstring(L, key);
      lua_pushnumber(L, value);
      lua_settable(L, -3);

      cout << "Stack size(set abc) = " << lua_gettop(L) << endl;
    }

    {
      // stuff = "test string"
      const char *key = "stuff";
      const char *value = "test string";
      lua_pushstring(L, key);
      lua_pushstring(L, value);
      lua_settable(L, -3);
    }

    // lua_setglobal
    lua_setglobal(L, "test");
    cout << "Stack size(lua_setglobal) = " << lua_gettop(L) << endl;
    cout << "--- Finish ---" << endl;

    lua_getglobal(L, "main");
    if (lua_pcall(L, 0, 0, 0)) {
      cout << "Error: " << lua_tostring(L, -1) << endl;
      lua_pop(L, 1);
    }

    lua_close(L);
  }

  return 0;
}
