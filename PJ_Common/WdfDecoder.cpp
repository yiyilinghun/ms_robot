#include "Precompiled.h"

WdfDecoder::WdfDecoder(RenderManager* xRenderManager, mstr xFileName)
    : m_RenderManager(xRenderManager)
{
    m_FileName = xFileName;
    m_FullFileName = GetBaseGamePath() + xFileName;
    m_hFile = ::CreateFileA(m_FullFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        m_FullFileName = "../../" + xFileName;
        m_hFile = ::CreateFileA(m_FullFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (m_hFile == INVALID_HANDLE_VALUE)
        {
            MsVector<DWORD> xList1; xList1.Add(3000000000); xList1.Add(577919341);
            MsVector<DWORD> xList2; xList2.Add(0x1730d4d0); xList2.Add(10);
            MsVector<DWORD> xList3; xList3.Add(0xeeff2000); xList3.Add(0xf);
            MsVector<DWORD> xList4; xList4.Add(0xc9103900); xList4.Add(0x79);
            ZIP_RES_INFO xResInfo(False);
            if (ZipUtils::ExtractZipToMemoryByName("..\\..\\res.zip", xFileName.c_str(), xResInfo, MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str()))
            {
                m_Memory = xResInfo.m_DataAddr;
                m_MemoryDataOffset = m_Memory;
            }
            else
            {
                MessageBoxA(nullptr, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s]未找到", xFileName.c_str()).c_str(), MB_OK);
                return;
            }
        }
    }

    // 检查文件头标记
    UInt32 xFileHeadFlag = 0;
    AUTO_READ_ASSIST(m_hFile, m_MemoryDataOffset, xFileHeadFlag, "文件头标记");
    if (xFileHeadFlag == 0x57444650)
    {
        // 读取文件头
        AUTO_READ_ASSIST(m_hFile, m_MemoryDataOffset, m_UnitCount, "单元数量");
        AUTO_READ_ASSIST(m_hFile, m_MemoryDataOffset, m_CatalogListAddr, "单元地址");

        AUTO_SET_POINTER_ASSIST(m_hFile, m_Memory, m_MemoryDataOffset, m_CatalogListAddr);
        for (int i = 0; i < m_UnitCount; i++)
        {
            UnitFileInfo xUnitFileInfo = { 0,0,0,0 };
            AUTO_READ_ASSIST(m_hFile, m_MemoryDataOffset, xUnitFileInfo, "单元信息");
            if (m_DictWdfUnit.ContainsKey(xUnitFileInfo.m_Key))
            {
                m_DictWdfUnit.Remove(xUnitFileInfo.m_Key);
            }
            m_DictWdfUnit.Add(xUnitFileInfo.m_Key, WdfUnit(xUnitFileInfo));
        }
    }
}

WdfDecoder::~WdfDecoder()
{
    FAST_FOREACH(m_DictWdfUnit)
    {
        auto& xTemp = xEnumValue;;
        SAFE_DELETE_ARRAY(xTemp.m_Data);
    }
    m_DictWdfUnit.Clear();

    SAFE_ENUMERATOR_DELETE(m_DictWasDecoder);
    SAFE_ENUMERATOR_DELETE(m_DictTgaDecoder);
    m_DictMp3Decoder.Clear();
    m_DictWavDecoder.Clear();
    SAFE_CLOSE(m_hFile);
    SAFE_DELETE_ARRAY(m_Memory);
    m_MemoryDataOffset = nullptr;
}

void WdfDecoder::Reset()
{
    SAFE_ENUMERATOR_DELETE(m_DictWasDecoder);
    SAFE_ENUMERATOR_DELETE(m_DictTgaDecoder);
    m_DictMp3Decoder.Clear();
    m_DictWavDecoder.Clear();
}

E_UNIT_TYPE WdfDecoder::GetUnitType(Int32 xKey)
{
    if (m_DictWdfUnit.ContainsKey(xKey))
    {
        WdfUnit* xWdfUnit = &(m_DictWdfUnit[xKey]);
        if (xWdfUnit->m_Data == nullptr)
        {
            var& xAddr = xWdfUnit->m_UnitFileInfo.m_Addr;
            var& xLen = xWdfUnit->m_UnitFileInfo.m_Len;
            xWdfUnit->m_Data = NEW Byte[xLen];
            var& xData = xWdfUnit->m_Data;
            AUTO_SET_POINTER_ASSIST(m_hFile, m_Memory, m_MemoryDataOffset, xAddr);
            AUTO_READ_LEN_ASSIST(m_hFile, m_MemoryDataOffset, xData, xLen, "");
        }
        MsMemoryStream xStream(xWdfUnit->m_Data, xWdfUnit->m_UnitFileInfo.m_Len);
        return this->GetUnitType(xStream);
    }
    return E_UNIT_TYPE::EUT_UNKNOW;
}

E_UNIT_TYPE WdfDecoder::GetUnitType(MsMemoryStream& xStream)
{
    xStream.OutSeek(0);
    WORD xHeadHi;
    WORD xHeadLo;
    xStream >> xHeadHi >> xHeadLo;
    switch (xHeadHi)
    {
        case 0x5053: { return EUT_WAS; }
        case 0x4D42: { return EUT_BMP; }
        case 0x4646:
        {
            switch (xHeadLo)
            {
                case 0x4952: { return EUT_RIFF; }
                default: { return EUT_UNKNOW; }
            }
        }
        case 0x0000:
        {
            switch (xHeadLo)
            {
                case 0x0002:
                case 0x000A: { return EUT_TGA; }
                default: { return EUT_UNKNOW; }
            }
        }
        case 0xf2ff:
        case 0xfbff: { return EUT_MP3; }

        case 0x4952: { return EUT_WAV; }

        default: { return EUT_UNKNOW; }
    }
}

Boolean WdfDecoder::ContainsKey(DWORD xKey)
{
    return m_DictWdfUnit.ContainsKey(xKey);
}

WdfUnit* WdfDecoder::GetUnit(DWORD xKey)
{
    if (m_DictWdfUnit.ContainsKey(xKey))
    {
        return &(m_DictWdfUnit[xKey]);
    }
    return nullptr;
}

WasDecoder* WdfDecoder::AnalysisOneWas(Int32 xKey, Boolean IsSaveFile)
{
    try
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            WdfUnit* xWdfUnit = &(m_DictWdfUnit[xKey]);
            if (xWdfUnit->m_Data == nullptr)
            {
                var& xAddr = xWdfUnit->m_UnitFileInfo.m_Addr;
                var& xLen = xWdfUnit->m_UnitFileInfo.m_Len;
                xWdfUnit->m_Data = NEW Byte[xLen];
                var& xData = xWdfUnit->m_Data;
                AUTO_SET_POINTER_ASSIST(m_hFile, m_Memory, m_MemoryDataOffset, xAddr);
                AUTO_READ_LEN_ASSIST(m_hFile, m_MemoryDataOffset, xData, xLen, "");
            }

            MsMemoryStream xStream(xWdfUnit->m_Data, xWdfUnit->m_UnitFileInfo.m_Len);
            switch (GetUnitType(xStream))
            {
                case EUT_WAS:
                {
                    WasDecoder* xWasDecoder = NEW WasDecoder(m_RenderManager);
                    xWasDecoder->Analysis(xStream, xKey, IsSaveFile);
                    return xWasDecoder;
                }

                case EUT_TGA:
                {
                    // 处理缓存TGA图片到磁盘
                    //HTEXTURE xTex = hge->Texture_LoadFromMemory(xStream.GetBuff(), xWdfUnit->m_UnitFileInfo.m_Len);
                    //hgeSprite* xhgeSprite = NEW hgeSprite(xTex, 0, 0, (Single)hge->Texture_GetWidth(xTex), (Single)hge->Texture_GetHeight(xTex));
                    //m_DictSprite.Add(xKey, xhgeSprite);
                    return nullptr;
                }
                break;

                default:
                {
                    return nullptr;
                }
            }
        }
    }
    catch (...)
    {
        ;
    }
    return nullptr;
}

LPVOID WdfDecoder::LoadOne(Int32 xKey, Boolean IsSaveFile, Int32& xSize)
{
    try
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            WdfUnit* xWdfUnit = &(m_DictWdfUnit[xKey]);
            if (xWdfUnit->m_Data == nullptr)
            {
                var& xAddr = xWdfUnit->m_UnitFileInfo.m_Addr;
                var& xLen = xWdfUnit->m_UnitFileInfo.m_Len;
                xWdfUnit->m_Data = NEW Byte[xLen];
                var& xData = xWdfUnit->m_Data;
                AUTO_SET_POINTER_ASSIST(m_hFile, m_Memory, m_MemoryDataOffset, xAddr);
                AUTO_READ_LEN_ASSIST(m_hFile, m_MemoryDataOffset, xData, xLen, "");
                xSize = xWdfUnit->m_UnitFileInfo.m_Len;
            }

            MsMemoryStream xStream(xWdfUnit->m_Data, xWdfUnit->m_UnitFileInfo.m_Len);
            switch (GetUnitType(xStream))
            {
                case EUT_WAS:
                {
                    WasDecoder* xWasDecoder = NEW WasDecoder(m_RenderManager);
                    xWasDecoder->Load(xStream, xKey, False, IsSaveFile);
                    return xWasDecoder;
                }break;

                case EUT_TGA:
                {
                    Int32 xWidth;
                    Int32 xHeight;
                    HTEXTURE hTex = m_RenderManager->Texture_LoadFromMemory(xWdfUnit->m_Data, xWdfUnit->m_UnitFileInfo.m_Len, xWidth, xHeight);
                    return NEW GameSprite(m_RenderManager, hTex, 0, 0, (Single)xWidth, (Single)xHeight);
                    // 处理缓存TGA图片到磁盘
                    //HTEXTURE xTex = hge->Texture_LoadFromMemory(xStream.GetBuff(), xWdfUnit->m_UnitFileInfo.m_Len);
                    //hgeSprite* xhgeSprite = NEW hgeSprite(xTex, 0, 0, (Single)hge->Texture_GetWidth(xTex), (Single)hge->Texture_GetHeight(xTex));
                    //m_DictSprite.Add(xKey, xhgeSprite);
                    //return xWdfUnit.m_Data;
                }break;

                case EUT_MP3:
                {
                    return xWdfUnit->m_Data;
                }break;

                case EUT_WAV:
                {
                    return xWdfUnit->m_Data;
                }break;

                default:
                {
                    return xWdfUnit->m_Data;
                }
            }
        }
    }
    catch (...)
    {
        ;
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", m_FileName.c_str(), xKey).c_str(), MB_OK);
    return nullptr;
}

WasDecoder* WdfDecoder::AnalysisWas(Int32 xKey, Boolean IsSaveFile)
{
    if (m_DictWdfUnit.ContainsKey(xKey))
    {
        return this->AnalysisOneWas(xKey, IsSaveFile);
    }
    return nullptr;
}

WasDecoder* WdfDecoder::LoadWas(Int32 xKey)
{
    if (!m_DictWasDecoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            Int32 xSize;
            //m_DictWasDecoder.Add(xKey, (WasDecoder*)this->LoadOne(xKey, False, xSize));
            m_DictWasDecoder.Add(xKey, (WasDecoder*)this->LoadOne(xKey, SAVE_RES_FILE, xSize));
        }
    }
    if (m_DictWasDecoder.ContainsKey(xKey))
    {
        return m_DictWasDecoder[xKey];
    }
    return nullptr;
}

GameSprite* WdfDecoder::LoadTga(Int32 xKey)
{
    if (!m_DictTgaDecoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            Int32 xSize;
            //m_DictTgaDecoder.Add(xKey, (GameSprite*)this->LoadOne(xKey, False, xSize));
            m_DictTgaDecoder.Add(xKey, (GameSprite*)this->LoadOne(xKey, SAVE_RES_FILE, xSize));
        }
    }
    if (m_DictTgaDecoder.ContainsKey(xKey))
    {
        return m_DictTgaDecoder[xKey];
    }
    return nullptr;
}


LPBYTE WdfDecoder::LoadMp3(Int32 xKey, Int32& xSize)
{
    if (!m_DictMp3Decoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            m_DictMp3Decoder.Add(xKey, (LPBYTE)this->LoadOne(xKey, xSize));
        }
    }
    if (m_DictMp3Decoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            WdfUnit* xWdfUnit = &(m_DictWdfUnit[xKey]);
            xSize = xWdfUnit->m_UnitFileInfo.m_Len;
            return m_DictMp3Decoder[xKey];
        }
    }
    return nullptr;
}

LPBYTE WdfDecoder::LoadWav(Int32 xKey, Int32& xSize)
{
    if (!m_DictWavDecoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            m_DictWavDecoder.Add(xKey, (LPBYTE)this->LoadOne(xKey, xSize));
        }
    }
    if (m_DictWavDecoder.ContainsKey(xKey))
    {
        if (m_DictWdfUnit.ContainsKey(xKey))
        {
            WdfUnit* xWdfUnit = &(m_DictWdfUnit[xKey]);
            xSize = xWdfUnit->m_UnitFileInfo.m_Len;
            return m_DictWavDecoder[xKey];
        }
    }
    return nullptr;
}

void WdfDecoder::ClearRes()
{
    MsList<DWORD> xTempList;
    FAST_FOREACH(m_DictWasDecoder)
    {
        var x = xEnumValue;
        SAFE_DELETE(x);
        xTempList.Add(xEnumKey);
    }

    FAST_FOREACH(xTempList)
    {
        m_DictWasDecoder.Remove(xEnumValue);
    }
}

LPVOID WdfDecoder::LoadOne(Int32 xKey, Boolean IsSaveFile)
{
    Int32 xSize;
    return LoadOne(xKey, IsSaveFile, xSize);
}

MsList<DWORD> WdfDecoder::GetAllWdfUnitKey()
{
    MsList<DWORD> xList;
    FAST_FOREACH(m_DictWdfUnit)
    {
        xList.Add(xEnumKey);
    }
    return xList;
}
