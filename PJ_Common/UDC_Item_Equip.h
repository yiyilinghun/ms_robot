#pragma once

class UD_Item_Equip : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Item_Equip);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_ENUM_FIELD(EQUIP_POS_TYPE, EquipPosType);
    DEF_ENUM_FIELD(EQUIP_TYPE, EquipType);
    DEF_I32_FIELD(����);
    DEF_I32_FIELD(����);
    DEF_I32_FIELD(����);
    DEF_I32_FIELD(����);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(��Դ����);
    DEF_U32_FIELD(����);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    //DEF_U32_BEGIN_FIELD;
    //DEF_U32_FIELD(ResKey);

    //DEF_STR_BEGIN_FIELD;
    //DEF_STR_FIELD(EquipPosType);

    //EQUIP_POS_TYPE m_װ����λ;
    //EQUIP_TYPE m_װ������;

    //DEF_I32_ENUM_BEGIN_FIELD;



    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��Դ�ļ�);
    DEF_STR_FIELD(��λ);
    DEF_STR_FIELD(����);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(ItemEquip);

        FAST_READ_STR_VALUE(��Դ�ļ�);
        FAST_READ_HEX_VALUE(��Դ����);

        FAST_READ_STR_VALUE(��λ);
        FAST_READ_STR_VALUE(����);

        FAST_READ_ENUM_VALUE(EQUIP_POS_TYPE, EquipPosType);
        FAST_READ_ENUM_VALUE(EQUIP_TYPE, EquipType);

        FAST_READ_INT32_VALUE(����);
        FAST_READ_INT32_VALUE(����);
        FAST_READ_INT32_VALUE(����);
        FAST_READ_INT32_VALUE(����);
        FAST_READ_INT32_VALUE(����);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(ItemEquip);

        if (this->Get��λ() == "����") { this->SetEquipPosType(EPT_PI_FENG); }
        else if (this->Get��λ() == "ñ��") { this->SetEquipPosType(EPT_MAO_ZI); }
        else if (this->Get��λ() == "����") { this->SetEquipPosType(EPT_WU_QI); }
        else if (this->Get��λ() == "�·�") { this->SetEquipPosType(EPT_YI_FU); }
        else if (this->Get��λ() == "�Ҽ�") { this->SetEquipPosType(EPT_GUA_JIAN); }
        else if (this->Get��λ() == "Ь��") { this->SetEquipPosType(EPT_XIE_ZI); }
        else if (this->Get��λ() == "���") { this->SetEquipPosType(EPT_MIAN_JU); }
        else if (this->Get��λ() == "����") { this->SetEquipPosType(EPT_XIANG_LIAN); }
        else if (this->Get��λ() == "��ָ") { this->SetEquipPosType(EPT_JIE_ZHI); }
        else if (this->Get��λ() == "����") { this->SetEquipPosType(EPT_YAO_DAI); }
        else { AssertLog("δ֪װ����λ%s", this->Get��λ().c_str()); }


        if (this->Get����() == "����_��") { this->SetEquipType(ET_PI_FENG_NAN); }
        else if (this->Get����() == "����_Ů") { this->SetEquipType(ET_PI_FENG_NV); }
        else if (this->Get����() == "ñ��_��") { this->SetEquipType(ET_MAO_ZI_NAN); }
        else if (this->Get����() == "ñ��_Ů") { this->SetEquipType(ET_MAO_ZI_NV); }
        else if (this->Get����() == "�·�_��") { this->SetEquipType(ET_YI_FU_NAN); }
        else if (this->Get����() == "�·�_Ů") { this->SetEquipType(ET_YI_FU_NV); }
        else if (this->Get����() == "�Ҽ�_��") { this->SetEquipType(ET_GUA_JIAN_NAN); }
        else if (this->Get����() == "�Ҽ�_Ů") { this->SetEquipType(ET_GUA_JIAN_NV); }
        else if (this->Get����() == "���_��") { this->SetEquipType(ET_MIAN_JU_NAN); }
        else if (this->Get����() == "���_Ů") { this->SetEquipType(ET_MIAN_JU_NV); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_JIAN); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_DAO); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_BIAN); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_CHUI); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_GUN); }
        else if (this->Get����() == "����_צ") { this->SetEquipType(ET_WU_QI_ZHUA); }
        else if (this->Get����() == "����_ǹ") { this->SetEquipType(ET_WU_QI_QIANG); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_FU); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_SHAN); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_GOU); }
        else if (this->Get����() == "����_ȭ") { this->SetEquipType(ET_WU_QI_QUAN); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_CHEN); }
        else if (this->Get����() == "����_�") { this->SetEquipType(ET_WU_QI_FAN); }
        else if (this->Get����() == "����_��") { this->SetEquipType(ET_WU_QI_DAI); }
        else if (this->Get����() == "����") { this->SetEquipType(ET_XIANG_LIAN); }
        else if (this->Get����() == "Ь��") { this->SetEquipType(ET_XIE_ZI); }
        else if (this->Get����() == "��ָ") { this->SetEquipType(ET_JIE_ZHI); }
        else if (this->Get����() == "����") { this->SetEquipType(ET_YAO_DAI); }
        else { AssertLog("δ֪װ������%s", this->Get����().c_str()); }

        return True;
    }

};
