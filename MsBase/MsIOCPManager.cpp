#include "Precompiled.h"

MsIOCPManager::MsIOCPManager()
    : m_hIoCompletionPort(nullptr)
    , m_SelfThreadId(INVALID_UID)
    , m_IsNodeRun(False)
    , m_IsClose(False)
    , m_IsClosed(False)
    , m_CloseAll(False)
    , m_ClusterNetServer(nullptr)
    , m_ClusterNetClient(nullptr)
    , m_CPU_Pressure(INVALID_NID)
    , m_LastUpdatePressureTime(MsBase::Now())
    , m_LastUpdateClusterDBInfoTime(0)
    , m_NodeIndex(INVALID_NID)
    , m_UnitNextIndex(INVALID_LID)
    //, m_Enumerator_Dict_Unit(m_Dict_Unit.GetEnumerator())
    //, m_Enumerator_Dict_TickUnit(m_Dict_TickUnit.GetEnumerator())
    //, m_Enumerator_Dict_SortTimers(m_Dict_SortTimers.GetEnumerator())
{
    m_SelfThreadId = ::GetCurrentThreadId();
    m_hIoCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    AssertNormal(m_hIoCompletionPort, "完成端口创建失败!");
    ZeroMemory(m_OverlappedEntry, sizeof(m_OverlappedEntry));
}

MsIOCPManager::~MsIOCPManager()
{
    if (m_hIoCompletionPort)
    {
        AssertNormal(::CloseHandle(m_hIoCompletionPort), "完成端口销毁失败!");
    }
}

Boolean MsIOCPManager::AttachObject(IocpUnitPtr xIocpUnit)
{
    //AssertNormal(!m_IsNodeRun || m_SelfThreadId == ::GetCurrentThreadId(), "非自有线程");

    IntPtr xHandle = xIocpUnit->GetHandle();
    Int64 xUnitIndex = xIocpUnit->m_UnitIndex;
    if (xHandle != 0 && xHandle != INVALID_HANDLE_VALUE)
    {
        ::CreateIoCompletionPort(xHandle, m_hIoCompletionPort, (ULONG_PTR)xUnitIndex, 0);
    }
    m_Dict_Unit.Add(xUnitIndex, xIocpUnit);
    m_Dict_TickUnit.Add(xUnitIndex, xIocpUnit);
    xIocpUnit->OnAttach();
    return True;
}

Boolean MsIOCPManager::AttachObject(IocpUnit& xIocpUnit)
{
    //AssertNormal(!m_IsNodeRun || m_SelfThreadId == ::GetCurrentThreadId(), "非自有线程");

    IocpUnitPtr lpIocpUnit(&xIocpUnit, [](IocpUnit*) {});

    IntPtr xHandle = lpIocpUnit->GetHandle();
    Int64 xUnitIndex = lpIocpUnit->m_UnitIndex;
    if (xHandle != 0 && xHandle != INVALID_HANDLE_VALUE)
    {
        if (m_hIoCompletionPort != ::CreateIoCompletionPort(xHandle, m_hIoCompletionPort, (ULONG_PTR)xUnitIndex, 0))
        {
            AssertLog("AttachObject::CreateIoCompletionPort::Failed");
        }
    }
    m_Dict_Unit.Add(xUnitIndex, lpIocpUnit);
    m_Dict_TickUnit.Add(xUnitIndex, lpIocpUnit);
    lpIocpUnit->OnAttach();
    return True;
}

Boolean MsIOCPManager::AttachObject(IocpUnit* xIocpUnit, Boolean xNoDelete)
{
    //AssertNormal(!m_IsNodeRun || m_SelfThreadId == ::GetCurrentThreadId(), "非自有线程");

    IocpUnitPtr lpIocpUnit;
    if (xNoDelete) { lpIocpUnit.reset(xIocpUnit, [](IocpUnit*) {}); }
    else { lpIocpUnit.reset(xIocpUnit); }

    IntPtr xHandle = lpIocpUnit->GetHandle();
    Int64 xUnitIndex = lpIocpUnit->m_UnitIndex;
    if (xHandle != 0 && xHandle != INVALID_HANDLE_VALUE)
    {
        ::CreateIoCompletionPort(xHandle, m_hIoCompletionPort, (ULONG_PTR)xUnitIndex, 0);
    }
    m_Dict_Unit.Add(xUnitIndex, lpIocpUnit);
    m_Dict_TickUnit.Add(xUnitIndex, lpIocpUnit);
    lpIocpUnit->OnAttach();
    return True;
}

Boolean MsIOCPManager::AttachObjectDeputyKey(Int64 xDeputyUnitIndex, Int64 xUnitIndex)
{
    //AssertNormal(!m_IsNodeRun || m_SelfThreadId == ::GetCurrentThreadId(), "非自有线程");
    if (m_Dict_Unit.ContainsKey(xUnitIndex) && !m_Dict_Unit.ContainsKey(xDeputyUnitIndex))
    {
        IocpUnitPtr lpIocpUnit = m_Dict_Unit.GetValue(xUnitIndex);
        m_Dict_Unit.Add(xDeputyUnitIndex, lpIocpUnit);
        lpIocpUnit->m_DeputyUnitIndex.Add(xDeputyUnitIndex);
        return True;
    }
    return False;
}

void MsIOCPManager::DetachObjectAttachedDeputy(Int64 xUnitIndex)
{
    auto xLambda_DetachObject = [&](Int64 xUnitIndex)
    {
        m_Dict_Unit.Remove(xUnitIndex);
        //if (m_Dict_Unit.IsValid() && m_Dict_Unit.GetKey() == xUnitIndex) { m_Dict_Unit.Remove(xUnitIndex); }
        //else { m_Dict_Unit.Remove(xUnitIndex); }

        m_Dict_TickUnit.Remove(xUnitIndex);
        //if (m_Enumerator_Dict_TickUnit.IsValid() && m_Enumerator_Dict_TickUnit.GetKey() == xUnitIndex) { m_Enumerator_Dict_TickUnit.Remove(); }
        //else { m_Dict_TickUnit.Remove(xUnitIndex); }
    };

    if (m_Dict_Unit.ContainsKey(xUnitIndex))
    {
        auto& xTemp = m_Dict_Unit[xUnitIndex];
        MsList<Int64>& xDeputyUnitIndex = xTemp->GetDeputyUnitIndex();
        FAST_FOREACH(xDeputyUnitIndex) { xLambda_DetachObject(xDeputyUnitIndex.Value()); }
        xLambda_DetachObject(xUnitIndex);
    }
}

//Boolean MsIOCPManager::DetachObject_(Int64 xUnitIndex)
//{
//    AssertNormal(!m_IsNodeRun || m_SelfThreadId == ::GetCurrentThreadId(), "非自有线程");
//
//    if (m_Enumerator_Dict_Unit.IsValid() && m_Enumerator_Dict_Unit.GetKey() == xUnitIndex)
//    {
//        m_Enumerator_Dict_Unit.Remove();
//    }
//    else
//    {
//        m_Dict_Unit.Remove(xUnitIndex);
//    }
//
//    if (m_Enumerator_Dict_TickUnit.IsValid() && m_Enumerator_Dict_TickUnit.GetKey() == xUnitIndex)
//    {
//        m_Enumerator_Dict_TickUnit.Remove();
//    }
//    else
//    {
//        m_Dict_TickUnit.Remove(xUnitIndex);
//    }
//    return False;
//}

Boolean MsIOCPManager::GetObjectStatus(ULONG& xNumEntriesRemoved)
{
#ifdef _WIN64
    if (!::GetQueuedCompletionStatusEx(m_hIoCompletionPort, m_OverlappedEntry, MAX_ENTRIES_COUNT, &xNumEntriesRemoved, 0, False))
    {
        DWORD dwErrorCode = ::GetLastError();
        switch (dwErrorCode)
        {
            case WAIT_TIMEOUT: { xNumEntriesRemoved = 0; return True; }break;
            case ERROR_ABANDONED_WAIT_0:
            default: { xNumEntriesRemoved = 0; return False; }break;
        }
    }
    return True;
#else
    xNumEntriesRemoved = 0;
    for (Int32 i = 0; i < MAX_ENTRIES_COUNT; i++)
    {
        if (!::GetQueuedCompletionStatus(m_hIoCompletionPort
            , &(m_OverlappedEntry[i].dwNumberOfBytesTransferred)
            , &(m_OverlappedEntry[i].lpCompletionKey)
            , &(m_OverlappedEntry[i].lpOverlapped)
            , 0))
        {
            DWORD dwErrorCode = ::GetLastError();
            switch (dwErrorCode)
            {
                case WAIT_TIMEOUT: { goto END; }break;
                case ERROR_ABANDONED_WAIT_0:
                default: { goto END; }break;
            }
        }
        else
        {
            xNumEntriesRemoved = i + 1;
        }
    }
END:
    return True;
#endif
}

void MsIOCPManager::TickOnce()
{
    if (!m_hIoCompletionPort) { return; }

    SAFE_FOREACH(m_Dict_SortTimers)
    {
        MsTimer* xTimer = xEnumValue;
        if (MsBase::TimeSpanCheck(xTimer))
        {
            DWORD dwEventId = xTimer->m_EventId;
            iocp_timer_ol xIocpol(xTimer->m_IocpUnit, xTimer);
            GetIocpUnitRoute(xTimer->m_IocpUnit->m_IocpUnitType)->ExecutePacket(xTimer->m_IocpUnit, xTimer->m_EventId, &xIocpol);
            m_Dict_SortTimers.RemoveCur();
            m_Dict_SortTimers.Add(xTimer->m_NextTriggerTime, xTimer);
        }
        else
        {
            break;
        }
    }

    ULONG xNumEntriesRemoved = 0;
    this->GetObjectStatus(xNumEntriesRemoved);
    ULONG i = 0;
EXE_MSG:
    try
    {
        for (; i < xNumEntriesRemoved; i++)
        {
            m_CurrentEntrieIndex = i;
            OVERLAPPED_ENTRY& xEntry = m_OverlappedEntry[i];
            if (m_Dict_Unit.ContainsKey((Int64)(xEntry.lpCompletionKey)))
            {
                m_Dict_Unit[(Int64)xEntry.lpCompletionKey]->OnMsg(xEntry);
            }
            else if (m_Dict_LogicUnit.ContainsKey((Int64)(xEntry.lpCompletionKey)))
            {
                m_Dict_LogicUnit[(Int64)xEntry.lpCompletionKey]->OnMsg(xEntry);
            }
        }
    }
    catch (...)
    {
        // lpIocpUnit 执行异常;
        i++;
        goto EXE_MSG;
    }

    if (m_CloseAll)
    {
        SAFE_FOREACH(m_Dict_Unit)
        {
            IocpUnitPtr lpIocpUnit = m_Dict_Unit.Value();
            SAFE_DETACH(lpIocpUnit);
        }
        //if (m_Enumerator_Dict_Unit.ToFirst())
        //{
        //    while (m_Enumerator_Dict_Unit.MoveNext())
        //    {
        //        IocpUnitPtr lpIocpUnit = m_Enumerator_Dict_Unit.Value();
        //        SAFE_DETACH(lpIocpUnit);
        //    }
        //}
    }
    else
    {
        SAFE_FOREACH(m_Dict_TickUnit)
        {
            IocpUnitPtr lpIocpUnit = m_Dict_TickUnit.Value();
            if (!lpIocpUnit->TickOnce())
            {
                SAFE_DETACH(lpIocpUnit);
            }
        }
        //if (m_Enumerator_Dict_TickUnit.ToFirst())
        //{
        //    while (m_Enumerator_Dict_TickUnit.MoveNext())
        //    {
        //        IocpUnitPtr lpIocpUnit = m_Enumerator_Dict_TickUnit.Value();
        //        if (!lpIocpUnit->TickOnce())
        //        {
        //            SAFE_DETACH(lpIocpUnit);
        //        }
        //    }
        //}
    }
}

Int32 MsIOCPManager::GetCount()
{
    return m_Dict_Unit.GetCount();
}

void MsIOCPManager::PostOLMsg(Int64 xUnitIndex, iocp_base_ol* lpIocpol, DWORD xParam)
{
    ::PostQueuedCompletionStatus(m_hIoCompletionPort, xParam, (ULONG_PTR)xUnitIndex, lpIocpol);
}

IocpUnit* MsIOCPManager::GetUnit(Int64 xKey)
{
    if (m_Dict_Unit.ContainsKey(xKey))
    {
        return m_Dict_Unit[xKey].get();
    }
    return nullptr;
}

//void MsIOCPManager::GetUnits(Int64 xKey, MsVector<IocpUnit*>& xListUnits)
//{
//    BEGIN_MULCONTAINER(xKey, m_Dict_Unit)
//    {
//        xListUnits.Add(xMulValue.get());
//    }
//    //auto xIts = m_Dict_Unit.GetValues(xKey);
//    //while (xIts.first != m_Dict_Unit.GetEnd() && xIts.first != xIts.second)
//    //{
//    //    xListUnits.Add((((xIts.first)++)->second).get());
//    //}
//}

Int64 MsIOCPManager::GetClusterNodeIntAddr()
{
    return m_InsideIntAddr;
}

MsClusterTerminal* MsIOCPManager::CheckNetTerminal(Int64 xIntAddr)
{
    if (xIntAddr == INVALID_LID || m_IsClose || m_IsClosed) { return nullptr; }
    if (m_Dict_ClientNode.ContainsKey(xIntAddr))
    {
        return m_Dict_ClientNode[xIntAddr];
    }
    else
    {
        WORD xPortWeb;
        WORD xPortNet;
        mstr xIP = MsBase::Int2IPPP(xIntAddr, xPortWeb, xPortNet);
        MsClusterTerminal* xMsClusterTerminal = NEW MsClusterTerminal(this, False, INVALID_SOCKET);
        xMsClusterTerminal->m_IntAddr = xIntAddr;
        if (!m_ClusterNetClient->BlockCreateClient(xIP, xPortNet, xMsClusterTerminal))
        {
            if (!m_ClusterNetClient->BlockCreateClient(xIP, xPortNet, xMsClusterTerminal))
            {
                if (!m_ClusterNetClient->BlockCreateClient(xIP, xPortNet, xMsClusterTerminal))
                {
                    SAFE_DELETE(xMsClusterTerminal);
                    return nullptr;
                }
            }
        }
        m_Dict_ClientNode.Add(xIntAddr, xMsClusterTerminal);
        return xMsClusterTerminal;
    }
}

void MsIOCPManager::SendPacketForward(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xForwardKey, Boolean IsLastSend)
{
    if (m_IsClose || m_IsClosed) { return; }
    MsClusterTerminal* xMsClusterTerminal = this->CheckNetTerminal(xIntAddr);
    if (xMsClusterTerminal)
    {
        xMsClusterTerminal->SendPacketForward(xBaseProto, xForwardKey, IsLastSend);
    }
}


void MsIOCPManager::SendPacketToUnit(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xUnitKey, Boolean IsLastSend)
{
    if (m_IsClose || m_IsClosed) { return; }
    MsClusterTerminal* xMsClusterTerminal = this->CheckNetTerminal(xIntAddr);
    if (xMsClusterTerminal)
    {
        xMsClusterTerminal->SendPacketToUnit(xBaseProto, xUnitKey, IsLastSend);
    }
    else
    {
        AssertLog("发送给[%lld][%lld]的[%s]失败", xIntAddr, xUnitKey, xBaseProto.GetTypeName().c_str());
    }
}

//void MsIOCPManager::RemoveUnit(Int64 xUnitIndex)
//{
//    m_Dict_Unit.Remove(xUnitIndex);
//}
//
//void MsIOCPManager::RemoveTickUnit(Int64 xUnitIndex)
//{
//    m_Dict_TickUnit.Remove(xUnitIndex);
//}

void MsIOCPManager::KillTimer(IocpUnit* xIocpUnit, DWORD xEventId, Int64 xParam)
{
    //// 如果是当前迭代定时器
    //if (m_Enumerator_Dict_SortTimers.Value()->m_IocpUnit == xIocpUnit && m_Enumerator_Dict_SortTimers.Value()->m_EventId == xEventId && m_Enumerator_Dict_SortTimers.Value()->m_Param == xParam)
    //{
    //    m_Dict_SortTimers.Remove();
    //}
    //else // 如果不是当前迭代定时器
    //{
    if (m_Dict_IocpUnitTimer.ContainsKey(xIocpUnit))
    {
        if (m_Dict_IocpUnitTimer[xIocpUnit].ContainsKey(xEventId))
        {
            auto xKey = m_Dict_IocpUnitTimer[xIocpUnit][xParam][xEventId]->m_NextTriggerTime;
            FAST_FOREACH(m_Dict_SortTimers)
            {
                if (m_Dict_SortTimers.Value()->m_IocpUnit == xIocpUnit && m_Dict_SortTimers.Value()->m_EventId == xEventId)
                {
                    m_Dict_SortTimers.Remove(xKey);
                }
            }
            //auto xIts = m_Dict_SortTimers.GetValues(xKey);
            //while (xIts.first != m_Dict_SortTimers.GetEnd() && xIts.first != xIts.second)
            //{
            //    if (xIts.first->second->m_IocpUnit == xIocpUnit && xIts.first->second->m_EventId == xEventId)
            //    {
            //        m_Dict_SortTimers.Remove(xIts.first->first);
            //    }
            //    ++(xIts.first);
            //}
        }
    }
    //}

    // 移除原始定时器信息
    if (m_Dict_IocpUnitTimer.ContainsKey(xIocpUnit))
    {
        m_Dict_IocpUnitTimer[xIocpUnit].Remove(xEventId);
        if (m_Dict_IocpUnitTimer[xIocpUnit].GetCount() == 0)
        {
            m_Dict_IocpUnitTimer.Remove(xIocpUnit);
        }
    }
}

void MsIOCPManager::SetTimer(IocpUnit* xIocpUnit, DWORD xEventId, Int32 xInterval, Int64 xParam, Boolean xInstantlyTrigger)
{
    SafePtr<MsTimer> xIocpUnitTimer(NEW MsTimer(xIocpUnit, xEventId, xInterval, xParam, xInstantlyTrigger));
    m_Dict_IocpUnitTimer[xIocpUnit][xParam][xEventId] = xIocpUnitTimer;
    m_Dict_SortTimers.Add(xIocpUnitTimer->m_NextTriggerTime, xIocpUnitTimer.get());
}

