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

    FAST_READ_INT32_VALUE(����);
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
    //this->SetType("���������Ϣ");

    FAST_READ_INT32_VALUE(��������);

    this->Load����ҩƷ(xMsXmlReader);
    this->Load��������(xMsXmlReader);
    this->Load����װ��(xMsXmlReader);
    this->Load�����ٻ���(xMsXmlReader);
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

    FAST_READ_INT32_VALUE(��������);
    FAST_READ_INT32_VALUE(��ʼ�ȼ�);
    FAST_READ_INT32_VALUE(��ʼ����);
    FAST_READ_INT32_VALUE(��ʼ����);
    FAST_READ_INT32_VALUE(��ʼ��͹���);
    FAST_READ_INT32_VALUE(��ʼ��߹���);

    FAST_READ_SINGLE_VALUE(��������X);
    FAST_READ_SINGLE_VALUE(��������Y);
    FAST_READ_SINGLE_VALUE(�����ɳ�);
    FAST_READ_SINGLE_VALUE(�����ɳ�);
    FAST_READ_SINGLE_VALUE(�����ɳ�);
    FAST_READ_SINGLE_VALUE(��ʼ�ٶ�);
    FAST_READ_SINGLE_VALUE(�ٶȳɳ�);

    FAST_READ_STR_VALUE(��������);
    FAST_READ_STR_VALUE(�����ٻ���);
    FAST_READ_STR_VALUE(��������);
    FAST_READ_STR_VALUE(��ɫ����);
    FAST_READ_STR_VALUE(����);
    FAST_READ_STR_VALUE(�Ա�);
}
