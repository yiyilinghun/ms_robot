#pragma once

class UD_ObjectActionRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectActionRes);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(�ļ�);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(�ܲ�);
    DEF_U32_FIELD(ս��);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(�ܻ�);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(����);
    DEF_U32_FIELD(ʩ��);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};

class UD_ObjectIconRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectIconRes);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(�ļ�);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(Сͷ��);
    DEF_U32_FIELD(��ͷ��);
    DEF_U32_FIELD(��ͷ��);
    DEF_U32_FIELD(Ф��);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};

class UD_ObjectRes : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ObjectRes);

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_ObjectActionRes, ����);
    DEF_PROTO_FIELD(UD_ObjectIconRes, ͷ��);

    virtual void SetName(mstr xName);
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
    virtual Boolean Load();
};
