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

    lua_close(L);
  }

  return 0;
}
