#include "Precompiled.h"
//#include "dsound.h"

Boolean RenderManager::InitSound()
{
    //    LPDIRECTSOUND8 lpDirectSound;
    //    if (FAILED(::DirectSoundCreate8(nullptr, &lpDirectSound, nullptr)))
    //    {
    //        AssertLog("Can't DirectSoundCreate8");
    //        return False;
    //    }
    //
    //    if (FAILED(lpDirectSound->SetCooperativeLevel(m_StateHWND.m_hWindow, DSSCL_PRIORITY)))
    //    {
    //        AssertLog("SetCooperativeLevel failed");
    //        return False;
    //    }
    //
    //    CWaveFile* g_pWaveFile = NULL;
    //    g_pWaveFile = new CWaveFile;
    //    //g_pWaveFile->Open(_T("D:\\新大话西游2\\music_wdf\\0002-0760A6FF.mp3"), NULL, WAVEFILE_READ);
    //    g_pWaveFile->Open(_T("D:\\我的音乐\\WAV\\BEST'97\\01 That Think You Do (Party Dance Mix).wav"), NULL, WAVEFILE_READ);
    //
    //#define MAX_AUDIO_BUF 4
    //#define BUFFERNOTIFYSIZE 1920000
    //    DSBUFFERDESC dsbd;
    //    ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
    //    dsbd.dwSize = sizeof(DSBUFFERDESC);
    //    dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
    //    dsbd.dwBufferBytes = MAX_AUDIO_BUF * BUFFERNOTIFYSIZE;
    //    dsbd.lpwfxFormat = g_pWaveFile->m_pwfx;
    //    LPDIRECTSOUNDBUFFER lpbuffer;
    //    if (FAILED(lpDirectSound->CreateSoundBuffer(&dsbd, &lpbuffer, NULL)))
    //    {
    //        AssertLog("SetCooperativeLevel failed");
    //        return False;
    //    }
    //
    //    LPDIRECTSOUNDBUFFER8 lpDSBuffer8 = nullptr;
    //    if (FAILED(lpbuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&lpDSBuffer8)))
    //    {
    //        AssertLog("QueryInterface failed");
    //        lpbuffer->Release();
    //        return False;
    //    }
    //
    //    LPVOID lplockbuf;
    //    DWORD len;
    //    DWORD dwWrite;
    //
    //    lpDSBuffer8->Lock(0, 0, &lplockbuf, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
    //    g_pWaveFile->Read((BYTE*)lplockbuf, len, &dwWrite);
    //    lpDSBuffer8->Unlock(lplockbuf, len, NULL, 0);
    //    lpDSBuffer8->SetCurrentPosition(0);
    //    lpDSBuffer8->Play(0, 0, DSBPLAY_LOOPING);
    return True;
}
