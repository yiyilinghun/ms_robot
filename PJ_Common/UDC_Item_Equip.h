#pragma once

class UD_Item_Equip : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Item_Equip);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_ENUM_FIELD(EQUIP_POS_TYPE, EquipPosType);
    DEF_ENUM_FIELD(EQUIP_TYPE, EquipType);
    DEF_I32_FIELD(根骨);
    DEF_I32_FIELD(灵性);
    DEF_I32_FIELD(力量);
    DEF_I32_FIELD(敏捷);

    DEF_U32_BEGIN_FIELD;
    DEF_U32_FIELD(资源索引);
    DEF_U32_FIELD(自由);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    //DEF_U32_BEGIN_FIELD;
    //DEF_U32_FIELD(ResKey);

    //DEF_STR_BEGIN_FIELD;
    //DEF_STR_FIELD(EquipPosType);

    //EQUIP_POS_TYPE m_装备部位;
    //EQUIP_TYPE m_装备类型;

    //DEF_I32_ENUM_BEGIN_FIELD;



    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(资源文件);
    DEF_STR_FIELD(部位);
    DEF_STR_FIELD(类型);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(ItemEquip);

        FAST_READ_STR_VALUE(资源文件);
        FAST_READ_HEX_VALUE(资源索引);

        FAST_READ_STR_VALUE(部位);
        FAST_READ_STR_VALUE(类型);

        FAST_READ_ENUM_VALUE(EQUIP_POS_TYPE, EquipPosType);
        FAST_READ_ENUM_VALUE(EQUIP_TYPE, EquipType);

        FAST_READ_INT32_VALUE(根骨);
        FAST_READ_INT32_VALUE(灵性);
        FAST_READ_INT32_VALUE(力量);
        FAST_READ_INT32_VALUE(敏捷);
        FAST_READ_INT32_VALUE(自由);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(ItemEquip);

        if (this->Get部位() == "披风") { this->SetEquipPosType(EPT_PI_FENG); }
        else if (this->Get部位() == "帽子") { this->SetEquipPosType(EPT_MAO_ZI); }
        else if (this->Get部位() == "武器") { this->SetEquipPosType(EPT_WU_QI); }
        else if (this->Get部位() == "衣服") { this->SetEquipPosType(EPT_YI_FU); }
        else if (this->Get部位() == "挂件") { this->SetEquipPosType(EPT_GUA_JIAN); }
        else if (this->Get部位() == "鞋子") { this->SetEquipPosType(EPT_XIE_ZI); }
        else if (this->Get部位() == "面具") { this->SetEquipPosType(EPT_MIAN_JU); }
        else if (this->Get部位() == "项链") { this->SetEquipPosType(EPT_XIANG_LIAN); }
        else if (this->Get部位() == "戒指") { this->SetEquipPosType(EPT_JIE_ZHI); }
        else if (this->Get部位() == "腰带") { this->SetEquipPosType(EPT_YAO_DAI); }
        else { AssertLog("未知装备部位%s", this->Get部位().c_str()); }


        if (this->Get类型() == "披风_男") { this->SetEquipType(ET_PI_FENG_NAN); }
        else if (this->Get类型() == "披风_女") { this->SetEquipType(ET_PI_FENG_NV); }
        else if (this->Get类型() == "帽子_男") { this->SetEquipType(ET_MAO_ZI_NAN); }
        else if (this->Get类型() == "帽子_女") { this->SetEquipType(ET_MAO_ZI_NV); }
        else if (this->Get类型() == "衣服_男") { this->SetEquipType(ET_YI_FU_NAN); }
        else if (this->Get类型() == "衣服_女") { this->SetEquipType(ET_YI_FU_NV); }
        else if (this->Get类型() == "挂件_男") { this->SetEquipType(ET_GUA_JIAN_NAN); }
        else if (this->Get类型() == "挂件_女") { this->SetEquipType(ET_GUA_JIAN_NV); }
        else if (this->Get类型() == "面具_男") { this->SetEquipType(ET_MIAN_JU_NAN); }
        else if (this->Get类型() == "面具_女") { this->SetEquipType(ET_MIAN_JU_NV); }
        else if (this->Get类型() == "武器_剑") { this->SetEquipType(ET_WU_QI_JIAN); }
        else if (this->Get类型() == "武器_刀") { this->SetEquipType(ET_WU_QI_DAO); }
        else if (this->Get类型() == "武器_鞭") { this->SetEquipType(ET_WU_QI_BIAN); }
        else if (this->Get类型() == "武器_锤") { this->SetEquipType(ET_WU_QI_CHUI); }
        else if (this->Get类型() == "武器_棍") { this->SetEquipType(ET_WU_QI_GUN); }
        else if (this->Get类型() == "武器_爪") { this->SetEquipType(ET_WU_QI_ZHUA); }
        else if (this->Get类型() == "武器_枪") { this->SetEquipType(ET_WU_QI_QIANG); }
        else if (this->Get类型() == "武器_斧") { this->SetEquipType(ET_WU_QI_FU); }
        else if (this->Get类型() == "武器_扇") { this->SetEquipType(ET_WU_QI_SHAN); }
        else if (this->Get类型() == "武器_钩") { this->SetEquipType(ET_WU_QI_GOU); }
        else if (this->Get类型() == "武器_拳") { this->SetEquipType(ET_WU_QI_QUAN); }
        else if (this->Get类型() == "武器_尘") { this->SetEquipType(ET_WU_QI_CHEN); }
        else if (this->Get类型() == "武器_幡") { this->SetEquipType(ET_WU_QI_FAN); }
        else if (this->Get类型() == "武器_带") { this->SetEquipType(ET_WU_QI_DAI); }
        else if (this->Get类型() == "项链") { this->SetEquipType(ET_XIANG_LIAN); }
        else if (this->Get类型() == "鞋子") { this->SetEquipType(ET_XIE_ZI); }
        else if (this->Get类型() == "戒指") { this->SetEquipType(ET_JIE_ZHI); }
        else if (this->Get类型() == "腰带") { this->SetEquipType(ET_YAO_DAI); }
        else { AssertLog("未知装备类型%s", this->Get类型().c_str()); }

        return True;
    }

};
