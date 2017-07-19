#include "Precompiled.h"
MsBaseService* g_lpMsBaseService = nullptr;

MsBaseService::MsBaseService(HINSTANCE hInstance, LPCWSTR xServiceName, LPCWSTR lpCmdLine)
    : m_hInstance(hInstance)
    , m_Exists(False)
    , m_IsRun(False)
    , m_IsDebug(False)
    , m_hService(nullptr)
    , m_IsAutoRun(False)
    , m_RunHideWnd(False)
{
    SET_DIR2FILEPATH;

    m_IsAutoRun = wcsstr(lpCmdLine, L"-autorun") ? true : false;
    m_RunHideWnd = wcsstr(lpCmdLine, L"-hidedlg") ? true : false;

    LPCWSTR xTemp = wcsstr(lpCmdLine, L"-config ");
    if (xTemp && wcslen(xTemp) > 8)
    {
        m_ConfigFileName = MsBase::W2M(xTemp + 8);
    }

    if (wcsstr(lpCmdLine, L"-install"))
    {
        m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_INSTALL_SERVICE;
    }
    else if (wcsstr(lpCmdLine, L"-uninstall"))
    {
        m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_UNINSTALL_SERVICE;
    }
    else if (wcsstr(lpCmdLine, L"-uninstall"))
    {
        m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_UNINSTALL_SERVICE;
    }
    else if (wcsstr(lpCmdLine, L"-run"))
    {
        m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_RUN_SERVICE;
    }
    else if (wcsstr(lpCmdLine, L"-stop"))
    {
        m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_STOP_SERVICE;
    }

    Char szPath[MAX_PATH];
    ::GetModuleFileNameA(::GetModuleHandle(nullptr), szPath, MAX_PATH);
    *(strrchr(szPath, '\\') + 1) = '\0';
    ::SetCurrentDirectoryA(szPath);

    ZeroObject(m_Status);
    wcsncpy_s(m_ServiceName, MAX_SERVICE_NAME_LEN, xServiceName, _TRUNCATE);
    g_lpMsBaseService = this;
}

Boolean MsBaseService::Init(HWND hDlg)
{
    HWND hButtonInstallUninstall = ::GetDlgItem(hDlg, IDC_BUTTON_INSTALL_UNINSTALL);
    HWND hButtonStartStop = ::GetDlgItem(hDlg, IDC_BUTTON_START_STOP);
    //HWND hStaticCaption = ::GetDlgItem(hDlg, IDC_STATIC_CAPTION);

    m_Exists = MsService::Service_Exists(m_ServiceName);
    m_IsRun = MsService::Service_IsRun(m_ServiceName);

    ::SetWindowText(hDlg, m_ServiceName);
    ::SetWindowText(hButtonInstallUninstall, m_Exists ? L"卸载" : L"安装");
    ::SetWindowText(hButtonStartStop, m_IsRun ? L"停止" : L"启动");

    return True;
}

Boolean MsBaseService::InstallUninstall()
{
    if (m_Exists)
    {
        return MsService::Service_Uninstall(m_ServiceName);
    }
    else
    {
        WCHAR szExePath[MAX_PATH] = { 0 };
        ::GetModuleFileName(nullptr, szExePath, MAX_PATH);
        return MsService::Service_Install(szExePath, m_ServiceName, False);
    }
}

Boolean MsBaseService::Start_Stop()
{
    if (m_IsRun)
    {
        return MsService::Service_Stop(m_ServiceName);
    }
    else
    {
        return MsService::Service_Start(m_ServiceName);
    }
}

void WINAPI MsBaseService::S_Handler(DWORD dwControl)throw()
{
    if (g_lpMsBaseService)
    {
        return g_lpMsBaseService->_Handler(dwControl);
    }
}

void MsBaseService::_Handler(DWORD dwControl)throw()
{
    __try
    {
        switch (dwControl)
        {
            case SERVICE_CONTROL_STOP:
            {
                ::InterlockedExchange(&m_Status.dwCurrentState, SERVICE_STOP_PENDING);
                ::SetServiceStatus(m_hService, &m_Status);
                this->OnStop();
                ::InterlockedExchange(&m_Status.dwCurrentState, SERVICE_STOPPED);
                ::SetServiceStatus(m_hService, &m_Status);
            }break;

            case SERVICE_CONTROL_PAUSE: { this->OnPause(); }break;
            case SERVICE_CONTROL_CONTINUE: { this->OnContinue(); }break;
            case SERVICE_CONTROL_SHUTDOWN: { this->OnShutdown(); }break;
            default: {}break;
        }
    }
    __except (MsBase::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
}

void WINAPI MsBaseService::S_ServiceMain(_In_ DWORD dwArgc, _In_reads_(dwArgc) _Deref_pre_z_ LPTSTR* lpszArgv) throw()
{
    if (g_lpMsBaseService)
    {
        return g_lpMsBaseService->_ServiceMain(dwArgc, lpszArgv);
    }
}

void MsBaseService::_ServiceMain(_In_ DWORD dwArgc, _In_reads_(dwArgc) _Deref_pre_z_ LPTSTR* lpszArgv) throw()
{
    USE_PARAM(lpszArgv, dwArgc);

    __try
    {
        m_hService = ::RegisterServiceCtrlHandler(m_ServiceName, S_Handler);

        m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        m_Status.dwCurrentState = SERVICE_STOPPED;
        m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
        m_Status.dwWin32ExitCode = 0;
        m_Status.dwServiceSpecificExitCode = 0;
        m_Status.dwCheckPoint = 0;
        m_Status.dwWaitHint = 0;

        DWORD dwState = SERVICE_START_PENDING;
        ::InterlockedExchange(&m_Status.dwCurrentState, dwState);
        ::SetServiceStatus(m_hService, &m_Status);

        m_Status.dwWin32ExitCode = S_OK;
        m_Status.dwCheckPoint = 0;
        m_Status.dwWaitHint = 0;

        this->OnStart();

        if (::InterlockedCompareExchange(&m_Status.dwCurrentState, SERVICE_RUNNING, SERVICE_START_PENDING) == SERVICE_START_PENDING)
        {
            ::SetServiceStatus(m_hService, &m_Status);
        }
    }
    __except (MsBase::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
}


INT_PTR CALLBACK MsBaseService::S_DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (g_lpMsBaseService)
    {
        return g_lpMsBaseService->DialogProc(hDlg, message, wParam, lParam);
    }
    return False;
}

INT_PTR MsBaseService::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            Boolean xRet = this->Init(hDlg);
            if (m_IsAutoRun)
            {
                switch (m_ServiceRunType)
                {
                    case ESRT_INSTALL_SERVICE:
                    case ESRT_UNINSTALL_SERVICE:
                    {
                        ::PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_INSTALL_UNINSTALL, 0), 0);
                    }break;

                    case ESRT_STOP_SERVICE:
                    case ESRT_RUN_SERVICE:
                    {
                        ::PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_START_STOP, 0), 0);
                    }break;

                    case ESRT_UNKNOW:
                    default:
                    {
                        ::PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_DEBUG, 0), 0);
                    }break;
                }
            }
            if (m_RunHideWnd) { ::ShowWindow(hDlg, SW_MINIMIZE); }
            return xRet;
        }break;

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case HELP_QUIT:
                {
                    if (m_IsDebug) { ::PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_DEBUG, 0), 0); }
                    ::PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BUTTON_CLOSE, 0), 0);
                }break;

                case IDC_BUTTON_INSTALL_UNINSTALL:
                {
                    if (this->InstallUninstall())
                    {
                        if (this->m_Exists)
                        {
                            m_RunHideWnd ? NULL : ::MessageBox(hDlg, L"服务卸载成功!", L"成功", MB_OK);
                            ::EndDialog(hDlg, 0);
                        }
                        else
                        {
                            m_RunHideWnd ? NULL : ::MessageBox(hDlg, L"服务安装成功!", L"成功", MB_OK);
                            ::EndDialog(hDlg, 0);
                        }
                    }
                    else
                    {
                        m_RunHideWnd ? NULL : ::MessageBox(hDlg, MsBase::GetLastErrorStringW().c_str(), L"失败", MB_OK);
                    }
                }break;

                case IDC_BUTTON_START_STOP:
                {
                    if (this->Start_Stop())
                    {
                        if (this->m_IsRun)
                        {
                            m_RunHideWnd ? NULL : ::MessageBox(hDlg, L"服务停止成功!", L"成功", MB_OK);
                            ::EndDialog(hDlg, 0);
                        }
                        else
                        {
                            m_RunHideWnd ? NULL : ::MessageBox(hDlg, L"服务启动成功!", L"成功", MB_OK);
                            ::EndDialog(hDlg, 0);
                        }
                    }
                    else
                    {
                        m_RunHideWnd ? NULL : ::MessageBox(hDlg, MsBase::GetLastErrorStringW().c_str(), L"失败", MB_OK);
                    }
                }break;

                case IDC_BUTTON_DEBUG:
                {
                    HWND hButtonDebug = ::GetDlgItem(hDlg, IDC_BUTTON_DEBUG);
                    if (this->m_IsDebug)
                    {
                        ::SetWindowText(hButtonDebug, L"开始调试");
                        this->OnStop();
                        this->m_IsDebug = False;
                        ::EnableWindow(::GetDlgItem(hDlg, IDC_BUTTON_INSTALL_UNINSTALL), True);
                        ::EnableWindow(::GetDlgItem(hDlg, IDC_BUTTON_START_STOP), True);
                    }
                    else
                    {
                        this->OnStart();
                        this->m_IsDebug = True;
                        ::SetWindowText(hButtonDebug, L"停止调试");
                        ::EnableWindow(::GetDlgItem(hDlg, IDC_BUTTON_INSTALL_UNINSTALL), False);
                        ::EnableWindow(::GetDlgItem(hDlg, IDC_BUTTON_START_STOP), False);
                    }
                }break;

                case IDC_BUTTON_CLOSE:
                {
                    this->OnShutdown();
                    ::EndDialog(hDlg, LOWORD(wParam));
                }break;
            }
        }break;
    }
    return (INT_PTR)FALSE;
}

Int32 MsBaseService::ServiceRun()
{
    if (MsService::Token_IsSystem())
    {
        SERVICE_TABLE_ENTRY st[] = { { m_ServiceName, S_ServiceMain } };
        if (!::StartServiceCtrlDispatcher(st))
        {
            return GetLastError();
        }
    }
    else
    {
        return (Int32)::DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), nullptr, S_DialogProc);
    }
    return 0;
}

