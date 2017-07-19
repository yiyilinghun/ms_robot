#include "Precompiled.h"
#include "Quantizer.h"
// 13 byte
#pragma pack(push, 1)
typedef struct _GIFHEADER
{
    BYTE Signature[6];
    WORD ScreenWidth;
    WORD ScreenHeight;
    BYTE GlobalFlagByte;
    BYTE BackGroundColor;
    BYTE AspectRatio;
}GIFHEARD;
#pragma pack(pop)

// 10 byte
#pragma pack(push, 1)
typedef struct _GIFDATAHEARD
{
    BYTE imageLabel;
    WORD imageLeft;
    WORD imageTop;
    WORD imageWidth;
    WORD imageHeight;
    BYTE localFlagByte;
}GIFDATAHEARD;
#pragma pack(pop)

typedef struct _GraphicController
{
    BYTE extensionIntroducer;
    BYTE graphicControlLabel;
    BYTE blockSize;
    BYTE packedField;
    WORD nDelayTime;
    BYTE transparentColorIndex;
    BYTE blockTerminator;
}GraphicController;

typedef struct _ApplicationExtension
{
    BYTE extensionIntroducer;
    BYTE applicationLabel;
    BYTE blockSize;
    CHAR applicationId[8];
    CHAR appAuthCode[3];
    CHAR cAppData[4];
    BYTE blockTerminator;
}ApplicationExtension;

void CreateGIFHeard(MsMemoryStream& xStream, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel)
{
    //GIF文件头，89a格式
    GIFHEARD heard;
    memcpy(heard.Signature, "GIF89a", 6);
    heard.ScreenWidth = nImageWidth;
    heard.ScreenHeight = nImageHeight;
    //heard.GlobalFlagByte = HASPalette|((bitsPixel-1)<<4)|0|(bitsPixel-1);全局调色板
    heard.GlobalFlagByte = ((bitsPixel - 1) << 4);
    heard.BackGroundColor = 0;
    heard.AspectRatio = 0;

    xStream.Write(&heard, 13);// 不要用sizeof(GIFHEARD)，除非把编译器设为1字节对齐

                              // 为了让IE循环播放，必须加入该应用程序块（如果只有一幅图片，可省略该块）
    ApplicationExtension appData;
    appData.extensionIntroducer = 0x21;
    appData.applicationLabel = 0xFF;
    appData.blockSize = 11;
    memcpy(appData.applicationId, "NETSCAPE", 8);
    memcpy(appData.appAuthCode, "2.0", 3);
    appData.cAppData[0] = 3;
    appData.cAppData[1] = 1;
    appData.cAppData[2] = 0;
    appData.cAppData[3] = 0;
    appData.blockTerminator = 0;

    xStream.Write(&appData, sizeof(ApplicationExtension));
}

void UpdateGIFHeard(MsMemoryStream& xStream, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel, WORD xTransparentColorIndex)
{
    //GIF文件头，89a格式
    GIFHEARD heard;
    memcpy(heard.Signature, "GIF89a", 6);
    heard.ScreenWidth = nImageWidth;
    heard.ScreenHeight = nImageHeight;
    //heard.GlobalFlagByte = HASPalette|((bitsPixel-1)<<4)|0|(bitsPixel-1);全局调色板
    heard.GlobalFlagByte = ((bitsPixel - 1) << 4);
    heard.BackGroundColor = (BYTE)xTransparentColorIndex;
    heard.AspectRatio = 0;

    xStream.Write(&heard, 13);// 不要用sizeof(GIFHEARD)，除非把编译器设为1字节对齐

                              // 为了让IE循环播放，必须加入该应用程序块（如果只有一幅图片，可省略该块）
    ApplicationExtension appData;
    appData.extensionIntroducer = 0x21;
    appData.applicationLabel = 0xFF;
    appData.blockSize = 11;
    memcpy(appData.applicationId, "NETSCAPE", 8);
    memcpy(appData.appAuthCode, "2.0", 3);
    appData.cAppData[0] = 3;
    appData.cAppData[1] = 1;
    appData.cAppData[2] = 0;
    appData.cAppData[3] = 0;
    appData.blockTerminator = 0;

    xStream.Write(&appData, sizeof(ApplicationExtension));
}

BOOL GetData(HBITMAP hBmp, LPBYTE* ppPalette, LPBYTE* ppData, BYTE *pBitsPixel, LPINT pWidth, LPINT pHeight, WORD& xTransparentColorIndex, DWORD xTransparentColor)
{
    xTransparentColor &= 0x00FFFFFF;
    xTransparentColorIndex = INVALID_WID;
    BITMAP bm;
    PBITMAPINFO bmpInf;

    if (GetObject(hBmp, sizeof(bm), &bm) == 0)
    {
        return FALSE;
    }

    int nPaletteSize = 0;

    if (bm.bmBitsPixel < 16)
    {
        nPaletteSize = (int)pow(2, bm.bmBitsPixel);
    }

    bmpInf = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD)*nPaletteSize + ((bm.bmWidth*bm.bmBitsPixel + 31) / 32) * 4 * bm.bmHeight);

    BYTE* buf = ((BYTE*)bmpInf) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nPaletteSize;

    //-----------------------------------------------
    bmpInf->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInf->bmiHeader.biWidth = bm.bmWidth;
    bmpInf->bmiHeader.biHeight = bm.bmHeight;
    bmpInf->bmiHeader.biPlanes = bm.bmPlanes;
    bmpInf->bmiHeader.biBitCount = bm.bmBitsPixel;
    bmpInf->bmiHeader.biCompression = BI_RGB;
    bmpInf->bmiHeader.biSizeImage = (bm.bmWidth + 7) / 8 * bm.bmHeight*bm.bmBitsPixel;
    //-----------------------------------------------

    HDC hDC = ::GetWindowDC(NULL);

    if (!::GetDIBits(hDC, hBmp, 0, (UINT)bm.bmHeight, buf, bmpInf, DIB_RGB_COLORS))
    {
        ::ReleaseDC(NULL, hDC);
        LocalFree(bmpInf);
        return FALSE;
    }

    ::ReleaseDC(NULL, hDC);

    *pWidth = bm.bmWidth;
    *pHeight = bm.bmHeight;

    *ppData = NEW BYTE[bm.bmWidth*bm.bmHeight];


    CQuantizer quan(256, 8);

    bmpInf->bmiHeader.biSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nPaletteSize;
    quan.ProcessImage(bmpInf);

    int nColorCount = quan.GetColorCount();
    RGBQUAD *pQuad = NEW RGBQUAD[nColorCount];

    quan.SetColorTable(pQuad);

    int nLnBytes;

    //每行数据是4字节的整数倍
    nLnBytes = ((bm.bmWidth*bm.bmBitsPixel + 31) / 32) * 4;

    int nOffset, i, nIndex = 0;
    BYTE r, g, b;
    DWORD nColorIndex;

    switch (bm.bmBitsPixel)
    {
        case 1:
        case 4:
        case 8:
        {
            SAFE_DELETE_ARRAY(pQuad);
            nColorCount = (int)pow(2, bm.bmBitsPixel);
            pQuad = NEW RGBQUAD[nColorCount];
            memcpy(pQuad, (BYTE*)bmpInf + sizeof(BITMAPINFOHEADER), sizeof(RGBQUAD)*nColorCount);

            for (i = bm.bmHeight - 1; i >= 0; i--)
            {
                nOffset = i*nLnBytes;

                for (int j = 0; j < bm.bmWidth; j++)
                {
                    if (bm.bmBitsPixel == 8)
                    {
                        (*ppData)[nIndex++] = buf[nOffset + j];
                    }
                    else
                    {
                        BYTE pos;
                        BYTE iDst = buf[nOffset + (j*bm.bmBitsPixel >> 3)];
                        if (bm.bmBitsPixel == 4)
                        {
                            pos = 4 * (1 - j % 2);
                            iDst &= (0x0F << pos);
                            (*ppData)[nIndex++] = iDst >> pos;
                        }
                        else if (bm.bmBitsPixel == 1)
                        {
                            pos = 7 - j % 8;
                            iDst &= (0x01 << pos);
                            (*ppData)[nIndex++] = iDst >> pos;
                        }
                    }
                }
            }
        }
        break;
        case 15:
        case 16:
            for (i = bm.bmHeight - 1; i >= 0; i--)
            {
                nOffset = i*nLnBytes;

                for (int j = 0; j < bm.bmWidth; j++)
                {
                    b = buf[nOffset + j * 2] & 0x1F;
                    g = buf[nOffset + j * 2] >> 5;
                    g |= (buf[nOffset + j * 2 + 1] & 0x03) << 3;
                    r = (buf[nOffset + j * 2 + 1] >> 2) & 0x1F;

                    r *= 8;
                    b *= 8;
                    g *= 8;

                    quan.GetColorIndex(r, g, b, &nColorIndex);
                    (*ppData)[nIndex++] = (BYTE)nColorIndex;
                }
            }
            break;
        case 24:
            for (i = bm.bmHeight - 1; i >= 0; i--)
            {
                nOffset = i*nLnBytes;

                for (int j = 0; j < bm.bmWidth; j++)
                {
                    b = buf[nOffset + j * 3];
                    g = buf[nOffset + j * 3 + 1];
                    r = buf[nOffset + j * 3 + 2];

                    quan.GetColorIndex(r, g, b, &nColorIndex);
                    (*ppData)[nIndex++] = (BYTE)nColorIndex;

                    //该算法效果好，但速度慢
                    //pData[nIndex++]=GetColorIndex(pQuad,nColorCount,r,g,b);
                }
            }
            break;
        case 32:
            for (i = bm.bmHeight - 1; i >= 0; i--)
            {
                nOffset = i*nLnBytes;

                for (int j = 0; j < bm.bmWidth; j++)
                {
                    b = buf[nOffset + j * 4];
                    g = buf[nOffset + j * 4 + 1];
                    r = buf[nOffset + j * 4 + 2];
                    quan.GetColorIndex(r, g, b, &nColorIndex);
                    (*ppData)[nIndex++] = (BYTE)nColorIndex;
                }
            }
            break;
        default:
            SAFE_DELETE_ARRAY(pQuad);
            LocalFree(bmpInf);
            return FALSE;
            break;
    }

    *pBitsPixel = 8;

    if (nColorCount <= 8)
    {
        *pBitsPixel = 3;
    }
    else if (nColorCount <= 16)
    {
        *pBitsPixel = 4;
    }
    else if (nColorCount <= 32)
    {
        *pBitsPixel = 5;
    }
    else if (nColorCount <= 64)
    {
        *pBitsPixel = 6;
    }
    else if (nColorCount <= 128)
    {
        *pBitsPixel = 7;
    }

    int nGIFPaletteSize = (int)pow((int)2, (int)*pBitsPixel);
    *ppPalette = NEW BYTE[nGIFPaletteSize * 3];
    memset(*ppPalette, 0, nGIFPaletteSize * 3);

    for (i = 0; i < (int)nColorCount; i++)
    {
        (*ppPalette)[i * 3] = pQuad[i].rgbRed;
        (*ppPalette)[i * 3 + 1] = pQuad[i].rgbGreen;
        (*ppPalette)[i * 3 + 2] = pQuad[i].rgbBlue;
        if (xTransparentColor == ((DWORD)(pQuad[i].rgbRed) * 0x100 * 0x100) + ((DWORD)(pQuad[i].rgbGreen) * 0x100) + (DWORD)(pQuad[i].rgbBlue))
        {
            xTransparentColorIndex = (WORD)i;
        }
    }

    SAFE_DELETE_ARRAY(pQuad);
    LocalFree(bmpInf);

    return TRUE;
}

HBITMAP CreateMemoryBitmap(Int32 xWidth, Int32 xHeight, LPVOID lpBits)
{
    return ::CreateBitmap(xWidth, xHeight, 1, 32, lpBits);
}

void EnCode(LPBYTE pData, DWORD nDataSize, BYTE bitsPixel, LPWORD* ppEnCodeData, LPDWORD pnSize)
{
    DWORD nDataBufIndex = 0;
    LPWORD pHeard = NEW WORD[4097];
    LPWORD pEnd = NEW WORD[4096];
    *ppEnCodeData = NEW WORD[10000000]; // 每幅图片最大10000000个像素

    DWORD nIndex = 0;
    DWORD nDataIndex = 0;

    pHeard[nIndex] = pData[nDataIndex];
    pEnd[nIndex] = pData[nDataIndex + 1];
    nIndex++;
    nDataIndex++;

    WORD wHeard, wEnd;
    BOOL bEndIsValide = TRUE;

    WORD wBeginCode = (((WORD)1) << bitsPixel) + 2;

    // 编码
    for (; nDataIndex < nDataSize; )
    {
        wHeard = pData[nDataIndex];

        if (nDataIndex + 1 >= nDataSize)
        {
            pHeard[nIndex] = wHeard;
            nIndex++;
            bEndIsValide = FALSE;
            break;
        }

        wEnd = pData[nDataIndex + 1];
        nDataIndex++;

        for (WORD i = 0; i < nIndex; i++)
        {
            if ((pHeard[i] == wHeard) && (pEnd[i] == wEnd))
            {
                wHeard = wBeginCode + i;
                if (nDataIndex + 1 >= nDataSize)
                {
                    pHeard[nIndex] = wHeard;
                    nIndex++;
                    bEndIsValide = FALSE;
                    break;
                }

                wEnd = pData[nDataIndex + 1];
                nDataIndex++;
            }
        }

        pHeard[nIndex] = wHeard;

        if (bEndIsValide)
        {
            pEnd[nIndex] = wEnd;
            nIndex++;
        }
        else
        {
            break;
        }

        // 达到4096(12位)后，重新开始编码
        if (wBeginCode + nIndex - 1 == 4096)
        {
            memcpy(((BYTE*)*ppEnCodeData) + nDataBufIndex, pHeard, nIndex * 2);
            nDataBufIndex += nIndex * 2;
            nIndex = 0;

            pHeard[nIndex] = pData[nDataIndex];
            pEnd[nIndex] = pData[nDataIndex + 1];
            nIndex++;
            nDataIndex++;
        }
    }

    memcpy(((BYTE*)*ppEnCodeData) + nDataBufIndex, pHeard, nIndex * 2);
    nDataBufIndex += nIndex * 2;

    *pnSize = nDataBufIndex / 2;

    SAFE_DELETE_ARRAY(pHeard);
    SAFE_DELETE_ARRAY(pEnd);
}

void addToBuf(MsMemoryStream& xStream, LPBYTE buf, DWORD &nUsedBits, BYTE data, int nBit)
{
    int nIndex, nCurByteUsedBits;

    nIndex = nUsedBits / 8;
    nCurByteUsedBits = nUsedBits % 8;

    if (nIndex >= 255)
    {
        //已经产生255个字节的数据
        BYTE bytes = 0xFF;
        xStream.Write(&bytes, 1);
        xStream.Write(buf, bytes);

        buf[0] = buf[255];
        nIndex = 0;
        nUsedBits -= 255 * 8;
    }

    if (nCurByteUsedBits == 0)
    {
        buf[nIndex] = data;
    }
    else
    {
        buf[nIndex] |= data << nCurByteUsedBits;

        if (nBit > 8 - nCurByteUsedBits)
        {
            nIndex++;
            buf[nIndex] = data >> (8 - nCurByteUsedBits);
        }
    }

    nUsedBits += nBit;
}

void WriteData(MsMemoryStream& xStream, LPWORD pHeard, int nDataCount, BYTE bitsPixel, BOOL bEnd)
{
    BYTE initBits = bitsPixel + 1;
    BYTE pByts[256];
    //LPBYTE pTmp = (LPBYTE)pHeard;
    int nCurCodeBits = initBits;
    WORD nMaxCode = 1;
    WORD nClearCode = 1;
    WORD nEndCode;

    nClearCode <<= nCurCodeBits - 1;
    nMaxCode <<= nCurCodeBits;
    nEndCode = nClearCode + 1;

    memset(pByts, 0, 256);

    DWORD nUsedBits = 0;

    addToBuf(xStream, pByts, nUsedBits, nClearCode & 0xFF, min(8, nCurCodeBits));
    if (nCurCodeBits > 8)
    {
        addToBuf(xStream, pByts, nUsedBits, (nClearCode & 0xFF00) >> 8, nCurCodeBits - 8);
    }

    int nHeardCode = nClearCode + 1;

    for (int i = 0; i < nDataCount; i++)
    {
        if (nHeardCode++ >= nMaxCode)
        {
            nCurCodeBits++;
            if (nCurCodeBits == 13)
            {
                //CString sMsg;
                //sMsg.Format("[%X,%X]",pHeard[i-1],pHeard[i]);
                //AfxMessageBox(sMsg);

                addToBuf(xStream, pByts, nUsedBits, nClearCode & 0xFF, 8);
                addToBuf(xStream, pByts, nUsedBits, (nClearCode & 0xFF00) >> 8, 4);
                nCurCodeBits = initBits;

                nHeardCode = nClearCode + 2;
            }

            nMaxCode = (WORD)1 << nCurCodeBits;
        }

        addToBuf(xStream, pByts, nUsedBits, pHeard[i] & 0xFF, min(8, nCurCodeBits));

        if (nCurCodeBits > 8)
        {
            addToBuf(xStream, pByts, nUsedBits, (pHeard[i] & 0xFF00) >> 8, nCurCodeBits - 8);
        }
    }

    if (bEnd)
    {
        addToBuf(xStream, pByts, nUsedBits, nEndCode & 0xFF, min(8, nCurCodeBits));
        if (nCurCodeBits > 8)
        {
            addToBuf(xStream, pByts, nUsedBits, (nEndCode & 0xFF00) >> 8, nCurCodeBits - 8);
        }
    }

    DWORD nByteCount = nUsedBits / 8;

    if (nUsedBits % 8 != 0)
    {
        nByteCount++;
    }

    BYTE bytes;
    int nIndex = 0;
    while (nByteCount > 0)
    {
        bytes = min(0xFF, (BYTE)nByteCount);
        nByteCount -= bytes;

        xStream.Write(&bytes, 1);
        xStream.Write(pByts + nIndex, bytes);

        nIndex += bytes;
    }

    bytes = 0x0;
    xStream.Write(&bytes, 1);
}

void AddImageToGIF(MsMemoryStream& xStream, BYTE *pData, BYTE *palette, WORD xCenterX, WORD xCenterY, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel, WORD nDelay, WORD nTransparentColorIndex)
{
    //控制块
    GraphicController control;

    control.extensionIntroducer = 0x21;
    control.graphicControlLabel = 0xF9;
    control.blockSize = 4;
    control.packedField = (nTransparentColorIndex == INVALID_WID ? 4 : 9);
    //control.packedField = 9;
    control.nDelayTime = nDelay;//延迟时间
    control.transparentColorIndex = (BYTE)(nTransparentColorIndex == INVALID_WID ? 0 : nTransparentColorIndex);
    control.blockTerminator = 0;

    xStream.Write(&control, sizeof(GraphicController));

    //file.Write(palette,nGIFPaletteSize*3);全局调色板

    // 图片数据头
    GIFDATAHEARD dataHeard;
    dataHeard.imageLabel = 0x2c;
    dataHeard.imageLeft = xCenterX;
    dataHeard.imageTop = xCenterY;
    dataHeard.imageWidth = nImageWidth;
    dataHeard.imageHeight = nImageHeight;
    dataHeard.localFlagByte = 0x80 | (bitsPixel - 1);

    xStream.Write(&dataHeard.imageLabel, 1);
    xStream.Write(&dataHeard.imageLeft, 2);
    xStream.Write(&dataHeard.imageTop, 2);
    xStream.Write(&dataHeard.imageWidth, 2);
    xStream.Write(&dataHeard.imageHeight, 2);
    xStream.Write(&dataHeard.localFlagByte, 1);

    //调色板
    xStream.Write(palette, (int)pow(2, bitsPixel) * 3);

    //每像素占用位数
    xStream.Write(&bitsPixel, 1);

    LPWORD pEnCodeData = NULL;
    DWORD nSize = 0;

    //编码
    EnCode(pData, nImageWidth*nImageHeight, bitsPixel, &pEnCodeData, &nSize);

    //写入数据
    WriteData(xStream, pEnCodeData, nSize, bitsPixel, TRUE);

    SAFE_DELETE_ARRAY(pEnCodeData);
}
