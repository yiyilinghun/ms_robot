#pragma once

#ifndef SAFE_FREE_NET_BUFF
#define SAFE_FREE_NET_BUFF(p)   ((p) ? (::NetApiBufferFree(p), (p)=nullptr) : ((p) = (p)))
#endif

#define MAX_NAME_LEN 100

class MS_USER_INFO_3
{
public:
    MS_USER_INFO_3();
    ~MS_USER_INFO_3();
    PUSER_INFO_3 lpBuf;
    wstr szDomain;
    DWORD dwLevel;
    DWORD dwPrefMaxLen;
    DWORD dwEntriesRead;
    DWORD dwTotalEntries;
    DWORD dwResumeHandle;
};


class MS_USER_INFO_4
{
public:
    MS_USER_INFO_4();
    ~MS_USER_INFO_4();
    PUSER_INFO_4 lpBuf;
    wstr szDomain;
    DWORD dwLevel;
    DWORD dwPrefMaxLen;
    DWORD dwEntriesRead;
    DWORD dwTotalEntries;
    DWORD dwResumeHandle;
};


class MsService
{
public:
#define SET_DIR2FILEPATH {WCHAR szPath[MAX_PATH]; ::GetModuleFileNameW(nullptr, szPath, MAX_PATH); ::PathRemoveFileSpecW(szPath); ::SetCurrentDirectoryW(szPath);}

#pragma region Token
    static Boolean Token_IsSystem()
    {
        HANDLE hToken;
        DWORD dwSize = 0;
        Byte xData[KB_SIZE * 100] = { 0 };
        PTOKEN_USER xPtokenUser = (PTOKEN_USER)xData;
        SID_NAME_USE xSidNameUse;
        WCHAR Name[MAX_PATH] = { 0 };
        WCHAR szDomainName[MAX_PATH] = { 0 };

        if (::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        {
            ::GetTokenInformation(hToken, TokenUser, nullptr, dwSize, &dwSize);
            if (::GetTokenInformation(hToken, TokenUser, xData, dwSize, &dwSize))
            {
                dwSize = sizeof(Name);
                ::LookupAccountSid(nullptr, xPtokenUser->User.Sid, Name, &dwSize, szDomainName, &dwSize, &xSidNameUse);
            }
        }
        return wcscmp(L"SYSTEM", Name) == 0;
    }

    static PSID Token_GetSid(HANDLE hToken)
    {
        DWORD dwSize = 0;
        Byte xData[KB_SIZE * 100] = { 0 };
        PTOKEN_USER xPtokenUser = (PTOKEN_USER)xData;
        ::GetTokenInformation(hToken, TokenUser, nullptr, dwSize, &dwSize);
        if (::GetTokenInformation(hToken, TokenUser, xData, dwSize, &dwSize))
        {
            return xPtokenUser->User.Sid;
        }
        else
        {
            return nullptr;
        }
    }

    static PSID Token_GetPrimaryGroupSid(HANDLE hToken)
    {
        DWORD dwSize = 0;
        Byte xData[KB_SIZE * 100] = { 0 };
        PTOKEN_PRIMARY_GROUP xPtokenPrimaryGroup = (PTOKEN_PRIMARY_GROUP)xData;
        ::GetTokenInformation(hToken, TokenPrimaryGroup, nullptr, dwSize, &dwSize);
        if (::GetTokenInformation(hToken, TokenPrimaryGroup, xData, dwSize, &dwSize))
        {
            return xPtokenPrimaryGroup->PrimaryGroup;
        }
        else
        {
            return nullptr;
        }
    }

    static Boolean Token_GetGroups(HANDLE hToken, LPBYTE xTempDataBuf, DWORD dwTempDataBufSize, DWORD& xGroupCount, PSID_AND_ATTRIBUTES& lpSidAdnAttributes)
    {
        if (xTempDataBuf)
        {
            DWORD dwSize = 0;
            PTOKEN_GROUPS xPtokenGroup = (PTOKEN_GROUPS)xTempDataBuf;
            ::GetTokenInformation(hToken, TokenGroups, nullptr, dwSize, &dwSize);
            if (dwTempDataBufSize < dwSize) { return False; }
            Boolean bRet = ::GetTokenInformation(hToken, TokenGroups, xTempDataBuf, dwSize, &dwSize);
            xGroupCount = xPtokenGroup->GroupCount;
            lpSidAdnAttributes = xPtokenGroup->Groups;
            return bRet;
        }
        return False;
    }

    static Boolean Token_GetUserName(HANDLE hToken, wstr& xUserName)
    {
        DWORD dwSize = 0;
        Byte xData[KB_SIZE * 100] = { 0 };
        PTOKEN_USER xPtokenUser = (PTOKEN_USER)xData;
        SID_NAME_USE xSidNameUse;
        WCHAR Name[MAX_PATH] = { 0 };
        WCHAR szDomainName[MAX_PATH] = { 0 };
        ::GetTokenInformation(hToken, TokenUser, nullptr, dwSize, &dwSize);
        if (::GetTokenInformation(hToken, TokenUser, xData, dwSize, &dwSize))
        {
            dwSize = sizeof(Name);
            if (::LookupAccountSid(nullptr, xPtokenUser->User.Sid, Name, &dwSize, szDomainName, &dwSize, &xSidNameUse))
            {
                xUserName = Name;
                return True;
            }
        }
        return False;
    }

    static wstr Token_GetCurrentTokenUserName()
    {
        HANDLE hToken = nullptr;
        ::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
        wstr xUserName;
        Token_GetUserName(hToken, xUserName);
        return xUserName;
    }

    static Boolean Token_IsDomainAdmin(HANDLE hToken)
    {
        DWORD dwSize = 0;
        Byte xData[KB_SIZE * 100] = { 0 };
        PTOKEN_GROUPS pGroupInfo = (PTOKEN_GROUPS)xData;
        PSID pSID = nullptr;
        SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
        ::GetTokenInformation(hToken, TokenGroups, nullptr, dwSize, &dwSize);
        if (::GetTokenInformation(hToken, TokenGroups, xData, dwSize, &dwSize))
        {
            if (::AllocateAndInitializeSid(&SIDAuth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID))
            {
                for (DWORD i = 0; i < pGroupInfo->GroupCount; i++)
                {
                    if (::EqualSid(pSID, pGroupInfo->Groups[i].Sid))
                    {
                        return True;
                    }
                }
            }
        }
        return False;
    }
#pragma endregion

#pragma region Net
    static void Net_AddUserGroup(wstr xDomain, wstr xGroupName, wstr xUserName)
    {
        xDomain = L"\\\\" + xDomain;
        ::NetGroupAddUser(xDomain.c_str(), xGroupName.c_str(), xUserName.c_str());
    }

    static void Net_DelUserGroup(wstr xDomain, wstr xGroupName, wstr xUserName)
    {
        xDomain = L"\\\\" + xDomain;
        ::NetGroupDelUser(xDomain.c_str(), xGroupName.c_str(), xUserName.c_str());
    }

    static PSID Net_GetUserSid(wstr xDomain, wstr xUserName)
    {
        MS_USER_INFO_4 xMsUserInfo;
        Net_GetUserInfo(xDomain, xUserName, xMsUserInfo);
        return xMsUserInfo.lpBuf->usri4_user_sid;
    }

    static void Net_SetUserPrimaryGroupId(wstr xDomain, DWORD xGroupId, wstr xUserName)
    {
        MS_USER_INFO_4 xMsUserInfo;
        Net_GetUserInfo(xDomain, xUserName, xMsUserInfo);

        xMsUserInfo.lpBuf->usri4_primary_group_id = xGroupId;
        DWORD xArmErr;
        xDomain = L"\\\\" + xDomain;
        ::NetUserSetInfo(xDomain.c_str(), xUserName.c_str(), xMsUserInfo.dwLevel, (LPBYTE)xMsUserInfo.lpBuf, &xArmErr);
    }

    static void Net_GetUserGroups(LPCWSTR xDomain, LPCWSTR xUserName, std::list<wstr>& xListGroups)
    {
        LPGROUP_USERS_INFO_0 lpGroups = nullptr;

        DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
        DWORD dwEntriesRead = 0;
        DWORD dwTotalEntries = 0;

        WCHAR szDomain[MAX_NAME_LEN] = { 0 };
        wsprintf(szDomain, L"\\\\%s", xDomain);
        ::NetUserGetGroups(szDomain,
            xUserName,
            0,
            (LPBYTE*)&lpGroups,
            dwPrefMaxLen,
            &dwEntriesRead,
            &dwTotalEntries);

        for (DWORD i = 0; i < dwTotalEntries; i++)
        {
            xListGroups.push_back(lpGroups[i].grui0_name);
        }
        SAFE_FREE_NET_BUFF(lpGroups);
    }

    static Boolean Net_CheckUserInGroup(LPCWSTR xDomain, LPCWSTR xUserName, LPCWSTR xGroupName)
    {
        LPGROUP_USERS_INFO_0 lpGroups = nullptr;
        __try
        {
            DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
            DWORD dwEntriesRead = 0;
            DWORD dwTotalEntries = 0;

            WCHAR szDomain[MAX_NAME_LEN] = { 0 };
            wsprintf(szDomain, L"\\\\%s", xDomain);
            ::NetUserGetGroups(szDomain,
                xUserName,
                0,
                (LPBYTE*)&lpGroups,
                dwPrefMaxLen,
                &dwEntriesRead,
                &dwTotalEntries);

            for (DWORD i = 0; i < dwTotalEntries; i++)
            {
                if (wcscmp(lpGroups[i].grui0_name, xGroupName) == 0)
                {
                    return True;
                }
            }
            return False;
        }
        __finally { SAFE_FREE_NET_BUFF(lpGroups); }
    }

    static void Net_GetAllUserInfo(IN OUT MS_USER_INFO_3& xMsUserInfo)
    {
        xMsUserInfo.dwPrefMaxLen = MAX_PREFERRED_LENGTH;
        ::NetUserEnum(
            (wstr(L"\\\\") + xMsUserInfo.szDomain).c_str(),
            xMsUserInfo.dwLevel,
            UF_WORKSTATION_TRUST_ACCOUNT,
            (LPBYTE*)&xMsUserInfo.lpBuf,
            xMsUserInfo.dwPrefMaxLen,
            &xMsUserInfo.dwEntriesRead,
            &xMsUserInfo.dwTotalEntries,
            &xMsUserInfo.dwResumeHandle
        );
    }

    static void Net_GetUserInfo(wstr xDomain, wstr xUserName, OUT MS_USER_INFO_3& xMsUserInfo)
    {
        ::NetUserGetInfo((wstr(L"\\\\") + xDomain).c_str()
            , xUserName.c_str()
            , xMsUserInfo.dwLevel
            , (LPBYTE*)&xMsUserInfo.lpBuf
        );
    }

    static void Net_GetUserInfo(wstr xDomain, wstr xUserName, OUT MS_USER_INFO_4& xMsUserInfo)
    {
        ::NetUserGetInfo((wstr(L"\\\\") + xDomain).c_str()
            , xUserName.c_str()
            , xMsUserInfo.dwLevel
            , (LPBYTE*)&xMsUserInfo.lpBuf
        );
    }
#pragma endregion

#pragma region Kerberos

    static Boolean Kerberos_GetInfo(SecPkgInfo& xSecPkgInfo)
    {
        PSecPkgInfo lpSecPkgInfo = nullptr;
        SECURITY_STATUS xRet = ::QuerySecurityPackageInfo(L"Kerberos", &lpSecPkgInfo);
        if (lpSecPkgInfo)
        {
            memcpy(&xSecPkgInfo, lpSecPkgInfo, sizeof(xSecPkgInfo));
            ::FreeContextBuffer(lpSecPkgInfo);
        }
        return ERROR_SUCCESS == xRet;
    }

    // 必须调用 DeleteSecurityContext 释放 PCtxtHandle pCtxtHandle 
    static Boolean Kerberos_GetRemoteToken(PCtxtHandle pCtxtHandle, LPCWSTR xDomain, LPCWSTR xHost, LPBYTE lpTokenData, DWORD& xTokenLen)
    {
        ULONG xContextAttr = 0;
        CredHandle xCredHandle;
        TimeStamp xTimeStamp;
        if (ERROR_SUCCESS != ::AcquireCredentialsHandle(nullptr, L"Kerberos", SECPKG_CRED_OUTBOUND, nullptr, nullptr, nullptr, nullptr, &xCredHandle, &xTimeStamp))
        {
            return False;
        }

        SecBufferDesc xOutSecBufferDesc;
        SecBuffer xOutSecBuffer;
        xOutSecBufferDesc.ulVersion = 0;
        xOutSecBufferDesc.cBuffers = 1;
        xOutSecBufferDesc.pBuffers = &xOutSecBuffer;

        xOutSecBuffer.cbBuffer = xTokenLen;
        xOutSecBuffer.BufferType = SECBUFFER_TOKEN;
        xOutSecBuffer.pvBuffer = lpTokenData;

        WCHAR szDomainHost[MAX_PATH] = { 0 };
        wsprintf(szDomainHost, L"%s@%s", xHost, xDomain);
        SECURITY_STATUS xRet = ::InitializeSecurityContext(&xCredHandle, nullptr, szDomainHost, 0, 0, SECURITY_NATIVE_DREP, nullptr, 0, pCtxtHandle, &xOutSecBufferDesc, (ULONG*)&xContextAttr, &xTimeStamp);
        ::FreeCredentialsHandle(&xCredHandle);
        xTokenLen = xOutSecBuffer.cbBuffer;
        return ERROR_SUCCESS == xRet;
    }

    // 必须调用 DeleteSecurityContext 释放 PCtxtHandle pCtxtHandle 
    static Boolean Kerberos_AcceptToken(PCtxtHandle pCtxtHandle, LPBYTE lpInTokenData, DWORD& xInTokenLen, LPBYTE lpOutTokenData, DWORD& xOutTokenLen)
    {
        ULONG xContextAttr = 0;
        CredHandle xCredHandle;
        TimeStamp xTimeStamp;
        if (ERROR_SUCCESS != ::AcquireCredentialsHandle(nullptr, L"Kerberos", SECPKG_CRED_INBOUND, nullptr, nullptr, nullptr, nullptr, &xCredHandle, &xTimeStamp))
        {
            return False;
        }

        SecBufferDesc xInSecBufferDesc;
        SecBuffer xInSecBuffer;
        xInSecBufferDesc.ulVersion = 0;
        xInSecBufferDesc.cBuffers = 1;
        xInSecBufferDesc.pBuffers = &xInSecBuffer;
        xInSecBuffer.cbBuffer = xInTokenLen;
        xInSecBuffer.BufferType = SECBUFFER_TOKEN;
        xInSecBuffer.pvBuffer = lpInTokenData;

        SecBufferDesc xOutSecBufferDesc;
        SecBuffer xOutSecBuffer;
        xOutSecBufferDesc.ulVersion = 0;
        xOutSecBufferDesc.cBuffers = 1;
        xOutSecBufferDesc.pBuffers = &xOutSecBuffer;
        xOutSecBuffer.cbBuffer = xOutTokenLen;
        xOutSecBuffer.BufferType = SECBUFFER_TOKEN;
        xOutSecBuffer.pvBuffer = lpOutTokenData;

        SECURITY_STATUS xRet = ::AcceptSecurityContext(&xCredHandle, nullptr, &xInSecBufferDesc, 0, SECURITY_NATIVE_DREP, pCtxtHandle, &xOutSecBufferDesc, &xContextAttr, &xTimeStamp);

        ::FreeCredentialsHandle(&xCredHandle);

        return ERROR_SUCCESS == xRet;
    }
#pragma endregion

#pragma region Service
    static Boolean Service_Exists(LPCWSTR xServiceName)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::OpenService(hSCManager, xServiceName, SC_MANAGER_ALL_ACCESS);
                if (hService)
                {
                    return True;
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }

    static Boolean Service_IsRun(LPCWSTR xServiceName)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::OpenService(hSCManager, xServiceName, SC_MANAGER_ALL_ACCESS);
                if (hService)
                {
                    SERVICE_STATUS xStatus;
                    ::QueryServiceStatus(hService, &xStatus);

                    return xStatus.dwCurrentState == SERVICE_START_PENDING || xStatus.dwCurrentState == SERVICE_RUNNING;
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }

    static Boolean Service_Install(LPCWSTR xServiceFullPathName, LPCWSTR xServiceName, Boolean IsAutoStart)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::CreateService(
                    hSCManager,
                    xServiceName,
                    xServiceName,
                    SC_MANAGER_ALL_ACCESS,
                    SERVICE_WIN32_OWN_PROCESS,
                    IsAutoStart ? SERVICE_AUTO_START : SERVICE_DEMAND_START,
                    SERVICE_ERROR_IGNORE,
                    xServiceFullPathName,
                    nullptr, nullptr, nullptr,
                    _T(".\\LocalSystem"), nullptr);

                if (hService)
                {
                    return True;
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }

    static Boolean Service_Uninstall(LPCWSTR xServiceName)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::OpenService(hSCManager, xServiceName, SC_MANAGER_ALL_ACCESS);
                if (hService)
                {
                    return ::DeleteService(hService);
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }

    static Boolean Service_Start(LPCWSTR xServiceName)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::OpenService(hSCManager, xServiceName, SC_MANAGER_ALL_ACCESS);
                if (hService)
                {
                    return ::StartService(hService, 0, nullptr);
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }

    static Boolean Service_Stop(LPCWSTR xServiceName)
    {
        SC_HANDLE hSCManager = nullptr;
        SC_HANDLE hService = nullptr;
        __try
        {
            hSCManager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
            if (hSCManager)
            {
                hService = ::OpenService(hSCManager, xServiceName, SC_MANAGER_ALL_ACCESS);
                if (hService)
                {
                    SERVICE_STATUS xStatus;
                    return ::ControlService(hService, SERVICE_CONTROL_STOP, &xStatus);
                }
            }
        }
        __finally
        {
            if (hService) { ::CloseServiceHandle(hService); }
            if (hSCManager) { ::CloseServiceHandle(hSCManager); }
        }
        return False;
    }
#pragma endregion
};
