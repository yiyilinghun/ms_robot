#if !defined(QUANTIZER_H_)
#define QUANTIZER_H_

typedef struct NODE
{
    BOOL bIsLeaf;
    LONG nPixelCount;
    LONG nRedSum;
    LONG nGreenSum;
    LONG nBlueSum;
    UInt32 nColorIndex;
    NODE* pChild[8];
    NODE* pNext;
}*LPNODE;

class CQuantizer
{
public:
    CQuantizer(UInt32 nMaxColors, UInt32 nColorBits);
    ~CQuantizer();

    BOOL ProcessImage(HANDLE hImage);

    UInt32 GetColorCount();

    void SetColorTable(RGBQUAD* prgb);

    BOOL GetColorIndex(BYTE r, BYTE g, BYTE b, LPDWORD pColorIndex);

protected:

    void AddColor(LPNODE* ppNode, BYTE r, BYTE g, BYTE b, UInt32 nColorBits, UInt32 nLevel, LPDWORD pLeafCount, LPNODE* pReducibleNodes);

    void* CreateNode(UInt32 nLevel, UInt32 nColorBits, LPDWORD pLeafCount, LPNODE* pReducibleNodes);

    void ReduceTree(UInt32 nColorBits, LPDWORD pLeafCount, LPNODE* pReducibleNodes);

    void DeleteTree(LPNODE* ppNode);

    void GetPaletteColors(LPNODE pTree, RGBQUAD* prgb, LPDWORD pIndex);

    BOOL FindColorIndex(LPNODE pNode, BYTE r, BYTE g, BYTE b, Int32 nLevel, LPDWORD pColorIndex);

    BYTE GetPixelIndex(LONG x, LONG y, Int32 nbit, LONG effwdt, BYTE *pimage);

    Int32 m_nColorBits;
    DWORD m_nLeafCount;
    DWORD m_nMaxColors;
    LPNODE m_pTree;
    LPNODE m_pReducibleNodes[9];
};

class CRGBQuantizer : public CQuantizer
{
public:
    CRGBQuantizer(UInt32 nMaxColors, UInt32 nColorBits);
    BOOL ProcessImageRGB(BYTE *pRGBData, UInt32 nWidth, UInt32 nHeight);
};

class CBitmapQuantizer : public CQuantizer
{
public:
    CBitmapQuantizer(UInt32 nMaxColors, UInt32 nColorBits);
    BOOL ProcessImageBitmap(HBITMAP hBmp);
};

#endif
