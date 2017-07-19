#pragma once

class UD_LevelInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_LevelInfo);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(等级);
    DEF_I32_FIELD(属性点);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(经验);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleLevel);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleLevel);
        //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
        //this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //this->SetType("角色等级");
        FAST_READ_INT32_VALUE(等级);
        FAST_READ_INT32_VALUE(属性点);
        FAST_READ_INT64_VALUE(经验);

        //this->Set等级(atoll(Attribs["等级"].c_str()));
        //this->Set经验(atoll(Attribs["经验"].c_str()));
        //this->Set属性点(atoi(Attribs["属性点"].c_str()));
    }
};
