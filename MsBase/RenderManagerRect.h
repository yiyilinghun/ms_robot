#pragma once

class RenderManagerSize
{
public:
    RenderManagerSize()
    {
        Width = Height = 0.0f;
    }

    RenderManagerSize(IN const RenderManagerSize& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    RenderManagerSize(IN Single width,
        IN Single height)
    {
        Width = width;
        Height = height;
    }

    RenderManagerSize operator+(IN const RenderManagerSize& sz) const
    {
        return RenderManagerSize(Width + sz.Width,
            Height + sz.Height);
    }

    RenderManagerSize operator-(IN const RenderManagerSize& sz) const
    {
        return RenderManagerSize(Width - sz.Width,
            Height - sz.Height);
    }

    BOOL Equals(IN const RenderManagerSize& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0.0f && Height == 0.0f);
    }

public:

    Single Width;
    Single Height;
};

//--------------------------------------------------------------------------
// Represents a dimension in a 2D coordinate system (integer coordinates)
//--------------------------------------------------------------------------

class Size
{
public:
    Size()
    {
        Width = Height = 0;
    }

    Size(IN const Size& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    Size(IN INT width,
        IN INT height)
    {
        Width = width;
        Height = height;
    }

    Size operator+(IN const Size& sz) const
    {
        return Size(Width + sz.Width,
            Height + sz.Height);
    }

    Size operator-(IN const Size& sz) const
    {
        return Size(Width - sz.Width,
            Height - sz.Height);
    }

    BOOL Equals(IN const Size& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0 && Height == 0);
    }

public:

    INT Width;
    INT Height;
};

class RenderManagerPoint
{
public:
    RenderManagerPoint()
    {
        X = Y = 0.0f;
    }

    RenderManagerPoint(IN const RenderManagerPoint &point)
    {
        X = point.X;
        Y = point.Y;
    }

    RenderManagerPoint(IN const RenderManagerSize &size)
    {
        X = size.Width;
        Y = size.Height;
    }

    RenderManagerPoint(IN Single x,
        IN Single y)
    {
        X = x;
        Y = y;
    }

    RenderManagerPoint operator+(IN const RenderManagerPoint& point) const
    {
        return RenderManagerPoint(X + point.X,
            Y + point.Y);
    }

    RenderManagerPoint operator-(IN const RenderManagerPoint& point) const
    {
        return RenderManagerPoint(X - point.X,
            Y - point.Y);
    }

    BOOL Equals(IN const RenderManagerPoint& point)
    {
        return (X == point.X) && (Y == point.Y);
    }

public:

    Single X;
    Single Y;
};

class Point
{
public:
    Point()
    {
        X = Y = 0;
    }

    Point(IN const Point &point)
    {
        X = point.X;
        Y = point.Y;
    }

    Point(IN const Size &size)
    {
        X = size.Width;
        Y = size.Height;
    }

    Point(IN INT x,
        IN INT y)
    {
        X = x;
        Y = y;
    }

    Point operator+(IN const Point& point) const
    {
        return Point(X + point.X,
            Y + point.Y);
    }

    Point operator-(IN const Point& point) const
    {
        return Point(X - point.X,
            Y - point.Y);
    }

    BOOL Equals(IN const Point& point)
    {
        return (X == point.X) && (Y == point.Y);
    }

public:

    INT X;
    INT Y;
};

class RenderManagerRect
{
public:
    Single x1, y1, x2, y2;

    RenderManagerRect(Int32 _x1, Int32 _y1, Int32 _x2, Int32 _y2);
    RenderManagerRect(Single _x1, Single _y1, Single _x2, Single _y2);

    RenderManagerRect();

    void Clear();

    Boolean IsClean() const;

    void Set(Single _x1, Single _y1, Single _x2, Single _y2);

    void SetRadius(Single x, Single y, Single r);

    void Encapsulate(Single x, Single y);

    Boolean TestPoint(Single x, Single y) const;

    Boolean Intersect(const RenderManagerRect* rect) const;
    Boolean Intersect(const RenderManagerRect& rect) const;

    Boolean Contains(Int32 x, Int32 y) const;
    Boolean Contains(Single x, Single y) const;

    void SetX(Single _x1);
    void SetY(Single _y1);

    void SetX(Int32 _x1);
    void SetY(Int32 _y1);

    Single GetX() const;
    Single GetY() const;

    Int32 GetIntX() const;
    Int32 GetIntY() const;

    Single GetWidth() const;
    Single GetHeight() const;

    Int32 GetIntWidth() const;
    Int32 GetIntHeight() const;

    void SetWidth(Single xWidth);
    void SetHeight(Single xHeight);

    void SetWidth(Int32 xWidth);
    void SetHeight(Int32 xHeight);

private:
    Boolean bClean;
};
