#include "Precompiled.h"

MsThread::MsThread(mstr xThreadName, MS_THREAD_TICK xTickFun, LPVOID lpThreadParameter, Int32 xTickInterval, Boolean IsAutoRun, DWORD dwStackSize)
    : m_ThreadName(xThreadName)
    , TickFunction(xTickFun)
    , m_lpThreadParameter(lpThreadParameter)
    , m_TickInterval(xTickInterval)
    , m_IsRun(False)
    , m_IsRuned(False)
    , m_dwStackSize(dwStackSize)
    , m_IsSuspend(False)
    , m_IsCreateThreadMsgQueue(False)
    , m_LastCheckPressureTime(MsBase::Now())
    , m_Pressure(INVALID_UID)
{
    if (IsAutoRun)
    {
        m_Thread = ::CreateThread(nullptr, dwStackSize, &_ThreadProc, this, 0, &m_dwThreadId);
        m_IsSuspend = False;

    }
    else
    {
        m_Thread = ::CreateThread(nullptr, dwStackSize, &_ThreadProc, this, CREATE_SUSPENDED, &m_dwThreadId);
        m_IsSuspend = True;
    }
}

Boolean MsThread::PostThreadMsg(MsThreadMsg* lpMsg)
{
    lpMsg->m_Time = MsBase::Now();
    lpMsg->m_TargetThreadId = m_dwThreadId;
    return ::PostThreadMessage(m_dwThreadId, lpMsg->m_Message, 0, (LPARAM)lpMsg);
}

Boolean MsThread::Run(Boolean IsReStart)
{
    if (IsReStart && IsCompleted())
    {
        m_Thread = ::CreateThread(nullptr, m_dwStackSize, &_ThreadProc, this, 0, &m_dwThreadId);
        while (!m_IsCreateThreadMsgQueue) { ::Sleep(1); }
        m_IsSuspend = False;
        return True;
    }
    else
    {
        if (INVALID_DVALUE != ::ResumeThread(m_Thread))
        {
            m_IsSuspend = False;
            ::Sleep(10);
            return True;
        }
        else
        {
            return False;
        }
    }
}

void MsThread::Stop()
{
    m_IsRun = False;
    m_IsCreateThreadMsgQueue = False;
}

void MsThread::WaitThread(void)
{
    if (m_IsRuned)
    {
        ::WaitForSingleObject(m_Thread, INFINITE);
    }
}

Boolean MsThread::IsCompleted(void)
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject(m_Thread, 0);
}

Boolean MsThread::IsRun(void)
{
    return m_IsRun;
}

Boolean MsThread::IsFree(void)
{
    return IsCompleted() || !m_IsRun;
}

void MsThread::Suspend(Boolean IsForceImmediately)
{
    if (IsForceImmediately)
    {
        ::SuspendThread(m_Thread);
    }
    else
    {
        m_IsSuspend = True;
    }
}

DWORD MsThread::ThreadProc()
{
    while (m_IsRun)
    {
        if (m_IsSuspend) { ::SuspendThread(m_Thread); }
        MsBase::TimeSpanCheckPressure(this->m_LastCheckPressureTime, this->m_Pressure);
        if (m_Pressure < m_TickInterval)
        {
            ::SleepEx((DWORD)(m_TickInterval - m_Pressure), True);
        }
        if (this->TickFunction) { this->TickFunction(this->m_lpThreadParameter); }
    }
    return 0;
}

DWORD MsThread::_ThreadProc(LPVOID lpParameter)
{
    MsThread* lpMsThread = (MsThread*)lpParameter;
    MsBase::SetThreadName(lpMsThread->m_ThreadName.c_str());
    lpMsThread->m_IsRuned = True;
    lpMsThread->m_IsRun = True;
    lpMsThread->m_IsCreateThreadMsgQueue = True;

MS_LOOP_TRY_AGAIN:
    __try
    {
        return lpMsThread->ThreadProc();
    }
    __except (MsBase::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
    goto MS_LOOP_TRY_AGAIN;
}
