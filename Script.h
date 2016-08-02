// Обработчик скрипта
extern "C"
{
	#include <lua.h>
}

lua_State* luaVM;



void InitScriptSystem(HWND hWnd)
{
	luaVM = lua_open();
	
	if (NULL == luaVM)
		MessageBox(hWnd, "Failed to create Lua VM.", "Error", MB_OK);
	else
	{
		lua_register(luaVM, "PlayerDeath", s_PlayerDeath);
	}
}




