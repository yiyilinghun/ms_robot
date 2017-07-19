#pragma once

class UD_Monster : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Monster);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(�ȼ�);


    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(��ɫ����);
    DEF_STR_FIELD(�����б�);


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
        //this->SetType("����");

        this->Set�ȼ�(atoi(Attribs["�ȼ�"].c_str()));

        this->Set����(Attribs["����"]);
        this->Set��ɫ����(Attribs["��ɫ����"]);
        this->Set�����б�(Attribs["�����б�"]);
    }
};
