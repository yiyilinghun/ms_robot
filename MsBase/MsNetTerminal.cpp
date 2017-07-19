#include "Precompiled.h"

//MsNetTerminal::MsNetTerminal(MsClusterNode* xMsClusterManager, MsIOCPFactory* xMsIOCPFactory, Boolean IsInServer, SOCKET xSocket, Int64 xUnitIndex)
//    : IocpUnit("MsNetTerminal", xMsClusterManager, xMsIOCPFactory, xUnitIndex)
//    , m_LastCommunicationTime(0)
//    , m_IsUTF8(False)
//    , m_Socket(xSocket)
//    , m_IsLastSend(False)
//    , m_IsReadySend(False)
//    , m_IsConnect(False)
//    , m_IsExeClose(True)
//    , m_IsInServer(IsInServer)
//    , m_RecvExitOL(MS_NET_TERMINAL_OL_TYPE::RECV_OL, 0, 0)
//{
//    ;
//}

MsNetTerminal::MsNetTerminal(MsIOCPManager* xMsIOCPManager, Boolean IsInServer, SOCKET xSocket, Int64 xUnitIndex)
    : IocpUnit("MsNetTerminal", xMsIOCPManager, xUnitIndex)
    , m_LastCommunicationTime(0)
    , m_IsUTF8(False)
    , m_Socket(xSocket)
    , m_IsLastSend(False)
    , m_IsReadySend(False)
    , m_IsConnect(False)
    , m_IsExeClose(True)
    , m_IsInServer(IsInServer)
    , m_RecvExitOL(MS_NET_TERMINAL_OL_TYPE::RECV_OL, 0, 0)
{
    DEF_IOCPUNIT_TYPE(MsNetTerminal);
}

MsNetTerminal::~MsNetTerminal()
{
    //m_ListPacketSending.Clear();
    //m_SetRecvOL.Clear();
    //m_SetSendOL.Clear();
    //m_QueuePacket.Clear();
    SAFE_ENUMERATOR_DELETE(m_ListPacketSending);
    SAFE_ENUMERATOR_DELETE(m_SetRecvOL);
    SAFE_ENUMERATOR_DELETE(m_SetSendOL);
    SAFE_ENUMERATOR_DELETE(m_QueuePacket);
}

void MsNetTerminal::OnAttach()
{
    if (m_IsInServer)
    {
        this->OnAccept();
    }
    else
    {
        this->OnConnect();
    }

    m_IsReadySend = True;
    this->GoRecv();
    this->RealSend();
}

//void MsNetTerminal::OnDetach()
//{
//    return;
//}

Boolean MsNetTerminal::TickOnce()
{
    return True;
}

void MsNetTerminal::OnMsg(OVERLAPPED_ENTRY& xEntry)
{
    iocp_base_ol* xIocpBaseol = (iocp_base_ol*)xEntry.lpOverlapped;
    if (xIocpBaseol->m_Type == IOT_NET)
    {
        iocp_net_ol* xIocpNetol = (iocp_net_ol*)xEntry.lpOverlapped;
        if (xEntry.dwNumberOfBytesTransferred == 0)
        {
            if (m_SetRecvOL.Contains(xIocpNetol))
            {
                m_SetRecvOL.Remove(xIocpNetol);
                if (xIocpNetol != &m_RecvExitOL) { SAFE_DELETE(xIocpNetol); }
            }
            if (m_SetSendOL.Contains(xIocpNetol))
            {
                m_SetSendOL.Remove(xIocpNetol);
                if (xIocpNetol != &m_RecvExitOL) { SAFE_DELETE(xIocpNetol); }
            }
            if (m_SetSending.Contains(xIocpNetol))
            {
                m_SetSending.Remove(xIocpNetol);
                if (xIocpNetol != &m_RecvExitOL) { SAFE_DELETE(xIocpNetol); }
            }
            this->Close();
            return;
        }

        if (!this->OnNetSendOrRecv(xIocpNetol, xEntry))
        {
            if (m_IsExeClose)
            {
                m_IsExeClose = False;
                this->Close();
            }
            else
            {
                AssertLog("MsNetTerminal::OnClose");
            }
        }
        if (xIocpNetol != &m_RecvExitOL) { SAFE_DELETE(xIocpNetol); }
    }
    else
    {
        IocpUnit::OnMsg(xEntry);
    }
}

Boolean MsNetTerminal::OnNetSendOrRecv(iocp_net_ol* xIocpNetol, OVERLAPPED_ENTRY& xEntry)
{
    if (xIocpNetol->m_NetType == SEND_OL)
    {
#pragma region NET_SEND
        if (xIocpNetol->m_SurplusSendDataLen == 0 && xIocpNetol->m_SumNeedSendDataLen == 0)
        {
            m_SetSendOL.Remove(xIocpNetol);
        }
        else
        {
            // 计算剩余发送
            xIocpNetol->m_SurplusSendDataLen -= xEntry.dwNumberOfBytesTransferred;
            if (xIocpNetol->m_SurplusSendDataLen == 0)
            {
                m_SetSending.Remove(xIocpNetol);
                m_SetSendOL.Remove(xIocpNetol);
            }
            // 如果没发送完成
            else
            {
                // 计算发送完毕位置
                while (m_ListPacketSending.GetCount() > 0 && xEntry.dwNumberOfBytesTransferred > 0)
                {
                    MsNetPacket* xMsNetPacket = m_ListPacketSending[0];
                    if (xEntry.dwNumberOfBytesTransferred >= xMsNetPacket->m_MsNetPacketHead.m_PacketLen + sizeof(MsNetPacketHead))
                    {
                        xEntry.dwNumberOfBytesTransferred -= xMsNetPacket->m_MsNetPacketHead.m_PacketLen + sizeof(MsNetPacketHead);
                        m_ListPacketSending.RemoveAtFirst();
                        SAFE_DELETE(xMsNetPacket);
                    }
                    else { break; }
                }

                if (m_ListPacketSending.GetCount() > 0)
                {
                    MsList<MsNetPacket*> xTempListPacketSending;
                    Int32 xCount = m_ListPacketSending.GetCount();
                    LPWSABUF xBuffs = NEW WSABUF[xCount];
                    DWORD xSumNeedSendDataLen = 0;
                    for (Int32 i = 0; i < xCount; i++)
                    {
                        MsNetPacket* xMsNetPacket = m_ListPacketSending.RemoveGetFirst();
                        if (i == 0)
                        {
                            (LPVOID&)xBuffs[i].buf = &(xMsNetPacket->m_MsNetPacketHead) + xEntry.dwNumberOfBytesTransferred;
                            xBuffs[i].len = sizeof(MsNetPacketHead) + xMsNetPacket->m_MsNetPacketHead.m_PacketLen - xEntry.dwNumberOfBytesTransferred;
                            xEntry.dwNumberOfBytesTransferred = 0;
                        }
                        else
                        {
                            (LPVOID&)xBuffs[i].buf = &(xMsNetPacket->m_MsNetPacketHead);
                            xBuffs[i].len = sizeof(MsNetPacketHead) + xMsNetPacket->m_MsNetPacketHead.m_PacketLen;
                        }
                        xSumNeedSendDataLen += sizeof(MsNetPacketHead) + xMsNetPacket->m_MsNetPacketHead.m_PacketLen;
                        xTempListPacketSending.Add(xMsNetPacket);
                    }
                    this->SendData(xBuffs, xCount, xSumNeedSendDataLen);
                    SAFE_DELETE_ARRAY(xBuffs);
                    m_ListPacketSending = xTempListPacketSending;
                }

                m_SetSending.Remove(xIocpNetol);
                if (m_SetSendOL.Contains(xIocpNetol))
                {
                    m_SetSendOL.Remove(xIocpNetol);
                }
            }
        }

        // 如果发送完毕
        if (m_SetSendOL.Empty())
        {
            SAFE_ENUMERATOR_DELETE(m_ListPacketSending);
            if (m_IsLastSend && m_IsExeClose)
            {
                for (Int32 i = 0; (i < 10) && ::shutdown(m_Socket, SD_SEND); i++)
                {
                    ::SleepEx(1, True);
                }
                this->Close();
            }
            else if (m_IsReadySend && m_SetSending.Empty() && m_ListPacketSending.Empty()) { this->RealSend(); }
        }
        return True;
#pragma endregion
    }
    else if (xIocpNetol->m_NetType == RECV_OL)
    {
#pragma region NET_RECV
        if (xIocpNetol->m_SurplusSendDataLen == 0 && xIocpNetol->m_SumNeedSendDataLen == 0)
        {
            if (m_SetRecvOL.Contains(xIocpNetol))
            {
                m_SetRecvOL.Remove(xIocpNetol);
            }
            return True;
        }
        else
        {
            xIocpNetol->m_SurplusSendDataLen -= xEntry.dwNumberOfBytesTransferred;
            //AssertLog("需要接收[%d],异步接收[%d],还需接收[%d]", lpIocpol->m_MsNet.m_SumNeedSendDataLen, xEntry.dwNumberOfBytesTransferred, lpIocpol->m_MsNet.m_SurplusSendDataLen);
            m_RecvPacket.m_dwCompleteBytes = xIocpNetol->m_SumNeedSendDataLen - xIocpNetol->m_SurplusSendDataLen;
            if (m_SetRecvOL.Contains(xIocpNetol))
            {
                m_SetRecvOL.Remove(xIocpNetol);
            }
            if (m_RecvPacket.m_dwCompleteBytes < sizeof(MsNetPacketHead))
            {
                if (this->RecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead)))
                {
                    if (m_RecvPacket.m_MsNetPacketHead.m_PacketLen == 0)
                    {
                        if (!this->OnPacket() && m_IsExeClose) { return False; }
                    }
                    else if (this->RecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead) + m_RecvPacket.m_MsNetPacketHead.m_PacketLen))
                    {
                        if (!this->OnPacket() && m_IsExeClose) { return False; }
                    }
                    else { return True; }
                }
                else { return True; }
            }
            else if (m_RecvPacket.m_dwCompleteBytes < (sizeof(MsNetPacketHead) + m_RecvPacket.m_MsNetPacketHead.m_PacketLen))
            {
                if (this->RecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead) + m_RecvPacket.m_MsNetPacketHead.m_PacketLen))
                {
                    if (!this->OnPacket() && m_IsExeClose) { return False; }
                }
                else { return True; }
            }
            else if (m_RecvPacket.m_dwCompleteBytes == (sizeof(MsNetPacketHead) + m_RecvPacket.m_MsNetPacketHead.m_PacketLen))
            {
                if (!this->OnPacket() && m_IsExeClose) { return False; }
            }
            else
            {
                AssertLog("啊???????????");
            }
        }

        while (True)
        {
            m_RecvPacket.m_dwCompleteBytes = 0;
            m_RecvPacket.m_MsNetPacketHead.m_Flag = 0;
            m_RecvPacket.m_MsNetPacketHead.m_Param = INVALID_QID;
            if (this->RecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead)))
            {
                if (m_RecvPacket.m_MsNetPacketHead.m_PacketLen == 0)
                {
                    if (!this->OnPacket() && m_IsExeClose) { return False; }
                    continue;
                }
                else if (this->RecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead) + m_RecvPacket.m_MsNetPacketHead.m_PacketLen))
                {
                    if (!this->OnPacket() && m_IsExeClose) { return False; }
                    continue;
                }
            }
            return True;
        }
        return True;
#pragma endregion
    }
    return False;
}

void MsNetTerminal::Close()
{
    if (m_Socket != INVALID_SOCKET)
    {
        ::closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
    this->PostMsg(&m_RecvExitOL, 0);
    m_IsConnect = False;
    SAFE_DETACH_THIS;
    //this->OnClose();
}

void MsNetTerminal::SendPacket(BaseProto& xBaseProto, Boolean IsLastSend)
{
    MsNetPacket xMsNetPacket;
    Int32 xPacketIndex = MsBase::GetProtoIntKey(xBaseProto);
    if (xMsNetPacket.FromProto(xPacketIndex, xBaseProto, m_IsUTF8))
    {
        SendPacket(xMsNetPacket);
        if (IsLastSend)
        {
            m_IsLastSend = True;
        }
    }
    else
    {
        AssertNormal(False, "serialize failed : %s", xBaseProto.GetTypeName().c_str());
    }
}

void MsNetTerminal::SendPacketForward(BaseProto& xBaseProto, Int64 xForwardKey, Boolean IsLastSend)
{
    MsNetPacket xMsNetPacket;
    Int32 xPacketIndex = MsBase::GetProtoIntKey(xBaseProto);
    if (xMsNetPacket.FromProto(xPacketIndex, xBaseProto, m_IsUTF8))
    {
        xMsNetPacket.AddForwardKey(xForwardKey);
        SendPacket(xMsNetPacket);
        if (IsLastSend)
        {
            m_IsLastSend = True;
        }
    }
    else
    {
        DAssertNormal(False, "不完整的PROTO");
        MsBase::ShowNotExistField(xBaseProto);
    }
}

void MsNetTerminal::SendPacketToUnit(BaseProto& xBaseProto, Int64 xUnitKey, Boolean IsLastSend)
{
    MsNetPacket xMsNetPacket;
    Int32 xPacketIndex = MsBase::GetProtoIntKey(xBaseProto);
    if (xMsNetPacket.FromProto(xPacketIndex, xBaseProto, m_IsUTF8))
    {
        xMsNetPacket.AddUnitKey(xUnitKey);
        SendPacket(xMsNetPacket);
        if (IsLastSend)
        {
            m_IsLastSend = True;
        }
    }
    else
    {
        DAssertNormal(False, "不完整的PROTO");
        MsBase::ShowNotExistField(xBaseProto);
    }
}

void MsNetTerminal::SendPacket(MsNetPacket& xMsNetPacket)
{
    if (!m_IsLastSend)
    {
        MsNetPacket* lpMsNetPacket = NEW MsNetPacket();
        memcpy(lpMsNetPacket, &xMsNetPacket, sizeof(MsNetPacket));
        m_QueuePacket.Add(lpMsNetPacket);
        if (m_IsReadySend && m_SetSending.Empty() && m_ListPacketSending.Empty())
        {
            this->RealSend();
        }
        else
        {
            AssertLog("Not RealSend![sending:%d][queue:%d]", m_SetSendOL.GetCount(), m_QueuePacket.GetCount());
        }
    }
}

void MsNetTerminal::RealSend()
{
    if (m_QueuePacket.GetCount() > 0)
    {
        Int32 xCount = m_QueuePacket.GetCount();
        if (xCount > MAX_SEND_BUFF_SIZE) { xCount = MAX_SEND_BUFF_SIZE; }
        DWORD xSumNeedSendDataLen = 0;
        for (Int32 i = 0; i < xCount; i++)
        {
            MsNetPacket* xMsNetPacket = m_QueuePacket.RemoveGetFirst();
            (LPVOID&)m_SendBuffs[i].buf = &(xMsNetPacket->m_MsNetPacketHead);
            m_SendBuffs[i].len = sizeof(MsNetPacketHead) + xMsNetPacket->m_MsNetPacketHead.m_PacketLen;
            xSumNeedSendDataLen += sizeof(MsNetPacketHead) + xMsNetPacket->m_MsNetPacketHead.m_PacketLen;
            m_ListPacketSending.Add(xMsNetPacket);
        }
        this->SendData(m_SendBuffs, xCount, xSumNeedSendDataLen);
    }
}

void MsNetTerminal::OnConnect()
{
    m_IsConnect = True;
}

void MsNetTerminal::OnAccept()
{
    m_IsConnect = True;
}

//void MsNetTerminal::OnClose()
//{
//    m_IsConnect = False;
//    this->Detach();
//    //MsNetTerminal* xMsNetTerminal = this;
//    //SAFE_DELETE(xMsNetTerminal);
//}

Boolean MsNetTerminal::OnPacket()
{
    if (m_RecvPacket.m_MsNetPacketHead.m_Flag == MNPH_FLAG_FORWARD)
    {
        MsNetTerminal* lpUnit = (MsNetTerminal*)m_MsIOCPManager->GetUnit((ULONG_PTR)m_RecvPacket.m_MsNetPacketHead.m_Param);
        m_RecvPacket.m_dwCompleteBytes = 0;
        m_RecvPacket.m_MsNetPacketHead.m_Flag = 0;
        m_RecvPacket.m_MsNetPacketHead.m_Param = INVALID_QID;
        if (lpUnit)
        {
            lpUnit->SendPacket(m_RecvPacket);
        }
        return True;
    }
    else if (m_RecvPacket.m_MsNetPacketHead.m_Flag == MNPH_FLAG_TO_UNIT_KEY)
    {
        IocpUnit* lpUnit = m_MsIOCPManager->GetUnit((ULONG_PTR)m_RecvPacket.m_MsNetPacketHead.m_Param);
        m_RecvPacket.m_dwCompleteBytes = 0;
        m_RecvPacket.m_MsNetPacketHead.m_Flag = 0;
        m_RecvPacket.m_MsNetPacketHead.m_Param = INVALID_QID;
        if (lpUnit)
        {
            iocp_packet_ol xIocpol(this, &m_RecvPacket);
            return lpUnit->OnNewPacket(m_RecvPacket.m_MsNetPacketHead.m_PacketIndex, &xIocpol);
        }
    }
    else
    {
        iocp_packet_ol xIocpol(this, &m_RecvPacket);
        m_RecvPacket.m_dwCompleteBytes = 0;
        return this->OnNewPacket(m_RecvPacket.m_MsNetPacketHead.m_PacketIndex, &xIocpol);
    }
    return False;
}

void MsNetTerminal::GoRecv()
{
    this->ForceAsynRecvData(&(m_RecvPacket.m_MsNetPacketHead), m_RecvPacket.m_dwCompleteBytes, sizeof(MsNetPacketHead));
}

mstr MsNetTerminal::GetLoaclIPAddr()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getsockname(m_Socket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::inet_ntoa(s->sin_addr);
        }
    }
    return "";
}

WORD MsNetTerminal::GetLoaclPort()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getsockname(m_Socket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::ntohs(s->sin_port);
        }
    }
    return 0xFFFF;
}

mstr MsNetTerminal::GetRemoteIPAddr()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getpeername(m_Socket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::inet_ntoa(s->sin_addr);
        }
    }
    return "";
}

WORD MsNetTerminal::GetRemotePort()
{
    sockaddr_storage sa;
    Int32 salen = sizeof(sa);
    if (::getpeername(m_Socket, (struct sockaddr*)&sa, &salen) != -1)
    {
        if (sa.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in*)&sa;
            return ::ntohs(s->sin_port);
        }
    }
    return 0xFFFF;
}

Boolean MsNetTerminal::SendData(LPWSABUF xBuffs, DWORD xBuffCount, DWORD dwSumNeedSendDataLen)
{
    if (m_Socket != INVALID_SOCKET)
    {
        iocp_net_ol* xSendol = NEW iocp_net_ol(MS_NET_TERMINAL_OL_TYPE::SEND_OL, dwSumNeedSendDataLen, dwSumNeedSendDataLen);
        m_SetSendOL.Add(xSendol);
        if (0 == ::WSASend(m_Socket, xBuffs, xBuffCount, nullptr, 0, xSendol, nullptr))
        {
            DWORD dwBytes = 0;
            DWORD dwFlags = 0;
            ::WSAGetOverlappedResult(m_Socket, xSendol, &dwBytes, False, &dwFlags);
            if (dwBytes == xSendol->m_SumNeedSendDataLen)
            {
                AssertNormal(xSendol->m_SumNeedSendDataLen > 0, "************");
                SAFE_ENUMERATOR_DELETE(m_ListPacketSending);
                xSendol->m_SurplusSendDataLen = 0;
                xSendol->m_SumNeedSendDataLen = 0;
                return True;
            }
            else
            {
                m_SetSending.Add(xSendol);
                return False;
            }
        }
        else
        {
            DWORD dwErrorCode = ::GetLastError();
            if (WSA_IO_PENDING == dwErrorCode)
            {
                m_SetSending.Add(xSendol);
                return False;
            }
            else
            {
                AssertLog("WSASend Error [%d]", dwErrorCode);
                this->Close();
                return False;
            }
        }
    }
    return False;
}

Boolean MsNetTerminal::ForceAsynRecvData(LPVOID xBeginDataAddr, DWORD& xOffset, DWORD dwNeedDataLen)
{
    if (m_Socket != INVALID_SOCKET)
    {
        WSABUF xBuf;
        xBuf.buf = ((LPSTR)xBeginDataAddr) + xOffset;
        xBuf.len = dwNeedDataLen - xOffset;
        DWORD dwBytes = 0;
        DWORD dwFlags = 0;

        iocp_net_ol* xRecvol = NEW iocp_net_ol(MS_NET_TERMINAL_OL_TYPE::RECV_OL, dwNeedDataLen, dwNeedDataLen);
        m_SetRecvOL.Add(xRecvol);
        if (0 == ::WSARecv(m_Socket, &xBuf, 1, &dwBytes, &dwFlags, xRecvol, nullptr))
        {
            return False;
        }
        else
        {
            DWORD dwErrorCode = ::GetLastError();
            if (WSA_IO_PENDING == dwErrorCode)
            {
                return False;
            }
            else
            {
                AssertLog("WSARecv Error [%d]", dwErrorCode);
                this->Close();
                return False;
            }
        }
    }
    return False;
}
Boolean MsNetTerminal::RecvData(LPVOID xBeginDataAddr, DWORD& xOffset, DWORD dwNeedDataLen)
{
    if (m_Socket != INVALID_SOCKET)
    {
        AssertNormal(xOffset <= MAX_PACKET_DATA_LEN + sizeof(MsNetPacketHead), "xOffset <= MAX_PACKET_DATA_LEN");
        AssertNormal(dwNeedDataLen <= MAX_PACKET_DATA_LEN + sizeof(MsNetPacketHead), "dwNeedDataLen <= MAX_PACKET_DATA_LEN");
        WSABUF xBuf;
        xBuf.buf = ((LPSTR)xBeginDataAddr) + xOffset;
        xBuf.len = dwNeedDataLen - xOffset;
        DWORD dwFlags = 0;

        iocp_net_ol* xRecvol = NEW iocp_net_ol(MS_NET_TERMINAL_OL_TYPE::RECV_OL, dwNeedDataLen, dwNeedDataLen - xOffset);
        m_SetRecvOL.Add(xRecvol);
        if (0 == ::WSARecv(m_Socket, &xBuf, 1, nullptr, &dwFlags, xRecvol, nullptr))
        {
            DWORD dwBytes = 0;
            ::WSAGetOverlappedResult(m_Socket, xRecvol, &dwBytes, False, &dwFlags);
            xOffset += dwBytes;
            if (xOffset == dwNeedDataLen)
            {
                xRecvol->m_SurplusSendDataLen = 0;
                xRecvol->m_SumNeedSendDataLen = 0;
                return True;
            }
            else if (dwBytes > 0)
            {
                //AssertLog("需要接收[%d],立即接收[%d],还需接收[%d]", xBuf.len, dwBytes, xBuf.len - dwBytes);
                return False;
            }
            else
            {
                return False;
            }
        }
        else
        {
            DWORD dwErrorCode = ::GetLastError();
            if (WSA_IO_PENDING == dwErrorCode)
            {
                return False;
            }
            else
            {
                AssertLog("WSARecv Error [%d]", dwErrorCode);
                this->Close();
                return False;
            }
        }
    }
    return False;
}
