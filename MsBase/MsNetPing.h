#pragma once
#include <windows.h>

//这里需要导入库 Ws2_32.lib，在不同的IDE下可能不太一样 
//#pragma comment(lib, "Ws2_32.lib")

#define DEF_PACKET_SIZE 32
#define ECHO_REQUEST 8
#define ECHO_REPLY 0

struct IPHeader
{
    BYTE m_byVerHLen; //4位版本+4位首部长度
    BYTE m_byTOS; //服务类型
    USHORT m_usTotalLen; //总长度
    USHORT m_usID; //标识
    USHORT m_usFlagFragOffset; //3位标志+13位片偏移
    BYTE m_byTTL; //TTL
    BYTE m_byProtocol; //协议
    USHORT m_usHChecksum; //首部检验和
    ULONG m_ulSrcIP; //源IP地址
    ULONG m_ulDestIP; //目的IP地址
};

struct ICMPHeader
{
    BYTE m_byType; //类型
    BYTE m_byCode; //代码
    USHORT m_usChecksum; //检验和 
    USHORT m_usID; //标识符
    USHORT m_usSeq; //序号
    ULONG m_ulTimeStamp; //时间戳（非标准ICMP头部）
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
