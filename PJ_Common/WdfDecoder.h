#pragma once
#define SAVE_RES_FILE 0

class WasDecoder;
class WdfDecoder
{
public:
    WdfDecoder(RenderManager* xRenderManager, mstr xFileName);
    ~WdfDecoder();

    E_UNIT_TYPE GetUnitType(Int32 xKey);
    E_UNIT_TYPE GetUnitType(MsMemoryStream& xStream);
    Boolean ContainsKey(DWORD xKey);
    WdfUnit* GetUnit(DWORD xKey);

    WasDecoder* AnalysisWas(Int32 xKey, Boolean IsSaveFile);
    WasDecoder* LoadWas(Int32 xKey);
    GameSprite* LoadTga(Int32 xKey);
    LPBYTE LoadMp3(Int32 xKey, Int32& xSize);
    LPBYTE LoadWav(Int32 xKey, Int32& xSize);
    void ClearRes();

    MsList<DWORD> GetAllWdfUnitKey();
    LPVOID LoadOne(Int32 xKey, Boolean IsSaveFile);

    void Reset();

    //private:
    LPVOID LoadOne(Int32 xKey, Boolean IsSaveFile, Int32& xSize);
    WasDecoder* AnalysisOneWas(Int32 xKey, Boolean IsSaveFile);

    RenderManager* m_RenderManager;
    HANDLE m_hFile = INVALID_HANDLE_VALUE;
    LPBYTE m_Memory = nullptr;
    LPBYTE m_MemoryDataOffset = nullptr;
    Int32 m_UnitCount;
    DWORD m_CatalogListAddr;
    mstr m_FullFileName;
    mstr m_FileName;

    MsUnorderedDictionary<DWORD, WdfUnit> m_DictWdfUnit;
    MsUnorderedDictionary<DWORD, WasDecoder*> m_DictWasDecoder;
    MsUnorderedDictionary<DWORD, GameSprite*> m_DictTgaDecoder;
    MsUnorderedDictionary<DWORD, LPBYTE> m_DictMp3Decoder;
    MsUnorderedDictionary<DWORD, LPBYTE> m_DictWavDecoder;
};
