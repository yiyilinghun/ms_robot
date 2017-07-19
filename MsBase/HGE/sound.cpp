/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: audio routines
*/


#include "bass.h"
#include "hge.h"
//#include "hge_bass.h"

//#define BASSDEF(f) (WINAPI *f)	// define the functions as pointers

#define LOADBASSFUNCTION(f) *((void **) &f) = (void *) GetProcAddress(hBass, #f)

#define MAKEMUSICPOS(order,row) (0x80000000|MAKELONG(order,row))

#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

#pragma warning(disable:4996)

int g_nRef = 0;

//HGE * CALL Plugin_Initialize(HGE *lpHGE)
//{
//    return (HGE *)HGE::_Interface_Get(lpHGE);
//}

//HGE_BASS * HGE::_Interface_Get(HGE *lpHGE)
//{
//    if (!g_lpHGE) g_lpHGE = new HGE_BASS(lpHGE);
//    g_nRef++;
//
//    return g_lpHGE;
//}

//void CALL HGE::Release()
//{
//    g_nRef--;
//
//    if (!g_nRef)
//    {
//        _SoundDone();
//
//        delete g_lpHGE;
//        g_lpHGE = 0;
//    }
//}


//bool CALL HGE::System_Initiate()
//{
//    if (hBass) return true;
//
//    return _SoundInit();
//}


HEFFECT CALL HGE::Effect_FromMemory(void* data, DWORD size)
{
    //DWORD length, samples;
    HSAMPLE hs;
    //HSTREAM hstrm;
    //BASS_CHANNELINFO info;
    //void *buffer;

    hs = BASS_SampleLoad(TRUE, data, 0, size, 4, BASS_SAMPLE_OVER_VOL);
    //if (!hs)
    //{
    //    hstrm = BASS_StreamCreateFile(TRUE, data, 0, size, BASS_STREAM_DECODE);
    //    if (hstrm)
    //    {
    //        length = (DWORD)BASS_ChannelGetLength(hstrm, BASS_POS_BYTE);
    //        BASS_ChannelGetInfo(hstrm, &info);
    //        samples = length;
    //        if (info.chans < 2) samples >>= 1;
    //        if ((info.flags & BASS_SAMPLE_8BITS) == 0) samples >>= 1;
    //        buffer = (void*)(ULONG_PTR)BASS_SampleCreate(samples, info.freq, 2, 4, info.flags | BASS_SAMPLE_OVER_VOL);
    //        if (!buffer)
    //        {
    //            BASS_StreamFree(hstrm);
    //            _PostError("Can't create sound effect: Not enough memory");
    //        }
    //        else
    //        {
    //            BASS_ChannelGetData(hstrm, buffer, length);
    //            // Error
    //            //hs = BASS_SampleCreateDone();
    //            BASS_StreamFree(hstrm);
    //            if (!hs)    _PostError("Can't create sound effect");
    //        }
    //    }
    //}

    return hs;
}

HEFFECT CALL HGE::Effect_FromFile(const char *filename, DWORD size)
{
    DWORD _size, length, samples;
    HSAMPLE hs;
    HSTREAM hstrm;
    BASS_CHANNELINFO info;
    void *buffer, *data;

    //if (hBass)
    {
        if (bSilent) return (HEFFECT)1;

        if (size) { data = (void *)filename; _size = size; }
        else
        {
            data = Resource_Load(filename, &_size);
            if (!data) return NULL;
        }

        hs = BASS_SampleLoad(TRUE, data, 0, _size, 4, BASS_SAMPLE_OVER_VOL);
        if (!hs)
        {
            hstrm = BASS_StreamCreateFile(TRUE, data, 0, _size, BASS_STREAM_DECODE);
            if (hstrm)
            {
                length = (DWORD)BASS_ChannelGetLength(hstrm, BASS_POS_BYTE);
                BASS_ChannelGetInfo(hstrm, &info);
                samples = length;
                if (info.chans < 2) samples >>= 1;
                if ((info.flags & BASS_SAMPLE_8BITS) == 0) samples >>= 1;
                buffer = (void*)(ULONG_PTR)BASS_SampleCreate(samples, info.freq, 2, 4, info.flags | BASS_SAMPLE_OVER_VOL);
                if (!buffer)
                {
                    BASS_StreamFree(hstrm);
                    _PostError("Can't create sound effect: Not enough memory");
                }
                else
                {
                    BASS_ChannelGetData(hstrm, buffer, length);
                    // Error
                    //hs = BASS_SampleCreateDone();
                    BASS_StreamFree(hstrm);
                    if (!hs)    _PostError("Can't create sound effect");
                }
            }
        }

        if (!size) Resource_Free(data);
        return hs;
    }
    //else return 0;
}

HCHANNEL CALL HGE::Effect_Play(HEFFECT eff)
{
    //if (hBass)
    {
        HCHANNEL chn;
        chn = BASS_SampleGetChannel(eff, FALSE);
        BASS_ChannelPlay(chn, TRUE);
        return chn;
    }
    //else return 0;
}

HCHANNEL CALL HGE::Effect_PlayEx(HEFFECT eff, int volume, int pan, float pitch, bool loop)
{
    //if (hBass)
    {
        BASS_SAMPLE info;
        HCHANNEL chn;
        BASS_SampleGetInfo(eff, &info);

        chn = BASS_SampleGetChannel(eff, FALSE);
        // Error
        //BASS_ChannelSetAttributes(chn, (int)(pitch*info.freq), volume, pan);

        info.flags &= ~BASS_SAMPLE_LOOP;
        if (loop) info.flags |= BASS_SAMPLE_LOOP;

        // Error
        //BASS_ChannelSetFlags(chn, info.flags);
        BASS_ChannelPlay(chn, TRUE);
        return chn;
    }
    //else return 0;
}


void CALL HGE::Effect_Free(HEFFECT eff)
{
    //if (hBass) 
    BASS_SampleFree(eff);
}


HMUSIC CALL HGE::Music_Load(const char *filename, DWORD size)
{
    void *data;
    DWORD _size;
    HMUSIC hm;

    //if (hBass)
    {
        if (size)
        {
            data = (void *)filename;
            _size = size;
        }
        else
        {
            data = Resource_Load(filename, &_size);
            if (!data) return 0;
        }

        hm = BASS_MusicLoad(TRUE, data, 0, 0, BASS_MUSIC_PRESCAN | BASS_MUSIC_POSRESETEX | BASS_MUSIC_RAMP, 0);
        if (!hm)	_PostError("Can't load music");
        if (!size) Resource_Free(data);
        return hm;
    }
    //else return 0;
}

HCHANNEL CALL HGE::Music_Play(HMUSIC mus, bool loop, int volume, int order, int row)
{
    //if (hBass)
    {

        // Error
        //DWORD pos = BASS_MusicGetOrderPosition(mus);
        //if (order == -1) order = LOWORD(pos);
        //if (row == -1) row = HIWORD(pos);
        BASS_ChannelSetPosition(mus, MAKEMUSICPOS(order, row), BASS_POS_BYTE);

        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(mus, &info);
        // Error
        //BASS_ChannelSetAttributes(mus, info.freq, volume, 0);

        info.flags &= ~BASS_SAMPLE_LOOP;
        if (loop) info.flags |= BASS_SAMPLE_LOOP;
        // Error
        BASS_ChannelFlags(mus, info.flags, 0xFFFFFFFF);

        BASS_ChannelPlay(mus, FALSE);

        return mus;
    }
    //else return 0;
}

bool CALL HGE::Music_Pause(HMUSIC mus)
{
    return BASS_ChannelPause(mus) ? true : false;
}

void CALL HGE::Music_Free(HMUSIC mus)
{
    //if (hBass) 
    BASS_MusicFree(mus);
}

void CALL HGE::Music_SetAmplification(HMUSIC music, int ampl)
{
    //if (hBass) BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY, ampl);
}

int CALL HGE::Music_GetAmplification(HMUSIC music)
{
    //if (hBass) return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY);
    //else return -1;
    return -1;
}

int CALL HGE::Music_GetLength(HMUSIC music)
{
    //if (hBass)
    //{
    //    return BASS_MusicGetOrders(music);
    //}
    //else return -1;
    return -1;
}

void CALL HGE::Music_SetPos(HMUSIC music, int order, int row)
{
    //if (hBass)
    {
        BASS_ChannelSetPosition(music, MAKEMUSICPOS(order, row), BASS_POS_MUSIC_ORDER);
    }
}

bool CALL HGE::Music_GetPos(HMUSIC music, int *order, int *row)
{
    //if (hBass)
    {
        QWORD pos;
        pos = BASS_ChannelGetPosition(music, BASS_POS_MUSIC_ORDER);
        if (pos == -1) return false;
        *order = LODWORD(pos);
        *row = HIDWORD(pos);
        return true;
    }
    //else return false;
}

void CALL HGE::Music_SetInstrVolume(HMUSIC music, int instr, int volume)
{
    //if (hBass)
    {
        //BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr, volume);
    }
}

int CALL HGE::Music_GetInstrVolume(HMUSIC music, int instr)
{
    //if (hBass)
    //{
    //    return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr);
    //}
    //else return -1;
    return -1;
}

void CALL HGE::Music_SetChannelVolume(HMUSIC music, int channel, int volume)
{
    //if (hBass)
    {
        //BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel, volume);
    }
}

int CALL HGE::Music_GetChannelVolume(HMUSIC music, int channel)
{
    //if (hBass)
    //{
    //    return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel);
    //}
    //else return -1;
    return -1;
}


HSTREAM CALL HGE::Stream_FromMemory(void* data, DWORD size)
{
    HSTREAM hs;
    CStreamList *stmItem;

    if (bSilent) return (HSTREAM)1;

    hs = BASS_StreamCreateFile(TRUE, data, 0, size, 0);
    if (!hs)
    {
        _PostError("Can't load stream");
        if (!size) Resource_Free(data);
        return 0;
    }
    if (!size)
    {
        stmItem = new CStreamList;
        stmItem->hstream = hs;
        stmItem->data = data;
        stmItem->next = streams;
        streams = stmItem;
    }
    return hs;
}

HSTREAM CALL HGE::Stream_FromFile(const char *filename, DWORD size)
{
    void *data;
    DWORD _size;
    HSTREAM hs;
    CStreamList *stmItem;

    //if (hBass)
    {
        if (bSilent) return (HSTREAM)1;

        if (size) { data = (void *)filename; _size = size; }
        else
        {
            data = Resource_Load(filename, &_size);
            if (!data) return 0;
        }
        hs = BASS_StreamCreateFile(TRUE, data, 0, _size, 0);
        if (!hs)
        {
            _PostError("Can't load stream");
            if (!size) Resource_Free(data);
            return 0;
        }
        if (!size)
        {
            stmItem = new CStreamList;
            stmItem->hstream = hs;
            stmItem->data = data;
            stmItem->next = streams;
            streams = stmItem;
        }
        return hs;
    }
    //else return 0;
}

void CALL HGE::Stream_Free(HSTREAM stream)
{
    CStreamList *stmItem = streams, *stmPrev = 0;

    //if (hBass)
    {
        while (stmItem)
        {
            if (stmItem->hstream == stream)
            {
                if (stmPrev) stmPrev->next = stmItem->next;
                else streams = stmItem->next;
                Resource_Free(stmItem->data);
                delete stmItem;
                break;
            }
            stmPrev = stmItem;
            stmItem = stmItem->next;
        }
        BASS_StreamFree(stream);
    }
}

HCHANNEL CALL HGE::Stream_Play(HSTREAM stream, bool loop, int volume)
{
    //if (hBass)
    {
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(stream, &info);

        // Error
        //BASS_ChannelSetAttributes(stream, info.freq, volume, 0);

        info.flags &= ~BASS_SAMPLE_LOOP;
        if (loop) info.flags |= BASS_SAMPLE_LOOP;
        // Error
        //BASS_ChannelSetFlags(stream, info.flags);
        BASS_ChannelPlay(stream, TRUE);
        return stream;
    }
    //else return 0;
}

void CALL HGE::Channel_SetPanning(HCHANNEL chn, int pan)
{
    //if (hBass) BASS_ChannelSetAttributes(chn, -1, -1, pan);
}

void CALL HGE::Channel_SetVolume(HCHANNEL chn, int volume)
{
    //if (hBass) BASS_ChannelSetAttributes(chn, -1, volume, -101);
}

void CALL HGE::Channel_SetPitch(HCHANNEL chn, float pitch)
{
    //if (hBass)
    {
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(chn, &info);
        //BASS_ChannelSetAttributes(chn, (int)(pitch*info.freq), -1, -101);
    }
}

void CALL HGE::Channel_Pause(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelPause(chn);
}

void CALL HGE::Channel_Resume(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelPlay(chn, FALSE);
}

void CALL HGE::Channel_Stop(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelStop(chn);
}

void CALL HGE::Channel_PauseAll()
{
    //if (hBass) 
    BASS_Pause();
}

void CALL HGE::Channel_ResumeAll()
{
    //if (hBass)
    BASS_Start();
}

void CALL HGE::Channel_StopAll()
{
    //if (hBass)
    {
        BASS_Stop();
        BASS_Start();
    }
}

bool CALL HGE::Channel_IsPlaying(HCHANNEL chn)
{
    //if (hBass)
    {
        if (BASS_ChannelIsActive(chn) == BASS_ACTIVE_PLAYING) return true;
        else return false;
    }
    //else return false;
}

float CALL HGE::Channel_GetLength(HCHANNEL chn)
{
    //if (hBass)
    //{
    //    return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetLength(chn));
    //}
    //else return -1;
    return -1;
}

float CALL HGE::Channel_GetPos(HCHANNEL chn)
{
    //if (hBass)
    //{
    //    return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetPosition(chn));
    //}
    //else return -1;
    return -1;
}

void CALL HGE::Channel_SetPos(HCHANNEL chn, float fSeconds)
{
    //if (hBass)
    {
        BASS_ChannelSetPosition(chn, BASS_ChannelSeconds2Bytes(chn, fSeconds), BASS_POS_MUSIC_ORDER);
    }
}

void CALL HGE::Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan, float pitch)
{
    //if (hBass)
    {
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(channel, &info);

        int freq;
        if (pitch == -1) freq = -1;
        else freq = (int)(pitch*info.freq);

        //BASS_ChannelSlideAttributes(channel, freq, volume, pan, DWORD(time * 1000));
    }
}

bool CALL HGE::Channel_IsSliding(HCHANNEL channel, DWORD attrib)
{
    //if (hBass)
    {
        if (BASS_ChannelIsSliding(channel, attrib)) return true;
        else return false;
    }
    //else return false;
}


//////// Implementation ////////


bool HGE::_SoundInit()
{
    if (!System_GetState(HGE_USESOUND)) return true;

    //hBass = LoadLibrary("bass.dll");
    //if (!hBass)
    //{
    //    _PostError("Can't load BASS.DLL");
    //    return false;
    //}

    //LOADBASSFUNCTION(BASS_GetVersion);

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
    {
        _PostError("Incorrect BASS.DLL version");
        return false;
    }

    //LOADBASSFUNCTION(BASS_GetDeviceDescription);
    //LOADBASSFUNCTION(BASS_Init);
    //LOADBASSFUNCTION(BASS_Free);
    //LOADBASSFUNCTION(BASS_Start);
    //LOADBASSFUNCTION(BASS_Pause);
    //LOADBASSFUNCTION(BASS_Stop);
    //LOADBASSFUNCTION(BASS_SetConfig);
    //LOADBASSFUNCTION(BASS_ErrorGetCode);

    //LOADBASSFUNCTION(BASS_SampleLoad);
    //LOADBASSFUNCTION(BASS_SampleCreate);
    //LOADBASSFUNCTION(BASS_SampleCreateDone);
    //LOADBASSFUNCTION(BASS_SampleGetInfo);
    //LOADBASSFUNCTION(BASS_SampleGetChannel);
    //LOADBASSFUNCTION(BASS_SampleFree);

    //LOADBASSFUNCTION(BASS_MusicLoad);
    //LOADBASSFUNCTION(BASS_MusicFree);
    //LOADBASSFUNCTION(BASS_MusicGetOrders);
    //LOADBASSFUNCTION(BASS_MusicGetOrderPosition);
    //LOADBASSFUNCTION(BASS_MusicSetAttribute);
    //LOADBASSFUNCTION(BASS_MusicGetAttribute);

    //LOADBASSFUNCTION(BASS_StreamCreateFile);
    //LOADBASSFUNCTION(BASS_StreamFree);

    //LOADBASSFUNCTION(BASS_ChannelGetInfo);
    //LOADBASSFUNCTION(BASS_ChannelGetAttributes);
    //LOADBASSFUNCTION(BASS_ChannelSetAttributes);
    //LOADBASSFUNCTION(BASS_ChannelSlideAttributes);
    //LOADBASSFUNCTION(BASS_ChannelIsSliding);
    //LOADBASSFUNCTION(BASS_ChannelSetFlags);
    //LOADBASSFUNCTION(BASS_ChannelGetData);
    //LOADBASSFUNCTION(BASS_ChannelPlay);
    //LOADBASSFUNCTION(BASS_ChannelPause);
    //LOADBASSFUNCTION(BASS_ChannelStop);
    //LOADBASSFUNCTION(BASS_ChannelIsActive);
    //LOADBASSFUNCTION(BASS_ChannelGetLength);
    //LOADBASSFUNCTION(BASS_ChannelGetPosition);
    //LOADBASSFUNCTION(BASS_ChannelSetPosition);
    //LOADBASSFUNCTION(BASS_ChannelSeconds2Bytes);
    //LOADBASSFUNCTION(BASS_ChannelBytes2Seconds);

    bSilent = false;

    if (!BASS_Init(-1, System_GetState(HGE_SAMPLERATE), 0, (HWND)System_GetState(HGE_HWND), NULL))
    {
        this->System_Log("BASS Init failed, using no sound");
        BASS_Init(0, System_GetState(HGE_SAMPLERATE), 0, (HWND)System_GetState(HGE_HWND), NULL);
        bSilent = true;
    }
    else
    {
        // Error
        //this->System_Log("Sound Device: %s", BASS_GetDeviceDescription(1));
        this->System_Log("Sample rate: %ld\n", System_GetState(HGE_SAMPLERATE));
    }

    //BASS_SetConfig(BASS_CONFIG_BUFFER, 5000);
    //BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 50);

    _SetFXVolume(System_GetState(HGE_FXVOLUME));
    _SetMusVolume(System_GetState(HGE_MUSVOLUME));
    _SetStreamVolume(System_GetState(HGE_STREAMVOLUME));

    return true;
}

void HGE::_SoundDone()
{
    CStreamList *stmItem = streams, *stmNext;

    //if (hBass)
    {
        BASS_Stop();
        BASS_Free();

        //int err = BASS_ErrorGetCode(); 

        //FreeLibrary(hBass);
        //hBass = 0;

        while (stmItem)
        {
            stmNext = stmItem->next;
            Resource_Free(stmItem->data);
            delete stmItem;
            stmItem = stmNext;
        }
        streams = 0;
    }
}

void HGE::_SetMusVolume(int vol)
{
    //if (hBass) 
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, vol);
}

void HGE::_SetStreamVolume(int vol)
{
    //if (hBass) 
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol);
}

void HGE::_SetFXVolume(int vol)
{
    //if (hBass) 
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
}
