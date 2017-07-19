#include <Precompiled.h>

void UD_ObjectActionRes::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

void UD_ObjectActionRes::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(UD_ObjectActionRes);

    FAST_READ_STR_VALUE(文件);

    FAST_READ_HEX_VALUE(待机);
    FAST_READ_HEX_VALUE(休闲);
    FAST_READ_HEX_VALUE(行走);
    FAST_READ_HEX_VALUE(跑步);
    FAST_READ_HEX_VALUE(战待);
    FAST_READ_HEX_VALUE(攻击);
    FAST_READ_HEX_VALUE(受击);
    FAST_READ_HEX_VALUE(防御);
    FAST_READ_HEX_VALUE(死亡);
    FAST_READ_HEX_VALUE(施法);
    if (Get休闲() == 0) { this->Set休闲(Get待机()); }
    if (Get行走() == 0) { this->Set行走(Get待机()); }
    if (Get跑步() == 0) { this->Set跑步(Get待机()); }
    if (Get战待() == 0) { this->Set战待(Get待机()); }
    if (Get攻击() == 0) { this->Set攻击(Get待机()); }
    if (Get受击() == 0) { this->Set受击(Get待机()); }
    if (Get防御() == 0) { this->Set防御(Get待机()); }
    if (Get死亡() == 0) { this->Set死亡(Get待机()); }
    if (Get施法() == 0) { this->Set施法(Get待机()); }
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

    FAST_READ_STR_VALUE(文件);

    FAST_READ_HEX_VALUE(小头像);
    FAST_READ_HEX_VALUE(中头像);
    FAST_READ_HEX_VALUE(大头像);
    FAST_READ_HEX_VALUE(肖像);
    if (Get中头像() == 0) { this->Set中头像(Get小头像()); }
    if (Get大头像() == 0) { this->Set大头像(Get小头像()); }
    if (Get肖像() == 0) { this->Set肖像(Get小头像()); }
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

    this->Load动作(xMsXmlReader);
    this->Load头像(xMsXmlReader);
}

Boolean UD_ObjectRes::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(UD_ObjectRes);

    this->Load动作();
    this->Load头像();
    return True;
}

