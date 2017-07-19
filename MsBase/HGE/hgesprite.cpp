/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeSprite helper class implementation
*/


#include "hgesprite.h"
#include <math.h>


//HGE *hgeSprite::hge = 0;


hgeSprite::hgeSprite(HGE* hge, HTEXTURE texture, float texx, float texy, float w, float h)
{
    m_hge = hge;
    m_Color = 0xFFFFFFFF;
    float texx1, texy1, texx2, texy2;

    //hge = hgeCreate(HGE_VERSION);

    tx = texx; ty = texy;
    width = w; height = h;

    if (texture)
    {
        tex_width = (float)m_hge->Texture_GetWidth(texture);
        tex_height = (float)m_hge->Texture_GetHeight(texture);
    }
    else
    {
        tex_width = 1.0f;
        tex_height = 1.0f;
    }

    hotX = 0;
    hotY = 0;
    bXFlip = false;
    bYFlip = false;
    bHSFlip = false;
    quad.tex = texture;

    texx1 = texx / tex_width;
    texy1 = texy / tex_height;
    texx2 = (texx + w) / tex_width;
    texy2 = (texy + h) / tex_height;

    quad.v[0].tx = texx1; quad.v[0].ty = texy1;
    quad.v[1].tx = texx2; quad.v[1].ty = texy1;
    quad.v[2].tx = texx2; quad.v[2].ty = texy2;
    quad.v[3].tx = texx1; quad.v[3].ty = texy2;

    quad.v[0].z =
        quad.v[1].z =
        quad.v[2].z =
        quad.v[3].z = 0.5f;

    quad.v[0].col =
        quad.v[1].col =
        quad.v[2].col =
        quad.v[3].col = 0xffffffff;

    quad.blend = BLEND_DEFAULT;
}

hgeSprite::hgeSprite(HGE* hge, const hgeSprite &spr)
{
    memcpy(this, &spr, sizeof(hgeSprite));
    m_hge = hge;
}

void hgeSprite::Render(float x, float y, bool xx)
{
    float tempx1, tempy1, tempx2, tempy2;

    tempx1 = x - hotX;
    tempy1 = y - hotY;
    tempx2 = x + width - hotX;
    tempy2 = y + height - hotY;

    quad.v[0].x = tempx1; quad.v[0].y = tempy1;
    quad.v[1].x = tempx2; quad.v[1].y = tempy1;
    quad.v[2].x = tempx2; quad.v[2].y = tempy2;
    quad.v[3].x = tempx1; quad.v[3].y = tempy2;

    m_hge->Gfx_RenderQuad(&quad);
}

void hgeSprite::RenderTexture(float x, float y, DWORD dwColor)
{
    float xTempTx = tx;
    float xTempTy = ty;

    //if (hotX < 0.0f)
    //{
    //    x += -hotX;
    //    hotX = 0.0f;
    //}
    //if (hotY < 0.0f)
    //{
    //    y += -hotY;
    //    hotY = 0.0f;
    //}

    //if (x < 0.0f)
    //{
    //    xTempTx += -x;
    //    x = 0;
    //}

    //if (y < 0.0f)
    //{
    //    xTempTy += -y;
    //    y = 0;
    //}

    float xTempW = xTempTx + width;
    float xTempH = xTempTy + height;
    if (xTempW > tex_width) { xTempW = tex_width; }
    if (xTempH > tex_height) { xTempH = tex_height; }

    RECT xRect = { (LONG)xTempTx, (LONG)xTempTy, (LONG)(xTempW), (LONG)(xTempH) };
    D3DXVECTOR3 xCenter(hotX, hotY, 0.0f);
    D3DXVECTOR3 xPosition(x, y, 0.0f);
    m_hge->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, 1.0f, 1.0f, dwColor);
}

void hgeSprite::RenderTexture(float x, float y, float w, float h, float zw, float zh, DWORD dwColor)
{
    float xTempTx = tx;
    float xTempTy = ty;

    if (w < 0.0f) { w = width; }
    if (h < 0.0f) { h = height; }

    if (zw < 0.0f) { zw = 1.0f; }
    else { zw /= w; }
    if (zh < 0.0f) { zh = 1.0f; }
    else { zh /= h; }

    float xTempW = xTempTx + w;
    float xTempH = xTempTy + h;
    if (xTempW > tex_width) { xTempW = tex_width; }
    if (xTempH > tex_height) { xTempH = tex_height; }

    RECT xRect = { (LONG)xTempTx, (LONG)xTempTy, (LONG)(xTempW), (LONG)(xTempH) };
    D3DXVECTOR3 xCenter(hotX, hotY, 0.0f);
    D3DXVECTOR3 xPosition(x, y, 0.0f);
    m_hge->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, zw, zh, dwColor);
}

void hgeSprite::Render(float x, float y, float w, float h, bool xx)
{
    float tempx1, tempy1, tempx2, tempy2;

    tempx1 = x - hotX;
    tempy1 = y - hotY;
    tempx2 = x + w - hotX;
    tempy2 = y + h - hotY;

    quad.v[0].x = tempx1; quad.v[0].y = tempy1;
    quad.v[1].x = tempx2; quad.v[1].y = tempy1;
    quad.v[2].x = tempx2; quad.v[2].y = tempy2;
    quad.v[3].x = tempx1; quad.v[3].y = tempy2;

    m_hge->Gfx_RenderQuad(&quad);
}


void hgeSprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
    float tx1, ty1, tx2, ty2;
    float sint, cost;

    if (vscale == 0) vscale = hscale;

    tx1 = -hotX*hscale;
    ty1 = -hotY*vscale;
    tx2 = (width - hotX)*hscale;
    ty2 = (height - hotY)*vscale;

    if (rot != 0.0f)
    {
        cost = cosf(rot);
        sint = sinf(rot);

        quad.v[0].x = tx1*cost - ty1*sint + x;
        quad.v[0].y = tx1*sint + ty1*cost + y;

        quad.v[1].x = tx2*cost - ty1*sint + x;
        quad.v[1].y = tx2*sint + ty1*cost + y;

        quad.v[2].x = tx2*cost - ty2*sint + x;
        quad.v[2].y = tx2*sint + ty2*cost + y;

        quad.v[3].x = tx1*cost - ty2*sint + x;
        quad.v[3].y = tx1*sint + ty2*cost + y;
    }
    else
    {
        quad.v[0].x = tx1 + x; quad.v[0].y = ty1 + y;
        quad.v[1].x = tx2 + x; quad.v[1].y = ty1 + y;
        quad.v[2].x = tx2 + x; quad.v[2].y = ty2 + y;
        quad.v[3].x = tx1 + x; quad.v[3].y = ty2 + y;
    }

    m_hge->Gfx_RenderQuad(&quad);
}


void hgeSprite::RenderStretch(float x1, float y1, float x2, float y2)
{
    quad.v[0].x = x1; quad.v[0].y = y1;
    quad.v[1].x = x2; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x1; quad.v[3].y = y2;

    m_hge->Gfx_RenderQuad(&quad);
}


void hgeSprite::Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    quad.v[0].x = x0; quad.v[0].y = y0;
    quad.v[1].x = x1; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x3; quad.v[3].y = y3;

    m_hge->Gfx_RenderQuad(&quad);
}


hgeRect* hgeSprite::GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale, hgeRect *rect) const
{
    float tx1, ty1, tx2, ty2;
    float sint, cost;

    rect->Clear();

    tx1 = -hotX*hscale;
    ty1 = -hotY*vscale;
    tx2 = (width - hotX)*hscale;
    ty2 = (height - hotY)*vscale;

    if (rot != 0.0f)
    {
        cost = cosf(rot);
        sint = sinf(rot);

        rect->Encapsulate(tx1*cost - ty1*sint + x, tx1*sint + ty1*cost + y);
        rect->Encapsulate(tx2*cost - ty1*sint + x, tx2*sint + ty1*cost + y);
        rect->Encapsulate(tx2*cost - ty2*sint + x, tx2*sint + ty2*cost + y);
        rect->Encapsulate(tx1*cost - ty2*sint + x, tx1*sint + ty2*cost + y);
    }
    else
    {
        rect->Encapsulate(tx1 + x, ty1 + y);
        rect->Encapsulate(tx2 + x, ty1 + y);
        rect->Encapsulate(tx2 + x, ty2 + y);
        rect->Encapsulate(tx1 + x, ty2 + y);
    }

    return rect;
}

void hgeSprite::SetFlip(bool bX, bool bY, bool bHotSpot)
{
    float tx, ty;

    if (bHSFlip && bXFlip) hotX = width - hotX;
    if (bHSFlip && bYFlip) hotY = height - hotY;

    bHSFlip = bHotSpot;

    if (bHSFlip && bXFlip) hotX = width - hotX;
    if (bHSFlip && bYFlip) hotY = height - hotY;

    if (bX != bXFlip)
    {
        tx = quad.v[0].tx; quad.v[0].tx = quad.v[1].tx; quad.v[1].tx = tx;
        ty = quad.v[0].ty; quad.v[0].ty = quad.v[1].ty; quad.v[1].ty = ty;
        tx = quad.v[3].tx; quad.v[3].tx = quad.v[2].tx; quad.v[2].tx = tx;
        ty = quad.v[3].ty; quad.v[3].ty = quad.v[2].ty; quad.v[2].ty = ty;

        bXFlip = !bXFlip;
    }

    if (bY != bYFlip)
    {
        tx = quad.v[0].tx; quad.v[0].tx = quad.v[3].tx; quad.v[3].tx = tx;
        ty = quad.v[0].ty; quad.v[0].ty = quad.v[3].ty; quad.v[3].ty = ty;
        tx = quad.v[1].tx; quad.v[1].tx = quad.v[2].tx; quad.v[2].tx = tx;
        ty = quad.v[1].ty; quad.v[1].ty = quad.v[2].ty; quad.v[2].ty = ty;

        bYFlip = !bYFlip;
    }
}


void hgeSprite::SetTexture(HTEXTURE tex)
{
    float tx1, ty1, tx2, ty2;
    float tw, th;

    quad.tex = tex;

    if (tex)
    {
        tw = (float)m_hge->Texture_GetWidth(tex);
        th = (float)m_hge->Texture_GetHeight(tex);
    }
    else
    {
        tw = 1.0f;
        th = 1.0f;
    }

    if (tw != tex_width || th != tex_height)
    {
        tx1 = quad.v[0].tx*tex_width;
        ty1 = quad.v[0].ty*tex_height;
        tx2 = quad.v[2].tx*tex_width;
        ty2 = quad.v[2].ty*tex_height;

        tex_width = tw;
        tex_height = th;

        tx1 /= tw; ty1 /= th;
        tx2 /= tw; ty2 /= th;

        quad.v[0].tx = tx1; quad.v[0].ty = ty1;
        quad.v[1].tx = tx2; quad.v[1].ty = ty1;
        quad.v[2].tx = tx2; quad.v[2].ty = ty2;
        quad.v[3].tx = tx1; quad.v[3].ty = ty2;
    }
}


void hgeSprite::SetTextureRect(float x, float y, float w, float h, bool adjSize)
{
    float tx1, ty1, tx2, ty2;
    bool bX, bY, bHS;

    tx = x;
    ty = y;

    if (adjSize)
    {
        width = w;
        height = h;
    }

    tx1 = tx / tex_width; ty1 = ty / tex_height;
    tx2 = (tx + w) / tex_width; ty2 = (ty + h) / tex_height;

    quad.v[0].tx = tx1; quad.v[0].ty = ty1;
    quad.v[1].tx = tx2; quad.v[1].ty = ty1;
    quad.v[2].tx = tx2; quad.v[2].ty = ty2;
    quad.v[3].tx = tx1; quad.v[3].ty = ty2;

    bX = bXFlip; bY = bYFlip; bHS = bHSFlip;
    bXFlip = false; bYFlip = false;
    SetFlip(bX, bY, bHS);
}


//void hgeSprite::SetColor(DWORD col, int i)
//{
//    if (i != -1)
//        quad.v[i].col = col;
//    else
//        quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = col;
//}

void hgeSprite::SetZ(float z, int i)
{
    if (i != -1)
        quad.v[i].z = z;
    else
        quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = z;
}

bool hgeSprite::IsBoundingBoxPickup(float x, float y, float X, float Y)
{
    hgeRect rect(X - hotX, Y - hotY, X - hotX + width, Y - hotY + height);
    return rect.Contains(x, y);
}

bool hgeSprite::IsPixelPickup(int xDirIndex, int xFrameIndex, float x, float y, float X, float Y)
{
    if (this->IsBoundingBoxPickup(x, y, X, Y))
    {
        int xOffsetX = (int)(xFrameIndex * width);
        int xOffsetY = (int)(xDirIndex * height);

        xOffsetX += (int)abs(x - (X - hotX));
        xOffsetY += (int)abs(y - (Y - hotY));

        LPDWORD lpData = m_hge->Texture_Lock(quad.tex);
        int xWidth = m_hge->Texture_GetWidth(quad.tex);
        DWORD xPixel = lpData[xOffsetY * xWidth + xOffsetX];
        m_hge->Texture_Unlock(quad.tex);
        if (xPixel & 0xFF000000)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
