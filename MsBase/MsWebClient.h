#pragma once
#define MAX_GET_LEN (16 * MB_SIZE)

class MsWebClient
{
public:

    static Boolean WebProtoRequestRetProto(LPBASEPROTO xSendProto, LPBASEPROTO xRetProto, LPCSTR xAddr, WORD xPort, LPCSTR xObjectName, ...)
    {
        va_list ap;
        va_start(ap, xObjectName);
        CHAR szBuff[DEFAULT_BUFF_SIZE];
        vsprintf_s(szBuff, xObjectName, ap);
        va_end(ap);
        mstr xRetStr;
        return WebProtoRequestRetProto(xSendProto, xRetProto, MsBase::M2W(xAddr).c_str(), xPort, MsBase::M2W(szBuff).c_str());
    }

    static Boolean WebProtoRequestRetProto(LPBASEPROTO xSendProto, LPBASEPROTO xRetProto, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName)
    {
        if (!xSendProto) { return WebDataRequestRetProto(nullptr, 0, xRetProto, xAddr, xPort, xObjectName); }

        LPBYTE xSendData[MAX_PACKET_DATA_LEN];
        Int32 xSendDataSize = xSendProto->ByteSize();
        if (xSendProto->IsInitialized())
        {
            if (xSendDataSize <= MAX_PACKET_DATA_LEN && xSendProto->SerializeToArray(xSendData, xSendDataSize))
            {
                mstr xRetStr;
                Boolean xRet = WebDataRequestRetStr(xSendData, xSendDataSize, xRetStr, xAddr, xPort, xObjectName);
                if (!xRet || !xRetProto->ParseFromString(xRetStr))
                {
                    AssertLog("WebRequestRetProto[%s]Failed!", xRetProto->GetTypeName().c_str());
                    return False;
                }
                return True;
            }
        }
        MsBase::ShowNotExistField(*xSendProto);
        return False;
    }

    static Boolean WebDataRequestRetProto(LPVOID lpSendData, Int32 xSendDataSize, LPBASEPROTO xRetProto, LPCSTR xAddr, WORD xPort, LPCSTR xObjectName, ...)
    {
        va_list ap;
        va_start(ap, xObjectName);
        CHAR szBuff[DEFAULT_BUFF_SIZE];
        vsprintf_s(szBuff, xObjectName, ap);
        va_end(ap);
        mstr xRetStr;
        return WebDataRequestRetProto(lpSendData, xSendDataSize, xRetProto, MsBase::M2W(xAddr).c_str(), xPort, MsBase::M2W(szBuff).c_str());
    }

    static Boolean WebDataRequestRetProto(LPVOID lpSendData, Int32 xSendDataSize, LPBASEPROTO xRetProto, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName)
    {
        mstr xRetStr;
        if (WebDataRequestRetStr(lpSendData, xSendDataSize, xRetStr, xAddr, xPort, xObjectName))
        {
            if (xRetProto)
            {
                if (xRetProto->ParseFromString(xRetStr))
                {
                    return True;
                }
                else
                {
                    AssertLog("WebRequestRetProto[%s]Failed!", xRetProto->GetTypeName().c_str());
                }
            }
        }
        return False;
    }

    static Boolean WebProtoRequestRetStr(LPBASEPROTO lpPostProto, mstr& xRetStr, LPCSTR xAddr, WORD xPort, LPCSTR xObjectName, ...)
    {
        va_list ap;
        va_start(ap, xObjectName);
        CHAR szBuff[DEFAULT_BUFF_SIZE];
        vsprintf_s(szBuff, xObjectName, ap);
        va_end(ap);
        return WebProtoRequestRetStr(lpPostProto, xRetStr, MsBase::M2W(xAddr).c_str(), xPort, MsBase::M2W(szBuff).c_str());
    }

    static Boolean WebProtoRequestRetStr(LPBASEPROTO lpPostProto, mstr& xRetStr, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName)
    {
        if (!lpPostProto) { return WebDataRequestRetStr(nullptr, 0, xRetStr, xAddr, xPort, xObjectName); }

        BYTE xData[MAX_GET_LEN];
        DWORD xRetLen = 0;

        xRetStr = "";
        LPBYTE xSendData[MAX_PACKET_DATA_LEN];
        Int32 xSendDataSize = lpPostProto->ByteSize();
        if (lpPostProto->IsInitialized())
        {
            if (xSendDataSize <= MAX_PACKET_DATA_LEN && lpPostProto->SerializeToArray(xSendData, xSendDataSize))
            {
                Boolean xRet = WebRequest(xSendData, xSendDataSize, xData, MAX_GET_LEN, xRetLen, xAddr, xPort, xObjectName);
                if (xRetLen > 0)
                {
                    xRetStr.resize(xRetLen);
                    memcpy((LPVOID)xRetStr.data(), xData, xRetLen);
                }
                return xRet;
            }
        }
        MsBase::ShowNotExistField(*lpPostProto);
        return False;
    }

    static Boolean WebDataRequestRetStr(LPVOID lpSendData, Int32 xSendDataSize, mstr &xRetStr, LPCSTR xAddr, WORD xPort, LPCSTR xObjectName, ...)
    {
        va_list ap;
        va_start(ap, xObjectName);
        CHAR szBuff[DEFAULT_BUFF_SIZE];
        vsprintf_s(szBuff, xObjectName, ap);
        va_end(ap);
        return WebDataRequestRetStr(lpSendData, xSendDataSize, xRetStr, MsBase::M2W(xAddr).c_str(), xPort, MsBase::M2W(szBuff).c_str());
    }

    static Boolean WebDataRequestRetStr(LPVOID lpSendData, Int32 xSendDataSize, mstr& xRetStr, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName)
    {
        BYTE xData[MAX_GET_LEN];
        DWORD xRetLen = 0;
        Boolean xRet = WebRequest(lpSendData, xSendDataSize, xData, MAX_GET_LEN, xRetLen, xAddr, xPort, xObjectName);
        xRetStr.resize(xRetLen);
        memcpy((LPVOID)xRetStr.data(), xData, xRetLen);
        return xRet;
    }

    static Boolean WebRequest(LPVOID lpSendData, Int32 xSendDataSize, LPBYTE xRetData, DWORD xMaxRetLen, DWORD& xRetLen, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName)
    {
        HINTERNET hSession = nullptr;
        HINTERNET hConnect = nullptr;
        HINTERNET hRequest = nullptr;
        __try
        {
            hSession = ::WinHttpOpen(nullptr, 0, nullptr, nullptr, 0);
            hConnect = ::WinHttpConnect(hSession, xAddr, (INTERNET_PORT)xPort, 0);
            hRequest = ::WinHttpOpenRequest(hConnect, L"GET", xObjectName, nullptr, nullptr, nullptr, 0);

            DWORD dwFlags;
            DWORD dwBuffLen = sizeof(dwFlags);
            ::WinHttpQueryOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);
            dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
            dwFlags |= SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
            dwFlags |= SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
            ::WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

            if (::WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, lpSendData, xSendDataSize, xSendDataSize, 0))
            {
                if (::WinHttpReceiveResponse(hRequest, nullptr))
                {
                    xRetLen = 0;
                    DWORD dwSize = 0;
                    ::WinHttpQueryDataAvailable(hRequest, &dwSize);
                    do
                    {
                        ::WinHttpReadData(hRequest, xRetData + xRetLen, dwSize, &dwSize);
                        xRetLen += dwSize;
                        if (xRetLen > xMaxRetLen)
                        {
                            throw _T("获取的数据超过最大限制!");
                        }
                        ::WinHttpQueryDataAvailable(hRequest, &dwSize);
                    } while (dwSize);
                    return True;
                }
            }
        }
        __finally
        {
            if (hRequest) { ::WinHttpCloseHandle(hRequest); }
            if (hConnect) { ::WinHttpCloseHandle(hConnect); }
            if (hSession) { ::WinHttpCloseHandle(hSession); }
        }
        return False;
    }

    static Boolean WebRequest(BaseProto& xRetProto, LPCWSTR xAddr, WORD xPort, LPCWSTR xObjectName, LPVOID lpSendData = nullptr, Int32 xSendDataSize = 0)
    {
        BYTE xData[MAX_GET_LEN];
        DWORD xRetLen = 0;
        Boolean xRet = WebRequest(lpSendData, xSendDataSize, xData, MAX_GET_LEN, xRetLen, xAddr, xPort, xObjectName);
        xRetProto.ParseFromArray(xData, xRetLen);
        return xRet;
    }
};

class MsThreadWebClient : public MsThread
{
public:
    MsThreadWebClient(LPCSTR xName);

    void ExeWeb_Asyn(WEB_OL* lpWebol);
    void TickOnce();

    static void MsThreadWebProc(LPVOID lpThreadParameter) { ((MsThreadWebClient*)lpThreadParameter)->TickOnce(); }

    MsLock m_WebLock;
    MsQueue<WEB_OL*> m_QueueWebol;
public:
};
