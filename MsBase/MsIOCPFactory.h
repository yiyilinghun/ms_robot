#pragma once

// 完成端口工厂
typedef Boolean(IocpUnit::*LP_IOCP_UNIT_MSG_PROC)(iocp_ol* lpIocpol);
class MsIOCPFactory
{
public:
    void RegisterPacket(DWORD xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc, LPCSTR xProcName);
    Boolean ExecutePacket(IocpUnit* lpIocpUnit, DWORD xMsgType, iocp_ol* lpIocpol);

    void RegisterPacket(mstr xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc);
    Boolean ExecutePacket(IocpUnit* lpIocpUnit, mstr xMsgType, iocp_ol* lpIocpol);

    MsDictionary<DWORD, mstr> m_Dict_NumIocpUnitMsgProcName;
    MsDictionary<DWORD, LP_IOCP_UNIT_MSG_PROC> m_Dict_NumIocpUnitMsgProc;
    MsDictionary<mstr, LP_IOCP_UNIT_MSG_PROC> m_Dict_strIocpUnitMsgProc;
};
struct MsIOCPFactoryRegisterAssist
{
    MsIOCPFactoryRegisterAssist(MsIOCPFactory* xMsIOCPFactory, DWORD xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc, LPCSTR xProcName)
    {
        xMsIOCPFactory->RegisterPacket(xMsgType, lpOnIocpUnitMsgProc, xProcName);
    }

    MsIOCPFactoryRegisterAssist(MsIOCPFactory* xMsIOCPFactory, mstr xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc)
    {
        xMsIOCPFactory->RegisterPacket(xMsgType, lpOnIocpUnitMsgProc);
    }
};
