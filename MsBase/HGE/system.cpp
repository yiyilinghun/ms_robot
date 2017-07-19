/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/

#include "hge.h"
//int  g_nRef = 0;
//HGE* G_HGE = nullptr;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         ((p)?((p)->Release(),(p)=nullptr):((p) = (p)))
#endif

#pragma warning(disable:4996)

//HGE* HGE::_Interface_Get(HGE*lpHGE)
//{
//    if (!g_lpHGE) g_lpHGE = new HGE(lpHGE);
//    g_nRef++;
//    return g_lpHGE;
//}

void CALL HGE::Release()
{
    m_Set_AdapterMode.clear();

    SAFE_RELEASE(m_RenderTexture);
    SAFE_RELEASE(m_RenderSurface);
    SAFE_RELEASE(m_CanEdit_RenderTexture);
    SAFE_RELEASE(m_CanEdit_RenderSurface);

    if (this->System_GetState(HGE_HWND)) System_Shutdown();
    Resource_RemoveAllPacks();

    _GfxDone();

    _SoundDone();

    GdiplusShutdown(m_gdiplusToken);
}

HCURSOR CALL HGE::System_CreateCursor(int xHotX, int xHotY, int xWidth, int xHeight, void* lpData)
{
    return ::CreateCursor(hInstance, xHotX, xHotY, xWidth, xHeight, lpData, lpData);
}

HCURSOR CALL HGE::System_LoadCursor(const char* filename)
{
    return ::LoadCursorFromFileA(filename);
}

void CALL HGE::System_FreeCursor(HCURSOR hCursor)
{
    ::DestroyCursor(hCursor);
}

//bool CALL HGE::System_Initiate(HWND hWnd)
//{
//    if (this->System_GetState(HGE_HWND)) return true;
//
//    HWND			hwnd;
//    OSVERSIONINFO	os_ver;
//    SYSTEMTIME		tm;
//    MEMORYSTATUS	mem_st;
//    WNDCLASS		winclass;
//    int				width, height;
//
//    // Log system info
//
//    System_Log("HGE Started..\n");
//
//    System_Log("HGE version: %X.%X", HGE_VERSION >> 8, HGE_VERSION & 0xFF);
//    GetLocalTime(&tm);
//    System_Log("Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond);
//
//    System_Log("Application: %s", this->System_GetState(HGE_TITLE));
//    os_ver.dwOSVersionInfoSize = sizeof(os_ver);
//    GetVersionEx(&os_ver);
//    System_Log("OS: Windows %ld.%ld.%ld", os_ver.dwMajorVersion, os_ver.dwMinorVersion, os_ver.dwBuildNumber);
//
//    GlobalMemoryStatus(&mem_st);
//    System_Log("Memory: %ldK total, %ldK free\n", mem_st.dwTotalPhys / 1024L, mem_st.dwAvailPhys / 1024L);
//
//
//    // Register window class
//
//    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
//    winclass.lpfnWndProc = WindowProc;
//    winclass.cbClsExtra = 0;
//    winclass.cbWndExtra = 0;
//    winclass.hInstance = hInstance;
//    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
//    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//    winclass.lpszMenuName = NULL;
//    winclass.lpszClassName = WINDOW_CLASS_NAME;
//
//    if (this->System_GetState(HGE_ICON)) winclass.hIcon = LoadIcon(hInstance, this->System_GetState(HGE_ICON));
//    else winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//
//    if (!RegisterClass(&winclass)) {
//        _PostError("Can't register window class");
//        return false;
//    }
//
//    // Create window
//
//    width = this->System_GetState(HGE_SCREENWIDTH) + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
//    height = this->System_GetState(HGE_SCREENHEIGHT) + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
//
//    rectW.left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
//    rectW.top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
//    rectW.right = rectW.left + width;
//    rectW.bottom = rectW.top + height;
//    styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
//
//    rectFS.left = 0;
//    rectFS.top = 0;
//    rectFS.right = this->System_GetState(HGE_SCREENWIDTH);
//    rectFS.bottom = this->System_GetState(HGE_SCREENHEIGHT);
//    styleFS = WS_POPUP | WS_VISIBLE; //WS_POPUP
//
//    if (this->System_GetState(HGE_HWNDPARENT))
//    {
//        rectW.left = 0;
//        rectW.top = 0;
//        rectW.right = this->System_GetState(HGE_SCREENWIDTH);
//        rectW.bottom = this->System_GetState(HGE_SCREENHEIGHT);
//        styleW = WS_CHILD | WS_VISIBLE;
//
//        this->System_SetState(HGE_WINDOWED, true);
//    }
//
//    //hwnd = hWnd;
//    //CSharpWndProc = (WNDPROC)::GetWindowLong(hwnd, GWL_WNDPROC);
//    //::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WindowProc);
//
//    if (this->System_GetState(HGE_WINDOWED))
//    {
//        hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, this->System_GetState(HGE_TITLE), styleW,
//            rectW.left, rectW.top, rectW.right - rectW.left, rectW.bottom - rectW.top,
//            (HWND)this->System_GetState(HGE_HWNDPARENT), NULL, hInstance, NULL);
//    }
//    else
//    {
//        hwnd = CreateWindowEx(WS_EX_TOPMOST, WINDOW_CLASS_NAME, this->System_GetState(HGE_TITLE), styleFS,
//            0, 0, 0, 0,
//            NULL, NULL, hInstance, NULL);
//    }
//
//    if (!hwnd)
//    {
//        _PostError("Can't create window");
//        return false;
//    }
//
//    this->System_SetState(HGE_HWND, (HWINDOW)hwnd);
//
//    ShowWindow(hwnd, SW_SHOW);
//
//    // Init subsystems
//
//    timeBeginPeriod(1);
//    Random_Seed();
//    _InitPowerStatus();
//    _InputInit();
//
//    System_Log("Init done.\n");
//
//    fTime = 0.0f;
//    t0 = t0fps = timeGetTime();
//    dt = cfps = 0;
//    nFPS = 0;
//
//    // Show splash
//
//#ifdef DEMO
//
//    bool(*func)();
//    bool(*rfunc)();
//    HWND			hwndTmp;
//
//    if (g_lpHGE->bDMO)
//    {
//        Sleep(200);
//        func = (bool(*)())g_lpHGE->System_GetStateFunc(HGE_FRAMEFUNC);
//        rfunc = (bool(*)())g_lpHGE->System_GetStateFunc(HGE_RENDERFUNC);
//        hwndTmp = hwndParent; hwndParent = 0;
//        g_lpHGE->System_SetStateFunc(HGE_FRAMEFUNC, DFrame);
//        g_lpHGE->System_SetStateFunc(HGE_RENDERFUNC, 0);
//        DInit();
//        g_lpHGE->System_Start();
//        DDone();
//        hwndParent = hwndTmp;
//        g_lpHGE->System_SetStateFunc(HGE_FRAMEFUNC, func);
//        g_lpHGE->System_SetStateFunc(HGE_RENDERFUNC, rfunc);
//    }
//
//#endif
//
//    // Done
//
//    return true;
//}

void CALL HGE::System_Shutdown()
{
    System_Log("\nFinishing..");

    timeEndPeriod(1);

    if (hSearch)
    {
        FindClose(hSearch);
        hSearch = 0;
    }

    _ClearQueue();
    _DonePowerStatus();

    if (this->System_GetState(HGE_HWND))
    {
        DestroyWindow((HWND)this->System_GetState(HGE_HWND));
        this->System_SetState(HGE_HWND, 0);
    }

    if (hInstance) UnregisterClassW(m_szWndName, hInstance);

    System_Log("The End.");
}


bool CALL HGE::System_Start()
{
    MSG msg;

    if (!this->System_GetState(HGE_HWND))
    {
        _PostError("System_Start: System_Initiate wasn't called");
        return false;
    }

    if (!this->System_GetState(HGE_FRAMEFUNC))
    {
        _PostError("System_Start: No frame function defined");
        return false;
    }

    bActive = true;

    if (this->System_GetState(HGE_FPS) > 0)
        nFixedDelta = int(1000.0f / this->System_GetState(HGE_FPS));
    else
        nFixedDelta = 0;

    ::SetForegroundWindow(this->System_GetState(HGE_HWND));

    // MAIN LOOP
    for (;;)
    {

        // Process window messages if not in "child mode"
        // (if in "child mode" the parent application will do this for us)

        if (!this->System_GetState(HGE_HWNDPARENT))
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    break;
                }
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                continue;
            }
        }

        // Check if mouse is over HGE window for Input_IsMouseOver
        _UpdateMouse();

        // If HGE window is focused or we have the "don't suspend" state - process the main loop
        if (bActive || this->System_GetState(HGE_DONTSUSPEND))
        {
            // Ensure we have at least 1ms time step
            // to not confuse user's code with 0
            do { dt = timeGetTime() - t0; } while (dt < 1);

            // If we reached the time for the next frame
            // or we just run in unlimited FPS mode, then
            // do the stuff
            if (dt >= nFixedDelta)
            {
                // fDeltaTime = time step in seconds returned by Timer_GetDelta
                fDeltaTime = dt / 1000.0f;

                // Cap too large time steps usually caused by lost focus to avoid jerks
                if (fDeltaTime > 0.2f)
                {
                    fDeltaTime = nFixedDelta ? nFixedDelta / 1000.0f : 0.01f;
                }

                // Update time counter returned Timer_GetTime
                fTime += fDeltaTime;

                // Store current time for the next frame
                // and count FPS
                t0 = timeGetTime();

                if (t0 - t0fps <= 1000)
                {
                    cfps++;
                }
                else
                {
                    nFPS = cfps; cfps = 0; t0fps = t0;
                    _UpdatePowerStatus();
                }

                // Do user's stuff
                if (this->System_GetState(HGE_FRAMEFUNC)())
                {
                    break;
                }
                if (this->System_GetState(HGE_RENDERFUNC)) this->System_GetState(HGE_RENDERFUNC)();

                // If if "child mode" - return after processing single frame
                if (this->System_GetState(HGE_HWNDPARENT))
                {
                    break;
                }

                // Clean up input events that were generated by
                // WindowProc and weren't handled by user's code
                _ClearQueue();

                // If we use VSYNC - we could afford a little
                // sleep to lower CPU usage

                // if(!bWindowed && nHGEFPS==HGEFPS_VSYNC) Sleep(1);
            }
            // If we have a fixed frame rate and the time
            // for the next frame isn't too close, sleep a bit
            else
            {
                if (nFixedDelta && dt + 3 < nFixedDelta) Sleep(1);
            }
        }
        // If main loop is suspended - just sleep a bit
        // (though not too much to allow instant window
        // redraw if requested by OS)
        else
        {
            Sleep(1);
        }
    }

    _ClearQueue();
    bActive = false;

    return true;
}

char* CALL HGE::System_GetErrorMessage()
{
    return szError;
}

void CALL HGE::System_Log_Time(const char* szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);
    char szBuff[1024];
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    sprintf_s(szBuff, "%04d/%02d/%02d %02d:%02d:%02d.%03d\t%s", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, szFormat);
    this->System_Log(szBuff, ap);
    va_end(ap);
}

void CALL HGE::System_Log(const char* szFormat, ...)
{
    FILE *hf = NULL;
    va_list ap;

    if (!this->System_GetState(HGE_LOGFILE)) return;

    hf = fopen(this->System_GetState(HGE_LOGFILE), "a");
    if (!hf) return;

    va_start(ap, szFormat);
    vfprintf(hf, szFormat, ap);
    va_end(ap);

    fprintf(hf, "\n");

    fclose(hf);
}

bool CALL HGE::System_Launch(const char* url)
{
    if ((QWORD)ShellExecuteA((HWND)this->System_GetState(HGE_HWND), NULL, url, NULL, NULL, SW_SHOWMAXIMIZED) > 32)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CALL HGE::System_Snapshot(const char* filename)
{
    LPDIRECT3DSURFACE9 pSurf;
    char *shotname, tempname[_MAX_PATH];
    int i;

    if (!filename)
    {
        i = 0;
        shotname = Resource_EnumFiles("shot???.bmp");
        while (shotname)
        {
            i++;
            shotname = Resource_EnumFiles();
        }
        sprintf(tempname, "shot%03d.bmp", i);
        filename = Resource_MakePath(tempname);
    }

    if (pD3DDevice)
    {
        pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
        D3DXSaveSurfaceToFileA(filename, D3DXIFF_BMP, pSurf, NULL, NULL);
        pSurf->Release();
    }
}

//////// Implementation ////////

//
//HGE::HGE()
//{
//    hInstance = GetModuleHandle(0);
//
//    this->System_SetState(HGE_HWND, 0);
//    this->System_SetState(HGE_HWNDPARENT, 0);
//
//    bActive = false;
//    szError[0] = 0;
//
//    hSearch = 0;
//    res = 0;
//
//    queue = 0;
//    Char = VKey = ClickKey = Zpos = 0;
//    Xpos = Ypos = 0.0f;
//    bMouseOver = false;
//    bCaptured = false;
//
//    this->System_SetState(HGE_FPS, HGEFPS_UNLIMITED);
//
//    fTime = 0.0f;
//    fDeltaTime = 0.0f;
//    nFPS = 0;
//
//    this->System_SetState(HGE_FRAMEFUNC, 0);
//    this->System_SetState(HGE_RENDERFUNC, 0);
//    this->System_SetState(HGE_FOCUSLOSTFUNC, 0);
//    this->System_SetState(HGE_FOCUSGAINFUNC, 0);
//    this->System_SetState(HGE_GFXRESTOREFUNC, 0);
//    this->System_SetState(HGE_EXITFUNC, 0);
//
//    this->System_SetState(HGE_ICON, 0);
//    this->System_SetState(HGE_TITLE, "HGE");
//
//    this->System_SetState(HGE_SCREENWIDTH, 800);
//    this->System_SetState(HGE_SCREENHEIGHT, 600);
//    this->System_SetState(HGE_SCREENBPP, 32);
//
//    this->System_SetState(HGE_WINDOWED, false);
//    this->System_SetState(HGE_ZBUFFER, false);
//    this->System_SetState(HGE_TEXTUREFILTER, true);
//
//    this->System_SetState(HGE_USESOUND, true);
//    this->System_SetState(HGE_SAMPLERATE, 44100);
//    this->System_SetState(HGE_FXVOLUME, 100);
//    this->System_SetState(HGE_MUSVOLUME, 100);
//    this->System_SetState(HGE_STREAMVOLUME, 100);
//
//    this->System_SetState(HGE_DONTSUSPEND, false);
//    this->System_SetState(HGE_HIDEMOUSE, true);
//
//    this->System_SetState(HGE_SHOWSPLASH, true);
//
//    this->System_SetState(HGE_LOGFILE, 0);
//    this->System_SetState(HGE_INIFILE, 0);
//
//    nFixedDelta = 0;
//
//    nPowerStatus = HGEPWR_UNSUPPORTED;
//    hKrnl32 = NULL;
//    lpfnGetSystemPowerStatus = NULL;
//
//#ifdef DEMO
//    bDMO = true;
//#endif
//
//    GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
//
//    int i;
//    for (i = (int)strlen(szAppPath) - 1; i > 0; i--) if (szAppPath[i] == '\\') break;
//
//    szAppPath[i + 1] = 0;
//}

void HGE::_FocusChange(bool bAct)
{
    bActive = bAct;

    if (bActive)
    {
        if (this->System_GetState(HGE_FOCUSGAINFUNC)) this->System_GetState(HGE_FOCUSGAINFUNC)();
    }
    else
    {
        if (this->System_GetState(HGE_FOCUSLOSTFUNC)) this->System_GetState(HGE_FOCUSLOSTFUNC)();
    }
}
bool g_IsRestart = false;
LONG g_WndX = -1;
LONG g_WndY = -1;
LRESULT CALLBACK _WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

HGE* g_temp_hge = nullptr;
std::map<HWND, HGE*> ms_map;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (g_temp_hge)
    {
        return g_temp_hge->HGE_WindowProc(hwnd, msg, wparam, lparam);
    }
    else
    {
        return ms_map[hwnd]->HGE_WindowProc(hwnd, msg, wparam, lparam);
    }
}
LRESULT HGE::HGE_WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    bool bActivating;

    switch (msg)
    {
        case WM_CREATE:
        {
            return FALSE;
        }

        case WM_PAINT:
        {
            if (this->System_GetState(HGE_HWND) && this->System_GetState(HGE_RENDERFUNC) && this->System_GetState(HGE_WINDOWED))
            {
                this->System_GetState(HGE_RENDERFUNC)();
            }
        } break;

        case WM_CHAR:
        {
            this->_BuildEvent(WM_CHAR, (int)wparam, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }break;

        case WM_IME_NOTIFY:
        {
            if (wparam == IMN_CHANGECANDIDATE)
            {
                HIMC hIMC = ::ImmGetContext(hwnd);
                DWORD dwCount = ::ImmGetCandidateListA(hIMC, 0, nullptr, 0);
                CHAR xCandidateData[8192];
                if (dwCount > 8192) { dwCount = 8192; }
                LPCANDIDATELIST xCandidate = (LPCANDIDATELIST)xCandidateData;
                ::ImmGetCandidateListA(hIMC, 0, xCandidate, dwCount);
                uIMECharCount = xCandidate->dwCount;
                if (uIMECharCount > 10) { uIMECharCount = 10; }
                for (DWORD i = 0; i < uIMECharCount; i++)
                {
                    strIMECharList[i] = xCandidateData + xCandidate->dwOffset[i];
                }
            }
            else
            {
                uIMECharCount = 0;
            }
        }break;

        case WM_IME_CHAR:
        {
            this->_BuildEvent(WM_IME_CHAR, (int)wparam, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }break;

        case WM_DESTROY:
        {
            RECT rc;
            ::GetWindowRect(this->System_GetState(HGE_HWND), &rc);
            g_WndX = rc.left;
            g_WndY = rc.top;

            if (!g_IsRestart)
            {
                PostQuitMessage(0);
            }
            return FALSE;
        }

        case WM_ACTIVATE:
        {
            // tricky: we should catch WA_ACTIVE and WA_CLICKACTIVE,
            // but only if HIWORD(wParam) (fMinimized) == FALSE (0)
            bActivating = (LOWORD(wparam) != WA_INACTIVE) && (HIWORD(wparam) == 0);

            if (this->System_GetState(HGE_HWND) && this->bActive != bActivating)
            {
                this->_FocusChange(bActivating);
            }

            //if (m_hParent)
            //{
            //    ::SetWindowPos(m_hParent, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            //}

            return FALSE;
        }

        case WM_SETCURSOR:
        {
            if (LOWORD(lparam) == HTCLIENT && this->System_GetState(HGE_HIDEMOUSE))
            {
                ::SetCursor(nullptr);
            }
            else if (m_hCursor)
            {
                ::SetCursor(m_hCursor);
            }
            else
            {
                ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
            }

            return FALSE;
        }

        case WM_SYSKEYDOWN:
        {
            if (wparam == VK_F4)
            {
                if (this->System_GetState(HGE_EXITFUNC) && !this->System_GetState(HGE_EXITFUNC)())
                {
                    return FALSE;
                }

                return DefWindowProc(hwnd, msg, wparam, lparam);
            }
            else if (wparam == VK_RETURN)
            {
                this->System_SetState(HGE_WINDOWED, !this->System_GetState(HGE_WINDOWED));

                return FALSE;
            }
            else
            {
                this->_BuildEvent(INPUT_KEYDOWN, (int)wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT : 0, -1, -1);

                return FALSE;
            }
        }

        case WM_KEYDOWN:
        {
            this->_BuildEvent(INPUT_KEYDOWN, (int)wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT : 0, -1, -1);
            return FALSE;
        }

        case WM_SYSKEYUP:
        {
            this->_BuildEvent(INPUT_KEYUP, (int)wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
            return FALSE;
        }

        case WM_KEYUP:
        {
            this->_BuildEvent(INPUT_KEYUP, (int)wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
            return FALSE;
        }

        case WM_LBUTTONDOWN:
        {
            SetFocus(hwnd);
            this->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_LBUTTONDBLCLK:
        {
            SetFocus(hwnd);
            this->_BuildEvent(INPUT_MOUSECLICK, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_MBUTTONDOWN:
        {
            SetFocus(hwnd);
            this->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_RBUTTONDOWN:
        {
            SetFocus(hwnd);
            this->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_MBUTTONDBLCLK:
        {
            this->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_RBUTTONDBLCLK:
        {
            this->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_LBUTTONUP:
        {
            this->_BuildEvent(INPUT_MBUTTONUP, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_MBUTTONUP:
        {
            this->_BuildEvent(INPUT_MBUTTONUP, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_RBUTTONUP:
        {
            this->_BuildEvent(INPUT_MBUTTONUP, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_MOUSEMOVE:
        {
            this->_BuildEvent(INPUT_MOUSEMOVE, 0, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
        {
            this->_BuildEvent(INPUT_MOUSEWHEEL, short(HIWORD(wparam)) / 120, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
            return FALSE;
        }

        case WM_SIZE:
        {
            if (hwnd && wparam == SIZE_RESTORED)
            {
                this->_Resize(LOWORD(lparam), HIWORD(lparam));
            }
        } break;

        case WM_SYSCOMMAND:
        {
            if (wparam == SC_CLOSE)
            {
                if (this->System_GetState(HGE_EXITFUNC) && !this->System_GetState(HGE_EXITFUNC)()) return FALSE;

                this->bActive = false;

                return DefWindowProc(hwnd, msg, wparam, lparam);
            }
            else if (wparam == SC_KEYMENU)
            {
                return true;
            }
        } break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void HGE::_Resize(int width, int height)
{
    if (this->System_GetState(HGE_HWNDPARENT))
    {
        this->System_SetState(HGE_SCREENWIDTH, width);
        this->System_SetState(HGE_SCREENHEIGHT, height);
    }

    //d3dppW.BackBufferWidth = width;
    //d3dppW.BackBufferHeight = height;

    //_SetProjectionMatrix(width, height);
    //_GfxRestore();
}
