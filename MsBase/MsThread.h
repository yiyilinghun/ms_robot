#pragma once
#include <winbase.h>

#define IocpUnitPtr SafePtr<IocpUnit>
#define UNLIMITED_COUNT 0x7FFFFFFFFFFFFFFF

typedef void(*MS_THREAD_TICK)(LPVOID lpThreadParameter);
class MsThread;
typedef struct MsThreadMsg
{
    MsThreadMsg(MsThread* xSourceMsThread) :m_SourceMsThread(xSourceMsThread) { ; }
    MsThreadMsg(MsThread* xSourceMsThread, DWORD xMessage, LPVOID xwParam, LPVOID xlParam) :m_SourceMsThread(xSourceMsThread)
    {
        m_Message = xMessage;
        m_wParam = xwParam;
        m_lParam = xlParam;
        m_SourceThreadId = ::GetCurrentThreadId();
    }

    DWORD       m_Message;
    LPVOID      m_wParam;
    LPVOID      m_lParam;
    Int64       m_Time;
    DWORD       m_TargetThreadId;
    DWORD       m_SourceThreadId;
    MsThread*   m_SourceMsThread;
}*LP_MS_THREAD_MSG;

class MsThread
{
public:
    MsThread(mstr xThreadName, MS_THREAD_TICK xTickFun, LPVOID lpThreadParameter, Int32 xTickInterval, Boolean IsAutoRun, DWORD dwStackSize = 100 * MB_SIZE);

    Boolean PostThreadMsg(MsThreadMsg* lpMsg);
    Boolean Run(Boolean IsReStart = False);
    void Stop();
    void WaitThread(void);
    Boolean IsCompleted(void);
    Boolean IsRun(void);
    Boolean IsFree(void);
    void Suspend(Boolean IsForceImmediately);

    //DWORD ThreadProc(MSG& xMsg);
    DWORD ThreadProc();
protected:
    //void SetTimer(IocpUnit* lpUnit, DWORD dwEventId, DWORD xInterval, QWORD xTickCount = UNLIMITED_COUNT);
    //void KillTimer(iocp_ol* lpIocpol);
    //void KillTimer(IocpUnit* lpUnit, DWORD dwEventId);
    //iocp_ol* GetTimer(IocpUnit* lpUnit, DWORD dwEventId);

    virtual Boolean OnThreadMsg(LP_MS_THREAD_MSG) { return True; }

    mstr m_ThreadName;
    MS_THREAD_TICK TickFunction;
    Int32 m_TickInterval;

    Int64 GetThreadPressure() { return m_Pressure; }

public:
    //void _ThreadExeTimer();
    static DWORD WINAPI _ThreadProc(LPVOID lpParameter);

    //MsList<iocp_ol*> m_ListTimer;
    //MsDictionary<IocpUnit*, MsDictionary<DWORD, iocp_ol*>> m_Dict_Timer;

    Int64 m_Pressure;
    Int64 m_LastCheckPressureTime;

    Boolean m_IsRuned;
    Boolean m_IsRun;
    DWORD m_dwStackSize;
    Boolean m_IsSuspend;
    Boolean m_IsCreateThreadMsgQueue;
    LPVOID m_lpThreadParameter;
    DWORD m_dwThreadId;
    HANDLE m_Thread;
};

