#include "Precompiled.h"

void AsserMessage(E_ASSERT_LEVEL eLevel, LPSTR msg)
{
    USE_PARAM(eLevel, msg);

    //DebugStr(msg);
    ::OutputDebugStringA(msg);

    CHAR MsgBuf[ASSERTX_STR_MAX_LEN] = { 0 };
    static Boolean s_IsSystem = MsService::Token_IsSystem();
    if (s_IsSystem) { return; }
    switch (eLevel)
    {
        // 标准
        case AL_NORMAL:
        {
            Int32 nRet = ::MessageBoxA(MsBase::GetCurrentThreadWnd(), msg, "【 是 】中断,【 否 】继续.【取消】,", MB_YESNOCANCEL);
            switch (nRet)
            {
                case IDOK:
                case IDNO:
                {
                    ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_CUSTOM_DEFAULT), MsgBuf, ASSERTX_STR_MAX_LEN, nullptr);
                    SIZE_T sMsgLen = strlen(MsgBuf);
                    if (sMsgLen)
                    {
                        MsgBuf[sMsgLen - 1] = '\0';
                    }
                    ::MessageBoxA(MsBase::GetCurrentThreadWnd(), MsgBuf, "错误码", MB_OK);
                }break;

                case IDYES:
                {
                    DebugCode(::DebugBreak());
                }break;

                case IDCANCEL:
                {
                    //__try { throw; }
                    //__except (MsBase::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
                }break;
            }
        }break;

        // 灾难
        case AL_CATAST:
        {
            Int32 nRet = ::MessageBoxA(MsBase::GetCurrentThreadWnd(), msg, "灾难性断言,程序即将结束.是否中断查看", MB_YESNO | MB_ICONERROR);
            switch (nRet)
            {
                case IDOK:
                case IDNO: {; }break;
                case IDYES:
                {
                    DebugCode(::DebugBreak());
                }break;
            }
            ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_CUSTOM_DEFAULT), MsgBuf, ASSERTX_STR_MAX_LEN, nullptr);
            SIZE_T sMsgLen = strlen(MsgBuf);
            if (sMsgLen)
            {
                MsgBuf[sMsgLen - 1] = _T('\0');
            }
            ::MessageBoxA(MsBase::GetCurrentThreadWnd(), MsgBuf, "结束程序", MB_OK | MB_USERICON);
            exit(INVALID_NID);
        }break;

        // 提示
        case AL_SHOWMSG: {; }break;
    }
}

void ASSERT_LOG(LPCSTR szTemp)
{
    static mstr s_AssertxFileName;
    if (s_AssertxFileName.empty())
    {
        CHAR szBuff[MAX_PATH];
        ::GetModuleBaseNameA(::GetCurrentProcess(), nullptr, szBuff, MAX_PATH);
        s_AssertxFileName = szBuff;
        s_AssertxFileName += ".log";
    }
    FILE* xFile = nullptr;
    fopen_s(&xFile, s_AssertxFileName.c_str(), "a+");
    if (xFile)
    {
        DWORD dwLogSize = (DWORD)(strnlen(szTemp, ASSERTX_STR_MAX_LEN) * sizeof(CHAR));
        fwrite(szTemp, sizeof(CHAR), dwLogSize, xFile);
        fclose(xFile);
    }
}

void ASSERTEX(E_ASSERT_LEVEL eLevel, LPCSTR file, DWORD line, LPCSTR func, LPCSTR expr, LPCSTR info, ...)
{
    SYSTEMTIME st;
    CURRENT_LOCALTIME(st);
    CHAR szBaseFormat[ASSERTX_STR_MAX_LEN] = { 0 };
    CHAR szAsserMessage[ASSERTX_STR_MAX_LEN] = { 0 };
    _snprintf_s(szBaseFormat, sizeof(szBaseFormat),
        "\n"
        "time:[%02d-%02d-%02d %02d:%02d:%02d.%03d]file:[%s(%d)] func:[%s(%s)]"
        "\n"
        "info:[%s]"
        "\n",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        file, line, func, expr, info);

    va_list ap;
    va_start(ap, info);
    vsprintf_s(szAsserMessage, szBaseFormat, ap);
    va_end(ap);
    AsserMessage(eLevel, szAsserMessage);
    if (eLevel > AL_SHOWMSG)
    {
        try
        {
            throw(expr);
        }
        catch (...)
        {

        }
    }
}
