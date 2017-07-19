#pragma once

class UD_Item_Drugs : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Item_Drugs);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(恢复生命);
    DEF_I32_FIELD(恢复法力);
    DEF_I32_FIELD(增加经验);
    DEF_I32_FIELD(增加游戏币);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(资源索引);
    DEF_U32_FIELD(数量);

    DEF_I64_BEGIN_FIELD;

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(资源文件);
    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }


    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(ItemDrugs);
        return True;
    }


    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(ItemDrugs);

        FAST_READ_STR_VALUE(资源文件);
        FAST_READ_HEX_VALUE(资源索引);

        FAST_READ_INT32_VALUE(恢复生命);
        FAST_READ_INT32_VALUE(恢复法力);
        FAST_READ_INT32_VALUE(增加经验);
        FAST_READ_INT32_VALUE(增加游戏币);
        //FAST_READ_INT32_VALUE(数量);
    }
};

