#pragma once
// 高级钩子消息索引
#define WEB_HOOK_PACKET_INDEX_HIGHER "-1"

// 低级钩子消息索引
#define WEB_HOOK_PACKET_INDEX_LOWER "-2"

extern const Byte WEB_CLIENT_ACCESS_POLICY_RET[364];

class MsHttpResponse : public HTTP_RESPONSE
{
public:
    MsHttpResponse();
    ~MsHttpResponse();

    void SetCharsetUTF8()
    {
        //Headers.KnownHeaders[HttpHeaderContentType].pRawValue = "text/html";
        //Headers.KnownHeaders[HttpHeaderContentType].RawValueLength = (USHORT)strlen(Headers.KnownHeaders[HttpHeaderContentType].pRawValue);

        //Headers.KnownHeaders[HttpHeaderContentEncoding].pRawValue = "utf-8";
        //Headers.KnownHeaders[HttpHeaderContentEncoding].RawValueLength = (USHORT)strlen(Headers.KnownHeaders[HttpHeaderContentEncoding].pRawValue);
    }

    void SetCharsetGB2312()
    {
        //Headers.KnownHeaders[HttpHeaderContentType].pRawValue = "text/html";
        //Headers.KnownHeaders[HttpHeaderContentType].RawValueLength = (USHORT)strlen(Headers.KnownHeaders[HttpHeaderContentType].pRawValue);

        //Headers.KnownHeaders[HttpHeaderContentEncoding].pRawValue = "gb2312";
        //Headers.KnownHeaders[HttpHeaderContentEncoding].RawValueLength = (USHORT)strlen(Headers.KnownHeaders[HttpHeaderContentEncoding].pRawValue);
    }

    void FillWebRet(USHORT xStatusCode, LPSTR xReason, LPCVOID xData, Int32 xDataLength)
    {
        StatusCode = xStatusCode;
        pReason = xReason;
        ReasonLength = (USHORT)strlen(xReason);

        if (xDataLength > 0)
        {
            pEntityChunks->DataChunkType = HttpDataChunkFromMemory;
            pEntityChunks->FromMemory.pBuffer = (LPVOID)xData;
            pEntityChunks->FromMemory.BufferLength = xDataLength;
        }
    }

    void AddNeedRecoveryObject(LPVOID xData) { m_List_NeedRecoveryObject.Add(xData); }
    void AddNeedRecoveryArray(LPVOID xData) { m_List_NeedRecoveryArray.Add(xData); }
    void AddNeedRecoveryHandle(HANDLE hObject) { m_List_NeedRecoveryHandle.Add(hObject); }

    void FillWebRetFile_Successful(wstr& xFileName)
    {
        HANDLE hFile = ::CreateFile(xFileName.c_str(), GENERIC_ALL, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        FillWebRetFile_Successful(hFile);
    }
    void FillWebRetFile_Successful(HANDLE hFile)
    {
        StatusCode = 200;
        pReason = "Successful";
        ReasonLength = 10;

        if (INVALID_HANDLE_VALUE != hFile)
        {
            EntityChunkCount = 1;
            pEntityChunks->DataChunkType = HttpDataChunkFromFileHandle;
            //DWORD dwFileSizeHigh = 0;
            memset(&(pEntityChunks->FromFileHandle.ByteRange.StartingOffset), 0, sizeof(pEntityChunks->FromFileHandle.ByteRange.StartingOffset));
            pEntityChunks->FromFileHandle.ByteRange.Length.QuadPart = HTTP_BYTE_RANGE_TO_EOF;
            pEntityChunks->FromFileHandle.FileHandle = hFile;
            this->AddNeedRecoveryHandle(hFile);
        }
    }

    DINLINE void FillWebRetMemory_Successful(LPCVOID xData, size_t xDataLength) { return FillWebRetMemory_Successful(xData, (Int32)xDataLength); }
    DINLINE void FillWebRetMemory_Successful(LPCVOID xData, Int32 xDataLength)
    {
        StatusCode = 200;
        pReason = "Successful";
        ReasonLength = 10;

        if (xDataLength > 0)
        {
            EntityChunkCount = 1;
            pEntityChunks->DataChunkType = HttpDataChunkFromMemory;
            pEntityChunks->FromMemory.pBuffer = (LPVOID)xData;
            pEntityChunks->FromMemory.BufferLength = xDataLength;
        }
    }

private:
    MsList<LPVOID> m_List_NeedRecoveryObject;
    MsList<LPVOID> m_List_NeedRecoveryArray;
    MsList<HANDLE> m_List_NeedRecoveryHandle;
};

typedef Boolean(*LP_WEB_EXECUTE_PACKET)(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList);

#define AUTO_WEB_REGISTER(factory, rawurl) Boolean _fun_##rawurl(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList);\
    MsWebPacketRegisterAssist g_ms_auto_##rawurl(factory, ""#rawurl, _fun_##rawurl); \
    DINLINE Boolean _fun_##rawurl(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList)

#define AUTO_WEB_REGISTER_HIGHE_HOOK(factory) Boolean _fun_##WEB_HOOK_PACKET_INDEX_HIGHER(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList);\
    MsWebPacketRegisterAssist g_ms_auto_##WEB_HOOK_PACKET_INDEX_HIGHER(factory, WEB_HOOK_PACKET_INDEX_HIGHER, _fun_##WEB_HOOK_PACKET_INDEX_HIGHER); \
    DINLINE Boolean _fun_##WEB_HOOK_PACKET_INDEX_HIGHER(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList)

#define AUTO_WEB_REGISTER_LOWER_HOOK(factory) Boolean _fun_##WEB_HOOK_PACKET_INDEX_LOWER(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList);\
    MsWebPacketRegisterAssist g_ms_auto_##WEB_HOOK_PACKET_INDEX_LOWER(factory, WEB_HOOK_PACKET_INDEX_LOWER, _fun_##WEB_HOOK_PACKET_INDEX_LOWER); \
    DINLINE Boolean _fun_##WEB_HOOK_PACKET_INDEX_LOWER(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse, MsVector<mstr>& xParamList)

class MsWebPacketFactory
{
public:
    DINLINE MsWebPacketFactory() :m_Hook_Higher_WebExecutePacket(nullptr), m_Hook_Lower_WebExecutePacket(nullptr) { ; }
    DINLINE ~MsWebPacketFactory() { ; }

    Boolean RegisterPacket(mstr xRawUrl, LP_WEB_EXECUTE_PACKET xWebExecutePacket)
    {
        if (xRawUrl == WEB_HOOK_PACKET_INDEX_HIGHER && m_Hook_Higher_WebExecutePacket == nullptr)
        {
            m_Hook_Higher_WebExecutePacket = xWebExecutePacket;
            return True;
        }
        else if (xRawUrl == WEB_HOOK_PACKET_INDEX_LOWER && m_Hook_Lower_WebExecutePacket == nullptr)
        {
            m_Hook_Lower_WebExecutePacket = xWebExecutePacket;
            return True;
        }
        else
        {
            if (m_Dict_WebExecutePacket.ContainsKey(xRawUrl))
            {
                return False;
            }
            else
            {
                m_Dict_WebExecutePacket.Add(xRawUrl, xWebExecutePacket);
                return True;
            }
        }
    }

    Boolean ExecutePacket(HTTP_REQUEST& xHttpRequest, MsHttpResponse& xHttpResponse)
    {
        MsVector<mstr> xParamList;

        if (m_Hook_Higher_WebExecutePacket)
        {
            return m_Hook_Higher_WebExecutePacket(xHttpRequest, xHttpResponse, xParamList);
        }
        if ((*(xHttpRequest.pRawUrl)) != '/' || (*(xHttpRequest.pRawUrl + 1)) == '\0')
        {
            return False;
        }

        CHAR szBuff[1024] = { 0 };
        strcpy_s(szBuff, 1024, xHttpRequest.pRawUrl + 1);
        LPSTR lpTemp = strchr((LPSTR)szBuff, '/');
        if (lpTemp) { MsBaseDef::StringSplit(lpTemp, '/', xParamList); (*lpTemp) = '\0'; }
        mstr xExeUrl = szBuff;

        if (m_Dict_WebExecutePacket.ContainsKey(xExeUrl))
        {
            return m_Dict_WebExecutePacket[xExeUrl](xHttpRequest, xHttpResponse, xParamList);
        }
        else if (m_Hook_Lower_WebExecutePacket)
        {
            return m_Hook_Lower_WebExecutePacket(xHttpRequest, xHttpResponse, xParamList);
        }
        else
        {
            return False;
        }
    }

    //DINLINE void GetUrlParamList(LPCSTR lpRawUrl, MsVector<mstr>& xParamList)
    //{
    //    LPSTR lpParam = (LPSTR)lpRawUrl;
    //    LPSTR lpTemp = strchr(lpParam, '/');
    //    while (lpTemp)
    //    {
    //        char x = *lpTemp;
    //        (*lpTemp) = '\0';
    //        if ((*lpParam) != '\0')
    //        {
    //            xParamList.Add(lpParam);
    //        }
    //        (*lpTemp) = x;
    //        lpParam = lpTemp + 1;
    //        lpTemp = strchr(lpParam, '/');
    //    }
    //    if ((*lpParam) != '\0')
    //    {
    //        xParamList.Add(lpParam);
    //    }
    //}

private:
    LP_WEB_EXECUTE_PACKET m_Hook_Higher_WebExecutePacket;
    LP_WEB_EXECUTE_PACKET m_Hook_Lower_WebExecutePacket;
    MsDictionary<mstr, LP_WEB_EXECUTE_PACKET> m_Dict_WebExecutePacket;
};


class MsWebPacketRegisterAssist { public: MsWebPacketRegisterAssist(MsWebPacketFactory& xMsWebPacketFactory, LPCSTR xRawUrl, LP_WEB_EXECUTE_PACKET xWebExecutePacket) { xMsWebPacketFactory.RegisterPacket(xRawUrl, xWebExecutePacket); } };
