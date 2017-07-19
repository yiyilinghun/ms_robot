#pragma once
#include "MsIOCP_OL.h"

template<typename T>
class UnitRoute
{
public:
    MsUnorderedDictionary<DWORD, std::function<void(iocp_base_ol*)>> m_FunLamtaId;
    MsUnorderedDictionary<mstr, std::function<void(iocp_base_ol*)>> m_FunLamtaStr;

    typedef Boolean(T::*LPEXEFUN)(iocp_base_ol*);
    void RegisterPacket(DWORD xKey, LPEXEFUN xFun)
    {
        m_FunExeId[xKey] = xFun;
    }
    void RegisterPacket(mstr xKey, LPEXEFUN xFun)
    {
        m_FunExeStr[xKey] = xFun;
    }
    Boolean ExecutePacket(T* xIocpUnit, DWORD xKey, iocp_base_ol* lpIocpol)
    {
        if (m_FunExeId.ContainsKey(xKey))
        {
            return (xIocpUnit->*m_FunExeId[xKey])(lpIocpol);
        }
        else if (m_FunLamtaId.ContainsKey(xKey))
        {
            m_FunLamtaId[xKey](lpIocpol);
            return True;
        }
        else
        {
            return xIocpUnit->DefaultExecutePacket(xKey, lpIocpol);
        }
    }
    Boolean ExecutePacket(T* xIocpUnit, mstr xKey, iocp_base_ol* lpIocpol)
    {
        if (m_FunExeStr.ContainsKey(xKey))
        {
            return (xIocpUnit->*m_FunExeStr[xKey])(lpIocpol);
        }
        else if (m_FunLamtaStr.ContainsKey(xKey))
        {
            m_FunLamtaStr[xKey](lpIocpol);
            return True;
        }
        else
        {
            return xIocpUnit->DefaultExecutePacket(xKey, lpIocpol);
        }
    }
private:
    MsUnorderedDictionary<DWORD, LPEXEFUN> m_FunExeId;
    MsUnorderedDictionary<mstr, LPEXEFUN> m_FunExeStr;
};

inline UnitRoute<LogicUnit>* GetIocpUnitRoute(E_ROUTE_TYPE xType);
class LogicUnit
{
public:
    E_ROUTE_TYPE m_IocpUnitType = E_ROUTE_TYPE::ERT_LogicUnit;

    inline UnitRoute<LogicUnit>* GetRoute() { return GetIocpUnitRoute(m_IocpUnitType); };
    LogicUnit(mstr xUnitName, MsIOCPManager* xMsIOCPManager, Int64 xUnitIndex)
        : m_UnitIndex(xUnitIndex)
        , m_MsIOCPManager(xMsIOCPManager)
        , m_UnitName(xUnitName) {}

    virtual ~LogicUnit() {}

    virtual void OnMsg(OVERLAPPED_ENTRY& xEntry)
    {
        iocp_base_ol* xIocpBaseol = (iocp_base_ol*)xEntry.lpOverlapped;
        if (xIocpBaseol->m_Type == IOCP_OL_TYPE::IOT_DIY)
        {
            iocp_diy_ol* xIocpDiyol = (iocp_diy_ol*)xEntry.lpOverlapped;
            GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xIocpDiyol->m_EventId, xIocpDiyol);
        }
        else
        {
            AssertLog("没有消息处理函数");
        }
    }

    virtual Boolean DefaultExecutePacket(DWORD xKey, iocp_base_ol* lpIocpol)
    {
        if ((*MsBase::m_lpDebugPacketCode).ContainsKey(xKey))
        {
            AssertLog("IocpUnit::DefaultExecutePacket[%s],DWORD", (*MsBase::m_lpDebugPacketCode)[xKey].c_str());
        }
        else
        {
            AssertLog("IocpUnit::DefaultExecutePacket[%08x],DWORD", xKey);
        }
        return True;
    }
    virtual Boolean DefaultExecutePacket(mstr xKey, iocp_base_ol* lpIocpol)
    {
        AssertLog("IocpUnit::DefaultExecutePacket[%s],mstr", xKey.c_str());
        return True;
    }
    Int64 m_UnitIndex;
    mstr m_UnitName;
    MsIOCPManager* m_MsIOCPManager;
};

class IocpUnit : public LogicUnit
{
public:
    IocpUnit(mstr xUnitName, MsIOCPManager* xMsIOCPManager, Int64 xUnitIndex = INVALID_LID);
    virtual ~IocpUnit();

public:

    virtual IntPtr GetHandle() { return nullptr; }
    virtual Boolean TickOnce();
    virtual Boolean PostMsg(iocp_base_ol* xIocpol, DWORD dwParam);

    Boolean OnTimer(MsTimer& xTimer);
    virtual Boolean OnNewPacket(DWORD xMsgType, iocp_packet_ol* xIocpol);

    virtual void OnAttach() { ; }

    void SetTimer(DWORD dwEventId, Int32 xInterval, Int64 xParam = 0, Boolean xInstantlyTrigger = False);
    void KillTimer(DWORD dwEventId);
    void CreateDirectTimer(DWORD dwEventId, Int32 xInterval, Int64 xParam, Boolean xInstantlyTrigger, UnitRoute<LogicUnit>::LPEXEFUN xFun);

    // xForwardKey默认INVALID_LID时,使用本身单元索引
    void SendPacketForward(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xForwardKey = INVALID_LID, Boolean IsLastSend = False);

    // xUnitKey默认INVALID_LID时,使用本身单元索引
    void SendPacketToUnit(Int64 xIntAddr, BaseProto& xBaseProto, Int64 xUnitKey = INVALID_LID, Boolean IsLastSend = False);

public:
    // 副索引
    MsList<Int64>& GetDeputyUnitIndex() { return m_DeputyUnitIndex; }
    MsList<Int64> m_DeputyUnitIndex;
};

struct AutoRegExeFunIocpUnitAssist
{
    inline AutoRegExeFunIocpUnitAssist(E_ROUTE_TYPE xType, DWORD xKey, UnitRoute<LogicUnit>::LPEXEFUN xFun, UnitRoute<LogicUnit>* xIocpUnitRoute)
    {
        GetIocpUnitRoute(xType)->RegisterPacket(xKey, xFun);
    }
    inline AutoRegExeFunIocpUnitAssist(E_ROUTE_TYPE xType, mstr xKey, UnitRoute<LogicUnit>::LPEXEFUN xFun, UnitRoute<LogicUnit>* xIocpUnitRoute)
    {
        GetIocpUnitRoute(xType)->RegisterPacket(xKey, xFun);
    }
};
