#pragma once
#define ONE_BAG_PAGE_CELL_COUNT 24

class UDC_CommonBornItem : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDC_CommonBornItem);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(数量);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UDC_CommonBornEquip : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDC_CommonBornEquip);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UDC_CommonBornSummonpet : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDC_CommonBornSummonpet);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UDC_CommonBorn : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDC_CommonBorn);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(背包数量);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornItem, 出生药品);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornItem, 出生道具);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornEquip, 出生装备);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornSummonpet, 出生召唤兽);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Born : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Born);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(出生场景);
    DEF_I32_FIELD(初始等级);
    DEF_I32_FIELD(初始生命);
    DEF_I32_FIELD(初始法力);
    DEF_I32_FIELD(初始最低攻击);
    DEF_I32_FIELD(初始最高攻击);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(出生坐标X);
    DEF_F_FIELD(出生坐标Y);
    DEF_F_FIELD(生命成长);
    DEF_F_FIELD(法力成长);
    DEF_F_FIELD(攻击成长);
    DEF_F_FIELD(速度成长);
    DEF_F_FIELD(初始速度);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(出生道具);
    DEF_STR_FIELD(出生召唤兽);
    DEF_STR_FIELD(出生技能);
    DEF_STR_FIELD(角色类型);
    DEF_STR_FIELD(种族);
    DEF_STR_FIELD(性别);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};
