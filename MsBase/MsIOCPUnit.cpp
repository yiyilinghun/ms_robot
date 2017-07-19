#include "Precompiled.h"

UnitRoute<LogicUnit>* GetIocpUnitRoute(E_ROUTE_TYPE xType)
{
    static UnitRoute<LogicUnit> ms_IocpUnitRoutes[ERT_MAX_COUNT];
    return &(ms_IocpUnitRoutes[xType]);
}

IocpUnit::IocpUnit(mstr xUnitName, MsIOCPManager* xMsIOCPManager, Int64 xUnitIndex)
    : LogicUnit(xUnitName, xMsIOCPManager, xUnitIndex)
{
    m_IocpUnitType = E_ROUTE_TYPE::ERT_IocpUnit;
    if (m_UnitIndex == INVALID_LID) { m_UnitIndex = m_MsIOCPManager->GetNextUnitIndex(); }
}

IocpUnit::~IocpUnit()
{
    ;
}

//Int64 IocpUnit::GetUnitIndex() { return m_UnitIndex; }
//mstr IocpUnit::GetUnitIndexStr()  
//{
//    CHAR szBuff[100];
//    sprintf_s(szBuff, "%lld", m_UnitIndex);
//    return szBuff;
//}

Boolean IocpUnit::TickOnce()
{
    m_MsIOCPManager->m_Dict_TickUnit.Remove(m_UnitIndex);
    return True;
}

//void IocpUnit::OnNetMsg(iocp_net_ol* xIocpNetol, OVERLAPPED_ENTRY& xEntry)
//{
//    std::shared_ptr<iocp_net_ol> lpIocpol; lpIocpol.reset(xIocpNetol);
//    GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xEntry.dwNumberOfBytesTransferred, xIocpNetol);
//}

//void IocpUnit::OnWebMsg(iocp_web_ol* xIocpWebol)
//{
//    std::shared_ptr<iocp_web_ol> lpIocpol; lpIocpol.reset(xIocpWebol);
//    GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xIocpWebol->m_Entry->dwNumberOfBytesTransferred, xIocpWebol);
//}

//void IocpUnit::OnDiyMsg(OVERLAPPED_ENTRY& xEntry)
//{
//    std::shared_ptr<iocp_ol> lpIocpol; lpIocpol.reset((iocp_ol*)xEntry.lpOverlapped);
//    GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xEntry.dwNumberOfBytesTransferred, lpIocpol.get());
//}

Boolean IocpUnit::PostMsg(iocp_base_ol* xIocpol, DWORD dwParam)
{
    AssertNormal(xIocpol, "IOCO_OL 不能为空!");
    return ::PostQueuedCompletionStatus(m_MsIOCPManager->m_hIoCompletionPort, dwParam, (ULONG_PTR)m_UnitIndex, xIocpol);
}

//Boolean IocpUnit::Attach()
//{
//    return m_MsIOCPManager->AttachObject(this);
//    return True;
//}

//void IocpUnit::Detach()
//{
//    return m_MsIOCPManager->DetachObjectAttachedDeputy(m_UnitIndex);
//}

//void IocpUnit::Destroy()
//{
//    if (m_MsClusterNode)
//    {
//        m_MsClusterNode->DestroyObject(this);
//    }
//    else if (m_MsThreadIOCPManager)
//    {
//        m_MsThreadIOCPManager->DestroyObject(this);
//    }
//    else if (m_MsIOCPManager)
//    {
//        m_MsIOCPManager->DestroyObject(this);
//    }
//}



//void IocpUnit::Direct_ClearTimer()
//{
//    m_DictTimer.Clear();
//}
//
//void IocpUnit::ClearTimer()
//{
//    this->PostMsg(NEW iocp_ol(this, IOCP_OL_TYPE::IOT_CLEAR_TIMER));
//}
//
//void IocpUnit::Wait_ClearTimer()
//{
//    while (m_DictTimer.GetCount() > 0) { ::Sleep(10); }
//}

//LPMSTIMER IocpUnit::GetTimer(DWORD dwEventId)
//{
//    if (m_DictTimer.ContainsKey(dwEventId))
//    {
//        return &(m_DictTimer[dwEventId]);
//    }
//    return nullptr;
//}

//void IocpUnit::SendPacket(Int64 xIntAddr, BaseProto& xBaseProto, Boolean IsLastSend)
//{
//    if (m_MsClusterNode)
//    {
//        m_MsClusterNode->SendPacket(xIntAddr, xBaseProto, IsLastSend);
//    }
//}

void IocpUnit::SendPacketForward(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xForwardKey, Boolean IsLastSend)
{
    if (xForwardKey == INVALID_LID)
    {
        m_MsIOCPManager->SendPacketForward(xIntAddr, xBaseProto, this->m_UnitIndex, IsLastSend);
    }
    else
    {
        m_MsIOCPManager->SendPacketForward(xIntAddr, xBaseProto, xForwardKey, IsLastSend);
    }
}

void IocpUnit::SendPacketToUnit(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xUnitKey, Boolean IsLastSend)
{
    if (xUnitKey == INVALID_LID)
    {
        m_MsIOCPManager->SendPacketToUnit(xIntAddr, xBaseProto, this->m_UnitIndex, IsLastSend);
    }
    else
    {
        m_MsIOCPManager->SendPacketToUnit(xIntAddr, xBaseProto, xUnitKey, IsLastSend);
    }
}

//void IocpUnit::SendPacketForward(Int64 xIntAddr, BaseProto& xBaseProto, Boolean IsLastSend)
//{
//    if (m_MsClusterNode) { m_MsClusterNode->SendPacketForward(xIntAddr, xBaseProto, m_UnitIndex, IsLastSend); }
//}

//void IocpUnit::SendPacketToUnit(Int64 xIntAddr, BaseProto& xBaseProto, Boolean IsLastSend)
//{
//    if (m_MsClusterNode)
//    {
//        m_MsClusterNode->SendPacketToUnit(xIntAddr, xBaseProto, m_UnitIndex, IsLastSend);
//    }
//}

//void IocpUnit::SendPacketToLogin(Int64 xIntAddr, BaseProto& xBaseProto, Boolean IsLastSend)
//{
//    if (m_MsClusterNode)
//    {
//        m_MsClusterNode->SendPacketToUnit(xIntAddr, xBaseProto, SERVER_LOGINMANAGER_IOCP_UNIT_KEY, IsLastSend);
//    }
//}


//void IocpUnit::SendPacketRetForward(Int64 xIntAddr, BaseProto& xBaseProto, Boolean IsLastSend)
//{
//    if (m_MsClusterNode)
//    {
//        m_MsClusterNode->SendPacket(xIntAddr, xBaseProto, INVALID_LID, m_UnitIndex, IsLastSend);
//    }
//}

//void IocpUnit::ExeWeb_Asyn(LPBASEPROTO lpPostProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xObjectName, ...)
//{
//    if (m_MsClusterNode && m_MsClusterNode->m_ClusterWebClient)
//    {
//        CHAR szUrl[KB_SIZE];
//        va_list ap;
//        va_start(ap, xObjectName);
//        _vsnprintf_s(szUrl, KB_SIZE, xObjectName, ap);
//        va_end(ap);
//        LPWEB_OL lpWebol = NEW WEB_OL(lpPostProto, xAddr, xPort, xCallbackKey, szUrl);
//        m_ListWebol.Add(lpWebol);
//        m_MsClusterNode->m_ClusterWebClient->ExeWeb_Asyn(lpWebol);
//    }
//}

//void IocpUnit::ExeWeb_Asyn(LPBASEPROTO lpPostProto, LPBASEPROTO lpRetProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xObjectName, ...)
//{
//    if (m_MsClusterNode && m_MsClusterNode->m_ClusterWebClient)
//    {
//        CHAR szUrl[KB_SIZE];
//        va_list ap;
//        va_start(ap, xObjectName);
//        _vsnprintf_s(szUrl, KB_SIZE, xObjectName, ap);
//        va_end(ap);
//        AssertLog("创建:%s", xObjectName);
//        LPWEB_OL lpWebol = NEW WEB_OL(lpPostProto, lpRetProto, xAddr, xPort, xCallbackKey, szUrl);
//        lpWebol->m_ObjectName = xObjectName;
//        m_ListWebol.Add(lpWebol);
//        m_MsClusterNode->m_ClusterWebClient->ExeWeb_Asyn(lpWebol);
//    }
//}

Boolean IocpUnit::OnTimer(MsTimer& xTimer)
{
    iocp_timer_ol xIocpol(this, &xTimer);
    return GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xTimer.m_EventId, &xIocpol);
};

//Boolean IocpUnit::IsSqlCompleted()
//{
//    return m_ListSqlol.Empty();
//}
//
//void IocpUnit::WaitSql()
//{
//    while (m_ListSqlol.GetCount() > 0) { ::Sleep(10); }
//}

Boolean IocpUnit::OnNewPacket(DWORD xMsgType, iocp_packet_ol* xIocpol)
{
    return GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xMsgType, xIocpol);
}

void IocpUnit::SetTimer(DWORD dwEventId, Int32 xInterval, Int64 xParam, Boolean xInstantlyTrigger)
{
    return m_MsIOCPManager->SetTimer(this, dwEventId, xInterval, xParam, xInstantlyTrigger);
}

void IocpUnit::KillTimer(DWORD dwEventId)
{
    return m_MsIOCPManager->KillTimer(this, dwEventId);
}

void IocpUnit::CreateDirectTimer(DWORD dwEventId, Int32 xInterval, Int64 xParam, Boolean xInstantlyTrigger, UnitRoute<LogicUnit>::LPEXEFUN xFun)
{
    GetIocpUnitRoute(m_IocpUnitType)->RegisterPacket(dwEventId, xFun);
    return m_MsIOCPManager->SetTimer(this, dwEventId, xInterval, xParam, xInstantlyTrigger);
}
