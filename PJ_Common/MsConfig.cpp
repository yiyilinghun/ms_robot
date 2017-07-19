#include "Precompiled.h"

#define MAX_REWARD_COUNT_IN_ONE_PACKAGE 100 // 一次开礼包最大奖励次数

// 检查指定礼包是否存在
Boolean MsConfig::CheckHavePackage(Int32 xPackageId)
{
    return m_Dict_package.ContainsKey(xPackageId);
}

// 增加指定个数物品到字典
Boolean AddItem(MsDictionary<Int32, Int32>& xDictReward, Int32 xItemId, Int32 xItemCount)
{
    if (xDictReward.ContainsKey(xItemId))
    {
        xDictReward[xItemId] += xItemCount;
    }
    else
    {
        xDictReward[xItemId] = xItemCount;
    }
    return True;
}

// 移除指定个数物品从字典
Boolean EraseItem(MsDictionary<Int32, Int32>& xDictReward, Int32 xItemId, Int32 xItemCount)
{
    if (xDictReward.ContainsKey(xItemId) && xDictReward[xItemId] >= xItemCount)
    {
        xDictReward[xItemId] -= xItemCount;

        if (xDictReward[xItemId] == 0)
        {
            xDictReward.Remove(xItemId);
        }
        return True;
    }
    return False;
}

// 打开奖励列表
Boolean MsConfig::OpenReward_PackageList(MsDictionary<Int32, Int32>& xDictReward_Package)
{
    MsDictionary<Int32, Int32> xTempReward = xDictReward_Package;
    BEGIN_ENUMERATOR(xTempReward)
    {
        lua_reward* xReward = this->Getreward(xEnumKey);
        if (!xReward) { return False; }

        if (PJBase::GetUnitTypeById(xReward->itemid()) == EGT_PACKAGE)
        {
            if (EraseItem(xDictReward_Package, xEnumKey, 1))
            {
                Boolean xRet = this->OpenPackage(xReward->itemid(), xDictReward_Package);
                if (!xRet)
                {
                    return False;
                }
                else
                {
                    continue;
                }
            }
        }
    }END_ENUMERATOR;
    return True;
}

// 打开指定礼包
Boolean MsConfig::OpenPackage(Int32 xPackageId, MsDictionary<Int32, Int32>& xDictReward)
{
    // 开始打开礼包
    lua_package* xPackage = this->Getpackage(xPackageId);
    if (xPackage)
    {
        MsList<WEIGHT_RAND_ASSIST> xListWeightReward;
        if (this->GetPackageWeightRewardList(xPackage, xListWeightReward))
        {
            Int32 xRewardCount = xPackage->randomcount();
            for (Int32 i = 0; i < xRewardCount; i++)
            {
                Int64 xMaxWeight = this->CalculateMaxWeight(xListWeightReward);
                Int64 xRand = R_INT64(0, xMaxWeight) + 1;
                Int32 xRewardKey = this->GetRewardKeyByRandProbability(xRand, xListWeightReward);
                if (xRewardKey == INVALID_NID)
                {
                    return False;
                }
                else
                {
                    AddItem(xDictReward, xRewardKey, 1);
                    if (xDictReward.GetCount() >= MAX_REWARD_COUNT_IN_ONE_PACKAGE)
                    {
                        return False;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            return OpenReward_PackageList(xDictReward);
            //MsList<Int32> xTempReward = xListReward;
            //BEGIN_ENUMERATOR(xTempReward)
            //{
            //    lua_reward* xReward = this->Getreward(xEnumValue);
            //    if (!xReward) { return False; }

            //    if (PJBase::GetUnitTypeById(xReward->itemid()) == EGT_PACKAGE)
            //    {
            //        ERASE_VALUE(xListReward, xEnumValue);
            //        Boolean xRet = this->OpenPackage(xReward->itemid(), xListReward);
            //        if (!xRet)
            //        {
            //            return False;
            //        }
            //        else
            //        {
            //            continue;
            //        }
            //    }
            //}END_ENUMERATOR;
            //return True;
        }
    }
    return False;
}

// 将奖励列表转换成单元(道具,装备,经验,金币,钻石)列表
Boolean MsConfig::RewardList2UnitList(MsDictionary<Int32, Int32>& xDictReward, MsList<ITEM_UNIT>& xListUnit)
{
    BEGIN_ENUMERATOR(xDictReward)
    {
        lua_reward* xReward = this->Getreward(xEnumKey);
        if (!xReward) { return False; }

        E_GOODS_TYPE xUnitType = PJBase::GetUnitTypeById(xReward->itemid());
        switch (xUnitType)
        {
            // 金币
            case EGT_GOLD:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // 体力
            case EGT_STAMINA:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // 钻石
            case EGT_DIAMOND_COIN:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // 经验
            case EGT_EXP:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // (奖励中应该已经拆开所有礼包,不应该再有礼包)礼包(非法)
            case EGT_PACKAGE:
            {
                AssertLog("奖励中应该已经拆开所有礼包,不应该再有礼包");
                return False;
                //xListUnit.Add({ xUnitType, xReward->itemid(),xReward->count() });
            }break;

            case EGT_ITEM:
            {
                lua_item* xItem = this->Getitem(xReward->itemid());
                if (!xItem)
                {
                    AssertLog("奖励中[%d]道具不存在!", xReward->itemid());
                    return False;
                }
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            case EGT_EQUIP:
            {
                lua_equip* xEquip = this->Getequip(xReward->itemid());
                if (!xEquip)
                {
                    AssertLog("奖励中[%d]装备不存在!", xReward->itemid());
                    return False;
                }
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;
        }

    }END_ENUMERATOR;

    return True;
}

// 根据随机结果得到奖励索引
Int32 MsConfig::GetRewardKeyByRandProbability(Int64 xRand, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
{
    Int32 xTempMaxWeigth = 0;
    Int32 xTempRewardKey = INVALID_NID;
    BEGIN_ENUMERATOR(xListWeightReward)
    {
        if (xEnumValue.m_Invalid)
        {
            if (xTempMaxWeigth < xRand)
            {
                xTempRewardKey = xEnumValue.m_Value;
            }
            xTempMaxWeigth += xEnumValue.m_Weight;
            if (xTempMaxWeigth >= xRand)
            {
                xEnumValue.m_Invalid = False;
                break;
            }
        }
        else
        {
            continue;
        }
    }END_ENUMERATOR;
    return xTempRewardKey;
}

// 计算最大权重值
Int64 MsConfig::CalculateMaxWeight(MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
{
    Int64 xMaxWeight = 0;
    BEGIN_ENUMERATOR(xListWeightReward)
    {
        if (xEnumValue.m_Invalid)
        {
            xMaxWeight += xEnumValue.m_Weight;
        }
    }END_ENUMERATOR;
    return xMaxWeight;
}

// 获取指定礼包权重奖励内容
Boolean MsConfig::GetPackageWeightRewardList(lua_package* xPackage, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
{
    // 检查礼包是否存在
    if (xPackage)
    {
        // 检查礼包内容数量是否合法
        Int32 xRewardCount = xPackage->rewardlist_size();
        if (xRewardCount > 0 && xRewardCount == xPackage->weightlist_size())
        {
            xListWeightReward.Clear();
            for (Int32 i = 0; i < xRewardCount; i++)
            {
                xListWeightReward.Add({ xPackage->weightlist(i) , True, xPackage->rewardlist(i) });
            }
            return xRewardCount == xListWeightReward.GetCount();
        }
    }
    return False;
}

void MsConfig::InitMemoryConfig()
{
    //this->m_lua_item.m_Dict_LuaTable.Add();
}

MsConfig::~MsConfig()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_born);
    SAFE_ENUMERATOR_DELETE(m_Dict_role_level);
    SAFE_ENUMERATOR_DELETE(m_Dict_scene);
    SAFE_ENUMERATOR_DELETE(m_Dict_item);
    SAFE_ENUMERATOR_DELETE(m_Dict_suit);
    SAFE_ENUMERATOR_DELETE(m_Dict_equip);
    SAFE_ENUMERATOR_DELETE(m_Dict_suit_shop);
    SAFE_ENUMERATOR_DELETE(m_Dict_face_shop);
    SAFE_ENUMERATOR_DELETE(m_Dict_face_discount);
    SAFE_ENUMERATOR_DELETE(m_Dict_trans_info);
    SAFE_ENUMERATOR_DELETE(m_Dict_transport);
    SAFE_ENUMERATOR_DELETE(m_Dict_npc);
    SAFE_ENUMERATOR_DELETE(m_Dict_battle_wave);
    SAFE_ENUMERATOR_DELETE(m_Dict_monster);
    SAFE_ENUMERATOR_DELETE(m_Dict_summons);
    SAFE_ENUMERATOR_DELETE(m_Dict_random_box);
    SAFE_ENUMERATOR_DELETE(m_Dict_package);
    SAFE_ENUMERATOR_DELETE(m_Dict_reward);
}


void MsConfig::InitFromDB(LPCSTR xConfigName)
{
    this->LoadFromDB_born(xConfigName);          // 出生
    this->LoadFromDB_role_level(xConfigName);    // 等级
    this->LoadFromDB_scene(xConfigName);         // 场景
    this->LoadFromDB_SC(xConfigName);            // 场景
    this->LoadFromDB_item(xConfigName);          // 道具
    this->LoadFromDB_suit(xConfigName);          // 套装
    this->LoadFromDB_equip(xConfigName);         // 装备
    this->LoadFromDB_suit_shop(xConfigName);     // 套装商店
    this->LoadFromDB_face_shop(xConfigName);     // 表情商店
    this->LoadFromDB_face_discount(xConfigName); // 打折表情
    this->LoadFromDB_trans_info(xConfigName);    // 传送信息
    this->LoadFromDB_transport(xConfigName);     // 传送点
    this->LoadFromDB_npc(xConfigName);           // NPC
    this->LoadFromDB_battle_wave(xConfigName);   // 波次
    this->LoadFromDB_monster(xConfigName);       // 怪物
    this->LoadFromDB_summons(xConfigName);       // 召唤兽
    this->LoadFromDB_random_box(xConfigName);    // 随机宝箱
    this->LoadFromDB_package(xConfigName);       // 礼包信息
    this->LoadFromDB_reward(xConfigName);        // 奖励信息
}

Boolean MsConfig::InitFromLua(mstr xResPath, mstr xMainLuaPath)
{
    this->AddPath(xResPath);
    this->DoFile((xResPath + xMainLuaPath).c_str());

    this->LoadFromLua_born();           // 出生
    this->LoadFromLua_role_level();     // 等级
    this->LoadFromLua_scene();          // 场景
    this->LoadFromLua_item();           // 道具
    this->LoadFromLua_suit();           // 套装
    this->LoadFromLua_equip();          // 装备
    this->LoadFromLua_suit_shop();      // 套装商店
    this->LoadFromLua_face_shop();      // 表情商店
    this->LoadFromLua_face_discount();  // 打折表情
    this->LoadFromLua_trans_info();     // 传送信息
    this->LoadFromLua_transport();      // 传送点
    this->LoadFromLua_npc();            // NPC
    this->LoadFromLua_battle_wave();    // 波次
    this->LoadFromLua_monster();        // 怪物
    this->LoadFromLua_summons();        // 召唤兽
    this->LoadFromLua_random_box();     // 随机宝箱
    this->LoadFromLua_package();        // 礼包信息
    this->LoadFromLua_reward();         // 奖励信息

    return True;
}


//void MsConfig::LoadFromDBborn(LPCSTR xRegistryFieldName)
//{
//    MsMysql xMsMysql(xRegistryFieldName);
//    xMsMysql.Connect();
//    MysqlAutoRes xRes;
//    mstr xError;
//    xMsMysql.ExeSQL(xError, xRes, "SELECT * FROM `lua_born`;");
//    if (xRes.m_Res)
//    {
//        MysqlAutoRow xRow = xRes.GetResultRowData();
//        while (xRow)
//        {
//            DWORD xSqlFieldLen = 0;
//            LPBYTE xSqlFieldData = xRow.GetBytes(xRes.m_Res, 1, xSqlFieldLen);
//            lua_born* luaT = NEW lua_born();
//            luaT->ParseFromArray(xSqlFieldData, xSqlFieldLen);
//            this->m_lua_born.m_Dict_LuaTable.Add(xRow.GetInt32(0), luaT);
//            xRow = xRes.GetResultRowData();
//        }
//    }
//    xMsMysql.Disconnect();
//}


// 根据背包道具填充Tips
Boolean MsConfig::FillItemTipsByItem(item* xItem)
{
    mstr xTips = "";
    CHAR szBuff[KB_SIZE];
    if (m_Dict_item.ContainsKey(xItem->item_id()))
    {
        lua_item* xItemConfig = m_Dict_item[xItem->item_id()];
        if (xItemConfig && xItemConfig->type() == "药品")
        {
            DrugsItem xDrugs(xItem, xItemConfig);

            xItem->set_item_res_file(xItemConfig->res_file());
            xItem->set_item_res_key(xItemConfig->res_key());
            xItem->set_item_name(xItemConfig->name());

            sprintf_s(szBuff, "总计%d个\n", xItem->item_count());
            xTips += szBuff;

            if (xDrugs.GetRecoveryHP() > 0)
            {
                sprintf_s(szBuff, "恢复生命%d\n", xDrugs.GetRecoveryHP());
                xTips += szBuff;
            }

            if (xDrugs.GetRecoveryMP() > 0)
            {
                sprintf_s(szBuff, "恢复法力%d\n", xDrugs.GetRecoveryMP());
                xTips += szBuff;
            }

            xItem->set_item_tips(xTips);
        }
        else if (xItemConfig && xItemConfig->type() == "装备")
        {
            EquipItem xEquip(xItem, xItemConfig);

            xItem->set_item_res_file(xItemConfig->res_file());
            xItem->set_item_res_key(xItemConfig->res_key());
            xItem->set_item_name(xItemConfig->name());

            sprintf_s(szBuff, "总计%d个\n", xItem->item_count());
            xTips += szBuff;

            if (xEquip.GetHpPoint() > 0) { sprintf_s(szBuff, "根骨+%d\n", xEquip.GetHpPoint()); xTips += szBuff; }
            if (xEquip.GetMpPoint() > 0) { sprintf_s(szBuff, "灵性+%d\n", xEquip.GetMpPoint()); xTips += szBuff; }
            if (xEquip.GetApPoint() > 0) { sprintf_s(szBuff, "力量+%d\n", xEquip.GetApPoint()); xTips += szBuff; }
            if (xEquip.GetSpPoint() > 0) { sprintf_s(szBuff, "敏捷+%d\n", xEquip.GetSpPoint()); xTips += szBuff; }
            if (xEquip.GetSurplusPoint() > 0) { sprintf_s(szBuff, "自由+%d\n", xEquip.GetSurplusPoint()); xTips += szBuff; }

            xItem->set_item_tips(xTips);
        }
        return True;
    }
    return False;
}
