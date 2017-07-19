#pragma once

// װ����������
class UD_GameEquipBaseAttr : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_GameEquipBaseAttr);

    DEF_I32_ENUM_BEGIN_FIELD;

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);

    DEF_PROTO_BEGIN_FIELD;

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};


mstr Getװ����λ�ı�(EQUIP_POS_TYPE xEquipPosType);
mstr Getװ�������ı�(EQUIP_TYPE xEquipType);

// ����
class UD_GameItem : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_GameItem);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(ItemPos);
    DEF_I32_FIELD(ItemCount);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(ResKey);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(ItemName);
    DEF_STR_FIELD(ItemType);

    DEF_PROTO_BEGIN_FIELD;
    DEF_UNIVERSAL_FIELD(0);
    DEF_UNIVERSAL_FIELD_INSTANCE(UD_Item_Drugs, 0);
    DEF_UNIVERSAL_FIELD_INSTANCE(UD_Item_Equip, 0);
    //UD_Item_Drugs* GetItemByDrugs();
    //void SetItemToDrugs(UD_Item_Drugs* xItemDrugs);
    //UD_Item_Equip* GetItemByEquip();
    //void SetItemToEquip(UD_Item_Equip* xItemEquip);
    Int64 GetItemIndex();

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};


// ����
class UD_GameTask : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_GameTask);

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


// ����
class UD_RoleFriend : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleFriend);

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
