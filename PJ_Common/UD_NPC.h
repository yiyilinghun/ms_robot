#pragma once

class UD_Npc_Shop_Item : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_Shop_Item);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(收购价);
    DEF_I32_FIELD(销售价);
    DEF_I32_FIELD(初始数量);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(是否可以跑商);

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
    DEF_STR_FIELD(昵称);
    DEF_STR_FIELD(类型);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Shop_Item, 商店商品列表);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc_DlgText : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_DlgText);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(内容);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc_Dlg : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc_Dlg);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_DlgText, 文本);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_Npc : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Npc);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(等级);
    DEF_I32_FIELD(转生次数);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(索引);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(坐标X);
    DEF_F_FIELD(坐标Y);


    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(朝向);
    DEF_STR_FIELD(昵称);
    DEF_STR_FIELD(种族);
    DEF_STR_FIELD(性别);
    DEF_STR_FIELD(角色类型);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Dlg, 对话列表);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc_Shop, 商店列表);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};
