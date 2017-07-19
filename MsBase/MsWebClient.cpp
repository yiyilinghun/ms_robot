#include "Precompiled.h"

MsThreadWebClient::MsThreadWebClient(LPCSTR xName)
    : MsThread(xName, MsThreadWebProc, this, 50, False, True)
{
    ;
}

void MsThreadWebClient::TickOnce()
{
    for (Int32 i = 0; i < m_QueueWebol.GetCount(); i++)
    {
        m_WebLock.Lock();
        WEB_OL* lpWebol = m_QueueWebol.Dequeue();
        m_WebLock.UnLock();

        Boolean xRet = False;
        if (lpWebol->m_lpRetProto)
        {
            xRet = MsWebClient::WebProtoRequestRetProto(lpWebol->m_lpPostProto, lpWebol->m_lpRetProto
                , MsBase::M2W(lpWebol->m_Addr).c_str()
                , lpWebol->m_Port
                , MsBase::M2W(lpWebol->m_Url).c_str());
        }
        else
        {
            xRet = MsWebClient::WebProtoRequestRetStr(lpWebol->m_lpPostProto, lpWebol->m_RetStr
                , MsBase::M2W(lpWebol->m_Addr).c_str()
                , lpWebol->m_Port
                , MsBase::M2W(lpWebol->m_Url).c_str());
        }
        lpWebol->m_IsFailed = !xRet;
        lpWebol->m_IsCompleted = True;
    }
}

void MsThreadWebClient::ExeWeb_Asyn(WEB_OL* lpWebol)
{
    m_WebLock.Lock();
    m_QueueWebol.Enqueue(lpWebol);
    m_WebLock.UnLock();
}
