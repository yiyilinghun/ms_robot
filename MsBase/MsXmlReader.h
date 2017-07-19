#pragma once

enum MarkupDocFlags
{
    MDF_DEFAULT = 0,
    MDF_IGNORECASE = 8,
};

enum MarkupNodeFlags
{
    MNF_WITHCDATA = 0x01,
    MNF_WITHNOLINES = 0x02,
    MNF_WITHXHTMLSPACE = 0x04,
    MNF_WITHREFS = 0x08,
    MNF_WITHNOEND = 0x10,
    MNF_ESCAPEQUOTES = 0x100,
    MNF_NONENDED = 0x100000,
    MNF_ILLDATA = 0x200000,
};
enum MarkupNodeType
{
    MNT_END_INVALID = -2,
    MNT_DEFAULT_INVALID = -1,
    MNT_INVALID = 0,  // 0x00
    MNT_ELEMENT = 1,  // 0x01
    MNT_TEXT = 2,  // 0x02
    MNT_WHITESPACE = 4,  // 0x04
    MNT_CDATA_SECTION = 8,  // 0x08
    MNT_PROCESSING_INSTRUCTION = 16, // 0x10
    MNT_COMMENT = 32, // 0x20
    MNT_DOCUMENT_TYPE = 64, // 0x40
    MNT_EXCLUDE_WHITESPACE = 123,// 0x7b
    MNT_LONE_END_TAG = 128,// 0x80
    MNT_NODE_ERROR = 32768 // 0x8000
};

enum MarkupNodeFlagsInternal
{
    MNF_DEFAULT = 0x000000,
    MNF_REPLACE = 0x001000,
    MNF_INSERT = 0x002000,
    MNF_CHILD = 0x004000,
    MNF_QUOTED = 0x008000,
    MNF_EMPTY = 0x010000,
    MNF_DELETED = 0x020000,
    MNF_FIRST = 0x080000,
    MNF_PUBLIC = 0x300000,
    MNF_ILLFORMED = 0x800000,
};

class MsXmlReader;
class MsXmlAttribReader
{
public:
    MsXmlAttribReader(MsXmlReader* xMsXmlReader);
    mstr operator[](LPCSTR szName);
    DINLINE mstr operator[](mstr& szName) { return this->operator[](szName.c_str()); }
    MsXmlReader* m_MsXmlReader;
};

class MsXmlReader
{
public:
    MsXmlReader();
    MsXmlReader(LPCSTR szDoc);
    MsXmlReader(MarkupDocFlags nFlags);
    MsXmlReader(const MsXmlReader& markup);

    void operator=(const MsXmlReader& markup);
    bool operator!() { return !m_IsLastOK; }
    operator bool() { return m_IsLastOK ? true : false; }
    MsXmlReader& operator[](LPCSTR szName);
    DINLINE MsXmlReader& MsXmlReader::operator[](mstr& szName) { return this->operator[](szName.c_str()); }

    // Navigate
    Boolean Load(LPCSTR szFileName);
    Boolean LoadFromZip(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword);
    Boolean LoadFromMem(LPBYTE pszBuf, Int32 nSize);
    Boolean LoadFromMem(LPCSTR pszBuf, Int32 nSize);
    Boolean SetDoc(LPCSTR szDoc);
    Boolean IsWellFormed();

    Boolean SelectRootElem(mstr szName) { return this->SelectRootElem(szName.c_str()); }
    Boolean FindNextElem(mstr szName) { return this->FindNextElem(szName.c_str()); }
    Boolean FindChildElem(mstr szName) { return this->FindChildElem(szName.c_str()); }
    Boolean SelectRootElem(LPCSTR szName = nullptr);
    Boolean FindNextElem(LPCSTR szName = nullptr);
    Boolean FindChildElem(LPCSTR szName = nullptr);

    Boolean IsOK();
    Boolean m_IsLastOK;
    Boolean m_IsOpenOK;

    MsXmlAttribReader Attribs;

    // Read
    void    SelectParentElem();
    Boolean IntoElem();
    Boolean OutOfElem();
    void    ResetChildPos() { _SetPos(m_iPosParent, m_iPos, 0); };
    void    ResetMainPos() { _SetPos(m_iPosParent, 0, 0); };
    void    ResetPos() { _SetPos(0, 0, 0); };
    mstr    GetTagName() const;
    mstr    GetChildTagName() const { return x_GetTagName(m_iPosChild); };
    mstr    GetData() const { return x_GetData(m_iPos); };
    mstr    GetChildData() const { return x_GetData(m_iPosChild); };
    mstr    GetElemContent() const { return x_GetElemContent(m_iPos); };
    mstr    GetAttrib(LPCSTR szAttrib) const { return x_GetAttrib(m_iPos, szAttrib); };
    mstr    GetChildAttrib(LPCSTR szAttrib) const { return x_GetAttrib(m_iPosChild, szAttrib); };
    mstr    GetAttribName(Int32 n) const;
    Int32   FindNode(Int32 nType = 0);
    Int32   GetNodeType() { return m_nNodeType; };
    Boolean SavePos(LPCSTR szPosName = "");
    Boolean RestorePos(LPCSTR szPosName = "");
    mstr&   GetError() { return m_strError; };
    void    SetDocFlags(MarkupDocFlags nFlags) { m_nFlags = nFlags; };
    MarkupDocFlags GetDocFlags() { return m_nFlags; };

    // Create
    Boolean Save(LPCSTR szFileName);
    mstr&   GetDoc() { return m_strDoc; };
    Boolean AddElem(LPCSTR szName, LPCSTR szData = nullptr, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, szData, nFlags); };
    Boolean InsertElem(LPCSTR szName, LPCSTR szData = nullptr, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, szData, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_INSERT)); };
    Boolean AddChildElem(LPCSTR szName, LPCSTR szData = nullptr, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, szData, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_CHILD)); };
    Boolean InsertChildElem(LPCSTR szName, LPCSTR szData = nullptr, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, szData, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_INSERT | (Int32)MNF_CHILD)); };
    Boolean AddElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, nValue, nFlags); };
    Boolean InsertElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, nValue, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_INSERT)); };
    Boolean AddChildElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, nValue, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_CHILD)); };
    Boolean InsertChildElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_AddElem(szName, nValue, (MarkupNodeFlagsInternal)((Int32)nFlags | (Int32)MNF_INSERT | (Int32)MNF_CHILD)); };
    Boolean AddAttrib(LPCSTR szAttrib, LPCSTR szValue) { return x_SetAttrib(m_iPos, szAttrib, szValue); };
    Boolean AddChildAttrib(LPCSTR szAttrib, LPCSTR szValue) { return x_SetAttrib(m_iPosChild, szAttrib, szValue); };
    Boolean AddAttrib(LPCSTR szAttrib, Int32 nValue) { return x_SetAttrib(m_iPos, szAttrib, nValue); };
    Boolean AddChildAttrib(LPCSTR szAttrib, Int32 nValue) { return x_SetAttrib(m_iPosChild, szAttrib, nValue); };
    Boolean AddSubDoc(LPCSTR szSubDoc) { return x_AddSubDoc(szSubDoc, MNF_DEFAULT); };
    Boolean InsertSubDoc(LPCSTR szSubDoc) { return x_AddSubDoc(szSubDoc, MNF_INSERT); };
    mstr    GetSubDoc() const { return x_GetSubDoc(m_iPos); };
    Boolean AddChildSubDoc(LPCSTR szSubDoc) { return x_AddSubDoc(szSubDoc, MNF_CHILD); };
    Boolean InsertChildSubDoc(LPCSTR szSubDoc) { return x_AddSubDoc(szSubDoc, (MarkupNodeFlagsInternal)((Int32)MNF_CHILD | (Int32)MNF_INSERT)); };
    mstr    GetChildSubDoc() const { return x_GetSubDoc(m_iPosChild); };
    Boolean AddNode(MarkupNodeType nType, LPCSTR szText) { return x_AddNode(nType, szText, MNF_DEFAULT); };
    Boolean InsertNode(MarkupNodeType nType, LPCSTR szText) { return x_AddNode(nType, szText, MNF_INSERT); };


    // Modify
    Boolean RemoveElem();
    Boolean RemoveChildElem();
    Boolean RemoveNode();
    Boolean SetAttrib(LPCSTR szAttrib, mstr& szValue) { return x_SetAttrib(m_iPos, szAttrib, szValue.c_str()); };
    Boolean SetAttrib(LPCSTR szAttrib, LPCSTR szValue) { return x_SetAttrib(m_iPos, szAttrib, szValue); };
    Boolean SetChildAttrib(LPCSTR szAttrib, LPCSTR szValue) { return x_SetAttrib(m_iPosChild, szAttrib, szValue); };
    Boolean SetAttrib(LPCSTR szAttrib, Int32 nValue) { return x_SetAttrib(m_iPos, szAttrib, nValue); };
    Boolean SetChildAttrib(LPCSTR szAttrib, Int32 nValue) { return x_SetAttrib(m_iPosChild, szAttrib, nValue); };
    Boolean SetData(LPCSTR szData, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_SetData(m_iPos, szData, nFlags); };
    Boolean SetChildData(LPCSTR szData, MarkupNodeFlagsInternal nFlags = MNF_DEFAULT) { return x_SetData(m_iPosChild, szData, nFlags); };
    Boolean SetData(Int32 nValue) { return x_SetData(m_iPos, nValue); };
    Boolean SetChildData(Int32 nValue) { return x_SetData(m_iPosChild, nValue); };
    Boolean SetElemContent(LPCSTR szContent) { return x_SetElemContent(szContent); };


    // Utility
    static Boolean ReadTextFile(LPCSTR szFileName, mstr& strDoc, mstr& pstrError, MarkupDocFlags* pnFlags = nullptr);
    static Boolean WriteTextFile(LPCSTR szFileName, mstr& strDoc, mstr& pstrError, MarkupDocFlags* pnFlags = nullptr);
    static mstr EscapeText(LPCSTR szText, Int32 nFlags = 0);
    static mstr UnescapeText(LPCSTR szText, Int32 nTextLength = -1);

    template <size_t size>
    Boolean ReadCharArray(LPCSTR pszAttrib, char(&cItem)[size])
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() < size)
        {
            memcpy(cItem, strData.c_str(), strData.size());
            cItem[strData.size()] = '\0';
            return True;
        }
        else
        {
            return False;
        }
    }

    template <typename T>
    Boolean ReadAttrib(LPCSTR pszAttrib, T* obItem)
    {
        return False;
    }

    template <typename T>
    Boolean ReadAttrib(LPCSTR pszAttrib, T& obItem)
    {
        return False;
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, char& cItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            cItem = static_cast<char>(atoi(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, unsigned char& byItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            byItem = static_cast<unsigned char>(atoi(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, short& shItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            shItem = static_cast<short>(atoi(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, unsigned short& wItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            wItem = static_cast<unsigned short>(atoi(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, Int32& nItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            nItem = atoi(strData.c_str());
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, UInt32& dwItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            dwItem = static_cast<UInt32>(atoi(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, float& fItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            fItem = static_cast<float>(atof(strData.c_str()));
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, double& fItem)
    {
        mstr strData = GetAttrib(pszAttrib);
        if (strData.size() > 0)
        {
            fItem = atof(strData.c_str());
            return True;
        }
        else
        {
            return False;
        }
    }

    template <>
    Boolean ReadAttrib(LPCSTR pszAttrib, mstr& strItem)
    {
        strItem = GetAttrib(pszAttrib);
        return True;
    }

    //protected:
public:

#ifdef _DEBUG
    LPCSTR m_pMainDS;
    LPCSTR m_pChildDS;
#endif

    mstr m_strDoc;
    mstr m_strError;

    Int32 m_iPosParent;
    Int32 m_iPos;
    Int32 m_iPosChild;
    Int32 m_iPosFree;
    Int32 m_iPosDeleted;
    MarkupNodeType m_nNodeType;
    Int32 m_nNodeOffset;
    Int32 m_nNodeLength;
    MarkupDocFlags m_nFlags;

    class AscIISet
    {
    public:
        AscIISet(const char *s)
        {
            memset(m_map, 0, sizeof(m_map));
            for (; *s; ++s)
            {
                m_map[(unsigned char)*s] = 1;
            }
        }
        Boolean Has(char ch) const
        {
            return m_map[(unsigned char)ch] != 0;
        }
    private:
        char m_map[256];
    };

    struct ElemPos
    {
        ElemPos() {};
        ElemPos(const ElemPos& pos) { *this = pos; };
        enum { EP_STBITS = 22, EP_STMASK = 0x2fffff, EP_LEVMASK = 0xffff };
        Int32 StartTagLen() const { return (nTagLengths & EP_STMASK); };
        void SetStartTagLen(Int32 n) { nTagLengths = (nTagLengths & ~EP_STMASK) + n; };
        void AdjustStartTagLen(Int32 n) { nTagLengths += n; };
        Int32 EndTagLen() const { return (nTagLengths >> EP_STBITS); };
        void SetEndTagLen(Int32 n) { nTagLengths = (nTagLengths & EP_STMASK) + (n << EP_STBITS); };
        Boolean IsEmptyElement() { return StartTagLen() == nLength; };
        Int32 StartContent() const { return nStart + StartTagLen(); };
        Int32 ContentLen() const { return nLength - StartTagLen() - EndTagLen(); };
        Int32 StartAfter() const { return nStart + nLength; };
        Int32 Level() const { return nFlags & EP_LEVMASK; };
        void SetLevel(Int32 nLev) { nFlags = (nFlags & ~EP_LEVMASK) | nLev; };
        void ClearVirtualParent() { memset(this, 0, sizeof(ElemPos)); };

        // Memory size: 8 32-bit integers == 32 bytes
        Int32 nStart;
        Int32 nLength;
        Int32 nTagLengths; // 22 bits 4MB limit for start tag, 10 bits 1K limit for end tag
        Int32 nFlags; // 16 bits flags, 16 bits level 65536 depth limit
        Int32 iElemParent;
        Int32 iElemChild; // first child
        Int32 iElemNext;
        Int32 iElemPrev; // if this is first child, iElemPrev points to last
    };

    struct NodePos
    {
        NodePos() {};
        NodePos(Int32 n) { nFlags = n; nNodeType = 0; nStart = 0; nLength = 0; };
        Int32 nNodeType;
        Int32 nStart;
        Int32 nLength;
        Int32 nFlags;
        mstr strMeta;
    };

    struct TokenPos
    {
        TokenPos(LPCSTR sz, Int32 n) { Clear(); szDoc = sz; nTokenFlags = n; };
        TokenPos(const mstr& str, Int32 n) { Clear(); szDoc = str.c_str(); nTokenFlags = n; };
        void Clear() { nL = 0; nR = -1; nNext = 0; };
        Int32 Length() const { return nR - nL + 1; };
        Boolean Match(const mstr& strName) { return Match(strName.c_str()); };
        Boolean Match(LPCSTR szName)
        {
            Int32 nLen = nR - nL + 1;
            if (nTokenFlags & MDF_IGNORECASE)
                return (
#ifdef WIN32
                (_strnicmp(&szDoc[nL], szName, nLen) == 0)
#else
                    (strncasecmp(&szDoc[nL], szName, nLen) == 0)
#endif
                    && (szName[nLen] == '\0' || strchr(" =/[]", szName[nLen])));
            else
                return ((strncmp(&szDoc[nL], szName, nLen) == 0)
                    && (szName[nLen] == '\0' || strchr(" =/[]", szName[nLen])));
        };
        Int32 nL;
        Int32 nR;
        Int32 nNext;
        LPCSTR szDoc;
        Int32 nTokenFlags;
        Int32 nPreSpaceStart;
        Int32 nPreSpaceLength;
    };

    struct SavedPos
    {
        SavedPos() { nSavedPosFlags = 0; iPos = 0; };
        mstr strName;
        Int32 iPos;
        Int32 nSavedPosFlags;
    };

    struct SavedPosMap
    {
        SavedPosMap() { pTable = nullptr; };
        ~SavedPosMap() { RemoveAll(); };
        void RemoveAll() { if (pTable) Release(); pTable = nullptr; };
        enum { SPM_SIZE = 7, SPM_MAIN = 1, SPM_CHILD = 2, SPM_USED = 4, SPM_LAST = 8 };
        void Release() { for (Int32 n = 0; n < SPM_SIZE; ++n) if (pTable[n]) delete[] pTable[n]; delete[] pTable; };
        void AllocMapTable() { pTable = new SavedPos*[SPM_SIZE]; for (Int32 n = 0; n < SPM_SIZE; ++n) pTable[n] = nullptr; };
        Int32 Hash(LPCSTR szName) { Int32 n = 0; while (*szName) n += *szName++; return n % SPM_SIZE; };
        SavedPos** pTable;
    };
    SavedPosMap m_mapSavedPos;

    struct PosArray
    {
        PosArray() { Clear(); };
        ~PosArray() { Release(); };
        enum { PA_SEGBITS = 16, PA_SEGMASK = 0xffff };
        void RemoveAll() { Release(); Clear(); };
        void Release() { for (Int32 n = 0; n < SegsUsed(); ++n) delete[](char*)pSegs[n]; if (pSegs) delete[](char*)pSegs; };
        void Clear() { nSegs = 0; nSize = 0; pSegs = nullptr; };
        Int32 GetSize() const { return nSize; };
        Int32 SegsUsed() const { return ((nSize - 1) >> PA_SEGBITS) + 1; };
        ElemPos& operator[](Int32 n) const { return pSegs[n >> PA_SEGBITS][n&PA_SEGMASK]; };
        ElemPos** pSegs;
        Int32 nSize;
        Int32 nSegs;
    };
    PosArray m_aPos;

    struct NodeStack
    {
        NodeStack() { nTop = -1; nSize = 0; pN = nullptr; };
        ~NodeStack() { if (pN) delete[] pN; };
        NodePos& Top() { return pN[nTop]; };
        NodePos& At(Int32 n) { return pN[n]; };
        void Add() { ++nTop; if (nTop == nSize) Alloc(nSize * 2 + 6); };
        void Remove() { --nTop; };
        Int32 TopIndex() { return nTop; };
    protected:
        void Alloc(Int32 nNewSize) { NodePos* pNNew = new NodePos[nNewSize]; Copy(pNNew); nSize = nNewSize; };
        void Copy(NodePos* pNNew) { for (Int32 n = 0; n < nSize; ++n) pNNew[n] = pN[n]; if (pN) delete[] pN; pN = pNNew; };
        NodePos* pN;
        Int32 nSize;
        Int32 nTop;
    };

    DINLINE void BackupPos(Int32& iPosParent, Int32& iPos, Int32& iPosChild)
    {
        iPosParent = m_iPosParent;
        iPos = m_iPos;
        iPosChild = m_iPosChild;
    }
    DINLINE void _SetPos(Int32 iPosParent, Int32 iPos, Int32 iPosChild)
    {
        m_iPosParent = iPosParent;
        m_iPos = iPos;
        m_iPosChild = iPosChild;
        m_nNodeOffset = 0;
        m_nNodeLength = 0;
        m_nNodeType = iPos ? MNT_ELEMENT : MNT_INVALID;
    };

    Int32 x_GetFreePos()
    {
        if (m_iPosFree == m_aPos.GetSize())
            x_AllocPosArray();
        return m_iPosFree++;
    };
    Boolean x_AllocPosArray(Int32 nNewSize = 0);

    void InitDocFlags()
    {
        // To always ignore case, define MARKUP_IGNORECASE
#ifdef MARKUP_IGNORECASE
        m_nFlags = MDF_IGNORECASE;
#else
        m_nFlags = MDF_DEFAULT;
#endif
    };

    Boolean x_ParseDoc();
    Int32 x_ParseElem(Int32 iPos, TokenPos& token);
    static Boolean x_FindAny(LPCSTR szDoc, Int32& nChar);
    static Boolean x_FindName(TokenPos& token);
    static mstr x_GetToken(const TokenPos& token);
    Int32 x_FindElem(Int32 iPosParent, Int32 iPos, LPCSTR szPath) const;
    mstr x_GetPath(Int32 iPos) const;
    mstr x_GetTagName(Int32 iPos) const;
    mstr x_GetData(Int32 iPos) const;
    mstr x_GetAttrib(Int32 iPos, LPCSTR szAttrib) const;
    static mstr x_EncodeCDATASection(LPCSTR szData);
    Boolean x_AddElem(LPCSTR szName, LPCSTR szValue, Int32 nFlags) { return x_AddElem(szName, szValue, (MarkupNodeFlagsInternal)nFlags); }
    Boolean x_AddElem(LPCSTR szName, Int32 nValue, Int32 nFlags) { return x_AddElem(szName, nValue, (MarkupNodeFlagsInternal)nFlags); }
    Boolean x_AddElem(LPCSTR szName, LPCSTR szValue, MarkupNodeFlagsInternal nFlags);
    Boolean x_AddElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags);
    mstr x_GetSubDoc(Int32 iPos) const;
    Boolean x_AddSubDoc(LPCSTR szSubDoc, MarkupNodeFlagsInternal nFlags);
    static Boolean x_FindAttrib(TokenPos& token, LPCSTR szAttrib, Int32 n = 0);
    Boolean x_SetAttrib(Int32 iPos, LPCSTR szAttrib, LPCSTR szValue);
    Boolean x_SetAttrib(Int32 iPos, LPCSTR szAttrib, Int32 nValue);
    Boolean x_AddNode(MarkupNodeType nNodeType, LPCSTR szText, Int32 nFlags) { return x_AddNode(nNodeType, szText, (MarkupNodeFlagsInternal)nFlags); }
    Boolean x_AddNode(MarkupNodeType nNodeType, LPCSTR szText, MarkupNodeFlagsInternal nFlags);
    void x_RemoveNode(Int32 iPosParent, Int32& iPos, MarkupNodeType& nNodeType, Int32& nNodeOffset, Int32& nNodeLength);
    void x_AdjustForNode(Int32 iPosParent, Int32 iPos, Int32 nShift);
    static Boolean x_CreateNode(mstr& strNode, MarkupNodeType nNodeType, LPCSTR szText);
    Int32 x_InsertNew(Int32 iPosParent, Int32& iPosRel, NodePos& node);
    void x_LinkElem(Int32 iPosParent, Int32 iPosBefore, Int32 iPos);
    Int32 x_UnlinkElem(Int32 iPos);
    Int32 x_ReleaseSubDoc(Int32 iPos);
    Int32 x_ReleasePos(Int32 iPos);
    void x_CheckSavedPos();
    static MarkupNodeType x_ParseNode(TokenPos& token, NodePos& node);
    Boolean x_SetData(Int32 iPos, LPCSTR szData, MarkupNodeFlagsInternal nFlags);
    Boolean x_SetData(Int32 iPos, Int32 nValue);
    Int32 x_RemoveElem(Int32 iPos);
    mstr x_GetElemContent(Int32 iPos) const;
    Boolean x_SetElemContent(LPCSTR szContent);
    void x_DocChange(Int32 nLeft, Int32 nReplace, const mstr& strInsert);
    void x_Adjust(Int32 iPos, Int32 nShift, Boolean bAfterPos = False);
};


