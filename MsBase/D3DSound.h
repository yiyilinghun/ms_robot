#pragma once

//#include "D3DPlayer.h"
//#include "stdafx.h"
#include "dsound.h"
#include "mpg123.h"

#define MAX_AUDIO_BUF       4
#define BUFFERNOTIFYSIZE    192000
#define IDB_OPEN            1001
#define IDB_CLOSE           1002
#define IDB_PLAY            1003
#define IDB_PAUSE           1004
#define IDB_STOP            1005
#define IDB_PLAYING         2001
#define IDB_TIMESHOW        2002
#define IDB_SONGINFO        2003
#define IDB_SONGTEXT        2004
#define IDB_SONGTIME        2005
#define IDB_SONGLIST        2006

struct WAVE_HEADER                    //44bytes
{
    char    riff_sig[4];
    long    waveform_chunk_size;
    char    wave_sig[4];
    char    format_sig[4];
    long    format_chunk_size;
    short   format_tag;
    short   channels;
    long    sample_rate;
    long    bytes_per_sec;
    short   block_align;
    short   bits_per_sample;
    char    data_sig[4];
    long    data_size;
};

struct WAVE_HEADER_FACT                //58bytes
{
    char    riff_sig[4];
    long    waveform_chunk_size;
    char    wave_sig[4];
    char    format_sig[4];
    long    format_chunk_size;
    short   format_tag;
    short   channels;
    long    sample_rate;
    long    bytes_per_sec;
    short   block_align;
    short   bits_per_sample;
    short    bits_per_sample2;
    char    fact_sig[4];
    short    fact_size;
    short    fact_size2;
    char    fact_data[4];
    char    data_sig[4];
    long    data_size;
};

struct myListMember
{
    int        m_idx;
    WCHAR    m_name[100];
    WCHAR    m_time[10];
    WCHAR    m_type[10];
    WCHAR    m_bits[20];
    WCHAR    m_path[MAX_PATH];
};

static DWORD ThreadNotifyEvent(LPVOID thread_data);
static DWORD ThreadNotifyEvent2(LPVOID thread_data);
static DWORD ThreadNotifyEvent3(LPVOID thread_data);
void ChartoWCHAR(const char*, WCHAR*);
void WCHARtoChar(const WCHAR*, char*);

class myD3DSound
{
private:
    OPENFILENAME opfn;
    bool    isPlaying;
    DWORD    m_ds_dwBuffSize;    //dxsound缓冲区大小
    HANDLE    m_hThread;            //控制buffer
    DWORD    m_thread_id;
    HANDLE    m_hThread2;            //滚动条显示
    DWORD    m_thread_id2;
    HANDLE    m_hThread3;            //剩余时间显示
    DWORD    m_thread_id3;
    FILE* m_fp;
    mpg123_handle* m_mpghandle;
    DWORD    m_dwPlayPos;
    WCHAR m_wstrTime[60];
    WCHAR m_wSongTime[30];        //总时长
    WCHAR m_wSongLeave[30];        //剩余时长
    typedef std::vector<myListMember> MY_SONG_LIST;
    MY_SONG_LIST m_list;
    WCHAR m_wSongPath[MAX_PATH];        //正在播放歌曲
    WCHAR m_wSongName[MAX_PATH];
    WCHAR m_wSongPathPre[MAX_PATH];        //单击选中歌曲
    WCHAR m_wSongNamePre[MAX_PATH];

    bool    m_bmpg;
    bool    m_factwav;
public:
    HWND m_father;
    //button
    HWND m_openbtn;
    HWND m_closebtn;
    HWND m_playbtn;
    HWND m_pausebtn;
    HWND m_stopbtn;
    //static
    HWND m_txtplaying;
    HWND m_songtxt;
    HWND m_songinfo;
    HWND m_timeshow;
    HWND m_songtime;
    //
    HWND m_scrollbar;
    HWND m_volumebar;
    HWND m_listview;

    IDirectSound8* m_pDirectSound;

    //playing file info
    LPDIRECTSOUNDBUFFER8    m_pBuffer8;
    HANDLE  m_hEvents[MAX_AUDIO_BUF];
    DWORD    m_ds_dwFileSize;    //文件大小
    DWORD    m_ds_dwFileTime;    //文件时长(s)
    DWORD    m_ds_dwFilebps;        //文件传输率
    DWORD    m_ds_dwPos;            //文件偏移量
    DWORD    m_ds_dwLeave;        //文件剩余量
    int        m_iScrollPos;        //进度条位置
    int        m_iVolumePos;        //音量条位置

    enum eFAIL_CODE {
        EFAIL_NOFILE = 1,
        EFAIL_NOTSUPPORT,
        EFAIL_PCMBUFFERERR,
        EFAIL_MPGBUFFERERR,
        EFAIL_OPENFILEERR,
        EFAIL_FORMATERR,
    };

private:
    int mySetSoundType();
    int myGetWAVFormat(DWORD* dwSize, DWORD* dwCycle, FILE* fp, WAVEFORMATEX** wfx);
    int myGetMP3Format(char* filestr, DWORD* dwSize, DWORD* dwCycle, int* bitrate, WAVEFORMATEX** wfx, bool isClose = TRUE);
    HRESULT myCreatePCMBuffer(WAVEFORMATEX* wfx, DWORD* dwBuffSize);
    HRESULT myCreateMPGBuffer(WAVEFORMATEX* wfx, DWORD* dwBuffSize);
    void myUpdateList();
    void myCleanBuffer();
    void cleanup();        //clear mpg123 handle
public:
    myD3DSound();
    ~myD3DSound();
    HRESULT myInit(HWND);
    void myCreateWin(HWND);
    void myChangeSize(HWND);
    void mySetList(WCHAR*);
    void myInitList();
    void mySetPlayInfo(NM_LISTVIEW* pNMListView, bool DBClick);
    void mySetTimer(SYSTEMTIME);
    bool myReadBuffer(long lock_pos, long lock_size);
    bool myReadMPGBuffer(long lock_pos, long lock_size);
    void mySetScrollPos(bool isUp, DWORD dPos);
    void mySetVolumePos(bool isUp, DWORD dPos);

    //button
    void myOpenFile();
    void myCloseFile();
    int myPlay();
    int myStop();
    int myPause();

    int mySongNum();
    WCHAR* myGetTimer();
    bool isPlay() { return isPlaying; }
    void SetPlaying(bool flags) { isPlaying = flags; }
    int GetBufferValue(FILE** fp, mpg123_handle** mpghandle, DWORD* BuffSize);
    bool IsMPG3() { return m_bmpg; }



    static int TestFun__1(LPDIRECTSOUNDBUFFER8 m_pBuffer8);
    //int TestFun__2();
    //void TestFun__3(int encording, unsigned short channels, unsigned int rate);
    //void TestFun__4(long fTime, unsigned int rate);
    //int TestFun__5(long fTime);
    //void TestFun__6(long lock_pos, long lock_size);
};

