//#pragma once
//EXTERN_C
//{
//#include "lua.h"
//};
//
//class MsLuaManager
//{
//public:
//    MsLuaManager();
//    ~MsLuaManager();
//
//public:
//    Boolean DoFile(mstr xLuaFile);
//    int GetParamCount(void);
//
//    void Push();
//    void Push(const char* param);
//    void Push(const mstr& param);
//    void Push(short param);
//    void Push(int param);
//    void Push(unsigned int param);
//    void Push(float param);
//    void Push(double param);
//    void Push(bool param);
//    void Push(lua_CFunction param);
//
//    void Pop(int count = 1);
//
//    void getValue(short& v, int index = -1);
//    void getValue(int& v, int index = -1);
//    void getValue(double& v, int index = -1);
//    void getValue(char*& v, int index = -1);
//    void getValue(mstr& v, int index = -1);
//    void getValue(bool& v, int index = -1);
//    void getValue(void*& v, int index = -1);
//
//    int Lua_CallGlobalFunc(char* func, int a, int b)
//    {
//        lua_getglobal(m_lua_State, func);
//        Push(a);
//        Push(b);
//        return Lua_Call<int>(2);
//    };
//
//    template<typename TResults>
//    TResults getValueNoPop(int index = -1)
//    {
//        //LOKI_STATIC_CHECK(TypeTraits<U>::isReference==false,must_use_a_value)
//        //typedef typename TypeTraits<U>::NonConstType ReturnType;
//
//        TResults r;
//        getValue(r, index);
//        return r;
//    }
//
//    template<typename TResults>
//    TResults getValue(int index = -1)
//    {
//        //LOKI_STATIC_CHECK(TypeTraits<U>::isReference==false,must_use_a_value)
//        //typedef typename TypeTraits<U>::NonConstType ReturnType;
//
//        TResults r;
//        getValue(r, index);
//        Pop();
//        return r;
//    }
//
//    template<class TResults>
//    TResults Lua_Call(int param_num)
//    {
//        if (lua_pcall(m_lua_State, param_num, 1, 0) != 0)
//        {
//            char* err = getValueNoPop<char*>();
//            USE_PARAM(err);
//            Pop();
//            return TResults();
//        }
//        return getValue<TResults>();
//    };
//
//private:
//    lua_State* m_lua_State;
//};
