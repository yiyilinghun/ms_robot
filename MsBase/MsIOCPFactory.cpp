#include "Precompiled.h"

void MsIOCPFactory::RegisterPacket(DWORD xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc, LPCSTR xProcName)
{
    m_Dict_NumIocpUnitMsgProcName.Add(xMsgType, xProcName);
    m_Dict_NumIocpUnitMsgProc.Add(xMsgType, lpOnIocpUnitMsgProc);
}

Boolean MsIOCPFactory::ExecutePacket(IocpUnit* lpIocpUnit, DWORD xMsgType, iocp_ol* lpIocpol)
{
    if (m_Dict_NumIocpUnitMsgProc.ContainsKey(xMsgType))
    {
        if (!(lpIocpUnit->*(m_Dict_NumIocpUnitMsgProc[xMsgType]))(lpIocpol))
        {
            AssertLog("[%s]OnNewPacket Failed!", m_Dict_NumIocpUnitMsgProcName[xMsgType].c_str());
            return False;
        }
        return True;
    }
    else
    {
        AssertLog("[0x%08X]Î´×¢²á!", xMsgType);
        return False;
    }
}

void MsIOCPFactory::RegisterPacket(mstr xMsgType, LP_IOCP_UNIT_MSG_PROC lpOnIocpUnitMsgProc)
{
    m_Dict_strIocpUnitMsgProc.Add(xMsgType, lpOnIocpUnitMsgProc);
}
Boolean MsIOCPFactory::ExecutePacket(IocpUnit* lpIocpUnit, mstr xMsgType, iocp_ol* lpIocpol)
{
    if (m_Dict_strIocpUnitMsgProc.ContainsKey(xMsgType))
    {
        return (lpIocpUnit->*(m_Dict_strIocpUnitMsgProc[xMsgType]))(lpIocpol);
    }
    else
    {
        AssertLog("[%s]Î´×¢²á!", xMsgType.c_str());
        return False;
    }
}
