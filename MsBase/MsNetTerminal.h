#pragma once

#define MAX_SEND_BUFF_SIZE 81920
class MsNetTerminal : public IocpUnit
{
public:
    MsNetTerminal(MsIOCPManager* xMsIOCPManager, Boolean IsInServer, SOCKET xSocket, Int64 xUnitIndex = INVALID_LID);
    virtual ~MsNetTerminal();

    virtual Boolean TickOnce();
    virtual void OnAttach();

    virtual IntPtr GetHandle() { return (IntPtr)m_Socket; }
    virtual void OnMsg(OVERLAPPED_ENTRY& xEntry);
    Boolean OnNetSendOrRecv(iocp_net_ol* xIocpNetol, OVERLAPPED_ENTRY& xEntry);

    Boolean IsValid() { return m_Socket != INVALID_SOCKET; }
    Boolean IsConnect() { return m_IsConnect && IsValid(); }
    virtual void Close();

    void SendPacket(MsNetPacket& xMsNetPacket);
    void SendPacket(BaseProto& xBaseProto, Boolean IsLastSend = False);
    void SendPacketForward(BaseProto& xBaseProto, Int64 xForwardKey, Boolean IsLastSend = False);
    void SendPacketToUnit(BaseProto& xBaseProto, Int64 xUnitKey, Boolean IsLastSend = False);

    void RealSend();
    void GoRecv();

    virtual void OnConnect();
    virtual void OnAccept();
    virtual Boolean OnPacket();

    mstr GetLoaclIPAddr();
    WORD GetLoaclPort();

    mstr GetRemoteIPAddr();
    WORD GetRemotePort();

public:
    Boolean SendData(LPWSABUF xBuffs, DWORD xBuffCount, DWORD dwSumNeedSendDataLen);
    Boolean RecvData(LPVOID xBeginDataAddr, DWORD& xOffset, DWORD dwNeedDataLen);
    Boolean ForceAsynRecvData(LPVOID xBeginDataAddr, DWORD& xOffset, DWORD dwNeedDataLen);

public:
    Boolean m_IsInServer;
    Boolean m_IsExeClose;
    Boolean m_IsConnect;
    SOCKET m_Socket;

    MsSet<iocp_net_ol*> m_SetRecvOL;
    MsNetPacket m_RecvPacket;

    Boolean m_IsLastSend;
    Boolean m_IsReadySend;
    MsList<MsNetPacket*> m_QueuePacket;
    MsSet<iocp_net_ol*> m_SetSendOL;
    MsSet<iocp_net_ol*> m_SetSending;
    MsList<MsNetPacket*> m_ListPacketSending;
    WSABUF m_SendBuffs[MAX_SEND_BUFF_SIZE];

    Int64 m_LastCommunicationTime;

public:
    iocp_net_ol m_RecvExitOL;
    Boolean m_IsUTF8;
};
