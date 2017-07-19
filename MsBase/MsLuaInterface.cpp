#include "Precompiled.h"
#include <iostream>

MsLuaInterface::MsLuaInterface(void)
    : lua_state_(0)
{
    lua_state_ = luaL_newstate();
    luaL_openlibs(lua_state_);
    tolua_open(lua_state_);
    lua_settop(lua_state_, 0);
}

template<>
void MsLuaInterface::_Call<void>(int param_num)
{
    if (lua_pcall(lua_state_, param_num, 0, 0) != 0)
    {
        char* err = getValueNoPop<char*>();
        printf("Lua error: %s\n", err);
        Pop();
    }

    return;
}

void MsLuaInterface::AddPath(mstr xPath)
{
    lua_getglobal(lua_state_, "package");
    lua_getfield(lua_state_, -1, "path");
    const char* cur_path = lua_tostring(lua_state_, -1);
    lua_pushfstring(lua_state_, "%s;%s\?.lua", cur_path, xPath.c_str());
    lua_setfield(lua_state_, -3, "path");
    lua_pop(lua_state_, 2);
}

bool popTable(lua_State* L, int idx)
{
    try {
        lua_pushnil(L);
        while (lua_next(L, idx) != 0) {
            int keyType = lua_type(L, -2);
            if (keyType == LUA_TNUMBER) {
                double value = lua_tonumber(L, -2);
                std::cout << "Key:" << value << endl;
            }
            else if (keyType == LUA_TSTRING) {
                const char*  value = lua_tostring(L, -2);
                std::cout << "Key:" << value << endl;
            }
            else {
                std::cout << "Invalid key type: " << keyType << endl;
                return false;
            }
            int valueType = lua_type(L, -1);
            switch (valueType) {
                case LUA_TNIL:
                {
                    std::cout << "Value: nil" << endl;
                    break;
                }
                case LUA_TBOOLEAN:
                {
                    int value = lua_toboolean(L, -1);
                    std::cout << value << endl;
                    break;
                }
                case LUA_TNUMBER:
                {    std::cout << "Value:" << lua_tonumber(L, -1) << endl;
                break;
                }
                case LUA_TSTRING:
                {
                    std::cout << "Value:" << lua_tostring(L, -1) << endl;
                    break;
                }
                case LUA_TTABLE:
                {
                    std::cout << "====sub table===" << endl;
                    int index = lua_gettop(L);
                    if (!popTable(L, index)) {
                        cout << "popTable error in  popTable,error occured" << endl;
                        return false;
                    }
                    break;
                }
                default:
                {
                    std::cout << "Invalid value type: " << valueType << endl;
                    return false;
                }
            }
            lua_pop(L, 1);
        }
    }
    catch (const char* s) {
        string errMsg = s;
        lua_pop(L, 1);
        std::cout << errMsg << endl;
        return false;
    }
    catch (std::exception& e) {
        const char* errMsg = e.what();
        lua_pop(L, 1);
        std::cout << errMsg << endl;
        return false;
    }
    catch (...) {
        const char* errMsg = lua_tostring(L, -1);
        lua_pop(L, 1);
        std::cout << errMsg << endl;
        return false;
    }
    return true;
}
