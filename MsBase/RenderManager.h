#pragma once
#include "RenderManagerDef.h"

struct UI_Key
{
    UI_Key(mstr xName, DWORD xId) : m_Name(xName), m_Id(xId) { ; }
    UI_Key(DWORD xId, mstr xName) : m_Name(xName), m_Id(xId) { ; }
    mstr    m_Name;
    DWORD   m_Id;
};

struct UIResInfo
{
    UIResInfo(Int32 xBegin, Int32 xEnd, UI_Key xResKey, LPVOID xWasDecoder)
        : m_Begin(xBegin), m_End(xEnd), m_ResKey(xResKey), m_WasDecoder(xWasDecoder)
    {
        ;
    }
    Int32   m_Begin;
    Int32   m_End;
    Int32   m_RenderPosX = INVALID_NID;
    Int32   m_RowIndex;
    Int32   m_RenderPosY;
    UI_Key  m_ResKey;
    LPVOID  m_WasDecoder;
};

class RenderManager : public IocpUnit
{
public:
    RenderManager(mstr xUnitName, MsIOCPManager* xMsIOCPManager);
    virtual Boolean TickOnce();
    virtual void OnAttach();
    virtual Boolean Release() = 0;

    Boolean m_IsLost = False;
    Int32 m_TempFPS;
    Int64 m_LastFPSTime = MsBase::Now();
    Int64 m_LastStatisticFPSTime = MsBase::Now();
    Boolean m_IsClose;
    Boolean m_IsReConfig = False;
    Int32 GetFPS();
private:
    Int32 m_FPS;
public:
    LONG m_ScreenWidth = 0;
    LONG m_ScreenHeight = 0;
    LONG m_ReadyScreenWidth = 0;
    LONG m_ReadyScreenHeight = 0;
    LONG m_ClientWidth = 0;
    LONG m_ClientHeight = 0;

    void System_Shutdown();
    //void System_Wait();

    //LPSTR System_GetErrorMessage();

    Boolean InitSound();

    Boolean System_Launch(LPCSTR url);
    void    System_Snapshot(LPCSTR xPath = nullptr);
    WNDPROC System_GetWndProc() { return nullptr; };

    void CloseGame(Boolean IsRestart = False);
    static Boolean IsReadyCloseGame();
protected:
    Boolean _System_Initiate(LPCTSTR szWndName, HICON hIcon);

public:
    static Boolean ms_IsRestart;
    //static Boolean ms_IsReLogin;

public:
    // ¹â±ê
    HCURSOR System_CreateCursor(Int32 xHotX, Int32 xHotY, Int32 xWidth, Int32 xHeight, LPVOID lpData);
    HCURSOR System_LoadCursor(LPCSTR xFileName);
    void    System_CheckUpdateCursor();
    void    System_ChangeCursor(mstr xCursorName);
    void    System_FreeCursor(HCURSOR hCursor);
    void    System_AddCursor(mstr xCursorName, HCURSOR hCursor, DWORD xCursorWidth);
    void    System_ClearListCursor(mstr xCursorName);
    void    System_ClearAllCursor();

    MsUnorderedDictionary<mstr, MsVector<HCURSOR>> m_CursorContainer;
    MsUnorderedDictionary<mstr, DWORD> m_CursorWidthContainer;
    DWORD   m_CurrentCursorWidth = 1;
    mstr    m_TempCurrentCursorName;
    Int32   m_CurrentCursorIndex;
    Single  m_CurrentSpeed;
    Int64   m_RenderCursorUpTime;
    static HCURSOR ms_RenderCursor;

public:
    void Resource_Free(LPVOID res);

    HEFFECT     Effect_FromMemory(LPVOID data, DWORD size = 0);
    HEFFECT     Effect_FromFile(LPCSTR filename, DWORD size = 0);
    void        Effect_Free(HEFFECT eff);
    HCHANNEL    Effect_Play(HEFFECT eff);

    HMUSIC      m_hMusic = NULL;
    HMUSIC      Music_Load(LPCSTR filename, DWORD size = 0);
    void        Music_Free(HMUSIC mus);
    HCHANNEL    Music_Play(Int32 order = -1, Int32 row = -1);
    HCHANNEL    Music_Play(HMUSIC mus, Boolean loop, Int32 volume = 100, Int32 order = -1, Int32 row = -1);
    Boolean     Music_Pause();
    Boolean     Music_Pause(HMUSIC mus);
    void        Music_SetPos(HMUSIC music, Int32 order, Int32 row);
    Boolean     Music_GetPos(HMUSIC music, Int32 *order, Int32 *row);

    void        Stream_FromMemoryToBack(LPVOID data, DWORD size = 0);
    HSTREAM     Stream_FromMemory(LPVOID data, DWORD size = 0);
    HSTREAM     Stream_FromFile(LPCSTR filename, DWORD size = 0);
    void        Stream_Free();
    void        Stream_Free(HSTREAM stream);
    HCHANNEL    Stream_Play(HSTREAM stream, Boolean loop, Int32 volume = 100);

    void    Channel_SetPanning(HCHANNEL chn, Int32 pan);
    void    Channel_SetVolume(HCHANNEL chn, Int32 volume);
    void    Channel_SetPitch(HCHANNEL chn, Single pitch);
    void    Channel_Pause(HCHANNEL chn);
    void    Channel_Resume(HCHANNEL chn);
    void    Channel_Stop(HCHANNEL chn);
    void    Channel_PauseAll();
    void    Channel_ResumeAll();
    void    Channel_StopAll();
    Boolean Channel_IsPlaying(HCHANNEL chn);
    Double  Channel_GetLength(HCHANNEL chn);
    Double  Channel_GetPos(HCHANNEL chn);
    void    Channel_SetPos(HCHANNEL chn, Single fSeconds);
    void    Channel_SlideTo(HCHANNEL channel, Single time, Int32 volume, Int32 pan = -101, Single pitch = -1);
    Boolean Channel_IsSliding(HCHANNEL channel, DWORD attrib);

    //Boolean Input_IsMouseOver();
    Boolean Input_GetKeyState(Int32 key);
    LPSTR   Input_GetKeyName(Int32 key);

    POINT   m_MousePos;
    Boolean m_MouseInGame;

    //Single fDeltaTime;
    //Single fTime;
    //Int32 t0, t0fps, dt;
    //Int32 cfps;

    virtual Boolean OnKeyEvent(KEY_EVENT_TYPE xEventType, Int32 xKey) = 0;
    virtual void OnKeyEventFree(KEY_EVENT_TYPE xEventType, Int32 xKey) = 0;
    virtual Boolean OnMouseEvent(MOUSE_EVENT_TYPE xEventType, Int32 xMouseX, Int32 xMouseY) = 0;
    virtual void OnMouseEventFree(MOUSE_EVENT_TYPE xEventType, Int32 xMouseX, Int32 xMouseY) = 0;
    virtual Boolean OnButtonWheel(Int32 xMouseX, Int32 xMouseY, Int32 xParam) = 0;
    virtual void OnButtonWheelFree(Int32 xMouseX, Int32 xMouseY, Int32 xParam) = 0;
    virtual Int32 OnNchitTest(Int32, Int32) { return HTERROR; };
    virtual Int32 OnNchitTestFree(Int32, Int32) { return HTERROR; }

    Boolean Gfx_BeginScene(HTARGET target = 0);
    void    Gfx_EndScene();

    void Gfx_Clear(DWORD color);
    void Gfx_RenderLine(Single x1, Single y1, Single x2, Single y2, DWORD color = 0xFFFFFFFF, Single z = 0.5f);
    void Gfx_RenderTriple(const RenderManagerTriple *xRenderManagerTriple);
    void Gfx_RenderQuad(const RenderManagerQuad *quad);
    void Gfx_RenderTexture(HTEXTURE hTex, RECT& xRect, D3DXVECTOR3& xCenter, D3DXVECTOR3& xPosition, Single zw, Single zh, DWORD dwColor);

    RenderManagerVertex* Gfx_StartBatch(Int32 prim_type, HTEXTURE tex, Int32 blend, Int32 *max_prim);

    void Gfx_FinishBatch(Int32 nprim);
    void Gfx_SetClipping(Int32 x = 0, Int32 y = 0, Int32 w = 0, Int32 h = 0);
    void Gfx_SetTransform(Single x = 0, Single y = 0, Single dx = 0, Single dy = 0, Single rot = 0, Single hscale = 0, Single vscale = 0);

    HTARGET     Target_Create(Int32 width, Int32 height, Boolean zbuffer);
    void        Target_Free(HTARGET target);
    HTEXTURE    Target_GetTexture(HTARGET target);

    HTEXTURE        Texture_Create(Int32 width, Int32 height);
    HTEXTURE        Texture_LoadFromFile(LPCSTR filename, OUT Int32& width, OUT Int32& height, Boolean bMipmap = false);
    HTEXTURE        Texture_LoadFromMemory(LPVOID data, DWORD size, OUT Int32& width, OUT Int32& height, Boolean bMipmap = false);
    void            Texture_SaveToFile(HTEXTURE tex, LPCSTR filename);
    ID3DXBuffer*    Texture_SaveToMemory(HTEXTURE tex);
    ID3DXBuffer*    Surface_SaveToMemory(PDIRECT3DSURFACE9 xSurface);
    void            Texture_Free(HTEXTURE tex);
    Int32           Texture_GetWidth(HTEXTURE tex, Boolean bOriginal = false);
    Int32           Texture_GetHeight(HTEXTURE tex, Boolean bOriginal = false);
    LPDWORD         Texture_Lock(HTEXTURE tex, Boolean bReadOnly = true, Int32 left = 0, Int32 top = 0, Int32 width = 0, Int32 height = 0);
    void            Texture_Unlock(HTEXTURE tex);

    RenderFont  Font_Create(mstr xFontName, UINT xWidth, UINT xHeight, UINT xWeight, DWORD xFontColor, LPCWSTR xFaceName, BOOL xItalic = FALSE);
    void        Font_Free(RenderFont& xRenderFont);
    SIZE        Font_GetDrawTextRectA(RenderFont& xFont, std::string xStr, Int32 xStrLen = -1);
    Int32       Font_GetDrawMaxTextCountInWidthA(RenderFont& xFont, INT xWidth, std::string xStr);
    Int32       Font_GetDrawMaxTextCountInWidthDescA(RenderFont& xFont, INT xWidth, std::string xStr);
    SIZE        Font_GetDrawTextRectW(RenderFont& xFont, std::wstring xStr, Int32 xStrLen = -1);
    Int32       Font_GetDrawMaxTextCountInWidthW(RenderFont& xFont, INT xWidth, std::wstring xStr, MsList<UIResInfo>& xTempList, Int32 xRowIndex);
    Int32       Font_GetDrawMaxTextCountInWidthW(RenderFont& xFont, INT xWidth, std::wstring xStr, LONG& xRealWidth);
    Int32       Font_GetDrawMaxTextCountInWidthDescW(RenderFont& xFont, INT xWidth, std::wstring xStr);
    Int32       Font_DrawTextA(RenderFont& xFont, std::string xStr, LONG x1, LONG y1);
    Int32       Font_DrawTextW(RenderFont& xFont, std::wstring xStr, LONG x1, LONG y1);
    Int32       Font_DrawTextA(RenderFont& xFont, std::string xStr, LONG x1, LONG y1, LONG w, LONG h);
    Int32       Font_DrawTextW(RenderFont& xFont, std::wstring xStr, LONG x1, LONG y1, LONG w, LONG h);

    void    GoNextIME();
    void    GoPrevIME();
    //Boolean m_IsCtrl = False;
    //Boolean m_IsShift = False;
    //Boolean m_IsAlt = False;

    void    IME_StrDraw(RenderFont& xFont, LONG x, LONG y, LONG xCharH);
    void    IME_StrDraw(RenderFont& xFont, HGE_E_POS_TYPE xPosType, LONG dwOffsetX, LONG dwOffsetY, LONG xCharH);

    HDC     DC_Get() { return ::GetDC(m_StateHWND.m_hWindow); }

    Boolean BeginScene(DWORD color);
    void    EndScene();

    void    PushRenderTarget();
    void    PopRenderTarget();
    void    RenderTexture_Create();
    void    RenderTexture_Begin();
    void    RenderTexture_Clear(DWORD color);
    LPDWORD RenderTexture_End();
    void    RenderTexture_UnLock();

    Boolean Get_GDI_Render();
    void    Set_GDI_Render(Boolean Is_GDI_Draw);

private:
    ULONG_PTR           m_gdiplusToken;
    BITMAPINFO          m_BitmapInfo;
    Boolean             m_IsGDIDraw;
    //LPDIRECT3DTEXTURE9  m_RenderTexture;
    //PDIRECT3DSURFACE9   m_RenderSurface;
    PDIRECT3DSURFACE9   m_RenderBakBuff;

    LPDIRECT3DTEXTURE9  m_CanEdit_RenderTexture;
    PDIRECT3DSURFACE9   m_CanEdit_RenderSurface;

public:
    RenderManager_StateBoolean  m_StateBoolean;
    RenderManager_StateHWND     m_StateHWND;
    RenderManager_StateInt      m_StateInt;

    static D3DFORMAT            ms_Format;

    LRESULT HGE_WindowProc(HWND hWnd, UINT xMsg, WPARAM wParam, LPARAM lParam);
    //HWND    m_hParent;
    tstr    m_WndName;

    RenderFont GetRenderFont(mstr xFontName);
    MsVector<mstr> m_List_AdapterMode;
    //DWORD   m_FullScreenModeWidth = 0;
    //DWORD   m_FullScreenModeHeight = 0;
    mstr    m_IMEGuideLine;
    mstr    m_IMECharList[10];
    DWORD   m_IMECharCount;
    Boolean m_IsWndFocus;

protected:

    Boolean _GfxInit();
    void    _GfxDone();
    //void    _AdjustWindow();
    //void    _Resize(Int32 width, Int32 height);
    Boolean _init_lost();
    Int32   _format_id(D3DFORMAT xFormat);
    void    _render_batch(Boolean bEndScene = False);
    void    _SetBlendMode(Int32 xBlend);
    void    _SetProjectionMatrix(Int32 xWidth, Int32 xHeight);

    // Graphics
    D3DPRESENT_PARAMETERS*  m_D3dPresentParameters;
    D3DPRESENT_PARAMETERS   m_D3dPresentParametersWindowed;
    LONG                    m_WndStyle = 0;
    DWORD                   m_dwExStyle = 0;

    D3DPRESENT_PARAMETERS   m_D3dPresentParametersFullScreen;
    //RECT                    m_RectFullScreen;
    //LONG                    m_StyleFullScreen;

    IDirect3D9*             pD3D;
    IDirect3DDevice9*       pD3DDevice;
    IDirect3DVertexBuffer9* pVB;
    IDirect3DIndexBuffer9*  pIB;
    LPD3DXSPRITE            m_lpSprite;

    IDirect3DSurface9*      pScreenSurf;
    IDirect3DSurface9*      pScreenDepth;
    CRenderTargetList*      pTargets;
    CRenderTargetList*      pCurTarget;

    D3DXMATRIX              matView;
    D3DXMATRIX              matProj;

    CTextureList*           textures;
    RenderManagerVertex*    VertArray;

    Int32       nPrim;
    Int32       CurPrimType;
    Int32       CurBlendMode;
    HTEXTURE    CurTexture;
    HTEXTURE    CurShader;

    //CHAR    szError[256];
    //void    _PostError(CHAR *error);

    virtual Boolean Render() = 0;
    virtual Boolean LogicTick() = 0;

    void    _UpdateMouse();

    HINSTANCE   hInstance;
    CHAR        m_szAppPath[_MAX_PATH];
    CHAR        szIniString[256];

    Int32   Zpos;
    Single  Xpos;
    Single  Ypos;
    Boolean bCaptured;

    // Resources
    CHAR                szTmpFilename[_MAX_PATH];
    CResourceList*      m_Res;
    HANDLE              hSearch;
    WIN32_FIND_DATAA    SearchData;

    // Power
    Int32   nPowerStatus;
    HMODULE hKrnl32;
    GetSystemPowerStatusFunc lpfnGetSystemPowerStatus;

    RenderFont m_DefaultFont;
    MsUnorderedDictionary<mstr, RenderFont> m_DictFont;
protected:
    void _InitPowerStatus();
    void _UpdatePowerStatus();
    void _DonePowerStatus();

    // Audio
    Boolean         bSilent;
    CStreamList*    streams;

public:
    Boolean ResetSound();
    Boolean _SoundInit();
    void    _SoundDone();
    void    SetMusVolume(Int32 vol);
    void    SetStreamVolume(Int32 vol);
    void    SetFXVolume(Int32 vol);

protected:

};
//extern RenderManager* g_lpTempRenderManager;
#define LOWORDINT(n) ((Int32)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((Int32)((signed short)(HIWORD(n))))
