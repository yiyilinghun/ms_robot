#pragma once

#pragma pack(push, 1)
struct WPixel { union { WORD m_Color/*16bit*/; struct { WORD Blue : 5/*5bit*/; WORD Green : 6/*6bit*/; WORD Red : 5/*5bit*/; }S_TEMP; }; DWORD MakeARGB(DWORD A) { DWORD R = (m_Color << 8 & 0xf80000); DWORD G = (m_Color << 5 & 0x7fc00); DWORD B = ((m_Color << 3 | (m_Color & 7)) & 0x3ff); DWORD dwColor = (A << 24) + R + G + B; return dwColor; }; operator DWORD() const { return (m_Color << 5 & 0x7fc00) | (m_Color << 8 & 0xf80000) | ((m_Color << 3 | (m_Color & 7)) & 0x3ff); } };
struct WPixels { WPixel m_Palette[256]; };
#pragma pack(pop)

class RenderManager;
class WasDecoder
{
public:
    WasDecoder(RenderManager* xRenderManager);
    ~WasDecoder();

    void SetColor(DWORD dwColor);
    DWORD GetColor();
    void ClearColor();
    Boolean AnalysisHead(MsMemoryStream& xStream, Int32 xKey);
    HTEXTURE ExeOneTex(MsMemoryStream& xStream, Int32 xKey, Int32* xFrameOffsets, WPixels& xPalette, Boolean IsSaveFile);
    MsList<HTEXTURE> ExeSomeTex(MsMemoryStream& xStream, Int32 xKey, Int32* xFrameOffsets, WPixels& xPalette, Boolean IsSaveFile);
    MsList<HTEXTURE> Analysis(MsMemoryStream& xStream, Int32 xKey, Boolean IsSaveFile);
    void Load(MsMemoryStream& xStream, Int32 xKey, Boolean IsFromFile = False, Boolean IsSaveFile = False);
    GameSprite* GetRenderSprite();
    GameSprite* GetGameSprite(Int32 xNextIndex, Int32 xDirIndex);

    void GetRenderRect(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, RenderManagerRect& xOutRect);
    void GetRenderCenter(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, RenderManagerPoint& xCenterPoint);

    Boolean RenderNoHot(Int64 xNow, Int32 X, Int32 Y, DWORD dwColor = 0xFFFFFFFF) { return RenderNoHot(xNow, (Single)X, (Single)Y, dwColor); }
    Boolean RenderNoHot(Int64 xNow, Single X, Single Y, DWORD dwColor = 0xFFFFFFFF);

    Boolean Render(Int64 xNow, Int32 X, Int32 Y, DWORD dwColor = 0xFFFFFFFF) { return Render(xNow, (Single)X, (Single)Y, dwColor); }
    Boolean Render(Int64 xNow, Single X, Single Y, DWORD dwColor = 0xFFFFFFFF);

    Boolean RenderNoHotWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 X, Int32 Y, DWORD dwColor = 0xFFFFFFFF) { return RenderNoHotWithIndex(xNow, xNextIndex, xDirIndex, (Single)X, (Single)Y, dwColor); }
    Boolean RenderNoHotWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, DWORD dwColor = 0xFFFFFFFF);
    Boolean RenderWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 X, Int32 Y, DWORD dwColor = 0xFFFFFFFF) { return RenderWithIndex(xNow, xNextIndex, xDirIndex, (Single)X, (Single)Y, dwColor); }
    Boolean RenderWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, DWORD dwColor = 0xFFFFFFFF);

    Boolean RenderWithSizeRatio(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, Single W, Single H, Single ZW = -1.0f, Single ZH = -1.0f, DWORD dwColor = 0xFFFFFFFF);

    Boolean RenderWithSize(Int64 xNow, Int32 X, Int32 Y, Int32 W, Int32 H, DWORD dwColor = 0xFFFFFFFF) { return RenderWithSize(xNow, (Single)X, (Single)Y, (Single)W, (Single)H, dwColor); }
    Boolean RenderWithSize(Int64 xNow, Single X, Single Y, Single W, Single H, DWORD dwColor = 0xFFFFFFFF);

    Boolean CheckPixelPickup(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y);
    Boolean IsBoundingBoxPickup(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y);

    void GetRenderRect(Int64 xNow, Int32 xNextIndex, RenderManagerRect& xOutRect);
    void GetRenderCenter(Int64 xNow, Int32 xNextIndex, RenderManagerPoint& xCenterPoint);
    Boolean CheckPixelPickup(Int64 xNow, Int32 xNextIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y);
    Boolean IsBoundingBoxPickup(Int64 xNow, Int32 xNextIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y);

    Boolean CopyDataFromFrameIndex(LPDWORD lpData, Int32 xFrameIndex);

    void ToGif(mstr xGifFile, WORD xSpeed = 30, DWORD xTransparentColor = 1);

    Boolean m_IsIgnoreCenter = False;
    Int32 m_ReferenceCount;
public:

    void ParseOne(
        MsMemoryStream& xStream,
        LPDWORD lpTextureData,
        Int32 xTextureWidth,
        WPixels& xPalette,
        Int32 xSpriteIndex,
        Int32 xFrameIndex,
        Int32 xFrameOffset,
        Int32* xLineOffsets,
        DWORD xFrameWidth,
        DWORD xFrameHeight,
        Boolean& IsInterlacedgh
    );


    void ParseSome(
        MsMemoryStream& xStream,
        LPDWORD lpTextureData,
        Int32 xTextureWidth,
        WPixels& xPalette,
        Int32 xFrameOffset,
        Int32* xLineOffsets,
        DWORD xFrameWidth,
        DWORD xFrameHeight,
        Boolean& IsInterlacedgh
    );

    RenderManager* m_RenderManager;
    Boolean m_IsSomeSprite;
    MsList<GameSprite*> m_ListGameSprite;
    GameSprite* m_GameSprite;

    WORD m_ImageHeaderSize;

    MsList<Int32> m_ListDelays;

    WORD m_DirectionCount;
    WORD m_FrameCount;
    short m_CenterX;
    short m_CenterY;
    WORD m_Width;
    WORD m_Height;
    Int32* m_ListCenterX;
    Int32* m_ListCenterY;
};

class AtuoWas
{
public:
    WasDecoder* m_WasDecoder = nullptr;
    Single m_WasFPSSpeed = 1.0f;
    Int64 m_RenderUpTime = MsBase::Now();
    Int32 m_NextIndex = 0;
    Boolean m_IsIgnoreCenter = False;
    Boolean m_IsForceZoom = False;
    Boolean m_KeepClearly = True;
    Single m_PosX = 0.0f;
    Single m_PosY = 0.0f;
    Single m_RenderWidth = -1.0f;
    Single m_RenderHeight = -1.0f;
    Single GetWidth()
    {
        if (m_IsForceZoom && m_RenderWidth >= 0.0f)
        {
            if (m_KeepClearly) { return (Single)(Int32)m_RenderWidth; }
            return m_RenderWidth;
        }
        else if (m_WasDecoder)
        {
            return (Single)m_WasDecoder->m_Width;
        }
        else
        {
            if (m_KeepClearly) { return (Single)(Int32)m_RenderWidth; }
            return m_RenderWidth;
        }
    }

    Single GetHeight()
    {
        if (m_IsForceZoom && m_RenderHeight >= 0.0f)
        {
            if (m_KeepClearly) { return (Single)(Int32)m_RenderHeight; }
            return m_RenderHeight;
        }
        else if (m_WasDecoder)
        {
            return (Single)m_WasDecoder->m_Height;
        }
        else
        {
            if (m_KeepClearly) { return (Single)(Int32)m_RenderHeight; }
            return m_RenderHeight;
        }
    }

    void Render(Int64 xNow, DWORD dwColor = 0xFFFFFFFF)
    {
        if (m_WasDecoder)
        {
            if (m_RenderUpTime + (Int64)((1000.0f / (m_WasFPSSpeed * WAS_FPS)) * 10000.0f) < xNow)
            {
                m_RenderUpTime = xNow;
                m_NextIndex++;
            }

            if (m_KeepClearly)
            {
                m_PosX = (Single)(Int32)m_PosX;
                m_PosY = (Single)(Int32)m_PosY;
            }

            m_WasDecoder->m_IsIgnoreCenter = m_IsIgnoreCenter;
            if (m_WasDecoder->RenderWithSizeRatio(xNow, m_NextIndex, 0,
                (Single)m_PosX,
                (Single)m_PosY,
                -1.0f, -1.0f,
                this->GetWidth(), this->GetHeight(), dwColor))
            {
                m_NextIndex = INVALID_NID;
            }
        }
    }
};

struct ScenePosWas
{
    bool operator==(ScenePosWas xPosWas)
    {
        if (xPosWas.m_WasDecoder == m_WasDecoder && xPosWas.X == X && xPosWas.Y == Y && xPosWas.m_AnimationNextIndex == m_AnimationNextIndex)
        {
            return True;
        }
        return false;
    }

    Int32 GetActionNextIndex(Int64 xNow)
    {
        if (m_RenderUpTime + (Int64)((1000.0f / (m_ActionSpeed * WAS_FPS)) * 10000.0f) < xNow)
        {
            m_AnimationNextIndex++;
            m_RenderUpTime = xNow;
        }
        return m_AnimationNextIndex;
    }

    DWORD m_RenderColor;
    Int64 m_RenderUpTime;
    WasDecoder* m_WasDecoder;
    Int32 m_AnimationNextIndex;
    Single m_ActionSpeed;
    Single X;
    Single Y;
};

struct ObjPosWas
{
    bool operator==(ObjPosWas xPosWas)
    {
        if (xPosWas.m_WasDecoder == m_WasDecoder && xPosWas.m_IsHeadPoint == m_IsHeadPoint && xPosWas.m_AnimationNextIndex == m_AnimationNextIndex)
        {
            return True;
        }
        return false;
    }

    Int32 GetActionNextIndex(Int64 xNow)
    {
        if (m_RenderUpTime + (Int64)((1000.0f / (m_ActionSpeed * WAS_FPS)) * 10000.0f) < xNow)
        {
            m_AnimationNextIndex++;
            m_RenderUpTime = xNow;
        }
        return m_AnimationNextIndex;
    }

    DWORD m_RenderColor;
    Int64 m_RenderUpTime;
    WasDecoder* m_WasDecoder;
    Int32 m_AnimationNextIndex;
    Single m_ActionSpeed;
    //Boolean m_IsSolePoint;
    Boolean m_IsHeadPoint;
};