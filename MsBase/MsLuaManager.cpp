#include "Precompiled.h"
//#ifdef G
//#undef G
//#endif
//
//EXTERN_C
//{
//#include "lualib.h"
//#include "lauxlib.h"
//#include "lstate.h"
//};
//
//MsLuaManager::MsLuaManager()
//    :m_lua_State(luaL_newstate())
//{
//    luaL_openlibs(m_lua_State);
//    //luaL_openlibs(m_lua_State);
//    //tolua_open(m_lua_State);
//    //luaopen_lfs(m_lua_State);
//    //tolua_vxLua_open(m_lua_State);
//    lua_settop(m_lua_State, 0);
//}
//
//MsLuaManager::~MsLuaManager()
//{
//    lua_close(m_lua_State);
//}
//
//Boolean MsLuaManager::DoFile(mstr xLuaFile)
//{
//    //LPTSTR strANSIText = (LPTSTR)(LPVOID)System::Runtime::interopServices::Marshal::StringToHGlobalAnsi(strFileList[i]);
//    //return LUA_OK == luaL_loadfile(m_lua_State, xLuaFile.c_str());
//    //{
//    //    lua_getglobal(m_lua_State, "test_event");
//
//    //    lua_pushnumber(m_lua_State, 10);
//    //    lua_pushnumber(m_lua_State, 20);
//
//    //    int xResults = -1;
//    //    lua_call(m_lua_State, 2, xResults);
//    //    //L_FileList->Items->Add(strFileList[i], VALID_IMAGE_INDEX);
//    //    //m_strErrorCodeList[i] = "ÎÞÓï·¨´íÎó";
//    //}
//    //else
//    //{
//    //    //L_FileList->Items->Add(strFileList[i], FAILING_IMAGE_INDEX);
//    //    //m_strErrorCodeList[i] = gcnew System::String(lua_tostring(g_L, -1));
//    //    lua_pop(m_lua_State, 1);
//    //}
//    //return True;
//}
//
//int MsLuaManager::GetParamCount(void)
//{
//    return lua_gettop(m_lua_State);
//}
//
//void MsLuaManager::Push() { lua_pushnil(m_lua_State); }
//void MsLuaManager::Push(const char* param) { lua_pushstring(m_lua_State, param); }
//void MsLuaManager::Push(const mstr& param) { lua_pushlstring(m_lua_State, param.c_str(), param.size()); }
//void MsLuaManager::Push(short param) { lua_pushnumber(m_lua_State, param); }
//void MsLuaManager::Push(int param) { lua_pushnumber(m_lua_State, param); }
//void MsLuaManager::Push(unsigned int param) { lua_pushnumber(m_lua_State, param); }
//void MsLuaManager::Push(float param) { lua_pushnumber(m_lua_State, param); }
//void MsLuaManager::Push(double param) { lua_pushnumber(m_lua_State, param); }
//void MsLuaManager::Push(bool param) { lua_pushboolean(m_lua_State, param); }
//void MsLuaManager::Push(lua_CFunction  param) { lua_pushcfunction(m_lua_State, param); }
//
//void MsLuaManager::Pop(int count) { lua_pop(m_lua_State, count); }
//void MsLuaManager::getValue(short& v, int index) { v = (short)lua_tonumberx(m_lua_State, index, 0); }
//void MsLuaManager::getValue(int& v, int index) { v = (int)lua_tonumberx(m_lua_State, index, 0); }
//void MsLuaManager::getValue(double& v, int index) { v = lua_tonumberx(m_lua_State, index, 0); }
//void MsLuaManager::getValue(char*& v, int index) { v = (char*)lua_tolstring(m_lua_State, index, 0); }
//void MsLuaManager::getValue(mstr& v, int index) { v = lua_tolstring(m_lua_State, index, 0); }
//void MsLuaManager::getValue(bool& v, int index) { v = lua_toboolean(m_lua_State, index) ? true : false; }
//void MsLuaManager::getValue(void*& v, int index) { v = lua_touserdata(m_lua_State, index); }
//
