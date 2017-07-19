#include <Precompiled.h>

MsNetTerminal* MsNetClient::BlockCreateClient(mstr xAddr, UInt16 xPort, MsIOCPManager* xMsIOCPManager)
{
    MsNetTerminal* xMsNetTerminal = this->GetNewNetTerminal(xMsIOCPManager);
    if (xMsNetTerminal)
    {
        MsNetClient::BlockCreateClient(xAddr, xPort, xMsNetTerminal);
    }
    return xMsNetTerminal;
}

Boolean MsNetClient::BlockCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal* xMsNetTerminal)
{
    WSADATA xWsaData;
    if (xMsNetTerminal && NO_ERROR == ::WSAStartup(MAKEWORD(2, 2), &xWsaData))
    {
        if (!xMsNetTerminal->IsValid())
        {
            SOCKET xClientSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
            if (INVALID_SOCKET != xClientSocket)
            {
                xMsNetTerminal->m_Socket = xClientSocket;
            }
        }
        sockaddr_in xSockaddrIn;
        xSockaddrIn.sin_family = AF_INET;
        Int32 xAddrLen = sizeof(xSockaddrIn);
        if (!MsBase::MsInetPtonA(xAddr, xSockaddrIn, xAddrLen))
        {
            xAddr = MsBase::GetIPFromHost(xAddr.c_str()).c_str();
        }
        struct timeval xOptval = { 1, 0 };
        socklen_t len = sizeof(xOptval);
        ::setsockopt(xMsNetTerminal->m_Socket, SOL_SOCKET, SO_RCVTIMEO, (LPCSTR)&xOptval, len);

        struct linger xLinger { 0, 0 };
        ::setsockopt(xMsNetTerminal->m_Socket, SOL_SOCKET, SO_LINGER, (LPCSTR)&xLinger, len);

        Int32 xFlag = 1;
        len = sizeof(xFlag);
        ::setsockopt(xMsNetTerminal->m_Socket, SOL_SOCKET, SO_REUSEADDR, (LPCSTR)&xFlag, len);
        if (MsBase::MsInetPtonA(xAddr, xSockaddrIn, xAddrLen))
        {
            xSockaddrIn.sin_port = ::htons(xPort);
            if (SOCKET_ERROR != ::connect(xMsNetTerminal->m_Socket, (LPSOCKADDR)&xSockaddrIn, sizeof(xSockaddrIn)))
            {
                // 设置套接字为非阻塞模式
                ULONG IsNoEncumber = TRUE;
                if (SOCKET_ERROR != ::ioctlsocket(xMsNetTerminal->m_Socket, FIONBIO, &IsNoEncumber))
                {
                    xMsNetTerminal->m_MsIOCPManager->AttachObject(xMsNetTerminal);
                }
                return True;
            }
        }
    }
    return False;
}


Boolean MsNetClient::BlockCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal& xMsNetTerminal)
{
    WSADATA xWsaData;
    if (NO_ERROR == ::WSAStartup(MAKEWORD(2, 2), &xWsaData))
    {
        if (!xMsNetTerminal.IsValid())
        {
            SOCKET xClientSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
            if (INVALID_SOCKET != xClientSocket)
            {
                xMsNetTerminal.m_Socket = xClientSocket;
            }
        }
        sockaddr_in xSockaddrIn;
        xSockaddrIn.sin_family = AF_INET;
        Int32 xAddrLen = sizeof(xSockaddrIn);
        if (!MsBase::MsInetPtonA(xAddr, xSockaddrIn, xAddrLen))
        {
            xAddr = MsBase::GetIPFromHost(xAddr.c_str()).c_str();
        }
        struct timeval xOptval = { 1, 0 };
        socklen_t len = sizeof(xOptval);
        ::setsockopt(xMsNetTerminal.m_Socket, SOL_SOCKET, SO_RCVTIMEO, (LPCSTR)&xOptval, len);

        struct linger xLinger { 0, 0 };
        ::setsockopt(xMsNetTerminal.m_Socket, SOL_SOCKET, SO_LINGER, (LPCSTR)&xLinger, len);

        Int32 xFlag = 1;
        len = sizeof(xFlag);
        ::setsockopt(xMsNetTerminal.m_Socket, SOL_SOCKET, SO_REUSEADDR, (LPCSTR)&xFlag, len);
        if (MsBase::MsInetPtonA(xAddr, xSockaddrIn, xAddrLen))
        {
            xSockaddrIn.sin_port = ::htons(xPort);
            if (SOCKET_ERROR != ::connect(xMsNetTerminal.m_Socket, (LPSOCKADDR)&xSockaddrIn, sizeof(xSockaddrIn)))
            {
                // 设置套接字为非阻塞模式
                ULONG IsNoEncumber = TRUE;
                if (SOCKET_ERROR != ::ioctlsocket(xMsNetTerminal.m_Socket, FIONBIO, &IsNoEncumber))
                {
                    xMsNetTerminal.m_MsIOCPManager->AttachObject(xMsNetTerminal);
                }
                return True;
            }
        }
    }
    return False;
}


MsNetTerminal* MsNetClient::TryCreateClient(mstr xAddr, UInt16 xPort, MsIOCPManager* xMsIOCPManager)
{
    MsNetTerminal* xMsNetTerminal = this->GetNewNetTerminal(xMsIOCPManager);
    if (xMsNetTerminal)
    {
        MsNetClient::TryCreateClient(xAddr, xPort, xMsNetTerminal);
    }
    return xMsNetTerminal;
}

Boolean MsNetClient::TryCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal* xMsNetTerminal)
{
    WSADATA xWsaData;
    if (xMsNetTerminal && NO_ERROR == ::WSAStartup(MAKEWORD(2, 2), &xWsaData))
    {
        if (!xMsNetTerminal->IsValid())
        {
            SOCKET xClientSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
            if (INVALID_SOCKET != xClientSocket)
            {
                // 设置套接字为非阻塞模式
                ULONG IsNoEncumber = TRUE;
                if (SOCKET_ERROR != ::ioctlsocket(xClientSocket, FIONBIO, &IsNoEncumber))
                {
                    xMsNetTerminal->m_Socket = xClientSocket;
                }
            }
        }

        sockaddr_in xSockaddr_in;
        xSockaddr_in.sin_family = AF_INET;
        if (!::InetPtonA(AF_INET, xAddr.c_str(), &xSockaddr_in.sin_addr.s_addr))
        {
            xAddr = MsBase::GetIPFromHost(xAddr.c_str()).c_str();
        }
        if (TRUE == ::InetPtonA(AF_INET, xAddr.c_str(), &xSockaddr_in.sin_addr.s_addr))
        {
            xSockaddr_in.sin_port = ::htons(xPort);
            if (SOCKET_ERROR != ::connect(xMsNetTerminal->m_Socket, (LPSOCKADDR)&xSockaddr_in, sizeof(xSockaddr_in)))
            {
                xMsNetTerminal->m_MsIOCPManager->AttachObject(xMsNetTerminal);
                return True;
            }
            else
            {
                switch (::WSAGetLastError())
                {
                    case WSAEWOULDBLOCK:
                    case WSAEALREADY:
                    {
                    }break;

                    case WSAEISCONN:
                    {
                        xMsNetTerminal->m_MsIOCPManager->AttachObject(xMsNetTerminal);
                        return True;
                    }break;
                }
            }
        }
    }
    return False;
}

