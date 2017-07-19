#include "Precompile.h"

#define MAX_CLIENT 10000000
//#define MAX_THREAD 3
#define SERVER_HOST "1.1.1.11"
#define SERVER_PORT 19998


enum E_PACKET_INDEX
{
    TEST,
};

Boolean Test_NetExecutePacket(MsNetTerminal& xMsNetTerminal, MsNetPacket& xPacket);
INT_PTR DialogProcCommand_Test_100(HWND hDlg);
INT_PTR DialogProcCommand_Test_101(HWND hDlg);
void TestFun(Int32 x, Int32 y, Int32 z);

MsNetPacketFactory* g_lpMsNetPacketFactory = NEW MsNetPacketFactory();

INT32 APIENTRY _tWinMain(
    _In_ HINSTANCE      hInstance,
    _In_opt_ HINSTANCE  hPrevInstance,
    _In_ LPTSTR         lpCmdLine,
    _In_ INT32          nCmdShow)
{
    USE_PARAM(hPrevInstance);
    USE_PARAM(lpCmdLine);
    USE_PARAM(nCmdShow);

    BEGIN_INIT;

    Int32 xRet = (Int32)::DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), nullptr, MainDialogProc);

    END_CLEAR;

    return xRet;
}

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT32 xMsgIndex, WPARAM wParam, LPARAM lParam)
{
    USE_PARAM(lParam);

    switch (xMsgIndex)
    {
        case WM_INITDIALOG:{ return DialogInit(hDlg); }
        case WM_CLOSE:{ return DialogClose(hDlg); }
        case WM_COMMAND:{ return DialogProcCommand(hDlg, LOWORD(wParam)); }break;
    }
    return (INT_PTR)FALSE;
}

static MsNetTerminal* g_lpClient = nullptr;
static Boolean g_IsRun = True;
static Boolean g_IsStop = True;

INT_PTR DialogInit(HWND hDlg)
{
    USE_PARAM(hDlg);

    g_lpMsNetServer = NEW MsNetServer(g_lpMsNetPacketFactory);
    g_lpMsNetServer->CreateServer(SERVER_HOST, SERVER_PORT, MAX_CLIENT);

    return (INT_PTR)TRUE;
}

INT_PTR DialogClose(HWND hDlg)
{
    USE_PARAM(hDlg);

    g_IsRun = False;
    while (!g_IsStop);

    DELETE_RULE(g_lpMsNetClient);
    DELETE_RULE(g_lpMsNetServer);
    DELETE_RULE(g_lpMsNetPacketFactory);

    ::PostQuitMessage(0);

    return (INT_PTR)TRUE;
}

INT_PTR DialogProcCommand(HWND hDlg, DWORD xControlId)
{
    switch (xControlId)
    {
        case ID_EXIT:{ return ::SendMessage(hDlg, WM_CLOSE, 0, 0); }break;
        case ID_TEST:{ return DialogProcCommand_Test(hDlg); }break;
        case ID_TEST_100:{ return DialogProcCommand_Test_100(hDlg); }break;
        case ID_TEST_101:{ return DialogProcCommand_Test_101(hDlg); }break;
    }
    return (INT_PTR)FALSE;
}

DWORD WINAPI _NetClientThreadProc(LPVOID)
{
    while (g_IsRun)
    {
        g_lpMsNetClient->CreateClient(SERVER_HOST, SERVER_PORT, g_lpMsNetPacketFactory);
        ::Sleep(1);
    }
    g_IsStop = True;

    return 0;
}


INT_PTR DialogProcCommand_Test_100(HWND hDlg)
{
    USE_PARAM(hDlg);

    if (!g_lpMsNetClient)
    {
        g_lpMsNetClient = NEW MsNetClient();
    }

    DWORD dwThreadId;
    g_IsStop = False;
    ::CreateThread(nullptr, 0, &_NetClientThreadProc, nullptr, 0, &dwThreadId);

    return (INT_PTR)TRUE;
}

INT_PTR DialogProcCommand_Test_101(HWND hDlg)
{
    USE_PARAM(hDlg);

    Char szText[256] = { 0 };
    Int32 xCount = g_lpMsNetServer->GetTerminalCount();
    itoa(xCount, szText, 10);
    MessageBoxA(hDlg, szText, "个数", MB_OK);

    //YH_Proto_Test xYH_Proto_Test;
    //xYH_Proto_Test.set_yh_name("尹辉");
    //xYH_Proto_Test.set_yh_sexuality("未知或多性");

    //MsNetPacket xMsNetPacket;
    //xMsNetPacket.FromProto(10000, xYH_Proto_Test);

    //MsNetTerminal xMsNetTerminal;
    //g_lpMsNetPacketFactory->ExecutePacket(xMsNetTerminal, xMsNetPacket);

    //RoleBaseInfo xRoleBaseInfo;
    //xRoleBaseInfo.set_level(100);

    //MsNetPacket xMsNetPacket;
    //xMsNetPacket.FromProto(100, xRoleBaseInfo);

    //MsNetPacket xMsNetPacket;
    //xMsNetPacket.FromProto()
    //g_lpClient->SendPacket(xMsNetPacket);

    //MsNetTerminal xMsNetTerminal;
    //g_lpMsNetPacketFactory->ExecutePacket(xMsNetTerminal, xMsNetPacket);

    return (INT_PTR)TRUE;
}

INT_PTR DialogProcCommand_Test(HWND hDlg)
{
    RoleBaseInfo xRoleBaseInfo;
    xRoleBaseInfo.set_level(101);

    MsNetPacket xMsNetPacket;
    xMsNetPacket.FromProto(101, xRoleBaseInfo);

    //RoleBaseInfo xxRoleBaseInfo;
    //xMsNetPacket.ToProto(xxRoleBaseInfo);

    MsNetTerminal xMsNetTerminal;
    g_lpMsNetPacketFactory->ExecutePacket(xMsNetTerminal, xMsNetPacket);

    ::MessageBox(hDlg, _T("123"), _T("456"), MB_OK);
    return (INT_PTR)TRUE;
}

//Boolean _Test_NetExecutePacket100(MsNetTerminal& xMsNetTerminal, MsNetPacket& xPacket);
//MsNetPacketRegisterAssist g_ms_auto_RoleBaseInfo100(g_lpMsNetPacketFactory, 100, _Test_NetExecutePacket100);
//Boolean Test_NetExecutePacket100(MsNetTerminal& xMsNetTerminal, RoleBaseInfo& xRoleBaseInfo);
//Boolean _Test_NetExecutePacket100(MsNetTerminal& xMsNetTerminal, MsNetPacket& xPacket)
//{
//    RoleBaseInfo xRoleBaseInfo;
//    if (xPacket.ToProto(xRoleBaseInfo))
//    {
//        return Test_NetExecutePacket100(xMsNetTerminal, xRoleBaseInfo);
//    }
//    return False;
//}
//Boolean Test_NetExecutePacket100(MsNetTerminal& xMsNetTerminal, RoleBaseInfo& xRoleBaseInfo)
AUTO_REGISTER(g_lpMsNetPacketFactory, RoleBaseInfo, 100, Test_NetExecutePacket100)
{
    USE_PARAM(xMsNetTerminal, xRoleBaseInfo);
    if (100 == xRoleBaseInfo.level())
    {
        ::MessageBox(nullptr, _T("包收到100"), _T("RoleBaseInfo"), MB_OK);
        return True;
    }
    return False;
}

AUTO_REGISTER(g_lpMsNetPacketFactory, PetBaseInfo, 101, Test_NetExecutePacket101)
{
    USE_PARAM(xMsNetTerminal, xPetBaseInfo);
    //if (101 == xPetBaseInfo.level())
    {
        ::MessageBoxA(nullptr, xPetBaseInfo.name().c_str(), "包收到宠物消息", MB_OK);
        return True;
    }
    //return False;
}

AUTO_REGISTER(g_lpMsNetPacketFactory, YH_Proto_Test, 10000, Test_NetExecutePacket)
{
    USE_PARAM(xMsNetTerminal, xYH_Proto_Test);
    //if (101 == xPetBaseInfo.level())
    {
        ::MessageBoxA(nullptr, xYH_Proto_Test.yh_name().c_str(), xYH_Proto_Test.yh_sexuality().c_str(), MB_OK);
        return True;
    }
    //return False;
}


void TestFun(Int32 x, Int32 y, Int32 z)
{
    USE_PARAM(x, y, z);
}

//MsNetPacketRegisterAssist g_ms_auto_MsNetPacketRegisterAssist(g_lpMsNetPacketFactory, 100, Test_NetExecutePacket);
//Boolean Test_NetExecutePacket(MsNetTerminal& xMsNetTerminal, MsNetPacket& xPacket)
//{
//    USE_PARAM(xMsNetTerminal, xPacket);
//
//    RoleBaseInfo xRoleBaseInfo;
//    if (xPacket.ToProto(xRoleBaseInfo))
//    {
//        if (100 == xRoleBaseInfo.level())
//        {
//            ::MessageBox(nullptr, _T("包收到100"), _T("RoleBaseInfo"), MB_OK);
//            return True;
//        }
//    }
//    return False;
//}
