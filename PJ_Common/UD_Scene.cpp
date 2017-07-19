#include "Precompiled.h"

Boolean UD_Scene::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(Scene);

    this->LoadTransferPoint();
    this->LoadNpc();
    this->LoadMonster();

    return True;
}

void UD_Scene::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene);

    this->SetID(atoll(Attribs["ID"].c_str()));

    FAST_READ_INT32_VALUE(ս������);
    FAST_READ_HEX_VALUE(��������);
    FAST_READ_HEX_VALUE(С��ͼ��Դ);

    FAST_READ_STR_VALUE(����);
    FAST_READ_STR_VALUE(����);
    FAST_READ_STR_VALUE(�ļ�);
    FAST_READ_STR_VALUE(�����ļ�);
    FAST_READ_STR_VALUE(С��ͼ�ļ�);

    this->LoadTransferPoint(xMsXmlReader);
    this->LoadNpc(xMsXmlReader);
    this->LoadMonster(xMsXmlReader);
}

