#pragma once

class GameSprite : public RenderSprite
{
public:
    GameSprite(RenderManager* xRenderManager, HTEXTURE tex, float x, float y, float w, float h);
    ~GameSprite();
    DINLINE Boolean CheckPixelPickup(Int64 xNow, Int32 xMouseX, Int32 xMouseY, LONG X, LONG Y) { return CheckPixelPickup(xNow, xMouseX, xMouseY, (Single)X, (Single)Y); }
    Boolean CheckPixelPickup(Int64 xNow, Int32 xMouseX, Int32 xMouseY, Single X, Single Y);

    void SaveToFile(mstr xFileName);
};

struct CenterBitmap
{
    CenterBitmap(Int32 X, Int32 Y, RenderSprite& xRenderSprite)
        : m_RenderSprite(xRenderSprite)
    {
        m_PointX = X; m_PointY = Y;
    }
    void Render(Single xPosX, Single xPosY, DWORD dwColor)
    {
        m_RenderSprite.RenderTexture(xPosX - m_PointX, xPosY - m_PointY, dwColor);
    }

    Int32 m_PointX;
    Int32 m_PointY;
    RenderSprite& m_RenderSprite;
};

#include "WdfDecoder.h"

class WasDecoder;
class ResManager
{
public:
    ResManager(RenderManager* xRenderManager);
    ~ResManager();

    E_UNIT_TYPE GetResType(mstr xFile, DWORD xKey);
    WasDecoder* AnalysisWas(mstr xFile, DWORD xKey, Boolean IsSaveFile);
    WasDecoder* LoadWas(mstr xFile, DWORD xKey);
    GameSprite* LoadTga(mstr xFile, DWORD xKey);
    LPBYTE LoadMp3(mstr xFile, DWORD xKey, Int32& xSize);
    LPBYTE LoadWav(mstr xFile, DWORD xKey, Int32& xSize);
    WasDecoder* GetMoveFlagWas(mstr xFile, DWORD xdwResKey);
    WdfDecoder* GetWdf(mstr xFile);

    void ResReset();

    RenderManager* m_RenderManager;
    MsUnorderedDictionary<mstr, WdfDecoder*> m_DictFileWdfDecoder;
};
