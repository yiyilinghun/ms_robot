#pragma once

class UD_LevelInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_LevelInfo);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(�ȼ�);
    DEF_I32_FIELD(���Ե�);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(����);

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
        //this->SetType("��ɫ�ȼ�");
        FAST_READ_INT32_VALUE(�ȼ�);
        FAST_READ_INT32_VALUE(���Ե�);
        FAST_READ_INT64_VALUE(����);

        //this->Set�ȼ�(atoll(Attribs["�ȼ�"].c_str()));
        //this->Set����(atoll(Attribs["����"].c_str()));
        //this->Set���Ե�(atoi(Attribs["���Ե�"].c_str()));
    }
};
