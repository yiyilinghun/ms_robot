#pragma once
#define ONE_BAG_PAGE_CELL_COUNT 24

class UDC_CommonBornItem : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDC_CommonBornItem);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(����);

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
    DEF_I32_FIELD(��������);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornItem, ����ҩƷ);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornItem, ��������);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornEquip, ����װ��);
    DEF_DICT_PROTO_FIELD_BY_NAME(UDC_CommonBornSummonpet, �����ٻ���);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Born : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Born);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(��������);
    DEF_I32_FIELD(��ʼ�ȼ�);
    DEF_I32_FIELD(��ʼ����);
    DEF_I32_FIELD(��ʼ����);
    DEF_I32_FIELD(��ʼ��͹���);
    DEF_I32_FIELD(��ʼ��߹���);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(��������X);
    DEF_F_FIELD(��������Y);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�����ɳ�);
    DEF_F_FIELD(�ٶȳɳ�);
    DEF_F_FIELD(��ʼ�ٶ�);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��������);
    DEF_STR_FIELD(�����ٻ���);
    DEF_STR_FIELD(��������);
    DEF_STR_FIELD(��ɫ����);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�Ա�);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};
