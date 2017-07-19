#pragma once

#define DEF_CONFIG_LUA_FIELD(field_name) MsDictionary<Int64, lua_##field_name*> m_Dict_##field_name; \
void LoadFromLua_##field_name() { LoadAllLuaTableListData<lua_##field_name>(m_Dict_##field_name, "ms", ""#field_name); }\
lua_##field_name* Get##field_name(INT_PTR xKey)\
{\
    if (m_Dict_##field_name.ContainsKey(xKey))\
    {\
        return m_Dict_##field_name[xKey];\
    }\
    return nullptr;\
}\
void WriteToDB_##field_name(LPCSTR xRegistryFieldName) \
{\
    MsMysql xMsMysql(xRegistryFieldName);\
    xMsMysql.Connect();\
    MysqlAutoRes xRes;\
    mstr xError;\
    xMsMysql.ExeSQL(xError, xRes, "DELETE FROM `config` WHERE `name` = '"#field_name##"';"); \
    auto& xTempDict = this->m_Dict_##field_name;\
    BEGIN_ENUMERATOR(xTempDict)\
    {\
        mstr xData; MsBaseSql::P2BS(*xEnumValue, xData);\
        xMsMysql.ExeSQLFormat(xError, xRes, " INSERT INTO `config` (`id`, `name`, `value`) VALUES (%lld, '%s', %s);", xEnumKey, ""#field_name, xData.c_str());\
    }END_ENUMERATOR;\
    xMsMysql.Disconnect();\
}\
void LoadFromDB_##field_name(LPCSTR xRegistryFieldName) \
{\
    MsMysql xMsMysql(xRegistryFieldName);\
    xMsMysql.Connect();\
    MysqlAutoRes xRes;\
    mstr xError;\
    xMsMysql.ExeSQL(xError, xRes, "SELECT * FROM `config` WHERE `name` = '"#field_name##"';");\
    if (xRes.m_Res)\
    {\
        MysqlAutoRow xRow = xRes.GetResultRowData();\
        while (xRow)\
        {\
            DWORD xSqlFieldLen = 0;\
            LPBYTE xSqlFieldData = xRow.GetBytes(xRes.m_Res, 2, xSqlFieldLen);\
            lua_##field_name* luaT = NEW lua_##field_name();\
            luaT->ParseFromArray(xSqlFieldData, xSqlFieldLen);\
            this->m_Dict_##field_name.Add(xRow.GetInt32(0), luaT);\
            xRow = xRes.GetResultRowData();\
        }\
    }\
    xMsMysql.Disconnect();\
}

struct WEIGHT_RAND_ASSIST
{
    Int32  m_Weight;
    Boolean m_Invalid;
    Int32  m_Value;
};

struct ITEM_UNIT
{
    E_GOODS_TYPE m_Type;
    Int32  m_UnitId;
    Int32  m_UnitCount;
};

class MsConfig : public MsLuaInterface
{
public:
    ~MsConfig();

    void InitFromDB(LPCSTR xDBConfigName);

    void InitMemoryConfig();

    Boolean InitFromLua(mstr xResPath, mstr xMainLuaPath);

    // ���ָ������Ƿ����
    Boolean CheckHavePackage(Int32 xPackageId);

    // �򿪽����б�
    Boolean OpenReward_PackageList(MsDictionary<Int32, Int32>& xDictReward_Package);

    // ��ָ�����
    Boolean OpenPackage(Int32 xPackageId, MsDictionary<Int32, Int32>& xDictReward_Package);

    // �������б�ת���ɵ�Ԫ(����,װ��,����,���,��ʯ)�б�
    Boolean RewardList2UnitList(MsDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    // ���ݱ����������Tips
    Boolean FillItemTipsByItem(item* xItem);

protected:

    // �����������õ���������
    Int32 GetRewardKeyByRandProbability(Int64 xRand, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward);

    // �������Ȩ��ֵ
    Int64 CalculateMaxWeight(MsList<WEIGHT_RAND_ASSIST>& xListWeightReward);

    // ��ȡָ�����Ȩ�ؽ�������
    Boolean GetPackageWeightRewardList(lua_package* xPackage, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward);

public:
    void WriteToDB_SC(LPCSTR xConfigName)
    {
        //MsMysql xMsMysql(xConfigName);
        //xMsMysql.Connect();
        //MysqlAutoRes xRes;
        //mstr xError;
        //xMsMysql.ExeSQL(xError, xRes, "DELETE FROM `config` WHERE `name` = 'scene';");
        //auto& xTempDict = this->m_Dict_scene;
        //BEGIN_ENUMERATOR(xTempDict)
        //{
        //    mstr xData; MsBaseSql::P2BS(*xEnumValue, xData);
        //    xMsMysql.ExeSQLFormat(xError, xRes, " INSERT INTO `config` (`id`, `name`, `value`) VALUES (%lld, '%s', %s);", xEnumKey, "scene", xData.c_str());
        //}END_ENUMERATOR;
        //xMsMysql.Disconnect();
    }

    void LoadFromDB_SC(LPCSTR xConfigName)
    {
        //MsMysql xMsMysql(xConfigName);
        //xMsMysql.Connect();
        //MysqlAutoRes xRes;
        //mstr xError;
        //xMsMysql.ExeSQL(xError, xRes, "SELECT * FROM `config` WHERE `name` = 'scene';");
        //if (xRes.m_Res)
        //{
        //    MysqlAutoRow xRow = xRes.GetResultRowData();
        //    while (xRow)
        //    {
        //        DWORD xSqlFieldLen = 0;
        //        LPBYTE xSqlFieldData = xRow.GetBytes(xRes.m_Res, 2, xSqlFieldLen);
        //        lua_scene* luaT = NEW lua_scene();
        //        luaT->ParseFromArray(xSqlFieldData, xSqlFieldLen);
        //        this->m_Dict_scene.Add(xRow.GetInt32(0), luaT);
        //        xRow = xRes.GetResultRowData();
        //    }
        //}
        //xMsMysql.Disconnect();
    }
public:
    DEF_CONFIG_LUA_FIELD(born);             // ����
    DEF_CONFIG_LUA_FIELD(role_level);       // �ȼ�
    DEF_CONFIG_LUA_FIELD(scene);            // ����
    DEF_CONFIG_LUA_FIELD(item);             // ����
    DEF_CONFIG_LUA_FIELD(suit);             // ��װ
    DEF_CONFIG_LUA_FIELD(equip);            // װ��
    DEF_CONFIG_LUA_FIELD(suit_shop);        // ��װ�̵�
    DEF_CONFIG_LUA_FIELD(face_shop);        // �����̵�
    DEF_CONFIG_LUA_FIELD(face_discount);    // ���۱���
    DEF_CONFIG_LUA_FIELD(trans_info);       // ������Ϣ
    DEF_CONFIG_LUA_FIELD(transport);        // ���͵�
    DEF_CONFIG_LUA_FIELD(npc);              // NPC
    DEF_CONFIG_LUA_FIELD(battle_wave);      // ����
    DEF_CONFIG_LUA_FIELD(monster);          // ����
    DEF_CONFIG_LUA_FIELD(summons);          // �ٻ���
    DEF_CONFIG_LUA_FIELD(random_box);       // �������
    DEF_CONFIG_LUA_FIELD(package);          // �����Ϣ
    DEF_CONFIG_LUA_FIELD(reward);           // ������Ϣ
};
