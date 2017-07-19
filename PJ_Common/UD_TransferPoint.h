#pragma once

class UD_TransferPoint : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_TransferPoint);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(Ŀ�곡��ID);
    DEF_I32_FIELD(�ȼ�����);
    DEF_I32_FIELD(�ȼ�����);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(����X);
    DEF_F_FIELD(����Y);
    DEF_F_FIELD(Ŀ������X);
    DEF_F_FIELD(Ŀ������Y);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(����);
    DEF_I64_FIELD(�������);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(������Դ);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(TransferPoint);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(TransferPoint);
        //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
        //this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //this->SetType("���͵�");
        FAST_READ_INT32_VALUE(Ŀ�곡��ID);
        FAST_READ_INT32_VALUE(�ȼ�����);
        FAST_READ_INT32_VALUE(�ȼ�����);

        FAST_READ_SINGLE_VALUE(����X);
        FAST_READ_SINGLE_VALUE(����Y);
        FAST_READ_SINGLE_VALUE(Ŀ������X);
        FAST_READ_SINGLE_VALUE(Ŀ������Y);

        FAST_READ_INT64_VALUE(�������);

        FAST_READ_STR_VALUE(������Դ);
        //this->Set����X(atoi(Attribs["����X"].c_str()));
        //this->Set����Y(atoi(Attribs["����Y"].c_str()));
        //this->SetĿ�곡��ID(atoi(Attribs["Ŀ�곡��ID"].c_str()));
        //this->SetĿ������X(atoi(Attribs["Ŀ������X"].c_str()));
        //this->SetĿ������Y(atoi(Attribs["Ŀ������Y"].c_str()));
        //this->Set�ȼ�����(atoi(Attribs["�ȼ�����"].c_str()));
        //this->Set�ȼ�����(atoi(Attribs["�ȼ�����"].c_str()));
        //this->Set�������(atoi(Attribs["�������"].c_str()));
        //this->Set������Դ(Attribs["������Դ"]);
    }
};