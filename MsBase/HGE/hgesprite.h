/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeSprite helper class header
*/


#ifndef HGESPRITE_H
#define HGESPRITE_H


#include "hge.h"
#include "hgerect.h"

#include <stdio.h>
#include <string.h>

/*
** HGE Sprite class
*/
class hgeSprite
{
public:
    hgeSprite(HGE* hge, HTEXTURE tex, float x, float y, float w, float h);
    hgeSprite(HGE* hge, const hgeSprite &spr);
    ~hgeSprite() { /*hge->Release();*/ }


    void        RenderTexture(float x, float y, DWORD dwColor = 0xFFFFFFFF);
    void        RenderTexture(float x, float y, float w, float h, float zw = -1.0f, float zh = -1.0f, DWORD dwColor = 0xFFFFFFFF);
    void        Render(float x, float y, bool xx);
    void        Render(float x, float y, float w, float h, bool xx);
    void        RenderEx(float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f);
    void        RenderStretch(float x1, float y1, float x2, float y2);
    void        Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

    void        SetTexture(HTEXTURE tex);
    void        SetTextureRect(float x, float y, float w, float h, bool adjSize = true);
    //void        SetColor(DWORD col, int i = -1);
    void        SetColor(DWORD dwColor) { m_Color = dwColor; }
    void        ClearColor() { m_Color = 0xFFFFFFFF; }
    void        SetZ(float z, int i = -1);
    void        SetBlendMode(int blend) { quad.blend = blend; }
    void        SetHotSpot(float x, float y) { hotX = x; hotY = y; }
    void        SetFlip(bool bX, bool bY, bool bHotSpot = false);

    HTEXTURE    GetTexture() const { return quad.tex; }
    void        GetTextureRect(float *x, float *y, float *w, float *h) const { *x = tx; *y = ty; *w = width; *h = height; }
    //DWORD       GetColor(int i = 0) const { return quad.v[i].col; }
    DWORD       GetColor() const { return m_Color; }
    float       GetZ(int i = 0) const { return quad.v[i].z; }
    int         GetBlendMode() const { return quad.blend; }
    void        GetHotSpot(float *x, float *y) const { *x = hotX; *y = hotY; }
    void        GetFlip(bool *bX, bool *bY) const { *bX = bXFlip; *bY = bYFlip; }

    float       GetWidth() const { return width; }
    float       GetHeight() const { return height; }
    hgeRect*    GetBoundingBox(float x, float y, hgeRect *rect) const { rect->Set(x - hotX, y - hotY, x - hotX + width, y - hotY + height); return rect; }
    bool        IsBoundingBoxPickup(float x, float y, float X, float Y);
    bool        IsPixelPickup(int xDirIndex, int xFrameIndex, float x, float y, float X, float Y);
    hgeRect*    GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale, hgeRect *rect) const;

protected:
    hgeSprite();
    friend class GameSprite;

    HGE*        m_hge;
    DWORD       m_Color;
    hgeQuad     quad;
    float       tx, ty, width, height;
    float       tex_width, tex_height;
    float       hotX, hotY;
    bool        bXFlip, bYFlip, bHSFlip;
};

typedef hgeSprite *LPHGESPRITE;

#endif
