/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Common core implementation header
*/


#pragma once

#include "..\core\hge_plugin.h"

#include <windows.h>

#include <stdio.h>

#include "d3d9.h"
#include "d3dx9.h"

#if defined(_WIN64)
#pragma comment(lib, ".\\Lib\\x64\\d3d9.lib")
#pragma comment(lib, ".\\Lib\\x64\\d3dx9.lib")
#else
#pragma comment(lib, ".\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, ".\\Lib\\x86\\d3dx9.lib")
#endif

#define D3DFVF_HGEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUFFER_SIZE 4000

struct CRenderTargetList
{
    int					width;
    int					height;
    IDirect3DTexture9*	pTex;
    IDirect3DSurface9*	pDepth;
    CRenderTargetList*	next;
};

struct CTextureList
{
    HTEXTURE			tex;
    int					width;
    int					height;
    CTextureList*		next;
};

/*
** HGE Plugin implementation
*/
class HGE_DX : public HGE_Plugin
{
public:
    virtual	void		CALL	Release();

    virtual bool		CALL	System_Initiate(HWND hWnd);

    virtual bool		CALL	Gfx_BeginScene(HTARGET target = 0);
    virtual void		CALL	Gfx_EndScene();
    virtual void		CALL	Gfx_Clear(DWORD color);
    virtual void		CALL	Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color = 0xFFFFFFFF, float z = 0.5f);
    virtual void		CALL	Gfx_RenderTriple(const hgeTriple *triple);
    virtual void		CALL	Gfx_RenderQuad(const hgeQuad *quad);
    virtual hgeVertex*	CALL	Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim);
    virtual void		CALL	Gfx_FinishBatch(int nprim);
    virtual void		CALL	Gfx_SetClipping(int x = 0, int y = 0, int w = 0, int h = 0);
    virtual void		CALL	Gfx_SetTransform(float x = 0, float y = 0, float dx = 0, float dy = 0, float rot = 0, float hscale = 0, float vscale = 0);

    virtual HSHADER		CALL	Shader_Create(const char *filename, const char *function = "ps_main", const char *profile = "ps_2_0");
    virtual void		CALL	Shader_Free(HSHADER shader);
    virtual void		CALL	Gfx_SetShader(HSHADER shader);

    virtual HTARGET		CALL	Target_Create(int width, int height, bool zbuffer);
    virtual void		CALL	Target_Free(HTARGET target);
    virtual HTEXTURE	CALL	Target_GetTexture(HTARGET target);

    virtual HTEXTURE	CALL	Texture_Create(int width, int height);
    virtual HTEXTURE	CALL	Texture_LoadFromFile(const char* filename, DWORD size = 0, bool bMipmap = false);
    virtual HTEXTURE	CALL	Texture_LoadFromMemory(void* data, DWORD size, bool bMipmap = false);
    virtual void		CALL	Texture_Free(HTEXTURE tex);
    virtual int			CALL	Texture_GetWidth(HTEXTURE tex, bool bOriginal = false);
    virtual int			CALL	Texture_GetHeight(HTEXTURE tex, bool bOriginal = false);
    virtual DWORD*		CALL	Texture_Lock(HTEXTURE tex, bool bReadOnly = true, int left = 0, int top = 0, int width = 0, int height = 0);
    virtual void		CALL	Texture_Unlock(HTEXTURE tex);

    virtual bool        CALL    Font_Create(D3DXFONT_DESCA* xD3dFont, LPD3DXFONT* lpID3DXFont);

    static  HGE_DX*             _Interface_Get(HGE *lpHGE);

private:
    HGE_DX(HGE *lpHGE);

    bool _GfxInit();
    void _GfxDone();
    bool _GfxRestore();
    void _AdjustWindow();
    void _Resize(int width, int height);
    bool _init_lost();
    int	_format_id(D3DFORMAT fmt);
    void _render_batch(bool bEndScene = false);
    void _SetBlendMode(int blend);
    void _SetProjectionMatrix(int width, int height);

    // Graphics
    D3DPRESENT_PARAMETERS*  d3dpp;

    D3DPRESENT_PARAMETERS   d3dppW;
    RECT					rectW;
    LONG					styleW;

    D3DPRESENT_PARAMETERS   d3dppFS;
    RECT					rectFS;
    LONG					styleFS;

    IDirect3D9*				pD3D;
    IDirect3DDevice9*		pD3DDevice;
    IDirect3DVertexBuffer9*	pVB;
    IDirect3DIndexBuffer9*	pIB;

    IDirect3DSurface9*		pScreenSurf;
    IDirect3DSurface9*		pScreenDepth;
    CRenderTargetList*		pTargets;
    CRenderTargetList*		pCurTarget;

    D3DXMATRIX				matView;
    D3DXMATRIX				matProj;

    CTextureList*			textures;
    hgeVertex*				VertArray;

    int						nPrim;
    int						CurPrimType;
    int						CurBlendMode;
    HTEXTURE				CurTexture;
    HTEXTURE				CurShader;

    char				szError[256];
    void				_PostError(char *error);
};

extern HGE* g_lpHGE;
