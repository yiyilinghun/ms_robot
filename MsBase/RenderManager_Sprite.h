#pragma once

class RenderManager;
class RenderSprite
{
public:
    RenderSprite(RenderManager* xRenderManager, HTEXTURE tex, Single x, Single y, Single w, Single h);
    RenderSprite(const RenderSprite &spr);
    ~RenderSprite();

    DINLINE void RenderTexture(LONG x, LONG y, DWORD dwColor = 0xFFFFFFFF) { return RenderTexture((Single)x, (Single)y, dwColor); }
    void RenderTexture(Single x, Single y, DWORD dwColor = 0xFFFFFFFF);
    DINLINE void RenderTexture(LONG x, LONG y, LONG w, LONG h, Single zw, Single zh, DWORD dwColor = 0xFFFFFFFF) { return RenderTexture((Single)x, (Single)y, (Single)w, (Single)h, zw, zh, dwColor); }
    void RenderTexture(Single x, Single y, Single w, Single h, Single zw = -1.0f, Single zh = -1.0f, DWORD dwColor = 0xFFFFFFFF);
    void Render(Single x, Single y);
    void Render(Single x, Single y, Single w, Single h);
    void RenderEx(Single x, Single y, Single rot, Single hscale = 1.0f, Single vscale = 0.0f);
    void RenderStretch(Single x1, Single y1, Single x2, Single y2);
    void Render4V(Single x0, Single y0, Single x1, Single y1, Single x2, Single y2, Single x3, Single y3);

    void SetTexture(HTEXTURE tex);
    void SetTextureRect(Single x, Single y, Single w, Single h, Boolean adjSize = true);
    void SetColor(DWORD dwColor) { m_Color = dwColor; }
    void ClearColor() { m_Color = 0xFFFFFFFF; }
    void SetZ(Single z, Int32 i = -1);
    void SetBlendMode(Int32 blend) { quad.blend = blend; }
    void SetHotSpot(Single x, Single y) { hotX = x; hotY = y; }
    void SetFlip(Boolean bX, Boolean bY, Boolean bHotSpot = false);

    HTEXTURE    GetTexture() const { return quad.tex; }
    void        GetTextureRect(Single* x, Single* y, Single* w, Single* h) const { *x = tx; *y = ty; *w = width; *h = height; }
    DWORD       GetColor() const { return m_Color; }
    Single      GetZ(Int32 i = 0) const { return quad.v[i].z; }
    Int32       GetBlendMode() const { return quad.blend; }
    void        GetHotSpot(Single* x, Single* y) const { *x = hotX; *y = hotY; }
    void        GetFlip(Boolean *bX, Boolean *bY) const { *bX = bXFlip; *bY = bYFlip; }

    Single      GetWidth() const { return width; }
    Single      GetHeight() const { return height; }

    RenderManagerRect* GetBoundingBox(Single x, Single y, RenderManagerRect *rect) const
    {
        rect->Set(x - hotX, y - hotY, x - hotX + width, y - hotY + height);
        return rect;
    }

    Boolean IsBoundingBoxPickup(Single x, Single y, Single X, Single Y);
    Boolean IsPixelPickup(Int32 xDirIndex, Int32 xFrameIndex, Single x, Single y, Single X, Single Y);
    RenderManagerRect*  GetBoundingBoxEx(Single x, Single y, Single rot, Single hscale, Single vscale, RenderManagerRect *rect) const;

public:
    RenderManager*      m_RenderManager;
    DWORD               m_Color;
    RenderManagerQuad   quad;
    Single              tx, ty, width, height;
    Single              tex_width, tex_height;
    Single              hotX, hotY;
    Boolean             bXFlip, bYFlip, bHSFlip;
};

typedef RenderSprite *LPHGESPRITE;

