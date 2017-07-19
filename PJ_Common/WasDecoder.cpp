#include "Precompiled.h"
#include "gif_write.h"

#define WAS_MAKE_ARGB(c, a) (a << 24) + (c << 8 & 0xf80000) + (c << 5 & 0x7fc00) + (((c << 3 | (c & 7)) & 0x3ff))

WasDecoder::WasDecoder(RenderManager* m_RenderManager)
    : m_RenderManager(m_RenderManager)
    , m_GameSprite(nullptr)
    , m_ReferenceCount(0)
    , m_IsSomeSprite(False)
{
    ;
}

WasDecoder::~WasDecoder()
{
    SAFE_ENUMERATOR_DELETE(m_ListGameSprite);

    SAFE_DELETE(m_GameSprite);

    SAFE_DELETE_ARRAY(m_ListCenterX);
    SAFE_DELETE_ARRAY(m_ListCenterY);
}


void WasDecoder::SetColor(DWORD dwColor)
{
    return m_GameSprite->SetColor(dwColor);
}

DWORD WasDecoder::GetColor()
{
    return m_GameSprite->GetColor();
}

void WasDecoder::ClearColor()
{
    return m_GameSprite->SetColor(0xFFFFFFFF);
}

Boolean WasDecoder::AnalysisHead(MsMemoryStream& xStream, Int32 xKey)
{
    xStream.OutSeek(0);
    //Int32 xOffect = 0;

    // 检查文件头标记
    WORD xFileHeadFlag;
    xStream >> xFileHeadFlag;
    if (xFileHeadFlag == 0x5053)
    {
        //xStream >> m_ImageHeaderSize >> m_DirectionCount >> m_FrameCount >> m_Width >> m_Height >> m_CenterX >> m_CenterY;

        // 读取帧延迟信息
        Int16 xLen = (Int16)(m_ImageHeaderSize - 12);
        if (xLen > 0)
        {
            for (Int32 i = 0; i < xLen; i++)
            {
                Byte xDelays = 0;
                xStream >> xDelays;
                m_ListDelays.Add((Int32)xDelays);
            }
        }

        xStream.OutSeek(0);
        return True;
    }
    return False;
}

HTEXTURE WasDecoder::ExeOneTex(MsMemoryStream& xStream, Int32 xKey, Int32* xFrameOffsets, WPixels& xPalette, Boolean IsSaveFile)
{
    HTEXTURE xTexture = m_RenderManager->Texture_Create(m_Width * m_FrameCount, m_Height * m_DirectionCount);
    if (!xTexture) { return NULL; }

    Int32 xTextureWidth = m_RenderManager->Texture_GetWidth(xTexture);
    Int32 xTextureHeight = m_RenderManager->Texture_GetHeight(xTexture);
    LPDWORD lpTextureData = m_RenderManager->Texture_Lock(xTexture);
    //Int32 xImageIndex = 0;
    Boolean IsInterlacedgh = False;

    // 处理得到图像
    for (Int32 xDirectionIndex = 0; xDirectionIndex < m_DirectionCount; xDirectionIndex++)
    {
        for (Int32 xFrameIndex = 0; xFrameIndex < m_FrameCount; xFrameIndex++)
        {
            Int32 xOffset = xFrameOffsets[(xDirectionIndex * m_FrameCount + xFrameIndex)];
            if (xOffset != 0)
            {
                xStream.OutSeek(xOffset + m_ImageHeaderSize + 4);
                Int32 xFrameWidth;
                Int32 xFrameHeight;

                xStream >> (m_ListCenterX[xDirectionIndex * m_FrameCount + xFrameIndex])
                    >> (m_ListCenterY[xDirectionIndex * m_FrameCount + xFrameIndex])
                    >> xFrameWidth >> xFrameHeight;

                Int32* xLineOffsets = NEW Int32[xFrameHeight];
                for (Int32 l = 0; l < xFrameHeight; l++)
                {
                    xStream >> xLineOffsets[l];
                }

                if (xFrameWidth > 0 || xFrameHeight > 0)
                {
                    this->ParseOne(xStream, lpTextureData, xTextureWidth, xPalette, xDirectionIndex, xFrameIndex, xOffset, xLineOffsets, xFrameWidth, xFrameHeight, IsInterlacedgh);
                }
                SAFE_DELETE_ARRAY(xLineOffsets);
            }
        }
    }

    if (IsInterlacedgh)
    {
        //xTextureWidth = hge->Texture_GetWidth(x);
        //xTextureHeight = hge->Texture_GetHeight(x);
        for (int y = 0; y < xTextureHeight; y++)
        {
            if (((y % 2) == 0) && y < (xTextureHeight - 1))
            {
                LPVOID lpDst = &(lpTextureData[(y + 1) * xTextureWidth]);
                LPVOID lpSrc = &(lpTextureData[(y)* xTextureWidth]);
                memcpy(lpDst, lpSrc, xTextureWidth * sizeof(DWORD));
            }
        }
    }

    m_RenderManager->Texture_Unlock(xTexture);

    if (IsSaveFile)
    {
        ::CreateDirectoryA("Data", nullptr);
        FFORMAT(szDirBuff, "Data/%x.png", xKey);
        m_RenderManager->Texture_SaveToFile(xTexture, szDirBuff.c_str());
    }
    SAFE_DELETE_ARRAY(xFrameOffsets);
    xStream.OutSeek(0);

    return xTexture;
}

MsList<HTEXTURE> WasDecoder::ExeSomeTex(MsMemoryStream& xStream, Int32 xKey, Int32* xFrameOffsets, WPixels& xPalette, Boolean IsSaveFile)
{
    MsList<HTEXTURE> xListTex;
    for (Int32 xDirectionIndex = 0; xDirectionIndex < m_DirectionCount; xDirectionIndex++)
    {
        for (Int32 xFrameIndex = 0; xFrameIndex < m_FrameCount; xFrameIndex++)
        {
            HTEXTURE xTexture = m_RenderManager->Texture_Create(m_Width, m_Height);
            Int32 xTextureWidth = m_RenderManager->Texture_GetWidth(xTexture);
            Int32 xTextureHeight = m_RenderManager->Texture_GetHeight(xTexture);
            LPDWORD lpTextureData = m_RenderManager->Texture_Lock(xTexture);
            //Int32 xImageIndex = 0;
            Boolean IsInterlacedgh = False;

            Int32 xOffset = xFrameOffsets[(xDirectionIndex * m_FrameCount + xFrameIndex)];
            if (xOffset != 0)
            {
                xStream.OutSeek(xOffset + m_ImageHeaderSize + 4);
                Int32 xFrameWidth;
                Int32 xFrameHeight;

                xStream >> (m_ListCenterX[xDirectionIndex * m_FrameCount + xFrameIndex])
                    >> (m_ListCenterY[xDirectionIndex * m_FrameCount + xFrameIndex])
                    >> xFrameWidth >> xFrameHeight;

                Int32* xLineOffsets = NEW Int32[xFrameHeight];
                for (Int32 l = 0; l < xFrameHeight; l++)
                {
                    xStream >> xLineOffsets[l];
                }

                if (xFrameWidth > 0 || xFrameHeight > 0)
                {
                    this->ParseSome(xStream, lpTextureData, xTextureWidth, xPalette, xOffset, xLineOffsets, xFrameWidth, xFrameHeight, IsInterlacedgh);
                }
                SAFE_DELETE_ARRAY(xLineOffsets);
            }

            if (IsInterlacedgh)
            {
                //xTextureWidth = hge->Texture_GetWidth(x);
                //xTextureHeight = hge->Texture_GetHeight(x);
                for (int y = 0; y < xTextureHeight; y++)
                {
                    if (((y % 2) == 0) && y < (xTextureHeight - 1))
                    {
                        LPVOID lpDst = &(lpTextureData[(y + 1) * xTextureWidth]);
                        LPVOID lpSrc = &(lpTextureData[(y)* xTextureWidth]);
                        memcpy(lpDst, lpSrc, xTextureWidth * sizeof(DWORD));
                    }
                }
            }

            m_RenderManager->Texture_Unlock(xTexture);

            if (IsSaveFile)
            {
                ::CreateDirectoryA("Data", nullptr);
                FFORMAT(szDirBuff, "Data/%x_%d.png", xKey, (xDirectionIndex * m_FrameCount + xFrameIndex));
                m_RenderManager->Texture_SaveToFile(xTexture, szDirBuff.c_str());
            }
            xStream.OutSeek(0);

            xListTex.Add(xTexture);
        }
    }

    SAFE_DELETE_ARRAY(xFrameOffsets);
    return xListTex;
}

MsList<HTEXTURE> WasDecoder::Analysis(MsMemoryStream& xStream, Int32 xKey, Boolean IsSaveFile)
{
    MsList<HTEXTURE> xListTex;

    xStream.OutSeek(0);
    //Int32 xOffect = 0;

    // 检查文件头标记
    WORD xFileHeadFlag;
    xStream >> xFileHeadFlag;
    if (xFileHeadFlag == 0x5053)
    {
        xStream >> m_ImageHeaderSize >> m_DirectionCount >> m_FrameCount >> m_Width >> m_Height >> m_CenterX >> m_CenterY;

        m_ListCenterX = NEW Int32[m_DirectionCount * m_FrameCount];
        m_ListCenterY = NEW Int32[m_DirectionCount * m_FrameCount];

        // 读取帧延迟信息
        Int16 xLen = (Int16)(m_ImageHeaderSize - 12);
        if (xLen > 0)
        {
            for (Int32 i = 0; i < xLen; i++)
            {
                Byte xDelays = 0;
                xStream >> xDelays;
                m_ListDelays.Add((Int32)xDelays);
            }
        }

        WPixels xPalette = { 0 };
        // 读取调色板
        xStream.OutSeek(m_ImageHeaderSize + 4);
        //for (Int32 i = 0; i < 256; i++)
        //{
        xStream >> xPalette;
        //}

        // 读取帧偏移
        Int32* xFrameOffsets = NEW Int32[m_DirectionCount * m_FrameCount];
        xStream.OutSeek(m_ImageHeaderSize + 4 + 512);
        for (Int32 i = 0; i < m_DirectionCount; i++)
        {
            for (Int32 n = 0; n < m_FrameCount; n++)
            {
                xStream >> xFrameOffsets[(i * m_FrameCount + n)];
            }
        }

        //if (m_Width * m_FrameCount > WAS_TEXTURE_MAX_SIZE || m_Height * m_DirectionCount > WAS_TEXTURE_MAX_SIZE)
        //{
        xListTex = ExeSomeTex(xStream, xKey, xFrameOffsets, xPalette, IsSaveFile);
        //}
        //else
        //{
        //    HTEXTURE xTexture = ExeOneTex(m_RenderManager, xStream, xKey, xFrameOffsets, xPalette, IsSaveFile);
        //    xListTex.Add(xTexture);
        //}
    }
    return xListTex;
}

void WasDecoder::Load(MsMemoryStream& xStream, Int32 xKey, Boolean IsFromFile, Boolean IsSaveFile)
{
    MsList<HTEXTURE> hTex;
    if (IsFromFile)
    {
        // 从本地文件加载,需要识别文件名后面的索引
        FFORMAT(szBuff, "Data/%x.png", xKey);
        Int32 width, height;
        hTex.Add(m_RenderManager->Texture_LoadFromFile(szBuff.c_str(), width, height));
    }
    else
    {
        hTex = this->Analysis(xStream, xKey, IsSaveFile);
        //hTex = this->Analysis(xStream, xKey, True);
    }
    if (hTex.GetCount() == 1)
    {
        m_GameSprite = NEW GameSprite(m_RenderManager, hTex[0], 0, 0, (Single)m_Width, (Single)m_Height);
        m_IsSomeSprite = False;
    }
    else
    {
        FAST_FOREACH(hTex)
        {
            m_ListGameSprite.Add(NEW GameSprite(m_RenderManager, xEnumValue, 0, 0, (Single)m_Width, (Single)m_Height));
        }

        m_IsSomeSprite = True;
    }
}

void WasDecoder::GetRenderRect(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, RenderManagerRect& xOutRect)
{
    if (xNextIndex >= m_FrameCount || xNextIndex == INVALID_NID) { xNextIndex = m_FrameCount - 1; }
    Int32 xRealIndex = xNextIndex % m_FrameCount;
    xOutRect.SetX(xRealIndex * m_Width);
    xOutRect.SetY(xDirIndex * m_Height);
    xOutRect.SetWidth(m_Width);
    xOutRect.SetHeight(m_Height);
}

void WasDecoder::GetRenderCenter(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, RenderManagerPoint& xCenterPoint)
{
    if (xNextIndex >= m_FrameCount || xNextIndex == INVALID_NID) { xNextIndex = m_FrameCount - 1; }
    Int32 xRealIndex = xNextIndex % m_FrameCount;
    xCenterPoint.X = (Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xRealIndex];
    xCenterPoint.Y = (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xRealIndex];
}

GameSprite* WasDecoder::GetRenderSprite()
{
    return m_GameSprite;
}

GameSprite* WasDecoder::GetGameSprite(Int32 xNextIndex, Int32 xDirIndex)
{
    if (m_IsSomeSprite)
    {
        if (xNextIndex >= m_FrameCount || xNextIndex == INVALID_NID) { xNextIndex = m_FrameCount - 1; }
        return m_ListGameSprite[(xDirIndex * m_FrameCount) + xNextIndex];
    }
    else
    {
        return m_GameSprite;
    }
};

Boolean WasDecoder::RenderNoHot(Int64 xNow, Single X, Single Y, DWORD dwColor)
{
    m_IsIgnoreCenter = True;
    return Render(xNow, X, Y, dwColor);
}

Boolean WasDecoder::Render(Int64 xNow, Single X, Single Y, DWORD dwColor)
{
    GameSprite* xGameSprite = m_GameSprite;
    xGameSprite->SetTextureRect(0.0f, 0.0f, m_Width, m_Height);
    if (m_IsIgnoreCenter)
    {
        xGameSprite->SetHotSpot(0.0f, 0.0f);
    }
    else
    {
        xGameSprite->SetHotSpot((Single)m_ListCenterX[0], (Single)m_ListCenterY[0]);
    }
    m_IsIgnoreCenter = False;
    xGameSprite->SetColor(dwColor);
    xGameSprite->RenderTexture(X, Y, dwColor);

    return True;
}

Boolean WasDecoder::RenderNoHotWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, DWORD dwColor)
{
    m_IsIgnoreCenter = True;
    return RenderWithIndex(xNow, xNextIndex, xDirIndex, X, Y, dwColor);
}

Boolean WasDecoder::RenderWithIndex(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, DWORD dwColor)
{
    return RenderWithSizeRatio(xNow, xNextIndex, xDirIndex, X, Y, -1.0f, -1.0f, -1.0f, -1.0f, dwColor);
}

Boolean WasDecoder::RenderWithSizeRatio(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Single X, Single Y, Single W, Single H, Single ZW, Single ZH, DWORD dwColor)
{
    Boolean xRet = False;
    if (xNextIndex >= m_FrameCount - 1) { xNextIndex = m_FrameCount - 1; xRet = True; }
    if (xNextIndex == INVALID_NID) { xNextIndex = m_FrameCount - 1; }

    GameSprite* xGameSprite = this->GetGameSprite(xNextIndex, xDirIndex);
    if (xGameSprite)
    {
        if (m_IsSomeSprite)
        {
            xGameSprite->SetTextureRect(0.0f, 0.0f, m_Width, m_Height);
            if (m_IsIgnoreCenter)
            {
                xGameSprite->SetHotSpot(0.0f, 0.0f);
            }
            else
            {
                xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);
            }
        }
        else
        {
            xGameSprite->SetTextureRect((Single)(xNextIndex * m_Width), (Single)(xDirIndex * m_Height), m_Width, m_Height);
            if (m_IsIgnoreCenter)
            {
                xGameSprite->SetHotSpot(0.0f, 0.0f);
            }
            else
            {
                xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);

            }
        }
        m_IsIgnoreCenter = False;
        xGameSprite->SetColor(dwColor);
        xGameSprite->RenderTexture(X, Y, W, H, ZW, ZH, dwColor);
    }
    else
    {
        AssertLog("??????????????????");
    }
    return xRet;
}

Boolean WasDecoder::RenderWithSize(Int64 xNow, Single X, Single Y, Single W, Single H, DWORD dwColor)
{
    return RenderWithSizeRatio(xNow, 0, 0, X, Y, W, H, -1.0f, -1.0f, dwColor);
}

Boolean WasDecoder::IsBoundingBoxPickup(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y)
{
    GameSprite* xGameSprite = this->GetGameSprite(xNextIndex, xDirIndex);
    if (m_IsSomeSprite)
    {
        xGameSprite->SetTextureRect(0.0f, 0.0f, m_Width, m_Height);
        xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);
        return xGameSprite->IsBoundingBoxPickup((Single)xMouseX, (Single)xMouseY, X, Y);
    }
    else
    {
        xGameSprite->SetTextureRect((Single)(xNextIndex * m_Width), (Single)(xDirIndex * m_Height), m_Width, m_Height);
        xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);
        return xGameSprite->IsBoundingBoxPickup((Single)xMouseX, (Single)xMouseY, X, Y);
    }
}

Boolean WasDecoder::CheckPixelPickup(Int64 xNow, Int32 xNextIndex, Int32 xDirIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y)
{
    GameSprite* xGameSprite = this->GetGameSprite(xNextIndex, xDirIndex);
    if (m_IsSomeSprite)
    {
        xGameSprite->SetTextureRect(0.0f, 0.0f, m_Width, m_Height);
        xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);
        return xGameSprite->IsPixelPickup(0, 0, (Single)xMouseX, (Single)xMouseY, X, Y);
    }
    else
    {
        xGameSprite->SetTextureRect((Single)(xNextIndex * m_Width), (Single)(xDirIndex * m_Height), m_Width, m_Height);
        xGameSprite->SetHotSpot((Single)m_ListCenterX[(xDirIndex * m_FrameCount) + xNextIndex], (Single)m_ListCenterY[(xDirIndex * m_FrameCount) + xNextIndex]);
        return xGameSprite->IsPixelPickup(xDirIndex, xNextIndex, (Single)xMouseX, (Single)xMouseY, X, Y);
    }
}

void WasDecoder::GetRenderRect(Int64 xNow, Int32 xNextIndex, RenderManagerRect& xOutRect)
{
    return this->GetRenderRect(xNow, xNextIndex, 0, xOutRect);
}

void WasDecoder::GetRenderCenter(Int64 xNow, Int32 xNextIndex, RenderManagerPoint& xCenterPoint)
{
    return this->GetRenderCenter(xNow, xNextIndex, 0, xCenterPoint);
}

//Boolean WasDecoder::Render(Int64 xNow, Int32 xNextIndex, Single X, Single Y, DWORD dwColor)
//{
//    return this->Render(xNow, xNextIndex, 0, X, Y, dwColor);
//}

Boolean WasDecoder::IsBoundingBoxPickup(Int64 xNow, Int32 xNextIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y)
{
    return this->IsBoundingBoxPickup(xNow, xNextIndex, 0, xMouseX, xMouseY, X, Y);
}

Boolean WasDecoder::CheckPixelPickup(Int64 xNow, Int32 xNextIndex, Int32 xMouseX, Int32 xMouseY, Single X, Single Y)
{
    return this->CheckPixelPickup(xNow, xNextIndex, 0, xMouseX, xMouseY, X, Y);
}

Boolean WasDecoder::CopyDataFromFrameIndex(LPDWORD lpData, Int32 xFrameIndex)
{
    GameSprite* xGameSprite = this->GetGameSprite(xFrameIndex, 0);
    if (xGameSprite)
    {
        HTEXTURE xTex = xGameSprite->GetTexture();
        if (!xTex) { return False; }

        DWORD xCursorWidth = (DWORD)xGameSprite->GetWidth();
        DWORD xCursorHeight = (DWORD)xGameSprite->GetHeight();
        LPDWORD xTexData = m_RenderManager->Texture_Lock(xTex);

        if (m_IsSomeSprite)
        {
            memcpy(lpData, xTexData, xCursorWidth * xCursorHeight * sizeof(DWORD));
            //    for (DWORD w = 0; w < xCursorWidth; w++)
            //{
            //    for (DWORD h = 0; h < xCursorHeight; h++)
            //    {
            //        lpData[xCursorWidth * h + w] = xTexData[xCursorWidth * h + w];
            //    }
            //}
        }
        else
        {
            DWORD xTexWidth = m_RenderManager->Texture_GetWidth(xTex);
            //DWORD xTexHeight = m_RenderManager->Texture_GetHeight(xTex);
            for (DWORD w = 0; w < xCursorWidth; w++)
            {
                for (DWORD h = 0; h < xCursorHeight; h++)
                {
                    lpData[xCursorWidth * h + w] = xTexData[xTexWidth * h + (xFrameIndex * xCursorWidth + w)];
                }
            }
        }

        m_RenderManager->Texture_Unlock(xTex);
        return True;
    }
    return False;
}

void WasDecoder::ParseOne(
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
    Boolean& IsInterlacedgh)
{
    DWORD xWidth = m_Width;
    DWORD xHeight = m_Height;
    DWORD xLimitWidth = xWidth * (xFrameIndex + 1);
    DWORD xLimitHeight = xHeight * (xSpriteIndex + 1);

    IsInterlacedgh = True;
    for (DWORD xIndexY = 0; xIndexY < xFrameHeight; xIndexY++)
    {
        xStream.OutSeek(xLineOffsets[xIndexY] + xFrameOffset + m_ImageHeaderSize + 4);
        for (DWORD xIndexX = 0; xIndexX < xFrameWidth;)
        {
            Byte xTemp;
            xStream >> xTemp;
            DWORD xPixelTemp = (DWORD)xTemp;

            // 象素行结束
            if (xPixelTemp == 0) { break; }

            switch (xPixelTemp & 0xC0)
            {
                case 0:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    if (xPixelTemp & 0x20)
                    {
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD x = (xFrameIndex * xWidth) + xIndexX;
                        DWORD y = (xSpriteIndex * xHeight) + xIndexY;
                        DWORD a = (xPixelTemp & 0x1F);
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            if (a) { a = (a << 3) - 7; lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, a); }
                            xIndexX++;
                        }
                    }
                    else if (xPixelTemp)
                    {
                        DWORD c = xPixelTemp & 0x1F;
                        DWORD a = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        if (a) { a = (a << 3) - 7; }
                        for (DWORD xIndex = 0; xIndex < c; xIndex++)
                        {
                            DWORD x = (xFrameIndex * xWidth) + xIndexX;
                            DWORD y = (xSpriteIndex * xHeight) + xIndexY;
                            if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                            {
                                lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, a);
                                xIndexX++;
                            }
                        }
                    }
                    else
                    {
                        xIndexX = xFrameWidth;
                    }
                }break;

                case 0x40:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    DWORD c = xPixelTemp & 0x3F;
                    for (DWORD xIndex = 0; xIndex < c; xIndex++)
                    {
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD x = (xFrameIndex * xWidth) + xIndexX;
                        DWORD y = (xSpriteIndex * xHeight) + xIndexY;
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, 0xFF);
                            xIndexX++;
                        }
                    }
                }break;

                case 0x80:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    DWORD c = xPixelTemp & 0x3F;
                    DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                    for (DWORD xIndex = 0; xIndex < c; xIndex++)
                    {
                        DWORD x = (xFrameIndex * xWidth) + xIndexX;
                        DWORD y = (xSpriteIndex * xHeight) + xIndexY;
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, 0xFF);
                            xIndexX++;
                        }
                    }
                }break;
                default: { xIndexX += (xPixelTemp & 0x3F); }break;
            }
        }
    }
}

void WasDecoder::ParseSome(MsMemoryStream& xStream,
    LPDWORD lpTextureData,
    Int32 xTextureWidth,
    WPixels& xPalette,
    Int32 xFrameOffset,
    Int32* xLineOffsets,
    DWORD xFrameWidth,
    DWORD xFrameHeight,
    Boolean& IsInterlacedgh)
{
    DWORD xWidth = (DWORD)m_Width;
    DWORD xHeight = (DWORD)m_Height;
    DWORD xLimitWidth = xWidth;
    DWORD xLimitHeight = xHeight;

    IsInterlacedgh = True;
    for (DWORD xIndexY = 0; xIndexY < xFrameHeight; xIndexY++)
    {
        xStream.OutSeek(xLineOffsets[xIndexY] + xFrameOffset + m_ImageHeaderSize + 4);
        for (DWORD xIndexX = 0; xIndexX < xFrameWidth;)
        {
            Byte xTemp;
            xStream >> xTemp;
            DWORD xPixelTemp = (DWORD)xTemp;

            // 象素行结束
            if (xPixelTemp == 0) { break; }

            switch (xPixelTemp & 0xC0)
            {
                case 0:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    if (xPixelTemp & 0x20)
                    {
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD x = xIndexX;
                        DWORD y = xIndexY;
                        DWORD a = (xPixelTemp & 0x1F);
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            if (a) { a = (a << 3) - 7; lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, a); }
                            xIndexX++;
                        }
                    }
                    else if (xPixelTemp)
                    {
                        DWORD c = xPixelTemp & 0x1F;
                        DWORD a = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        if (a) { a = (a << 3) - 7; }
                        for (DWORD xIndex = 0; xIndex < c; xIndex++)
                        {
                            DWORD x = xIndexX;
                            DWORD y = xIndexY;
                            if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                            {
                                lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, a);
                                xIndexX++;
                            }
                        }
                    }
                    else
                    {
                        xIndexX = xFrameWidth;
                    }
                }break;

                case 0x40:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    DWORD c = xPixelTemp & 0x3F;
                    for (DWORD xIndex = 0; xIndex < c; xIndex++)
                    {
                        DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                        DWORD x = xIndexX;
                        DWORD y = xIndexY;
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, 0xFF);
                            xIndexX++;
                        }
                    }
                }break;

                case 0x80:
                {
                    if (xIndexY % 2) { IsInterlacedgh = False; }

                    DWORD c = xPixelTemp & 0x3F;
                    DWORD i = (DWORD)(xStream >> xTemp, xTemp);
                    for (DWORD xIndex = 0; xIndex < c; xIndex++)
                    {
                        DWORD x = xIndexX;
                        DWORD y = xIndexY;
                        if (x >= 0 && x < xLimitWidth && y >= 0 && y < xLimitHeight)
                        {
                            lpTextureData[xTextureWidth * y + x] = WAS_MAKE_ARGB(xPalette.m_Palette[i].m_Color, 0xFF);
                            xIndexX++;
                        }
                    }
                }break;
                default: { xIndexX += (xPixelTemp & 0x3F); }break;
            }
        }
    }
}

void WasDecoder::ToGif(mstr xGifFile, WORD xSpeed, DWORD xTransparentColor)
{
    MsMemoryStream xGifMemoryStream(MB_SIZE * 20);
    LPBYTE palette = nullptr;
    LPBYTE pData = nullptr;
    WORD xTransparentColorIndex = 0;
    Int32 nWidth, nHeight;
    BYTE bitsPixel = 8;
    CreateGIFHeard(xGifMemoryStream, m_Width, m_Height, bitsPixel);
    if (m_GameSprite)
    {
        //LPDWORD xSrcData = g_lpMainWndRender->Texture_Lock(m_GameSprite->GetTexture(), True);
        //HBITMAP hBitmap = CreateMemoryBitmap(m_Width, m_Height, xSrcData);
        //g_lpMainWndRender->Texture_Unlock(m_GameSprite->GetTexture());
        //GetData(hBitmap, &palette, &pData, &bitsPixel, &nWidth, &nHeight, xTransparentColorIndex, xTransparentColor);
        //AddImageToGIF(xGifMemoryStream, pData, palette, 0, 0, (WORD)nWidth, (WORD)nHeight, bitsPixel, xSpeed, xTransparentColorIndex);
        //SAFE_DELETE_ARRAY(pData);
        return;
    }
    else
    {
        //Int32 i = 0;
        FAST_FOREACH(m_ListGameSprite)
        {
            try
            {
                var* xTemp = xEnumValue;
                LPDWORD xSrcData = g_lpMainWndRender->Texture_Lock(xTemp->GetTexture(), True);
                for (Int32 i = 0; i < m_Width*m_Height; i++)
                {
                    if (xSrcData[i] == 0) { xSrcData[i] = 1; }
                }
                HBITMAP hBitmap = CreateMemoryBitmap(m_Width, m_Height, xSrcData);
                g_lpMainWndRender->Texture_Unlock(xTemp->GetTexture());
                GetData(hBitmap, &palette, &pData, &bitsPixel, &nWidth, &nHeight, xTransparentColorIndex, xTransparentColor);
                if (pData)
                {
                    AddImageToGIF(xGifMemoryStream, pData, palette, 0, 0, (WORD)nWidth, (WORD)nHeight, bitsPixel, xSpeed, xTransparentColorIndex);
                    SAFE_DELETE_ARRAY(pData);
                }
                ::DeleteObject(hBitmap);
                //i++;
            }
            catch (...)
            {
                SAFE_DELETE_ARRAY(pData);
                return;
            }
        }
    }

    DWORD dwOutOffect, dwInOffect;
    xGifMemoryStream.GetBuff(dwOutOffect, dwInOffect);
    //xGifMemoryStream.InSeek(0);
    //UpdateGIFHeard(xGifMemoryStream, m_Width, m_Height, bitsPixel, xTransparentColorIndex);
    //xGifMemoryStream.InSeek(dwInOffect);

    FILE* xFile = nullptr;
    fopen_s(&xFile, xGifFile.c_str(), "wb+");
    if (xFile == nullptr)
    {
        AssertLog("%d", ::GetLastError());
    }
    //xGifMemoryStream.GetBuff(dwOutOffect, dwInOffect);
    fwrite(xGifMemoryStream.GetBuff(), 1, dwInOffect, xFile);
    fclose(xFile);
}
