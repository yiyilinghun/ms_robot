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
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�Ա�);

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

// ��������
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
    DEF_I32_FIELD(�Ƿ������Ϸ);

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
    DEF_I32_FIELD(ͼ������);
    DEF_I32_FIELD(��Դ����);
    DEF_I32_FIELD(�ȼ�);
    DEF_I32_FIELD(������);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��������);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(ͼ���ļ�);
    DEF_STR_FIELD(��Դ�ļ�);
    DEF_STR_FIELD(����˳��);
    DEF_STR_FIELD(����Ч��);
    DEF_STR_FIELD(Ŀ������);
    DEF_STR_FIELD(���ķ����б�);
    DEF_STR_FIELD(Ч��ǿ���б�);
    DEF_STR_FIELD(Ŀ�������б�);


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
        //this->SetType("����");

        FAST_READ_HEX_VALUE(��Դ����);
        FAST_READ_HEX_VALUE(ͼ������);

        this->Set�ȼ�(atoi(Attribs["�ȼ�"].c_str()));
        this->Set������(atoi(Attribs["������"].c_str()));

        this->Set��������(Attribs["��������"]);
        this->Set����(Attribs["����"]);
        this->Setͼ���ļ�(Attribs["ͼ���ļ�"]);
        this->Set��Դ�ļ�(Attribs["��Դ�ļ�"]);
        this->Set����˳��(Attribs["����˳��"]);
        this->Set����Ч��(Attribs["����Ч��"]);
        this->SetĿ������(Attribs["Ŀ������"]);
        this->Set���ķ����б�(Attribs["���ķ����б�"]);
        this->SetЧ��ǿ���б�(Attribs["Ч��ǿ���б�"]);
        this->SetĿ�������б�(Attribs["Ŀ�������б�"]);
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
    //DEF_STR_FIELD(�ֱ���);
    DEF_PROTO_BEGIN_FIELD;

    DEF_B_BEGIN_FIELD;
    DEF_B_FIELD(�رյ�ǰ�����);
    DEF_B_FIELD(�ܾ�İ������Ϣ);
    //DEF_B_FIELD(ȫ��);
    //DEF_B_FIELD(����);
    DEF_B_FIELD(�д迪��);
    DEF_B_FIELD(�������);
    DEF_B_FIELD(��Ϸ¼��);
    DEF_B_FIELD(�ż�����);
    DEF_B_FIELD(������Ч);
    DEF_B_FIELD(�������);
    DEF_B_FIELD(ս��¼��);
    DEF_B_FIELD(������Ʒ);
    DEF_B_FIELD(ȫ����������);
    DEF_B_FIELD(ˮī);
    DEF_B_FIELD(��ͳ);
    //DEF_B_FIELD(��Ч);
    //DEF_B_FIELD(����);

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


// ����
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

    // �����߼�
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




// ����װ��
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

    // װ���߼�
    // ��ȡȫ�����Ե�
    Int32 GetAllEquipHPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get����(); }
        return xPoint;
    }

    Int32 GetAllEquipMPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get����(); }
        return xPoint;
    }

    Int32 GetAllEquipAPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get����(); }
        return xPoint;
    }

    Int32 GetAllEquipSPPoint()
    {
        Int32 xPoint = 0;
        FAST_FOREACH(m_Dict_EquipByPosType) { xPoint += m_Dict_EquipByPosType.Value()->GetUD_Item_Equip()->Get����(); }
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



// ��������
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
            m_Dict_SkillName[xSkill.Get��������()] = &xSkill;
        }

        return True;
    }

    // �����߼�
    UD_RoleSkill* GetSkillByName(mstr xSkillName)
    {
        return m_Dict_SkillName.ContainsKey(xSkillName) ? m_Dict_SkillName[xSkillName] : nullptr;
    }
    MsUnorderedDictionary<mstr, UD_RoleSkill*> m_Dict_SkillName;
};


// ��������
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


// �ٻ���
class UD_Summonpet : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Summonpet);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(��ʼ�ȼ�);
    DEF_I32_FIELD(��ʼ����);
    DEF_I32_FIELD(��ʼ����);
    DEF_I32_FIELD(��ʼ��͹���);
    DEF_I32_FIELD(��ʼ��߹���);
    DEF_ENUM_FIELD(E_OBJECT_TYPE, ObjectType);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(��ʼ�ٶ�);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�ٶȳɳ�);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��ɫ����);
    DEF_STR_FIELD(Ʒ��);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�Ա�);

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

        FAST_READ_INT32_VALUE(��ʼ�ȼ�);
        FAST_READ_INT32_VALUE(��ʼ����);
        FAST_READ_INT32_VALUE(��ʼ����);
        FAST_READ_INT32_VALUE(��ʼ��͹���);
        FAST_READ_INT32_VALUE(��ʼ��߹���);

        FAST_READ_SINGLE_VALUE(��ʼ�ٶ�);
        FAST_READ_SINGLE_VALUE(�����ɳ�);
        FAST_READ_SINGLE_VALUE(�����ɳ�);
        FAST_READ_SINGLE_VALUE(�����ɳ�);
        FAST_READ_SINGLE_VALUE(�ٶȳɳ�);

        FAST_READ_STR_VALUE(��ɫ����);
        FAST_READ_STR_VALUE(Ʒ��);
        FAST_READ_STR_VALUE(����);
        FAST_READ_STR_VALUE(�Ա�);

        this->GetRoleAttr()->SetLevel(this->Get��ʼ�ȼ�());
        this->GetRoleAttr()->SetBaseHp(this->Get��ʼ����());
        this->GetRoleAttr()->SetBaseMp(this->Get��ʼ����());
        this->GetRoleAttr()->SetBaseMinAp(this->Get��ʼ��͹���());
        this->GetRoleAttr()->SetBaseMaxAp(this->Get��ʼ��߹���());
        this->GetRoleAttr()->SetBaseSp(this->Get��ʼ�ٶ�());

        this->GetRoleAttr()->SetGrowthHp(this->Get�����ɳ�());
        this->GetRoleAttr()->SetGrowthMp(this->Get�����ɳ�());
        this->GetRoleAttr()->SetGrowthAp(this->Get�����ɳ�());
        this->GetRoleAttr()->SetGrowthSp(this->Get�ٶȳɳ�());
    }
};


// ��������
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

    // �ٻ����߼�
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



// ��������
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

    //DEF_DICT_PROTO_FIELD_BY_KEY(�����б�);

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
        //    this->SetType("���͵�");

        //    this->Set����X(atoi(Attribs["����X"].c_str()));
        //    this->Set����Y(atoi(Attribs["����Y"].c_str()));
        //    this->SetĿ�곡��ID(atoi(Attribs["Ŀ�곡��ID"].c_str()));
        //    this->SetĿ������X(atoi(Attribs["Ŀ������X"].c_str()));
        //    this->SetĿ������Y(atoi(Attribs["Ŀ������Y"].c_str()));
        //    this->Set�ȼ�����(atoi(Attribs["�ȼ�����"].c_str()));
        //    this->Set�ȼ�����(atoi(Attribs["�ȼ�����"].c_str()));
        //    this->Set�������(atoi(Attribs["�������"].c_str()));
        //    this->Set������Դ(Attribs["������Դ"]);
    }


    Boolean InitMemory()
    {
        this->GetRolePublicMemory()->SetTargetPosX(this->GetRolePublic()->GetScenePosX());
        this->GetRolePublicMemory()->SetTargetPosY(this->GetRolePublic()->GetScenePosY());

        return True;
    }








    // �����߼�
    UD_RoleSkill* GetSkillByName(mstr xSkillName)
    {
        return m_Dict_SkillName.ContainsKey(xSkillName) ? m_Dict_SkillName[xSkillName] : nullptr;
    }
    MsUnorderedDictionary<mstr, UD_RoleSkill*> m_Dict_SkillName;











};
