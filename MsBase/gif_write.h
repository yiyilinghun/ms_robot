
extern void CreateGIFHeard(MsMemoryStream& xStream, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel);
extern void UpdateGIFHeard(MsMemoryStream& xStream, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel, WORD xTransparentColorIndex);
extern Boolean GetData(HBITMAP hBmp, LPBYTE* ppPalette, LPBYTE* ppData, BYTE *pBitsPixel, LPINT pWidth, LPINT pHeight, WORD& xTransparentColorIndex, DWORD xTransparentColor = 0);
extern HBITMAP CreateMemoryBitmap(Int32 xWidth, Int32 xHeight, LPVOID lpBits);
extern void AddImageToGIF(MsMemoryStream& xStream, BYTE *pData, BYTE *palette, WORD xCenterX, WORD xCenterY, WORD nImageWidth, WORD nImageHeight, BYTE bitsPixel, WORD nDelay, WORD nTransparentColorIndex);
