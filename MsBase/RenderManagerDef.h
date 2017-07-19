#pragma once
#include "DX9/d3d9.h"
#include "DX9/d3dx9.h"

// 动画帧数
#define WAS_FPS 10.0f

// WAS纹理最大尺寸
#define WAS_TEXTURE_MAX_SIZE    4096

#define HGE_VERSION 0x190

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif

//enum WND_POS_TYPE
//{
//    WPT_MAIN_WND,
//    WPT_POS_LEFT,
//    WPT_POS_RIGHT,
//    WPT_POS_TOP,
//    WPT_POS_BOTTOM,
//};

enum KEY_EVENT_TYPE
{
    KET_KEY_DOWN,
    KET_KEY_UP,
    KET_KEY_PASS,
    KET_IME_KEY_PASS,
};

enum MOUSE_EVENT_TYPE
{
    MET_LBUTTONDOWN,
    MET_LBUTTONUP,
    MET_LBUTTONDBCLICK,

    MET_RBUTTONDOWN,
    MET_RBUTTONUP,
    MET_RBUTTONDBCLICK,

    MET_MBUTTONDOWN,
    MET_MBUTTONUP,
    MET_MBUTTONDBCLICK,

    MET_MOUSEMOVE,
    MET_MOUSELEAVE,
};

//typedef HWND    HWINDOW;
typedef DWORD   HPLUGIN;
typedef DWORD   HFUNCTION;

typedef QWORD   HTEXTURE;
typedef QWORD   HSHADER;
typedef QWORD   HTARGET;

typedef DWORD   HEFFECT;
typedef DWORD   HMUSIC;
typedef DWORD   HSTREAM;
typedef DWORD   HCHANNEL;

/*
** Hardware color macros
*/
#define ARGB(a,r,g,b)   ((DWORD(a)<<24) + (DWORD(r)<<16) + (DWORD(g)<<8) + DWORD(b))
#define GETA(col)       ((col)>>24)
#define GETR(col)       (((col)>>16) & 0xFF)
#define GETG(col)       (((col)>>8) & 0xFF)
#define GETB(col)       ((col) & 0xFF)
#define SETA(col,a)     (((col) & 0x00FFFFFF) + (DWORD(a)<<24))
#define SETR(col,r)     (((col) & 0xFF00FFFF) + (DWORD(r)<<16))
#define SETG(col,g)     (((col) & 0xFFFF00FF) + (DWORD(g)<<8))
#define SETB(col,b)     (((col) & 0xFFFFFF00) + DWORD(b))

/*
** HGE Blending constants
*/
#define BLEND_COLORADD      1
#define BLEND_COLORMUL      0
#define BLEND_ALPHABLEND    2
#define BLEND_ALPHAADD      0
#define BLEND_ZWRITE        4
#define BLEND_NOZWRITE      0

#define BLEND_DEFAULT       (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z     (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)

struct Shortcutkeys
{
    Boolean m_Ctrl;
    Boolean m_Alt;
    Boolean m_Shift;
    DWORD   m_Key;
};


/*
** Callback protoype used by HGE
*/
typedef Boolean(*RMCALLBACK)();

///*
//** HGE_FPS system state special constants
//*/
//#define HGEFPS_UNLIMITED    0
//#define HGEFPS_VSYNC        -1

/*
** HGE_POWERSTATUS system state special constants
*/
#define HGEPWR_AC           -1
#define HGEPWR_UNSUPPORTED  -2

/*
** HGE Primitive type constants
*/
#define HGEPRIM_LINES   2
#define HGEPRIM_TRIPLES 3
#define HGEPRIM_QUADS   4

struct RenderManagerVertex
{
    Single  x, y;   // screen position    
    Single  z;      // Z-buffer depth 0..1
    DWORD   col;    // color
    Single  tx, ty; // texture coordinates
};


/*
** HGE Triple structure
*/
struct RenderManagerTriple
{
    RenderManagerVertex v[3];
    HTEXTURE            tex;
    Int32               blend;
};

struct RenderManagerQuad
{
    RenderManagerVertex v[4];
    HTEXTURE            tex;
    Int32               blend;
};


/*
** HGE Input Event structure
*/
struct hgeInputEvent
{
    int     type;   // event type
    int     key;    // key code
    int     flags;  // event flags
    int     chr;    // character code
    int     wheel;  // wheel shift
    float   x;      // mouse cursor x-coordinate
    float   y;      // mouse cursor y-coordinate
};

/*
** HGE Input Event type constants
*/
#define INPUT_KEYDOWN       1
#define INPUT_KEYUP         2
#define INPUT_MBUTTONDOWN   3
#define INPUT_MBUTTONUP     4
#define INPUT_MOUSEMOVE     5
#define INPUT_MOUSEWHEEL    6
#define INPUT_MOUSECLICK    7

/*
** HGE Input Event flags
*/
#define HGEINP_SHIFT        1
#define HGEINP_CTRL         2
#define HGEINP_ALT          4
#define HGEINP_CAPSLOCK     8
#define HGEINP_SCROLLLOCK   16
#define HGEINP_NUMLOCK      32
#define HGEINP_REPEAT       64






/*
** HGE Virtual-key codes
*/
#define HGEK_LBUTTON    0x01
#define HGEK_RBUTTON    0x02
#define HGEK_MBUTTON    0x04

#define HGEK_ESCAPE     0x1B
#define HGEK_BACKSPACE  0x08
#define HGEK_TAB        0x09
#define HGEK_ENTER      0x0D
#define HGEK_SPACE      0x20

#define HGEK_SHIFT      0x10
#define HGEK_CTRL       0x11
#define HGEK_ALT        0x12

#define HGEK_LWIN       0x5B
#define HGEK_RWIN       0x5C
#define HGEK_APPS       0x5D

#define HGEK_PAUSE      0x13
#define HGEK_CAPSLOCK   0x14
#define HGEK_NUMLOCK    0x90
#define HGEK_SCROLLLOCK 0x91

#define HGEK_PGUP       0x21
#define HGEK_PGDN       0x22
#define HGEK_HOME       0x24
#define HGEK_END        0x23
#define HGEK_PRINT      0x2C
#define HGEK_INSERT     0x2D
#define HGEK_DELETE     0x2E

#define HGEK_LEFT       0x25
#define HGEK_UP         0x26
#define HGEK_RIGHT      0x27
#define HGEK_DOWN       0x28

#define HGEK_0          0x30
#define HGEK_1          0x31
#define HGEK_2          0x32
#define HGEK_3          0x33
#define HGEK_4          0x34
#define HGEK_5          0x35
#define HGEK_6          0x36
#define HGEK_7          0x37
#define HGEK_8          0x38
#define HGEK_9          0x39

#define HGEK_A          0x41
#define HGEK_B          0x42
#define HGEK_C          0x43
#define HGEK_D          0x44
#define HGEK_E          0x45
#define HGEK_F          0x46
#define HGEK_G          0x47
#define HGEK_H          0x48
#define HGEK_I          0x49
#define HGEK_J          0x4A
#define HGEK_K          0x4B
#define HGEK_L          0x4C
#define HGEK_M          0x4D
#define HGEK_N          0x4E
#define HGEK_O          0x4F
#define HGEK_P          0x50
#define HGEK_Q          0x51
#define HGEK_R          0x52
#define HGEK_S          0x53
#define HGEK_T          0x54
#define HGEK_U          0x55
#define HGEK_V          0x56
#define HGEK_W          0x57
#define HGEK_X          0x58
#define HGEK_Y          0x59
#define HGEK_Z          0x5A

#define HGEK_GRAVE      0xC0
#define HGEK_MINUS      0xBD
#define HGEK_EQUALS     0xBB
#define HGEK_BACKSLASH  0xDC
#define HGEK_LBRACKET   0xDB
#define HGEK_RBRACKET   0xDD
#define HGEK_SEMICOLON  0xBA
#define HGEK_APOSTROPHE 0xDE
#define HGEK_COMMA      0xBC
#define HGEK_PERIOD     0xBE
#define HGEK_SLASH      0xBF

#define HGEK_NUMPAD0    0x60
#define HGEK_NUMPAD1    0x61
#define HGEK_NUMPAD2    0x62
#define HGEK_NUMPAD3    0x63
#define HGEK_NUMPAD4    0x64
#define HGEK_NUMPAD5    0x65
#define HGEK_NUMPAD6    0x66
#define HGEK_NUMPAD7    0x67
#define HGEK_NUMPAD8    0x68
#define HGEK_NUMPAD9    0x69

#define HGEK_MULTIPLY   0x6A
#define HGEK_DIVIDE     0x6F
#define HGEK_ADD        0x6B
#define HGEK_SUBTRACT   0x6D
#define HGEK_DECIMAL    0x6E

#define HGEK_F1         0x70
#define HGEK_F2         0x71
#define HGEK_F3         0x72
#define HGEK_F4         0x73
#define HGEK_F5         0x74
#define HGEK_F6         0x75
#define HGEK_F7         0x76
#define HGEK_F8         0x77
#define HGEK_F9         0x78
#define HGEK_F10        0x79
#define HGEK_F11        0x7A
#define HGEK_F12        0x7B


#define D3DFVF_RENDERMANAGER_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUFFER_SIZE 4000


typedef Single REAL;
#define REAL_MAX        FLT_MAX
#define REAL_MIN        FLT_MIN
#define REAL_TOLERANCE  (FLT_MIN * 100)
#define REAL_EPSILON    1.192092896e-07F        /* FLT_EPSILON */

struct RenderFont
{
    Boolean     m_IsStroke;
    DWORD       m_StrokeColor;
    DWORD       m_Color;
    DWORD       m_Effect;
    DWORD       m_Format;
    LPD3DXFONT  m_D3DFont;
    HDC         m_GDI_DC;
    HFONT       m_GDIFont;
    LONG        m_FontWidth;
    LONG        m_FontHeight;
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

struct CRenderTargetList
{
    Int32               width;
    Int32               height;
    IDirect3DTexture9*  pTex;
    IDirect3DSurface9*  pDepth;
    CRenderTargetList*  next;
};

struct CTextureList
{
    HTEXTURE        tex;
    Int32           width;
    Int32           height;
    CTextureList*   next;
};

struct CResourceList
{
    CHAR            filename[_MAX_PATH];
    CHAR            password[64];
    CResourceList*  next;
};

struct CInputEventList
{
    hgeInputEvent       event;
    CInputEventList*    next;
};

struct CStreamList
{
    HSTREAM         hstream;
    LPVOID          data;
    CStreamList*    next;
};


struct RenderManager_StateBoolean
{
    RenderManager_StateBoolean()
    {
        m_bFullScreen = False; m_bZBuffer = False; m_bDontSuspend = False; m_bVSync = False;
        m_bTextureFilter = True; m_bMusic = True; m_bHideMouse = True; m_bShowSplash = True;
    }
    Int32 m_ReadyFullScreen = 2;
    Int32 m_ReadyMusic = 2;
    Int32 m_ReadySound = 2;

    Boolean m_bFullScreen;
    Boolean m_bMusic;
    Boolean m_bSound;

    Boolean m_bZBuffer;
    Boolean m_bDontSuspend;
    Boolean m_bVSync;
    Boolean m_bTextureFilter;
    Boolean m_bHideMouse;
    Boolean m_bShowSplash;
};

struct RenderManager_StateHWND
{
    RenderManager_StateHWND()
    {
        m_hWindow = nullptr;
    }
    HWND m_hWindow;
    //HWND m_hWindowParent;
};

struct RenderManager_StateInt
{
    RenderManager_StateInt()
    {
        m_nScreenBPP = 32;
        m_nSampleRate = 44100;
        m_nFXVolume = 80;       // (0-100)
        m_nMusVolume = 80;      // (0-100)
        m_nStreamVolume = 80;   // (0-100)
    }
    Int32 m_nScreenBPP;
    Int32 m_nSampleRate;
    Int32 m_nFXVolume;
    Int32 m_nMusVolume;
    Int32 m_nStreamVolume;
};

class RenderManager;
typedef BOOL(WINAPI *GetSystemPowerStatusFunc)(LPSYSTEM_POWER_STATUS);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
//LRESULT CALLBACK _WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
