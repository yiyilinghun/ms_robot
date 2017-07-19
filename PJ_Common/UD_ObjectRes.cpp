#include <Precompiled.h>

void UD_ObjectActionRes::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

void UD_ObjectActionRes::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(UD_ObjectActionRes);

    FAST_READ_STR_VALUE(�ļ�);

    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(�ܲ�);
    FAST_READ_HEX_VALUE(ս��);
    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(�ܻ�);
    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(����);
    FAST_READ_HEX_VALUE(ʩ��);
    if (Get����() == 0) { this->Set����(Get����()); }
    if (Get����() == 0) { this->Set����(Get����()); }
    if (Get�ܲ�() == 0) { this->Set�ܲ�(Get����()); }
    if (Getս��() == 0) { this->Setս��(Get����()); }
    if (Get����() == 0) { this->Set����(Get����()); }
    if (Get�ܻ�() == 0) { this->Set�ܻ�(Get����()); }
    if (Get����() == 0) { this->Set����(Get����()); }
    if (Get����() == 0) { this->Set����(Get����()); }
    if (Getʩ��() == 0) { this->Setʩ��(Get����()); }
    //Int32 iPosParent, iPos, iPosChild;
    //xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
    //xMsXmlReader[""#field];
    //xMsXmlReader._SetPos(iPosParent, iPos, iPosChild); 
}

Boolean UD_ObjectActionRes::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(UD_ObjectActionRes);
    return True;
}







void UD_ObjectIconRes::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

void UD_ObjectIconRes::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(UD_ObjectIconRes);

    FAST_READ_STR_VALUE(�ļ�);

    FAST_READ_HEX_VALUE(Сͷ��);
    FAST_READ_HEX_VALUE(��ͷ��);
    FAST_READ_HEX_VALUE(��ͷ��);
    FAST_READ_HEX_VALUE(Ф��);
    if (Get��ͷ��() == 0) { this->Set��ͷ��(GetСͷ��()); }
    if (Get��ͷ��() == 0) { this->Set��ͷ��(GetСͷ��()); }
    if (GetФ��() == 0) { this->SetФ��(GetСͷ��()); }
}

Boolean UD_ObjectIconRes::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(UD_ObjectIconRes);
    return True;
}








void UD_ObjectRes::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

void UD_ObjectRes::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(UD_ObjectRes);

    this->Load����(xMsXmlReader);
    this->Loadͷ��(xMsXmlReader);
}

Boolean UD_ObjectRes::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(UD_ObjectRes);

    this->Load����();
    this->Loadͷ��();
    return True;
}

