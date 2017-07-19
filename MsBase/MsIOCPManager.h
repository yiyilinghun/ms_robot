#pragma once

// 完成端口管理器
class MsClusterTerminal;
class MsClusterNetClient;
class MsClusterNetServer;
typedef class MsIOCPManager
{
public:
    MsIOCPManager();
    ~MsIOCPManager();

    Boolean AttachObject(IocpUnitPtr xIocpUnit);
    Boolean AttachObject(IocpUnit& xIocpUnit);
    Boolean AttachObject(IocpUnit* xIocpUnit, Boolean xNoDelete = False);
    Boolean AttachObjectDeputyKey(Int64 xDeputyUnitIndex, Int64 xUnitIndex);
    void DetachObjectAttachedDeputy(Int64 xUnitIndex);

    Boolean GetObjectStatus(ULONG& xNumEntriesRemoved);
    virtual void TickOnce();

    Int32 GetCount();

    void PostOLMsg(Int64 xUnitIndex, iocp_base_ol* lpIocpol, DWORD xParam = 0);

    template<typename T> T* GetUnitBy(Int64 xKey) { return (T*)this->GetUnit(xKey); }
    IocpUnit* GetUnit(Int64 xKey);
    //void GetUnits(Int64 xKey, MsVector<IocpUnit*>& xListUnits);

    Int64 GetClusterNodeIntAddr();
    MsClusterTerminal* CheckNetTerminal(Int64 xIntAddr);
    void SendPacketForward(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xForwardKey, Boolean IsLastSend = False);
    void SendPacketToUnit(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xUnitKey, Boolean IsLastSend = False);

    ULONG m_CurrentEntrieIndex = INVALID_UID;
public:
    friend class IocpUnit;
    OVERLAPPED_ENTRY m_OverlappedEntry[MAX_ENTRIES_COUNT];
    MsUnorderedDictionary<Int64, LogicUnit*> m_Dict_LogicUnit;

private:
    MsUnorderedDictionary<Int64, IocpUnitPtr> m_Dict_Unit;
    MsUnorderedDictionary<Int64, IocpUnitPtr> m_Dict_TickUnit;

private:
    MsUnorderedDictionary<LPVOID, MsUnorderedDictionary<Int64, MsUnorderedDictionary<DWORD, SafePtr<MsTimer>>>> m_Dict_IocpUnitTimer;
    MsMulDictionary<Int64, MsTimer*> m_Dict_SortTimers;

public:
    void SetTimer(IocpUnit* xIocpUnit, DWORD xEventId, Int32 xInterval, Int64 xParam = 0, Boolean xInstantlyTrigger = False);
    void KillTimer(IocpUnit* xIocpUnit, DWORD xEventId, Int64 xParam = 0);
private:

public:
    Boolean m_CloseAll;
    Boolean m_IsClose;
    Boolean m_IsClosed;
    Boolean m_IsNodeRun;
    DWORD m_SelfThreadId;
    HANDLE m_hIoCompletionPort;

    Int64 GetNextUnitIndex() { return m_UnitNextIndex++; }
    Int64 m_UnitNextIndex;

    Int64   m_LastUpdatePressureTime;       // 最后更新压力时间
    Int64   m_LastUpdateClusterDBInfoTime;  // 最后更新信息到数据库时间
    Int64   m_CPU_Pressure;                 // 服务CPU压力
    //Int64   m_Pressure;                     // 服务Memory压力
    MsLock m_LockFastCluster;

    //MsList<LPWEB_OL>    m_ListWebol;
    MsClusterNetClient* m_ClusterNetClient;
    MsClusterNetServer* m_ClusterNetServer;

    MsUnorderedDictionary<Int64, MsClusterTerminal*> m_Dict_ServerNode;
    MsUnorderedDictionary<Int64, MsClusterTerminal*> m_Dict_ClientNode;

    WORD GetCenterCount() { return (WORD)m_ListCenterIntAddr.GetCount(); }
    mstr GetCenterAddrByUnitIndex(Int64 xUnitIndex)
    {
        WORD xPortA, xPortB;
        Int32 xCenterIndex = abs(xUnitIndex) % GetCenterCount();
        return MsBase::Int2IPPP(m_ListCenterIntAddr[xCenterIndex], xPortA, xPortB);
    }
    WORD GetCenterPortByUnitIndex(Int64 xUnitIndex)
    {
        WORD xPortA, xPortB;
        Int32 xCenterIndex = abs(xUnitIndex) % GetCenterCount();
        MsBase::Int2IPPP(m_ListCenterIntAddr[xCenterIndex], xPortA, xPortB);
        return xPortA;
    }
    MsList<Int64> m_ListCenterIntAddr;

    // 内部配置
    mstr    m_InsideAddr;
    WORD    m_InsideNetPort;
    Int64   m_InsideIntAddr;

    Int32  m_NodeIndex;
}*LPMSIOCPMANAGER;

class MsThreadIOCPManager : public MsThread, public MsIOCPManager
{
public:
    DINLINE MsThreadIOCPManager(mstr xThreadName, Int32 xTickInterval, Boolean IsAutoRun)
        : MsThread(xThreadName, MsThreadIOCPManagerProc, this, xTickInterval, IsAutoRun, False)
    {
        m_SelfThreadId = m_dwThreadId;
    }

public:
    inline static void MsThreadIOCPManagerProc(LPVOID lpThreadParameter) { ((MsThreadIOCPManager*)lpThreadParameter)->TickOnce(); }
};

