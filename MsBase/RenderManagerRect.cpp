#include "Precompiled.h"

RenderManagerRect::RenderManagerRect(Int32 _x1, Int32 _y1, Int32 _x2, Int32 _y2)
{
    x1 = (Single)_x1;
    y1 = (Single)_y1;
    x2 = (Single)_x2;
    y2 = (Single)_y2;
    bClean = false;
}

RenderManagerRect::RenderManagerRect(Single _x1, Single _y1, Single _x2, Single _y2)
{
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
    bClean = false;
}

RenderManagerRect::RenderManagerRect()
{
    bClean = true;
}

void RenderManagerRect::Clear()
{
    bClean = true;
}

Boolean RenderManagerRect::IsClean() const
{
    return bClean;
}

void RenderManagerRect::Set(Single _x1, Single _y1, Single _x2, Single _y2)
{
    x1 = _x1; x2 = _x2; y1 = _y1; y2 = _y2;
    bClean = false;
}

void RenderManagerRect::SetRadius(Single x, Single y, Single r)
{
    x1 = x - r; x2 = x + r; y1 = y - r; y2 = y + r; bClean = false;
}

void RenderManagerRect::Encapsulate(Single x, Single y)
{
    if (bClean)
    {
        x1 = x2 = x;
        y1 = y2 = y;
        bClean = false;
    }
    else
    {
        if (x < x1) x1 = x;
        if (x > x2) x2 = x;
        if (y < y1) y1 = y;
        if (y > y2) y2 = y;
    }
}

Boolean RenderManagerRect::TestPoint(Single x, Single y) const
{
    if (x >= x1 && x < x2 && y >= y1 && y < y2) return true;

    return false;
}

Boolean RenderManagerRect::Intersect(const RenderManagerRect* rect) const
{
    if (fabs(x1 + x2 - rect->x1 - rect->x2) < (x2 - x1 + rect->x2 - rect->x1))
    {
        if (fabs(y1 + y2 - rect->y1 - rect->y2) < (y2 - y1 + rect->y2 - rect->y1))
        {
            return true;
        }
    }
    return false;
}

Boolean RenderManagerRect::Intersect(const RenderManagerRect& rect) const
{
    return Intersect(&rect);
}

Boolean RenderManagerRect::Contains(Single x, Single y) const
{
    return x >= x1 && x < x2 && y >= y1 && y < y2;
}

Boolean RenderManagerRect::Contains(Int32 x, Int32 y) const
{
    return Contains((Single)x, (Single)y);
}

void RenderManagerRect::SetX(Single _x1)
{
    x1 = _x1;
}

void RenderManagerRect::SetY(Single _y1)
{
    y1 = _y1;
}

void RenderManagerRect::SetX(Int32 _x1)
{
    x1 = (Single)_x1;

}

void RenderManagerRect::SetY(Int32 _y1)
{
    y1 = (Single)_y1;
}

Single RenderManagerRect::GetX() const
{
    return x1;
}

Single RenderManagerRect::GetY() const
{
    return y1;
}

Int32 RenderManagerRect::GetIntX() const
{
    return (Int32)GetX();
}

Int32 RenderManagerRect::GetIntY() const
{
    return (Int32)GetY();
}

Single RenderManagerRect::GetWidth() const
{
    return x2 - x1;
}

Single RenderManagerRect::GetHeight() const
{
    return y2 - y1;
}

Int32 RenderManagerRect::GetIntWidth() const
{
    return (Int32)GetWidth();
}

Int32 RenderManagerRect::GetIntHeight() const
{
    return (Int32)GetHeight();
}

void RenderManagerRect::SetWidth(Single xWidth)
{
    x2 = x1 + xWidth;
}

void RenderManagerRect::SetHeight(Single xHeight)
{
    y2 = y1 + xHeight;
}

void RenderManagerRect::SetWidth(Int32 xWidth)
{
    this->SetWidth((Single)xWidth);
}

void RenderManagerRect::SetHeight(Int32 xHeight)
{
    this->SetHeight((Single)xHeight);
}
