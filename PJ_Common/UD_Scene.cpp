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

    FAST_READ_INT32_VALUE(战斗场景);
    FAST_READ_HEX_VALUE(音乐索引);
    FAST_READ_HEX_VALUE(小地图资源);

    FAST_READ_STR_VALUE(名称);
    FAST_READ_STR_VALUE(类型);
    FAST_READ_STR_VALUE(文件);
    FAST_READ_STR_VALUE(音乐文件);
    FAST_READ_STR_VALUE(小地图文件);

    this->LoadTransferPoint(xMsXmlReader);
    this->LoadNpc(xMsXmlReader);
    this->LoadMonster(xMsXmlReader);
}

