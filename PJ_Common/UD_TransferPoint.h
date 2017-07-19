#pragma once

class UD_TransferPoint : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_TransferPoint);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(目标场景ID);
    DEF_I32_FIELD(等级下限);
    DEF_I32_FIELD(等级上限);

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(坐标X);
    DEF_F_FIELD(坐标Y);
    DEF_F_FIELD(目标坐标X);
    DEF_F_FIELD(目标坐标Y);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(索引);
    DEF_I64_FIELD(金币消耗);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(对象资源);

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
        //this->SetType("传送点");
        FAST_READ_INT32_VALUE(目标场景ID);
        FAST_READ_INT32_VALUE(等级下限);
        FAST_READ_INT32_VALUE(等级上限);

        FAST_READ_SINGLE_VALUE(坐标X);
        FAST_READ_SINGLE_VALUE(坐标Y);
        FAST_READ_SINGLE_VALUE(目标坐标X);
        FAST_READ_SINGLE_VALUE(目标坐标Y);

        FAST_READ_INT64_VALUE(金币消耗);

        FAST_READ_STR_VALUE(对象资源);
        //this->Set坐标X(atoi(Attribs["坐标X"].c_str()));
        //this->Set坐标Y(atoi(Attribs["坐标Y"].c_str()));
        //this->Set目标场景ID(atoi(Attribs["目标场景ID"].c_str()));
        //this->Set目标坐标X(atoi(Attribs["目标坐标X"].c_str()));
        //this->Set目标坐标Y(atoi(Attribs["目标坐标Y"].c_str()));
        //this->Set等级下限(atoi(Attribs["等级下限"].c_str()));
        //this->Set等级上限(atoi(Attribs["等级上限"].c_str()));
        //this->Set金币消耗(atoi(Attribs["金币消耗"].c_str()));
        //this->Set对象资源(Attribs["对象资源"]);
    }
};