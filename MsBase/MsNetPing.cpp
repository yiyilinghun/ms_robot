#include <Precompiled.h>

MsNetPing::MsNetPing()
    : m_bIsInitSucc(FALSE)
    , m_usPacketSeq(0)

{
    WSADATA WSAData;
    WSAStartup(MAKEWORD(1, 1), &WSAData);

    m_event = WSACreateEvent();
    m_usCurrentProcID = (USHORT)GetCurrentProcessId();

    if ((m_sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, nullptr, 0, 0)) != SOCKET_ERROR)
    {
        WSAEventSelect(m_sockRaw, m_event, FD_READ);
        m_bIsInitSucc = TRUE;

        //m_szICMPData = NEW Byte[DEF_PACKET_SIZE + sizeof(ICMPHeader)];

        //if (m_szICMPData == nullptr)
        //{
        //    m_bIsInitSucc = FALSE;
        //}
    }
}

MsNetPing::~MsNetPing()
{
    ::closesocket(m_sockRaw);
    //WSACleanup();
    //if (nullptr != m_szICMPData)
    //{
        //free(m_szICMPData);
        //m_szICMPData = nullptr;
    //}
}

BOOL MsNetPing::CreatePing(DWORD dwDestIP, DWORD dwTimeout)
{
    return PingCore(dwDestIP, dwTimeout);
}

BOOL MsNetPing::CreatePing(const char *szDestIP, DWORD dwTimeout)
{
    if (nullptr != szDestIP)
    {
        sockaddr_in xSockaddrIn;
        xSockaddrIn.sin_family = AF_INET;
        Int32 xAddrLen = sizeof(xSockaddrIn);
        if (!MsBase::MsInetPtonA(szDestIP, xSockaddrIn, xAddrLen))
        {
            szDestIP = MsBase::GetIPFromHost(szDestIP).c_str();
        }

        return PingCore(inet_addr(szDestIP), dwTimeout);
    }
    return FALSE;
}

BOOL MsNetPing::Ping(DWORD dwDestIP, DWORD dwTimeout)
{
    PingCore(dwDestIP, dwTimeout);
    BOOL xIsTimeOut = FALSE;
    while (xIsTimeOut)
    {
        if (this->CheckRecv(xIsTimeOut))
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MsNetPing::Ping(const char *szDestIP, DWORD dwTimeout)
{
    if (nullptr != szDestIP)
    {
        if (!PingCore(inet_addr(szDestIP), dwTimeout))
        {
            BOOL xIsTimeOut = FALSE;
            while (!xIsTimeOut)
            {
                if (this->CheckRecv(xIsTimeOut))
                {
                    return TRUE;
                }
            }
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MsNetPing::CheckRecv(BOOL& xIsTimeOut)
{
    char recvbuf[256] = { "\0" };
    //接收响应报文
    if (WSAWaitForMultipleEvents(1, &m_event, FALSE, 0, FALSE) != WSA_WAIT_TIMEOUT)
    {
        WSANETWORKEVENTS netEvent;
        WSAEnumNetworkEvents(m_sockRaw, m_event, &netEvent);

        if (netEvent.lNetworkEvents & FD_READ)
        {
            ULONG nRecvTimestamp = GetTickCountCalibrate();
            int nPacketSize = recvfrom(m_sockRaw, recvbuf, 256, 0, (struct sockaddr*)&m_sockaddrDest, &m_nSockaddrDestSize);
            if (nPacketSize != SOCKET_ERROR)
            {
                IPHeader *pIPHeader = (IPHeader*)recvbuf;
                USHORT usIPHeaderLen = (USHORT)((pIPHeader->m_byVerHLen & 0x0f) * 4);
                ICMPHeader *pICMPHeader = (ICMPHeader*)m_szICMPData;
                pICMPHeader = (ICMPHeader*)(recvbuf + usIPHeaderLen);

                if (pICMPHeader->m_usID == m_usCurrentProcID //是当前进程发出的报文
                    && pICMPHeader->m_byType == ECHO_REPLY //是ICMP响应报文
                    && pICMPHeader->m_usSeq == m_usSeq //是本次请求报文的响应报文
                    )
                {
                    m_PingReply.m_usSeq = m_usSeq;
                    m_PingReply.m_dwRoundTripTime = nRecvTimestamp - pICMPHeader->m_ulTimeStamp;
                    m_PingReply.m_dwBytes = nPacketSize - usIPHeaderLen - sizeof(ICMPHeader);
                    m_PingReply.m_dwTTL = pIPHeader->m_byTTL;
                    return TRUE;
                }
            }
        }
    }
    //超时
    if (GetTickCountCalibrate() - m_ulSendTimestamp >= m_dwTimeout)
    {
        xIsTimeOut = TRUE;
        return FALSE;
    }
    return FALSE;
}

BOOL MsNetPing::PingCore(DWORD dwDestIP, DWORD dwTimeout)
{
    m_dwTimeout = dwTimeout;

    //判断初始化是否成功
    if (!m_bIsInitSucc)
    {
        return FALSE;
    }

    //配置SOCKET
    m_sockaddrDest.sin_family = AF_INET;
    m_sockaddrDest.sin_addr.s_addr = dwDestIP;
    m_nSockaddrDestSize = sizeof(m_sockaddrDest);

    //构建ICMP包
    int nICMPDataSize = DEF_PACKET_SIZE + sizeof(ICMPHeader);
    m_ulSendTimestamp = GetTickCountCalibrate();
    m_usSeq = ++m_usPacketSeq;
    memset(m_szICMPData, 0, nICMPDataSize);
    ICMPHeader *pICMPHeader = (ICMPHeader*)m_szICMPData;
    pICMPHeader->m_byType = ECHO_REQUEST;
    pICMPHeader->m_byCode = 0;
    pICMPHeader->m_usID = m_usCurrentProcID;
    pICMPHeader->m_usSeq = m_usSeq;
    pICMPHeader->m_ulTimeStamp = m_ulSendTimestamp;
    pICMPHeader->m_usChecksum = CalCheckSum((USHORT*)m_szICMPData, nICMPDataSize);

    //发送ICMP报文
    if (sendto(m_sockRaw, m_szICMPData, nICMPDataSize, 0, (struct sockaddr*)&m_sockaddrDest, m_nSockaddrDestSize) == SOCKET_ERROR)
    {
        return FALSE;
    }

    BOOL xIsTimeOut;
    return this->CheckRecv(xIsTimeOut);
}

USHORT MsNetPing::CalCheckSum(USHORT *pBuffer, int nSize)
{
    unsigned long ulCheckSum = 0;
    while (nSize > 1)
    {
        ulCheckSum += *pBuffer++;
        nSize -= sizeof(USHORT);
    }
    if (nSize)
    {
        ulCheckSum += *(UCHAR*)pBuffer;
    }

    ulCheckSum = (ulCheckSum >> 16) + (ulCheckSum & 0xffff);
    ulCheckSum += (ulCheckSum >> 16);

    return (USHORT)(~ulCheckSum);
}

ULONG MsNetPing::GetTickCountCalibrate()
{
    static ULONG s_ulFirstCallTick = 0;
    static LONGLONG s_ullFirstCallTickMS = 0;

    SYSTEMTIME systemtime;
    FILETIME filetime;
    GetLocalTime(&systemtime);
    SystemTimeToFileTime(&systemtime, &filetime);
    LARGE_INTEGER liCurrentTime;
    liCurrentTime.HighPart = filetime.dwHighDateTime;
    liCurrentTime.LowPart = filetime.dwLowDateTime;
    LONGLONG llCurrentTimeMS = liCurrentTime.QuadPart / 10000;

    if (s_ulFirstCallTick == 0)
    {
        s_ulFirstCallTick = GetTickCount();
    }
    if (s_ullFirstCallTickMS == 0)
    {
        s_ullFirstCallTickMS = llCurrentTimeMS;
    }

    return s_ulFirstCallTick + (ULONG)(llCurrentTimeMS - s_ullFirstCallTickMS);
}
