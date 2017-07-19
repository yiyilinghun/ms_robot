#pragma once

class UD_FindMapInfoPos : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_FindMapInfoPos);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(X);
    DEF_U32_FIELD(Y);

    bool operator==(UD_FindMapInfoPos xSrc) { return xSrc.GetX() == GetX() && xSrc.GetY() == GetY(); }
    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_FindMapInfoPath : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_FindMapInfoPath);

    DEF_PROTO_BEGIN_FIELD;
    DEF_VECTOR_PROTO_FIELD(UD_FindMapInfoPos, PathPos);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_FindMapInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_FindMapInfo);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_KEY(UD_FindMapInfoPath, FindMapInfoPath);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

