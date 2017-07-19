#include "Precompiled.h"

void UD_FindMapInfoPos::SetName(mstr xName) { return UD_Base::SetName(xName); }
Boolean UD_FindMapInfoPos::Load() { SAFE_ENUMERATOR_DELETE(m_Dict_Data); FAST_READ_TYPE_NAME(FindMapInfoPos); return True; }
void UD_FindMapInfoPos::LoadFromObviousXml(MsXmlReader& xMsXmlReader) { FAST_READ_ATTR_TYPE_NAME(FindMapInfoPos); }


void UD_FindMapInfoPath::SetName(mstr xName) { return UD_Base::SetName(xName); }
Boolean UD_FindMapInfoPath::Load() { SAFE_ENUMERATOR_DELETE(m_Dict_Data); FAST_READ_TYPE_NAME(FindMapInfoPath); return True; }
void UD_FindMapInfoPath::LoadFromObviousXml(MsXmlReader& xMsXmlReader) { FAST_READ_ATTR_TYPE_NAME(FindMapInfoPath); }



void UD_FindMapInfo::SetName(mstr xName) { return UD_Base::SetName(xName); }
Boolean UD_FindMapInfo::Load() { SAFE_ENUMERATOR_DELETE(m_Dict_Data); FAST_READ_TYPE_NAME(FindMapInfo); return True; }
void UD_FindMapInfo::LoadFromObviousXml(MsXmlReader& xMsXmlReader) { FAST_READ_ATTR_TYPE_NAME(FindMapInfo); }

