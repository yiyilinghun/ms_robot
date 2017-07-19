#pragma once
#include <windows.h>

//������Ҫ����� Ws2_32.lib���ڲ�ͬ��IDE�¿��ܲ�̫һ�� 
//#pragma comment(lib, "Ws2_32.lib")

#define DEF_PACKET_SIZE 32
#define ECHO_REQUEST 8
#define ECHO_REPLY 0

struct IPHeader
{
    BYTE m_byVerHLen; //4λ�汾+4λ�ײ�����
    BYTE m_byTOS; //��������
    USHORT m_usTotalLen; //�ܳ���
    USHORT m_usID; //��ʶ
    USHORT m_usFlagFragOffset; //3λ��־+13λƬƫ��
    BYTE m_byTTL; //TTL
    BYTE m_byProtocol; //Э��
    USHORT m_usHChecksum; //�ײ������
    ULONG m_ulSrcIP; //ԴIP��ַ
    ULONG m_ulDestIP; //Ŀ��IP��ַ
};

struct ICMPHeader
{
    BYTE m_byType; //����
    BYTE m_byCode; //����
    USHORT m_usChecksum; //����� 
    USHORT m_usID; //��ʶ��
    USHORT m_usSeq; //���
    ULONG m_ulTimeStamp; //ʱ������Ǳ�׼ICMPͷ����
};

struct PingReply
{
    USHORT m_usSeq;
    DWORD m_dwRoundTripTime;
    DWORD m_dwBytes;
    DWORD m_dwTTL;
};

class MsNetPing
{
public:
    MsNetPing();
    ~MsNetPing();
    BOOL CreatePing(DWORD dwDestIP, DWORD dwTimeout);
    BOOL CreatePing(const char *szDestIP, DWORD dwTimeout);
    BOOL CheckRecv(BOOL& xIsTimeOut);

    BOOL Ping(DWORD dwDestIP, DWORD dwTimeout);
    BOOL Ping(const char *szDestIP, DWORD dwTimeout);
private:
    BOOL PingCore(DWORD dwDestIP, DWORD dwTimeout);
    USHORT CalCheckSum(USHORT *pBuffer, int nSize);
    ULONG GetTickCountCalibrate();

private:
    SOCKET m_sockRaw;
    WSAEVENT m_event;
    USHORT m_usCurrentProcID;
    char m_szICMPData[DEF_PACKET_SIZE + sizeof(ICMPHeader)];
    BOOL m_bIsInitSucc;
    USHORT m_usPacketSeq;
    sockaddr_in m_sockaddrDest;
    int m_nSockaddrDestSize;
    USHORT m_usSeq;
    PingReply m_PingReply;
    ULONG m_ulSendTimestamp;
    DWORD m_dwTimeout;
};
