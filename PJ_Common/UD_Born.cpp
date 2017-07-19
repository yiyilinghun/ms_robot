#include "Precompiled.h"

void UDC_CommonBornItem::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UDC_CommonBornItem::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(CommonBornItem);
    return True;
}

void UDC_CommonBornItem::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(CommonBornItem);

    FAST_READ_INT32_VALUE(数量);
}

void UDC_CommonBornEquip::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UDC_CommonBornEquip::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(CommonBornEquip);
    return True;
}

void UDC_CommonBornEquip::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(CommonBornEquip);
    //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
    //this->SetName(Attribs.m_MsXmlReader->GetTagName());
    //this->SetType("UDC_CommonBornEquip");
}

void UDC_CommonBornSummonpet::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UDC_CommonBornSummonpet::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(CommonBornSummonpet);
    return True;
}

void UDC_CommonBornSummonpet::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(CommonBornSummonpet);
    //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
    //this->SetName(Attribs.m_MsXmlReader->GetTagName());
    //this->SetType("UDC_CommonBornSummonpet");
}

void UDC_CommonBorn::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UDC_CommonBorn::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(CommonBorn);
    return True;
}

void UDC_CommonBorn::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(CommonBorn);
    //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
    //this->SetName(Attribs.m_MsXmlReader->GetTagName());
    //this->SetType("共享出生信息");

    FAST_READ_INT32_VALUE(背包数量);

    this->Load出生药品(xMsXmlReader);
    this->Load出生道具(xMsXmlReader);
    this->Load出生装备(xMsXmlReader);
    this->Load出生召唤兽(xMsXmlReader);
}



void UD_Born::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Born::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(Born);
    return True;
}

void UD_Born::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Born);

    FAST_READ_INT32_VALUE(出生场景);
    FAST_READ_INT32_VALUE(初始等级);
    FAST_READ_INT32_VALUE(初始生命);
    FAST_READ_INT32_VALUE(初始法力);
    FAST_READ_INT32_VALUE(初始最低攻击);
    FAST_READ_INT32_VALUE(初始最高攻击);

    FAST_READ_SINGLE_VALUE(出生坐标X);
    FAST_READ_SINGLE_VALUE(出生坐标Y);
    FAST_READ_SINGLE_VALUE(生命成长);
    FAST_READ_SINGLE_VALUE(法力成长);
    FAST_READ_SINGLE_VALUE(攻击成长);
    FAST_READ_SINGLE_VALUE(初始速度);
    FAST_READ_SINGLE_VALUE(速度成长);

    FAST_READ_STR_VALUE(出生道具);
    FAST_READ_STR_VALUE(出生召唤兽);
    FAST_READ_STR_VALUE(出生技能);
    FAST_READ_STR_VALUE(角色类型);
    FAST_READ_STR_VALUE(种族);
    FAST_READ_STR_VALUE(性别);
}
