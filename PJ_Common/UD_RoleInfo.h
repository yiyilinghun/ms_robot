#pragma once

class UD_RolePublic : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RolePublic);

    DINLINE Position GetPos() { return Position{ this->GetScenePosX(), this->GetScenePosY() }; }
    DINLINE void SetPos(Position& xPos) { this->SetScenePosX(xPos.X); this->SetScenePosY(xPos.Y); }

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(SceneId);
    DEF_ENUM_FIELD(E_OBJECT_TYPE, ObjectType);
    DEF_ENUM_FIELD(E_GAME_OBJECT_DIRECTION, ObjectDir);
    DEF_I32_FIELD(RebornSum);
    DEF_I32_FIELD(VipLevel);


    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleLevel);
    DEF_I64_FIELD(RoleIndex);
    DEF_I64_FIELD(BattleIndex);


    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(ScenePosX);
    DEF_F_FIELD(ScenePosY);
    DEF_F_FIELD(BakScenePosX);
    DEF_F_FIELD(BakScenePosY);


    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(NickName);
    DEF_STR_FIELD(RoleType);
    DEF_STR_FIELD(种族);
    DEF_STR_FIELD(性别);

    DEF_PROTO_BEGIN_FIELD;

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RolePublic);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RolePublic);
    }
};



class UD_RolePrivate : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RolePrivate);

    DEF_I32_ENUM_BEGIN_FIELD;

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);
    DEF_I64_FIELD(GameCoin);
    DEF_I64_FIELD(Stamina);
    DEF_I64_FIELD(DiamondCoin);
    DEF_I64_FIELD(SkillPoint);
    //DEF_I64_FIELD(CurExp);
    //DEF_I64_FIELD(MaxExp);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(Account);
    DEF_STR_FIELD(Phone);
    DEF_STR_FIELD(Email);
    DEF_STR_FIELD(QQ);
    DEF_STR_FIELD(PlatformType);

    DEF_PROTO_BEGIN_FIELD;


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RolePrivate);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RolePrivate);
    }
};

class UD_GameReward : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_GameReward);

    DEF_I32_ENUM_BEGIN_FIELD;

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_STR_BEGIN_FIELD;
    DEF_PROTO_BEGIN_FIELD;


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleTask);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleTask);
    }
};

// 奖励容器
class UD_RoleRewards : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleRewards);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_GameReward, GameReward);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);


    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_GameReward)
        {
            UD_GameReward& xGameReward = m_Dict_GameReward.Value();
            xGameReward.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleRewards);

        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleRewards);
    }
};

#include "UD_RoleAttr.h"

class UD_RolePublicMemory : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RolePublicMemory);

    DINLINE Position GetTarPos() { return Position{ this->GetTargetPosX(), this->GetTargetPosY() }; }
    DINLINE void SetTarPos(Position& xPos) { this->SetTargetPosX(xPos.X); this->SetTargetPosY(xPos.Y); }

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(TeamPos);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);
    DEF_I64_FIELD(TeamIndex);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(TargetPosX);
    DEF_F_FIELD(TargetPosY);

    DEF_STR_BEGIN_FIELD;
    DEF_PROTO_BEGIN_FIELD;


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RolePublicMemory);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RolePublicMemory);
    }
};


class UD_RolePrivateMemory : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RolePrivateMemory);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(是否进入游戏);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    //DEF_F_BEGIN_FIELD;
    //DEF_F_FIELD(TargetPosX);
    //DEF_F_FIELD(TargetPosY);

    DEF_STR_BEGIN_FIELD;
    DEF_PROTO_BEGIN_FIELD;


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }


    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RolePrivateMemory);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RolePrivateMemory);
    }
};


class UD_RoleSkill : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleSkill);

    Int32 GetConsume();
    Single GetPower();
    Int32 GetTarCount();

    E_TargetType m_TargetType;
    E_SelectOrder m_SelectOrder;
    MsVector<S_EffectPower> m_ListEffectPower;
    MsVector<S_Consume> m_ListConsume;
    MsVector<S_TargetCount> m_ListTargetCount;

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(图标索引);
    DEF_I32_FIELD(资源索引);
    DEF_I32_FIELD(等级);
    DEF_I32_FIELD(熟练度);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(技能名称);
    DEF_STR_FIELD(分组);
    DEF_STR_FIELD(图标文件);
    DEF_STR_FIELD(资源文件);
    DEF_STR_FIELD(过滤顺序);
    DEF_STR_FIELD(附带效果);
    DEF_STR_FIELD(目标类型);
    DEF_STR_FIELD(消耗法力列表);
    DEF_STR_FIELD(效果强度列表);
    DEF_STR_FIELD(目标数量列表);


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleSkill);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleSkill);
        //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
        //this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //this->SetType("技能");

        FAST_READ_HEX_VALUE(资源索引);
        FAST_READ_HEX_VALUE(图标索引);

        this->Set等级(atoi(Attribs["等级"].c_str()));
        this->Set熟练度(atoi(Attribs["熟练度"].c_str()));

        this->Set技能名称(Attribs["技能名称"]);
        this->Set分组(Attribs["分组"]);
        this->Set图标文件(Attribs["图标文件"]);
        this->Set资源文件(Attribs["资源文件"]);
        this->Set过滤顺序(Attribs["过滤顺序"]);
        this->Set附带效果(Attribs["附带效果"]);
        this->Set目标类型(Attribs["目标类型"]);
        this->Set消耗法力列表(Attribs["消耗法力列表"]);
        this->Set效果强度列表(Attribs["效果强度列表"]);
        this->Set目标数量列表(Attribs["目标数量列表"]);
    }
};

class UD_RoleConfig : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleConfig);

    DEF_I32_ENUM_BEGIN_FIELD;

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_STR_BEGIN_FIELD;
    //DEF_STR_FIELD(分辨率);
    DEF_PROTO_BEGIN_FIELD;

    DEF_B_BEGIN_FIELD;
    DEF_B_FIELD(关闭当前聊天框);
    DEF_B_FIELD(拒绝陌生人消息);
    //DEF_B_FIELD(全屏);
    //DEF_B_FIELD(窗口);
    DEF_B_FIELD(切磋开关);
    DEF_B_FIELD(接受组队);
    DEF_B_FIELD(游戏录像);
    DEF_B_FIELD(信件接收);
    DEF_B_FIELD(场景特效);
    DEF_B_FIELD(加入好友);
    DEF_B_FIELD(战斗录像);
    DEF_B_FIELD(接收物品);
    DEF_B_FIELD(全屏法术开关);
    DEF_B_FIELD(水墨);
    DEF_B_FIELD(传统);
    //DEF_B_FIELD(音效);
    //DEF_B_FIELD(音乐);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleConfig);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleConfig);
    }
};


// 背包
class UD_RoleBag : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleBag);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY_NO_IF(UD_GameItem, RoleBagItem);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(BagPageCount);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);


    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_RoleBagItem)
        {
            UD_GameItem& xBagItem = m_Dict_RoleBagItem.Value();
            xBagItem.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleBag);

        this->LoadRoleBagItem();

        m_Dict_BagItemPos.Clear();
        FAST_FOREACH(m_Dict_RoleBagItem)
        {
            UD_GameItem& xBagItem = m_Dict_RoleBagItem.Value();
            m_Dict_BagItemPos[xBagItem.GetItemPos()] = &xBagItem;
        }
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleBag);
    }

    // 背包逻辑
    DINLINE Boolean AddItemToBag(UD_GameItem* xGameItem) { return AddItemToBag(*xGameItem); }
    Boolean AddItemToBag(UD_GameItem& xGameItem)
    {
        if (m_Dict_BagItemPos.ContainsKey(xGameItem.GetItemPos())) { return False; }
        if (m_Dict_RoleBagItem.ContainsKey(xGameItem.GetKey())) { return False; }
        this->SetRoleBagItem(xGameItem.GetKey(), xGameItem);
        m_Dict_BagItemPos[xGameItem.GetItemPos()] = &m_Dict_RoleBagItem[xGameItem.GetKey()];
        this->SaveRoleBagItem();
        return True;
    }
    Int32 GetNextFreeBagItem()
    {
        for (Int32 i = 0; i < MAX_UNIT_IN_ONE_PAGE * MAX_BAG_PAGE_COUNT; i++)
        {
            if (!m_Dict_BagItemPos.ContainsKey(i)) { return i; }
        }
        return INVALID_NID;
    }
    Boolean CheckItem(Int32 xBagPos, Int64 xItemIndex = INVALID_LID, Int32 xItemCount = 1)
    {
        UD_GameItem* xGameItem = this->GetBagItem(xBagPos, xItemIndex);
        if (xGameItem)
        {
            return xGameItem->GetItemCount() >= xItemCount;
        }
        return False;
    }
    UD_GameItem* GetBagItem(Int32 xBagPos, Int64 xItemIndex = INVALID_LID)
    {
        UD_GameItem* xTemp = m_Dict_BagItemPos.ContainsKey(xBagPos) ? m_Dict_BagItemPos[xBagPos] : nullptr;
        return xTemp && (xTemp->GetItemIndex() == xItemIndex || xItemIndex == INVALID_LID) ? xTemp : nullptr;
    }
    Boolean RemoveItem(Int32 xBagPos, Int64 xItemIndex = INVALID_LID)
    {
        UD_GameItem* xTemp = this->GetBagItem(xBagPos, xItemIndex);
        if (xTemp)
        {
            m_Dict_BagItemPos.Remove(xBagPos);
            m_Dict_RoleBagItem.Remove(xTemp->GetKey());
            this->SaveRoleBagItem();
            return True;
        }
        return False;
    }
    Boolean RemoveCopyItem(UD_GameItem& xOutGameItem, Int32 xBagPos, Int64 xItemIndex = INVALID_LID)
    {
        UD_GameItem* xGameItem = this->GetBagItem(xBagPos);
        if (xGameItem)
        {
            xOutGameItem.CopyFrom_Load(xGameItem->GetUD());
            return this->RemoveItem(xBagPos, xItemIndex);
        }
        return False;
    }
    MsUnorderedDictionary<Int32, UD_GameItem*> m_Dict_BagItemPos;
};




// 身上装备
class UD_RoleEquips : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleEquips);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_GameItem, RoleEquipItem);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);


    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_RoleEquipItem)
        {
            UD_GameItem& xEquipItem = m_Dict_RoleEquipItem.Value();
            xEquipItem.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleEquips);

        this->LoadRoleEquipItem();

        m_Dict_EquipByPosType.Clear();
        FAST_FOREACH(m_Dict_RoleEquipItem)
        {
            UD_GameItem& xEquipItem = m_Dict_RoleEquipItem.Value();
            m_Dict_EquipByPosType[xEquipItem.GetUD_Item_Equip()->GetEquipPosType()] = &xEquipItem;
        }

        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleEquips);
    }

    // 装备逻辑
    // 获取全部属性点
    Int32 GetAllEquipHPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get根骨(); }
        return xPoint;
    }

    Int32 GetAllEquipMPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get灵性(); }
        return xPoint;
    }

    Int32 GetAllEquipAPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get力量(); }
        return xPoint;
    }

    Int32 GetAllEquipSPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get敏捷(); }
        return xPoint;
    }

    UD_GameItem* GetEquipByPosType(EQUIP_POS_TYPE xPosType)
    {
        return m_Dict_EquipByPosType.ContainsKey(xPosType) ? m_Dict_EquipByPosType[xPosType] : nullptr;
    }
    Boolean SetEquipByPosType(EQUIP_POS_TYPE xPosType, UD_GameItem& xGameItem)
    {
        this->SetRoleEquipItem(xGameItem.GetKey(), xGameItem);
        m_Dict_EquipByPosType[xPosType] = &(m_Dict_RoleEquipItem[xGameItem.GetKey()]);
        this->SaveRoleEquipItem();
        return True;
    }
    Boolean RemoveEquipByPosType(EQUIP_POS_TYPE xPosType)
    {
        UD_GameItem* xEquip = GetEquipByPosType(xPosType);
        if (xEquip)
        {
            m_Dict_EquipByPosType.Remove(xPosType);
            this->RemoveRoleEquipItem(xEquip->GetKey());
            this->SaveRoleEquipItem();
            return True;
        }
        return False;
    }
    MsUnorderedDictionary<EQUIP_POS_TYPE, UD_GameItem*> m_Dict_EquipByPosType;
};



// 技能容器
class UD_RoleSkills : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleSkills);


    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_RoleSkill, RoleSkill);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);


    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_RoleSkill)
        {
            UD_RoleSkill& xSkill = m_Dict_RoleSkill.Value();
            xSkill.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleSkills);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleSkills);

        this->LoadRoleSkill();

        m_Dict_SkillName.Clear();
        FAST_FOREACH(m_Dict_RoleSkill)
        {
            UD_RoleSkill& xSkill = m_Dict_RoleSkill.Value();
            m_Dict_SkillName[xSkill.Get技能名称()] = &xSkill;
        }

        return True;
    }

    // 技能逻辑
    UD_RoleSkill* GetSkillByName(mstr xSkillName)
    {
        return m_Dict_SkillName.ContainsKey(xSkillName) ? m_Dict_SkillName[xSkillName] : nullptr;
    }
    MsUnorderedDictionary<mstr, UD_RoleSkill*> m_Dict_SkillName;
};


// 任务容器
class UD_RoleTasks : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleTasks);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_GameTask, GameTask);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);


    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_GameTask)
        {
            UD_GameTask& xGameTask = m_Dict_GameTask.Value();
            xGameTask.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleTasks);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleTasks);
    }
};


// 召唤兽
class UD_Summonpet : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Summonpet);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(初始等级);
    DEF_I32_FIELD(初始生命);
    DEF_I32_FIELD(初始法力);
    DEF_I32_FIELD(初始最低攻击);
    DEF_I32_FIELD(初始最高攻击);
    DEF_ENUM_FIELD(E_OBJECT_TYPE, ObjectType);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(初始速度);
    DEF_F_FIELD(生命成长);
    DEF_F_FIELD(法力成长);
    DEF_F_FIELD(攻击成长);
    DEF_F_FIELD(速度成长);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(角色类型);
    DEF_STR_FIELD(品质);
    DEF_STR_FIELD(种族);
    DEF_STR_FIELD(性别);

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_RolePublic, RolePublic);
    DEF_PROTO_FIELD(UD_RoleAttr, RoleAttr);
    DEF_PROTO_FIELD(UD_RoleSkills, RoleSkills);

    DINLINE void SetRoleIndex(Int64 xRoleIndex) { return m_prt->set_key(xRoleIndex); }
    DINLINE Int64 GetRoleIndex() { return m_prt->key(); }
    //DINLINE Int64 role_index() { return m_prt->key(); }

    virtual void SetName(mstr xName)
    {
        //this->GetRolePublicMemory()->SetName(xName);
        //this->GetRolePrivateMemory()->SetName(xName);
        this->GetRolePublic()->SetName(xName);
        //this->GetRolePrivate()->SetName(xName);
        this->GetRoleAttr()->SetName(xName);
        this->GetRoleSkills()->SetName(xName);
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(Summonpet);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(Summonpet);

        FAST_READ_INT32_VALUE(初始等级);
        FAST_READ_INT32_VALUE(初始生命);
        FAST_READ_INT32_VALUE(初始法力);
        FAST_READ_INT32_VALUE(初始最低攻击);
        FAST_READ_INT32_VALUE(初始最高攻击);

        FAST_READ_SINGLE_VALUE(初始速度);
        FAST_READ_SINGLE_VALUE(生命成长);
        FAST_READ_SINGLE_VALUE(法力成长);
        FAST_READ_SINGLE_VALUE(攻击成长);
        FAST_READ_SINGLE_VALUE(速度成长);

        FAST_READ_STR_VALUE(角色类型);
        FAST_READ_STR_VALUE(品质);
        FAST_READ_STR_VALUE(种族);
        FAST_READ_STR_VALUE(性别);

        this->GetRoleAttr()->SetLevel(this->Get初始等级());
        this->GetRoleAttr()->SetBaseHp(this->Get初始生命());
        this->GetRoleAttr()->SetBaseMp(this->Get初始法力());
        this->GetRoleAttr()->SetBaseMinAp(this->Get初始最低攻击());
        this->GetRoleAttr()->SetBaseMaxAp(this->Get初始最高攻击());
        this->GetRoleAttr()->SetBaseSp(this->Get初始速度());

        this->GetRoleAttr()->SetGrowthHp(this->Get生命成长());
        this->GetRoleAttr()->SetGrowthMp(this->Get法力成长());
        this->GetRoleAttr()->SetGrowthAp(this->Get攻击成长());
        this->GetRoleAttr()->SetGrowthSp(this->Get速度成长());
    }
};


// 宝宝容器
class UD_Summonpets : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Summonpets);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_Summonpet, Summonpet);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_Summonpet)
        {
            UD_Summonpet& xSummonpet = m_Dict_Summonpet.Value();
            xSummonpet.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(Summonpets);

        this->LoadSummonpet();

        m_Dict_SummonPetByKeyIndex.Clear();
        m_Dict_SummonPetByPosIndex.Clear();
        Int32 xIndex = 0;
        FAST_FOREACH(m_Dict_Summonpet)
        {
            UD_Summonpet& xSummonpet = m_Dict_Summonpet.Value();
            m_Dict_SummonPetByKeyIndex[xSummonpet.GetKey()] = &xSummonpet;
            m_Dict_SummonPetByPosIndex[xIndex++] = &xSummonpet;
        }

        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(Summonpets);
    }

    // 召唤兽逻辑
    UD_Summonpet* FindSummonpetByRoleIndex(Int64 xRoleIndex)
    {
        return m_Dict_SummonPetByKeyIndex.ContainsKey(xRoleIndex) ? m_Dict_SummonPetByKeyIndex[xRoleIndex] : nullptr;
    }
    MsUnorderedDictionary<Int64, UD_Summonpet*> m_Dict_SummonPetByKeyIndex;

    UD_Summonpet* FindSummonpetByPosIndex(Int32 xPosIndex)
    {
        return m_Dict_SummonPetByPosIndex.ContainsKey(xPosIndex) ? m_Dict_SummonPetByPosIndex[xPosIndex] : nullptr;
    }
    MsUnorderedDictionary<Int32, UD_Summonpet*> m_Dict_SummonPetByPosIndex;
};



// 好友容器
class UD_RoleFriends : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleFriends);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_RoleFriend, GameFriend);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    virtual void SetName(mstr xName)
    {
        FAST_FOREACH(m_Dict_GameFriend)
        {
            UD_RoleFriend& xRoleFriend = m_Dict_GameFriend.Value();
            xRoleFriend.SetName(xName);
        }
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleFriends);

        this->LoadGameFriend();

        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleFriends);
    }
};


class S2C_Sync_RoleInfo;
class UD_RoleInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleInfo);

    DINLINE void SetTeamIndex(Int64 xTeamIndex)
    {
        this->GetRolePublicMemory()->SetTeamIndex(xTeamIndex);
        this->GetRoleAttr()->SetTeamIndex(xTeamIndex);
    }

    DINLINE void SetLevel(Int64 xLevel)
    {
        this->GetRolePublic()->SetRoleLevel(xLevel);
        this->GetRoleAttr()->SetLevel(xLevel);
    }

    DINLINE void SetNickName(mstr xNickName)
    {
        this->SetName(xNickName);
        this->GetRolePublic()->SetNickName(xNickName);
    }
    DINLINE void SetRoleIndex(Int64 xRoleIndex)
    {
        this->GetRolePublic()->SetRoleIndex(xRoleIndex);
        this->GetRolePublicMemory()->SetRoleIndex(xRoleIndex);
        this->GetRolePrivate()->SetRoleIndex(xRoleIndex);
        this->GetRolePrivateMemory()->SetRoleIndex(xRoleIndex);

        this->GetRoleAttr()->SetRoleIndex(xRoleIndex);
        this->GetRoleConfig()->SetRoleIndex(xRoleIndex);

        return m_prt->set_key(xRoleIndex);
    }
    DINLINE Int64 GetRoleIndex() { return m_prt->key(); }
    //DINLINE Int64 role_index() { return m_prt->key(); }
    DINLINE Boolean CheckExpToLevelUp() { return this->GetRoleAttr()->GetCurExp() >= this->GetRoleAttr()->GetMaxExp(); }

    void FillRoleInfo(S2C_Sync_RoleInfo* xRoleInfoPacket);

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_RolePublicMemory, RolePublicMemory);
    DEF_PROTO_FIELD(UD_RolePrivateMemory, RolePrivateMemory);
    DEF_PROTO_FIELD(UD_RolePublic, RolePublic);
    DEF_PROTO_FIELD(UD_RolePrivate, RolePrivate);
    DEF_PROTO_FIELD(UD_RoleAttr, RoleAttr);
    DEF_PROTO_FIELD(UD_RoleBag, RoleBag);
    DEF_PROTO_FIELD(UD_RoleEquips, RoleEquips);
    DEF_PROTO_FIELD(UD_RoleSkills, RoleSkills);
    DEF_PROTO_FIELD(UD_RoleTasks, RoleTasks);
    DEF_PROTO_FIELD(UD_RoleFriends, RoleFriends);
    DEF_PROTO_FIELD(UD_RoleConfig, RoleConfig);
    DEF_PROTO_FIELD(UD_Summonpets, Summonpets);

    //DEF_DICT_PROTO_FIELD_BY_KEY(任务列表);

    void Update(const universal_data& xRoleInfo)
    {
        if (xRoleInfo.type() == "RoleInfo") { this->CopyFrom_Load(xRoleInfo); }

        else if (xRoleInfo.type() == "RolePublicMemory") { this->GetRolePublicMemory()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RolePrivateMemory") { this->GetRolePrivateMemory()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RolePublic") { this->GetRolePublic()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RolePrivate") { this->GetRolePrivate()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleAttr") { this->GetRoleAttr()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleBag") { this->GetRoleBag()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleEquips") { this->GetRoleEquips()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleSkill") { this->GetRoleSkills()->CopyFrom_Load(xRoleInfo); }
        //else if (xRoleInfo.type() == "RoleRewards") { this->GetRoleRewards()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleTasks") { this->GetRoleTasks()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleFriends") { this->GetRoleFriends()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "RoleConfig") { this->GetRoleConfig()->CopyFrom_Load(xRoleInfo); }
        else if (xRoleInfo.type() == "Summonpets") { this->GetSummonpets()->CopyFrom_Load(xRoleInfo); }
        this->Load();
    }


    virtual void SetName(mstr xName)
    {
        this->GetRolePublicMemory()->SetName(xName);
        this->GetRolePrivateMemory()->SetName(xName);
        this->GetRolePublic()->SetName(xName);
        this->GetRolePrivate()->SetName(xName);
        this->GetRoleAttr()->SetName(xName);
        this->GetRoleConfig()->SetName(xName);
        this->GetRoleBag()->SetName(xName);
        this->GetRoleEquips()->SetName(xName);
        this->GetRoleSkills()->SetName(xName);
        //this->GetRoleRewards()->SetName(xName);
        this->GetRoleTasks()->SetName(xName);
        this->GetRoleFriends()->SetName(xName);
        this->GetSummonpets()->SetName(xName);
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleInfo);

        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleInfo);
        //    MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;

        //    this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //    this->SetType("传送点");

        //    this->Set坐标X(atoi(Attribs["坐标X"].c_str()));
        //    this->Set坐标Y(atoi(Attribs["坐标Y"].c_str()));
        //    this->Set目标场景ID(atoi(Attribs["目标场景ID"].c_str()));
        //    this->Set目标坐标X(atoi(Attribs["目标坐标X"].c_str()));
        //    this->Set目标坐标Y(atoi(Attribs["目标坐标Y"].c_str()));
        //    this->Set等级下限(atoi(Attribs["等级下限"].c_str()));
        //    this->Set等级上限(atoi(Attribs["等级上限"].c_str()));
        //    this->Set金币消耗(atoi(Attribs["金币消耗"].c_str()));
        //    this->Set对象资源(Attribs["对象资源"]);
    }


    Boolean InitMemory()
    {
        this->GetRolePublicMemory()->SetTargetPosX(this->GetRolePublic()->GetScenePosX());
        this->GetRolePublicMemory()->SetTargetPosY(this->GetRolePublic()->GetScenePosY());

        return True;
    }








    // 技能逻辑
    UD_RoleSkill* GetSkillByName(mstr xSkillName)
    {
        return m_Dict_SkillName.ContainsKey(xSkillName) ? m_Dict_SkillName[xSkillName] : nullptr;
    }
    MsUnorderedDictionary<mstr, UD_RoleSkill*> m_Dict_SkillName;











};
