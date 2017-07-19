#pragma once

class AsynchronousEventLog : public MsThread
{
public:

    AsynchronousEventLog(mstr xThreadName) : MsThread(xThreadName, nullptr, 8 * MB_SIZE, this, 1, False, True)
    {
        ;
    }

    Boolean PostLog(wstr xLog)
    {
        wstr* lpLog = NEW wstr();
        (*(lpLog)) = xLog;
        return this->PostThreadMsg(NEW MsThreadMsg(nullptr, 0, 0, lpLog));
    }

protected:
    virtual void OnStart();
    virtual void OnStop();
    virtual void OnShutdown();

private:
    virtual Boolean OnThreadMsg(LP_MS_THREAD_MSG lpMsThreadMsg)
    {
        if (lpMsThreadMsg)
        {
            switch (lpMsThreadMsg->m_Message)
            {
            case 0:
            {
                //wstr* lpLog = (wstr*)(lpMsThreadMsg->m_lParam);
                //lpLog->c_str();
                //((AsynchronousEventLog*)(&(lpMsThreadMsg->m_SourceMsThread)))->PostLogB(*this, lpLog->c_str());
                //SAFE_DELETE(lpLog);
            }break;
            }
        }
        return True;
    }
};

