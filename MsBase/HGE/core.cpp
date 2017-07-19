#include "hge.h"
#include <time.h>

#pragma warning(disable:4996)

int     nRef = 0;
//HGE*    g_lpHGE = 0;

//#if defined(WIN32) // Windows uses LoadLibrary/GetProcAddress/FreeLibrary
//#include <windows.h>
//#define LOADLIBRARY(n) (HPLUGIN) LoadLibrary(n)
//#define LOADFUNCTION(h, n) (HFUNCTION) GetProcAddress((HMODULE) h, n)
//#else // OSX and Linux use dlopen/sym/close
//#include <dlfcn.h>
//#define LOADLIBRARY(n) (HPLUGIN) dlopen(n, RTLD_LAZY)
//#define LOADFUNCTION(h, n) (HFUNCTION) dlsym((void *) h, n) 
//#endif

//HGE* CALL hgeCreate(int ver)
//{
//    if (ver == HGE_VERSION)
//        return (HGE *)HGE::_Interface_Get();
//    else
//        return 0;
//}
//
//HGE *HGE::_Interface_Get()
//{
//    if (!g_lpHGE) g_lpHGE = new HGE();
//    nRef++;
//
//    return g_lpHGE;
//}

//HGE::HGE()
//{
//    // load default libraries (if they don't exist they are ignored)
////#ifdef _DEBUG
////	Plugin_Load(HGE_PLUGIN_SYSTEM | HGE_PLUGIN_INPUT, "hge_windows_d.hgep");
////	Plugin_Load(HGE_PLUGIN_RENDERER, "hge_dx_d.hgep");
////	Plugin_Load(HGE_PLUGIN_SOUND, "hge_bass_d.hgep");
////#else
//    Plugin_Load(HGE_PLUGIN_SYSTEM | HGE_PLUGIN_INPUT, "hge_windows.dll");
//    Plugin_Load(HGE_PLUGIN_RENDERER, "hge_dx.dll");
//    Plugin_Load(HGE_PLUGIN_SOUND, "hge_bass.dll");
//    //#endif
//}

//void CALL HGE::Plugin_Load(int type, const char *filename)
//{
//    int cur_type = 1;
//    HPLUGIN hPlugin = LOADLIBRARY(filename);
//    LpFnPluginInitialize fnPluginInit;
//
//    if (hPlugin != NULL)
//    {
//        fnPluginInit = (LpFnPluginInitialize)LOADFUNCTION(hPlugin, "Plugin_Initialize");
//
//        if (fnPluginInit != NULL)
//        {
//            while (cur_type <= type)
//            {
//                if (type & cur_type)
//                {
//                    _SetPlugin(cur_type, fnPluginInit(this));
//                }
//
//                cur_type = cur_type << 1;
//            }
//        }
//    }
//}

//void CALL HGE::Plugin_Free(int type)
//{
//    PluginMap::iterator itr;
//
//    if (type == -1)
//    {
//        for (itr = m_Plugins.begin(); itr != m_Plugins.end(); itr = m_Plugins.begin())
//        {
//            Plugin_Free(itr->first);
//        }
//    }
//    else
//    {
//        itr = m_Plugins.find(type);
//
//        if (itr != m_Plugins.end())
//        {
//            itr->second->Release();
//            m_Plugins.erase(itr->first);
//        }
//    }
//}

//void HGE::_SetPlugin(int type, HGE * lpHGE)
//{
//    PluginMap::iterator itr = m_Plugins.find(type);
//
//    if (itr != m_Plugins.end())
//    {
//        Plugin_Free(itr->first);
//    }
//
//    m_Plugins[type] = lpHGE;
//}
//
//HGE * HGE::_GetPlugin(int type)
//{
//    PluginMap::iterator itr = m_Plugins.find(type);
//
//    if (itr != m_Plugins.end())
//    {
//        return itr->second;
//    }
//
//    return NULL;
//}

//void CALL HGE::Release()
//{
//    nRef--;
//
//    if (nRef == 0)
//    {
//        Resource_RemoveAllPacks();
//        Plugin_Free(-1);
//
//        delete g_lpHGE;
//        g_lpHGE = 0;
//    }
//}

//bool CALL HGE::System_Initiate(HWND hWnd)
//{
//    // initialize plugins in order, then call function
//    int cur_type = 1;
//    bool bReturnValue = true;
//
//    while ((cur_type <= HGE_PLUGIN_ALL) && bReturnValue)
//    {
//        if (_GetPlugin(cur_type) != NULL)
//        {
//            // all plugins should extend System_Initiate
//            bReturnValue = _GetPlugin(cur_type)->System_Initiate(hWnd);
//        }
//
//        cur_type <<= 1;
//    }
//
//    return bReturnValue;
//}
//
//void CALL HGE::System_Shutdown()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->System_Shutdown();
//}
//
//bool CALL HGE::System_Start()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->System_Start();
//    else
//        return false;
//}
//
//void CALL HGE::System_SetStateBool(hgeBoolState state, bool value)
//{
//    switch (state)
//    {
//        case HGE_WINDOWED:
//        {
//            if (m_hWindow) break;
//            m_bWindowed = value;
//        } break;
//
//        case HGE_ZBUFFER:
//        {
//            if (m_hWindow) break;
//            m_bZBuffer = value;
//        } break;
//
//        case HGE_TEXTUREFILTER:
//        {
//            if (m_hWindow) break;
//            m_bTextureFilter = value;
//        } break;
//
//        case HGE_USESOUND:
//        {
//            if (m_bUseSound != value)
//            {
//                m_bUseSound = value;
//
//                // init sound plugin here if required
//                if (m_bUseSound && m_hWindow) m_bUseSound = true; //_SoundInit();
//                if (!m_bUseSound && m_hWindow) m_bUseSound = false; //_SoundDone();
//            }
//        } break;
//
//        case HGE_HIDEMOUSE:
//        {
//            m_bHideMouse = value;
//        } break;
//
//        case HGE_DONTSUSPEND:
//        {
//            m_bDontSuspend = value;
//        } break;
//
//        case HGE_SHOWSPLASH:
//        {
//            m_bShowSplash = value;
//        } break;
//    }
//}
//
//void CALL HGE::System_SetStateFunc(hgeFuncState state, hgeCallback value)
//{
//    switch (state)
//    {
//        case HGE_FRAMEFUNC:		 m_FunctionFrame = value;      break;
//        case HGE_RENDERFUNC:	 m_FunctionRender = value;     break;
//        case HGE_FOCUSLOSTFUNC:	 m_FunctionFocusLost = value;  break;
//        case HGE_FOCUSGAINFUNC:	 m_FunctionFocusGain = value;  break;
//        case HGE_GFXRESTOREFUNC: m_FunctionGfxRestore = value; break;
//        case HGE_EXITFUNC:		 m_FunctionExit = value;       break;
//    }
//}
//
//void CALL HGE::System_SetStateHwnd(hgeHwndState state, HWINDOW value)
//{
//    switch (state)
//    {
//        case HGE_HWND:		 m_hWindow = value;       break;
//        case HGE_HWNDPARENT: m_hWindowParent = value; break;
//    }
//}
//
//void CALL HGE::System_SetStateInt(hgeIntState state, int value)
//{
//    switch (state)
//    {
//        case HGE_SCREENWIDTH:	if (!m_hWindow) m_nScreenWidth = value;		break;
//        case HGE_SCREENHEIGHT:	if (!m_hWindow) m_nScreenHeight = value;	break;
//        case HGE_SCREENBPP:		if (!m_hWindow) m_nScreenBPP = value;		break;
//        case HGE_SAMPLERATE:	if (!m_hWindow) m_nSampleRate = value;			break; // hBass
//        case HGE_FXVOLUME:		m_nFXVolume = value; /*_SetFXVolume(nFXVolume);*/ break;
//        case HGE_MUSVOLUME:		m_nMusVolume = value; /*_SetMusVolume(nMusVolume);*/ break;
//        case HGE_STREAMVOLUME:	m_nStreamVolume = value; /*_SetStreamVolume(nStreamVolume);*/ break;
//
//        case HGE_FPS:
//        {
//            if (m_nHGEFPS != value)
//            {
//                if (m_hWindow)
//                {
//                    //Gfx_Sync(
//                }
//
//                m_nHGEFPS = value;
//            }
//        } break;
//    }
//}
//
//void CALL HGE::System_SetStateString(hgeStringState state, const char *value)
//{
//    FILE *hf;
//
//    switch (state)
//    {
//        case HGE_ICON:
//        {
//            m_lpszIcon = value;
//
//            //if(pHGE->hwnd) SetClassLong(pHGE->hwnd, GCL_HICON, (LONG)LoadIcon(pHGE->hInstance, szIcon));
//        } break;
//
//        case HGE_TITLE:
//        {
//            if (value)
//                strcpy(m_szWinTitle, value);
//            else
//                strcpy(m_szWinTitle, "HGE");
//
//            //if(pHGE->hwnd) SetWindowText(pHGE->hwnd, szWinTitle);
//        } break;
//
//        case HGE_INIFILE:
//        {
//            if (value)
//                strcpy(m_szIniFile, Resource_MakePath(value));
//            else
//                m_szIniFile[0] = 0;
//        } break;
//
//        case HGE_LOGFILE:
//        {
//            if (value)
//            {
//                strcpy(m_szLogFile, Resource_MakePath(value));
//                hf = fopen(m_szLogFile, "w");
//
//                if (!hf)
//                    m_szLogFile[0] = 0;
//                else
//                    fclose(hf);
//            }
//            else
//            {
//                m_szLogFile[0] = 0;
//            }
//        } break;
//    }
//}
//
//bool CALL HGE::System_GetStateBool(hgeBoolState state)
//{
//    switch (state)
//    {
//        case HGE_WINDOWED:		return m_bWindowed;
//        case HGE_ZBUFFER:		return m_bZBuffer;
//        case HGE_TEXTUREFILTER:	return m_bTextureFilter;
//        case HGE_USESOUND:		return m_bUseSound;
//        case HGE_DONTSUSPEND:	return m_bDontSuspend;
//        case HGE_HIDEMOUSE:		return m_bHideMouse;
//        case HGE_SHOWSPLASH:	return m_bShowSplash;
//    }
//
//    return false;
//}
//
//hgeCallback CALL HGE::System_GetStateFunc(hgeFuncState state)
//{
//    hgeCallback hcbReturnValue = NULL;
//
//    switch (state)
//    {
//        case HGE_FRAMEFUNC:		 hcbReturnValue = m_FunctionFrame;      break;
//        case HGE_RENDERFUNC:	 hcbReturnValue = m_FunctionRender;     break;
//        case HGE_FOCUSLOSTFUNC:	 hcbReturnValue = m_FunctionFocusLost;  break;
//        case HGE_FOCUSGAINFUNC:	 hcbReturnValue = m_FunctionFocusGain;  break;
//        case HGE_GFXRESTOREFUNC: hcbReturnValue = m_FunctionGfxRestore; break;
//        case HGE_EXITFUNC:		 hcbReturnValue = m_FunctionExit;       break;
//    }
//
//    return hcbReturnValue;
//}
//
//HWINDOW CALL HGE::System_GetStateHwnd(hgeHwndState state)
//{
//    switch (state)
//    {
//        case HGE_HWND:			return m_hWindow;
//        case HGE_HWNDPARENT:	return m_hWindowParent;
//    }
//
//    return 0;
//}
//
//int	 CALL HGE::System_GetStateInt(hgeIntState state)
//{
//    switch (state)
//    {
//        case HGE_SCREENWIDTH:	return m_nScreenWidth;
//        case HGE_SCREENHEIGHT:	return m_nScreenHeight;
//        case HGE_SCREENBPP:		return m_nScreenBPP;
//        case HGE_SAMPLERATE:	return m_nSampleRate;
//        case HGE_FXVOLUME:		return m_nFXVolume;
//        case HGE_MUSVOLUME:		return m_nMusVolume;
//        case HGE_STREAMVOLUME:	return m_nStreamVolume;
//        case HGE_FPS:			return m_nHGEFPS;
//        case HGE_POWERSTATUS:	return m_nPowerStatus;
//    }
//
//    return 0;
//}
//
//const char* CALL HGE::System_GetStateString(hgeStringState state)
//{
//    switch (state)
//    {
//        case HGE_ICON:			return m_lpszIcon;
//        case HGE_TITLE:			return m_szWinTitle;
//
//        case HGE_INIFILE:
//        {
//            if (m_szIniFile[0]) return m_szIniFile;
//        } break;
//
//        case HGE_LOGFILE:
//        {
//            if (m_szLogFile[0]) return m_szLogFile;
//        } break;
//    }
//
//    return NULL;
//}
//
//char* CALL HGE::System_GetErrorMessage()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->System_GetErrorMessage();
//    else
//        return 0;
//}
//
//void CALL HGE::System_Log(const char *format, ...)
//{
//    va_list vl;
//    char message[1024];
//
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//    {
//        va_start(vl, format);
//        vsnprintf(message, 1024, format, vl);
//        va_end(vl);
//
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->System_Log(message);
//    }
//}
//
//bool CALL HGE::System_Launch(const char *url)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->System_Launch(url);
//    else
//        return false;
//}
//
//void CALL HGE::System_Snapshot(const char *filename)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->System_Snapshot(filename);
//}
//
//void * CALL HGE::Resource_Load(const char *filename, DWORD *size)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_Load(filename, size);
//    else
//        return 0;
//}
//
//void CALL HGE::Resource_Free(void *res)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_Free(res);
//}
//
//bool CALL HGE::Resource_AttachPack(const char *filename, const char *password)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_AttachPack(filename, password);
//    else
//        return false;
//}
//
//void CALL HGE::Resource_RemovePack(const char *filename)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_RemovePack(filename);
//}
//
//void CALL HGE::Resource_RemoveAllPacks()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_RemoveAllPacks();
//}
//
//char * CALL HGE::Resource_MakePath(const char *filename)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_MakePath(filename);
//    else
//        return 0;
//}
//
//char * CALL HGE::Resource_EnumFiles(const char *wildcard)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_EnumFiles(wildcard);
//    else
//        return 0;
//}
//
//char * CALL HGE::Resource_EnumFolders(const char *wildcard)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Resource_EnumFolders(wildcard);
//    else
//        return 0;
//}
//
//void CALL HGE::Ini_SetInt(const char *section, const char *name, int value)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_SetInt(section, name, value);
//}
//
//int  CALL HGE::Ini_GetInt(const char *section, const char *name, int def_val)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_GetInt(section, name, def_val);
//    else
//        return def_val;
//}
//
//void CALL HGE::Ini_SetFloat(const char *section, const char *name, float value)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_SetFloat(section, name, value);
//}
//
//float CALL HGE::Ini_GetFloat(const char *section, const char *name, float def_val)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_GetFloat(section, name, def_val);
//    else
//        return def_val;
//}
//
//void CALL HGE::Ini_SetString(const char *section, const char *name, const char *value)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM)) _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_SetString(section, name, value);
//}
//
//char * CALL HGE::Ini_GetString(const char *section, const char *name, const char *def_val)
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Ini_GetString(section, name, def_val);
//    else
//        return (char *)def_val;
//}

unsigned int g_seed = 0;

void CALL HGE::Random_Seed(int seed)
{
    if (!seed) g_seed = (unsigned int)time(NULL);
    else g_seed = seed;
}

int	 CALL HGE::Random_Int(int min, int max)
{
    g_seed = 214013 * g_seed + 2531011;
    return min + (g_seed ^ g_seed >> 15) % (max - min + 1);
}

float CALL HGE::Random_Float(float min, float max)
{
    g_seed = 214013 * g_seed + 2531011;
    return min + (g_seed >> 16) * (1.0f / 65535.0f) * (max - min);
}
//
//float CALL HGE::Timer_GetTime()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Timer_GetTime();
//    else
//        return 0;
//}
//
//float CALL HGE::Timer_GetDelta()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Timer_GetDelta();
//    else
//        return 0;
//}
//
//int	 CALL HGE::Timer_GetFPS()
//{
//    if (_GetPlugin(HGE_PLUGIN_SYSTEM))
//        return _GetPlugin(HGE_PLUGIN_SYSTEM)->Timer_GetFPS();
//    else
//        return 0;
//}

//
///*
//** Sound Functions
//*/
//HEFFECT CALL HGE::Effect_Load(const char *filename, DWORD size)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Effect_Load(filename, size);
//    else
//        return 0;
//}
//
//void CALL HGE::Effect_Free(HEFFECT eff)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Effect_Free(eff);
//}
//
//HCHANNEL CALL HGE::Effect_Play(HEFFECT eff)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Effect_Play(eff);
//    else
//        return 0;
//}
//
//HCHANNEL CALL HGE::Effect_PlayEx(HEFFECT eff, int volume, int pan, float pitch, bool loop)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Effect_PlayEx(eff, volume, pan, pitch, loop);
//    else
//        return 0;
//}
//
//HMUSIC CALL HGE::Music_Load(const char *filename, DWORD size)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_Load(filename, size);
//    else
//        return 0;
//}
//
//void CALL HGE::Music_Free(HMUSIC mus)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Music_Free(mus);
//}
//
//HCHANNEL CALL HGE::Music_Play(HMUSIC mus, bool loop, int volume, int order, int row)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_Play(mus, loop, volume, order, row);
//    else
//        return 0;
//}
//
//void CALL HGE::Music_SetAmplification(HMUSIC music, int ampl)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Music_SetAmplification(music, ampl);
//}
//
//int	 CALL HGE::Music_GetAmplification(HMUSIC music)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_GetAmplification(music);
//    else
//        return 0;
//}
//
//int	 CALL HGE::Music_GetLength(HMUSIC music)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_GetLength(music);
//    else
//        return 0;
//}
//
//void CALL HGE::Music_SetPos(HMUSIC music, int order, int row)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Music_SetPos(music, order, row);
//}
//
//bool CALL HGE::Music_GetPos(HMUSIC music, int *order, int *row)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_GetPos(music, order, row);
//    else
//        return false;
//}
//
//void CALL HGE::Music_SetInstrVolume(HMUSIC music, int instr, int volume)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Music_SetInstrVolume(music, instr, volume);
//}
//
//int	 CALL HGE::Music_GetInstrVolume(HMUSIC music, int instr)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_GetInstrVolume(music, instr);
//    else
//        return 0;
//}
//
//void CALL HGE::Music_SetChannelVolume(HMUSIC music, int channel, int volume)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Music_SetChannelVolume(music, channel, volume);
//}
//
//int	 CALL HGE::Music_GetChannelVolume(HMUSIC music, int channel)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Music_GetChannelVolume(music, channel);
//    else
//        return 0;
//}
//
//HSTREAM CALL HGE::Stream_Load(const char *filename, DWORD size)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Stream_Load(filename, size);
//    else
//        return 0;
//}
//
//void CALL HGE::Stream_Free(HSTREAM stream)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Stream_Free(stream);
//}
//
//HCHANNEL CALL HGE::Stream_Play(HSTREAM stream, bool loop, int volume)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Stream_Play(stream, loop, volume);
//    else
//        return 0;
//}
//
//void CALL HGE::Channel_SetPanning(HCHANNEL chn, int pan)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_SetPanning(chn, pan);
//}
//
//void CALL HGE::Channel_SetVolume(HCHANNEL chn, int volume)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_SetVolume(chn, volume);
//}
//
//void CALL HGE::Channel_SetPitch(HCHANNEL chn, float pitch)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_SetPitch(chn, pitch);
//}
//
//void CALL HGE::Channel_Pause(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_Pause(chn);
//}
//
//void CALL HGE::Channel_Resume(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_Resume(chn);
//}
//
//void CALL HGE::Channel_Stop(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_Stop(chn);
//}
//
//void CALL HGE::Channel_PauseAll()
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_PauseAll();
//}
//
//void CALL HGE::Channel_ResumeAll()
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_ResumeAll();
//}
//
//void CALL HGE::Channel_StopAll()
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_StopAll();
//}
//
//bool CALL HGE::Channel_IsPlaying(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return 	_GetPlugin(HGE_PLUGIN_SOUND)->Channel_IsPlaying(chn);
//    else
//        return false;
//}
//
//float CALL HGE::Channel_GetLength(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return 	_GetPlugin(HGE_PLUGIN_SOUND)->Channel_GetLength(chn);
//    else
//        return 0;
//}
//
//float CALL HGE::Channel_GetPos(HCHANNEL chn)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return 	_GetPlugin(HGE_PLUGIN_SOUND)->Channel_GetPos(chn);
//    else
//        return 0;
//}
//
//void CALL HGE::Channel_SetPos(HCHANNEL chn, float fSeconds)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_SetPos(chn, fSeconds);
//}
//
//void CALL HGE::Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan, float pitch)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND)) _GetPlugin(HGE_PLUGIN_SOUND)->Channel_SlideTo(channel, time, volume, pan, pitch);
//}
//
//bool CALL HGE::Channel_IsSliding(HCHANNEL channel)
//{
//    if (_GetPlugin(HGE_PLUGIN_SOUND))
//        return _GetPlugin(HGE_PLUGIN_SOUND)->Channel_IsSliding(channel);
//    else
//        return false;
//}

//
///*
//** Input Functions
//*/
//void CALL HGE::Input_GetMousePos(float *x, float *y)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT)) _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetMousePos(x, y);
//}
//
//void CALL HGE::Input_SetMousePos(float x, float y)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT)) _GetPlugin(HGE_PLUGIN_INPUT)->Input_SetMousePos(x, y);
//}
//
//int	 CALL HGE::Input_GetMouseWheel()
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetMouseWheel();
//    else
//        return 0;
//}
//
//bool CALL HGE::Input_IsMouseOver()
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_IsMouseOver();
//    else
//        return false;
//}
//
//bool CALL HGE::Input_KeyDown(int key)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_KeyDown(key);
//    else
//        return false;
//}
//
//bool CALL HGE::Input_KeyUp(int key)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_KeyUp(key);
//    else
//        return false;
//}
//
//bool CALL HGE::Input_GetKeyState(int key)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetKeyState(key);
//    else
//        return false;
//}
//
//char* CALL HGE::Input_GetKeyName(int key)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetKeyName(key);
//    else
//        return 0;
//}
//
//int	 CALL HGE::Input_GetKey()
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetKey();
//    else
//        return 0;
//}
//
//int	 CALL HGE::Input_GetChar()
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetChar();
//    else
//        return 0;
//}
//
//bool CALL HGE::Input_GetEvent(hgeInputEvent *evt)
//{
//    if (_GetPlugin(HGE_PLUGIN_INPUT))
//        return _GetPlugin(HGE_PLUGIN_INPUT)->Input_GetEvent(evt);
//    else
//        return false;
//}
//
//
///*
//** Renderer functions
//*/
//bool CALL HGE::Gfx_BeginScene(HTARGET target)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_BeginScene(target);
//    else
//        return false;
//}
//
//void CALL HGE::Gfx_EndScene()
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_EndScene();
//}
//
//void CALL HGE::Gfx_Clear(DWORD color)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_Clear(color);
//}
//
//void CALL HGE::Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color, float z)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_RenderLine(x1, y1, x2, y2, color, z);
//}
//
//void CALL HGE::Gfx_RenderTriple(const hgeTriple *triple)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_RenderTriple(triple);
//}
//
//void CALL HGE::Gfx_RenderQuad(const hgeQuad *quad)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_RenderQuad(quad);
//}
//
//hgeVertex* CALL HGE::Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_StartBatch(prim_type, tex, blend, max_prim);
//    else
//        return 0;
//}
//
//void CALL HGE::Gfx_FinishBatch(int nprim)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_FinishBatch(nprim);
//}
//
//void CALL HGE::Gfx_SetClipping(int x, int y, int w, int h)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_SetClipping(x, y, w, h);
//}
//
//void CALL HGE::Gfx_SetTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_SetTransform(x, y, dx, dy, rot, hscale, vscale);
//}
//
//
//HSHADER CALL HGE::Shader_Create(const char *filename, const char *function, const char *profile)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Shader_Create(filename, function, profile);
//    else
//        return 0;
//}
//
//void CALL HGE::Shader_Free(HSHADER shader)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Shader_Free(shader);
//}
//
//void CALL HGE::Gfx_SetShader(HSHADER shader)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Gfx_SetShader(shader);
//}
//
//HTARGET CALL HGE::Target_Create(int width, int height, bool zbuffer)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Target_Create(width, height, zbuffer);
//    else
//        return 0;
//}
//
//void CALL HGE::Target_Free(HTARGET target)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Target_Free(target);
//}
//
//HTEXTURE CALL HGE::Target_GetTexture(HTARGET target)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Target_GetTexture(target);
//    else
//        return 0;
//}
//
//HTEXTURE CALL HGE::Texture_Create(int width, int height)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_Create(width, height);
//    else
//        return 0;
//}
//
//HTEXTURE CALL HGE::Texture_LoadFromFile(const char* filename, DWORD size, bool bMipmap)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_LoadFromFile(filename, size, bMipmap);
//    else
//        return 0;
//}
//
//HTEXTURE CALL HGE::Texture_LoadFromMemory(void* data, DWORD size, bool bMipmap)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_LoadFromMemory(data, size, bMipmap);
//    else
//        return 0;
//}
//
//void CALL HGE::Texture_Free(HTEXTURE tex)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_Free(tex);
//}
//
//int	 CALL HGE::Texture_GetWidth(HTEXTURE tex, bool bOriginal)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_GetWidth(tex, bOriginal);
//    else
//        return 0;
//}
//
//int CALL HGE::Texture_GetHeight(HTEXTURE tex, bool bOriginal)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_GetHeight(tex, bOriginal);
//    else
//        return 0;
//}
//
//DWORD * CALL HGE::Texture_Lock(HTEXTURE tex, bool bReadOnly, int left, int top, int width, int height)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_Lock(tex, bReadOnly, left, top, width, height);
//    else
//        return 0;
//}
//
//void CALL HGE::Texture_Unlock(HTEXTURE tex)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER)) _GetPlugin(HGE_PLUGIN_RENDERER)->Texture_Unlock(tex);
//}
//
//bool CALL HGE::Font_Create(D3DXFONT_DESCA* xD3dFont, LPD3DXFONT* lpID3DXFont)
//{
//    if (_GetPlugin(HGE_PLUGIN_RENDERER))
//        return _GetPlugin(HGE_PLUGIN_RENDERER)->Font_Create(xD3dFont, lpID3DXFont);
//    else
//        return false;
//}
