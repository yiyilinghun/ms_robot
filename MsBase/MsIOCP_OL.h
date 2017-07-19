#pragma once
#include "MsIOCPDef.h"

class MsIOCPManager;
class MsClusterNode;
class MsNetPacket;
class MsNetTerminal;
class MsThreadIOCPManager;

#define BUFFERLEN 256*KB_SIZE
#define MAX_URL_LEN 1024
#define WEB_BACKLOG_MAX_COUNT 1024ui64
#define ANY_WEB_ADDR "+"

class IocpUnit;
#pragma pack(push,1)
struct iocp_base_ol : public OVERLAPPED { IOCP_OL_TYPE m_Type; };
struct iocp_timer_ol : public iocp_base_ol
{
    DINLINE iocp_timer_ol(IocpUnit* xIocpUnit, MsTimer* xTimer) : m_IocpUnit(xIocpUnit), m_Timer(xTimer) {
        m_Type = IOCP_OL_TYPE::IOT_BASE;
    }
    IocpUnit* m_IocpUnit;
    MsTimer* m_Timer;
};
struct iocp_net_ol : public iocp_base_ol
{
    DINLINE iocp_net_ol(MS_NET_TERMINAL_OL_TYPE xNetType, DWORD xSumNeedSendDataLen, DWORD xSurplusSendDataLen)
        : m_NetType(xNetType), m_IsHaveInstantlyComplete(False), m_SumNeedSendDataLen(xSumNeedSendDataLen), m_SurplusSendDataLen(xSurplusSendDataLen) {
        m_Type = IOCP_OL_TYPE::IOT_NET;
    }

    MS_NET_TERMINAL_OL_TYPE m_NetType;
    Boolean m_IsHaveInstantlyComplete;
    DWORD m_SumNeedSendDataLen;
    DWORD m_SurplusSendDataLen;
};
struct iocp_packet_ol : public iocp_base_ol
{
    DINLINE iocp_packet_ol(MsNetTerminal* xMsNetTerminal, MsNetPacket* xPacket)
        : m_MsNetTerminal(xMsNetTerminal), m_Packet(xPacket) {
        m_Type = IOCP_OL_TYPE::IOT_PACKET;
    }
    MsNetTerminal* m_MsNetTerminal;
    MsNetPacket* m_Packet;
};
enum HttpAsynSendType
{
    HAST_FILE,
    HAST_ARRAY,
};
struct iocp_web_ol : public OVERLAPPED
{
    inline Boolean HasData() { return this->m_FirstBuff.m_HttpRequest.Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS; }
    union MyUnion
    {
        HTTP_REQUEST m_HttpRequest;
        BYTE m_BuffData[BUFFERLEN];
    }m_FirstBuff;

    HTTP_RESPONSE m_HttpResponse;
    HTTP_DATA_CHUNK m_EntityChunks;
    Boolean m_WaitContinue = False;

    Int64 m_Time;
    SafePtr<void> m_SendData;
    HttpAsynSendType m_AsynSendType;
    QWORD m_SendNeedLen;
    QWORD m_SendCurrentLen = 0;

    MsVector<SafePtr<void>> m_RecvBuffs;
    MsVector<Int32> m_RecvNeedLen;
    Int32 m_RecvedLen = 0;

    mstr m_FirstParam;
    MsVector<mstr> m_UrlParam;
};
struct iocp_diy_ol : public iocp_base_ol
{
    DINLINE iocp_diy_ol(IocpUnit* xIocpUnit, Int32 xEventId) : m_IocpUnit(xIocpUnit), m_EventId(xEventId) { m_Type = IOCP_OL_TYPE::IOT_DIY; }
    DINLINE iocp_diy_ol(IocpUnit* xIocpUnit, Int32 xEventId, MsVector<Int64>& xIntParams) : m_IocpUnit(xIocpUnit), m_EventId(xEventId), m_IntParams(xIntParams) { m_Type = IOCP_OL_TYPE::IOT_DIY; }
    DINLINE iocp_diy_ol(IocpUnit* xIocpUnit, Int32 xEventId, MsVector<LPVOID>& xPtrParams) : m_IocpUnit(xIocpUnit), m_EventId(xEventId), m_PtrParams(xPtrParams) { m_Type = IOCP_OL_TYPE::IOT_DIY; }
    DINLINE iocp_diy_ol(IocpUnit* xIocpUnit, Int32 xEventId, MsVector<LPVOID>& xPtrParams, MsVector<Int64>& xIntParams)
        : m_IocpUnit(xIocpUnit), m_EventId(xEventId), m_PtrParams(xPtrParams), m_IntParams(xIntParams) {
        m_Type = IOCP_OL_TYPE::IOT_DIY;
    }

    Int32 m_EventId;
    IocpUnit* m_IocpUnit;
    MsVector<LPVOID> m_PtrParams;
    MsVector<Int64> m_IntParams;
};

struct WEB_OL
{
    WEB_OL(LPBASEPROTO lpPostProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xUrl);
    WEB_OL(LPBASEPROTO lpPostProto, LPBASEPROTO lpRetProto, LPCSTR xAddr, WORD xPort, DWORD xCallbackKey, LPCSTR xUrl);
    ~WEB_OL();

    mstr m_ObjectName;
    mstr m_Addr;
    WORD m_Port;

    Boolean m_IsCompleted;
    Boolean m_IsFailed;

    mstr m_Url;
    DWORD m_CallbackKey;
    LPBASEPROTO m_lpPostProto;
    mstr m_RetStr;
    LPBASEPROTO m_lpRetProto;
};


#pragma pack(pop)

