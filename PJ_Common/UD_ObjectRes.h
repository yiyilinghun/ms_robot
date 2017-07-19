#pragma once

class UD_ObjectActionRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectActionRes);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(文件);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(待机);
    DEF_U32_FIELD(休闲);
    DEF_U32_FIELD(行走);
    DEF_U32_FIELD(跑步);
    DEF_U32_FIELD(战待);
    DEF_U32_FIELD(攻击);
    DEF_U32_FIELD(受击);
    DEF_U32_FIELD(防御);
    DEF_U32_FIELD(死亡);
    DEF_U32_FIELD(施法);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};

class UD_ObjectIconRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectIconRes);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(文件);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(小头像);
    DEF_U32_FIELD(中头像);
    DEF_U32_FIELD(大头像);
    DEF_U32_FIELD(肖像);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};

class UD_ObjectRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectRes);

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_ObjectActionRes, 动作);
    DEF_PROTO_FIELD(UD_ObjectIconRes, 头像);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};
