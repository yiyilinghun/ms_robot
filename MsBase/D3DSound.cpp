#include "Precompiled.h"
#include "D3DSound.h"

const WCHAR* g_cwErrorMsg[7] = {
    TEXT(""),
    TEXT("Pelase choose a file!"),
    TEXT("Only supports .mp3 and .wav file!"),
    TEXT("Create PCM buffer fail!"),
    TEXT("Create MPG buffer fail!"),
    TEXT("Cannot play the file!"),
    TEXT("File format error!")
};
WCHAR* g_cwSongList[6] = {
    TEXT("序号"),
    TEXT("名称"),
    TEXT("持续时间"),
    TEXT("扩展名"),
    TEXT("比特率"),
    TEXT("文件路径")
};



myD3DSound*    g_pmySound = NULL;


///////////////////////
//public function
///////////////////////

HRESULT myD3DSound::myInit(HWND hWnd)
{
    m_father = hWnd;
    if (FAILED(DirectSoundCreate8(NULL, &m_pDirectSound, NULL)))
        return E_FAIL;

    if (FAILED(m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
        return E_FAIL;

    return S_OK;
}

myD3DSound::myD3DSound() :
    m_pDirectSound(NULL),
    m_pBuffer8(NULL),
    m_father(NULL),
    m_openbtn(NULL),
    m_closebtn(NULL),
    m_playbtn(NULL),
    m_pausebtn(NULL),
    m_stopbtn(NULL),
    m_txtplaying(NULL),
    m_songtxt(NULL),
    m_timeshow(NULL),
    m_songinfo(NULL),
    m_songtime(NULL),
    m_scrollbar(NULL),
    m_volumebar(NULL),
    m_listview(NULL),
    m_fp(NULL),
    isPlaying(FALSE),
    m_bmpg(FALSE),
    m_factwav(FALSE),
    m_hThread(NULL),
    m_hThread2(NULL),
    m_mpghandle(NULL)
{

    m_ds_dwBuffSize = 0;
#ifdef __MAX_BUFFER__
    m_ds_dwFileSize = 0;
#endif
    m_ds_dwFileTime = 0;
    m_ds_dwFilebps = 0;
    m_ds_dwPos = 0;
    m_ds_dwLeave = 0;
    m_iScrollPos = 0;
    m_iVolumePos = 50;

    m_thread_id = 0;
    m_dwPlayPos = 0;
    ZeroMemory(&opfn, sizeof(OPENFILENAME));
    memset(m_wstrTime, 0, sizeof(m_wstrTime));
    memset(m_wSongTime, 0, sizeof(m_wSongTime));
    memset(m_wSongLeave, 0, sizeof(m_wSongLeave));
    memset(m_wSongPath, 0, sizeof(m_wSongPath));
    memset(m_wSongName, 0, sizeof(m_wSongName));
    memset(m_wSongPathPre, 0, sizeof(m_wSongPathPre));
    memset(m_wSongNamePre, 0, sizeof(m_wSongNamePre));
    m_list.clear();
    g_pmySound = this;
}

myD3DSound::~myD3DSound()
{
    SAFE_RELEASE(m_pBuffer8);
    SAFE_RELEASE(m_pDirectSound);
    ZeroMemory(&opfn, sizeof(OPENFILENAME));
    memset(m_wstrTime, 0, sizeof(m_wstrTime));
    memset(m_wSongTime, 0, sizeof(m_wSongTime));
    memset(m_wSongLeave, 0, sizeof(m_wSongLeave));
    memset(m_wSongPath, 0, sizeof(m_wSongPath));
    memset(m_wSongName, 0, sizeof(m_wSongName));
    memset(m_wSongPathPre, 0, sizeof(m_wSongPathPre));
    memset(m_wSongNamePre, 0, sizeof(m_wSongNamePre));
    if (m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
    m_list.clear();
    cleanup();
}


//
//void myD3DSound::myCreateWin(HWND hWnd)
//{
//    RECT dynamic_rc;
//    RECT button_rc;
//    RECT static_rc;
//    RECT list_rc;
//    GetClientRect(hWnd, &dynamic_rc);
//    LONG Width = dynamic_rc.right - dynamic_rc.left;
//    LONG Height = dynamic_rc.bottom - dynamic_rc.top;
//
//    button_rc.left = 15;
//    button_rc.top = 15;
//    m_openbtn = CreateWindow(TEXT("BUTTON"), TEXT("Add"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
//        button_rc.left, button_rc.top, 50, 20, hWnd, (HMENU)IDB_OPEN, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    button_rc.left += 60;
//    m_closebtn = CreateWindow(TEXT("BUTTON"), TEXT("Del"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
//        button_rc.left, button_rc.top, 50, 20, hWnd, (HMENU)IDB_CLOSE, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    button_rc.left += 60;
//    m_playbtn = CreateWindow(TEXT("BUTTON"), TEXT("Play"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
//        button_rc.left, button_rc.top, 50, 20, hWnd, (HMENU)IDB_PLAY, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    button_rc.left += 60;
//    m_pausebtn = CreateWindow(TEXT("BUTTON"), TEXT("Pause"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
//        button_rc.left, button_rc.top, 50, 20, hWnd, (HMENU)IDB_PAUSE, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    button_rc.left += 60;
//    m_stopbtn = CreateWindow(TEXT("BUTTON"), TEXT("Stop"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
//        button_rc.left, button_rc.top, 50, 20, hWnd, (HMENU)IDB_STOP, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    button_rc.left += 60;
//    m_scrollbar = CreateWindow(TRACKBAR_CLASS, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER | TBS_HORZ | TBS_TOOLTIPS,
//        button_rc.left, button_rc.top, 275, 20, hWnd, (HMENU)12, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//    //static
//    static_rc.left = 15;
//    static_rc.top = 50;
//    m_txtplaying = CreateWindow(TEXT("STATIC"), TEXT("playing now:"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_OWNERDRAW,
//        static_rc.left, static_rc.top, 90, 20, hWnd, (HMENU)IDB_PLAYING, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    static_rc.left += (90 + 10);
//    m_songtxt = CreateWindow(TEXT("STATIC"), TEXT("无文件"), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_LEFT | SS_OWNERDRAW,
//        static_rc.left, static_rc.top, 560, 20, hWnd, (HMENU)IDB_SONGTEXT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    static_rc.left = 15;
//    static_rc.top = Height - 5 - 16;
//    m_songinfo = CreateWindow(TEXT("STATIC"), TEXT("请选择wav文件进行播放。"), WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER | SS_OWNERDRAW,
//        static_rc.left, static_rc.top, 350, 16, hWnd, (HMENU)IDB_SONGINFO, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    static_rc.left += (350 + 10);
//    m_songtime = CreateWindow(TEXT("STATIC"), TEXT("00:00 / 00:00"), WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER | SS_OWNERDRAW,
//        static_rc.left, static_rc.top, 140, 16, hWnd, (HMENU)IDB_SONGTIME, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    static_rc.left = Width - 15 - 150;
//    m_timeshow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_OWNERDRAW,
//        static_rc.left, static_rc.top, 150, 16, hWnd, (HMENU)IDB_TIMESHOW, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//
//    static_rc.left = Width - 90;
//    static_rc.top = 15;
//    m_volumebar = CreateWindow(TRACKBAR_CLASS, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER | TBS_HORZ | TBS_TOOLTIPS,
//        static_rc.left, static_rc.top, 80, 20, hWnd, (HMENU)13, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//    SendMessage(m_volumebar, TBM_SETPOS, TRUE, (LPARAM)50);
//
//    list_rc.left = 30;
//    list_rc.top = 80;
//    m_listview = CreateWindowEx(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT, WC_LISTVIEW, TEXT(""),
//        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
//        list_rc.left, list_rc.top, 640, 145, hWnd, (HMENU)IDB_SONGLIST, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
//}



void myD3DSound::myChangeSize(HWND hWnd)
{
    RECT dynamic_rc;
    GetClientRect(hWnd, &dynamic_rc);
    LONG Width = dynamic_rc.right - dynamic_rc.left;
    LONG Height = dynamic_rc.bottom - dynamic_rc.top;

    MoveWindow(m_songinfo, 15, Height - 5 - 16, 350, 16, TRUE);

    MoveWindow(m_songtime, 15 + 350 + 10, Height - 5 - 16, 140, 16, TRUE);

    MoveWindow(m_timeshow, Width - 15 - 150, Height - 5 - 16, 150, 16, TRUE);

    MoveWindow(m_songtxt, 115, 50, Width - 115 - 15, 20, TRUE);

    MoveWindow(m_scrollbar, 315, 15, Width - 315 - 15 - 90, 20, TRUE);

    MoveWindow(m_volumebar, Width - 90, 15, 80, 20, TRUE);

    MoveWindow(m_listview, 30, 80, Width - 60, Height - 80 - 10 - 21, TRUE);

}


void myD3DSound::mySetList(WCHAR* wcFilename)
{
    myListMember tmplist;
    //m_path
    StrCpyW(tmplist.m_path, wcFilename);
    //m_name
    char    cName[100];
    char    tmpstr[MAX_PATH];
    memset(cName, 0, 100);
    memset(tmpstr, 0, MAX_PATH);

    WCHARtoChar(wcFilename, tmpstr);
    Int32 len = (Int32)strlen(tmpstr);
    char* p = &tmpstr[len];
    int len_name = 0;
    bool isname = 0;
    bool bk = 0;
    while (!bk)
    {
        if (!isname && (*p == '.'))
        {
            isname = true;
            p--;
            continue;
        }
        if (isname)
        {
            if (*p == '\\')
            {
                bk = true;
                p++;
                continue;
            }
            len_name++;
        }
        p--;
    }
    memcpy(cName, p, len_name);
    ChartoWCHAR(cName, tmplist.m_name);

    //type
    LPWSTR    lwType;
    char    ctmpType[5];
    char    cType[4];
    lwType = PathFindExtension(wcFilename);
    WCHARtoChar(lwType, ctmpType);
    sprintf_s(cType, "%s", ctmpType + 1);
    ChartoWCHAR(cType, tmplist.m_type);

    if (StrCmpW(lwType, TEXT(".wav")) == 0 || StrCmpW(lwType, TEXT(".WAV")) == 0)
    {
        //m_bits
        FILE* tmpfp = NULL;
        char    cBits[20];
        DWORD    tmpSize;
        DWORD    tmpCycle;
        WAVEFORMATEX* pwfx = NULL;
        memset(cBits, 0, 20);
        if (fopen_s(&tmpfp, tmpstr, "rb") == 0)
        {
            myGetWAVFormat(&tmpSize, &tmpCycle, tmpfp, &pwfx);
            if (pwfx != NULL)
            {
                sprintf_s(cBits, 20, "%d kbps", (pwfx->wBitsPerSample * pwfx->nChannels * pwfx->nSamplesPerSec) / 1000);
            }
            fclose(tmpfp);
        }
        ChartoWCHAR(cBits, tmplist.m_bits);
        //time
        char cTime[10];
        memset(cTime, 0, 10);
        sprintf_s(cTime, "%02d:%02d", tmpCycle / 60, tmpCycle % 60);
        ChartoWCHAR(cTime, tmplist.m_time);
    }
    else if (StrCmpW(lwType, TEXT(".mp3")) == 0 || StrCmpW(lwType, TEXT(".MP3")) == 0)
    {
        char    cBits[20];
        int        bits;
        DWORD    tmpSize;
        DWORD    tmpCycle;
        FILE* tmpfp = NULL;
        WAVEFORMATEX* pwfx = NULL;
        memset(cBits, 0, 20);
        if (myGetMP3Format(tmpstr, &tmpSize, &tmpCycle, &bits, &pwfx) == 0)
        {
            sprintf_s(cBits, 20, "%d kbps", bits);
        }
        ChartoWCHAR(cBits, tmplist.m_bits);
        //time
        char cTime[10];
        memset(cTime, 0, 10);
        sprintf_s(cTime, "%02d:%02d", tmpCycle / 60, tmpCycle % 60);
        ChartoWCHAR(cTime, tmplist.m_time);
    }

    m_list.push_back(tmplist);

    myUpdateList();
}



void myD3DSound::myUpdateList()
{
    ListView_DeleteAllItems(m_listview);

    Int32 iSize = (Int32)m_list.size();
    LVITEM vitem;
    vitem.mask = LVIF_TEXT;
    for (int i = 0; i < iSize; i++)
    {
        char    cIdx[100];
        WCHAR    wIdx[100];
        memset(cIdx, 0, 100);
        memset(wIdx, 0, 100);

        vitem.iItem = i;
        m_list[i].m_idx = i + 1;
        sprintf_s(cIdx, "%d", i + 1);
        ChartoWCHAR(cIdx, wIdx);
        vitem.pszText = wIdx;
        vitem.iSubItem = 0;
        ListView_InsertItem(m_listview, &vitem);

        vitem.iSubItem = 1;
        vitem.pszText = m_list[i].m_name;
        ListView_SetItem(m_listview, &vitem);
        vitem.iSubItem = 2;
        vitem.pszText = m_list[i].m_time;
        ListView_SetItem(m_listview, &vitem);
        vitem.iSubItem = 3;
        vitem.pszText = m_list[i].m_type;
        ListView_SetItem(m_listview, &vitem);
        vitem.iSubItem = 4;
        vitem.pszText = m_list[i].m_bits;
        ListView_SetItem(m_listview, &vitem);
        vitem.iSubItem = 5;
        vitem.pszText = m_list[i].m_path;
        ListView_SetItem(m_listview, &vitem);
    }
}



Int32 myD3DSound::mySongNum()
{
    return (Int32)m_list.size();
}



void myD3DSound::myInitList()
{
    LVCOLUMN vcl;
    vcl.mask = LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT | LVCF_FMT;
    vcl.fmt = LVCFMT_LEFT;
    for (int i = 0; i < 6; i++)
    {
        vcl.pszText = g_cwSongList[i];
        vcl.cx = 80;
        if (i == 5)
            vcl.cx = 800;
        vcl.iSubItem = i;
        ListView_InsertColumn(m_listview, i, &vcl);
    }
}



void myD3DSound::mySetTimer(SYSTEMTIME time)
{
    char strtime[60];
    memset(strtime, 0, 60);
    sprintf_s(strtime, "%04d-%02d-%02d %02d:%02d:%02d",
        time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    ChartoWCHAR(strtime, m_wstrTime);
}



WCHAR* myD3DSound::myGetTimer()
{
    return m_wstrTime;
}



void myD3DSound::mySetScrollPos(bool isUp, DWORD dPos)
{
    if (isUp)
    {
        LRESULT iPos;
        iPos = SendMessage(m_scrollbar, TBM_GETPOS, 0, 0);
        if (g_pmySound->isPlay() && g_pmySound->m_ds_dwFileSize <= DSBSIZE_MAX)
        {
            DWORD dNewPos;
            dNewPos = (DWORD)iPos * (g_pmySound->m_ds_dwFileSize / 100);
            g_pmySound->m_pBuffer8->SetCurrentPosition(dNewPos);
            g_pmySound->m_pBuffer8->Play(0, 0, DSBPLAY_LOOPING);
        }
        else
        {
            iPos = SendMessage(m_scrollbar, TBM_SETPOS, TRUE, (LPARAM)0);
        }

    }
    else
    {

    }
}




void myD3DSound::mySetVolumePos(bool isUp, DWORD dPos)
{
    LONG vol = 0;
    double dbValue;
    LRESULT iPos;
    if (isUp && m_pBuffer8 != NULL)
    {
        iPos = SendMessage(m_volumebar, TBM_GETPOS, 0, 0);
        if (iPos > 0 && iPos <= 100)
        {
            dbValue = 20.0f * log10((double)(iPos / 100.0f));
            vol = (LONG)(dbValue * 100.0f);
        }
        else if (iPos == 0)
        {
            vol = DSBVOLUME_MIN;
        }
        else
        {
            vol = DSBVOLUME_MAX;
        }
        m_pBuffer8->SetVolume(vol);
    }
}




void myD3DSound::myOpenFile()
{
    WCHAR strFilename[MAX_PATH];
    ZeroMemory(&opfn, sizeof(OPENFILENAME));

    opfn.lStructSize = sizeof(OPENFILENAME);                                //结构体大小
    opfn.lpstrFilter = L"所有文件\0*.*\0wav文件\0*.wav\0MP3文件\0*.mp3\0";    //过滤器
    opfn.nFilterIndex = 1;                                                    //过滤索引
    opfn.lpstrFile = strFilename;                                            //文件名的缓冲,不需要初始化则必须为null
    opfn.lpstrFile[0] = '\0';
    opfn.nMaxFile = sizeof(strFilename);
    opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&opfn))
    {
        mySetList(strFilename);
    }
}

void myD3DSound::myCloseFile()
{
    ZeroMemory(&opfn, sizeof(OPENFILENAME));
    if (StrCmpW(m_wSongPath, m_wSongPathPre) == 0)
    {
        SetWindowText(m_songtxt, TEXT(""));
        myStop();
        if (m_fp)
        {
            fclose(m_fp);
            m_fp = NULL;
        }
    }
    MY_SONG_LIST::iterator Songit;
    for (Songit = m_list.begin(); Songit != m_list.end(); Songit++)
    {
        if (StrCmpW(Songit->m_path, m_wSongPathPre) == 0)
        {
            m_list.erase(Songit);
            memset(m_wSongPathPre, 0, MAX_PATH);
            memset(m_wSongPath, 0, MAX_PATH);
            myUpdateList();
            return;
        }
    }

}



void myD3DSound::mySetPlayInfo(NM_LISTVIEW* pNMListView, bool DBClick)
{
    if (DBClick)
    {
        if (StrCmpW(m_list[pNMListView->iItem].m_path, m_wSongPath))
        {//双击新歌则停止当前播放
            myStop();
        }
        StrCpyW(m_wSongPath, m_list[pNMListView->iItem].m_path);
        StrCpyW(m_wSongName, m_list[pNMListView->iItem].m_name);
        StrCpyW(m_wSongPathPre, m_list[pNMListView->iItem].m_path);
        StrCpyW(m_wSongNamePre, m_list[pNMListView->iItem].m_name);
    }
    else
    {//单击
        StrCpyW(m_wSongPathPre, m_list[pNMListView->iItem].m_path);
        StrCpyW(m_wSongNamePre, m_list[pNMListView->iItem].m_name);
        if (!isPlay())
        {
            StrCpyW(m_wSongPath, m_list[pNMListView->iItem].m_path);
            StrCpyW(m_wSongName, m_list[pNMListView->iItem].m_name);
        }
    }
}



int myD3DSound::myPlay()
{
    if (isPlay())
    {
        if (StrCmpW(m_wSongPath, m_wSongPathPre))
        {
            myStop();
            StrCpyW(m_wSongName, m_wSongNamePre);
            StrCpyW(m_wSongPath, m_wSongPathPre);
        }
        else
        {
            return 0;
        }
    }
    if (m_pBuffer8 == NULL)
    {
        int res = 0;
        res = mySetSoundType();
        if (res != 0)
        {
            MessageBox(m_father, g_cwErrorMsg[res], TEXT("ERROR"), MB_OK);
            return 0;
        }
        //create notification thread
        m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadNotifyEvent, NULL, 0, &m_thread_id);
        m_hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadNotifyEvent2, NULL, 0, &m_thread_id2);
        m_hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadNotifyEvent3, NULL, 0, &m_thread_id3);
        SetWindowText(m_songtxt, m_wSongName);
    }
    SetPlaying(TRUE);
    mySetVolumePos(TRUE, 0);
    m_pBuffer8->SetCurrentPosition(m_dwPlayPos);
    m_pBuffer8->Play(0, 0, DSBPLAY_LOOPING);
    return 0;
}



int myD3DSound::myStop()
{
    myCleanBuffer();
    if (isPlay())
    {
        CloseHandle(m_hEvents[0]);

        if (m_hThread != NULL)
        {
            TerminateThread(m_hThread, 0);
            CloseHandle(m_hThread);
        }
    }
    if (IsMPG3())
    {
        cleanup();
    }
    SetWindowText(m_songinfo, TEXT("请选择wav文件进行播放。"));
    StrCpyW(m_wSongName, TEXT(""));
    StrCpyW(m_wSongPath, TEXT(""));
    SetWindowText(m_songtxt, m_wSongName);
    SendMessage(g_pmySound->m_scrollbar, TBM_SETPOS, TRUE, (LPARAM)0);
    SetWindowText(g_pmySound->m_songtime, TEXT("00:00 / 00:00"));
    return 0;
}



int myD3DSound::myPause()
{
    if (m_pBuffer8 == NULL)
        return -1;
    if (isPlay())
    {
        m_pBuffer8->GetCurrentPosition(&m_dwPlayPos, NULL);
        SetPlaying(FALSE);
        m_pBuffer8->Stop();
    }
    else
    {
        SetPlaying(TRUE);
        m_pBuffer8->SetCurrentPosition(m_dwPlayPos);
        m_pBuffer8->Play(0, 0, DSBPLAY_LOOPING);
    }
    return 0;
}



int myD3DSound::GetBufferValue(FILE** fp, mpg123_handle** mpghandle, DWORD* BuffSize)
{
    *fp = m_fp;
    *mpghandle = m_mpghandle;
    *BuffSize = m_ds_dwBuffSize;
    return 0;
}


///////////////////////
//private
///////////////////////

int myD3DSound::mySetSoundType()
{
    WCHAR cNameStr[MAX_PATH];
    LPWSTR lpNameType;
    char cType[4];
    char cTmpStr[5];
    memset(cNameStr, 0, MAX_PATH);
    memset(cType, 0, 4);
    memset(cTmpStr, 0, 5);

    StrCpyW(cNameStr, m_wSongPath);

    if (cNameStr[0] == '\0')
    {
        return EFAIL_NOFILE;
    }
    lpNameType = PathFindExtension(cNameStr);

    WCHARtoChar(lpNameType, cTmpStr);
    sprintf_s(cType, "%s", cTmpStr + 1);
    if (StrCmpW(lpNameType, TEXT(".mp3")) == 0 || StrCmpW(lpNameType, TEXT(".MP3")) == 0)
    {
        DWORD dwSize;        //stream size
        DWORD dwCycle;
        DWORD dwBuffSize;
        int bitrate;
        WAVEFORMATEX wfx;
        WAVEFORMATEX* pTmpWfx = NULL;
        char filepath[MAX_PATH];
        memset(filepath, 0, MAX_PATH);
        WCHARtoChar(cNameStr, filepath);
        if (myGetMP3Format(filepath, &dwSize, &dwCycle, &bitrate, &pTmpWfx, FALSE) != 0)
        {
            return EFAIL_FORMATERR;
        }
        m_ds_dwFileSize = dwSize;

        m_ds_dwFileTime = dwCycle;
        m_ds_dwFilebps = pTmpWfx->nAvgBytesPerSec;
        m_ds_dwPos = 0;        //offset position
        m_ds_dwLeave = dwSize;    //leave data size
        wfx.wFormatTag = pTmpWfx->wFormatTag;
        wfx.nChannels = pTmpWfx->nChannels;
        wfx.nSamplesPerSec = pTmpWfx->nSamplesPerSec;
        wfx.wBitsPerSample = pTmpWfx->wBitsPerSample;
        wfx.nBlockAlign = pTmpWfx->nBlockAlign;
        wfx.nAvgBytesPerSec = pTmpWfx->nAvgBytesPerSec;
        pTmpWfx = NULL;
        if (FAILED(myCreateMPGBuffer(&wfx, &dwBuffSize)))
        {
            return EFAIL_MPGBUFFERERR;
        }
        m_ds_dwBuffSize = dwBuffSize;
        //song info
        WCHAR    wcStr_info[100];
        char    cStr_info[100];
        char    cStr_type[5];
        memset(wcStr_info, 0, 100);
        memset(cStr_info, 0, 100);
        memset(cStr_type, 0, 5);
        WCHARtoChar(lpNameType, cStr_type);
        sprintf_s(cStr_info, "%s | %d kbps | %d Hz", cStr_type + 1, bitrate, wfx.nSamplesPerSec);
        ChartoWCHAR(cStr_info, wcStr_info);
        SetWindowText(m_songinfo, wcStr_info);
        m_bmpg = TRUE;
    }

    else if (StrCmpW(lpNameType, TEXT(".wav")) == 0 || StrCmpW(lpNameType, TEXT(".WAV")) == 0)
    {
        WAVEFORMATEX wfx;
        DWORD dwSize;            //声音文件总大小
        DWORD dwCycle;

        DWORD dwBuffSize;        //创建的缓冲区总大小
        int inum = WideCharToMultiByte(CP_ACP, 0, cNameStr, -1, NULL, 0, NULL, 0);
        char* cfilename = NULL;
        cfilename = (char*)malloc(inum * sizeof(char));
        if (cfilename == NULL)
            free(cfilename);
        memset(cfilename, 0, inum * sizeof(char));
        WideCharToMultiByte(CP_ACP, 0, cNameStr, -1, cfilename, inum, NULL, 0);

        if (fopen_s(&m_fp, cfilename, "rb"))
        {
            return EFAIL_OPENFILEERR;
        }
        WAVEFORMATEX* pTmpWfx = NULL;
        myGetWAVFormat(&dwSize, &dwCycle, m_fp, &pTmpWfx);

        if (pTmpWfx == NULL)
        {
            return EFAIL_FORMATERR;
        }
        m_ds_dwFileSize = dwSize;

        m_ds_dwFileTime = dwCycle;
        m_ds_dwFilebps = pTmpWfx->nAvgBytesPerSec;
        if (m_factwav)
            m_ds_dwPos = sizeof(WAVE_HEADER_FACT);    //offset position
        else
            m_ds_dwPos = sizeof(WAVE_HEADER);

        m_ds_dwLeave = dwSize;                //leave data size
        wfx.wFormatTag = pTmpWfx->wFormatTag;
        wfx.nChannels = pTmpWfx->nChannels;
        wfx.nSamplesPerSec = pTmpWfx->nSamplesPerSec;
        wfx.wBitsPerSample = pTmpWfx->wBitsPerSample;
        wfx.nBlockAlign = pTmpWfx->nBlockAlign;
        wfx.nAvgBytesPerSec = pTmpWfx->nAvgBytesPerSec;
        pTmpWfx = NULL;
        if (FAILED(myCreatePCMBuffer(&wfx, &dwBuffSize)))
        {
            return EFAIL_PCMBUFFERERR;
        }
        m_ds_dwBuffSize = dwBuffSize;    //返回缓冲区大小
                                         //songinfo
        WCHAR    wcStr_info[100];    //output info
        char    cStr_info[100];
        char    cStr_type[5];
        memset(wcStr_info, 0, 100);
        memset(cStr_info, 0, 100);
        memset(cStr_type, 0, 5);
        WCHARtoChar(lpNameType, cStr_type);
        sprintf_s(cStr_info, "%s | %d kbps | %d Hz", cStr_type + 1,
            (wfx.wBitsPerSample * wfx.nChannels * wfx.nSamplesPerSec) / 1000, wfx.nSamplesPerSec);    //类型|比特率|频率
        ChartoWCHAR(cStr_info, wcStr_info);
        SetWindowText(m_songinfo, wcStr_info);
        m_bmpg = FALSE;
    }
    else
    {
        return EFAIL_NOTSUPPORT;
    }


    return 0;

}



int myD3DSound::myGetWAVFormat(DWORD* dwSize, DWORD* dwCycle, FILE* fp, WAVEFORMATEX** wfx)
{
    WAVE_HEADER            wave_header;
    WAVE_HEADER_FACT    wave_header2;
#ifndef _DEBUG
    volatile WAVEFORMATEX    wave_format;
#else
    WAVEFORMATEX    wave_format;
#endif
    char fact[4];
    fseek(fp, 38, SEEK_SET);
    fread(fact, 1, 4, fp);
    fseek(fp, 0, SEEK_SET);
    if (memcmp(fact, "fact", 4) == 0)
    {
        fread(&wave_header2, 1, sizeof(WAVE_HEADER_FACT), fp);
        m_factwav = TRUE;
        if (memcmp(wave_header2.riff_sig, "RIFF", 4) ||
            memcmp(wave_header2.wave_sig, "WAVE", 4) ||
            memcmp(wave_header2.format_sig, "fmt ", 4))
        {
            return -1;
        }
        wave_format.wFormatTag = WAVE_FORMAT_PCM;
        wave_format.nChannels = wave_header2.channels;
        wave_format.nSamplesPerSec = wave_header2.sample_rate;
        wave_format.wBitsPerSample = wave_header2.bits_per_sample;
        wave_format.nBlockAlign = wave_header2.bits_per_sample / 8 * wave_header2.channels;
        wave_format.nAvgBytesPerSec = wave_header2.sample_rate * wave_format.nBlockAlign;
        *dwSize = wave_header2.data_size;
        *dwCycle = wave_header2.data_size / wave_format.nAvgBytesPerSec;
    }
    else
    {
        fread(&wave_header, 1, sizeof(WAVE_HEADER), fp);
        m_factwav = FALSE;
        if (memcmp(wave_header.riff_sig, "RIFF", 4) ||
            memcmp(wave_header.wave_sig, "WAVE", 4) ||
            memcmp(wave_header.format_sig, "fmt ", 4))
        {
            return -1;
        }
        wave_format.wFormatTag = WAVE_FORMAT_PCM;
        wave_format.nChannels = wave_header.channels;
        wave_format.nSamplesPerSec = wave_header.sample_rate;
        wave_format.wBitsPerSample = wave_header.bits_per_sample;
        wave_format.nBlockAlign = wave_header.bits_per_sample / 8 * wave_header.channels;
        wave_format.nAvgBytesPerSec = wave_header.sample_rate * wave_format.nBlockAlign;
        *dwSize = wave_header.data_size;
        *dwCycle = wave_header.data_size / wave_format.nAvgBytesPerSec;
    }

    *wfx = (WAVEFORMATEX*)&wave_format;
    return 0;
}

int myD3DSound::myGetMP3Format(char* filestr, DWORD* dwSize, DWORD* dwCycle, int* bitrate, WAVEFORMATEX** wfx, bool isClose)
{
    int        ret = MPG123_OK;
    int        channels = 0;    //声道
    int        encoding = 0;    //编码格式
    long    rate = 0;        //频率
    int        perbits = 16;    //bits per second
    long    fTime = 0;
    long    fSize = 0;
    int        simpleNum = 1152;
    long    frameNum;
    //long    streamSize;
    //long    streamSize1;
    //long    streamSize2;
    //long    streamSize3;
    FILE*    tmpfp = NULL;
    volatile WAVEFORMATEX    wave_format;
    mpg123_frameinfo mpginfo;

    cleanup();

    ret = mpg123_init();
    if (ret != MPG123_OK || (m_mpghandle = mpg123_new(NULL, &ret)) == NULL)
    {
        cleanup();
        return -1;
    }
    if (mpg123_open(m_mpghandle, filestr) != MPG123_OK || mpg123_getformat(m_mpghandle, &rate, &channels, &encoding) != MPG123_OK)
    {
        cleanup();
        return -1;
    }

    if ((encoding & MPG123_ENC_16) == MPG123_ENC_16)
        perbits = 16;
    else if ((encoding & MPG123_ENC_32) == MPG123_ENC_32)
        perbits = 32;
    else
        perbits = 8;

    //wfx
    wave_format.wFormatTag = WAVE_FORMAT_PCM;
    wave_format.nChannels = channels;
    wave_format.nSamplesPerSec = rate;
    wave_format.wBitsPerSample = perbits;
    wave_format.nBlockAlign = perbits / 8 * channels;
    wave_format.nAvgBytesPerSec = rate * perbits / 8 * channels;
    *wfx = (WAVEFORMATEX*)&wave_format;

    mpg123_seek(m_mpghandle, 0, SEEK_END);
    frameNum = mpg123_tellframe(m_mpghandle);
    //总帧数法：Total_Time = Total_Frame_Number * （Sample_Number * （1 / Frame_Sample_Rate））
    fTime = (long)(frameNum * simpleNum / rate);

    //time and buffer size
    *dwCycle = fTime;
    //data size = ftime * nAvgBytesPerSec = (frameNum*simpleNum/rate)*(rate*perbits/8*channels)
    *dwSize = frameNum * simpleNum * perbits * channels / 8;

    if (mpg123_info(m_mpghandle, &mpginfo) != MPG123_OK)
    {
        cleanup();
        return -1;
    }
    if (mpginfo.layer != 3)
    {
        cleanup();
        return -1;
    }

    //bit rate
    if (mpginfo.vbr == MPG123_CBR)
    {
        *bitrate = mpginfo.bitrate;
    }
    else if (mpginfo.vbr == MPG123_VBR)
    {
        if (fopen_s(&tmpfp, filestr, "rb") == 0)
        {
            fseek(tmpfp, 0, SEEK_END);
            fSize = ftell(tmpfp);        //文件大小
            fclose(tmpfp);
            tmpfp = NULL;
            *bitrate = (fSize * 8) / (fTime * 1000);    //(kbits/s) : filesize(bytes)*8(bits)/filetime(s)/1000
                                                        //平均比特率 = 文件大小/总时间/1000
        }
    }
    if (isClose)
    {
        cleanup();
    }
    return 0;
}




HRESULT myD3DSound::myCreatePCMBuffer(WAVEFORMATEX* wfx, DWORD* dwBuffSize)
{
    DSBUFFERDESC dsbd;
    WAVEFORMATEX wave;
    IDirectSound8* lpDS8;
    LPDIRECTSOUNDBUFFER        pTmpBuffer;

    LPDIRECTSOUNDNOTIFY8    pNotify;

    DSBPOSITIONNOTIFY        dspNotify;

    DSCAPS caps;

    if (m_pDirectSound == NULL)
        return E_FAIL;

    lpDS8 = m_pDirectSound;
    ZeroMemory(&dsbd, sizeof(dsbd));

    wave.wFormatTag = WAVE_FORMAT_PCM;
    if (wfx)
    {
        wave.nChannels = wfx->nChannels;            //音频文件的通道数量
        wave.nSamplesPerSec = wfx->nSamplesPerSec;    //采样频率
        wave.wBitsPerSample = wfx->wBitsPerSample;    //每次采样样本的大小
    }
    else
    {
        wave.nChannels = 2;
        wave.nSamplesPerSec = 44100;
        wave.wBitsPerSample = 16;
    }
    wave.nBlockAlign = wave.nChannels * wave.wBitsPerSample / 8;
    wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;
    wave.cbSize = 0;

    dsbd.dwSize = sizeof(dsbd);
    dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;

    if (m_ds_dwFileSize > DSBSIZE_MAX)
        dsbd.dwBufferBytes = DSBSIZE_MAX;
    else
        dsbd.dwBufferBytes = m_ds_dwFileSize;


    *dwBuffSize = dsbd.dwBufferBytes;        //返回缓冲区大小
    dsbd.lpwfxFormat = &wave;

    caps.dwSize = sizeof(DSCAPS);
    if (SUCCEEDED(lpDS8->GetCaps(&caps)))
    {
        if (caps.dwMaxHwMixingStreamingBuffers > 0)
            dsbd.dwFlags |= DSBCAPS_LOCDEFER;
        else
            dsbd.dwFlags |= DSBCAPS_STATIC;
    }

    if (FAILED(lpDS8->CreateSoundBuffer(&dsbd, &pTmpBuffer, NULL)))
        return E_FAIL;

    if (FAILED(pTmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pBuffer8)))
        return E_FAIL;
    pTmpBuffer->Release();



    if (FAILED(m_pBuffer8->QueryInterface(IID_IDirectSoundNotify, (void**)&pNotify)))
        return E_FAIL;

    dspNotify.dwOffset = dsbd.dwBufferBytes - 1;
    m_hEvents[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    dspNotify.hEventNotify = m_hEvents[0];
    pNotify->SetNotificationPositions(1, &dspNotify);
    pNotify->Release();

    fseek(m_fp, m_ds_dwPos, SEEK_SET);
    if (myReadBuffer(0, dsbd.dwBufferBytes))
    {
        m_ds_dwPos += dsbd.dwBufferBytes;
        //if(m_ds_dwFileSize <= DSBSIZE_MAX), this m_ds_dwLeave will be 0
        m_ds_dwLeave -= dsbd.dwBufferBytes;

        return S_OK;
    }
    else
        return E_FAIL;

}




HRESULT myD3DSound::myCreateMPGBuffer(WAVEFORMATEX* wfx, DWORD* dwBuffSize)
{
    DSBUFFERDESC dsbd;
    WAVEFORMATEX wave;
    IDirectSound8* lpDS8;
    LPDIRECTSOUNDBUFFER        pTmpBuffer;

    LPDIRECTSOUNDNOTIFY8    pNotify;

    DSBPOSITIONNOTIFY        dspNotify;


    DSCAPS caps;

    if (m_pDirectSound == NULL)
        return E_FAIL;

    lpDS8 = m_pDirectSound;
    ZeroMemory(&dsbd, sizeof(dsbd));

    wave.wFormatTag = WAVE_FORMAT_PCM;
    if (wfx)
    {
        wave.nChannels = wfx->nChannels;
        wave.nSamplesPerSec = wfx->nSamplesPerSec;
        wave.wBitsPerSample = wfx->wBitsPerSample;
    }
    else
    {
        wave.nChannels = 2;
        wave.nSamplesPerSec = 44100;
        wave.wBitsPerSample = 16;
    }
    wave.nBlockAlign = wave.nChannels * wave.wBitsPerSample / 8;
    wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;
    wave.cbSize = 0;

    dsbd.dwSize = sizeof(dsbd);
    dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;

    if (m_ds_dwFileSize > DSBSIZE_MAX)
        dsbd.dwBufferBytes = DSBSIZE_MAX;
    else
        dsbd.dwBufferBytes = m_ds_dwFileSize;

    *dwBuffSize = dsbd.dwBufferBytes;        //返回缓冲区大小
    dsbd.lpwfxFormat = &wave;

    caps.dwSize = sizeof(DSCAPS);
    if (SUCCEEDED(lpDS8->GetCaps(&caps)))
    {
        if (caps.dwMaxHwMixingStreamingBuffers > 0)
            dsbd.dwFlags |= DSBCAPS_LOCDEFER;
        else
            dsbd.dwFlags |= DSBCAPS_STATIC;
    }

    if (FAILED(lpDS8->CreateSoundBuffer(&dsbd, &pTmpBuffer, NULL)))
        return E_FAIL;

    if (FAILED(pTmpBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pBuffer8)))
        return E_FAIL;
    pTmpBuffer->Release();


    if (FAILED(m_pBuffer8->QueryInterface(IID_IDirectSoundNotify, (void**)&pNotify)))
        return E_FAIL;

    dspNotify.dwOffset = dsbd.dwBufferBytes - 1;
    m_hEvents[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    dspNotify.hEventNotify = m_hEvents[0];
    pNotify->SetNotificationPositions(1, &dspNotify);
    pNotify->Release();

    if (m_mpghandle == NULL)
    {
        return E_FAIL;
    }
    mpg123_seek(m_mpghandle, 0, SEEK_SET);

    if (myReadMPGBuffer(0, dsbd.dwBufferBytes))
    {
        m_ds_dwPos += dsbd.dwBufferBytes;
        //if(m_ds_dwFileSize <= DSBSIZE_MAX), this m_ds_dwLeave will be 0
        m_ds_dwLeave -= dsbd.dwBufferBytes;

        return S_OK;
    }
    else
        return E_FAIL;
}




bool myD3DSound::myReadBuffer(long lock_pos, long lock_size)
{
    if (m_pBuffer8 == NULL || m_fp == NULL)
        return 0;

    LPVOID buf = NULL;
    DWORD  buf_len = 0;
    if (SUCCEEDED(m_pBuffer8->Lock(lock_pos, lock_size, &buf, &buf_len, NULL, NULL, 0)))
    {
        fread(buf, 1, buf_len, m_fp);
        m_pBuffer8->Unlock(buf, buf_len, NULL, 0);
    }
    return 1;
}



bool myD3DSound::myReadMPGBuffer(long lock_pos, long lock_size)
{
    if (m_pBuffer8 == NULL || m_mpghandle == NULL)
        return 0;

    LPVOID buf = NULL;
    DWORD  buf_len = 0;
    unsigned char* _buffer;
    size_t outsize;
    _buffer = (unsigned char*)malloc(lock_size * sizeof(unsigned char));
    if (SUCCEEDED(m_pBuffer8->Lock(lock_pos, lock_size, &buf, &buf_len, NULL, NULL, 0)))
    {
        mpg123_read(m_mpghandle, _buffer, lock_size, &outsize);
        memcpy(buf, _buffer, outsize);
        m_pBuffer8->Unlock(buf, buf_len, NULL, 0);
    }
    return 1;
}




void myD3DSound::myCleanBuffer()
{
    if (m_pBuffer8)
    {
        m_pBuffer8->Stop();
    }
    SetPlaying(FALSE);
    m_ds_dwPos = 0;
    m_ds_dwLeave = 0;
    m_ds_dwBuffSize = 0;
    m_dwPlayPos = 0;
    SAFE_RELEASE(m_pBuffer8);
#ifdef __MAX_BUFFER__
    m_ds_dwFileSize = 0;
#endif
    m_ds_dwFileTime = 0;
    m_ds_dwFilebps = 0;
    m_iScrollPos = 0;

}

void myD3DSound::cleanup()
{
    if (m_mpghandle != NULL)
    {
        mpg123_close(m_mpghandle);
        mpg123_delete(m_mpghandle);
        m_mpghandle = NULL;
        mpg123_exit();
    }
}

///////////////////////
//global function
///////////////////////


DWORD ThreadNotifyEvent(LPVOID thread_data)
{
    DWORD res_msg = 0;
    DWORD dwBuffsize = 0;
    FILE* fp = NULL;
    mpg123_handle* mpghandle = NULL;
    if (g_pmySound == NULL)
        return 0;

    g_pmySound->GetBufferValue(&fp, &mpghandle, &dwBuffsize);
    if ((!g_pmySound->IsMPG3() && fp == NULL) || dwBuffsize == 0 || (g_pmySound->IsMPG3() && mpghandle == 0))
        return 0;
    while (1)
    {
        res_msg = WaitForSingleObject(g_pmySound->m_hEvents[0], INFINITE);

        if (res_msg == WAIT_OBJECT_0)
        {
            //update buffer
            if (g_pmySound->m_ds_dwLeave == 0)
            {
                g_pmySound->myStop();
                ExitThread(0);
            }
            if (g_pmySound->IsMPG3())
            {
                mpg123_seek(mpghandle, g_pmySound->m_ds_dwPos, SEEK_SET);
                if (g_pmySound->m_ds_dwLeave <= dwBuffsize)
                {
                    g_pmySound->myReadMPGBuffer(0, g_pmySound->m_ds_dwLeave);
                    g_pmySound->m_ds_dwLeave = 0;
                }
                else
                {
                    g_pmySound->myReadMPGBuffer(0, dwBuffsize);
                    g_pmySound->m_ds_dwPos += dwBuffsize;
                    g_pmySound->m_ds_dwLeave -= dwBuffsize;
                }
            }
            else
            {
                fseek(fp, g_pmySound->m_ds_dwPos, SEEK_SET);
                if (g_pmySound->m_ds_dwLeave <= dwBuffsize)
                {
                    g_pmySound->myReadBuffer(0, g_pmySound->m_ds_dwLeave);
                    g_pmySound->m_ds_dwLeave = 0;
                }
                else
                {
                    g_pmySound->myReadBuffer(0, dwBuffsize);
                    g_pmySound->m_ds_dwPos += dwBuffsize;
                    g_pmySound->m_ds_dwLeave -= dwBuffsize;
                }
            }
        }
    }
    return 0;
}


DWORD ThreadNotifyEvent2(LPVOID thread_data)
{
    DWORD d_FileSize = g_pmySound->m_ds_dwFileSize;
    if (d_FileSize <= DSBSIZE_MAX)
    {
        DWORD    d_PosFile;
        int        icut = 1;
        while (1)
        {
            //update slider
            if (g_pmySound->m_pBuffer8 == NULL)
            {
                SendMessage(g_pmySound->m_scrollbar, TBM_SETPOS, TRUE, (LPARAM)0);
                ExitThread(0);
            }
            if (FAILED(g_pmySound->m_pBuffer8->GetCurrentPosition(&d_PosFile, NULL)))
            {
                SendMessage(g_pmySound->m_scrollbar, TBM_SETPOS, TRUE, (LPARAM)0);
                ExitThread(0);
            }
            if (d_PosFile >= (d_FileSize / 100)* icut)
            {
                SendMessage(g_pmySound->m_scrollbar, TBM_SETPOS, TRUE, (LPARAM)icut);
                icut++;
            }
            if (icut >= 100)
            {
                ExitThread(0);
            }
        }
    }
    return 0;
}

DWORD ThreadNotifyEvent3(LPVOID thread_data)
{
    DWORD    d_FileTime = g_pmySound->m_ds_dwFileTime;
    DWORD    d_Filebps = g_pmySound->m_ds_dwFilebps;        //每秒传输字节
    char    ctmpTime[20];
    WCHAR    wtmpTime[20];
    RECT rect;
    memset(ctmpTime, 0, 20);
    memset(wtmpTime, 0, 20);
    DWORD    d_Nowtime = 0;
    sprintf_s(ctmpTime, "%02d:%02d / %02d:%02d", d_Nowtime / 60, d_Nowtime % 60, d_FileTime / 60, d_FileTime % 60);
    ChartoWCHAR(ctmpTime, wtmpTime);
    SetWindowText(g_pmySound->m_songtime, wtmpTime);
    while (1)
    {
        DWORD    d_PosFile;
        SYSTEMTIME time;
        memset(ctmpTime, 0, 20);
        memset(wtmpTime, 0, 20);
        if (g_pmySound->m_pBuffer8 == NULL)
        {
            SetWindowText(g_pmySound->m_songtime, TEXT("00:00 / 00:00"));
            ExitThread(0);
        }
        if (FAILED(g_pmySound->m_pBuffer8->GetCurrentPosition(&d_PosFile, NULL)))
        {
            SetWindowText(g_pmySound->m_songtime, TEXT("00:00 / 00:00"));
            ExitThread(0);
        }
        if (d_PosFile >= d_Filebps *(d_Nowtime + 1))
        {
            d_Nowtime++;
            sprintf_s(ctmpTime, "%02d:%02d / %02d:%02d", d_Nowtime / 60, d_Nowtime % 60, d_FileTime / 60, d_FileTime % 60);
            ChartoWCHAR(ctmpTime, wtmpTime);
            SetWindowText(g_pmySound->m_songtime, wtmpTime);
            GetLocalTime(&time);
            g_pmySound->mySetTimer(time);
            SetWindowText(g_pmySound->m_timeshow, g_pmySound->myGetTimer());
            GetClientRect(g_pmySound->m_father, &rect);
            InvalidateRect(g_pmySound->m_father, &rect, TRUE);
        }
        if (d_Nowtime == d_FileTime)
        {
            ExitThread(0);
        }
    }
}

void ChartoWCHAR(const char* dsc, WCHAR* dst)
{
    int len_c;
    len_c = MultiByteToWideChar(CP_ACP, 0, dsc, -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, dsc, -1, dst, len_c);
}
void WCHARtoChar(const WCHAR* dsc, char* dst)
{
    int len_w;
    len_w = WideCharToMultiByte(CP_ACP, 0, dsc, -1, NULL, 0, NULL, 0);
    WideCharToMultiByte(CP_ACP, 0, dsc, -1, dst, len_w, NULL, 0);

}

