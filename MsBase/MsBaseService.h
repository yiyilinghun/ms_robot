#pragma once

#define MAX_SERVICE_NAME_LEN 100
enum E_SERVICE_RUN_TYPE
{
    ESRT_UNKNOW,
    ESRT_INSTALL_SERVICE,
    ESRT_UNINSTALL_SERVICE,
    ESRT_RUN_SERVICE,
    ESRT_STOP_SERVICE,
};

class MsBaseService
{
public:
    MsBaseService(HINSTANCE hInstance, LPCWSTR xServiceName, LPCWSTR lpCmdLine);
    Boolean Init(HWND hDlg);
    Boolean InstallUninstall();
    Boolean Start_Stop();

    void static WINAPI S_Handler(DWORD dwControl)throw();
    void _Handler(DWORD dwControl)throw();


    void static WINAPI S_ServiceMain(_In_ DWORD dwArgc, _In_reads_(dwArgc) _Deref_pre_z_ LPTSTR* lpszArgv) throw();
    void _ServiceMain(_In_ DWORD dwArgc, _In_reads_(dwArgc) _Deref_pre_z_ LPTSTR* lpszArgv) throw();

    Int32 ServiceRun();

    INT_PTR static CALLBACK S_DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    E_SERVICE_RUN_TYPE m_ServiceRunType = E_SERVICE_RUN_TYPE::ESRT_UNKNOW;
    Boolean m_IsAutoRun;
    Boolean m_RunHideWnd;

    mstr m_ConfigFileName;

protected:
    virtual Boolean OnStart() = 0;
    virtual Boolean OnStop() = 0;
    virtual Boolean OnShutdown() = 0;
    virtual Boolean OnPause() { return True; };
    virtual Boolean OnContinue() { return True; };

protected:
    HINSTANCE m_hInstance;
    Boolean m_Exists;
    Boolean m_IsRun;
    Boolean m_IsDebug;
    WCHAR m_ServiceName[MAX_SERVICE_NAME_LEN];
    SERVICE_STATUS_HANDLE m_hService;
    SERVICE_STATUS m_Status;
};
