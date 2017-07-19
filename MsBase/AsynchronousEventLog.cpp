#include "Precompiled.h"

void AsynchronousEventLog::OnStart()
{
    this->Run(true);
};

void AsynchronousEventLog::OnStop()
{
    if (!this->IsFree())
    {
        this->Stop();
        this->Wait();
    }
};

void AsynchronousEventLog::OnShutdown()
{
    if (!this->IsFree())
    {
        this->Stop();
        this->Wait();
    }
};

//Int32 MsMain()
//{
//    __try
//    {
//        BEGIN_INIT;
//        g_CodeStatisticsService.ServiceRun();
//        END_CLEAR;
//        return 0;
//    }
//    __except (MsBaseDef::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER){}
//    return (DWORD)-1;
//}
//
//
//Int32 APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ Int32 nCmdShow)
//{
//    return MsMain();
//}
