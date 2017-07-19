#pragma once

class UD_Npc_Shop_Item : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_Shop_Item);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(�չ���);
    DEF_I32_FIELD(���ۼ�);
    DEF_I32_FIELD(��ʼ����);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(�Ƿ��������);

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_GameItem, GameItem);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc_Shop : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_Shop);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(�ǳ�);
    DEF_STR_FIELD(����);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Shop_Item, �̵���Ʒ�б�);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc_DlgText : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_DlgText);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(����);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc_Dlg : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_Dlg);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_DlgText, �ı�);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(�ȼ�);
    DEF_I32_FIELD(ת������);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(����);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(����X);
    DEF_F_FIELD(����Y);


    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�ǳ�);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�Ա�);
    DEF_STR_FIELD(��ɫ����);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Dlg, �Ի��б�);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Shop, �̵��б�);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};
