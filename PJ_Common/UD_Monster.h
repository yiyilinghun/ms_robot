#pragma once

class UD_Monster : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Monster);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(等级);


    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(概率);
    DEF_STR_FIELD(角色类型);
    DEF_STR_FIELD(技能列表);


    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(Monster);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(Monster);
        //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
        //this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //this->SetType("怪物");

        this->Set等级(atoi(Attribs["等级"].c_str()));

        this->Set概率(Attribs["概率"]);
        this->Set角色类型(Attribs["角色类型"]);
        this->Set技能列表(Attribs["技能列表"]);
    }
};
