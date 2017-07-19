extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
    //#include <The3rd/luafilesystem/src/lfs.h>
}

#include <tolua++.h>
#include <string>
using namespace std;

#define FILENAMEKEY  "LOADEDFILENAME"

#define MAX_SCRIPT_HOLD 4096

using namespace google::protobuf;
typedef FieldDescriptor::Type ProtoType;

typedef lua_State Lua_State;
#define Lua_ValueToNumber lua_tointeger
#define Lua_ValueToString lua_tostring
#define Lua_GetTopIndex  lua_gettop
#define Lua_PushNumber lua_pushnumber
#define Lua_PushString lua_pushstring
#define Lua_IsNumber lua_isnumber

#ifdef GetMessage
#undef GetMessage
#endif


enum E_LUA_TYPE
{
    ELT_TNIL = LUA_TNIL,
    ELT_TBOOLEAN = LUA_TBOOLEAN,
    ELT_TLIGHTUSERDATA = LUA_TLIGHTUSERDATA,
    ELT_TNUMBER = LUA_TNUMBER,
    ELT_TSTRING = LUA_TSTRING,
    ELT_TTABLE = LUA_TTABLE,
    ELT_TFUNCTION = LUA_TFUNCTION,
    ELT_TUSERDATA = LUA_TUSERDATA,
    ELT_TTHREAD = LUA_TTHREAD,
};

//template<typename T>
//struct LuaTable
//{
//    ~LuaTable() { SAFE_ENUMERATOR_DELETE(m_Dict_LuaTable); }
//    //MsDictionary<mstr, Int64>   m_Dict_KeyIndex;
//    MsDictionary<Int64, T*>     m_Dict_LuaTable;
//    T* operator[](Int64 xKey)
//    {
//        return m_Dict_LuaTable[xKey];
//    }
//};

class MsLuaInterface
{
public:
    MsLuaInterface(void);
    ~MsLuaInterface(void) { lua_close(lua_state_); }

    int GetParamCount(void) { return lua_gettop(lua_state_); }

    template<typename R>
    R GetGlobal(const char* name) { lua_getglobal(lua_state_, name); return getValue<R>(); }

    template<typename T>
    void SetGlobal(const char* name, T value) { Push(value); lua_setglobal(lua_state_, name); }

    template<typename R>
    R getGlobalVarField(const char* var_name, const char* field_name) { lua_getglobal(lua_state_, var_name); Push(field_name); lua_gettable(lua_state_, -2); return getValue<R>(); }

    template<typename T>
    void setGlobalVarField(const char* var_name, const char* field_name, T value) { lua_getglobal(lua_state_, var_name); Push(field_name); Push(value); lua_settable(lua_state_, -3); return; }

    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(û�в����汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE>
    R callObjFunc(OBJTYPE obj, const char* func)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);

        return _Call<R>(1);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(һ�������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @return ��������ֵ(����з���ֵ)
    *    @note ����ֵ���ͱ���Ϊ���¼���֮һ�����߿����Զ�ת��Ϊ��Щ����:
    *    int,double,bool,char*,void*��
    *    ���������Ҫ����һ���Զ��������ô���뷵������ָ�룬��ָ������ֵ����Ϊvoid*��Ȼ����ǿ��ת��Ϊ�Զ������͡���:
    *    @code Npc* npc0 = (Npc*)script.callObjFunc<void*>(npc_mng,"createNPC"); @endcode
    *    ����˺���û�з���ֵ������ֵ���ͱ�����Ϊ void��
    *    Ŀǰֻ֧�����һ������ֵ��
    *    �����Ҫ�����Զ��������Ϊ��������ô���봫�����ָ�룬������ֵ�������á�
    *    ��Ϊ��������Ķ�����뾭������ addWrapperToCObj() ����
    *    @see addWrapperToCObj(T cobj)
    */
    template<typename R, typename OBJTYPE, typename P1>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);

        return _Call<R>(2);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(���������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE, typename P1, typename P2>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1, P2 p2)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);
        Push(p2);

        return _Call<R>(3);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(���������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE, typename P1, typename P2, typename P3>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1, P2 p2, P3 p3)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);
        Push(p2);
        Push(p3);

        return _Call<R>(4);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(�ĸ������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE, typename P1, typename P2, typename P3, typename P4>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);

        return _Call<R>(5);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(��������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @param p5 ����5
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE, typename P1, typename P2, typename P3, typename P4, typename P5>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)

        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);

        return _Call<R>(6);
    }
    /**
    *    ����ĳ�������ڽű��ж���ĳ�Ա����(���������汾)��
    *    @param obj ����ָ�롣������ָ�룬������ֵ��������
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @param p5 ����5
    *    @param p6 ����6
    *    @return ��������ֵ(����з���ֵ)
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename R, typename OBJTYPE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    R callObjFunc(OBJTYPE obj, const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        Push(obj);
        lua_pushstring(lua_state_, func);
        lua_gettable(lua_state_, -2);
        lua_insert(lua_state_, -2);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);

        return _Call<R>(7);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(û�в����汾)��
    *    @param func ��Ա����������
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R>
    R callGlobalFunc(const char* func)
    {
        lua_getglobal(lua_state_, func);

        return _Call<R>(0);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(һ�������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @return ��������ֵ(����з���ֵ)
    *    @note ����ֵ���ͱ���Ϊ���¼���֮һ�����߿����Զ�ת��Ϊ��Щ����:
    *    int,double,bool,char*,void*��
    *    ���������Ҫ����һ���Զ��������ô���뷵������ָ�룬��ָ������ֵ����Ϊvoid*��Ȼ����ǿ��ת��Ϊ�Զ������͡���:
    *    @code Npc* npc0 = (Npc*)script.callGlobalFunc<void*>("createNPC"); @endcode
    *    ����˺���û�з���ֵ������ֵ���ͱ�����Ϊ void��
    *    Ŀǰֻ֧�����һ������ֵ��
    *    �����Ҫ�����Զ��������Ϊ��������ô���봫�����ָ�룬������ֵ�������á�
    *    ��Ϊ��������Ķ�����뾭������ addWrapperToCObj() ����
    *    @see addWrapperToCObj(T cobj)
    */
    template<typename R, typename P1>
    R callGlobalFunc(const char* func, P1 p1)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);

        return _Call<R>(1);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(���������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R, typename P1, typename P2>
    R callGlobalFunc(const char* func, P1 p1, P2 p2)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);

        return _Call<R>(2);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(���������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R, typename P1, typename P2, typename P3>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);

        return _Call<R>(3);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(�ĸ������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R, typename P1, typename P2, typename P3, typename P4>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);

        return _Call<R>(4);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(��������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @param p5 ����5
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);

        return _Call<R>(5);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(���������汾)��
    *    @param func ��Ա����������
    *    @param p1 ����1
    *    @param p2 ����2
    *    @param p3 ����3
    *    @param p4 ����4
    *    @param p5 ����5
    *    @param p6 ����6
    *    @return ��������ֵ(����з���ֵ)
    *    @see callGlobalFunc(const char* func,P1 p1)
    */
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);

        return _Call<R>(6);
    }
    /**
    *    ���ýű��ж����ȫ�ֺ���(�߸������汾)��
    */
    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)
        //LuaInterfaceLocker __lock(mOwner);

        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);
        Push(p7);

        return _Call<R>(7);
    }

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)
        //LuaInterfaceLocker __lock(mOwner);

        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);
        Push(p7);
        Push(p8);

        return _Call<R>(8);
    }

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)
        //LuaInterfaceLocker __lock(mOwner);

        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);
        Push(p7);
        Push(p8);
        Push(p9);

        return _Call<R>(9);
    }

    template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
    R callGlobalFunc(const char* func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
    {
        //LOKI_STATIC_CHECK(TypeTraits<R>::isReference==false,must_use_a_value)
        //LuaInterfaceLocker __lock(mOwner);

        lua_getglobal(lua_state_, func);
        Push(p1);
        Push(p2);
        Push(p3);
        Push(p4);
        Push(p5);
        Push(p6);
        Push(p7);
        Push(p8);
        Push(p9);
        Push(p10);

        return _Call<R>(10);
    }

    /**
    *    ��ӡ�������Ķ�ջ
    */
    void DumpStack()
    {
        int i;
        int top = lua_gettop(lua_state_);
        for (i = 1; i <= top; i++)    //  repeat for each level 
        {
            int t = lua_type(lua_state_, i);
            switch (t)
            {
                case LUA_TSTRING:  // strings 
                    printf("script_dumpstack  %s\n", lua_tostring(lua_state_, i));
                    break;

                case LUA_TBOOLEAN:  // booleans 
                    printf("script_dumpstack  %s\n", lua_toboolean(lua_state_, i) ? "true" : "false");
                    break;

                case LUA_TNUMBER:  // numbers 
                    printf("script_dumpstack  %g\n", lua_tonumber(lua_state_, i));
                    break;

                default:  // other values 
                    printf("script_dumpstack  %s\n", lua_typename(lua_state_, t));
                    break;
            }
        }
    }



    //template<typename T>
    //void ParseLuaTableKey(MsDictionary<Int64, T*>& xDictTable, LPSTR lua_type_name, LPSTR lua_base_name)
    //{
    //    lua_getglobal(lua_state_, lua_base_name);
    //    T xProto;
    //    lua_getfield(lua_state_, -1, lua_type_name);
    //    lua_getfield(lua_state_, -1, "key");

    //    //const Reflection* xReflection = xProto.GetReflection();
    //    const Descriptor* xDescriptor = xProto.GetDescriptor();
    //    Int32 xFieldCount = xDescriptor->field_count();
    //    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    //    {
    //        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
    //        mstr xName = xField->name();
    //        lua_getfield(lua_state_, -1, xField->name().c_str());
    //        xLuaTable->m_Dict_KeyIndex.Add(xField->name(), lua_tointeger(lua_state_, -1));
    //        lua_pop(lua_state_, 1);
    //    }
    //    lua_settop(lua_state_, 0);
    //}

    template<typename T>
    void LoadAllLuaTableListData(MsDictionary<Int64, T*>& xDictTable, LPSTR lua_base_name, LPSTR lua_type_name)
    {
        lua_getglobal(lua_state_, lua_base_name);
        lua_getfield(lua_state_, -1, lua_type_name);
        lua_getfield(lua_state_, -1, "list");

        Int32 xIndex = lua_gettop(lua_state_);
        lua_pushnil(lua_state_);
        while (lua_next(lua_state_, xIndex) != 0)
        {
            Int64 xKey = lua_tointeger(lua_state_, -2);
            T* xProto = NEW T();
            LoadParseLuaTableListData(xProto);
            xDictTable.Add(xKey, xProto);
            lua_pop(lua_state_, 1);
        }
        lua_pop(lua_state_, xIndex);
    }
    template<typename T>
    void LoadParseLuaTableListData(MsDictionary<Int64, T*>& xDictTable, LPSTR lua_type_name, LPSTR lua_base_name, INT_PTR lua_index_key)
    {
        lua_getglobal(lua_state_, lua_base_name);
        T* xProto = NEW T();
        lua_getfield(lua_state_, -1, lua_type_name);
        lua_getfield(lua_state_, -1, "list");
        lua_pushinteger(lua_state_, lua_index_key);
        lua_gettable(lua_state_, -2);
        LoadParseLuaTableListData(xProto);
        xDictTable.Add(lua_index_key, xProto);
    }
    template<typename T>
    void LoadParseLuaTableListData(T* xProto)
    {
        const Reflection* xReflection = xProto->GetReflection();
        const Descriptor* xDescriptor = xProto->GetDescriptor();
        Int32 xFieldCount = xDescriptor->field_count();
        for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
        {
            const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
            mstr xName = xField->name();
            lua_rawgeti(lua_state_, -1, xFieldIndex + 1);

            switch (xField->type())
            {
                case ProtoType::TYPE_FLOAT:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddFloat(xProto, xField, (float)lua_tonumber(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetFloat(xProto, xField, (float)lua_tonumber(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_DOUBLE:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddDouble(xProto, xField, lua_tonumber(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetDouble(xProto, xField, lua_tonumber(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_INT64:
                case ProtoType::TYPE_SINT64:
                case ProtoType::TYPE_FIXED64:
                case ProtoType::TYPE_SFIXED64:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddInt64(xProto, xField, lua_tointeger(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetInt64(xProto, xField, lua_tointeger(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_UINT64:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_INT32:
                case ProtoType::TYPE_SINT32:
                case ProtoType::TYPE_FIXED32:
                case ProtoType::TYPE_SFIXED32:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_UINT32:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_ENUM:
                {
                    AssertNormal(False, "lua_table_config not supported enum");
                }break;

                case ProtoType::TYPE_BOOL:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
                    }
                }break;

                case ProtoType::TYPE_STRING:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_BYTES:
                {
                    //AssertNormal(False, "lua_table_config not supported bytes");
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            lua_rawgeti(lua_state_, -1, i);
                            xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
                    }
                }break;

                case ProtoType::TYPE_MESSAGE:
                {
                    if (xField->is_repeated())
                    {
                        int xLen = (int)lua_objlen(lua_state_, -1);
                        for (int i = 1; i <= xLen; i++)
                        {
                            Message* xMsg = xReflection->AddMessage(xProto, xField);
                            lua_rawgeti(lua_state_, -1, i);
                            this->LoadParseLuaTableListData(xMsg);
                            lua_pop(lua_state_, 1);
                        }
                    }
                    else
                    {
                        Message* xMsg = xReflection->MutableMessage(xProto, xField);
                        this->LoadParseLuaTableListData(xMsg);
                    }
                }break;
            }
            lua_pop(lua_state_, 1);
        }
    }

public:

    void AddPath(mstr xPath);

    /**
    *    װ�벢ִ��ĳ���ű��ļ�
    *    @param fname �ļ���
    *    @return ��������ִ�н��
    */
    int DoFile(const char* fname)
    {
        return luaL_dofile(lua_state_, fname);
    }
    /**
    *    ��ĳ��C++���ɵĶ�����һ�£�ʹ֮���Ա��ű����ʡ��˶�������ͱ������Ѿ�ӳ�䵽�ű�������͡�
    *    @param cobj ����ָ�롣������ָ�룬������ֵ��������
    *    @see callObjFunc(OBJTYPE obj,const char* func,P1 p1)
    */
    template<typename T>
    void addWrapperToCObj(T cobj)
    {
        //LOKI_STATIC_CHECK(TypeTraits<T>::isPointer,must_use_a_pointer)

        addWrapperToCObj(cobj, cobj->className());
    }

    void clearStack() { lua_settop(lua_state_, 0); }




    void Push()
    {
        lua_pushnil(lua_state_);
    }
    void Push(const char* param)
    {
        lua_pushstring(lua_state_, param);
    }
    void Push(const string& param)
    {
        lua_pushlstring(lua_state_, param.c_str(), param.size());
    }
    void Push(short param)
    {
        lua_pushnumber(lua_state_, param);
    }
    void Push(int param)
    {
        lua_pushnumber(lua_state_, param);
    }
    void Push(unsigned int param)
    {
        lua_pushnumber(lua_state_, param);
    }
    void Push(float param)
    {
        lua_pushnumber(lua_state_, param);
    }
    void Push(double param)
    {
        lua_pushnumber(lua_state_, param);
    }
    void Push(bool param)
    {
        lua_pushboolean(lua_state_, param);
    }
    void Push(lua_CFunction  param)
    {
        lua_pushcfunction(lua_state_, param);
    }

    template<typename U>
    void Push(U param)
    {
        //LOKI_STATIC_CHECK(TypeTraits<U>::isPointer,must_use_a_pointer)
        //typedef typename TypeTraits<U>::PointeeType ParamType;

        if (!param) return;
        Cobj2LuaObjStack(param, param->className());
    }

protected:
    void Cobj2LuaObjStack(void* cobj, const char* className)
    {
        tolua_pushusertype(lua_state_, (void*)cobj, className);
    }
    void Pop(int count = 1)
    {
        lua_pop(lua_state_, count);
    }
    void getValue(short& v, int index = -1)
    {
        v = (short)tolua_tonumber(lua_state_, index, 0);
    }
    void getValue(int& v, int index = -1)
    {
        v = (int)tolua_tonumber(lua_state_, index, 0);
    }

    void getValue(double& v, int index = -1)
    {
        v = tolua_tonumber(lua_state_, index, 0);
    }

    void getValue(char*& v, int index = -1)
    {
        v = (char*)tolua_tostring(lua_state_, index, 0);
    }

    void getValue(string& v, int index = -1)
    {
        v = tolua_tostring(lua_state_, index, 0);
    }

    void getValue(bool& v, int index = -1)
    {
        v = tolua_toboolean(lua_state_, index, 0) ? true : false;
    }
    void getValue(void*& v, int index = -1)
    {
        v = tolua_tousertype(lua_state_, index, 0);
    }
    //void getValue(long long*& v,int index=-1)
    //{
    //    v = tolua_tousertype(lua_state_,index,0);
    //}

public:
    template<typename U>
    U getValue(int index = -1)
    {
        //LOKI_STATIC_CHECK(TypeTraits<U>::isReference==false,must_use_a_value)
        //typedef typename TypeTraits<U>::NonConstType ReturnType;

        U r;
        getValue(r, index);
        Pop();
        return r;
    }

    template<typename U>
    U getValueNoPop(int index = -1)
    {
        //LOKI_STATIC_CHECK(TypeTraits<U>::isReference==false,must_use_a_value)
        //typedef typename TypeTraits<U>::NonConstType ReturnType;

        U r;
        getValue(r, index);
        return r;
    }

protected:
    template<typename R>
    R _Call(int param_num)
    {
        if (lua_pcall(lua_state_, param_num, 1, 0) != 0)
        {
            char* err = getValueNoPop<char*>();
            printf("Lua error: %s\n", err);
            Pop();
            return R();
        }

        return getValue<R>();
    }


    void addWrapperToCObj(void* cobj, const char* class_name)
    {
        tolua_pushusertype(lua_state_, (void*)cobj, class_name);
        Pop();
    }

public:
    void    RegisterCFunction(void);
    int     ExeScriptInt(char* funcname);
    int     ExeScriptInt(char* funcname, int Param0);

    string ExeScriptStr(char* funcname);
    string ExeScriptStr(char* funcname, int Param0);

    lua_State* GetState() { return lua_state_; }

protected:
    lua_State* lua_state_;
};


//#ifndef WIN32
template<>
void MsLuaInterface::_Call<void>(int);
//#endif
