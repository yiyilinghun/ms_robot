#include "Precompiled.h"

RenderSprite::RenderSprite(RenderManager* xRenderManager, HTEXTURE texture, Single texx, Single texy, Single w, Single h)
    : m_RenderManager(xRenderManager)
{
    m_Color = 0xFFFFFFFF;
    Single texx1, texy1, texx2, texy2;

    //hge = hgeCreate(HGE_VERSION);

    tx = texx; ty = texy;
    width = w; height = h;

    if (texture)
    {
        tex_width = (Single)m_RenderManager->Texture_GetWidth(texture);
        tex_height = (Single)m_RenderManager->Texture_GetHeight(texture);
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

RenderSprite::RenderSprite(const RenderSprite &spr)
{
    memcpy(this, &spr, sizeof(RenderSprite));
}

RenderSprite::~RenderSprite()
{
    ;
}

void RenderSprite::Render(Single x, Single y)
{
    Single tempx1, tempy1, tempx2, tempy2;

    tempx1 = x - hotX;
    tempy1 = y - hotY;
    tempx2 = x + width - hotX;
    tempy2 = y + height - hotY;

    quad.v[0].x = tempx1; quad.v[0].y = tempy1;
    quad.v[1].x = tempx2; quad.v[1].y = tempy1;
    quad.v[2].x = tempx2; quad.v[2].y = tempy2;
    quad.v[3].x = tempx1; quad.v[3].y = tempy2;

    m_RenderManager->Gfx_RenderQuad(&quad);
}

void RenderSprite::RenderTexture(Single x, Single y, DWORD dwColor)
{
    Single xTempTx = tx;
    Single xTempTy = ty;

    Single xTempW = xTempTx + width;
    Single xTempH = xTempTy + height;
    if (xTempW > tex_width) { xTempW = tex_width; }
    if (xTempH > tex_height) { xTempH = tex_height; }

    RECT xRect = { (LONG)xTempTx, (LONG)xTempTy, (LONG)(xTempW), (LONG)(xTempH) };
    //if (m_IsIgnoreCenter)
    //{
    //    D3DXVECTOR3 xCenter(0.0f, 0.0f, 0.0f);
    //    D3DXVECTOR3 xPosition(x, y, 0.0f);
    //    m_RenderManager->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, 1.0f, 1.0f, dwColor);
    //}
    //else
    //{
    D3DXVECTOR3 xCenter(hotX, hotY, 0.0f);
    D3DXVECTOR3 xPosition(x, y, 0.0f);
    m_RenderManager->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, 1.0f, 1.0f, dwColor);
    //}
}

void RenderSprite::RenderTexture(Single x, Single y, Single w, Single h, Single zw, Single zh, DWORD dwColor)
{
    Single xTempTx = tx;
    Single xTempTy = ty;

    if (w < 0.0f) { w = width; }
    if (h < 0.0f) { h = height; }

    if (w > width) { zw = w / width; }
    else if (zw < 0.0f) { zw = 1.0f; }
    else { zw /= w; }

    if (h > height) { zh = h / height; }
    else if (zh < 0.0f) { zh = 1.0f; }
    else { zh /= h; }

    Single xTempW = xTempTx + w;
    Single xTempH = xTempTy + h;
    //if (xTempW > tex_width) { xTempW = tex_width; }
    //if (xTempH > tex_height) { xTempH = tex_height; }

    RECT xRect = { (LONG)xTempTx, (LONG)xTempTy, (LONG)(xTempW), (LONG)(xTempH) };
    //if (m_IsIgnoreCenter)
    //{
    //    D3DXVECTOR3 xCenter(0.0f, 0.0f, 0.0f);
    //    D3DXVECTOR3 xPosition(x, y, 0.0f);
    //    m_RenderManager->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, zw, zh, dwColor);
    //}
    //else
    //{
    D3DXVECTOR3 xCenter(hotX, hotY, 0.0f);
    D3DXVECTOR3 xPosition(x, y, 0.0f);
    m_RenderManager->Gfx_RenderTexture(quad.tex, xRect, xCenter, xPosition, zw, zh, dwColor);
    //}
}

void RenderSprite::Render(Single x, Single y, Single w, Single h)
{
    Single tempx1, tempy1, tempx2, tempy2;

    tempx1 = x - hotX;
    tempy1 = y - hotY;
    tempx2 = x + w - hotX;
    tempy2 = y + h - hotY;

    quad.v[0].x = tempx1; quad.v[0].y = tempy1;
    quad.v[1].x = tempx2; quad.v[1].y = tempy1;
    quad.v[2].x = tempx2; quad.v[2].y = tempy2;
    quad.v[3].x = tempx1; quad.v[3].y = tempy2;

    m_RenderManager->Gfx_RenderQuad(&quad);
}


void RenderSprite::RenderEx(Single x, Single y, Single rot, Single hscale, Single vscale)
{
    Single tx1, ty1, tx2, ty2;
    Single sint, cost;

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

    m_RenderManager->Gfx_RenderQuad(&quad);
}


void RenderSprite::RenderStretch(Single x1, Single y1, Single x2, Single y2)
{
    quad.v[0].x = x1; quad.v[0].y = y1;
    quad.v[1].x = x2; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x1; quad.v[3].y = y2;

    m_RenderManager->Gfx_RenderQuad(&quad);
}


void RenderSprite::Render4V(Single x0, Single y0, Single x1, Single y1, Single x2, Single y2, Single x3, Single y3)
{
    quad.v[0].x = x0; quad.v[0].y = y0;
    quad.v[1].x = x1; quad.v[1].y = y1;
    quad.v[2].x = x2; quad.v[2].y = y2;
    quad.v[3].x = x3; quad.v[3].y = y3;

    m_RenderManager->Gfx_RenderQuad(&quad);
}


RenderManagerRect* RenderSprite::GetBoundingBoxEx(Single x, Single y, Single rot, Single hscale, Single vscale, RenderManagerRect* rect) const
{
    Single tx1, ty1, tx2, ty2;
    Single sint, cost;

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

void RenderSprite::SetFlip(Boolean bX, Boolean bY, Boolean bHotSpot)
{
    //Single tx, ty;

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


void RenderSprite::SetTexture(HTEXTURE tex)
{
    Single tx1, ty1, tx2, ty2;
    Single tw, th;

    quad.tex = tex;

    if (tex)
    {
        tw = (Single)m_RenderManager->Texture_GetWidth(tex);
        th = (Single)m_RenderManager->Texture_GetHeight(tex);
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


void RenderSprite::SetTextureRect(Single x, Single y, Single w, Single h, Boolean adjSize)
{
    Single tx1, ty1, tx2, ty2;
    Boolean bX, bY, bHS;

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


//void RenderSprite::SetColor(DWORD col, Int32 i)
//{
//    if (i != -1)
//        quad.v[i].col = col;
//    else
//        quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = col;
//}

void RenderSprite::SetZ(Single z, Int32 i)
{
    if (i != -1)
        quad.v[i].z = z;
    else
        quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = z;
}

Boolean RenderSprite::IsBoundingBoxPickup(Single x, Single y, Single X, Single Y)
{
    RenderManagerRect rect(
        X - hotX,
        Y - hotY,
        X - hotX + width,
        Y - hotY + height
    );
    return rect.Contains(x, y);
}

Boolean RenderSprite::IsPixelPickup(Int32 xDirIndex, Int32 xFrameIndex, Single x, Single y, Single X, Single Y)
{
    if (this->IsBoundingBoxPickup(x, y, X, Y))
    {
        Int32 xOffsetX = (Int32)(xFrameIndex * width);
        Int32 xOffsetY = (Int32)(xDirIndex * height);

        xOffsetX += (Int32)abs(x - (X - hotX));
        xOffsetY += (Int32)abs(y - (Y - hotY));

        LPDWORD lpData = m_RenderManager->Texture_Lock(quad.tex);
        Int32 xWidth = m_RenderManager->Texture_GetWidth(quad.tex);
        DWORD xPixel = lpData[xOffsetY * xWidth + xOffsetX];
        m_RenderManager->Texture_Unlock(quad.tex);
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
