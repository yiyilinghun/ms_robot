#pragma once

//#include "hge_def.h"
#include <windows.h>
#include <stdio.h>
#include <map>
#include <list>
#include <set>
#include <tchar.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#include "d3d9.h"
#include "d3dx9.h"

#define D3DFVF_HGEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUFFER_SIZE 4000


typedef float REAL;
#define REAL_MAX            FLT_MAX
#define REAL_MIN            FLT_MIN
#define REAL_TOLERANCE     (FLT_MIN * 100)
#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */

//#pragma warning (disable : 4311)
//#pragma warning (disable : 4302)

struct HgeFont
{
    bool m_IsStroke;
    DWORD m_StrokeColor;
    DWORD m_Color;
    DWORD m_Effect;
    DWORD m_Format;
    LPD3DXFONT m_D3DFont;
    HDC m_GDI_DC;
    HFONT m_GDIFont;
};

enum HGE_E_POS_TYPE
{
    HGE_EPT_NULL,
    HGE_EPT_CENTER,
    HGE_EPT_LEFTTOP,
    HGE_EPT_LEFTBOTTOM,
    HGE_EPT_RIGHTTOP,
    HGE_EPT_RIGHTBOTTOM,
};

struct MS_D3DXFONT_DESCW : public D3DXFONT_DESCW
{
    bool operator<(const MS_D3DXFONT_DESCW& xObj)const
    {
        if (Height < xObj.Height) { return true; }
        else if (Height > xObj.Height) { return false; }

        if (Width < xObj.Width) { return true; }
        else if (Width > xObj.Width) { return false; }

        if (Weight < xObj.Weight) { return true; }
        else if (Weight > xObj.Weight) { return false; }

        if (MipLevels < xObj.MipLevels) { return true; }
        else if (MipLevels > xObj.MipLevels) { return false; }

        if (Italic < xObj.Italic) { return true; }
        else if (Italic > xObj.Italic) { return false; }

        if (CharSet < xObj.CharSet) { return true; }
        else if (CharSet > xObj.CharSet) { return false; }

        if (OutputPrecision < xObj.OutputPrecision) { return true; }
        else if (OutputPrecision > xObj.OutputPrecision) { return false; }

        if (Quality < xObj.Quality) { return true; }
        else if (Quality > xObj.Quality) { return false; }

        if (PitchAndFamily < xObj.PitchAndFamily) { return true; }
        else if (PitchAndFamily > xObj.PitchAndFamily) { return false; }

        return wcscmp(FaceName, xObj.FaceName) < 0;
    }
};

struct CRenderTargetList
{
    int                 width;
    int                 height;
    IDirect3DTexture9*  pTex;
    IDirect3DSurface9*  pDepth;
    CRenderTargetList*  next;
};

struct CTextureList
{
    HTEXTURE        tex;
    int             width;
    int             height;
    CTextureList*   next;
};

struct CResourceList
{
    char                filename[_MAX_PATH];
    char                password[64];
    CResourceList*      next;
};

struct CInputEventList
{
    hgeInputEvent       event;
    CInputEventList*    next;
};

struct CStreamList
{
    HSTREAM         hstream;
    void*           data;
    CStreamList*    next;
};

class HGE;
extern bool g_IsRestart;
extern LONG g_WndX;
extern LONG g_WndY;
extern HGE* g_temp_hge;
extern std::map<HWND, HGE*> ms_map;
typedef BOOL(WINAPI *GetSystemPowerStatusFunc)(LPSYSTEM_POWER_STATUS);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK _WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

typedef void(*ON_GFX_LOST)();

class HGE
{
public:
    HGE();

    void CALL Release();

    bool CALL System_Initiate(LPCTSTR szWndName, ON_GFX_LOST xOnGfxLost);
    ON_GFX_LOST m_lpOnGfxLostFun;

    void CALL System_Shutdown();
    bool CALL System_Start();

    char* CALL System_GetErrorMessage();

    void CALL System_Log_Time(const char* szFormat, ...);
    void CALL System_Log(const char* szFormat, ...);
    bool CALL System_Launch(const char* url);
    void CALL System_Snapshot(const char* filename = 0);
    WNDPROC CALL System_GetWndProc() { return nullptr; };

private:
    void CALL System_SetStateBool(hgeBoolState state, bool value);
    void CALL System_SetStateFunc(hgeFuncState state, hgeCallback value);
    void CALL System_SetStateHwnd(hgeHwndState state, HWINDOW value);
    void CALL System_SetStateInt(hgeIntState state, int value);
    void CALL System_SetStateString(hgeWStringState state, const WCHAR* value);
    void CALL System_SetStateString(hgeStringState state, const char* value);
    bool CALL System_GetStateBool(hgeBoolState state);
    hgeCallback CALL System_GetStateFunc(hgeFuncState state);
    HWINDOW CALL System_GetStateHwnd(hgeHwndState state);
    int CALL System_GetStateInt(hgeIntState state);
    const char* CALL System_GetStateString(hgeStringState state);
    const WCHAR* CALL System_GetStateString(hgeWStringState state);

public:

    inline void System_SetState(hgeBoolState   state, bool        value) { System_SetStateBool(state, value); }
    inline void System_SetState(hgeFuncState   state, hgeCallback value) { System_SetStateFunc(state, value); }
    inline void System_SetState(hgeHwndState   state, HWINDOW     value) { System_SetStateHwnd(state, value); }
    inline void System_SetState(hgeIntState    state, int         value) { System_SetStateInt(state, value); }
    inline void System_SetState(hgeWStringState state, const WCHAR* value) { System_SetStateString(state, value); }
    inline void System_SetState(hgeStringState state, const char* value) { System_SetStateString(state, value); }
    inline bool System_GetState(hgeBoolState   state) { return System_GetStateBool(state); }
    inline hgeCallback System_GetState(hgeFuncState   state) { return System_GetStateFunc(state); }
    inline HWINDOW System_GetState(hgeHwndState   state) { return System_GetStateHwnd(state); }
    inline int System_GetState(hgeIntState    state) { return System_GetStateInt(state); }
    inline const char* System_GetState(hgeStringState state) { return System_GetStateString(state); }
    inline const WCHAR* System_GetState(hgeWStringState state) { return System_GetStateString(state); }

    // ¹â±ê
    HCURSOR CALL System_CreateCursor(int xHotX, int xHotY, int xWidth, int xHeight, void* lpData);
    HCURSOR CALL System_LoadCursor(const char* filename);
    void CALL System_FreeCursor(HCURSOR hCursor);
    void CALL System_SetCursor(HCURSOR hCursor) { m_hCursor = hCursor; }
    HCURSOR CALL System_GetCursor() { return m_hCursor; }

private:
    HCURSOR m_hCursor;

public:
    void* CALL Resource_Load(const char* filename, DWORD *size = 0);
    void  CALL Resource_Free(void *res);
    bool  CALL Resource_AttachPack(const char* filename, const char* password = 0);
    void  CALL Resource_RemovePack(const char* filename);
    void  CALL Resource_RemoveAllPacks();
    char* CALL Resource_MakePath(const char* filename = 0);
    char* CALL Resource_EnumFiles(const char* wildcard = 0);
    char* CALL Resource_EnumFolders(const char* wildcard = 0);

    void  CALL Ini_SetInt(const char* section, const char* name, int value);
    int   CALL Ini_GetInt(const char* section, const char* name, int def_val);
    void  CALL Ini_SetFloat(const char* section, const char* name, float value);
    float CALL Ini_GetFloat(const char* section, const char* name, float def_val);
    void  CALL Ini_SetString(const char* section, const char* name, const char* value);
    char* CALL Ini_GetString(const char* section, const char* name, const char* def_val);

    void  CALL Random_Seed(int seed = 0);
    int   CALL Random_Int(int min, int max);
    float CALL Random_Float(float min, float max);

    float CALL Timer_GetTime();
    float CALL Timer_GetDelta();
    int   CALL Timer_GetFPS();

    HEFFECT  CALL Effect_FromMemory(void* data, DWORD size = 0);
    HEFFECT  CALL Effect_FromFile(const char* filename, DWORD size = 0);
    void     CALL Effect_Free(HEFFECT eff);

    HCHANNEL CALL Effect_Play(HEFFECT eff);
    HCHANNEL CALL Effect_PlayEx(HEFFECT eff, int volume = 100, int pan = 0, float pitch = 1.0f, bool loop = false);

    HMUSIC   CALL Music_Load(const char* filename, DWORD size = 0);
    void     CALL Music_Free(HMUSIC mus);
    HCHANNEL CALL Music_Play(HMUSIC mus, bool loop, int volume = 100, int order = -1, int row = -1);
    bool     CALL Music_Pause(HMUSIC mus);
    void     CALL Music_SetAmplification(HMUSIC music, int ampl);
    int      CALL Music_GetAmplification(HMUSIC music);
    int      CALL Music_GetLength(HMUSIC music);
    void     CALL Music_SetPos(HMUSIC music, int order, int row);
    bool     CALL Music_GetPos(HMUSIC music, int *order, int *row);
    void     CALL Music_SetInstrVolume(HMUSIC music, int instr, int volume);
    int      CALL Music_GetInstrVolume(HMUSIC music, int instr);
    void     CALL Music_SetChannelVolume(HMUSIC music, int channel, int volume);
    int      CALL Music_GetChannelVolume(HMUSIC music, int channel);

    HSTREAM  CALL Stream_FromMemory(void* data, DWORD size = 0);
    HSTREAM  CALL Stream_FromFile(const char* filename, DWORD size = 0);
    void     CALL Stream_Free(HSTREAM stream);
    HCHANNEL CALL Stream_Play(HSTREAM stream, bool loop, int volume = 100);

    void    CALL Channel_SetPanning(HCHANNEL chn, int pan);
    void    CALL Channel_SetVolume(HCHANNEL chn, int volume);
    void    CALL Channel_SetPitch(HCHANNEL chn, float pitch);
    void    CALL Channel_Pause(HCHANNEL chn);
    void    CALL Channel_Resume(HCHANNEL chn);
    void    CALL Channel_Stop(HCHANNEL chn);
    void    CALL Channel_PauseAll();
    void    CALL Channel_ResumeAll();
    void    CALL Channel_StopAll();
    bool    CALL Channel_IsPlaying(HCHANNEL chn);
    float   CALL Channel_GetLength(HCHANNEL chn);
    float   CALL Channel_GetPos(HCHANNEL chn);
    void    CALL Channel_SetPos(HCHANNEL chn, float fSeconds);
    void    CALL Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101, float pitch = -1);
    bool    CALL Channel_IsSliding(HCHANNEL channel, DWORD attrib);

    void    CALL Input_GetMousePos(float *x, float *y);
    void    CALL Input_SetMousePos(float x, float y);
    int     CALL Input_GetMouseWheel();
    bool    CALL Input_IsMouseOver();
    bool    CALL Input_KeyDown(int key);
    bool    CALL Input_KeyUp(int key);
    bool    CALL Input_GetKeyState(int key);
    char*   CALL Input_GetKeyName(int key);
    std::list<int> CALL Input_GetKeyPass();
    std::list<int> CALL Input_GetIMEKey();
    int  CALL Input_GetKey();
    int  CALL Input_GetClickKey();
    int  CALL Input_GetChar();
    bool CALL Input_GetEvent(hgeInputEvent *event);

    bool CALL Gfx_BeginScene(HTARGET target = 0);
    void CALL Gfx_EndScene();

    void        CALL Gfx_Clear(DWORD color);
    void        CALL Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color = 0xFFFFFFFF, float z = 0.5f);
    void        CALL Gfx_RenderTriple(const hgeTriple *triple);
    void        CALL Gfx_RenderQuad(const hgeQuad *quad);
    void        CALL Gfx_RenderTexture(HTEXTURE hTex, RECT& xRect, D3DXVECTOR3& xCenter, D3DXVECTOR3& xPosition, float zw, float zh, DWORD dwColor);
    hgeVertex*  CALL Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim);
    void        CALL Gfx_FinishBatch(int nprim);
    void        CALL Gfx_SetClipping(int x = 0, int y = 0, int w = 0, int h = 0);
    void        CALL Gfx_SetTransform(float x = 0, float y = 0, float dx = 0, float dy = 0, float rot = 0, float hscale = 0, float vscale = 0);

    HSHADER CALL Shader_Create(const char* filename, const char* function = "ps_main", const char* profile = "ps_2_0");
    void    CALL Shader_Free(HSHADER shader);
    void    CALL Gfx_SetShader(HSHADER shader);

    HTARGET     CALL Target_Create(int width, int height, bool zbuffer);
    void        CALL Target_Free(HTARGET target);
    HTEXTURE    CALL Target_GetTexture(HTARGET target);

    HTEXTURE        CALL Texture_Create(int width, int height);
    HTEXTURE        CALL Texture_LoadFromFile(const char* filename, DWORD size = 0, bool bMipmap = false);
    HTEXTURE        CALL Texture_LoadFromMemory(void* data, DWORD size, OUT int& width, OUT int& height, bool bMipmap = false);
    void            CALL Texture_SaveToFile(HTEXTURE tex, const char* filename);
    ID3DXBuffer*    CALL Texture_SaveToMemory(HTEXTURE tex);
    ID3DXBuffer*    CALL Surface_SaveToMemory(PDIRECT3DSURFACE9 xSurface);
    void            CALL Texture_Free(HTEXTURE tex);
    int             CALL Texture_GetWidth(HTEXTURE tex, bool bOriginal = false);
    int             CALL Texture_GetHeight(HTEXTURE tex, bool bOriginal = false);
    DWORD*          CALL Texture_Lock(HTEXTURE tex, bool bReadOnly = true, int left = 0, int top = 0, int width = 0, int height = 0);
    void            CALL Texture_Unlock(HTEXTURE tex);

    HgeFont CALL Font_Create(UINT xWidth, UINT xHeight, UINT xWeight, DWORD xFontColor, const wchar_t* xFaceName, BOOL xItalic = FALSE);
    void    CALL Font_Free(HgeFont xHgeFont);
    SIZE    CALL Font_GetDrawTextRectA(HgeFont xFont, std::string xStr, int xStrLen = -1);
    INT     CALL Font_GetDrawMaxTextCountInWidthA(HgeFont xFont, INT xWidth, std::string xStr);
    INT     CALL Font_GetDrawMaxTextCountInWidthDescA(HgeFont xFont, INT xWidth, std::string xStr);
    SIZE    CALL Font_GetDrawTextRectW(HgeFont xFont, std::wstring xStr, int xStrLen = -1);
    INT     CALL Font_GetDrawMaxTextCountInWidthW(HgeFont xFont, INT xWidth, std::wstring xStr);
    INT     CALL Font_GetDrawMaxTextCountInWidthDescW(HgeFont xFont, INT xWidth, std::wstring xStr);
    INT     CALL Font_DrawTextA(HgeFont xFont, std::string xStr, LONG x1, LONG y1);
    INT     CALL Font_DrawTextW(HgeFont xFont, std::wstring xStr, LONG x1, LONG y1);
    INT     CALL Font_DrawTextA(HgeFont xFont, std::string xStr, LONG x1, LONG y1, LONG w, LONG h);
    INT     CALL Font_DrawTextW(HgeFont xFont, std::wstring xStr, LONG x1, LONG y1, LONG w, LONG h);

    void    CALL IME_StrDraw(HgeFont& xFont, LONG x, LONG y, LONG xCharH);
    void    CALL IME_StrDraw(HgeFont& xFont, HGE_E_POS_TYPE xPosType, LONG dwOffsetX, LONG dwOffsetY, LONG xCharH);

    //std::map<MS_D3DXFONT_DESCW, LPD3DXFONT> m_MapD3DFont;
    //std::map<MS_D3DXFONT_DESCW, HFONT> m_MapGDIFont;


    HDC  CALL DC_Get() { return ::GetDC((HWND)m_hWindow); }

    bool CALL BeginScene(DWORD color);
    void CALL EndScene();

    void    CALL PushRenderTarget();
    void    CALL PopRenderTarget();
    void    CALL RenderTexture_Create();
    void    CALL RenderTexture_Begin();
    void    CALL RenderTexture_Clear(DWORD color);
    LPDWORD CALL RenderTexture_End();
    void    CALL RenderTexture_UnLock();

    bool    Get_GDI_Render();
    void    Set_GDI_Render(bool Is_GDI_Draw);

private:
    ULONG_PTR           m_gdiplusToken;
    BITMAPINFO          m_BitmapInfo;
    bool                m_Is_GDI_Draw;
    LPDIRECT3DTEXTURE9  m_RenderTexture;
    PDIRECT3DSURFACE9   m_RenderSurface;
    PDIRECT3DSURFACE9   m_RenderBakBuff;

    LPDIRECT3DTEXTURE9  m_CanEdit_RenderTexture;
    PDIRECT3DSURFACE9   m_CanEdit_RenderSurface;


public:
    LRESULT HGE_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    HWND m_hParent;
    TCHAR m_szWndName[100];


    std::set<std::string> m_Set_AdapterMode;
    std::string strIMECharList[10];
    DWORD       uIMECharCount;

private:

    bool    _GfxInit();
    void    _GfxDone();
    bool    _GfxRestore();
    void    _AdjustWindow();
    void    _Resize(int width, int height);
    bool    _init_lost();
    int     _format_id(D3DFORMAT fmt);
    void    _render_batch(bool bEndScene = false);
    void    _SetBlendMode(int blend);
    void    _SetProjectionMatrix(int width, int height);

    // Graphics
    D3DPRESENT_PARAMETERS*  d3dpp;

    D3DPRESENT_PARAMETERS   d3dppW;
    RECT                    rectW;
    LONG                    styleW;

    D3DPRESENT_PARAMETERS   d3dppFS;
    RECT                    rectFS;
    LONG                    styleFS;

    IDirect3D9*             pD3D;
    IDirect3DDevice9*       pD3DDevice;
    IDirect3DVertexBuffer9* pVB;
    IDirect3DIndexBuffer9*  pIB;
    LPD3DXSPRITE            pSprite;


    IDirect3DSurface9*      pScreenSurf;
    IDirect3DSurface9*      pScreenDepth;
    CRenderTargetList*      pTargets;
    CRenderTargetList*      pCurTarget;

    D3DXMATRIX              matView;
    D3DXMATRIX              matProj;

    CTextureList*           textures;
    hgeVertex*              VertArray;

    int                     nPrim;
    int                     CurPrimType;
    int                     CurBlendMode;
    HTEXTURE                CurTexture;
    HTEXTURE                CurShader;

    char                    szError[256];
    void                    _PostError(char *error);

    //HGE* _GetPlugin(int type);
    //void _SetPlugin(int type, HGE*plugin);

    //typedef std::map<int, HGE*> PluginMap;
    //PluginMap m_Plugins;


    // GetStateBool values
    bool    m_bDontSuspend;
    bool    m_bHideMouse;
    bool    m_bShowSplash;
    bool    m_bTextureFilter;
    bool    m_bUseSound;
    bool    m_bWindowed;
    bool    m_bZBuffer;

    // GetStateFunc values
    bool(*m_FunctionFrame)();
    bool(*m_FunctionRender)();
    bool(*m_FunctionFocusLost)();
    bool(*m_FunctionFocusGain)();
    bool(*m_FunctionGfxRestore)();
    bool(*m_FunctionExit)();

    // GetStateHwnd values
    HWINDOW m_hWindow;
    HWINDOW m_hWindowParent;

    // GetStateInt values
    int m_nFXVolume;
    int m_nHGEFPS;
    int m_nMusVolume;
    int m_nPowerStatus;
    int m_nSampleRate;
    int m_nScreenBPP;
    int m_nScreenX;
    int m_nScreenY;
    int m_nScreenHeight;
    int m_nScreenWidth;
    int m_nStreamVolume;

    // GetStateString values
    const WCHAR*  m_lpszIcon;
    WCHAR   m_szWinTitle[256];
    char    m_szIniFile[_MAX_PATH];
    char    m_szLogFile[_MAX_PATH];


    HGE* m_lpMain; // fall back to main dll



    void    _FocusChange(bool bAct);
    //void    _Resize(int width, int height);

    void    _UpdateMouse();
    void    _InputInit();
    void    _ClearQueue();
    void    _BuildEvent(int type, int key, int scan, int flags, int x, int y);

    HINSTANCE   hInstance;
    bool        bActive;
    //char        szError[256];
    char        szAppPath[_MAX_PATH];
    char        szIniString[256];

    //RECT        rectW;
    //LONG        styleW;

    //RECT        rectFS;
    //LONG        styleFS;

    // Input
    std::list<int> IMEChar;
    std::list<int> CharKey;
    int     VKey;
    int     ClickKey;
    int     Char;
    int     Zpos;
    float   Xpos;
    float   Ypos;
    bool    bMouseOver;
    bool    bCaptured;
    char    keyz[256];
    CInputEventList* queue;


    // Resources
    char                szTmpFilename[_MAX_PATH];
    CResourceList*      res;
    HANDLE              hSearch;
    WIN32_FIND_DATAA    SearchData;

    // Power
    int         nPowerStatus;
    HMODULE     hKrnl32;
    GetSystemPowerStatusFunc lpfnGetSystemPowerStatus;

    // Timer
    float   fTime;
    float   fDeltaTime;
    DWORD   nFixedDelta;
    int     nFPS;
    DWORD   t0, t0fps, dt;
    int     cfps;

private:
    //void    _PostError(char *error);

    void    _InitPowerStatus();
    void    _UpdatePowerStatus();
    void    _DonePowerStatus();


    // Audio
    //HINSTANCE       hBass;
    bool            bSilent;
    CStreamList*    streams;

public:
    bool    _SoundInit();
    void    _SoundDone();
    void    _SetMusVolume(int vol);
    void    _SetStreamVolume(int vol);
    void    _SetFXVolume(int vol);

private:

};

#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))

//extern const WCHAR* WINDOW_CLASS_NAME;
//extern HGE* G_HGE;
