#pragma once

#define FAST_WEB_RET_FILE(file) HANDLE hFile = ::CreateFileA(""#file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);\
if (hFile != INVALID_HANDLE_VALUE){LARGE_INTEGER xFileSize; DWORD hiSize = 0;DWORD loSize = ::GetFileSizeEx(hFile, &hiSize);xMsHttpResponse->FillWebRetFileSuccessful(hFile, 0, (QWORD)loSize);return True;}\
xMsHttpResponse->RetFailure();return True;

class MsWebServer : public IocpUnit
{
public:
    MsWebServer(UInt16 xPort, MsThreadIOCPManager* xMsThreadIOCPManager, Int64 xUnitIndex);
    MsWebServer(mstr xHost, UInt16 xPort, MsThreadIOCPManager* xMsThreadIOCPManager, Int64 xUnitIndex);
    virtual ~MsWebServer();

    virtual IntPtr GetHandle();
    virtual void OnMsg(OVERLAPPED_ENTRY& xEntry);

    static mstr AddHead(LPCSTR xTitle, LPCSTR xCharset);
    static mstr LinkHtmlText(LPCSTR xUrl, LPCSTR xText);
    static mstr LinkHtmlText(LPCSTR xUrl);
    static mstr FontHtmlText(LPCSTR xText, Int32 xSize, LPCSTR xColor);
    static mstr SetReloadTimer(Int32 xTimer);

    // ANY_WEB_ADDR
    Boolean CreateServer(mstr xHost, UInt16 xPort);
    void DestroyServer();

    Boolean RecvData(iocp_web_ol* xIocpWebol, Int32 xBuffSize);

    Boolean RetFailure(iocp_web_ol* xIocpWebol);
    Boolean RetProto(iocp_web_ol* xIocpWebol, BaseProto& xProto);
    Boolean RetStr(iocp_web_ol* xIocpWebol, mstr& xText) { return RetStr(xIocpWebol, xText.c_str()); }
    Boolean RetStr(iocp_web_ol* xIocpWebol, LPCSTR xText);
    Boolean RetFilePath(iocp_web_ol* xIocpWebol, mstr xFileName);
    Boolean RetFileHandle(iocp_web_ol* xIocpWebol, HANDLE hFile);
    Boolean RetCopyMemory(iocp_web_ol* xIocpWebol, LPCVOID xMemoryData, QWORD xDataLength);

    Boolean m_IsWebShow;

private:
    void CheckRequestQueue(Int32 xCount = WEB_BACKLOG_MAX_COUNT);

    inline iocp_web_ol* GetFreeRequestol()
    {
        SafePtr<iocp_web_ol> xPtr(NEW iocp_web_ol());
        ZeroMemory(xPtr.get(), sizeof(OVERLAPPED));
        m_Set_HttpRecvOverlapped[xPtr.get()] = xPtr;
        return xPtr.get();
    }
    MsUnorderedDictionary<iocp_web_ol*, SafePtr<iocp_web_ol>> m_Set_HttpRecvOverlapped;
    MsUnorderedDictionary<HTTP_REQUEST_ID, SafePtr<iocp_web_ol>> m_Dict_HttpRecvOverlapped;
    MsUnorderedDictionary<HTTP_REQUEST_ID, SafePtr<iocp_web_ol>> m_Dict_HttpSendOverlapped;
    //MsMulDictionary<Int64, HTTP_REQUEST_ID> m_Dict_Time;
private:
    Boolean m_IsDestroy = True;

    HANDLE m_ReqQueueHandle;
    HTTP_SERVER_SESSION_ID m_ServerSessionId;
    HTTP_URL_GROUP_ID m_UrlGroupID;
    HTTP_URL_CONTEXT m_UrlContext;
    WCHAR m_TopdeskQualifiedUrl[MAX_URL_LEN];
};

