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

    // 检查指定礼包是否存在
    Boolean CheckHavePackage(Int32 xPackageId);

    // 打开奖励列表
    Boolean OpenReward_PackageList(MsDictionary<Int32, Int32>& xDictReward_Package);

    // 打开指定礼包
    Boolean OpenPackage(Int32 xPackageId, MsDictionary<Int32, Int32>& xDictReward_Package);

    // 将奖励列表转换成单元(道具,装备,经验,金币,钻石)列表
    Boolean RewardList2UnitList(MsDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    // 根据背包道具填充Tips
    Boolean FillItemTipsByItem(item* xItem);

protected:

    // 根据随机结果得到奖励索引
    Int32 GetRewardKeyByRandProbability(Int64 xRand, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward);

    // 计算最大权重值
    Int64 CalculateMaxWeight(MsList<WEIGHT_RAND_ASSIST>& xListWeightReward);

    // 获取指定礼包权重奖励内容
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
    DEF_CONFIG_LUA_FIELD(born);             // 出生
    DEF_CONFIG_LUA_FIELD(role_level);       // 等级
    DEF_CONFIG_LUA_FIELD(scene);            // 场景
    DEF_CONFIG_LUA_FIELD(item);             // 道具
    DEF_CONFIG_LUA_FIELD(suit);             // 套装
    DEF_CONFIG_LUA_FIELD(equip);            // 装备
    DEF_CONFIG_LUA_FIELD(suit_shop);        // 套装商店
    DEF_CONFIG_LUA_FIELD(face_shop);        // 表情商店
    DEF_CONFIG_LUA_FIELD(face_discount);    // 打折表情
    DEF_CONFIG_LUA_FIELD(trans_info);       // 传送信息
    DEF_CONFIG_LUA_FIELD(transport);        // 传送点
    DEF_CONFIG_LUA_FIELD(npc);              // NPC
    DEF_CONFIG_LUA_FIELD(battle_wave);      // 波次
    DEF_CONFIG_LUA_FIELD(monster);          // 怪物
    DEF_CONFIG_LUA_FIELD(summons);          // 召唤兽
    DEF_CONFIG_LUA_FIELD(random_box);       // 随机宝箱
    DEF_CONFIG_LUA_FIELD(package);          // 礼包信息
    DEF_CONFIG_LUA_FIELD(reward);           // 奖励信息
};
