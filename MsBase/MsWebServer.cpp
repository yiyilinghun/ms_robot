#include "Precompiled.h"
//#include "atlutil.h"

Boolean MsWebServer::RetFailure(iocp_web_ol* xIocpWebol)
{
    xIocpWebol->m_HttpResponse.StatusCode = 200;
    xIocpWebol->m_HttpResponse.pReason = "Failure";
    xIocpWebol->m_HttpResponse.ReasonLength = 7;
    return True;
}

Boolean MsWebServer::RetProto(iocp_web_ol* xIocpWebol, BaseProto& xProto)
{
    xIocpWebol->m_HttpResponse.Version = HTTPAPI_VERSION_2;
    xIocpWebol->m_HttpResponse.StatusCode = 200;
    xIocpWebol->m_HttpResponse.pReason = "Successful";
    xIocpWebol->m_HttpResponse.ReasonLength = 10;
    if (m_IsWebShow)
    {
        mstr xStrProto = xProto.DebugString();
        this->RetCopyMemory(xIocpWebol, xStrProto.data(), (Int32)xStrProto.size());
    }
    else
    {
        Int32 xDataSize = xProto.ByteSize();
        if (xDataSize > 0)
        {
            xIocpWebol->m_SendData.reset(NEW CHAR[xDataSize], [](LPCSTR xData) { SAFE_DELETE_ARRAY(xData); });
            if (xProto.SerializeToArray(xIocpWebol->m_SendData.get(), (Int32)xDataSize))
            {
                if (xDataSize > 0)
                {
                    xIocpWebol->m_HttpResponse.pEntityChunks->DataChunkType = HttpDataChunkFromMemory;
                    xIocpWebol->m_HttpResponse.pEntityChunks->FromMemory.pBuffer = xIocpWebol->m_SendData.get();
                    xIocpWebol->m_HttpResponse.pEntityChunks->FromMemory.BufferLength = (ULONG)xDataSize;
                    xIocpWebol->m_HttpResponse.EntityChunkCount = 1;
                }
                return True;
            }
        }
    }
    xIocpWebol->m_HttpResponse.Version = HTTPAPI_VERSION_2;
    xIocpWebol->m_HttpResponse.StatusCode = 200;
    xIocpWebol->m_HttpResponse.pReason = "Failure";
    xIocpWebol->m_HttpResponse.ReasonLength = 7;
    return False;
}

Boolean MsWebServer::RetCopyMemory(iocp_web_ol* xIocpWebol, LPCVOID xMemoryData, QWORD xDataLength)
{
    if (xDataLength > 0)
    {
        xIocpWebol->m_SendData.reset(NEW CHAR[(size_t)xDataLength], [](LPCSTR xData) { SAFE_DELETE_ARRAY(xData); });
        memcpy(xIocpWebol->m_SendData.get(), xMemoryData, (size_t)xDataLength);
        xIocpWebol->m_AsynSendType = HttpAsynSendType::HAST_ARRAY;
        xIocpWebol->m_SendCurrentLen = 0;
        xIocpWebol->m_SendNeedLen = xDataLength;

        xIocpWebol->m_HttpResponse.pEntityChunks->DataChunkType = HttpDataChunkFromMemory;
        xIocpWebol->m_HttpResponse.pEntityChunks->FromMemory.pBuffer = xIocpWebol->m_SendData.get();
        xIocpWebol->m_HttpResponse.pEntityChunks->FromMemory.BufferLength = (ULONG)xDataLength;
        xIocpWebol->m_HttpResponse.EntityChunkCount = 1;
    }
    return True;
}

Boolean MsWebServer::RetStr(iocp_web_ol* xIocpWebol, LPCSTR xData)
{
    xIocpWebol->m_HttpResponse.Version = HTTPAPI_VERSION_2;
    xIocpWebol->m_HttpResponse.StatusCode = 200;
    xIocpWebol->m_HttpResponse.pReason = "Successful";
    xIocpWebol->m_HttpResponse.ReasonLength = 10;
    return this->RetCopyMemory(xIocpWebol, xData, (Int32)strlen(xData));
}

Boolean MsWebServer::RetFilePath(iocp_web_ol* xIocpWebol, mstr xFileName)
{
    HANDLE hFile = ::CreateFileA(xFileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    return RetFileHandle(xIocpWebol, hFile);
}

Boolean MsWebServer::RetFileHandle(iocp_web_ol* xIocpWebol, HANDLE hFile)
{
    xIocpWebol->m_HttpResponse.Version = HTTPAPI_VERSION_2;
    xIocpWebol->m_HttpResponse.StatusCode = 200;
    xIocpWebol->m_HttpResponse.pReason = "Successful";
    xIocpWebol->m_HttpResponse.ReasonLength = 10;

    if (INVALID_HANDLE_VALUE != hFile)
    {
        xIocpWebol->m_SendData.reset(hFile, [](HANDLE hFile) { ::CloseHandle(hFile); });

        xIocpWebol->m_AsynSendType = HttpAsynSendType::HAST_ARRAY;
        xIocpWebol->m_SendCurrentLen = 0;
        LARGE_INTEGER xFileSize;
        if (!::GetFileSizeEx(hFile, &xFileSize)) { xFileSize.QuadPart = 0; }
        xIocpWebol->m_SendNeedLen = xFileSize.QuadPart;

        if (xIocpWebol->m_SendNeedLen > 0)
        {
            xIocpWebol->m_HttpResponse.EntityChunkCount = 1;
            xIocpWebol->m_HttpResponse.pEntityChunks->DataChunkType = HttpDataChunkFromFileHandle;
            xIocpWebol->m_HttpResponse.pEntityChunks->FromFileHandle.ByteRange.StartingOffset.QuadPart = 0;
            xIocpWebol->m_HttpResponse.pEntityChunks->FromFileHandle.ByteRange.Length.QuadPart = xIocpWebol->m_SendNeedLen;
            xIocpWebol->m_HttpResponse.pEntityChunks->FromFileHandle.FileHandle = hFile;
        }
        return True;
    }
    return False;
}

MsWebServer::MsWebServer(UInt16 xPort, MsThreadIOCPManager* xMsThreadIOCPManager, Int64 xUnitIndex)
    : MsWebServer(ANY_WEB_ADDR, xPort, xMsThreadIOCPManager, xUnitIndex)
{
    ;
}

MsWebServer::MsWebServer(mstr xHost, UInt16 xPort, MsThreadIOCPManager* xMsThreadIOCPManager, Int64 xUnitIndex)
    : IocpUnit("MsWebServer", xMsThreadIOCPManager, xUnitIndex)
    , m_ReqQueueHandle(nullptr)
    , m_ServerSessionId(0)
    , m_UrlGroupID(0)
    , m_UrlContext(0)
{
    DEF_IOCPUNIT_TYPE(MsWebServer);

    ::ZeroMemory(m_TopdeskQualifiedUrl, sizeof(m_TopdeskQualifiedUrl));

    AssertNormal(this->CreateServer(xHost, xPort), "MsWebServer::CreateServer::Failed");
}

MsWebServer::~MsWebServer()
{
    this->DestroyServer();
    SAFE_FOREACH(m_Set_HttpRecvOverlapped)
    {
        ::HttpCancelHttpRequest(m_ReqQueueHandle, m_Set_HttpRecvOverlapped.Key()->m_FirstBuff.m_HttpRequest.RequestId, m_Set_HttpRecvOverlapped.Key());
        m_Set_HttpRecvOverlapped.RemoveCur();
    }

    SAFE_FOREACH(m_Dict_HttpRecvOverlapped)
    {
        ::HttpCancelHttpRequest(m_ReqQueueHandle, m_Dict_HttpRecvOverlapped.Key(), m_Dict_HttpRecvOverlapped.Value().get());
        m_Dict_HttpRecvOverlapped.RemoveCur();
    }

    SAFE_FOREACH(m_Dict_HttpSendOverlapped)
    {
        ::HttpCancelHttpRequest(m_ReqQueueHandle, m_Dict_HttpSendOverlapped.Key(), m_Dict_HttpSendOverlapped.Value().get());
        m_Dict_HttpSendOverlapped.RemoveCur();
    }
}

mstr MsWebServer::AddHead(LPCSTR xTitle, LPCSTR xCharset)
{
    CHAR szBuff[KB_SIZE];
    sprintf_s(szBuff, "<head><title>%s</title><meta http - equiv = \"Content-Type\" content = \"text/html; charset=%s\" /></head>", xTitle, xCharset);
    return szBuff;
}

mstr MsWebServer::LinkHtmlText(LPCSTR xUrl, LPCSTR xText)
{
    CHAR szBuff[KB_SIZE];
    sprintf_s(szBuff, "<a href=\"%s\">%s</a>", xUrl, xText);
    return szBuff;
}

mstr MsWebServer::LinkHtmlText(LPCSTR xUrl)
{
    CHAR szBuff[KB_SIZE];
    sprintf_s(szBuff, "<a href=\"%s\">{0}</a>", xUrl);
    return szBuff;
}

mstr MsWebServer::FontHtmlText(LPCSTR xText, Int32 xSize, LPCSTR xColor)
{
    CHAR szBuff[KB_SIZE];
    sprintf_s(szBuff, "<font size=\"%d\" color=\"%s\">%s</font>", xSize, xColor, xText);
    return szBuff;
}

mstr MsWebServer::SetReloadTimer(Int32 xTimer)
{
    CHAR szBuff[KB_SIZE];
    sprintf_s(szBuff, "<script language=javascript>var int=setInterval(\"ReloadTimer()\",%d);function ReloadTimer(){{location.reload(true);}}</script>", xTimer);
    return szBuff;
}

IntPtr MsWebServer::GetHandle()
{
    return m_ReqQueueHandle;
}

Boolean MsWebServer::CreateServer(mstr xHost, UInt16 xPort)
{
    swprintf_s(m_TopdeskQualifiedUrl, L"http://%s:%d/", MsBase::M2W(xHost).c_str(), xPort);

    HTTPAPI_VERSION xHttpApiVersion = HTTPAPI_VERSION_2;
    if (NO_ERROR != ::HttpInitialize(xHttpApiVersion, HTTP_INITIALIZE_CONFIG | HTTP_INITIALIZE_SERVER, nullptr))
    {
        return False;
    }

    if (NO_ERROR != ::HttpCreateServerSession(xHttpApiVersion, &m_ServerSessionId, 0))
    {
        return False;
    }

    HTTP_LOGGING_INFO LogginInfo;
    ZeroMemory(&LogginInfo, sizeof(HTTP_LOGGING_INFO));
    LogginInfo.Flags.Present = 1;
    LogginInfo.Format = HttpLoggingTypeW3C;
    LogginInfo.Fields = HTTP_LOG_FIELD_TIME | HTTP_LOG_FIELD_CLIENT_IP;
    LogginInfo.DirectoryName = _T("d:\\http.log");
    LogginInfo.DirectoryNameLength = (USHORT)wcslen(_T("d:\\http.log")) * 2;
    LogginInfo.RolloverType = HttpLoggingRolloverDaily;
    if (NO_ERROR != ::HttpSetServerSessionProperty(m_ServerSessionId, HttpServerLoggingProperty, &LogginInfo, sizeof(HTTP_LOGGING_INFO)))
    {
        return False;
    }

    if (NO_ERROR != ::HttpCreateHttpHandle(&m_ReqQueueHandle, 0))
    {
        return False;
    }

    if (NO_ERROR != ::HttpCreateUrlGroup(m_ServerSessionId, &m_UrlGroupID, 0))
    {
        return False;
    }

    if (NO_ERROR != ::HttpAddUrlToUrlGroup(m_UrlGroupID, m_TopdeskQualifiedUrl, m_UrlContext, 0))
    {
        return False;
    }

    if (NO_ERROR != ::HttpCreateRequestQueue(xHttpApiVersion, nullptr, nullptr, 0, &m_ReqQueueHandle))
    {
        return False;
    }

    HTTP_BINDING_INFO Binding;
    Binding.Flags.Present = 1;
    Binding.RequestQueueHandle = m_ReqQueueHandle;
    if (NO_ERROR != ::HttpSetUrlGroupProperty(m_UrlGroupID, HttpServerBindingProperty, &Binding, sizeof(HTTP_BINDING_INFO)))
    {
        return False;
    }

    //// 缓冲区内容
    //HTTP_DATA_CHUNK cacheChunk;
    //cacheChunk.DataChunkType = HttpDataChunkFromMemory;
    //cacheChunk.FromMemory.pBuffer = "缓冲区内容";
    //cacheChunk.FromMemory.BufferLength = (ULONG)strlen("缓冲区内容");
    //HTTP_CACHE_POLICY CachePolicy;
    //CachePolicy.Policy = HttpCachePolicyTimeToLive;
    //CachePolicy.SecondsToLive = 1000 * 60;
    //WCHAR szTemp[MAX_URL_LEN] = { 0 };
    //swprintf_s(szTemp, L"%sTopsec", m_TopdeskQualifiedUrl);
    //if (NO_ERROR != ::HttpAddFragmentToCache(m_ReqQueueHandle, szTemp, &cacheChunk, &CachePolicy, nullptr))
    //{
    //    return False;
    //}

    //struct HTTP_RECV_OVERLAPPED : OVERLAPPED
    //{
    //    DWORD x;
    //    Byte m_Buffer[MB_SIZE];
    //    HTTP_REQUEST_ID m_HttpRequestId;
    //};

    //HTTP_RECV_OVERLAPPED m_HttpOverlapped;
    //ZeroMemory(&m_HttpOverlapped, sizeof(HTTP_RECV_OVERLAPPED));
    //m_HttpOverlapped.m_HttpRequestId = 0;
    //ULONG xRet = ::HttpReceiveHttpRequest(m_ReqQueueHandle, m_HttpOverlapped.m_HttpRequestId, 0, (PHTTP_REQUEST)m_HttpOverlapped.m_Buffer, 16 * 1024, nullptr, &m_HttpOverlapped);

    //iocp_web_ol ol;
    //ZeroMemory(&ol, sizeof(ol));

    //ULONG xBytesReceived = 0;
    ////ZeroArray(ol.m_Buffer);
    //ULONG xRet = ::HttpReceiveHttpRequest(m_ReqQueueHandle, HTTP_NULL_ID, 0, (PHTTP_REQUEST)(ol.m_Buffer), MB_SIZE, nullptr, &ol);
    //if (NO_ERROR != xRet && WSA_IO_PENDING != xRet)
    //{
    //    AssertLog("MsWebServer::CheckRequestQueue Failed!");
    //    return False;
    //}

    this->CheckRequestQueue();

    m_IsDestroy = False;
    return True;
}

void MsWebServer::DestroyServer()
{
    if (!m_IsDestroy)
    {
        ::CancelIoEx(m_ReqQueueHandle, nullptr);
        ::HttpRemoveUrlFromUrlGroup(m_UrlGroupID, nullptr, HTTP_URL_FLAG_REMOVE_ALL);
        ::HttpCloseUrlGroup(m_UrlGroupID);
        ::HttpCloseRequestQueue(m_ReqQueueHandle);
        ::HttpCloseServerSession(m_ServerSessionId);
        ::HttpTerminate(HTTP_INITIALIZE_CONFIG | HTTP_INITIALIZE_SERVER, nullptr);

        m_IsDestroy = True;
    }
}

void MsWebServer::OnMsg(OVERLAPPED_ENTRY& xEntry)
{
    iocp_web_ol* xIocpWebol = (iocp_web_ol*)xEntry.lpOverlapped;
    if (m_IsDestroy || !xIocpWebol->m_FirstBuff.m_HttpRequest.pRawUrl) { return; }

    LPCSTR lpRawUrl = xIocpWebol->m_FirstBuff.m_HttpRequest.pRawUrl;
    //SAFE_FOREACH(m_Dict_Time)
    //{
    //    if (MsBase::TimeSpanCheckOnce(m_Dict_Time.Key(), 5000))
    //    {
    //        HTTP_REQUEST_ID xRequestId = m_Dict_Time.Value();
    //        ::HttpCancelHttpRequest(m_ReqQueueHandle, xRequestId, m_Dict_HttpSendOverlapped[xRequestId].get());
    //        m_Set_HttpRecvOverlapped.Remove(xIocpWebol);
    //        m_Dict_HttpRecvOverlapped.Remove(xRequestId);
    //        m_Dict_HttpSendOverlapped.Remove(xRequestId);
    //        m_Dict_Time.RemoveCur();
    //    }
    //    else
    //    {
    //        break;
    //    }
    //}

    // 如果是首次请求
    if (m_Set_HttpRecvOverlapped.ContainsKey(xIocpWebol))
    {
        SafePtr<iocp_web_ol> xSafeIocpWebol = m_Set_HttpRecvOverlapped[xIocpWebol];
        m_Set_HttpRecvOverlapped.Remove(xIocpWebol);

        xIocpWebol->m_HttpResponse.pEntityChunks = &(xIocpWebol->m_EntityChunks);

        xIocpWebol->m_UrlParam.Clear();
        if (lpRawUrl[0] == '/')
        {
            LPSTR lpTemp = (LPSTR)strchr((lpRawUrl + 1), '/');
            if (lpTemp)
            {
                lpTemp[0] = '\0';
                xIocpWebol->m_FirstParam = lpRawUrl + 1;
                lpTemp[0] = '/';
                MsBase::StringSplit(lpTemp + 1, '/', xIocpWebol->m_UrlParam);
                WCHAR xOutUrl[4 * KB_SIZE];
                FOREACH(xIocpWebol->m_UrlParam)
                {
                    DWORD xOutUrlLen = 4 * KB_SIZE;
                    wstr xUrl = MsBase::M2W(xIocpWebol->m_UrlParam.Value().c_str());
                    LPWSTR xInUrl = (LPWSTR)xUrl.c_str();
                    ::UrlUnescapeW(xInUrl, xOutUrl, &xOutUrlLen, URL_ESCAPE_AS_UTF8);
                    xIocpWebol->m_UrlParam.Value() = MsBase::W2M(xOutUrl);
                }
            }
            else
            {
                xIocpWebol->m_FirstParam = lpRawUrl + 1;
            }
        }

        if (xIocpWebol->m_FirstParam == "" &&  xIocpWebol->m_UrlParam.Empty())
        {
            ULONG BytesSent;
            xIocpWebol->m_HttpResponse.StatusCode = 404;
            xIocpWebol->m_HttpResponse.pReason = "Not Found";
            xIocpWebol->m_HttpResponse.ReasonLength = 9;
            ::HttpSendHttpResponse(m_ReqQueueHandle, xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, 0, &(xIocpWebol->m_HttpResponse), nullptr, &BytesSent, nullptr, 0, nullptr, nullptr);
            return;
        }

        xIocpWebol->m_WaitContinue = False;
        if (GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xIocpWebol->m_FirstParam, (iocp_base_ol*)xIocpWebol))
        {
            if (xIocpWebol->m_WaitContinue)
            {
                m_Dict_HttpRecvOverlapped.Add(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, xSafeIocpWebol);
            }
            else
            {
                ULONG BytesSent;
                ZeroMemory(xIocpWebol, sizeof(OVERLAPPED));
                ::HttpSendHttpResponse(m_ReqQueueHandle, xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, 0, &(xIocpWebol->m_HttpResponse), nullptr, &BytesSent, nullptr, 0, xIocpWebol, nullptr);
                m_Dict_HttpSendOverlapped.Add(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, xSafeIocpWebol);
            }
        }
        else
        {
            ULONG BytesSent;
            xIocpWebol->m_HttpResponse.StatusCode = 406;
            xIocpWebol->m_HttpResponse.pReason = "Execute Failed";
            xIocpWebol->m_HttpResponse.ReasonLength = 14;
            ::HttpSendHttpResponse(m_ReqQueueHandle, xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, 0, &(xIocpWebol->m_HttpResponse), nullptr, &BytesSent, nullptr, 0, nullptr, nullptr);
        }
    }
    // 如果是继续接收
    else if (m_Dict_HttpRecvOverlapped.ContainsKey(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId))
    {
        SafePtr<iocp_web_ol> xSafeIocpWebol = m_Dict_HttpRecvOverlapped[xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId];
        m_Dict_HttpRecvOverlapped.Remove(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId);
        if (xIocpWebol->m_RecvNeedLen.GetCount() > 0)
        {
            xIocpWebol->m_RecvedLen += xEntry.dwNumberOfBytesTransferred;
            xIocpWebol->m_RecvNeedLen[xIocpWebol->m_RecvNeedLen.GetCount() - 1] = xEntry.dwNumberOfBytesTransferred;
            if (GetIocpUnitRoute(m_IocpUnitType)->ExecutePacket(this, xIocpWebol->m_FirstParam, (iocp_base_ol*)xIocpWebol))
            {
                if (xIocpWebol->m_WaitContinue)
                {
                    m_Dict_HttpRecvOverlapped.Add(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, xSafeIocpWebol);
                }
                else
                {
                    ULONG BytesSent;
                    ZeroMemory(xIocpWebol, sizeof(OVERLAPPED));
                    ::HttpSendHttpResponse(m_ReqQueueHandle, xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, 0, &(xIocpWebol->m_HttpResponse), nullptr, &BytesSent, nullptr, 0, xIocpWebol, nullptr);
                    m_Dict_HttpSendOverlapped.Add(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId, xSafeIocpWebol);
                }
            }
        }
    }
    // 如果是回复中
    else if (m_Dict_HttpSendOverlapped.ContainsKey(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId))
    {
        //m_Dict_Time.Remove(xIocpWebol->m_Time, xIocpWebol->m_First.m_HttpRequest.RequestId);
        m_Dict_HttpSendOverlapped.Remove(xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId);
    }
    this->CheckRequestQueue();
}


void MsWebServer::CheckRequestQueue(Int32 xCount)
{
    while (m_Set_HttpRecvOverlapped.GetCount() < xCount)
    {
        auto xHttpRequestol = this->GetFreeRequestol();
        ULONG xBytesReceived;
        ULONG xRet = ::HttpReceiveHttpRequest(m_ReqQueueHandle, HTTP_NULL_ID, 0,
            (PHTTP_REQUEST)xHttpRequestol->m_FirstBuff.m_BuffData,
            sizeof(xHttpRequestol->m_FirstBuff.m_BuffData),
            &xBytesReceived,
            xHttpRequestol
        );
        if (NO_ERROR != xRet && WSA_IO_PENDING != xRet)
        {
            AssertLog("MsWebServer::CheckRequestQueue Failed!");
            return;
        }
    }
}

Boolean MsWebServer::RecvData(iocp_web_ol* xIocpWebol, Int32 xBuffSize)
{
    xIocpWebol->m_WaitContinue = True;
    SafePtr<void> xTemp(NEW BYTE[xBuffSize], [](LPBYTE xData) { SAFE_DELETE_ARRAY(xData); });
    xIocpWebol->m_RecvBuffs.Add(xTemp);
    xIocpWebol->m_RecvNeedLen.Add(0);
    ULONG BytesRead = 0;
    ULONG xResult = ::HttpReceiveRequestEntityBody(m_ReqQueueHandle,
        xIocpWebol->m_FirstBuff.m_HttpRequest.RequestId,
        HTTP_RECEIVE_REQUEST_ENTITY_BODY_FLAG_FILL_BUFFER,
        xTemp.get(),
        xBuffSize,
        &BytesRead,
        xIocpWebol
    );
    switch (xResult)
    {
        case ERROR_IO_PENDING:
        case NO_ERROR:
        {
            return False;
        }   break;

        case ERROR_HANDLE_EOF:
        {
            if (xIocpWebol->m_RecvNeedLen.ToLast().Value() == 0)
            {
                xIocpWebol->m_RecvBuffs.RemoveAtLast();
                xIocpWebol->m_RecvNeedLen.RemoveAtLast();
            }
            xIocpWebol->m_WaitContinue = False;
            return True;
        }break;
    }
    return False;
}
