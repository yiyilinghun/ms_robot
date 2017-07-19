#include "Precompiled.h"

MS_USER_INFO_3::MS_USER_INFO_3()
    : lpBuf(nullptr)
    , dwLevel(3)
    , dwPrefMaxLen(0)
    , dwEntriesRead(0)
    , dwTotalEntries(0)
    , dwResumeHandle(0)
{
    ;
}

MS_USER_INFO_3::~MS_USER_INFO_3()
{
    if (lpBuf)
    {
        SAFE_FREE_NET_BUFF(lpBuf);
    }
}

MS_USER_INFO_4::MS_USER_INFO_4()
    : lpBuf(nullptr)
    , dwLevel(4)
    , dwPrefMaxLen(0)
    , dwEntriesRead(0)
    , dwTotalEntries(0)
    , dwResumeHandle(0)
{
    ;
}

MS_USER_INFO_4::~MS_USER_INFO_4()
{
    if (lpBuf)
    {
        SAFE_FREE_NET_BUFF(lpBuf);
    }
}