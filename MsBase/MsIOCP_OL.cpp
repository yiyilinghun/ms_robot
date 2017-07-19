#include "Precompiled.h"

WEB_OL::WEB_OL(LPBASEPROTO lpPostProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xUrl)
{
    m_Addr = xAddr;
    m_Port = xPort;
    m_lpPostProto = lpPostProto;
    m_lpRetProto = nullptr;
    m_Url = xUrl;
    m_IsCompleted = False;
    m_IsFailed = False;
    m_CallbackKey = xCallbackKey;
}

WEB_OL::WEB_OL(LPBASEPROTO lpPostProto, LPBASEPROTO lpRetProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xUrl)
{
    m_Addr = xAddr;
    m_Port = xPort;
    m_lpPostProto = lpPostProto;
    m_lpRetProto = lpRetProto;
    m_Url = xUrl;
    m_IsCompleted = False;
    m_IsFailed = False;
    m_CallbackKey = xCallbackKey;
}

WEB_OL::~WEB_OL()
{
    SAFE_DELETE(m_lpPostProto);
    SAFE_DELETE(m_lpRetProto);
}
