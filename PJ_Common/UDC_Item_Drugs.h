#pragma once

class UD_Item_Drugs : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Item_Drugs);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(�ָ�����);
    DEF_I32_FIELD(�ָ�����);
    DEF_I32_FIELD(���Ӿ���);
    DEF_I32_FIELD(������Ϸ��);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(��Դ����);
    DEF_U32_FIELD(����);

    DEF_I64_BEGIN_FIELD;

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��Դ�ļ�);
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

        FAST_READ_STR_VALUE(��Դ�ļ�);
        FAST_READ_HEX_VALUE(��Դ����);

        FAST_READ_INT32_VALUE(�ָ�����);
        FAST_READ_INT32_VALUE(�ָ�����);
        FAST_READ_INT32_VALUE(���Ӿ���);
        FAST_READ_INT32_VALUE(������Ϸ��);
        //FAST_READ_INT32_VALUE(����);
    }
};

