/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: graphics
*/

#include "hge.h"
#include <time.h>

//const WCHAR* WINDOW_CLASS_NAME = L"MS_HGE_WNDCLASS";
#pragma warning(disable:4996)
#pragma warning(disable:4311)
#pragma warning(disable:4302)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         ((p)?((p)->Release(),(p)=nullptr):((p) = (p)))
#endif

void CALL HGE::System_SetStateBool(hgeBoolState state, bool value)
{
    switch (state)
    {
        case HGE_WINDOWED:
        {
            if (m_hWindow) break;
            m_bWindowed = value;
        } break;

        case HGE_ZBUFFER:
        {
            if (m_hWindow) break;
            m_bZBuffer = value;
        } break;

        case HGE_TEXTUREFILTER:
        {
            if (m_hWindow) break;
            m_bTextureFilter = value;
        } break;

        case HGE_USESOUND:
        {
            if (m_bUseSound != value)
            {
                m_bUseSound = value;

                // init sound plugin here if required
                if (m_bUseSound && m_hWindow) m_bUseSound = true; //_SoundInit();
                if (!m_bUseSound && m_hWindow) m_bUseSound = false; //_SoundDone();
            }
        } break;

        case HGE_HIDEMOUSE:
        {
            m_bHideMouse = value;
        } break;

        case HGE_DONTSUSPEND:
        {
            m_bDontSuspend = value;
        } break;

        case HGE_SHOWSPLASH:
        {
            m_bShowSplash = value;
        } break;
    }
}

void CALL HGE::System_SetStateFunc(hgeFuncState state, hgeCallback value)
{
    switch (state)
    {
        case HGE_FRAMEFUNC:		 m_FunctionFrame = value;      break;
        case HGE_RENDERFUNC:	 m_FunctionRender = value;     break;
        case HGE_FOCUSLOSTFUNC:	 m_FunctionFocusLost = value;  break;
        case HGE_FOCUSGAINFUNC:	 m_FunctionFocusGain = value;  break;
        case HGE_GFXRESTOREFUNC: m_FunctionGfxRestore = value; break;
        case HGE_EXITFUNC:		 m_FunctionExit = value;       break;
    }
}

void CALL HGE::System_SetStateHwnd(hgeHwndState state, HWINDOW value)
{
    switch (state)
    {
        case HGE_HWND:		 m_hWindow = value;       break;
        case HGE_HWNDPARENT: m_hWindowParent = value; break;
    }
}

void CALL HGE::System_SetStateInt(hgeIntState state, int value)
{
    switch (state)
    {
        case HGE_SCREENX:       if (!m_hWindow) m_nScreenX = value;         break;
        case HGE_SCREENY:       if (!m_hWindow) m_nScreenY = value;         break;
        case HGE_SCREENWIDTH:   if (!m_hWindow) m_nScreenWidth = value;     break;
        case HGE_SCREENHEIGHT:  if (!m_hWindow) m_nScreenHeight = value;    break;
        case HGE_SCREENBPP:     if (!m_hWindow) m_nScreenBPP = value;       break;
        case HGE_SAMPLERATE:    if (!m_hWindow) m_nSampleRate = value;      break; // hBass
        case HGE_FXVOLUME:      m_nFXVolume = value; /*_SetFXVolume(nFXVolume);*/ break;
        case HGE_MUSVOLUME:     m_nMusVolume = value; /*_SetMusVolume(nMusVolume);*/ break;
        case HGE_STREAMVOLUME:  m_nStreamVolume = value; /*_SetStreamVolume(nStreamVolume);*/ break;

        case HGE_FPS:
        {
            if (m_nHGEFPS != value)
            {
                if (m_hWindow)
                {
                    //Gfx_Sync(
                }
                m_nHGEFPS = value;
            }
        } break;
    }
}

void CALL HGE::System_SetStateString(hgeWStringState state, const WCHAR* value)
{
    switch (state)
    {
        case HGE_ICON:
        {
            m_lpszIcon = value;
        } break;

        case HGE_TITLE:
        {
            if (value)
            {
                wcscpy(m_szWinTitle, value);
            }
            else
            {
                wcscpy(m_szWinTitle, L"HGE");
            }
        } break;
    }
}

void CALL HGE::System_SetStateString(hgeStringState state, const char* value)
{
    FILE *hf;
    switch (state)
    {
        case HGE_INIFILE:
        {
            if (value)
                strcpy(m_szIniFile, Resource_MakePath(value));
            else
                m_szIniFile[0] = 0;
        } break;

        case HGE_LOGFILE:
        {
            if (value)
            {
                strcpy(m_szLogFile, Resource_MakePath(value));
                hf = fopen(m_szLogFile, "w");

                if (!hf)
                    m_szLogFile[0] = 0;
                else
                    fclose(hf);
            }
            else
            {
                m_szLogFile[0] = 0;
            }
        } break;
    }
}

bool CALL HGE::System_GetStateBool(hgeBoolState state)
{
    switch (state)
    {
        case HGE_WINDOWED:      return m_bWindowed;
        case HGE_ZBUFFER:       return m_bZBuffer;
        case HGE_TEXTUREFILTER: return m_bTextureFilter;
        case HGE_USESOUND:      return m_bUseSound;
        case HGE_DONTSUSPEND:   return m_bDontSuspend;
        case HGE_HIDEMOUSE:     return m_bHideMouse;
        case HGE_SHOWSPLASH:    return m_bShowSplash;
    }

    return false;
}

hgeCallback CALL HGE::System_GetStateFunc(hgeFuncState state)
{
    hgeCallback hcbReturnValue = NULL;

    switch (state)
    {
        case HGE_FRAMEFUNC:         hcbReturnValue = m_FunctionFrame;      break;
        case HGE_RENDERFUNC:        hcbReturnValue = m_FunctionRender;     break;
        case HGE_FOCUSLOSTFUNC:     hcbReturnValue = m_FunctionFocusLost;  break;
        case HGE_FOCUSGAINFUNC:     hcbReturnValue = m_FunctionFocusGain;  break;
        case HGE_GFXRESTOREFUNC:    hcbReturnValue = m_FunctionGfxRestore; break;
        case HGE_EXITFUNC:          hcbReturnValue = m_FunctionExit;       break;
    }

    return hcbReturnValue;
}

HWINDOW CALL HGE::System_GetStateHwnd(hgeHwndState state)
{
    switch (state)
    {
        case HGE_HWND:          return m_hWindow;
        case HGE_HWNDPARENT:    return m_hWindowParent;
    }

    return 0;
}

int	 CALL HGE::System_GetStateInt(hgeIntState state)
{
    switch (state)
    {
        case HGE_SCREENX:       return m_nScreenX;
        case HGE_SCREENY:       return m_nScreenY;
        case HGE_SCREENWIDTH:   return m_nScreenWidth;
        case HGE_SCREENHEIGHT:  return m_nScreenHeight;
        case HGE_SCREENBPP:     return m_nScreenBPP;
        case HGE_SAMPLERATE:    return m_nSampleRate;
        case HGE_FXVOLUME:      return m_nFXVolume;
        case HGE_MUSVOLUME:     return m_nMusVolume;
        case HGE_STREAMVOLUME:  return m_nStreamVolume;
        case HGE_FPS:           return m_nHGEFPS;
        case HGE_POWERSTATUS:   return m_nPowerStatus;
    }

    return 0;
}

const WCHAR* CALL HGE::System_GetStateString(hgeWStringState state)
{
    switch (state)
    {
        case HGE_ICON: return m_lpszIcon;
        case HGE_TITLE: return m_szWinTitle;
    }
    return NULL;
}

const char* CALL HGE::System_GetStateString(hgeStringState state)
{
    switch (state)
    {
        //case HGE_ICON:			return m_lpszIcon;

        case HGE_INIFILE:
        {
            if (m_szIniFile[0]) return m_szIniFile;
        } break;

        case HGE_LOGFILE:
        {
            if (m_szLogFile[0]) return m_szLogFile;
        } break;
    }

    return NULL;
}

HGE::HGE()
    : pD3D(nullptr)
    , pD3DDevice(nullptr)
    , d3dpp(nullptr)
    , pVB(nullptr)
    , pIB(nullptr)

    , pTargets(nullptr)
    , pCurTarget(nullptr)
    , pScreenSurf(nullptr)
    , pScreenDepth(nullptr)
    , pSprite(nullptr)
    , VertArray(nullptr)
    , textures(nullptr)
    , m_hCursor(nullptr)
    , m_RenderTexture(nullptr)
    , m_RenderSurface(nullptr)
    , m_CanEdit_RenderTexture(nullptr)
    , m_CanEdit_RenderSurface(nullptr)
    , m_Is_GDI_Draw(false)
{
    bSilent = false;
    streams = 0;

    hInstance = GetModuleHandle(0);

    this->System_SetState(HGE_HWND, 0);
    this->System_SetState(HGE_HWNDPARENT, 0);

    bActive = false;
    szError[0] = 0;

    hSearch = 0;
    res = 0;

    IMEChar.clear();
    CharKey.clear();

    queue = 0;
    Char = VKey = ClickKey = Zpos = 0;
    Xpos = Ypos = 0.0f;
    bMouseOver = false;
    bCaptured = false;

    this->System_SetState(HGE_FPS, HGEFPS_UNLIMITED);

    fTime = 0.0f;
    fDeltaTime = 0.0f;
    nFPS = 0;

    this->System_SetState(HGE_FRAMEFUNC, 0);
    this->System_SetState(HGE_RENDERFUNC, 0);
    this->System_SetState(HGE_FOCUSLOSTFUNC, 0);
    this->System_SetState(HGE_FOCUSGAINFUNC, 0);
    this->System_SetState(HGE_GFXRESTOREFUNC, 0);
    this->System_SetState(HGE_EXITFUNC, 0);

    this->System_SetState(HGE_ICON, 0);
    this->System_SetState(HGE_TITLE, L"HGE");

    this->System_SetState(HGE_SCREENWIDTH, 1024);
    this->System_SetState(HGE_SCREENHEIGHT, 768);
    this->System_SetState(HGE_SCREENBPP, 32);

    this->System_SetState(HGE_WINDOWED, false);
    this->System_SetState(HGE_ZBUFFER, false);
    this->System_SetState(HGE_TEXTUREFILTER, true);

    this->System_SetState(HGE_USESOUND, true);
    this->System_SetState(HGE_SAMPLERATE, 44100);
    this->System_SetState(HGE_FXVOLUME, 100);
    this->System_SetState(HGE_MUSVOLUME, 100);
    this->System_SetState(HGE_STREAMVOLUME, 100);

    this->System_SetState(HGE_DONTSUSPEND, false);
    this->System_SetState(HGE_HIDEMOUSE, true);

    this->System_SetState(HGE_SHOWSPLASH, true);

    this->System_SetState(HGE_LOGFILE, 0);
    this->System_SetState(HGE_INIFILE, 0);

    nFixedDelta = 0;

    nPowerStatus = HGEPWR_UNSUPPORTED;
    hKrnl32 = NULL;
    lpfnGetSystemPowerStatus = NULL;

#ifdef DEMO
    bDMO = true;
#endif

    GetModuleFileNameA(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));

    int i;
    for (i = (int)strlen(szAppPath) - 1; i > 0; i--) if (szAppPath[i] == '\\') break;

    szAppPath[i + 1] = 0;
}

SIZE TryCreateWindow(SIZE xNeedSize, DWORD dwStyle, WNDCLASSW xWinclass)
{
    xWinclass.lpszClassName = L"_MS_HGE_WNDCLASS";
    xWinclass.lpfnWndProc = _WindowProc;
    ::RegisterClassW(&xWinclass);

    dwStyle &= ~WS_VISIBLE;
    HWND hwnd = CreateWindowExW(0, L"_MS_HGE_WNDCLASS", L"", dwStyle,
        0, 0, xNeedSize.cx, xNeedSize.cy,
        nullptr, nullptr, nullptr, nullptr);
    RECT r;
    ::GetClientRect(hwnd, &r);
    ::CloseWindow(hwnd);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(xWinclass.lpszClassName, xWinclass.hInstance);
    return{ xNeedSize.cx - (r.right - r.left), xNeedSize.cy - (r.bottom - r.top) };
}

bool CALL HGE::System_Initiate(LPCTSTR szWndName, ON_GFX_LOST xOnGfxLost)
{
    g_temp_hge = this;

    m_lpOnGfxLostFun = xOnGfxLost;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    m_hParent = nullptr;
    _SoundInit();

    uIMECharCount = 0;

    if (this->System_GetState(HGE_HWND)) return true;

    HWND            hwnd;
    OSVERSIONINFO   os_ver;
    SYSTEMTIME      tm;
    MEMORYSTATUS    mem_st;
    WNDCLASSW       winclass;
    int             width, height;

    // Log system info

    System_Log("HGE Started..\n");

    System_Log("HGE version: %X.%X", HGE_VERSION >> 8, HGE_VERSION & 0xFF);
    GetLocalTime(&tm);
    System_Log("Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond);

    System_Log("Application: %s", this->System_GetState(HGE_TITLE));
    os_ver.dwOSVersionInfoSize = sizeof(os_ver);
    GetVersionEx(&os_ver);
    System_Log("OS: Windows %ld.%ld.%ld", os_ver.dwMajorVersion, os_ver.dwMinorVersion, os_ver.dwBuildNumber);

    GlobalMemoryStatus(&mem_st);
    System_Log("Memory: %ldK total, %ldK free\n", mem_st.dwTotalPhys / 1024L, mem_st.dwAvailPhys / 1024L);


    // Register window class

    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hInstance;
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    //winclass.lpszClassName = WINDOW_CLASS_NAME;
    winclass.lpszClassName = szWndName;
    _tcscpy(m_szWndName, szWndName);

    if (this->System_GetState(HGE_ICON))
    {
        winclass.hIcon = ::LoadIcon(hInstance, this->System_GetState(HGE_ICON));
    }
    else
    {
        winclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    }

    if (!RegisterClassW(&winclass)) {
        _PostError("Can't register window class");
        return false;
    }

    // Create window
    //width = this->System_GetState(HGE_SCREENWIDTH) + (GetSystemMetrics(SM_CXSIZEFRAME) + 4) * 2;
    //height = this->System_GetState(HGE_SCREENHEIGHT) + (GetSystemMetrics(SM_CYSIZEFRAME) + 4) * 2 + GetSystemMetrics(SM_CYCAPTION);
    width = this->System_GetState(HGE_SCREENWIDTH);
    height = this->System_GetState(HGE_SCREENHEIGHT);
    //styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
    styleW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

    SIZE xSize = TryCreateWindow({ this->System_GetState(HGE_SCREENWIDTH),this->System_GetState(HGE_SCREENHEIGHT) }, styleW, winclass);
    width += xSize.cx;
    height += xSize.cy;

    if ((m_nScreenX + width) > GetSystemMetrics(SM_CXSCREEN)) { m_nScreenX = GetSystemMetrics(SM_CXSCREEN) - width; }
    if (m_nScreenX < 0) { m_nScreenX = 0; }

    if ((m_nScreenY + height) > GetSystemMetrics(SM_CYSCREEN)) { m_nScreenY = GetSystemMetrics(SM_CYSCREEN) - height - 105; }
    if (m_nScreenY < 0) { m_nScreenY = 0; }

    rectW.left = m_nScreenX;
    rectW.top = m_nScreenY;
    rectW.right = rectW.left + width;
    rectW.bottom = rectW.top + height;


    rectFS.left = m_nScreenX;
    rectFS.top = m_nScreenY;
    rectFS.right = width;
    rectFS.bottom = height;
    styleFS = WS_VISIBLE; //WS_POPUP
    //styleFS = WS_POPUP | WS_VISIBLE; //WS_POPUP

    if (this->System_GetState(HGE_HWNDPARENT))
    {
        rectW.left = m_nScreenX;
        rectW.top = m_nScreenY;
        rectW.right = width;
        rectW.bottom = height;
        styleW = WS_CHILD | WS_VISIBLE;

        this->System_SetState(HGE_WINDOWED, true);
    }

    //hwnd = hWnd;
    //CSharpWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_WNDPROC);
    //::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WindowProc);

    if (this->System_GetState(HGE_WINDOWED))
    {
        hwnd = CreateWindowExW(0, m_szWndName, this->System_GetState(HGE_TITLE), styleW,
            rectW.left, rectW.top, rectW.right - rectW.left, rectW.bottom - rectW.top,
            (HWND)this->System_GetState(HGE_HWNDPARENT), NULL, hInstance, NULL);
    }
    else
    {
        hwnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, m_szWndName, this->System_GetState(HGE_TITLE), styleFS,
            0, 0, 0, 0,
            NULL, NULL, hInstance, NULL);
    }

    if (!hwnd)
    {
        _PostError("Can't create window");
        return false;
    }

    this->System_SetState(HGE_HWND, (HWINDOW)hwnd);
    ms_map[hwnd] = g_temp_hge;
    g_temp_hge = nullptr;

    ShowWindow(hwnd, SW_SHOW);

    // Init subsystems

    timeBeginPeriod(1);
    Random_Seed();
    _InitPowerStatus();
    _InputInit();

    System_Log("Init done.\n");

    fTime = 0.0f;
    t0 = t0fps = timeGetTime();
    dt = cfps = 0;
    nFPS = 0;

    // Show splash

#ifdef DEMO

    bool(*func)();
    bool(*rfunc)();
    HWND			hwndTmp;

    if (g_lpHGE->bDMO)
    {
        Sleep(200);
        func = (bool(*)())g_lpHGE->System_GetStateFunc(HGE_FRAMEFUNC);
        rfunc = (bool(*)())g_lpHGE->System_GetStateFunc(HGE_RENDERFUNC);
        hwndTmp = hwndParent; hwndParent = 0;
        g_lpHGE->System_SetStateFunc(HGE_FRAMEFUNC, DFrame);
        g_lpHGE->System_SetStateFunc(HGE_RENDERFUNC, 0);
        DInit();
        g_lpHGE->System_Start();
        DDone();
        hwndParent = hwndTmp;
        g_lpHGE->System_SetStateFunc(HGE_FRAMEFUNC, func);
        g_lpHGE->System_SetStateFunc(HGE_RENDERFUNC, rfunc);
}

#endif

    // Done



    if (System_GetState(HGE_HWNDPARENT))
    {
        rectW.left = m_nScreenX;
        rectW.top = m_nScreenY;
        rectW.right = width;
        rectW.bottom = height;
        styleW = WS_CHILD | WS_VISIBLE;
    }
    else
    {
        rectW.left = m_nScreenX;
        rectW.top = m_nScreenY;
        rectW.right = rectW.left + width;
        rectW.bottom = rectW.top + height;
        //styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
        styleW = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
    }

    rectFS.left = m_nScreenX;
    rectFS.top = m_nScreenY;
    rectFS.right = width;
    rectFS.bottom = height;
    //styleFS = WS_POPUP | WS_VISIBLE;
    styleFS = WS_VISIBLE;

    memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
    m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_BitmapInfo.bmiHeader.biWidth = m_nScreenWidth;
    m_BitmapInfo.bmiHeader.biHeight = -(int)m_nScreenHeight;
    m_BitmapInfo.bmiHeader.biPlanes = 1;
    m_BitmapInfo.bmiHeader.biBitCount = 32;
    m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
    m_BitmapInfo.bmiHeader.biSizeImage = m_nScreenWidth * m_nScreenHeight * sizeof(DWORD);

    return _GfxInit();
}

void CALL HGE::Gfx_Clear(DWORD color)
{
    if (pCurTarget)
    {
        if (pCurTarget->pDepth)
            pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
        else
            pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
    }
    else
    {
        if (System_GetState(HGE_ZBUFFER))
            pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
        else
            pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
    }
}

void CALL HGE::Gfx_SetClipping(int x, int y, int w, int h)
{
    D3DVIEWPORT9 vp;
    int scr_width, scr_height;

    if (!pCurTarget)
    {
        scr_width = System_GetState(HGE_SCREENWIDTH);
        scr_height = System_GetState(HGE_SCREENHEIGHT);
    }
    else
    {
        scr_width = Texture_GetWidth((HTEXTURE)pCurTarget->pTex);
        scr_height = Texture_GetHeight((HTEXTURE)pCurTarget->pTex);
    }

    if (!w)
    {
        vp.X = 0;
        vp.Y = 0;
        vp.Width = scr_width;
        vp.Height = scr_height;
    }
    else
    {
        if (x < 0) { w += x; x = 0; }
        if (y < 0) { h += y; y = 0; }

        if (x + w > scr_width) w = scr_width - x;
        if (y + h > scr_height) h = scr_height - y;

        vp.X = x;
        vp.Y = y;
        vp.Width = w;
        vp.Height = h;
    }

    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;

    _render_batch();
    pD3DDevice->SetViewport(&vp);

    D3DXMATRIX tmp;
    D3DXMatrixScaling(&matProj, 1.0f, -1.0f, 1.0f);
    D3DXMatrixTranslation(&tmp, -0.5f, +0.5f, 0.0f);
    D3DXMatrixMultiply(&matProj, &matProj, &tmp);
    D3DXMatrixOrthoOffCenterLH(&tmp, (float)vp.X, (float)(vp.X + vp.Width), -((float)(vp.Y + vp.Height)), -((float)vp.Y), vp.MinZ, vp.MaxZ);
    D3DXMatrixMultiply(&matProj, &matProj, &tmp);
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CALL HGE::Gfx_SetTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale)
{
    D3DXMATRIX tmp;

    if (vscale == 0.0f)
    {
        D3DXMatrixIdentity(&matView);
    }
    else
    {
        D3DXMatrixTranslation(&matView, -x, -y, 0.0f);
        D3DXMatrixScaling(&tmp, hscale, vscale, 1.0f);
        D3DXMatrixMultiply(&matView, &matView, &tmp);
        D3DXMatrixRotationZ(&tmp, -rot);
        D3DXMatrixMultiply(&matView, &matView, &tmp);
        D3DXMatrixTranslation(&tmp, x + dx, y + dy, 0.0f);
        D3DXMatrixMultiply(&matView, &matView, &tmp);
    }

    _render_batch();
    pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

bool CALL HGE::Gfx_BeginScene(HTARGET targ)
{
    CRenderTargetList *target = (CRenderTargetList *)(ULONG_PTR)targ;
    LPDIRECT3DSURFACE9 pSurf = 0, pDepth = 0;
    D3DDISPLAYMODE Mode;

    HRESULT hr = pD3DDevice->TestCooperativeLevel();

    if (hr == D3DERR_DEVICELOST)
    {
        return false;
    }
    else if (hr == D3DERR_DEVICENOTRESET)
    {
        if (System_GetState(HGE_WINDOWED))
        {
            if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode)) || Mode.Format == D3DFMT_UNKNOWN)
            {
                _PostError("Can't determine desktop video mode");
                return false;
            }

            d3dppW.BackBufferFormat = Mode.Format;

            if (_format_id(Mode.Format) < 4)
            {
                System_SetState(HGE_SCREENBPP, 16);
            }
            else
            {
                System_SetState(HGE_SCREENBPP, 32);
            }
        }

        if (!_GfxRestore()) return false;
    }

    if (VertArray)
    {
        _PostError("Gfx_BeginScene: Scene is already being rendered");
        return false;
    }

    if (target != pCurTarget)
    {
        if (target)
        {
            target->pTex->GetSurfaceLevel(0, &pSurf);
            pDepth = target->pDepth;
        }
        else
        {
            pSurf = pScreenSurf;
            pDepth = pScreenDepth;
        }

        if (FAILED(pD3DDevice->SetRenderTarget(0, pSurf)))
        {
            if (target) pSurf->Release();
            _PostError("Gfx_BeginScene: Can't set render target");
            return false;
        }

        if (target)
        {
            pSurf->Release();

            if (target->pDepth)
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            }
            else
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
            }

            _SetProjectionMatrix(target->width, target->height);
        }
        else
        {
            if (System_GetState(HGE_ZBUFFER))
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            }
            else
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
            }

            _SetProjectionMatrix(System_GetState(HGE_SCREENWIDTH), System_GetState(HGE_SCREENHEIGHT));
        }

        pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
        D3DXMatrixIdentity(&matView);
        pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

        pCurTarget = target;
    }

    pD3DDevice->BeginScene();
    pVB->Lock(0, 0, (VOID**)&VertArray, 0);

    pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);

    return true;
}

HSHADER CALL HGE::Shader_Create(const char* filename, const char* lpszFunction, const char* lpszProfile)
{
    LPD3DXBUFFER					code = NULL;
    LPDIRECT3DPIXELSHADER9          pixelShader = NULL;
    HRESULT result = D3DXCompileShaderFromFileA(Resource_MakePath(filename),   //filepath
        NULL,          //macro's            
        NULL,          //includes           
        lpszFunction,  //main function      
        lpszProfile,   //shader profile     
        0,             //flags              
        &code,         //compiled operations
        NULL,          //errors
        NULL);         //constants

    if (FAILED(result)) {
        _PostError("Can't create shader");
        return NULL;
    }

    pD3DDevice->CreatePixelShader((DWORD*)code->GetBufferPointer(), &pixelShader);
    code->Release();
    return (HSHADER)pixelShader;
}

void CALL HGE::Gfx_SetShader(HSHADER shader)
{
    if (CurShader != shader)
    {
        _render_batch();
        CurShader = shader;
        pD3DDevice->SetPixelShader((LPDIRECT3DPIXELSHADER9)(ULONG_PTR)shader);
    }
}

void CALL HGE::Shader_Free(HSHADER shader)
{
    if (shader) ((LPDIRECT3DPIXELSHADER9)(ULONG_PTR)shader)->Release();
}

void CALL HGE::Gfx_EndScene()
{
    pSprite->End();

    _render_batch(true);

    pD3DDevice->EndScene();
    if (!pCurTarget) pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CALL HGE::Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color, float z)
{
    if (VertArray)
    {
        if (CurPrimType != HGEPRIM_LINES || nPrim >= VERTEX_BUFFER_SIZE / HGEPRIM_LINES || CurTexture || CurBlendMode != BLEND_DEFAULT)
        {
            _render_batch();
            CurPrimType = HGEPRIM_LINES;

            if (CurBlendMode != BLEND_DEFAULT) _SetBlendMode(BLEND_DEFAULT);

            if (CurTexture)
            {
                pD3DDevice->SetTexture(0, 0);
                CurTexture = 0;
            }
        }

        int i = nPrim * HGEPRIM_LINES;

        VertArray[i].x = x1; VertArray[i + 1].x = x2;
        VertArray[i].y = y1; VertArray[i + 1].y = y2;
        VertArray[i].z = VertArray[i + 1].z = z;
        VertArray[i].col = VertArray[i + 1].col = color;
        VertArray[i].tx = VertArray[i + 1].tx = 0.0f;
        VertArray[i].ty = VertArray[i + 1].ty = 0.0f;

        nPrim++;
    }
}

void CALL HGE::Gfx_RenderTriple(const hgeTriple *triple)
{
    if (VertArray)
    {
        if (CurPrimType != HGEPRIM_TRIPLES || nPrim >= VERTEX_BUFFER_SIZE / HGEPRIM_TRIPLES || CurTexture != triple->tex || CurBlendMode != triple->blend)
        {
            _render_batch();
            CurPrimType = HGEPRIM_TRIPLES;

            if (CurBlendMode != triple->blend) _SetBlendMode(triple->blend);

            if (triple->tex != CurTexture)
            {
                pD3DDevice->SetTexture(0, (LPDIRECT3DTEXTURE9)(ULONG_PTR)triple->tex);
                CurTexture = triple->tex;
            }
        }

        memcpy(&VertArray[nPrim*HGEPRIM_TRIPLES], triple->v, sizeof(hgeVertex) * HGEPRIM_TRIPLES);
        nPrim++;
    }
}

void CALL HGE::Gfx_RenderQuad(const hgeQuad *quad)
{
    if (VertArray)
    {
        if (CurPrimType != HGEPRIM_QUADS || nPrim >= VERTEX_BUFFER_SIZE / HGEPRIM_QUADS || CurTexture != quad->tex || CurBlendMode != quad->blend)
        {
            _render_batch();
            CurPrimType = HGEPRIM_QUADS;

            if (CurBlendMode != quad->blend)
            {
                _SetBlendMode(quad->blend);
            }

            if (quad->tex != CurTexture)
            {
                pD3DDevice->SetTexture(0, (LPDIRECT3DTEXTURE9)(ULONG_PTR)quad->tex);
                CurTexture = quad->tex;
            }
        }

        memcpy(&VertArray[nPrim * HGEPRIM_QUADS], quad->v, sizeof(hgeVertex) * HGEPRIM_QUADS);
        nPrim++;
    }
}

hgeVertex* CALL HGE::Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim)
{
    if (VertArray)
    {
        _render_batch();
        CurPrimType = prim_type;

        if (CurBlendMode != blend) _SetBlendMode(blend);

        if (tex != CurTexture)
        {
            pD3DDevice->SetTexture(0, (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex);
            CurTexture = tex;
        }

        *max_prim = VERTEX_BUFFER_SIZE / prim_type;
        return VertArray;
    }
    else
    {
        return 0;
    }
}

void CALL HGE::Gfx_FinishBatch(int nprim)
{
    nPrim = nprim;
}

HTARGET CALL HGE::Target_Create(int width, int height, bool zbuffer)
{
    CRenderTargetList *pTarget;

    pTarget = new CRenderTargetList;
    pTarget->pTex = 0;
    pTarget->pDepth = 0;

    D3DSURFACE_DESC TDesc;

    if (FAILED(D3DXCreateTexture(pD3DDevice, width, height, 1, D3DUSAGE_RENDERTARGET,
        d3dpp->BackBufferFormat, D3DPOOL_DEFAULT, &pTarget->pTex)))
    {
        _PostError("Can't create render target texture");
        delete pTarget;
        return 0;
    }

    pTarget->pTex->GetLevelDesc(0, &TDesc);
    pTarget->width = TDesc.Width;
    pTarget->height = TDesc.Height;

    if (zbuffer)
    {
        if (FAILED(pD3DDevice->CreateDepthStencilSurface(width, height,
            D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, false, &pTarget->pDepth, NULL)))
        {
            pTarget->pTex->Release();
            _PostError("Can't create render target depth buffer");
            delete pTarget;
            return 0;
        }
    }

    pTarget->next = pTargets;
    pTargets = pTarget;

    return (HTARGET)pTarget;
}

void CALL HGE::Target_Free(HTARGET target)
{
    CRenderTargetList *pTarget = pTargets, *pPrevTarget = NULL;

    while (pTarget)
    {
        if ((CRenderTargetList*)(ULONG_PTR)target == pTarget)
        {
            if (pPrevTarget)
                pPrevTarget->next = pTarget->next;
            else
                pTargets = pTarget->next;

            if (pTarget->pTex) pTarget->pTex->Release();
            if (pTarget->pDepth) pTarget->pDepth->Release();

            delete pTarget;
            return;
        }

        pPrevTarget = pTarget;
        pTarget = pTarget->next;
    }
}

HTEXTURE CALL HGE::Target_GetTexture(HTARGET target)
{
    CRenderTargetList *targ = (CRenderTargetList*)(ULONG_PTR)target;
    if (target) return (HTEXTURE)targ->pTex;
    else return 0;
}


bool CALL HGE::BeginScene(DWORD color)
{
    bool ret = false;
    if (m_Is_GDI_Draw)
    {
        this->RenderTexture_Begin();
        this->RenderTexture_Clear(color);
        ret = true;
    }
    else
    {
        ret = this->Gfx_BeginScene();
        this->Gfx_Clear(color);
    }
    return ret;
}

void CALL HGE::EndScene()
{
    if (m_Is_GDI_Draw)
    {
        ::SetDIBitsToDevice(::GetDC((HWND)m_hWindow), 0, 0, m_nScreenWidth, m_nScreenHeight, 0, 0, 0, m_nScreenHeight, this->RenderTexture_End(), &m_BitmapInfo, DIB_RGB_COLORS);
        this->RenderTexture_UnLock();
    }
    else
    {
        this->Gfx_EndScene();
    }
}

void CALL HGE::PushRenderTarget()
{
    this->RenderTexture_Create();

    pD3DDevice->GetRenderTarget(0, &m_RenderBakBuff);
    ((LPDIRECT3DTEXTURE9)m_RenderTexture)->GetSurfaceLevel(0, &m_RenderSurface);
    ((LPDIRECT3DTEXTURE9)m_CanEdit_RenderTexture)->GetSurfaceLevel(0, &m_CanEdit_RenderSurface);
    pD3DDevice->SetRenderTarget(0, m_RenderSurface);

}

void CALL HGE::PopRenderTarget()
{
    pD3DDevice->SetRenderTarget(0, m_RenderBakBuff);
}

void CALL HGE::RenderTexture_Create()
{
    if (!m_RenderTexture && !m_CanEdit_RenderTexture)
    {
        if (FAILED(D3DXCreateTexture(pD3DDevice, m_nScreenWidth, m_nScreenHeight,
            1,                      // Mip levels
            D3DUSAGE_RENDERTARGET,  // Usage
            D3DFMT_A8R8G8B8,        // Format
            D3DPOOL_DEFAULT,        // Memory pool
            &m_RenderTexture)))
        {
            _PostError("Can't create texture");
            return;
        }

        if (FAILED(D3DXCreateTexture(pD3DDevice, m_nScreenWidth, m_nScreenHeight,
            1,                      // Mip levels
            0,                      // Usage
            D3DFMT_A8R8G8B8,        // Format
            D3DPOOL_SYSTEMMEM,      // Memory pool
            &m_CanEdit_RenderTexture)))
        {
            _PostError("Can't create texture");
            return;
        }
    }
}

void CALL HGE::RenderTexture_Begin()
{
    if (m_RenderTexture)
    {
        pD3DDevice->BeginScene();
        pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
    }
}

void CALL HGE::RenderTexture_Clear(DWORD color)
{
    pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0);
}

LPDWORD CALL HGE::RenderTexture_End()
{
    pSprite->End();
    pD3DDevice->EndScene();
    ((LPDIRECT3DTEXTURE9)m_CanEdit_RenderTexture)->GetSurfaceLevel(0, &m_CanEdit_RenderSurface);
    if (pD3DDevice->GetRenderTargetData(m_RenderSurface, m_CanEdit_RenderSurface) == D3D_OK)
    {
        D3DLOCKED_RECT lockbits;
        if (m_CanEdit_RenderSurface->LockRect(&lockbits, nullptr, D3DLOCK_READONLY) == D3D_OK)
        {
            return (LPDWORD)lockbits.pBits;
        }
    }
    return nullptr;
}

void CALL HGE::RenderTexture_UnLock()
{
    m_CanEdit_RenderSurface->UnlockRect();
    SAFE_RELEASE(m_CanEdit_RenderSurface);
}

bool HGE::Get_GDI_Render()
{
    return m_Is_GDI_Draw;
}

void HGE::Set_GDI_Render(bool Is_GDI_Draw)
{
    if (m_Is_GDI_Draw && !Is_GDI_Draw)
    {
        this->PopRenderTarget();
    }

    if (!m_Is_GDI_Draw && Is_GDI_Draw)
    {
        this->PushRenderTarget();
    }
    m_Is_GDI_Draw = Is_GDI_Draw;
}

HTEXTURE CALL HGE::Texture_Create(int width, int height)
{
    LPDIRECT3DTEXTURE9 pTex;
    CTextureList *texItem;

    if (FAILED(D3DXCreateTexture(pD3DDevice, width, height,
        1,                  // Mip levels
        0,                  // Usage
        D3DFMT_A8R8G8B8,    // Format
        D3DPOOL_MANAGED,    // Memory pool
        &pTex)))
    {
        _PostError("Can't create texture");
        return NULL;
    }

    texItem = new CTextureList;
    texItem->tex = (HTEXTURE)pTex;
    texItem->width = width;
    texItem->height = height;
    texItem->next = textures;
    textures = texItem;

    return (HTEXTURE)pTex;
}

HTEXTURE CALL HGE::Texture_LoadFromFile(const char* filename, DWORD size, bool bMipmap)
{
    int width, height;
    void *data;
    DWORD _size;
    CTextureList *texItem;

    if (size)
    {
        data = (void *)filename; _size = size;
    }
    else
    {
        data = Resource_Load(filename, &_size);
        if (!data) return NULL;
    }

    D3DFORMAT fmt1, fmt2;
    LPDIRECT3DTEXTURE9 pTex;
    D3DXIMAGE_INFO info;

    if (*(DWORD*)data == 0x20534444) // Compressed DDS format magic number
    {
        fmt1 = D3DFMT_UNKNOWN;
        fmt2 = D3DFMT_A8R8G8B8;
    }
    else
    {
        fmt1 = D3DFMT_A8R8G8B8;
        fmt2 = D3DFMT_UNKNOWN;
    }

    if (FAILED(D3DXCreateTextureFromFileInMemoryEx(pD3DDevice, data, _size,
        D3DX_DEFAULT, D3DX_DEFAULT,
        bMipmap ? 0 : 1,    // Mip levels
        0,                  // Usage
        fmt1,               // Format
        D3DPOOL_MANAGED,    // Memory pool
        D3DX_FILTER_NONE,   // Filter
        D3DX_DEFAULT,       // Mip filter
        0,                  // Color key
        &info, NULL,
        &pTex)))
    {
        if (FAILED(D3DXCreateTextureFromFileInMemoryEx(pD3DDevice, data, _size,
            D3DX_DEFAULT, D3DX_DEFAULT,
            bMipmap ? 0 : 1,    // Mip levels
            0,                  // Usage
            fmt2,               // Format
            D3DPOOL_MANAGED,    // Memory pool
            D3DX_FILTER_NONE,   // Filter
            D3DX_DEFAULT,       // Mip filter
            0,                  // Color key
            &info, NULL,
            &pTex)))
        {
            _PostError("Can't create texture");
            if (!size) Resource_Free(data);
            return NULL;
        }
    }

    width = info.Width;
    height = info.Height;

    if (!size) Resource_Free(data);

    texItem = new CTextureList;
    texItem->tex = (HTEXTURE)pTex;
    texItem->width = width;
    texItem->height = height;
    texItem->next = textures;
    textures = texItem;

    return (HTEXTURE)pTex;
}


HTEXTURE CALL HGE::Texture_LoadFromMemory(void* data, DWORD size, OUT int& width, OUT int& height, bool bMipmap)
{
    DWORD _size = size;
    CTextureList *texItem;

    D3DFORMAT fmt1, fmt2;
    LPDIRECT3DTEXTURE9 pTex;
    D3DXIMAGE_INFO info;

    if (*(DWORD*)data == 0x20534444) // Compressed DDS format magic number
    {
        fmt1 = D3DFMT_UNKNOWN;
        fmt2 = D3DFMT_A8R8G8B8;
    }
    else
    {
        fmt1 = D3DFMT_A8R8G8B8;
        fmt2 = D3DFMT_UNKNOWN;
    }

    if (FAILED(D3DXCreateTextureFromFileInMemoryEx(pD3DDevice, data, _size,
        D3DX_DEFAULT, D3DX_DEFAULT,
        bMipmap ? 0 : 1,    // Mip levels
        0,                  // Usage
        fmt1,               // Format
        D3DPOOL_MANAGED,    // Memory pool
        D3DX_FILTER_NONE,   // Filter
        D3DX_DEFAULT,       // Mip filter
        0,                  // Color key
        &info, NULL,
        &pTex)))
    {
        if (FAILED(D3DXCreateTextureFromFileInMemoryEx(pD3DDevice, data, _size,
            D3DX_DEFAULT, D3DX_DEFAULT,
            bMipmap ? 0 : 1,    // Mip levels
            0,                  // Usage
            fmt2,               // Format
            D3DPOOL_MANAGED,    // Memory pool
            D3DX_FILTER_NONE,   // Filter
            D3DX_DEFAULT,       // Mip filter
            0,                  // Color key
            &info, NULL,
            &pTex)))
        {
            _PostError("Can't create texture");
            return NULL;
        }
    }

    width = info.Width;
    height = info.Height;

    texItem = new CTextureList;
    texItem->tex = (HTEXTURE)pTex;
    texItem->width = width;
    texItem->height = height;
    texItem->next = textures;
    textures = texItem;

    return (HTEXTURE)pTex;
}

void CALL HGE::Texture_SaveToFile(HTEXTURE tex, const char* filename)
{
    ::D3DXSaveTextureToFileA(filename, D3DXIFF_PNG, (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex, nullptr);
}

ID3DXBuffer* CALL HGE::Texture_SaveToMemory(HTEXTURE tex)
{
    ID3DXBuffer* pD3DXBuffer = nullptr;
    ::D3DXSaveTextureToFileInMemory(&pD3DXBuffer, D3DXIFF_BMP, (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex, nullptr);
    return pD3DXBuffer;
}

ID3DXBuffer* CALL HGE::Surface_SaveToMemory(PDIRECT3DSURFACE9 xSurface)
{
    ID3DXBuffer* pD3DXBuffer = nullptr;
    ::D3DXSaveSurfaceToFileInMemory(&pD3DXBuffer, D3DXIFF_BMP, xSurface, nullptr, nullptr);
    return pD3DXBuffer;
}

void CALL HGE::Texture_Free(HTEXTURE tex)
{
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    CTextureList *texItem = textures, *texPrev = 0;

    while (texItem)
    {
        if (texItem->tex == tex)
        {
            if (texPrev)
            {
                texPrev->next = texItem->next;
            }
            else
            {
                textures = texItem->next;
            }

            delete texItem;
            break;
        }

        texPrev = texItem;
        texItem = texItem->next;
    }

    if (pTex != NULL)
    {
        pTex->Release();
    }
}

int CALL HGE::Texture_GetWidth(HTEXTURE tex, bool bOriginal)
{
    D3DSURFACE_DESC TDesc;
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    CTextureList *texItem = textures;
    int width = 0;

    if (bOriginal)
    {
        while (texItem && width == 0)
        {
            if (texItem->tex == tex)
            {
                width = texItem->width;
            }

            texItem = texItem->next;
        }
    }
    else
    {
        if (SUCCEEDED(pTex->GetLevelDesc(0, &TDesc)))
        {
            width = TDesc.Width;
        }
    }

    return width;
}


int CALL HGE::Texture_GetHeight(HTEXTURE tex, bool bOriginal)
{
    D3DSURFACE_DESC TDesc;
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    CTextureList *texItem = textures;
    int height = 0;

    if (bOriginal)
    {
        while (texItem && height == 0)
        {
            if (texItem->tex == tex)
            {
                height = texItem->height;
            }

            texItem = texItem->next;
        }
    }
    else
    {
        if (SUCCEEDED(pTex->GetLevelDesc(0, &TDesc)))
        {
            height = TDesc.Height;
        }
    }

    return height;
}


DWORD * CALL HGE::Texture_Lock(HTEXTURE tex, bool bReadOnly, int left, int top, int width, int height)
{
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    D3DSURFACE_DESC TDesc;
    D3DLOCKED_RECT TRect;
    RECT region, *prec;
    int flags;

    pTex->GetLevelDesc(0, &TDesc);
    if (TDesc.Format != D3DFMT_A8R8G8B8 && TDesc.Format != D3DFMT_X8R8G8B8) return 0;

    if (width && height)
    {
        region.left = left;
        region.top = top;
        region.right = left + width;
        region.bottom = top + height;
        prec = &region;
    }
    else
    {
        prec = 0;
    }

    if (bReadOnly)
    {
        flags = D3DLOCK_READONLY;
    }
    else
    {
        flags = 0;
    }

    if (FAILED(pTex->LockRect(0, &TRect, prec, flags)))
    {
        _PostError("Can't lock texture");
        return 0;
    }

    return (DWORD *)TRect.pBits;
}


void CALL HGE::Texture_Unlock(HTEXTURE tex)
{
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    pTex->UnlockRect(0);
}

HgeFont CALL HGE::Font_Create(UINT xWidth, UINT xHeight, UINT xWeight, DWORD xFontColor, const wchar_t* xFaceName, BOOL xItalic)
{
    HgeFont xHgeFont;
    memset(&xHgeFont, 0, sizeof(xHgeFont));

    xHgeFont.m_GDI_DC = ::GetDC((HWND)m_hWindow);

    xHgeFont.m_D3DFont = nullptr;
    MS_D3DXFONT_DESCW d3dFont;
    memset(&d3dFont, 0, sizeof(d3dFont));
    d3dFont.Height = xHeight;
    d3dFont.Width = xWidth;
    //d3dFont.Width = 0;
    d3dFont.Weight = xWeight;
    d3dFont.Italic = xItalic;
    d3dFont.Quality = PROOF_QUALITY;
    d3dFont.MipLevels = 1;
    d3dFont.PitchAndFamily = FIXED_PITCH;
    d3dFont.CharSet = DEFAULT_CHARSET;
    wcscpy_s(d3dFont.FaceName, xFaceName);

    //if (m_MapGDIFont.find(d3dFont) != m_MapGDIFont.end())
    //{
    //    xHgeFont.m_GDIFont = m_MapGDIFont[d3dFont];
    //}
    //else
    {
        xHgeFont.m_GDIFont = ::CreateFontW(
            d3dFont.Height,
            d3dFont.Width,
            0,
            0,
            d3dFont.Weight,
            d3dFont.Italic,
            FALSE,
            FALSE,
            d3dFont.CharSet,
            d3dFont.OutputPrecision,
            CLIP_DEFAULT_PRECIS,
            d3dFont.Quality,
            d3dFont.PitchAndFamily,
            d3dFont.FaceName);
        //m_MapGDIFont.insert(std::pair<MS_D3DXFONT_DESCW, HFONT>(d3dFont, xHgeFont.m_GDIFont));
    }

    //if (m_MapD3DFont.find(d3dFont) != m_MapD3DFont.end())
    //{
    //    xHgeFont.m_D3DFont = m_MapD3DFont[d3dFont];
    //}
    //else
    {
        if (FAILED(::D3DXCreateFontIndirectW(pD3DDevice, &d3dFont, &(xHgeFont.m_D3DFont))))
        {
            _PostError("Can't create font");
        }
        //m_MapD3DFont.insert(std::pair<MS_D3DXFONT_DESCW, LPD3DXFONT>(d3dFont, xHgeFont.m_D3DFont));
    }
    xHgeFont.m_Color = xFontColor;
    return xHgeFont;
}

void CALL HGE::Font_Free(HgeFont xHgeFont)
{
    xHgeFont.m_D3DFont->Release();
    ::DeleteObject(xHgeFont.m_GDIFont);
    //::ReleaseDC((HWND)m_hWindow, xHgeFont.m_GDI_DC);
}

SIZE CALL HGE::Font_GetDrawTextRectW(HgeFont xFont, std::wstring xStr, int xStrLen)
{
    SIZE xSize;
    if (xStrLen == -1)
    {
        xStrLen = (int)xStr.size();
    }
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), xStrLen, &xSize);
    return xSize;
}

SIZE CALL HGE::Font_GetDrawTextRectA(HgeFont xFont, std::string xStr, int xStrLen)
{
    SIZE xSize;
    if (xStrLen == -1)
    {
        xStrLen = (int)xStr.size();
    }
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    ::GetTextExtentPoint32A(xFont.m_GDI_DC, xStr.c_str(), xStrLen, &xSize);
    return xSize;
}

INT CALL HGE::Font_GetDrawMaxTextCountInWidthA(HgeFont xFont, INT xWidth, std::string xStr)
{
    SIZE xSize;
    INT xStrLen = (INT)xStr.size();
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    for (INT i = 0; i < xStrLen; i++)
    {
        ::GetTextExtentPoint32A(xFont.m_GDI_DC, xStr.c_str(), i, &xSize);
        if (xSize.cx > xWidth)
        {
            return i - 1;
        }
    }
    return xStrLen;
}

INT CALL HGE::Font_GetDrawMaxTextCountInWidthW(HgeFont xFont, INT xWidth, std::wstring xStr)
{
    SIZE xSize;
    INT xStrLen = (INT)xStr.size();
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    for (INT i = 0; i < xStrLen; i++)
    {
        ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), i, &xSize);
        if (xSize.cx >= xWidth)
        {
            return i - 1;
        }
    }
    return xStrLen;
}

INT CALL HGE::Font_GetDrawMaxTextCountInWidthDescA(HgeFont xFont, INT xWidth, std::string xStr)
{
    SIZE xSize;
    INT xStrLen = (INT)xStr.size();
    char szBuff[1024];
    strcpy_s(szBuff, xStr.c_str());
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    for (INT i = 0; i < xStrLen; i++)
    {
        ::GetTextExtentPoint32A(xFont.m_GDI_DC, &(szBuff[i]), (xStrLen - i), &xSize);
        if (xSize.cx <= xWidth)
        {
            return i;
        }
    }
    return xStrLen;
}

INT CALL HGE::Font_GetDrawMaxTextCountInWidthDescW(HgeFont xFont, INT xWidth, std::wstring xStr)
{
    SIZE xSize;
    INT xStrLen = (INT)xStr.size();
    wchar_t szBuff[1024];
    wcscpy_s(szBuff, xStr.c_str());
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    for (INT i = 0; i < xStrLen; i++)
    {
        ::GetTextExtentPoint32W(xFont.m_GDI_DC, &(szBuff[i]), (xStrLen - i), &xSize);
        if (xSize.cx <= xWidth)
        {
            return i;
        }
    }
    return xStrLen;
}

INT CALL HGE::Font_DrawTextW(HgeFont xFont, std::wstring xStr, LONG x1, LONG y1)
{
    SIZE xSize = this->Font_GetDrawTextRectW(xFont, xStr, -1);
    return this->Font_DrawTextW(xFont, xStr, x1, y1, xSize.cx, xSize.cy);
}

INT CALL HGE::Font_DrawTextA(HgeFont xFont, std::string xStr, LONG x1, LONG y1)
{
    SIZE xSize = this->Font_GetDrawTextRectA(xFont, xStr, -1);
    return this->Font_DrawTextA(xFont, xStr, x1, y1, xSize.cx, xSize.cy);
}

INT CALL HGE::Font_DrawTextA(HgeFont xFont, std::string xStr, LONG x1, LONG y1, LONG w, LONG h)
{
    RECT xRect = { x1, y1, x1 + w, y1 + h };
    switch (xFont.m_Effect)
    {
        case 1:
        {
            if (xFont.m_IsStroke)
            {
                RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 };
                xFont.m_D3DFont->DrawTextA(pSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor);

                RECT xRect_1 = { xRect.left - 1, xRect.top - 1, xRect.right - 1 ,xRect.bottom - 1 };
                xFont.m_D3DFont->DrawTextA(pSprite, xStr.c_str(), -1, &xRect_1, xFont.m_Format, xFont.m_StrokeColor);
            }
            return xFont.m_D3DFont->DrawTextA(pSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }break;

        case 0:
        default:
        {
            if (xFont.m_IsStroke)
            {
                RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 };
                xFont.m_D3DFont->DrawTextA(pSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor);
            }
            return xFont.m_D3DFont->DrawTextA(pSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }
    }
}

INT CALL HGE::Font_DrawTextW(HgeFont xFont, std::wstring xStr, LONG x1, LONG y1, LONG w, LONG h)
{
    RECT xRect = { x1, y1, x1 + w, y1 + h };
    switch (xFont.m_Effect)
    {
        case 1:
        {
            if (xFont.m_IsStroke) { RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 }; xFont.m_D3DFont->DrawTextW(pSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor); }
            return xFont.m_D3DFont->DrawTextW(pSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }break;
        case 0:
        default:
        {
            if (xFont.m_IsStroke) { RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 }; xFont.m_D3DFont->DrawTextW(pSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor); }
            return xFont.m_D3DFont->DrawTextW(pSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }
    }
}

void CALL HGE::IME_StrDraw(HgeFont& xFont, LONG x, LONG y, LONG xCharH)
{
    for (DWORD i = 0; i < this->uIMECharCount; i++)
    {
        this->Font_DrawTextA(xFont, this->strIMECharList[i] + "\n", x, y + xCharH * i);
    }
}

void CALL HGE::IME_StrDraw(HgeFont& xFont, HGE_E_POS_TYPE xPosType, LONG dwOffsetX, LONG dwOffsetY, LONG xCharH)
{
    switch (xPosType)
    {
        case HGE_EPT_LEFTTOP:
        {
            return this->IME_StrDraw(xFont, dwOffsetX, dwOffsetY, xCharH);
        }break;

        case HGE_EPT_LEFTBOTTOM:
        {
            LONG dwScreenHeight = this->System_GetState(HGE_SCREENHEIGHT);
            LONG dwBackHeight = xCharH * this->uIMECharCount;
            return this->IME_StrDraw(xFont, dwOffsetX, dwScreenHeight - dwBackHeight - dwOffsetY, xCharH);
        }break;
        case HGE_EPT_RIGHTTOP:
        {
            LONG dwScreenWidth = this->System_GetState(HGE_SCREENWIDTH);
            LONG dwBackWidth = xCharH * this->uIMECharCount;
            return this->IME_StrDraw(xFont, dwScreenWidth - dwBackWidth - dwOffsetX, dwOffsetY, xCharH);
        }break;
    }
}

void CALL HGE::Gfx_RenderTexture(HTEXTURE hTex, RECT& xRect, D3DXVECTOR3& xCenter, D3DXVECTOR3& xPosition, float zw, float zh, DWORD dwColor)
{
    if (zw != 1.0f || zh != 1.0f)
    {
        D3DXMATRIX matScale;
        pSprite->GetTransform(&matScale);
        D3DXMatrixScaling(&matScale, zw, zh, 1.0f);
        pSprite->SetTransform(&matScale);

        float xW = (float)(xRect.right - xRect.left);
        xPosition.x *= xW / (xW*zw);

        float xH = (float)(xRect.bottom - xRect.top);
        xPosition.y *= xH / (xH*zh);

        pSprite->Draw((LPDIRECT3DTEXTURE9)hTex, &xRect, &xCenter, &xPosition, dwColor);

        D3DXMATRIX default_mat;
        D3DXMatrixScaling(&default_mat, 1.0f, 1.0f, 1.0f);
        pSprite->SetTransform(&default_mat);
    }
    else
    {
        pSprite->Draw((LPDIRECT3DTEXTURE9)hTex, &xRect, &xCenter, &xPosition, dwColor);
    }
}



//////// Implementation ////////

void HGE::_render_batch(bool bEndScene)
{
    if (VertArray)
    {
        pVB->Unlock();

        if (nPrim)
        {
            switch (CurPrimType)
            {
                case HGEPRIM_QUADS:
                    pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nPrim << 2, 0, nPrim << 1);
                    break;

                case HGEPRIM_TRIPLES:
                    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nPrim);
                    break;

                case HGEPRIM_LINES:
                    pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, nPrim);
                    break;
            }

            nPrim = 0;
        }

        if (bEndScene) VertArray = 0;
        else pVB->Lock(0, 0, (VOID**)&VertArray, 0);
    }
}

void HGE::_SetBlendMode(int blend)
{
    if ((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND))
    {
        if (blend & BLEND_ALPHABLEND) pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        else pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    if ((blend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE))
    {
        if (blend & BLEND_ZWRITE) pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        else pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }

    if ((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD))
    {
        if (blend & BLEND_COLORADD) pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
        else pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }

    CurBlendMode = blend;
}

void HGE::_SetProjectionMatrix(int width, int height)
{
    D3DXMATRIX tmp;
    D3DXMatrixScaling(&matProj, 1.0f, -1.0f, 1.0f);
    D3DXMatrixTranslation(&tmp, -0.5f, height + 0.5f, 0.0f);
    D3DXMatrixMultiply(&matProj, &matProj, &tmp);
    D3DXMatrixOrthoOffCenterLH(&tmp, 0, (float)width, 0, (float)height, 0.0f, 1.0f);
    D3DXMatrixMultiply(&matProj, &matProj, &tmp);
}

bool HGE::_GfxInit()
{
    static const char* szFormats[] = { "UNKNOWN", "R5G6B5", "X1R5G5B5", "A1R5G5B5", "X8R8G8B8", "A8R8G8B8" };
    D3DADAPTER_IDENTIFIER9 AdID;
    D3DDISPLAYMODE Mode;
    D3DFORMAT Format = D3DFMT_UNKNOWN;
    UINT nModes, i;

    // Init D3D

    pD3D = Direct3DCreate9(D3D_SDK_VERSION); // D3D_SDK_VERSION
    if (pD3D == NULL)
    {
        _PostError("Can't create D3D interface");
        return false;
    }

    // Get adapter info

    pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &AdID);
    this->System_Log("D3D Driver: %s", AdID.Driver);
    this->System_Log("Description: %s", AdID.Description);
    this->System_Log("Version: %d.%d.%d.%d",
        HIWORD(AdID.DriverVersion.HighPart),
        LOWORD(AdID.DriverVersion.HighPart),
        HIWORD(AdID.DriverVersion.LowPart),
        LOWORD(AdID.DriverVersion.LowPart));

    // Set up Windowed presentation parameters

    if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode)) || Mode.Format == D3DFMT_UNKNOWN)
    {
        _PostError("Can't determine desktop video mode");
        if (System_GetState(HGE_WINDOWED)) return false;
    }

    ZeroMemory(&d3dppW, sizeof(d3dppW));

    d3dppW.BackBufferWidth = System_GetState(HGE_SCREENWIDTH);
    d3dppW.BackBufferHeight = System_GetState(HGE_SCREENHEIGHT);
    d3dppW.BackBufferFormat = Mode.Format;
    d3dppW.BackBufferCount = 1;
    d3dppW.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dppW.hDeviceWindow = (HWND)System_GetState(HGE_HWND);
    d3dppW.Windowed = TRUE;

    if (System_GetState(HGE_FPS) == HGEFPS_VSYNC)
    {
        d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
        d3dppW.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }
    else
    {
        d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
        d3dppW.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    if (System_GetState(HGE_ZBUFFER))
    {
        d3dppW.EnableAutoDepthStencil = TRUE;
        d3dppW.AutoDepthStencilFormat = D3DFMT_D16;
    }

    // Set up Full Screen presentation parameters

    nModes = pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, Mode.Format);
    m_Set_AdapterMode.clear();

    for (i = 0; i < nModes; i++)
    {
        pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, Mode.Format, i, &Mode);

        char szBuff[100];
        sprintf_s(szBuff, "%d*%d", Mode.Width, Mode.Height);
        m_Set_AdapterMode.insert(szBuff);

        if (Mode.Width != (UINT)System_GetState(HGE_SCREENWIDTH) || Mode.Height != (UINT)System_GetState(HGE_SCREENHEIGHT)) continue;
        if (System_GetState(HGE_SCREENBPP) == 16 && (_format_id(Mode.Format) > _format_id(D3DFMT_A1R5G5B5))) continue;
        if (_format_id(Mode.Format) > _format_id(Format)) Format = Mode.Format;
    }

    if (Format == D3DFMT_UNKNOWN)
    {
        _PostError("Can't find appropriate full screen video mode");
        if (!System_GetState(HGE_WINDOWED)) return false;
    }

    ZeroMemory(&d3dppFS, sizeof(d3dppFS));

    d3dppFS.BackBufferWidth = System_GetState(HGE_SCREENWIDTH);
    d3dppFS.BackBufferHeight = System_GetState(HGE_SCREENHEIGHT);
    d3dppFS.BackBufferFormat = Format;
    d3dppFS.BackBufferCount = 1;
    d3dppFS.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dppFS.hDeviceWindow = (HWND)System_GetState(HGE_HWND);
    d3dppFS.Windowed = FALSE;

    d3dppFS.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dppFS.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    if (System_GetState(HGE_FPS) == HGEFPS_VSYNC)
    {
        d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    }
    else
    {
        d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    if (System_GetState(HGE_ZBUFFER))
    {
        d3dppFS.EnableAutoDepthStencil = TRUE;
        d3dppFS.AutoDepthStencilFormat = D3DFMT_D16;
    }

    d3dpp = System_GetState(HGE_WINDOWED) ? &d3dppW : &d3dppFS;

    if (_format_id(d3dpp->BackBufferFormat) < 4) System_SetState(HGE_SCREENBPP, 16);
    else System_SetState(HGE_SCREENBPP, 32);

    // Create D3D Device

    if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)System_GetState(HGE_HWND),
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        d3dpp, &pD3DDevice)))
    {
        _PostError("Can't create D3D device");
        return false;
    }

    _AdjustWindow();

    this->System_Log("Mode: %d x %d x %s\n", System_GetState(HGE_SCREENWIDTH), System_GetState(HGE_SCREENHEIGHT), szFormats[_format_id(Format)]);

    // Create vertex batch buffer

    VertArray = 0;
    textures = 0;

    // Init all stuff that can be lost

    _SetProjectionMatrix(System_GetState(HGE_SCREENWIDTH), System_GetState(HGE_SCREENHEIGHT));
    D3DXMatrixIdentity(&matView);

    if (FAILED(D3DXCreateSprite(pD3DDevice, &pSprite)))
    {
        _PostError("Can't create Sprite");
        return false;
    }

    if (!_init_lost()) return false;
    Gfx_Clear(0);

    return true;
}

int HGE::_format_id(D3DFORMAT fmt)
{
    switch (fmt)
    {
        case D3DFMT_R5G6B5:		return 1;
        case D3DFMT_X1R5G5B5:	return 2;
        case D3DFMT_A1R5G5B5:	return 3;
        case D3DFMT_X8R8G8B8:	return 4;
        case D3DFMT_A8R8G8B8:	return 5;
        default:				return 0;
    }
}

void HGE::_AdjustWindow()
{
    RECT *rc;
    LONG style;

    if (System_GetState(HGE_WINDOWED)) { rc = &rectW; style = styleW; }
    else { rc = &rectFS; style = styleFS; }
    SetWindowLong((HWND)System_GetState(HGE_HWND), GWL_STYLE, style);

    style = GetWindowLong((HWND)System_GetState(HGE_HWND), GWL_EXSTYLE);
    if (System_GetState(HGE_WINDOWED))
    {
        //SetWindowLong((HWND)System_GetState(HGE_HWND), GWL_EXSTYLE, style & (~WS_EX_TOPMOST));
        SetWindowPos((HWND)System_GetState(HGE_HWND), HWND_NOTOPMOST, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_FRAMECHANGED);
    }
    else
    {
        //SetWindowLong((HWND)System_GetState(HGE_HWND), GWL_EXSTYLE, style | WS_EX_TOPMOST);
        SetWindowPos((HWND)System_GetState(HGE_HWND), HWND_TOPMOST, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_FRAMECHANGED);
    }
}

//void HGE::_Resize(int width, int height)
//{
//    if (System_GetState(HGE_HWNDPARENT))
//    {
//        d3dppW.BackBufferWidth = width;
//        d3dppW.BackBufferHeight = height;
//
//        System_SetState(HGE_SCREENWIDTH, width);
//        System_SetState(HGE_SCREENHEIGHT, height);
//
//        _SetProjectionMatrix(System_GetState(HGE_SCREENWIDTH), System_GetState(HGE_SCREENHEIGHT));
//        _GfxRestore();
//    }
//}

void HGE::_GfxDone()
{
    CRenderTargetList *target = pTargets, *next_target;

    while (textures)
    {
        Texture_Free(textures->tex);
    }

    if (pScreenSurf)
    {
        pScreenSurf->Release();
        pScreenSurf = nullptr;
    }

    if (pScreenDepth)
    {
        pScreenDepth->Release();
        pScreenDepth = nullptr;
    }

    if (pSprite)
    {
        pSprite->Release();
        pSprite = nullptr;
    }

    while (target)
    {
        if (target->pTex)
        {
            target->pTex->Release();
        }

        if (target->pDepth)
        {
            target->pDepth->Release();
        }

        next_target = target->next;
        delete target;
        target = next_target;
    }
    pTargets = 0;

    if (pIB)
    {
        pD3DDevice->SetIndices(NULL);
        pIB->Release();
        pIB = 0;
    }

    if (pVB)
    {
        if (VertArray)
        {
            pVB->Unlock();
            VertArray = 0;
        }
        pD3DDevice->SetStreamSource(0, nullptr, 0, sizeof(hgeVertex));
        pVB->Release();
        pVB = 0;
    }

    if (pD3DDevice)
    {
        pD3DDevice->Release();
        pD3DDevice = nullptr;
    }

    if (pD3D)
    {
        pD3D->Release();
        pD3D = nullptr;
    }
}


bool HGE::_GfxRestore()
{
    if (m_lpOnGfxLostFun)
    {
        m_lpOnGfxLostFun();
    }

    //    return true;
    //
    //    this->_GfxDone();
    //
    //    exit(0);
    //
    //    this->_GfxInit();
    //
    //    if (!_init_lost())
    //    {
    //        return false;
    //    }
    //
    //    if (System_GetState(HGE_GFXRESTOREFUNC)) return System_GetState(HGE_GFXRESTOREFUNC)();
    //
    return true;
}


bool HGE::_init_lost()
{
    CRenderTargetList *target = pTargets;

    // Store render target
    pScreenSurf = 0;
    pScreenDepth = 0;

    if (!pD3DDevice) { return true; }

    pD3DDevice->GetRenderTarget(0, &pScreenSurf);
    pD3DDevice->GetDepthStencilSurface(&pScreenDepth);

    while (target)
    {
        if (target->pTex)
        {
            D3DXCreateTexture(pD3DDevice, target->width, target->height, 1, D3DUSAGE_RENDERTARGET, d3dpp->BackBufferFormat, D3DPOOL_DEFAULT, &target->pTex);
        }
        if (target->pDepth)
        {
            pD3DDevice->CreateDepthStencilSurface(target->width, target->height, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, false, &target->pDepth, NULL);
        }

        target = target->next;
    }

    // Create Vertex buffer

    if (FAILED(pD3DDevice->CreateVertexBuffer(VERTEX_BUFFER_SIZE * sizeof(hgeVertex),
        D3DUSAGE_WRITEONLY,
        D3DFVF_HGEVERTEX,
        D3DPOOL_DEFAULT,
        &pVB,
        NULL)))
    {
        _PostError("Can't create D3D vertex buffer");
        return false;
    }

    pD3DDevice->SetVertexShader(NULL);
    pD3DDevice->SetFVF(D3DFVF_HGEVERTEX);
    pD3DDevice->SetStreamSource(0, pVB, 0, sizeof(hgeVertex));

    // Create and setup Index buffer

    if (FAILED(pD3DDevice->CreateIndexBuffer(VERTEX_BUFFER_SIZE * 6 / 4 * sizeof(WORD),
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_DEFAULT,
        &pIB
        , NULL)))
    {
        _PostError("Can't create D3D index buffer");
        return false;
    }

    WORD *pIndices, n = 0;
    if (FAILED(pIB->Lock(0, 0, (void**)&pIndices, 0)))
    {
        _PostError("Can't lock D3D index buffer");
        return false;
    }

    for (int i = 0; i < VERTEX_BUFFER_SIZE / 4; i++) {
        *pIndices++ = n;
        *pIndices++ = n + 1;
        *pIndices++ = n + 2;
        *pIndices++ = n + 2;
        *pIndices++ = n + 3;
        *pIndices++ = n;
        n += 4;
    }

    pIB->Unlock();
    pD3DDevice->SetIndices(pIB);

    // Set common render states

        //pD3DDevice->SetRenderState( D3DRS_LASTPIXEL, FALSE );
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);
    pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

    if (System_GetState(HGE_TEXTUREFILTER))
    {
        pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    else
    {
        pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    }

    nPrim = 0;
    CurPrimType = HGEPRIM_QUADS;
    CurBlendMode = BLEND_DEFAULT;
    CurTexture = NULL;
    CurShader = NULL;

    pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

    return true;
}

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

void HGE::_PostError(char *error)
{
    this->System_Log(error);
    strcpy(szError, error);
}
