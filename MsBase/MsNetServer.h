#pragma once

#define ANY_NET_ADDR            "0.0.0.0"
#define ACCEPT_TIME_OUT         100
#define MAX_LISTEN_COUNT        8192
#define ECOT_MSNETSERVER_TYPEID 0x00000003

class MsNetServer : public IocpUnit
{
public:
    MsNetServer(MsIOCPManager* xMsIOCPManager, Int64 xUnitIndex);
    virtual ~MsNetServer();
    Boolean ServerListen(LPCSTR xAddr = ANY_NET_ADDR, UInt16 xPort = 0);
    mstr GetBindIPAddr();
    WORD GetBindPort();

public:
    virtual IntPtr GetHandle();
    virtual MsNetTerminal* GetNewNetTerminal(SOCKET xAcceptSocket)
    {
        return NEW MsNetTerminal(m_MsIOCPManager, True, xAcceptSocket);
    }

    virtual Boolean TickOnce();
    virtual void OnAttach();

protected:
    void CheckAccept();
    Boolean ServerBind(LPCSTR xAddr, UInt16 xPort);

    SOCKET m_ServerSocket;
    LPFN_ACCEPTEX m_lpfnAcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
};
