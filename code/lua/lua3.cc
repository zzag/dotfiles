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

    lua_pushstring(L, "some text is here");
    lua_setglobal(L, "src");

    cout << "Stack size: " << lua_gettop(L) << endl;
    lua_getglobal(L, "test");
    cout << "Value: " << lua_tonumber(L, -1) << endl;
    cout << "Stack size: " << lua_gettop(L) << endl;
    lua_pop(L, 1);

    if(lua_pcall(L, 0, LUA_MULTRET, 0)) {
      cout << "Error: " << lua_tostring(L, -1) << endl;
      lua_pop(L, 1);
    }

    cout << "Stack size: " << lua_gettop(L) << endl;
    lua_getglobal(L, "test");
    cout << "Value: " << lua_tostring(L, -1) << endl;
    cout << "Stack size: " << lua_gettop(L) << endl;

    lua_getglobal(L, "abcd");
    cout << "abcd = " << lua_tostring(L, -1) << endl;
    lua_pop(L, 1);

    lua_close(L);
  }

  return 0;
}
