#include "Precompiled.h"

#define MAX_REWARD_COUNT_IN_ONE_PACKAGE 100 // һ�ο�������������

// ���ָ������Ƿ����
Boolean MsConfig::CheckHavePackage(Int32 xPackageId)
{
    return m_Dict_package.ContainsKey(xPackageId);
}

// ����ָ��������Ʒ���ֵ�
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

// �Ƴ�ָ��������Ʒ���ֵ�
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

// �򿪽����б�
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

// ��ָ�����
Boolean MsConfig::OpenPackage(Int32 xPackageId, MsDictionary<Int32, Int32>& xDictReward)
{
    // ��ʼ�����
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

// �������б�ת���ɵ�Ԫ(����,װ��,����,���,��ʯ)�б�
Boolean MsConfig::RewardList2UnitList(MsDictionary<Int32, Int32>& xDictReward, MsList<ITEM_UNIT>& xListUnit)
{
    BEGIN_ENUMERATOR(xDictReward)
    {
        lua_reward* xReward = this->Getreward(xEnumKey);
        if (!xReward) { return False; }

        E_GOODS_TYPE xUnitType = PJBase::GetUnitTypeById(xReward->itemid());
        switch (xUnitType)
        {
            // ���
            case EGT_GOLD:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // ����
            case EGT_STAMINA:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // ��ʯ
            case EGT_DIAMOND_COIN:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // ����
            case EGT_EXP:
            {
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            // (������Ӧ���Ѿ����������,��Ӧ���������)���(�Ƿ�)
            case EGT_PACKAGE:
            {
                AssertLog("������Ӧ���Ѿ����������,��Ӧ���������");
                return False;
                //xListUnit.Add({ xUnitType, xReward->itemid(),xReward->count() });
            }break;

            case EGT_ITEM:
            {
                lua_item* xItem = this->Getitem(xReward->itemid());
                if (!xItem)
                {
                    AssertLog("������[%d]���߲�����!", xReward->itemid());
                    return False;
                }
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;

            case EGT_EQUIP:
            {
                lua_equip* xEquip = this->Getequip(xReward->itemid());
                if (!xEquip)
                {
                    AssertLog("������[%d]װ��������!", xReward->itemid());
                    return False;
                }
                xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
            }break;
        }

    }END_ENUMERATOR;

    return True;
}

// �����������õ���������
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

// �������Ȩ��ֵ
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

// ��ȡָ�����Ȩ�ؽ�������
Boolean MsConfig::GetPackageWeightRewardList(lua_package* xPackage, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
{
    // �������Ƿ����
    if (xPackage)
    {
        // ���������������Ƿ�Ϸ�
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
    this->LoadFromDB_born(xConfigName);          // ����
    this->LoadFromDB_role_level(xConfigName);    // �ȼ�
    this->LoadFromDB_scene(xConfigName);         // ����
    this->LoadFromDB_SC(xConfigName);            // ����
    this->LoadFromDB_item(xConfigName);          // ����
    this->LoadFromDB_suit(xConfigName);          // ��װ
    this->LoadFromDB_equip(xConfigName);         // װ��
    this->LoadFromDB_suit_shop(xConfigName);     // ��װ�̵�
    this->LoadFromDB_face_shop(xConfigName);     // �����̵�
    this->LoadFromDB_face_discount(xConfigName); // ���۱���
    this->LoadFromDB_trans_info(xConfigName);    // ������Ϣ
    this->LoadFromDB_transport(xConfigName);     // ���͵�
    this->LoadFromDB_npc(xConfigName);           // NPC
    this->LoadFromDB_battle_wave(xConfigName);   // ����
    this->LoadFromDB_monster(xConfigName);       // ����
    this->LoadFromDB_summons(xConfigName);       // �ٻ���
    this->LoadFromDB_random_box(xConfigName);    // �������
    this->LoadFromDB_package(xConfigName);       // �����Ϣ
    this->LoadFromDB_reward(xConfigName);        // ������Ϣ
}

Boolean MsConfig::InitFromLua(mstr xResPath, mstr xMainLuaPath)
{
    this->AddPath(xResPath);
    this->DoFile((xResPath + xMainLuaPath).c_str());

    this->LoadFromLua_born();           // ����
    this->LoadFromLua_role_level();     // �ȼ�
    this->LoadFromLua_scene();          // ����
    this->LoadFromLua_item();           // ����
    this->LoadFromLua_suit();           // ��װ
    this->LoadFromLua_equip();          // װ��
    this->LoadFromLua_suit_shop();      // ��װ�̵�
    this->LoadFromLua_face_shop();      // �����̵�
    this->LoadFromLua_face_discount();  // ���۱���
    this->LoadFromLua_trans_info();     // ������Ϣ
    this->LoadFromLua_transport();      // ���͵�
    this->LoadFromLua_npc();            // NPC
    this->LoadFromLua_battle_wave();    // ����
    this->LoadFromLua_monster();        // ����
    this->LoadFromLua_summons();        // �ٻ���
    this->LoadFromLua_random_box();     // �������
    this->LoadFromLua_package();        // �����Ϣ
    this->LoadFromLua_reward();         // ������Ϣ

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


// ���ݱ����������Tips
Boolean MsConfig::FillItemTipsByItem(item* xItem)
{
    mstr xTips = "";
    CHAR szBuff[KB_SIZE];
    if (m_Dict_item.ContainsKey(xItem->item_id()))
    {
        lua_item* xItemConfig = m_Dict_item[xItem->item_id()];
        if (xItemConfig && xItemConfig->type() == "ҩƷ")
        {
            DrugsItem xDrugs(xItem, xItemConfig);

            xItem->set_item_res_file(xItemConfig->res_file());
            xItem->set_item_res_key(xItemConfig->res_key());
            xItem->set_item_name(xItemConfig->name());

            sprintf_s(szBuff, "�ܼ�%d��\n", xItem->item_count());
            xTips += szBuff;

            if (xDrugs.GetRecoveryHP() > 0)
            {
                sprintf_s(szBuff, "�ָ�����%d\n", xDrugs.GetRecoveryHP());
                xTips += szBuff;
            }

            if (xDrugs.GetRecoveryMP() > 0)
            {
                sprintf_s(szBuff, "�ָ�����%d\n", xDrugs.GetRecoveryMP());
                xTips += szBuff;
            }

            xItem->set_item_tips(xTips);
        }
        else if (xItemConfig && xItemConfig->type() == "װ��")
        {
            EquipItem xEquip(xItem, xItemConfig);

            xItem->set_item_res_file(xItemConfig->res_file());
            xItem->set_item_res_key(xItemConfig->res_key());
            xItem->set_item_name(xItemConfig->name());

            sprintf_s(szBuff, "�ܼ�%d��\n", xItem->item_count());
            xTips += szBuff;

            if (xEquip.GetHpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetHpPoint()); xTips += szBuff; }
            if (xEquip.GetMpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetMpPoint()); xTips += szBuff; }
            if (xEquip.GetApPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetApPoint()); xTips += szBuff; }
            if (xEquip.GetSpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetSpPoint()); xTips += szBuff; }
            if (xEquip.GetSurplusPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetSurplusPoint()); xTips += szBuff; }

            xItem->set_item_tips(xTips);
        }
        return True;
    }
    return False;
}
