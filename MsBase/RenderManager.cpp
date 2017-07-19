#include "Precompiled.h"
#include "RenderManager.h"
#pragma comment(lib, "Imm32.lib")
//#pragma comment(lib, R"(..\MsBase\HGE\Lib\x64\dsound.lib)")
//#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "..\\MsBase\\HGE\\Lib\\x64\\dxguid.lib")
//#pragma comment(lib, "d3d10.lib")
//#pragma comment(lib, "..\\MsBase\\HGE\\Lib\\x64\\DxErr.lib")
//#pragma comment(lib, "..\\MsBase\\HGE\\Lib\\x64\\DxErr.lib")
//#pragma comment(lib, "..\\MsBase\\HGE\\Lib\\x64\\dxguid.lib")

//d3d10.lib
//d3dx10d.lib
//dxerr.lib
//dxguid.lib

#include "dsound.h"
#include "CWaveFile.h"
//#include "dmsuic.h"

RenderManager* ms_lpTempRenderManager = nullptr;
HCURSOR RenderManager::ms_RenderCursor = nullptr;
D3DFORMAT RenderManager::ms_Format = D3DFMT_UNKNOWN;
Boolean RenderManager::ms_IsRestart = False;
//Boolean RenderManager::ms_IsReLogin = False;

RenderManager::RenderManager(mstr xUnitName, MsIOCPManager* xMsIOCPManager)
    : IocpUnit(xUnitName, xMsIOCPManager)
    , pD3D(nullptr)
    , pD3DDevice(nullptr)
    , m_D3dPresentParameters(nullptr)
    , pVB(nullptr)
    , pIB(nullptr)
    , m_FPS(INVALID_NID)
    , m_TempFPS(INVALID_NID)
    , m_MouseInGame(False)

    , pTargets(nullptr)
    , pCurTarget(nullptr)
    , pScreenSurf(nullptr)
    , pScreenDepth(nullptr)
    , m_lpSprite(nullptr)
    , VertArray(nullptr)
    , textures(nullptr)
    , m_CurrentSpeed(1.0f)
    , m_RenderCursorUpTime(MsBase::Now())
    , m_CanEdit_RenderTexture(nullptr)
    , m_CanEdit_RenderSurface(nullptr)
    , m_IsGDIDraw(False)
    //, m_IsDetach(False)
    , m_IsClose(False)
    , m_MousePos({ 0,0 })
{
    DEF_IOCPUNIT_TYPE(RenderManager);

    m_WndName = MsBase::M2W(xUnitName);
    bSilent = False;
    streams = 0;

    hInstance = ::GetModuleHandle(nullptr);

    m_StateHWND.m_hWindow = nullptr;

    m_IsWndFocus = False;

    hSearch = 0;
    m_Res = 0;

    Xpos = Ypos = 0.0f;
    bCaptured = False;

    nPowerStatus = HGEPWR_UNSUPPORTED;
    hKrnl32 = nullptr;
    lpfnGetSystemPowerStatus = nullptr;

    ::GetModuleFileNameA(::GetModuleHandle(nullptr), m_szAppPath, sizeof(m_szAppPath));

    Int32 i;
    for (i = (Int32)strlen(m_szAppPath) - 1; i > 0; i--) if (m_szAppPath[i] == '\\') break;
    m_szAppPath[i + 1] = 0;
}

void RenderManager::OnAttach()
{
    //m_IsDetach = False;
}

//void RenderManager::OnDetach()
//{
//    m_IsDetach = True;
//}

Boolean RenderManager::TickOnce()
{
    if (m_IsClose) { return False; }

    this->LogicTick();

    MSG xMsg;
    while (::PeekMessage(&xMsg, nullptr, 0, 0, PM_REMOVE))
    {
        if (xMsg.message == WM_QUIT || xMsg.message == WM_CLOSE)
        {
            this->Release();
            return False;
        }
        ::TranslateMessage(&xMsg);
        ::DispatchMessage(&xMsg);
    }
    _UpdateMouse();

    if (m_IsLost) { return True; }

    this->Render();
    this->System_CheckUpdateCursor();

    if (MsBase::TimeSpanCheck(m_LastStatisticFPSTime, 1000))
    {
        m_FPS = m_TempFPS;
        m_TempFPS = 0;
    }
    m_TempFPS++;

    return True;
}

Int32 RenderManager::GetFPS() { return m_FPS; }

Boolean RenderManager::_System_Initiate(LPCTSTR szWndName, HICON hIcon)
{
    ms_lpTempRenderManager = this;
    //m_hParent = nullptr;

    _SoundInit();
    m_IMECharCount = 0;

    if (m_StateHWND.m_hWindow) { return True; }

    AssertLog("RenderManager Started..");
    AssertLog("RenderManager version: %X.%X", HGE_VERSION >> 8, HGE_VERSION & 0xFF);
    SYSTEMTIME xTempTM;
    GetLocalTime(&xTempTM);
    AssertLog("Date: %02d.%02d.%d, %02d:%02d:%02d", xTempTM.wDay, xTempTM.wMonth, xTempTM.wYear, xTempTM.wHour, xTempTM.wMinute, xTempTM.wSecond);
    AssertLog("Application: %s", MsBase::W2M(szWndName).c_str());
    MEMORYSTATUS xMemST;
    ::GlobalMemoryStatus(&xMemST);
    AssertLog("Memory: %ldK total, %ldK free", xMemST.dwTotalPhys / 1024L, xMemST.dwAvailPhys / 1024L);

    m_WndName = szWndName;
    WNDCLASS xWinClass;
    xWinClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    xWinClass.lpfnWndProc = WindowProc;
    xWinClass.cbClsExtra = 0;
    xWinClass.cbWndExtra = 0;
    xWinClass.hInstance = hInstance;
    xWinClass.hCursor = nullptr;
    xWinClass.hbrBackground = nullptr;
    xWinClass.lpszMenuName = nullptr;
    xWinClass.lpszClassName = m_WndName.c_str();
    xWinClass.hIcon = hIcon;

    if (!::RegisterClassW(&xWinClass)) { AssertLog("Can't register window class"); return False; }

    RECT xTempWndRect = { 0, 0, m_ScreenWidth, m_ScreenHeight };

    if (this->m_StateBoolean.m_ReadyFullScreen != 0 && this->m_StateBoolean.m_ReadyFullScreen != 1)
    {
        this->m_StateBoolean.m_ReadyFullScreen = this->m_StateBoolean.m_bFullScreen;
    }
    if (m_StateBoolean.m_ReadyFullScreen)
    {
        m_dwExStyle = WS_EX_TOPMOST;
        m_WndStyle = WS_POPUP;
    }
    else
    {
        m_WndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        RECT WorkAreaRect;
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
        xTempWndRect.left = (WorkAreaRect.right - WorkAreaRect.left - m_ScreenWidth) / 2;
        xTempWndRect.top = (WorkAreaRect.bottom - WorkAreaRect.top - m_ScreenHeight) / 2;
        xTempWndRect.right = xTempWndRect.left + m_ScreenWidth;
        xTempWndRect.bottom = xTempWndRect.top + m_ScreenHeight;
    }

    m_StateHWND.m_hWindow = CreateWindowExW(m_dwExStyle, m_WndName.c_str(), m_WndName.c_str(), m_WndStyle,
        xTempWndRect.left, xTempWndRect.top, xTempWndRect.right - xTempWndRect.left, xTempWndRect.bottom - xTempWndRect.top,
        nullptr, nullptr, hInstance, nullptr);

    RECT xClientRect;
    ::GetClientRect(m_StateHWND.m_hWindow, &xClientRect);
    m_ClientWidth = xClientRect.right - xClientRect.left;
    m_ClientHeight = xClientRect.bottom - xClientRect.top;
    //FFORMAT(xTempBuff, "%d*%d", xTempWndRect.right - xTempWndRect.left, xTempWndRect.bottom - xTempWndRect.top);
    //::MessageBoxA(m_StateHWND.m_hWindow, xTempBuff.c_str(), "", MB_OK);

    if (!m_StateHWND.m_hWindow)
    {
        AssertLog("Can't create window");
        return False;
    }

    ::ShowWindow(m_StateHWND.m_hWindow, SW_SHOW);

    ::timeBeginPeriod(1);
    _InitPowerStatus();
    AssertLog("Init done.");

    memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
    m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_BitmapInfo.bmiHeader.biWidth = m_ClientWidth;
    m_BitmapInfo.bmiHeader.biHeight = -m_ClientHeight;
    m_BitmapInfo.bmiHeader.biPlanes = 1;
    m_BitmapInfo.bmiHeader.biBitCount = 32;
    m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
    m_BitmapInfo.bmiHeader.biSizeImage = m_ClientWidth * m_ClientHeight * sizeof(DWORD);

    RenderManager::ms_IsRestart = False;
    return _GfxInit();
}

void RenderManager::Gfx_Clear(DWORD color)
{
    if (pCurTarget)
    {
        if (pCurTarget->pDepth)
            pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
        else
            pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0);
    }
    else
    {
        if (m_StateBoolean.m_bZBuffer)
            pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
        else
            pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0);
    }
}

void RenderManager::Gfx_SetClipping(Int32 x, Int32 y, Int32 w, Int32 h)
{
    D3DVIEWPORT9 vp;
    Int32 scr_width, scr_height;

    if (!pCurTarget)
    {
        scr_width = m_ClientWidth;
        scr_height = m_ClientHeight;
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
    D3DXMatrixOrthoOffCenterLH(&tmp, (Single)vp.X, (Single)(vp.X + vp.Width), -((Single)(vp.Y + vp.Height)), -((Single)vp.Y), vp.MinZ, vp.MaxZ);
    D3DXMatrixMultiply(&matProj, &matProj, &tmp);
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void RenderManager::Gfx_SetTransform(Single x, Single y, Single dx, Single dy, Single rot, Single hscale, Single vscale)
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

Boolean RenderManager::Gfx_BeginScene(HTARGET targ)
{
    CRenderTargetList *target = (CRenderTargetList *)(ULONG_PTR)targ;
    LPDIRECT3DSURFACE9 pSurf = 0, pDepth = 0;

    HRESULT hr = pD3DDevice->TestCooperativeLevel();

    if (hr == D3DERR_DEVICELOST)
    {
        return False;
    }
    else if (hr == D3DERR_DEVICENOTRESET)
    {
        D3DDISPLAYMODE Mode;
        if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode)) || Mode.Format == D3DFMT_UNKNOWN)
        {
            AssertLog("Can't determine desktop video mode");
            return False;
        }

        m_D3dPresentParametersWindowed.BackBufferFormat = Mode.Format;

        if (_format_id(Mode.Format) < 4)
        {
            m_StateInt.m_nScreenBPP = 16;
        }
        else
        {
            m_StateInt.m_nScreenBPP = 32;
        }

        ms_IsRestart = True;
        return False;
    }


    if (VertArray)
    {
        AssertLog("Gfx_BeginScene: Scene is already being rendered");
        return False;
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
            AssertLog("Gfx_BeginScene: Can't set render target");
            return False;
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
            if (m_StateBoolean.m_bZBuffer)
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            }
            else
            {
                pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
            }

            _SetProjectionMatrix(m_ClientWidth, m_ClientHeight);
        }

        pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
        D3DXMatrixIdentity(&matView);
        pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

        pCurTarget = target;
    }

    pD3DDevice->BeginScene();
    pVB->Lock(0, 0, (VOID**)&VertArray, 0);

    m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);

    return True;
}

void RenderManager::Gfx_EndScene()
{
    m_lpSprite->End();

    m_lpSprite->Flush();

    _render_batch(True);

    pD3DDevice->EndScene();

    if (!pCurTarget)
    {
        if (FAILED(pD3DDevice->Present(nullptr, nullptr, m_StateHWND.m_hWindow, nullptr)))
        {
            AssertLog("设备丢失!");
            m_IsLost = True;
        }
    }
}

void RenderManager::Gfx_RenderLine(Single x1, Single y1, Single x2, Single y2, DWORD color, Single z)
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

        Int32 i = nPrim * HGEPRIM_LINES;

        VertArray[i].x = x1; VertArray[i + 1].x = x2;
        VertArray[i].y = y1; VertArray[i + 1].y = y2;
        VertArray[i].z = VertArray[i + 1].z = z;
        VertArray[i].col = VertArray[i + 1].col = color;
        VertArray[i].tx = VertArray[i + 1].tx = 0.0f;
        VertArray[i].ty = VertArray[i + 1].ty = 0.0f;

        nPrim++;
    }
}

void RenderManager::Gfx_RenderTriple(const RenderManagerTriple* xRenderManagerTriple)
{
    if (VertArray)
    {
        if (CurPrimType != HGEPRIM_TRIPLES || nPrim >= VERTEX_BUFFER_SIZE / HGEPRIM_TRIPLES || CurTexture != xRenderManagerTriple->tex || CurBlendMode != xRenderManagerTriple->blend)
        {
            _render_batch();
            CurPrimType = HGEPRIM_TRIPLES;

            if (CurBlendMode != xRenderManagerTriple->blend) _SetBlendMode(xRenderManagerTriple->blend);

            if (xRenderManagerTriple->tex != CurTexture)
            {
                pD3DDevice->SetTexture(0, (LPDIRECT3DTEXTURE9)(ULONG_PTR)xRenderManagerTriple->tex);
                CurTexture = xRenderManagerTriple->tex;
            }
        }

        memcpy(&VertArray[nPrim*HGEPRIM_TRIPLES], xRenderManagerTriple->v, sizeof(RenderManagerVertex) * HGEPRIM_TRIPLES);
        nPrim++;
    }
}

void RenderManager::Gfx_RenderQuad(const RenderManagerQuad *quad)
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

        memcpy(&VertArray[nPrim * HGEPRIM_QUADS], quad->v, sizeof(RenderManagerVertex) * HGEPRIM_QUADS);
        nPrim++;
    }
}

RenderManagerVertex* RenderManager::Gfx_StartBatch(Int32 prim_type, HTEXTURE tex, Int32 blend, Int32 *max_prim)
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

void RenderManager::Gfx_FinishBatch(Int32 nprim)
{
    nPrim = nprim;
}

HTARGET RenderManager::Target_Create(Int32 width, Int32 height, Boolean zbuffer)
{
    CRenderTargetList* pTarget;

    pTarget = NEW CRenderTargetList;
    pTarget->pTex = 0;
    pTarget->pDepth = 0;

    D3DSURFACE_DESC TDesc;

    if (FAILED(D3DXCreateTexture(pD3DDevice, width, height, 1, D3DUSAGE_RENDERTARGET,
        m_D3dPresentParameters->BackBufferFormat, D3DPOOL_DEFAULT, &pTarget->pTex)))
    {
        AssertLog("Can't create render target texture");
        delete pTarget;
        return 0;
    }

    pTarget->pTex->GetLevelDesc(0, &TDesc);
    pTarget->width = TDesc.Width;
    pTarget->height = TDesc.Height;

    if (zbuffer)
    {
        if (FAILED(pD3DDevice->CreateDepthStencilSurface(width, height,
            D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, False, &pTarget->pDepth, nullptr)))
        {
            pTarget->pTex->Release();
            AssertLog("Can't create render target depth buffer");
            delete pTarget;
            return 0;
        }
    }

    pTarget->next = pTargets;
    pTargets = pTarget;

    return (HTARGET)pTarget;
}

void RenderManager::Target_Free(HTARGET target)
{
    CRenderTargetList *pTarget = pTargets, *pPrevTarget = nullptr;

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

HTEXTURE RenderManager::Target_GetTexture(HTARGET target)
{
    CRenderTargetList *targ = (CRenderTargetList*)(ULONG_PTR)target;
    if (target) return (HTEXTURE)targ->pTex;
    else return 0;
}


Boolean RenderManager::BeginScene(DWORD color)
{
    Boolean ret = False;
    if (m_IsGDIDraw)
    {
        this->RenderTexture_Begin();
        this->RenderTexture_Clear(color);
        ret = True;
    }
    else
    {
        ret = this->Gfx_BeginScene();
        this->Gfx_Clear(color);
    }
    return ret;
}

void RenderManager::EndScene()
{
    if (m_IsGDIDraw)
    {
        ::SetDIBitsToDevice(::GetDC(m_StateHWND.m_hWindow), 0, 0, m_ClientWidth, m_ClientHeight, 0, 0, 0, m_ClientHeight, this->RenderTexture_End(), &m_BitmapInfo, DIB_RGB_COLORS);
        this->RenderTexture_UnLock();
    }
    else
    {
        this->Gfx_EndScene();
    }
}

void RenderManager::PushRenderTarget()
{
    this->RenderTexture_Create();

    //pD3DDevice->GetRenderTarget(0, &m_RenderBakBuff);
    //((LPDIRECT3DTEXTURE9)m_RenderTexture)->GetSurfaceLevel(0, &m_RenderSurface);
    //((LPDIRECT3DTEXTURE9)m_CanEdit_RenderTexture)->GetSurfaceLevel(0, &m_CanEdit_RenderSurface);
    //pD3DDevice->SetRenderTarget(0, m_RenderSurface);

}

void RenderManager::PopRenderTarget()
{
    pD3DDevice->SetRenderTarget(0, m_RenderBakBuff);
}

void RenderManager::RenderTexture_Create()
{
    //if (!m_RenderTexture && !m_CanEdit_RenderTexture)
    //{
    //    if (FAILED(D3DXCreateTexture(pD3DDevice, m_ScreenWidth, m_ScreenHeight,
    //        1,                      // Mip levels
    //        D3DUSAGE_RENDERTARGET,  // Usage
    //        D3DFMT_A8R8G8B8,        // Format
    //        D3DPOOL_DEFAULT,        // Memory pool
    //        &m_RenderTexture)))
    //    {
    //        AssertLog("Can't create texture");
    //        return;
    //    }

    //    if (FAILED(D3DXCreateTexture(pD3DDevice, m_ScreenWidth, m_ScreenHeight,
    //        1,                      // Mip levels
    //        0,                      // Usage
    //        D3DFMT_A8R8G8B8,        // Format
    //        D3DPOOL_SYSTEMMEM,      // Memory pool
    //        &m_CanEdit_RenderTexture)))
    //    {
    //        AssertLog("Can't create texture");
    //        return;
    //    }
    //}
}

void RenderManager::RenderTexture_Begin()
{
    //if (m_RenderTexture)
    //{
    //    pD3DDevice->BeginScene();
    //    m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
    //}
}

void RenderManager::RenderTexture_Clear(DWORD color)
{
    pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1.0f, 0);
}

LPDWORD RenderManager::RenderTexture_End()
{
    //m_lpSprite->End();
    //pD3DDevice->EndScene();
    //((LPDIRECT3DTEXTURE9)m_CanEdit_RenderTexture)->GetSurfaceLevel(0, &m_CanEdit_RenderSurface);
    //if (pD3DDevice->GetRenderTargetData(m_RenderSurface, m_CanEdit_RenderSurface) == D3D_OK)
    //{
    //    D3DLOCKED_RECT lockbits;
    //    if (m_CanEdit_RenderSurface->LockRect(&lockbits, nullptr, D3DLOCK_READONLY) == D3D_OK)
    //    {
    //        return (LPDWORD)lockbits.pBits;
    //    }
    //}
    return nullptr;
}

void RenderManager::RenderTexture_UnLock()
{
    m_CanEdit_RenderSurface->UnlockRect();
    SAFE_RELEASE(m_CanEdit_RenderSurface);
}

Boolean RenderManager::Get_GDI_Render()
{
    return m_IsGDIDraw;
}

void RenderManager::Set_GDI_Render(Boolean Is_GDI_Draw)
{
    if (m_IsGDIDraw && !Is_GDI_Draw)
    {
        this->PopRenderTarget();
    }

    if (!m_IsGDIDraw && Is_GDI_Draw)
    {
        this->PushRenderTarget();
    }
    m_IsGDIDraw = Is_GDI_Draw;
}

HTEXTURE RenderManager::Texture_Create(Int32 width, Int32 height)
{
    LPDIRECT3DTEXTURE9 pTex;
    CTextureList* texItem;

    if (FAILED(D3DXCreateTexture(pD3DDevice, width, height,
        1,                  // Mip levels
        0,                  // Usage
        D3DFMT_A8R8G8B8,    // Format
        D3DPOOL_MANAGED,    // Memory pool
        &pTex)))
    {
        AssertLog("Can't create texture");
        return NULL;
    }

    texItem = NEW CTextureList;
    texItem->tex = (HTEXTURE)pTex;
    texItem->width = width;
    texItem->height = height;
    texItem->next = textures;
    textures = texItem;

    return (HTEXTURE)pTex;
}

HTEXTURE RenderManager::Texture_LoadFromFile(LPCSTR filename, OUT Int32& width, OUT Int32& height, Boolean bMipmap)
{
    HANDLE hFile = ::CreateFileA(filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    DWORD size = ::GetFileSize(hFile, nullptr);
    LPBYTE data = NEW BYTE[size];
    ::ReadFile(hFile, data, size, &size, nullptr);
    HTEXTURE xTex = this->Texture_LoadFromMemory(data, size, width, height, bMipmap);
    SAFE_DELETE_ARRAY(data);
    ::CloseHandle(hFile);
    return xTex;
}


HTEXTURE RenderManager::Texture_LoadFromMemory(LPVOID data, DWORD size, OUT Int32& width, OUT Int32& height, Boolean bMipmap)
{
    DWORD _size = size;
    CTextureList *texItem;

    D3DFORMAT fmt1, fmt2;
    LPDIRECT3DTEXTURE9 pTex;
    D3DXIMAGE_INFO info;

    if (*(LPDWORD)data == 0x20534444) // Compressed DDS format magic number
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
        &info, nullptr,
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
            &info, nullptr,
            &pTex)))
        {
            AssertLog("Can't create texture");
            return NULL;
        }
    }

    width = info.Width;
    height = info.Height;

    texItem = NEW CTextureList;
    texItem->tex = (HTEXTURE)pTex;
    texItem->width = width;
    texItem->height = height;
    texItem->next = textures;
    textures = texItem;

    return (HTEXTURE)pTex;
}

void RenderManager::Texture_SaveToFile(HTEXTURE tex, LPCSTR filename)
{
    ::D3DXSaveTextureToFileA(filename, D3DXIFF_PNG, (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex, nullptr);
}

ID3DXBuffer*  RenderManager::Texture_SaveToMemory(HTEXTURE tex)
{
    ID3DXBuffer* pD3DXBuffer = nullptr;
    ::D3DXSaveTextureToFileInMemory(&pD3DXBuffer, D3DXIFF_BMP, (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex, nullptr);
    return pD3DXBuffer;
}

ID3DXBuffer*  RenderManager::Surface_SaveToMemory(PDIRECT3DSURFACE9 xSurface)
{
    ID3DXBuffer* pD3DXBuffer = nullptr;
    ::D3DXSaveSurfaceToFileInMemory(&pD3DXBuffer, D3DXIFF_BMP, xSurface, nullptr, nullptr);
    return pD3DXBuffer;
}

void RenderManager::Texture_Free(HTEXTURE tex)
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

    if (pTex != nullptr)
    {
        pTex->Release();
    }
}

Int32 RenderManager::Texture_GetWidth(HTEXTURE tex, Boolean bOriginal)
{
    if (!tex) { return 0; }

    D3DSURFACE_DESC TDesc;
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    CTextureList *texItem = textures;
    Int32 width = 0;

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

Int32 RenderManager::Texture_GetHeight(HTEXTURE tex, Boolean bOriginal)
{
    if (!tex) { return 0; }

    D3DSURFACE_DESC TDesc;
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    CTextureList *texItem = textures;
    Int32 height = 0;

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

LPDWORD RenderManager::Texture_Lock(HTEXTURE tex, Boolean bReadOnly, Int32 left, Int32 top, Int32 width, Int32 height)
{
    if (!tex) { return nullptr; }

    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    D3DSURFACE_DESC TDesc;
    D3DLOCKED_RECT TRect;
    RECT region, *prec;
    Int32 flags;

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
        AssertLog("Can't lock texture");
        return 0;
    }

    return (LPDWORD)TRect.pBits;
}


void RenderManager::Texture_Unlock(HTEXTURE tex)
{
    LPDIRECT3DTEXTURE9 pTex = (LPDIRECT3DTEXTURE9)(ULONG_PTR)tex;
    pTex->UnlockRect(0);
}

RenderFont RenderManager::Font_Create(mstr xFontName, UINT xWidth, UINT xHeight, UINT xWeight, DWORD xFontColor, LPCWSTR xFaceName, BOOL xItalic)
{
    //if (xFontName != "默认字体") { return m_DictFont["默认字体"]; }
    if (m_DictFont.ContainsKey(xFontName))
    {
        RenderFont& xRenderFont = m_DictFont[xFontName];
        xRenderFont.m_Color = xFontColor;
        return xRenderFont;
    }

    RenderFont& xRenderFont = m_DictFont[xFontName];
    memset(&xRenderFont, 0, sizeof(xRenderFont));

    xRenderFont.m_GDI_DC = ::GetDC(m_StateHWND.m_hWindow);
    xRenderFont.m_D3DFont = nullptr;
    xRenderFont.m_FontWidth = xWidth;
    xRenderFont.m_FontHeight = xHeight;
    D3DXFONT_DESCW d3dFont;
    memset(&d3dFont, 0, sizeof(d3dFont));
    d3dFont.Height = xHeight;
    d3dFont.Width = xWidth;
    d3dFont.Weight = xWeight;
    d3dFont.Italic = xItalic;
    d3dFont.Quality = PROOF_QUALITY;
    d3dFont.MipLevels = 1;
    d3dFont.PitchAndFamily = FIXED_PITCH;
    d3dFont.CharSet = DEFAULT_CHARSET;
    wcscpy_s(d3dFont.FaceName, xFaceName);

    xRenderFont.m_GDIFont = ::CreateFontW(d3dFont.Height, d3dFont.Width, 0, 0, d3dFont.Weight, d3dFont.Italic, FALSE, FALSE,
        d3dFont.CharSet, d3dFont.OutputPrecision, CLIP_DEFAULT_PRECIS, d3dFont.Quality, d3dFont.PitchAndFamily, d3dFont.FaceName);

    if (FAILED(::D3DXCreateFontIndirectW(pD3DDevice, &d3dFont, &(xRenderFont.m_D3DFont))))
    {
        AssertLog("Can't create font");
    }

    xRenderFont.m_Color = xFontColor;
    return xRenderFont;
}

void RenderManager::Font_Free(RenderFont& xRenderFont)
{
    xRenderFont.m_D3DFont->Release();
    ::DeleteObject(xRenderFont.m_GDIFont);
    //::ReleaseDC((HWND)m_hWindow, xRenderFont.m_GDI_DC);
}

SIZE  RenderManager::Font_GetDrawTextRectW(RenderFont& xFont, std::wstring xStr, Int32 xStrLen)
{
    SIZE xSize;
    if (xStrLen == -1)
    {
        xStrLen = (Int32)xStr.size();
    }
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), xStrLen, &xSize);
    return xSize;
}

SIZE  RenderManager::Font_GetDrawTextRectA(RenderFont& xFont, std::string xStr, Int32 xStrLen)
{
    SIZE xSize;
    if (xStrLen == -1)
    {
        xStrLen = (Int32)xStr.size();
    }
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);
    ::GetTextExtentPoint32A(xFont.m_GDI_DC, xStr.c_str(), xStrLen, &xSize);
    return xSize;
}

Int32 RenderManager::Font_GetDrawMaxTextCountInWidthA(RenderFont& xFont, INT xWidth, std::string xStr)
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

Int32 RenderManager::Font_GetDrawMaxTextCountInWidthW(RenderFont& xFont, INT xWidth, std::wstring xStr, MsList<UIResInfo>& xTempList, Int32 xRowIndex)
{
    LONG xRealWidth = 0;
    Int32 xTempSize = Font_GetDrawMaxTextCountInWidthW(xFont, xWidth, xStr, xRealWidth);
    //Int32 xTempSum = 0;
    FAST_FOREACH(xTempList)
    {
        UIResInfo& xValue = xTempList.Value();
        if (xTempSize > xValue.m_Begin && xTempSize < xValue.m_End)
        {
            xTempSize = xValue.m_Begin;
            if (xValue.m_RenderPosX == INVALID_NID)
            {
                xValue.m_RenderPosX = 0;
                xValue.m_RowIndex = xRowIndex + 1;
            }
        }
        else if (xValue.m_End <= xTempSize)
        {
            if (xValue.m_RenderPosX == INVALID_NID)
            {
                SIZE xSize;
                ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), xValue.m_Begin, &xSize);
                xValue.m_RenderPosX = xSize.cx;
                xValue.m_RowIndex = xRowIndex;
            }
        }
        else
        {
            xValue.m_Begin -= xTempSize;
            xValue.m_End -= xTempSize;
            //return xTempSize;
        }
    }
    return xTempSize;
}

Int32 RenderManager::Font_GetDrawMaxTextCountInWidthW(RenderFont& xFont, INT xWidth, std::wstring xStr, LONG& xRealWidth)
{
    //SIZE xOneSize;
    //SIZE xTestSize;
    //SIZE xMaxSize;
    SIZE xSize;
    INT xStrLen = (INT)xStr.size();
    ::SelectObject(xFont.m_GDI_DC, xFont.m_GDIFont);

    ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), xStrLen, &xSize);
    if (xSize.cx <= xWidth) { xRealWidth = xSize.cx; return xStrLen; }

    //::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), 1, &xOneSize);
    //Int32 xBegin = xWidth / (xOneSize.cx ? xOneSize.cx : 1);

    //::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), xBegin, &xTestSize);
    //if (xTestSize.cx > xWidth)
    //{
    //    for (INT i = xBegin; i > 0; i--)
    //    {
    //        ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), i, &xSize);
    //        if (xSize.cx >= xWidth)
    //        {
    //            if (i >= xStrLen) { return xStrLen; }
    //            else { return i - 1; }
    //        }
    //    }
    //    return 0;
    //}
    //else if (xTestSize.cx < xWidth)
    //{
    for (INT i = 0; i < xStrLen; i++)
    {
        ::GetTextExtentPoint32W(xFont.m_GDI_DC, xStr.c_str(), i, &xSize);
        if (xSize.cx >= xWidth)
        {
            return i - 1;
        }
        xRealWidth = xSize.cx;
    }
    return xStrLen;
    //}
    //else
    //{
    //    return xBegin;
    //}
}

Int32 RenderManager::Font_GetDrawMaxTextCountInWidthDescA(RenderFont& xFont, INT xWidth, std::string xStr)
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

Int32 RenderManager::Font_GetDrawMaxTextCountInWidthDescW(RenderFont& xFont, INT xWidth, std::wstring xStr)
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

Int32 RenderManager::Font_DrawTextW(RenderFont& xFont, std::wstring xStr, LONG x1, LONG y1)
{
    SIZE xSize = this->Font_GetDrawTextRectW(xFont, xStr, -1);
    return this->Font_DrawTextW(xFont, xStr, x1, y1, xSize.cx, xSize.cy);
}

Int32 RenderManager::Font_DrawTextA(RenderFont& xFont, std::string xStr, LONG x1, LONG y1)
{
    SIZE xSize = this->Font_GetDrawTextRectA(xFont, xStr, -1);
    return this->Font_DrawTextA(xFont, xStr, x1, y1, xSize.cx, xSize.cy);
}

Int32 RenderManager::Font_DrawTextA(RenderFont& xFont, std::string xStr, LONG x1, LONG y1, LONG w, LONG h)
{
    if (xStr.size() == 0) { return 0; }
    RECT xRect = { x1, y1, x1 + w, y1 + h };
    switch (xFont.m_Effect)
    {
        case 1:
        {
            if (xFont.m_IsStroke)
            {
                RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 };
                xFont.m_D3DFont->DrawTextA(m_lpSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor);

                RECT xRect_1 = { xRect.left - 1, xRect.top - 1, xRect.right - 1 ,xRect.bottom - 1 };
                xFont.m_D3DFont->DrawTextA(m_lpSprite, xStr.c_str(), -1, &xRect_1, xFont.m_Format, xFont.m_StrokeColor);
            }
            return xFont.m_D3DFont->DrawTextA(m_lpSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }break;

        case 0:
        default:
        {
            if (xFont.m_IsStroke)
            {
                RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 };
                xFont.m_D3DFont->DrawTextA(m_lpSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor);
            }
            return xFont.m_D3DFont->DrawTextA(m_lpSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }
    }
}

Int32 RenderManager::Font_DrawTextW(RenderFont& xFont, std::wstring xStr, LONG x1, LONG y1, LONG w, LONG h)
{
    if (xStr.size() == 0) { return 0; }
    RECT xRect = { x1, y1, x1 + w, y1 + h };
    switch (xFont.m_Effect)
    {
        case 1:
        {
            if (xFont.m_IsStroke) { RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 }; xFont.m_D3DFont->DrawTextW(m_lpSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor); }
            return xFont.m_D3DFont->DrawTextW(m_lpSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }break;
        case 0:
        default:
        {
            if (xFont.m_IsStroke) { RECT xRect_0 = { xRect.left + 1, xRect.top + 1, xRect.right + 1,xRect.bottom + 1 }; xFont.m_D3DFont->DrawTextW(m_lpSprite, xStr.c_str(), -1, &xRect_0, xFont.m_Format, xFont.m_StrokeColor); }
            return xFont.m_D3DFont->DrawTextW(m_lpSprite, xStr.c_str(), -1, &xRect, xFont.m_Format, xFont.m_Color);
        }
    }
}

void RenderManager::GoPrevIME()
{
    ::ActivateKeyboardLayout((HKL)HKL_PREV, KLF_ACTIVATE);
}

void RenderManager::GoNextIME()
{
    ::ActivateKeyboardLayout((HKL)HKL_NEXT, KLF_ACTIVATE);
}

void RenderManager::IME_StrDraw(RenderFont& xFont, LONG x, LONG y, LONG xCharH)
{
    for (DWORD i = 0; i < m_IMECharCount; i++)
    {
        FFORMAT(szBuff, "%d:%s\n", i + 1, m_IMECharList[i].c_str());
        this->Font_DrawTextA(xFont, szBuff.c_str(), x, y + xCharH * i);
    }
}

void RenderManager::IME_StrDraw(RenderFont& xFont, HGE_E_POS_TYPE xPosType, LONG dwOffsetX, LONG dwOffsetY, LONG xCharH)
{
    switch (xPosType)
    {
        case HGE_EPT_LEFTTOP:
        {
            return this->IME_StrDraw(xFont, dwOffsetX, dwOffsetY, xCharH);
        }break;

        case HGE_EPT_LEFTBOTTOM:
        {
            LONG dwScreenHeight = m_ScreenHeight;
            LONG dwBackHeight = xCharH * m_IMECharCount;
            return this->IME_StrDraw(xFont, dwOffsetX, dwScreenHeight - dwBackHeight - dwOffsetY, xCharH);
        }break;

        case HGE_EPT_RIGHTTOP:
        {
            LONG dwScreenWidth = m_ClientWidth;
            LONG dwBackWidth = xCharH * m_IMECharCount;
            return this->IME_StrDraw(xFont, dwScreenWidth - dwBackWidth - dwOffsetX, dwOffsetY, xCharH);
        }break;
    }
}

void RenderManager::Gfx_RenderTexture(HTEXTURE hTex, RECT& xRect, D3DXVECTOR3& xCenter, D3DXVECTOR3& xPosition, Single zw, Single zh, DWORD dwColor)
{
    if (zw != 1.0f || zh != 1.0f)
    {
        D3DXMATRIX matScale;
        m_lpSprite->GetTransform(&matScale);
        D3DXMatrixScaling(&matScale, zw, zh, 1.0f);
        m_lpSprite->SetTransform(&matScale);

        Single xW = (Single)(xRect.right - xRect.left);
        xPosition.x *= xW / (xW*zw);

        Single xH = (Single)(xRect.bottom - xRect.top);
        xPosition.y *= xH / (xH*zh);

        m_lpSprite->Draw((LPDIRECT3DTEXTURE9)hTex, &xRect, &xCenter, &xPosition, dwColor);

        D3DXMATRIX default_mat;
        D3DXMatrixScaling(&default_mat, 1.0f, 1.0f, 1.0f);
        m_lpSprite->SetTransform(&default_mat);
    }
    else
    {
        m_lpSprite->Draw((LPDIRECT3DTEXTURE9)hTex, &xRect, &xCenter, &xPosition, dwColor);
    }
}

void RenderManager::_render_batch(Boolean bEndScene)
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

void RenderManager::_SetBlendMode(Int32 xBlend)
{
    if ((xBlend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND))
    {
        if (xBlend & BLEND_ALPHABLEND) pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        else pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    if ((xBlend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE))
    {
        if (xBlend & BLEND_ZWRITE) pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        else pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    }

    if ((xBlend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD))
    {
        if (xBlend & BLEND_COLORADD) pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
        else pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    CurBlendMode = xBlend;
}

void RenderManager::_SetProjectionMatrix(Int32 xWidth, Int32 xHeight)
{
    D3DXMATRIX tmp;
    ::D3DXMatrixScaling(&matProj, 1.0f, -1.0f, 1.0f);
    ::D3DXMatrixTranslation(&tmp, -0.5f, xHeight + 0.5f, 0.0f);
    ::D3DXMatrixMultiply(&matProj, &matProj, &tmp);
    ::D3DXMatrixOrthoOffCenterLH(&tmp, 0, (Single)xWidth, 0, (Single)xHeight, 0.0f, 1.0f);
    ::D3DXMatrixMultiply(&matProj, &matProj, &tmp);
}

Boolean RenderManager::_GfxInit()
{
    static LPCSTR szFormats[] = { "UNKNOWN", "R5G6B5", "X1R5G5B5", "A1R5G5B5", "X8R8G8B8", "A8R8G8B8" };
    D3DADAPTER_IDENTIFIER9 AdID;
    D3DDISPLAYMODE Mode;

    pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
    if (pD3D == nullptr)
    {
        AssertLog("Can't create D3D interface");
        return False;
    }

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












    pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &AdID);
    AssertLog("D3D Driver: %s", AdID.Driver);
    AssertLog("Description: %s", AdID.Description);
    AssertLog("Version: %d.%d.%d.%d", HIWORD(AdID.DriverVersion.HighPart), LOWORD(AdID.DriverVersion.HighPart), HIWORD(AdID.DriverVersion.LowPart), LOWORD(AdID.DriverVersion.LowPart));

    if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode)) || Mode.Format == D3DFMT_UNKNOWN)
    {
        AssertLog("Can't determine desktop video mode");
        return False;
    }

    ZeroMemory(&m_D3dPresentParametersWindowed, sizeof(m_D3dPresentParametersWindowed));

    m_D3dPresentParametersWindowed.BackBufferWidth = m_ClientWidth;
    m_D3dPresentParametersWindowed.BackBufferHeight = m_ClientHeight;
    m_D3dPresentParametersWindowed.BackBufferFormat = Mode.Format;
    m_D3dPresentParametersWindowed.BackBufferCount = 1;
    m_D3dPresentParametersWindowed.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_D3dPresentParametersWindowed.hDeviceWindow = m_StateHWND.m_hWindow;
    if (this->m_StateBoolean.m_ReadyFullScreen != 0 && this->m_StateBoolean.m_ReadyFullScreen != 1)
    {
        this->m_StateBoolean.m_ReadyFullScreen = this->m_StateBoolean.m_bFullScreen;
    }
    //m_D3dPresentParametersWindowed.Windowed = m_StateBoolean.m_ReadyFullScreen;
    m_D3dPresentParametersWindowed.Windowed = True;

    if (m_StateBoolean.m_bVSync)
    {
        m_D3dPresentParametersWindowed.SwapEffect = D3DSWAPEFFECT_COPY;
        m_D3dPresentParametersWindowed.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }
    else
    {
        m_D3dPresentParametersWindowed.SwapEffect = D3DSWAPEFFECT_COPY;
        m_D3dPresentParametersWindowed.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    if (m_StateBoolean.m_bZBuffer)
    {
        m_D3dPresentParametersWindowed.EnableAutoDepthStencil = TRUE;
        m_D3dPresentParametersWindowed.AutoDepthStencilFormat = D3DFMT_D16;
    }

    ZeroMemory(&m_D3dPresentParametersFullScreen, sizeof(m_D3dPresentParametersFullScreen));
    m_D3dPresentParametersFullScreen.BackBufferWidth = m_ClientWidth;
    m_D3dPresentParametersFullScreen.BackBufferHeight = m_ClientHeight;
    m_D3dPresentParametersFullScreen.BackBufferFormat = ms_Format;
    m_D3dPresentParametersFullScreen.BackBufferCount = 1;
    m_D3dPresentParametersFullScreen.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_D3dPresentParametersFullScreen.hDeviceWindow = m_StateHWND.m_hWindow;
    m_D3dPresentParametersFullScreen.Windowed = FALSE;

    m_D3dPresentParametersFullScreen.SwapEffect = D3DSWAPEFFECT_FLIP;
    m_D3dPresentParametersFullScreen.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    if (m_StateBoolean.m_bVSync)
    {
        m_D3dPresentParametersFullScreen.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    }
    else
    {
        m_D3dPresentParametersFullScreen.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    if (m_StateBoolean.m_bZBuffer)
    {
        m_D3dPresentParametersFullScreen.EnableAutoDepthStencil = TRUE;
        m_D3dPresentParametersFullScreen.AutoDepthStencilFormat = D3DFMT_D16;
    }

    m_D3dPresentParameters = &m_D3dPresentParametersWindowed;

    if (_format_id(m_D3dPresentParameters->BackBufferFormat) < 4)
    {
        m_StateInt.m_nScreenBPP = 16;
    }
    else
    {
        m_StateInt.m_nScreenBPP = 32;
    }

    if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_StateHWND.m_hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, m_D3dPresentParameters, &pD3DDevice)))
    {
        AssertLog("Can't create D3D device[%d]", ::GetLastError());
        return False;
    }

    AssertLog("Mode: %d x %d x %s", m_ClientWidth, m_ClientHeight, szFormats[_format_id(ms_Format)]);

    m_List_AdapterMode.Clear();
    D3DCAPS9 caps;
    pD3DDevice->GetDeviceCaps(&caps);
    Int32 xModeCount = pD3D->GetAdapterModeCount(caps.AdapterOrdinal, Mode.Format);
    MsDictionary<mstr, Int32> xDictAdapterMode1;
    for (Int32 i = 0; i < xModeCount; ++i)
    {
        D3DDISPLAYMODE xTempMode;
        pD3D->EnumAdapterModes(caps.AdapterOrdinal, Mode.Format, i, &xTempMode);
        FFORMAT(szBuff, "%d*%d", xTempMode.Width, xTempMode.Height);
        xDictAdapterMode1.Add(szBuff, i);
    }
    MsDictionary<Int32, mstr> xDictAdapterMode2;
    FAST_FOREACH(xDictAdapterMode1)
    {
        xDictAdapterMode2.Add(xDictAdapterMode1.Value(), xDictAdapterMode1.GetKey());
    }

    auto xIt = xDictAdapterMode2.m_Container.end();
    xIt--;
    MsVector<mstr> xMaxTemp;
    MsBase::StringSplit(xIt->second.c_str(), '*', xMaxTemp);
    Int32 xMaxWidth = atoi(xMaxTemp[0].c_str());
    Int32 xMaxHeight = atoi(xMaxTemp[1].c_str());

    FAST_FOREACH(xDictAdapterMode2)
    {
        mstr& x = xDictAdapterMode2.Value();
        MsVector<mstr> xTemp;
        MsBase::StringSplit(x.c_str(), '*', xTemp);
        if (atoi(xTemp[0].c_str()) > xMaxWidth || atoi(xTemp[1].c_str()) > xMaxHeight)
        {
            continue;
        }
        m_List_AdapterMode.Add(x);
    }

    VertArray = 0;
    textures = 0;

    _SetProjectionMatrix(m_ClientWidth, m_ClientHeight);
    D3DXMatrixIdentity(&matView);

    if (FAILED(D3DXCreateSprite(pD3DDevice, &m_lpSprite)))
    {
        AssertLog("Can't create Sprite");
        return False;
    }

    if (!_init_lost()) return False;
    Gfx_Clear(0);

    m_DefaultFont = this->Font_Create("默认字体", 8, 16, 500, 0xFF000000, L"宋体");
    return True;
}

RenderFont RenderManager::GetRenderFont(mstr xFontName)
{
    if (m_DictFont.ContainsKey(xFontName))
    {
        return m_DictFont[xFontName];
    }
    return m_DefaultFont;
}

Int32 RenderManager::_format_id(D3DFORMAT xFormat)
{
    switch (xFormat)
    {
        case D3DFMT_R5G6B5:     return 1;
        case D3DFMT_X1R5G5B5:   return 2;
        case D3DFMT_A1R5G5B5:   return 3;
        case D3DFMT_X8R8G8B8:   return 4;
        case D3DFMT_A8R8G8B8:   return 5;
        default:                return 0;
    }
}

void RenderManager::_GfxDone()
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

    if (m_lpSprite)
    {
        m_lpSprite->Release();
        m_lpSprite = nullptr;
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
        pD3DDevice->SetIndices(nullptr);
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
        pD3DDevice->SetStreamSource(0, nullptr, 0, sizeof(RenderManagerVertex));
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

Boolean RenderManager::_init_lost()
{
    CRenderTargetList *target = pTargets;

    pScreenSurf = 0;
    pScreenDepth = 0;

    if (!pD3DDevice) { return True; }

    pD3DDevice->GetRenderTarget(0, &pScreenSurf);
    pD3DDevice->GetDepthStencilSurface(&pScreenDepth);

    while (target)
    {
        if (target->pTex)
        {
            D3DXCreateTexture(pD3DDevice, target->width, target->height, 1, D3DUSAGE_RENDERTARGET, m_D3dPresentParameters->BackBufferFormat, D3DPOOL_DEFAULT, &target->pTex);
        }
        if (target->pDepth)
        {
            pD3DDevice->CreateDepthStencilSurface(target->width, target->height, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, False, &target->pDepth, nullptr);
        }

        target = target->next;
    }

    if (FAILED(pD3DDevice->CreateVertexBuffer(VERTEX_BUFFER_SIZE * sizeof(RenderManagerVertex),
        D3DUSAGE_WRITEONLY,
        D3DFVF_RENDERMANAGER_VERTEX,
        D3DPOOL_DEFAULT,
        &pVB,
        nullptr)))
    {
        AssertLog("Can't create D3D vertex buffer");
        return False;
    }

    pD3DDevice->SetVertexShader(nullptr);
    pD3DDevice->SetFVF(D3DFVF_RENDERMANAGER_VERTEX);
    pD3DDevice->SetStreamSource(0, pVB, 0, sizeof(RenderManagerVertex));

    if (FAILED(pD3DDevice->CreateIndexBuffer(VERTEX_BUFFER_SIZE * 6 / 4 * sizeof(WORD),
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_DEFAULT,
        &pIB
        , nullptr)))
    {
        AssertLog("Can't create D3D index buffer");
        return False;
    }

    WORD *pIndices, n = 0;
    if (FAILED(pIB->Lock(0, 0, (LPVOID*)&pIndices, 0)))
    {
        AssertLog("Can't lock D3D index buffer");
        return False;
    }

    for (Int32 i = 0; i < VERTEX_BUFFER_SIZE / 4; i++) {
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

    if (m_StateBoolean.m_bTextureFilter)
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

    return True;
}

void RenderManager::Resource_Free(LPVOID res)
{
    SAFE_DELETE_ARRAY(res);
}

UInt32 g_seed = 0;

//void RenderManager::AssertLog(LPSTR error)
//{
//    strcpy_s(szError, error);
//}

HCURSOR RenderManager::System_CreateCursor(Int32 xHotX, Int32 xHotY, Int32 xWidth, Int32 xHeight, LPVOID lpBits)
{
    HBITMAP hBitmap;
    HCURSOR hAlphaCursor = nullptr;

    hBitmap = ::CreateBitmap(xWidth, xHeight, 1, 32, lpBits);

    ICONINFO ii;
    ii.fIcon = False;
    ii.xHotspot = xHotX;
    ii.yHotspot = xHotY;
    ii.hbmMask = hBitmap;
    ii.hbmColor = hBitmap;
    hAlphaCursor = ::CreateIconIndirect(&ii);
    ::DeleteObject(hBitmap);
    return hAlphaCursor;
}

HCURSOR RenderManager::System_LoadCursor(LPCSTR filename)
{
    return ::LoadCursorFromFileA(filename);
}

void RenderManager::System_ChangeCursor(mstr xCursorName)
{
    if (m_TempCurrentCursorName != xCursorName)
    {
        AssertLog("Set[%s]", xCursorName.c_str());
        m_TempCurrentCursorName = xCursorName;
        m_CurrentCursorIndex = 0;
        m_RenderCursorUpTime -= 10000000;
        //System_CheckUpdateCursor();
    }
}

void RenderManager::System_FreeCursor(HCURSOR hCursor)
{
    ::DestroyCursor(hCursor);
}

void RenderManager::System_AddCursor(mstr xCursorName, HCURSOR hCursor, DWORD xCursorWidth)
{
    m_CursorContainer[xCursorName].Add(hCursor);
    if (!m_CursorWidthContainer.ContainsKey(xCursorName))
    {
        m_CursorWidthContainer[xCursorName] = 1;
    }
    m_CursorWidthContainer[xCursorName] = xCursorWidth > m_CursorWidthContainer[xCursorName] ? xCursorWidth : m_CursorWidthContainer[xCursorName];
}

void RenderManager::System_CheckUpdateCursor()
{
    if (m_CursorContainer[m_TempCurrentCursorName].GetCount() > m_CurrentCursorIndex && m_CurrentCursorIndex >= 0)
    {
        Int64 xNow = MsBase::Now();
        if (m_RenderCursorUpTime + (Int64)((1000.0f / (m_CurrentSpeed * WAS_FPS)) * 10000.0f) < xNow)
        {
            m_CurrentCursorIndex++;
            m_CurrentCursorIndex %= m_CursorContainer[m_TempCurrentCursorName].GetCount();
            m_RenderCursorUpTime = xNow;
            ms_RenderCursor = m_CursorContainer[m_TempCurrentCursorName][m_CurrentCursorIndex];
            ::PostMessage(m_StateHWND.m_hWindow, WM_SETCURSOR, INVALID_PUL, INVALID_PNL);
            m_CurrentCursorWidth = m_CursorWidthContainer[m_TempCurrentCursorName];
        }
    }
}
void RenderManager::System_ClearListCursor(mstr xCursorName)
{
    if (m_CursorContainer.ContainsKey(xCursorName))
    {
        FAST_FOREACH(m_CursorContainer[xCursorName])
        {
            this->System_FreeCursor(m_CursorContainer[xCursorName].Value());
        }
        m_CursorContainer[xCursorName].Clear();
    }
}

void RenderManager::System_ClearAllCursor()
{
    FAST_FOREACH(m_CursorContainer)
    {
        this->System_ClearListCursor(m_CursorContainer.GetKey());
    }
    m_CursorContainer.Clear();
}

LPSTR KeyNames[] = {
    "?",
    "Left Mouse Button", "Right Mouse Button", "?", "Middle Mouse Button",
    "?", "?", "?", "Backspace", "Tab", "?", "?", "?", "Enter", "?", "?",
    "Shift", "Ctrl", "Alt", "Pause", "Caps Lock", "?", "?", "?", "?", "?", "?",
    "Escape", "?", "?", "?", "?",
    "Space", "Page Up", "Page Down", "End", "Home",
    "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow",
    "?", "?", "?", "?", "Insert", "Delete", "?",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "?", "?", "?", "?", "?", "?", "?",
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "Left Win", "Right Win", "Application", "?", "?",
    "NumPad 0", "NumPad 1", "NumPad 2", "NumPad 3", "NumPad 4",
    "NumPad 5", "NumPad 6", "NumPad 7", "NumPad 8", "NumPad 9",
    "Multiply", "Add", "?", "Subtract", "Decimal", "Divide",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "Num Lock", "Scroll Lock",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?",
    "Left bracket", "Backslash", "Right bracket", "Apostrophe",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?"
};


Boolean RenderManager::Input_GetKeyState(Int32 key)
{
    return ((::GetKeyState(key) & 0x8000) != 0);
}


char* RenderManager::Input_GetKeyName(Int32 key)
{
    return KeyNames[key];
}


void RenderManager::_UpdateMouse()
{
    RECT xRect;
    ::GetCursorPos(&m_MousePos);
    ::GetClientRect(m_StateHWND.m_hWindow, &xRect);
    ::MapWindowPoints(m_StateHWND.m_hWindow, nullptr, (LPPOINT)&xRect, 2);

    if (bCaptured || (::PtInRect(&xRect, m_MousePos) && ::WindowFromPoint(m_MousePos) == m_StateHWND.m_hWindow))
    {
        m_MouseInGame = True;
    }
    else
    {
        if (m_MouseInGame)
        {
            ::SendMessage(this->m_StateHWND.m_hWindow, WM_MOUSELEAVE, NULL, NULL);
        }
        m_MouseInGame = False;
    }
}



void RenderManager::_InitPowerStatus()
{
    hKrnl32 = LoadLibrary(_T("kernel32.dll"));

    if (hKrnl32 != nullptr)
        lpfnGetSystemPowerStatus = (GetSystemPowerStatusFunc)GetProcAddress(hKrnl32, "GetSystemPowerStatus");

    _UpdatePowerStatus();
}


void RenderManager::_UpdatePowerStatus()
{
    SYSTEM_POWER_STATUS ps;

    if (lpfnGetSystemPowerStatus != nullptr && lpfnGetSystemPowerStatus(&ps))
    {
        if (ps.ACLineStatus == 1)
        {
            nPowerStatus = HGEPWR_AC;
        }
        else if (ps.BatteryFlag < 128)
        {
            nPowerStatus = ps.BatteryLifePercent;
        }
        else
        {
            nPowerStatus = HGEPWR_UNSUPPORTED;
        }
    }
    else
    {
        nPowerStatus = HGEPWR_UNSUPPORTED;
    }
}

void RenderManager::_DonePowerStatus()
{
    if (hKrnl32 != nullptr) FreeLibrary(hKrnl32);
}

Boolean RenderManager::Release()
{
    this->m_MsIOCPManager->m_CloseAll = True;

    FAST_FOREACH(m_DictFont)
    {
        this->Font_Free(m_DictFont.Value());
    }
    m_DictFont.Clear();

    m_List_AdapterMode.Clear();

    //SAFE_RELEASE(m_RenderTexture);
    //SAFE_RELEASE(m_RenderSurface);
    SAFE_RELEASE(m_CanEdit_RenderTexture);
    SAFE_RELEASE(m_CanEdit_RenderSurface);

    if (m_StateHWND.m_hWindow)
    {
        System_Shutdown();
    }

    _GfxDone();
    _SoundDone();

    //HMODULE hDll = ::GetModuleHandleA("D3DX9_43.dll");
    //if (hDll)
    //{
    //    ::FreeLibrary(hDll);
    //}
    //this->Detach();

    return True;
}

void RenderManager::System_Shutdown()
{
    AssertLog("Finishing..");

    timeEndPeriod(1);

    if (hSearch)
    {
        FindClose(hSearch);
        hSearch = 0;
    }

    _DonePowerStatus();

    if (m_StateHWND.m_hWindow)
    {
        DestroyWindow((HWND)m_StateHWND.m_hWindow);
        m_StateHWND.m_hWindow = nullptr;
    }

    if (hInstance) UnregisterClassW(m_WndName.c_str(), hInstance);

    //this->Detach();
    AssertLog("The End.");
}

//void RenderManager::System_Wait()
//{
//    while (!m_IsDetach)
//    {
//        ::Sleep(100);
//    }
//}

//LPSTR RenderManager::System_GetErrorMessage()
//{
//    return szError;
//}

Boolean RenderManager::System_Launch(LPCSTR url)
{
    if ((QWORD)::ShellExecuteA((HWND)m_StateHWND.m_hWindow, nullptr, url, nullptr, nullptr, SW_SHOWMAXIMIZED) > 32)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RenderManager::System_Snapshot(LPCSTR xPath)
{
    CHAR xTempPath[MAX_PATH];
    if (!xPath)
    {
        SHGetSpecialFolderPathA(0, xTempPath, CSIDL_DESKTOPDIRECTORY, 0);
        xPath = xTempPath;
    }

    Int32 i = 0;
    while (true)
    {
        FFORMAT(xBuff, "%s/%08d.png", xPath, i);
        if (MsBase::FilePathExists(xBuff))
        {
            i++;
            continue;
        }
        else
        {
            LPDIRECT3DSURFACE9 pSurf;
            if (pD3DDevice)
            {
                pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
                ::D3DXSaveSurfaceToFileA(xBuff.c_str(), D3DXIFF_PNG, pSurf, nullptr, nullptr);
                pSurf->Release();
            }
            break;
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT xMsg, WPARAM wParam, LPARAM lParam)
{
    return ms_lpTempRenderManager->HGE_WindowProc(hWnd, xMsg, wParam, lParam);
}
LRESULT RenderManager::HGE_WindowProc(HWND hWnd, UINT xMsg, WPARAM wParam, LPARAM lParam)
{
    switch (xMsg)
    {
        case WM_CREATE:
        {
            return False;
        }

        case WM_SYSCHAR:
        {
            if (this->OnKeyEvent(KET_KEY_PASS, (Int32)wParam)) { this->OnKeyEventFree(KET_KEY_PASS, (Int32)wParam); }
            return False;
        }break;
        case WM_CHAR:
        {
            if (this->OnKeyEvent(KET_KEY_PASS, (Int32)wParam)) { this->OnKeyEventFree(KET_KEY_PASS, (Int32)wParam); }
            return False;
        }break;

        //case WM_IME_KEYDOWN:
        //{

        //}

        case WM_IME_NOTIFY:
        {
            if (wParam == IMN_CHANGECANDIDATE)
            {
                HIMC hIMC = ::ImmGetContext(hWnd);
                DWORD dwCount = ::ImmGetCandidateListA(hIMC, 0, nullptr, 0);
                if (dwCount > 0)
                {
                    CHAR xCandidateData[KB_SIZE * 10];
                    if (dwCount > KB_SIZE * 10) { dwCount = KB_SIZE * 10; }
                    LPCANDIDATELIST xCandidate = (LPCANDIDATELIST)xCandidateData;
                    ::ImmGetCandidateListA(hIMC, 0, xCandidate, dwCount);
                    m_IMECharCount = xCandidate->dwCount;
                    if (m_IMECharCount > 9) { m_IMECharCount = 9; }
                    for (DWORD i = 0; i < m_IMECharCount; i++)
                    {
                        m_IMECharList[i] = xCandidateData + xCandidate->dwOffset[i];
                    }
                }
            }
            else if (wParam == 270 || wParam == IMN_SETOPENSTATUS)
            {
                m_IMECharCount = 0;
            }
            else
            {
                //AssertLog("%d---", wParam);
                //m_IMECharCount = 0;
            }
        }break;


        case WM_IME_CHAR:
        {
            if (this->OnKeyEvent(KET_IME_KEY_PASS, (Int32)wParam)) { this->OnKeyEventFree(KET_IME_KEY_PASS, (Int32)wParam); }
        }break;

        case WM_CLOSE:
        case WM_DESTROY:
        {
            if (!ms_IsRestart)
            {
                ::PostQuitMessage(0);
            }
            return False;
        }

        case WM_KILLFOCUS: { m_IsWndFocus = False; }break;
        case WM_SETFOCUS:
        {
            m_IsWndFocus = True;
            if (m_IsLost)
            {
                this->CloseGame(True);
                m_IsLost = False;
            }
        }break;

        case WM_SETCURSOR:
        {
            if (ms_RenderCursor) { ::SetCursor(ms_RenderCursor); return False; }
            else { ::SetCursor(::LoadCursor(nullptr, IDC_ARROW)); return False; }
        }break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            if (m_IsWndFocus)
            {
                if ((Int32)wParam == HGEK_PRINT)
                {
                    this->System_Snapshot();
                }
                if ((Int32)wParam == HGEK_F4 && this->Input_GetKeyState(HGEK_ALT)) { ::PostMessage(hWnd, WM_CLOSE, NULL, NULL); }
                if (this->OnKeyEvent(KET_KEY_DOWN, (Int32)wParam)) { this->OnKeyEventFree(KET_KEY_DOWN, (Int32)wParam); }
            }
            return True;
        }break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            if (m_IsWndFocus)
            {
                if ((Int32)wParam == HGEK_PRINT)
                {
                    this->System_Snapshot();
                }
                //else if ((Int32)wParam == HGEK_CTRL && this->Input_GetKeyState(HGEK_SHIFT)) { this->GoNextIME(); }
                //else if ((Int32)wParam == HGEK_SHIFT && this->Input_GetKeyState(HGEK_CTRL)) { this->GoNextIME(); }

                if (this->OnKeyEvent(KET_KEY_UP, (Int32)wParam)) { this->OnKeyEventFree(KET_KEY_UP, (Int32)wParam); }
            }
            return True;
        }break;

        case WM_LBUTTONDOWN:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_LBUTTONDOWN, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_LBUTTONDOWN, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_LBUTTONUP:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_LBUTTONUP, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_LBUTTONUP, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_LBUTTONDBLCLK:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_LBUTTONDBCLICK, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_LBUTTONDBCLICK, m_MousePos.x, m_MousePos.y); }
            }
        }break;



        case WM_RBUTTONDOWN:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_RBUTTONDOWN, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_RBUTTONDOWN, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_RBUTTONUP:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_RBUTTONUP, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_RBUTTONUP, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_RBUTTONDBLCLK:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_RBUTTONDBCLICK, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_RBUTTONDBCLICK, m_MousePos.x, m_MousePos.y); }
            }
        }break;


        case WM_MBUTTONDOWN:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_MBUTTONDOWN, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_MBUTTONDOWN, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_MBUTTONUP:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_MBUTTONUP, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_MBUTTONUP, m_MousePos.x, m_MousePos.y); }
            }
        }break;
        case WM_MBUTTONDBLCLK:
        {
            if (m_IsWndFocus)
            {
                //::SetFocus(hWnd);
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_MBUTTONDBCLICK, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_MBUTTONDBCLICK, m_MousePos.x, m_MousePos.y); }
            }
        }break;

        case WM_MOUSEMOVE:
        {
            if (m_IsWndFocus)
            {
                m_MouseInGame = True;
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                if (this->OnMouseEvent(MET_MOUSEMOVE, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_MOUSEMOVE, m_MousePos.x, m_MousePos.y); }
            }
        }break;

        case WM_MOUSELEAVE:
        {
            //if (m_IsWndFocus)
            //{
            m_MouseInGame = False;
            m_MousePos.x = INVALID_NID;
            m_MousePos.y = INVALID_NID;
            if (this->OnMouseEvent(MET_MOUSELEAVE, m_MousePos.x, m_MousePos.y)) { this->OnMouseEventFree(MET_MOUSELEAVE, m_MousePos.x, m_MousePos.y); }
            //}
        }break;

        case WM_NCHITTEST:
        {
            LRESULT xRet = ::DefWindowProc(hWnd, xMsg, wParam, lParam);
            if (xRet == HTERROR || xRet == HTCLIENT)
            {
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                ::ScreenToClient(hWnd, &m_MousePos);
                xRet = this->OnNchitTest(m_MousePos.x, m_MousePos.y);
                if (xRet == HTERROR) { xRet = this->OnNchitTestFree(m_MousePos.x, m_MousePos.y); }
                if (xRet == HTERROR)
                {
                    break;
                }
                else
                {
                    return xRet;
                }
            }
            return xRet;
        }break;



        case WM_MOUSEWHEEL:
        {
            if (m_IsWndFocus)
            {
                m_MousePos.x = LOWORDINT(lParam);
                m_MousePos.y = HIWORDINT(lParam);
                this->OnButtonWheel(m_MousePos.x, m_MousePos.y, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
            }
        }break;


        case WM_MOVE:
        {
            //if (m_WndPosType == WPT_MAIN_WND)
            //{
            //    ::GetWindowRect(hWnd, &ms_MainScreenRect);
            //}
            //else if (m_WndPosType == WPT_POS_TOP)
            //{
            //    RenderManager::UpdateDockWnd(m_WndPosType);
            //}
        }break;

        case WM_SIZE:
        {
            //if (ms_lpMainWnd && m_WndPosType == WPT_MAIN_WND)
            //{
            //    ms_lpLeftWnd ? ::ShowWindow(RenderManager::ms_lpLeftWnd->m_StateHWND.m_hWindow, SW_SHOWNORMAL) : NULL;
            //    ms_lpRightWnd ? ::ShowWindow(RenderManager::ms_lpRightWnd->m_StateHWND.m_hWindow, SW_SHOWNORMAL) : NULL;
            //    ms_lpTopWnd ? ::ShowWindow(RenderManager::ms_lpTopWnd->m_StateHWND.m_hWindow, SW_SHOWNORMAL) : NULL;
            //    ms_lpBottomWnd ? ::ShowWindow(RenderManager::ms_lpBottomWnd->m_StateHWND.m_hWindow, SW_SHOWNORMAL) : NULL;
            //}
        } break;

        case WM_SYSCOMMAND:
        {
            if (wParam == SC_CLOSE)
            {
                m_IsWndFocus = False;
                m_IsClose = True;
                ::PostQuitMessage(0);
                return ::DefWindowProc(hWnd, xMsg, wParam, lParam);
            }
            else if (wParam == SC_KEYMENU)
            {
                return True;
            }
        } break;
    }
    return ::DefWindowProc(hWnd, xMsg, wParam, lParam);
}

void RenderManager::CloseGame(Boolean IsRestart)
{
    ::PostMessage(m_StateHWND.m_hWindow, WM_QUIT, 0, 0);
    RenderManager::ms_IsRestart = IsRestart;
}

Boolean RenderManager::IsReadyCloseGame()
{
    return RenderManager::ms_IsRestart;
}
