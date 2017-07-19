#include "Precompiled.h"
#include "Mstcpip.h"

MsNetServer::MsNetServer(MsIOCPManager* xMsIOCPManager, Int64 xUnitIndex)
    : IocpUnit("MsNetServer", xMsIOCPManager, xUnitIndex)
    , m_ServerSocket(INVALID_SOCKET)
    , m_lpfnAcceptEx(nullptr)
    , m_lpfnGetAcceptExSockAddrs(nullptr)
{
    DEF_IOCPUNIT_TYPE(MsNetServer);

    WSADATA xWsaData;
    AssertNormal(NO_ERROR == ::WSAStartup(MAKEWORD(2, 2), &xWsaData), "Initialize Server Socket Failed");

    m_ServerSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    AssertNormal(INVALID_SOCKET != m_ServerSocket, "Initialize Server Socket Failed");

    Int32 xFlag = 1;
    socklen_t len = sizeof(xFlag);
    ::setsockopt(m_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (LPCSTR)&xFlag, len);

    struct linger xLinger { 0, 0 };
    ::setsockopt(m_ServerSocket, SOL_SOCKET, SO_LINGER, (LPCSTR)&xLinger, len);

    // 设置套接字为非阻塞模式
    ULONG IsNoEncumber = TRUE;
    AssertNormal(SOCKET_ERROR != ::ioctlsocket(m_ServerSocket, FIONBIO, &IsNoEncumber), "Initialize Server Socket Failed");
}

MsNetServer::~MsNetServer()
{
    if (m_ServerSocket != INVALID_SOCKET)
    {
        ::closesocket(m_ServerSocket);
        m_ServerSocket = INVALID_SOCKET;
    }
}

Boolean MsNetServer::ServerListen(LPCSTR xAddr, UInt16 xPort)
{
    if (ServerBind(xAddr, xPort))
    {
        if (SOCKET_ERROR != ::listen(m_ServerSocket, MAX_LISTEN_COUNT))
        {
            return True;
        }
    }
    return False;
}

mstr MsNetServer::GetBindIPAddr()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getsockname(m_ServerSocket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::inet_ntoa(s->sin_addr);
        }
    }
    return "";
}

WORD MsNetServer::GetBindPort()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getsockname(m_ServerSocket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::ntohs(s->sin_port);
        }
    }
    return 0xFFFF;
}


IntPtr MsNetServer::GetHandle()
{
    return (IntPtr)m_ServerSocket;
}

Boolean MsNetServer::TickOnce()
{
    CheckAccept();
    return True;
}

void MsNetServer::OnAttach()
{
    ;
}

//void MsNetServer::OnDetach()
//{
//    if (m_ServerSocket != INVALID_SOCKET)
//    {
//        ::closesocket(m_ServerSocket);
//        m_ServerSocket = INVALID_SOCKET;
//    }
//}

void MsNetServer::CheckAccept()
{
    for (Int32 i = 0; i < MAX_LISTEN_COUNT; i++)
    {
        SOCKADDR xSockAddr;
        Int32 xSAlen = sizeof(xSockAddr);
        SOCKET xAcceptSocket = ::accept(m_ServerSocket, &xSockAddr, &xSAlen);
        if (xAcceptSocket != INVALID_SOCKET)
        {
            Int32 xFlag = 1;
            socklen_t len = sizeof(xFlag);
            ::setsockopt(xAcceptSocket, SOL_SOCKET, SO_REUSEADDR, (LPCSTR)&xFlag, len);

            struct linger xLinger { 0, 0 };
            ::setsockopt(xAcceptSocket, SOL_SOCKET, SO_LINGER, (LPCSTR)&xLinger, len);

            //Int32 val = 1;
            //if (setsockopt(xAcceptSocket, SOL_SOCKET, SO_KEEPALIVE, (LPCSTR)&val, sizeof(val)) == -1)
            //{
            //    AssertLog("setsockopt SO_KEEPALIVE: %d", ::GetLastError());
            //    closesocket(xAcceptSocket);
            //    continue;
            //}

            //DWORD bytesnum;
            //struct tcp_keepalive keepin;
            //struct tcp_keepalive keepout;
            //keepin.keepalivetime = 5 * 1000;
            //keepin.keepaliveinterval = 1000;
            //keepin.onoff = 1;
            //::WSAIoctl(xAcceptSocket, SIO_KEEPALIVE_VALS, &keepin, sizeof(keepin), &keepout, sizeof(keepout), &bytesnum, nullptr, nullptr);

            MsNetTerminal* xMsNetTerminal = this->GetNewNetTerminal(xAcceptSocket);
            if (xMsNetTerminal)
            {
                xMsNetTerminal->m_MsIOCPManager->AttachObject(xMsNetTerminal);
            }
        }
        else
        {
            break;
        }
    }
}

Boolean MsNetServer::ServerBind(LPCSTR xAddr, UInt16 xPort)
{
    sockaddr_in xSockaddr_in;
    xSockaddr_in.sin_family = AF_INET;
    if (TRUE == ::InetPtonA(AF_INET, xAddr, &xSockaddr_in.sin_addr.s_addr))
    {
        xSockaddr_in.sin_port = ::htons(xPort);
        if (SOCKET_ERROR != ::bind(m_ServerSocket, (LPSOCKADDR)&xSockaddr_in, sizeof(xSockaddr_in)))
        {
            return True;
        }
    }
    return False;
}
