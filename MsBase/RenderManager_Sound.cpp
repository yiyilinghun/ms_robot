#include "Precompiled.h"
#include "bass.h"
#if defined(_WIN64)
#pragma comment(lib, "..\\MsBase\\bass_64.lib")
#else
#pragma comment(lib, "..\\MsBase\\bass_32.lib")
#endif

#define LOADBASSFUNCTION(f) *((void **) &f) = (void *) GetProcAddress(hBass, #f)
#define MAKEMUSICPOS(order,row) (0x80000000|MAKELONG(order,row))

#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

#pragma warning(disable:4996)

Int32 g_nRef = 0;

HEFFECT RenderManager::Effect_FromMemory(void* data, DWORD size)
{
    //DWORD length, samples;
    HSAMPLE hs;
    //HSTREAM hstrm;
    //BASS_CHANNELINFO info;
    //void *buffer;

    hs = BASS_SampleLoad(True, data, 0, size, 4, BASS_SAMPLE_OVER_VOL);
    //if (!hs)
    //{
    //    hstrm = BASS_StreamCreateFile(True, data, 0, size, BASS_STREAM_DECODE);
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
    //            AssertLog("Can't create sound effect: Not enough memory");
    //        }
    //        else
    //        {
    //            BASS_ChannelGetData(hstrm, buffer, length);
    //            // Error
    //            //hs = BASS_SampleCreateDone();
    //            BASS_StreamFree(hstrm);
    //            if (!hs)    AssertLog("Can't create sound effect");
    //        }
    //    }
    //}

    return hs;
}

HEFFECT RenderManager::Effect_FromFile(const char *filename, DWORD size)
{
    DWORD _size, length, samples;
    HSAMPLE hs;
    HSTREAM hstrm;
    BASS_CHANNELINFO info;
    void *buffer, *data;

    //if (hBass)
    {
        if (bSilent) return (HEFFECT)1;

        if (size)
        {
            data = (void *)filename; _size = size;

            hs = BASS_SampleLoad(True, data, 0, _size, 4, BASS_SAMPLE_OVER_VOL);
            if (!hs)
            {
                hstrm = BASS_StreamCreateFile(True, data, 0, _size, BASS_STREAM_DECODE);
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
                        AssertLog("Can't create sound effect: Not enough memory");
                    }
                    else
                    {
                        BASS_ChannelGetData(hstrm, buffer, length);
                        // Error
                        //hs = BASS_SampleCreateDone();
                        BASS_StreamFree(hstrm);
                        if (!hs)
                        {
                            AssertLog("Can't create sound effect");
                        }
                    }
                }
            }

            if (!size) Resource_Free(data);
            return hs;
        }
        //else
        //{
        //    data = Resource_Load(filename, &_size);
        //    if (!data) return NULL;
        //}
    }
    //else return 0;
    return NULL;
}

HCHANNEL RenderManager::Effect_Play(HEFFECT eff)
{
    if (m_StateBoolean.m_ReadySound)
    {
        HCHANNEL chn;
        chn = BASS_SampleGetChannel(eff, False);
        BASS_ChannelPlay(chn, True);
        return chn;
    }
    return NULL;
}

//HCHANNEL RenderManager::Effect_PlayEx(HEFFECT eff, Int32 volume, Int32 pan, Single pitch, Boolean loop)
//{
//    BASS_SAMPLE info;
//    HCHANNEL chn;
//    BASS_SampleGetInfo(eff, &info);
//
//    chn = BASS_SampleGetChannel(eff, False);
//    // Error
//    //BASS_ChannelSetAttribute(chn, (Int32)(pitch*info.freq), volume, pan);
//
//    info.flags &= ~BASS_SAMPLE_LOOP;
//    if (loop) info.flags |= BASS_SAMPLE_LOOP;
//
//    // Error
//    //BASS_ChannelSetFlags(chn, info.flags);
//    BASS_ChannelPlay(chn, True);
//    return chn;
//}


void RenderManager::Effect_Free(HEFFECT eff)
{
    //if (hBass) 
    BASS_SampleFree(eff);
}


HMUSIC RenderManager::Music_Load(const char *filename, DWORD size)
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

            hm = BASS_MusicLoad(True, data, 0, 0, BASS_MUSIC_PRESCAN | BASS_MUSIC_POSRESETEX | BASS_MUSIC_RAMP, 0);
            if (!hm)
            {
                AssertLog("Can't load music");
            }
            if (!size) Resource_Free(data);
            return hm;
        }
        //else
        //{
        //    data = Resource_Load(filename, &_size);
        //    if (!data) return 0;
        //}
    }
    //else return 0;
    return NULL;
}

HCHANNEL RenderManager::Music_Play(Int32 order, Int32 row)
{
    return this->Music_Play(m_hMusic, True, 100, order, row);
}

HCHANNEL RenderManager::Music_Play(HMUSIC mus, Boolean loop, Int32 volume, Int32 order, Int32 row)
{
    if (this->m_StateBoolean.m_bMusic)
    {
        BASS_ChannelSetPosition(mus, MAKEMUSICPOS(order, row), BASS_POS_BYTE);
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(mus, &info);
        Channel_SetVolume(mus, volume);
        info.flags &= ~BASS_SAMPLE_LOOP;
        if (loop) info.flags |= BASS_SAMPLE_LOOP;
        BASS_ChannelFlags(mus, info.flags, 0xFFFFFFFF);
        BASS_ChannelPlay(mus, False);
        return mus;
    }
    return NULL;
}

Boolean RenderManager::Music_Pause()
{
    return this->Music_Pause(m_hMusic);
}

Boolean RenderManager::Music_Pause(HMUSIC mus)
{
    return BASS_ChannelPause(mus) ? True : False;
}

void RenderManager::Music_Free(HMUSIC mus)
{
    //if (hBass) 
    BASS_MusicFree(mus);
}

//void RenderManager::Music_SetAmplification(HMUSIC music, Int32 ampl)
//{
//    //if (hBass) BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY, ampl);
//}
//
//Int32 RenderManager::Music_GetAmplification(HMUSIC music)
//{
//    //if (hBass) return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY);
//    //else return -1;
//    return -1;
//}

//Int32 RenderManager::Music_GetLength(HMUSIC music)
//{
//    //if (hBass)
//    //{
//    //    return BASS_MusicGetOrders(music);
//    //}
//    //else return -1;
//    return -1;
//}

void RenderManager::Music_SetPos(HMUSIC music, Int32 order, Int32 row)
{
    BASS_ChannelSetPosition(music, MAKEMUSICPOS(order, row), BASS_POS_MUSIC_ORDER);
}

Boolean RenderManager::Music_GetPos(HMUSIC music, Int32 *order, Int32 *row)
{
    //if (hBass)
    {
        QWORD pos;
        pos = BASS_ChannelGetPosition(music, BASS_POS_MUSIC_ORDER);
        if (pos == -1) return False;
        *order = LODWORD(pos);
        *row = HIDWORD(pos);
        return True;
    }
    //else return False;
}

//void RenderManager::Music_SetInstrVolume(HMUSIC music, Int32 instr, Int32 volume)
//{
//    //if (hBass)
//    {
//        //BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr, volume);
//    }
//}

//Int32 RenderManager::Music_GetInstrVolume(HMUSIC music, Int32 instr)
//{
//    //if (hBass)
//    //{
//    //    return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr);
//    //}
//    //else return -1;
//    return -1;
//}
//
//void RenderManager::Music_SetChannelVolume(HMUSIC music, Int32 channel, Int32 volume)
//{
//    //if (hBass)
//    {
//        //BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel, volume);
//    }
//}
//
//Int32 RenderManager::Music_GetChannelVolume(HMUSIC music, Int32 channel)
//{
//    //if (hBass)
//    //{
//    //    return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel);
//    //}
//    //else return -1;
//    return -1;
//}


void RenderManager::Stream_FromMemoryToBack(LPVOID data, DWORD size)
{
    m_hMusic = this->Stream_FromMemory(data, size);
    this->SetStreamVolume(10000);
    this->Music_Play(m_hMusic, True);
}

HSTREAM RenderManager::Stream_FromMemory(void* data, DWORD size)
{
    HSTREAM hs;
    CStreamList *stmItem;

    if (bSilent) return (HSTREAM)1;

    hs = BASS_StreamCreateFile(True, data, 0, size, 0);
    if (!hs)
    {
        AssertLog("Can't load stream");
        if (!size) Resource_Free(data);
        return 0;
    }
    if (!size)
    {
        stmItem = NEW CStreamList;
        stmItem->hstream = hs;
        stmItem->data = data;
        stmItem->next = streams;
        streams = stmItem;
    }
    return hs;
}

HSTREAM RenderManager::Stream_FromFile(const char *filename, DWORD size)
{
    void *data;
    DWORD _size;
    HSTREAM hs;
    CStreamList *stmItem;

    //if (hBass)
    {
        if (bSilent) return (HSTREAM)1;

        if (size)
        {
            data = (void *)filename; _size = size;
            hs = BASS_StreamCreateFile(True, data, 0, _size, 0);
            if (!hs)
            {
                AssertLog("Can't load stream");
                if (!size) Resource_Free(data);
                return 0;
            }
            if (!size)
            {
                stmItem = NEW CStreamList;
                stmItem->hstream = hs;
                stmItem->data = data;
                stmItem->next = streams;
                streams = stmItem;
            }
            return hs;
        }
        //else
        //{
        //    //data = Resource_Load(filename, &_size);
        //    if (!data) return 0;
        //}
    }
    //else return 0;
    return NULL;
}

void RenderManager::Stream_Free()
{
    if (m_hMusic)
    {
        this->Stream_Free(m_hMusic);
        m_hMusic = NULL;
    }
}

void RenderManager::Stream_Free(HSTREAM stream)
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

HCHANNEL RenderManager::Stream_Play(HSTREAM stream, Boolean loop, Int32 volume)
{
    //if (hBass)
    {
        BASS_CHANNELINFO info;
        BASS_ChannelGetInfo(stream, &info);

        // Error
        this->Channel_SetVolume(stream, volume);
        //BASS_ChannelSetAttribute(stream, info.freq, volume, 0);

        info.flags &= ~BASS_SAMPLE_LOOP;
        if (loop) info.flags |= BASS_SAMPLE_LOOP;
        // Error
        //BASS_ChannelSetFlags(stream, info.flags);
        BASS_ChannelPlay(stream, True);
        return stream;
    }
    //else return 0;
}

void RenderManager::Channel_SetPanning(HCHANNEL chn, Int32 pan)
{
    BASS_ChannelSetAttribute((DWORD)chn, (DWORD)-1, (Single)pan);
}

void RenderManager::Channel_SetVolume(HCHANNEL chn, Int32 volume)
{
    BASS_ChannelSetAttribute((DWORD)chn, (DWORD)-101, (Single)volume);
}

void RenderManager::Channel_SetPitch(HCHANNEL chn, Single pitch)
{
    BASS_CHANNELINFO info;
    BASS_ChannelGetInfo(chn, &info);
    BASS_ChannelSetAttribute(chn, (Int32)(pitch*info.freq), -101);
}

void RenderManager::Channel_Pause(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelPause(chn);
}

void RenderManager::Channel_Resume(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelPlay(chn, False);
}

void RenderManager::Channel_Stop(HCHANNEL chn)
{
    //if (hBass) 
    BASS_ChannelStop(chn);
}

void RenderManager::Channel_PauseAll()
{
    //if (hBass) 
    BASS_Pause();
}

void RenderManager::Channel_ResumeAll()
{
    //if (hBass)
    BASS_Start();
}

void RenderManager::Channel_StopAll()
{
    //if (hBass)
    {
        BASS_Stop();
        BASS_Start();
    }
}

Boolean RenderManager::Channel_IsPlaying(HCHANNEL chn)
{
    if (BASS_ChannelIsActive(chn) == BASS_ACTIVE_PLAYING) return True;
    else return False;
}

Double RenderManager::Channel_GetLength(HCHANNEL chn)
{
    return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetLength(chn, BASS_POS_MUSIC_ORDER));
}

Double RenderManager::Channel_GetPos(HCHANNEL chn)
{
    return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetPosition(chn, BASS_POS_MUSIC_ORDER));
}

void RenderManager::Channel_SetPos(HCHANNEL chn, Single fSeconds)
{
    BASS_ChannelSetPosition(chn, BASS_ChannelSeconds2Bytes(chn, fSeconds), BASS_POS_MUSIC_ORDER);
}

void RenderManager::Channel_SlideTo(HCHANNEL channel, Single /*time*/, Int32 /*volume*/, Int32 /*pan*/, Single pitch)
{
    BASS_CHANNELINFO info;
    BASS_ChannelGetInfo(channel, &info);

    Int32 freq;
    if (pitch == -1) freq = -1;
    else freq = (Int32)(pitch*info.freq);
}

Boolean RenderManager::Channel_IsSliding(HCHANNEL channel, DWORD attrib)
{
    if (BASS_ChannelIsSliding(channel, attrib)) return True;
    else return False;
}

Boolean RenderManager::ResetSound()
{
    this->_SoundDone();
    return this->_SoundInit();
}

Boolean RenderManager::_SoundInit()
{
    //if (!this->m_StateBoolean.m_bMusic) return True;

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
    {
        AssertLog("Incorrect BASS.DLL version");
        return False;
    }

    bSilent = False;

    if (!BASS_Init(-1, m_StateInt.m_nSampleRate, 0, m_StateHWND.m_hWindow, nullptr))
    {
        AssertLog("BASS Init failed, using no sound");
        BASS_Init(0, m_StateInt.m_nSampleRate, 0, m_StateHWND.m_hWindow, nullptr);
        bSilent = True;
    }
    else
    {
        AssertLog("Sample rate: %ld", m_StateInt.m_nSampleRate);
    }

    //BASS_SetConfig(BASS_CONFIG_BUFFER, 5000);
    //BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 50);

    SetFXVolume(m_StateInt.m_nFXVolume);
    SetMusVolume(m_StateInt.m_nMusVolume);
    SetStreamVolume(m_StateInt.m_nStreamVolume);

    return True;
}

void RenderManager::_SoundDone()
{
    CStreamList *stmItem = streams, *stmNext;

    {
        BASS_Stop();
        BASS_Free();

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

void RenderManager::SetMusVolume(Int32 vol)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, vol);
}

void RenderManager::SetStreamVolume(Int32 vol)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol);
}

void RenderManager::SetFXVolume(Int32 vol)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
}
