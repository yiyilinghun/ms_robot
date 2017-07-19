#include "Precompiled.h"

MsXmlAttribReader::MsXmlAttribReader(MsXmlReader* xMsXmlReader)
    : m_MsXmlReader(xMsXmlReader)
{
    ;
}

//MsXmlAttribReader& MsXmlAttribReader::operator[](mstr& szName)
//{
//    return (*this)[szName.c_str()];
//}

mstr MsXmlAttribReader::operator[](LPCSTR szName)
{
    return m_MsXmlReader->GetAttrib(szName);
};

MsXmlReader::MsXmlReader()
    : m_IsLastOK(False)
    , m_IsOpenOK(False)
    , Attribs(this)
{
    this->SetDoc(nullptr); this->InitDocFlags();
}

MsXmlReader::MsXmlReader(LPCSTR szDoc)
    : m_IsLastOK(False)
    , m_IsOpenOK(False)
    , Attribs(this)
{
    this->SetDoc(szDoc); this->InitDocFlags();
}

MsXmlReader::MsXmlReader(MarkupDocFlags nFlags)
    : m_IsLastOK(False)
    , m_IsOpenOK(False)
    , Attribs(this)
{
    this->SetDoc(nullptr); m_nFlags = nFlags;
}

MsXmlReader::MsXmlReader(const MsXmlReader& markup)
    : m_IsLastOK(False)
    , m_IsOpenOK(False)
    , Attribs(this)
{
    *this = markup;
}

using namespace std;

// Customization
#define X_EOL "\r\n"                // can be \r\n or \n or empty
#define X_EOLLEN (sizeof(X_EOL)-1)  // mstr length of X_EOL
#define X_ATTRIBQUOTE "\""          // can be double or single quote

void MsXmlReader::operator=(const MsXmlReader& markup)
{
    m_iPosParent = markup.m_iPosParent;
    m_iPos = markup.m_iPos;
    m_iPosChild = markup.m_iPosChild;
    m_iPosFree = markup.m_iPosFree;
    m_iPosDeleted = markup.m_iPosDeleted;
    m_nNodeType = markup.m_nNodeType;
    m_nNodeOffset = markup.m_nNodeOffset;
    m_nNodeLength = markup.m_nNodeLength;
    m_strDoc = markup.m_strDoc;
    m_strError = markup.m_strError;
    m_nFlags = markup.m_nFlags;

    // Copy used part of the index array
    m_aPos.RemoveAll();
    m_aPos.nSize = m_iPosFree;
    if (m_aPos.nSize < 8)
    {
        m_aPos.nSize = 8;
    }
    m_aPos.nSegs = m_aPos.SegsUsed();
    if (m_aPos.nSegs)
    {
        m_aPos.pSegs = (ElemPos**)(NEW CHAR[m_aPos.nSegs * sizeof(CHAR*)]);
        Int32 nSegSize = 1 << m_aPos.PA_SEGBITS;
        for (Int32 nSeg = 0; nSeg < m_aPos.nSegs; ++nSeg)
        {
            if (nSeg + 1 == m_aPos.nSegs)
                nSegSize = m_aPos.GetSize() - (nSeg << m_aPos.PA_SEGBITS);
            m_aPos.pSegs[nSeg] = (ElemPos*)(NEW CHAR[nSegSize * sizeof(ElemPos)]);
            memcpy(m_aPos.pSegs[nSeg], markup.m_aPos.pSegs[nSeg], nSegSize * sizeof(ElemPos));
        }
    }

    // Copy SavedPos map
    m_mapSavedPos.RemoveAll();
    if (markup.m_mapSavedPos.pTable)
    {
        m_mapSavedPos.AllocMapTable();
        for (Int32 nSlot = 0; nSlot < SavedPosMap::SPM_SIZE; ++nSlot)
        {
            SavedPos* pCopySavedPos = markup.m_mapSavedPos.pTable[nSlot];
            if (pCopySavedPos)
            {
                Int32 nCount = 0;
                while (pCopySavedPos[nCount].nSavedPosFlags & SavedPosMap::SPM_USED)
                {
                    ++nCount;
                    if (pCopySavedPos[nCount - 1].nSavedPosFlags & SavedPosMap::SPM_LAST)
                        break;
                }
                SavedPos* pNewSavedPos = NEW SavedPos[nCount];
                for (Int32 nCopy = 0; nCopy < nCount; ++nCopy)
                    pNewSavedPos[nCopy] = pCopySavedPos[nCopy];
                pNewSavedPos[nCount - 1].nSavedPosFlags |= SavedPosMap::SPM_LAST;
                m_mapSavedPos.pTable[nSlot] = pNewSavedPos;
            }
        }
    }

    //MARKUP_SETDEBUGSTATE;
}

Boolean MsXmlReader::SetDoc(LPCSTR szDoc)
{
    // Set document text
    if (szDoc)
    {
        m_strDoc = szDoc;
    }
    else
    {
        m_strDoc.erase();
    }
    m_strError.erase();
    m_IsOpenOK = x_ParseDoc();
    return m_IsOpenOK;
};

Boolean MsXmlReader::IsWellFormed()
{
    if (m_aPos.GetSize())
    {
        auto& xTemp = m_aPos[0];
        if (!(xTemp.nFlags & MNF_ILLFORMED))
        {
            if (xTemp.iElemChild)
            {
                if (!m_aPos[xTemp.iElemChild].iElemNext)
                {
                    return True;
                }
            }
        }
    }
    return False;
}

Boolean MsXmlReader::Load(LPCSTR szFileName)
{
    if (!ReadTextFile(szFileName, m_strDoc, m_strError, &m_nFlags))
    {
        return False;
    }
    m_IsOpenOK = x_ParseDoc();
    return m_IsOpenOK;
}

Boolean MsXmlReader::LoadFromZip(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword)
{
    ZIP_RES_INFO xResInfo;
    if (ZipUtils::ExtractZipToMemoryByName(xZipName, szXmlPathName, xResInfo, szPassword))
    {
        return this->LoadFromMem(xResInfo.m_DataAddr, xResInfo.m_DataLen);
    }
    return False;
}

Boolean MsXmlReader::LoadFromMem(LPCSTR pszBuf, Int32 nSize)
{
    m_strDoc.assign(pszBuf, nSize);
    m_IsOpenOK = x_ParseDoc();
    return m_IsOpenOK;
}

Boolean MsXmlReader::LoadFromMem(LPBYTE pszBuf, Int32 nSize)
{
    m_strDoc.assign((LPCSTR)pszBuf, nSize);
    m_IsOpenOK = x_ParseDoc();
    return m_IsOpenOK;
}

Boolean MsXmlReader::ReadTextFile(LPCSTR szFileName, mstr& strDoc, mstr& pstrError, MarkupDocFlags* pnFlags)
{
    // Static common method to load text file into strDoc
    //
    // Open file to read binary
    FILE* fp = nullptr;
    fopen_s(&fp, szFileName, "rb");
    if (!fp)
    {
        CHAR szBuff[KB_SIZE];
        strerror_s(szBuff, errno);
        pstrError = szBuff;
        return False;
    }

    // Set flags to 0 unless flags argument provided
    MarkupDocFlags nFlags = pnFlags ? *pnFlags : MDF_DEFAULT;
    CHAR szDescBOM[20] = { 0 };
    CHAR szResult[100];
    strDoc.erase();

    // Get file length
    fseek(fp, 0, SEEK_END);
    Int32 nFileByteLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);


    // Read file directly
    if (nFileByteLen)
    {
        CHAR* pszBuffer = NEW CHAR[nFileByteLen];
        fread(pszBuffer, nFileByteLen, 1, fp);
        strDoc.assign(pszBuffer, nFileByteLen);
        delete[] pszBuffer;
    }
    sprintf_s(szResult, "%s%d bytes", szDescBOM, nFileByteLen);

    pstrError = szResult;

    fclose(fp);
    if (pnFlags)
    {
        *pnFlags = nFlags;
    }
    return True;
}

Boolean MsXmlReader::Save(LPCSTR szFileName)
{
    return WriteTextFile(szFileName, m_strDoc, m_strError, &m_nFlags);
}

Boolean MsXmlReader::WriteTextFile(LPCSTR szFileName, mstr& strDoc, mstr& pstrError, MarkupDocFlags* pnFlags)
{
    // Static common method to save strDoc to text file
    //
    // Open file to write binary
    Boolean bSuccess = True;
    FILE* fp = nullptr;
    fopen_s(&fp, szFileName, "wb");
    if (!fp)
    {
        CHAR szBuff[KB_SIZE];
        strerror_s(szBuff, errno);
        pstrError = szBuff;
        //if (pstrError)
            //*pstrError = strerror_s(errno);
        return False;
    }

    // Set flags to 0 unless flags argument provided
    MarkupDocFlags nFlags = pnFlags ? *pnFlags : MDF_DEFAULT;
    CHAR szDescBOM[20] = { 0 };
    CHAR szResult[100];

    // Get document length
    Int32 nDocLength = (Int32)strDoc.size();


    if (nDocLength)
        bSuccess = (fwrite(strDoc.c_str(), nDocLength, 1, fp) == 1);
    sprintf_s(szResult, "%s%d bytes", szDescBOM, nDocLength);
    //if (pstrError)
    pstrError = szResult;

    if (!bSuccess)
    {
        CHAR szBuff[KB_SIZE];
        strerror_s(szBuff, errno);
        pstrError = szBuff;
        //*pstrError = strerror(errno);
    }
    fclose(fp);
    if (pnFlags)
    {
        *pnFlags = nFlags;
    }
    return bSuccess;
}

Boolean MsXmlReader::SelectRootElem(LPCSTR szName)
{
    Int32 xTempPos = m_aPos[0].iElemChild;
    if (szName == nullptr || !szName[0])
    {
        _SetPos(m_aPos[xTempPos].iElemParent, xTempPos, m_aPos[xTempPos].iElemChild);
        return True;
    }

    // Search
    TokenPos token(m_strDoc, m_nFlags);
    while (xTempPos)
    {
        // Compare tag name
        token.nNext = m_aPos[xTempPos].nStart + 1;
        x_FindName(token); // Locate tag name
        if (token.Match(szName))
        {
            _SetPos(m_aPos[xTempPos].iElemParent, xTempPos, m_aPos[xTempPos].iElemChild);
            return True;
        }
        xTempPos = m_aPos[xTempPos].iElemNext;
    }
    return False;
}

Boolean MsXmlReader::FindNextElem(LPCSTR szName)
{
    if (!m_iPos) { SelectRootElem(); }

    //Int32 xTempPos = m_aPos[m_iPos + m_iPosChild].iElemNext;
    Int32 xTempPos = m_aPos[m_iPos].iElemNext;
    if (szName == nullptr || !szName[0])
    {
        _SetPos(m_aPos[xTempPos].iElemParent, xTempPos, m_aPos[xTempPos].iElemChild);
        return m_iPos != 0;
    }

    // Search
    TokenPos token(m_strDoc, m_nFlags);
    while (xTempPos)
    {
        // Compare tag name
        token.nNext = m_aPos[xTempPos].nStart + 1;
        x_FindName(token); // Locate tag name
        if (token.Match(szName))
        {
            _SetPos(m_aPos[xTempPos].iElemParent, xTempPos, m_aPos[xTempPos].iElemChild);
            return m_iPos != 0;
        }
        xTempPos = m_aPos[xTempPos].iElemNext;
    }
    return False;
}

Boolean MsXmlReader::FindChildElem(LPCSTR szName)
{
    if (!m_iPos) { this->SelectRootElem(); }

    Int32 iPosChild = x_FindElem(m_iPos, 0, szName);
    if (iPosChild)
    {
        _SetPos(m_aPos[iPosChild].iElemParent, iPosChild, m_aPos[iPosChild].iElemChild);
        return True;
    }

    return False;
}

void MsXmlReader::SelectParentElem()
{
    ElemPos& xPos = m_aPos[m_iPos];
    _SetPos(m_aPos[xPos.iElemParent].iElemParent, xPos.iElemParent, m_aPos[xPos.iElemParent].iElemChild);
}

mstr MsXmlReader::EscapeText(LPCSTR szText, Int32 nFlags)
{
    // Convert text as seen outside XML document to XML friendly
    // replacing special characters with ampersand escape codes
    // E.g. convert "6>7" to "6&gt;7"
    //
    // &lt;   less than
    // &amp;  ampersand
    // &gt;   greater than
    //
    // and for attributes:
    //
    // &apos; apostrophe or single quote
    // &quot; double quote
    //
    static LPCSTR szaReplace[] = { "&lt;","&amp;","&gt;","&apos;","&quot;" };
    LPCSTR pFind = (nFlags&MNF_ESCAPEQUOTES) ? "<&>\'\"" : "<&>";
    mstr strText;
    LPCSTR pSource = szText;
    Int32 nDestSize = (Int32)strlen(pSource);
    nDestSize += nDestSize / 10 + 7;
    strText.reserve(nDestSize);
    CHAR cSource = *pSource;
    LPCSTR pFound;
    while (cSource)
    {
        if ((pFound = strchr(pFind, cSource)) != nullptr)
        {
            Boolean bIgnoreAmpersand = False;
            if ((nFlags&MNF_WITHREFS) && *pFound == '&')
            {
                // Do not replace ampersand if it is start of any entity reference
                // &[#_:A-Za-zU][_:-.A-Za-z0-9U]*; where U is > 0x7f
                LPCSTR pCheckEntity = pSource;
                ++pCheckEntity;
                Int32 c = *pCheckEntity;
                if ((c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z')
                    || '#' == c || '_' == c || ':' == c || (Byte)c > 0x7f)
                {
                    while (1)
                    {
                        ++pCheckEntity;
                        c = *pCheckEntity;
                        if (c == ';')
                        {
                            Int32 nEntityLen = (Int32)(pCheckEntity - pSource) + 1;
                            strText.append(pSource, nEntityLen);
                            pSource = pCheckEntity;
                            bIgnoreAmpersand = True;
                        }
                        else if ((c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') || (c >= '0'&&c <= '9')
                            || '_' == c || ':' == c || '-' == c || '.' == c || (Byte)c > 0x7f)
                        {
                            continue;
                        }
                        break;
                    }
                }
            }
            if (!bIgnoreAmpersand)
            {
                pFound = szaReplace[pFound - pFind];
                strText.append(pFound);
            }
        }
        else
        {
            strText += cSource;
        }
        ++pSource;
        cSource = *pSource;
    }
    return strText;
}

mstr MsXmlReader::UnescapeText(LPCSTR szText, Int32 nTextLength /*=-1*/)
{
    // Convert XML friendly text to text as seen outside XML document
    // ampersand escape codes replaced with special characters e.g. convert "6&gt;7" to "6>7"
    // ampersand numeric codes replaced with character e.g. convert &#60; to <
    // Conveniently the result is always the same or shorter in UINT8 length
    //
    static LPCSTR szaCode[] = { "lt;","amp;","gt;","apos;","quot;" };
    static const Int32 anCodeLen[] = { 3,4,3,5,5 };
    static LPCSTR szSymbol = "<&>\'\"";
    mstr strText;
    LPCSTR pSource = szText;
    if (-1 == nTextLength)
        nTextLength = (Int32)strlen(szText);
    strText.reserve(nTextLength);
    Int32 nChar = 0;
    while (nChar < nTextLength)
    {
        if ('&' == pSource[nChar])
        {
            Boolean bCodeConverted = False;

            // Is it a numeric character reference?
            if ('#' == pSource[nChar + 1])
            {
                // Is it a hex number?
                Int32 nBase = 10;
                Int32 nNumericChar = nChar + 2;
                CHAR cChar = pSource[nNumericChar];
                if ('x' == cChar)
                {
                    ++nNumericChar;
                    cChar = pSource[nNumericChar];
                    nBase = 16;
                }

                // Look for terminating semi-colon within 7 characters
                Int32 nCodeLen = 0;
                while (nCodeLen < 7 && cChar && cChar != ';')
                {
                    // only ASCII digits 0-9, A-F, a-f expected
                    ++nCodeLen;
                    cChar = pSource[nNumericChar + nCodeLen];
                }

                // Process unicode
                if (':' == cChar)
                {
                    Int32 nUnicode = strtol(&pSource[nNumericChar], nullptr, nBase);
                    /* MBCS
                    Int32 nMBLen = wctomb( &pDest[nLen], (wchar_t)nUnicode );
                    if ( nMBLen > 0 )
                        nLen += nMBLen;
                    else
                        nUnicode = 0;
                    */
                    if (nUnicode < 0x80)
                        strText += (CHAR)nUnicode;
                    else if (nUnicode < 0x800)
                    {
                        // Convert to 2-UINT8 UTF-8
                        strText += (CHAR)(((nUnicode & 0x7c0) >> 6) | 0xc0);
                        strText += (CHAR)((nUnicode & 0x3f) | 0x80);
                    }
                    else
                    {
                        // Convert to 3-UINT8 UTF-8
                        strText += (CHAR)(((nUnicode & 0xf000) >> 12) | 0xe0);
                        strText += (CHAR)(((nUnicode & 0xfc0) >> 6) | 0x80);
                        strText += (CHAR)((nUnicode & 0x3f) | 0x80);
                    }
                    if (nUnicode)
                    {
                        // Increment index past ampersand semi-colon
                        nChar = nNumericChar + nCodeLen + 1;
                        bCodeConverted = True;
                    }
                }
            }
            else // does not start with #
            {
                // Look for matching &code;
                for (Int32 nMatch = 0; nMatch < 5; ++nMatch)
                {
                    if (nChar < nTextLength - anCodeLen[nMatch]
                        && strncmp(szaCode[nMatch], &pSource[nChar + 1], anCodeLen[nMatch]) == 0)
                    {
                        // Insert symbol and increment index past ampersand semi-colon
                        strText += szSymbol[nMatch];
                        nChar += anCodeLen[nMatch] + 1;
                        bCodeConverted = True;
                        break;
                    }
                }
            }

            // If the code is not converted, leave it as is
            if (!bCodeConverted)
            {
                strText += '&';
                ++nChar;
            }
        }
        else // not &
        {
            strText += pSource[nChar];
            ++nChar;
        }
    }
    return strText;
}


Int32 MsXmlReader::FindNode(Int32 nType)
{
    // Change current node position only if a node is found
    // If nType is 0 find any node, otherwise find node of type nType
    // Return type of node or 0 if not found
    // If found node is an element, change m_iPos

    // Determine where in document to start scanning for node
    MarkupNodeType nTypeFound = MNT_INVALID;
    Int32 nNodeOffset = m_nNodeOffset;
    if (m_nNodeType > 1)
    {
        // By-pass current node
        nNodeOffset += m_nNodeLength;
    }
    else
    {
        // Set position to begin looking for node
        nNodeOffset = 0; // default to start of document
        if (m_iPos)
        {
            // After element
            nNodeOffset = m_aPos[m_iPos].StartAfter();
        }
        else if (m_iPosParent)
        {
            // Immediately after start tag of parent
            if (m_aPos[m_iPosParent].IsEmptyElement())
                return 0;
            else
                nNodeOffset = m_aPos[m_iPosParent].StartContent();
        }
    }

    // Get nodes until we find what we're looking for
    Int32 iPosNew = m_iPos;
    TokenPos token(m_strDoc, m_nFlags);
    NodePos node;
    token.nNext = nNodeOffset;
    do
    {
        nNodeOffset = token.nNext;
        nTypeFound = x_ParseNode(token, node);
        if (nTypeFound == 0)
        {
            // Check if we have reached the end of the parent element
            // Otherwise it is a lone end tag
            if (m_iPosParent && nNodeOffset == m_aPos[m_iPosParent].StartContent()
                + m_aPos[m_iPosParent].ContentLen())
                return 0;
            nTypeFound = MNT_LONE_END_TAG;
        }
        else if (nTypeFound < 0)
        {
            if (nTypeFound == -2)
                return 0;
            // -1 is node error
            nTypeFound = MNT_NODE_ERROR;
        }
        else if (nTypeFound == MNT_ELEMENT)
        {
            if (iPosNew)
                iPosNew = m_aPos[iPosNew].iElemNext;
            else
                iPosNew = m_aPos[m_iPosParent].iElemChild;
            if (!iPosNew)
                return 0;
            if (!nType || (nType & nTypeFound))
            {
                // Found element node, move position to this element
                _SetPos(m_iPosParent, iPosNew, 0);
                return m_nNodeType;
            }
            token.nNext = m_aPos[iPosNew].StartAfter();
        }
    } while (nType && !(nType & nTypeFound));

    m_iPos = iPosNew;
    m_iPosChild = 0;
    m_nNodeOffset = nNodeOffset;
    m_nNodeLength = token.nNext - nNodeOffset;
    m_nNodeType = nTypeFound;
    //MARKUP_SETDEBUGSTATE;
    return m_nNodeType;
}

Boolean MsXmlReader::RemoveNode()
{
    if (m_iPos || m_nNodeLength)
    {
        x_RemoveNode(m_iPosParent, m_iPos, m_nNodeType, m_nNodeOffset, m_nNodeLength);
        m_iPosChild = 0;
        //MARKUP_SETDEBUGSTATE;
        return True;
    }
    return False;
}

mstr MsXmlReader::GetTagName() const
{
    // Return the tag name at the current main position
    mstr strTagName;

    // This method is primarily for elements, however
    // it does return something for certain other nodes
    if (m_nNodeLength)
    {
        switch (m_nNodeType)
        {
            case MNT_PROCESSING_INSTRUCTION:
            case MNT_LONE_END_TAG:
            {
                // <?target or </tagname
                TokenPos token(m_strDoc, m_nFlags);
                token.nNext = m_nNodeOffset + 2;
                if (x_FindName(token))
                    strTagName = x_GetToken(token);
            }
            break;
            case MNT_COMMENT:
                strTagName = "#comment";
                break;
            case MNT_CDATA_SECTION:
                strTagName = "#cdata-section";
                break;
            case MNT_DOCUMENT_TYPE:
            {
                // <!DOCTYPE name
                TokenPos token(m_strDoc, m_nFlags);
                token.nNext = m_nNodeOffset + 2;
                if (x_FindName(token) && x_FindName(token))
                    strTagName = x_GetToken(token);
            }
            break;
            case MNT_TEXT:
            case MNT_WHITESPACE:
                strTagName = "#text";
                break;
        }
        return strTagName;
    }

    if (m_iPos)
        strTagName = x_GetTagName(m_iPos);
    return strTagName;
}

Boolean MsXmlReader::IntoElem()
{
    // If there is no child position and IntoElem is called it will succeed in release 6.3
    // (A subsequent call to FindElem will find the first element)
    // The following short-hand behavior was never part of EDOM and was misleading
    // It would find a child element if there was no current child element position and go into it
    // It is removed in release 6.3, this change is NOT backwards compatible!
    // if ( ! m_iPosChild )
    //	FindChildElem();

    if (m_iPos && m_nNodeType == MNT_ELEMENT)
    {
        _SetPos(m_iPos, m_iPosChild, 0);
        return True;
    }
    return False;
}

Boolean MsXmlReader::OutOfElem()
{
    // Go to parent element
    if (m_iPosParent)
    {
        _SetPos(m_aPos[m_iPosParent].iElemParent, m_iPosParent, m_iPos);
        return True;
    }
    return False;
}

mstr MsXmlReader::GetAttribName(Int32 n) const
{
    // Return nth attribute name of main position
    TokenPos token(m_strDoc, m_nFlags);
    if (m_iPos && m_nNodeType == MNT_ELEMENT)
        token.nNext = m_aPos[m_iPos].nStart + 1;
    else if (m_nNodeLength && m_nNodeType == MNT_PROCESSING_INSTRUCTION)
        token.nNext = m_nNodeOffset + 2;
    else
        return "";
    if (x_FindAttrib(token, nullptr, n))
        return x_GetToken(token);
    return "";
}

Boolean MsXmlReader::SavePos(LPCSTR szPosName)
{
    // Save current element position in saved position map
    if (szPosName)
    {
        SavedPos savedpos;
        if (szPosName)
            savedpos.strName = szPosName;
        if (m_iPosChild)
        {
            savedpos.iPos = m_iPosChild;
            savedpos.nSavedPosFlags |= SavedPosMap::SPM_CHILD;
        }
        else if (m_iPos)
        {
            savedpos.iPos = m_iPos;
            savedpos.nSavedPosFlags |= SavedPosMap::SPM_MAIN;
        }
        else
        {
            savedpos.iPos = m_iPosParent;
        }
        savedpos.nSavedPosFlags |= SavedPosMap::SPM_USED;

        if (!m_mapSavedPos.pTable)
            m_mapSavedPos.AllocMapTable();
        Int32 nSlot = m_mapSavedPos.Hash(szPosName);
        SavedPos* pSavedPos = m_mapSavedPos.pTable[nSlot];
        Int32 nOffset = 0;
        if (!pSavedPos)
        {
            pSavedPos = NEW SavedPos[2];
            pSavedPos[1].nSavedPosFlags = SavedPosMap::SPM_LAST;
            m_mapSavedPos.pTable[nSlot] = pSavedPos;
        }
        else
        {
            while (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_USED)
            {
                if (pSavedPos[nOffset].strName == szPosName)
                    break;
                if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_LAST)
                {
                    Int32 nNewSize = (nOffset + 6) * 2;
                    SavedPos* pNewSavedPos = NEW SavedPos[nNewSize];
                    for (Int32 nCopy = 0; nCopy <= nOffset; ++nCopy)
                        pNewSavedPos[nCopy] = pSavedPos[nCopy];
                    pNewSavedPos[nOffset].nSavedPosFlags ^= SavedPosMap::SPM_LAST;
                    pNewSavedPos[nNewSize - 1].nSavedPosFlags = SavedPosMap::SPM_LAST;
                    delete[] pSavedPos;
                    pSavedPos = pNewSavedPos;
                    m_mapSavedPos.pTable[nSlot] = pSavedPos;
                    ++nOffset;
                    break;
                }
                ++nOffset;
            }
        }
        if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_LAST)
            savedpos.nSavedPosFlags |= SavedPosMap::SPM_LAST;
        pSavedPos[nOffset] = savedpos;

        /*
        // To review hash table balance, uncomment and watch strBalance
        mstr strBalance;
        CHAR szSlot[20];
        for ( nSlot=0; nSlot < SavedPosMap::SPM_SIZE; ++nSlot )
        {
            pSavedPos = m_mapSavedPos.pTable[nSlot];
            Int32 nCount = 0;
            while ( pSavedPos && pSavedPos->nSavedPosFlags & SavedPosMap::SPM_USED )
            {
                ++nCount;
                if ( pSavedPos->nSavedPosFlags & SavedPosMap::SPM_LAST )
                    break;
                ++pSavedPos;
            }
            sprintf_s( szSlot, "%d ", nCount );
            strBalance += szSlot;
        }
        */

        return True;
    }
    return False;
}

Boolean MsXmlReader::RestorePos(LPCSTR szPosName)
{
    // Restore element position if found in saved position map
    if (szPosName && m_mapSavedPos.pTable)
    {
        Int32 nSlot = m_mapSavedPos.Hash(szPosName);
        SavedPos* pSavedPos = m_mapSavedPos.pTable[nSlot];
        if (pSavedPos)
        {
            Int32 nOffset = 0;
            while (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_USED)
            {
                if (pSavedPos[nOffset].strName == szPosName)
                {
                    Int32 i = pSavedPos[nOffset].iPos;
                    if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_CHILD)
                        _SetPos(m_aPos[m_aPos[i].iElemParent].iElemParent, m_aPos[i].iElemParent, i);
                    else if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_MAIN)
                        _SetPos(m_aPos[i].iElemParent, i, 0);
                    else
                        _SetPos(i, 0, 0);
                    return True;
                }
                if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_LAST)
                    break;
                ++nOffset;
            }
        }
    }
    return False;
}

Boolean MsXmlReader::RemoveElem()
{
    // Remove current main position element
    if (m_iPos && m_nNodeType == MNT_ELEMENT)
    {
        Int32 iPos = x_RemoveElem(m_iPos);
        _SetPos(m_iPosParent, iPos, 0);
        return True;
    }
    return False;
}

Boolean MsXmlReader::RemoveChildElem()
{
    // Remove current child position element
    if (m_iPosChild)
    {
        Int32 iPosChild = x_RemoveElem(m_iPosChild);
        _SetPos(m_iPosParent, m_iPos, iPosChild);
        return True;
    }
    return False;
}


//////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////

Boolean MsXmlReader::x_AllocPosArray(Int32 nNewSize /*=0*/)
{
    // Resize m_aPos when the document is created or the array is filled
    // The PosArray class is implemented using segments to reduce contiguous memory requirements
    // It reduces reallocations (copying of memory) since this only occurs within one segment
    // The "Grow By" algorithm ensures there are no reallocations after 2 segments
    //
    if (!nNewSize)
        nNewSize = m_iPosFree + (m_iPosFree >> 1); // Grow By: multiply size by 1.5
    if (m_aPos.GetSize() < nNewSize)
    {
        // Grow By: NEW size can be at most one more complete segment
        Int32 nSeg = (m_aPos.GetSize() ? m_aPos.GetSize() - 1 : 0) >> m_aPos.PA_SEGBITS;
        Int32 nNewSeg = (nNewSize - 1) >> m_aPos.PA_SEGBITS;
        if (nNewSeg > nSeg + 1)
        {
            nNewSeg = nSeg + 1;
            nNewSize = (nNewSeg + 1) << m_aPos.PA_SEGBITS;
        }

        // Allocate array of segments
        if (m_aPos.nSegs <= nNewSeg)
        {
            Int32 nNewSegments = 4 + nNewSeg * 2;
            CHAR* pNewSegments = NEW CHAR[nNewSegments * sizeof(CHAR*)];
            if (m_aPos.SegsUsed())
                memcpy(pNewSegments, m_aPos.pSegs, m_aPos.SegsUsed() * sizeof(CHAR*));
            if (m_aPos.pSegs)
                delete[](CHAR*)m_aPos.pSegs;
            m_aPos.pSegs = (ElemPos**)pNewSegments;
            m_aPos.nSegs = nNewSegments;
        }

        // Calculate segment sizes
        Int32 nSegSize = m_aPos.GetSize() - (nSeg << m_aPos.PA_SEGBITS);
        Int32 nNewSegSize = nNewSize - (nNewSeg << m_aPos.PA_SEGBITS);

        // Complete first segment
        Int32 nFullSegSize = 1 << m_aPos.PA_SEGBITS;
        if (nSeg < nNewSeg && nSegSize < nFullSegSize)
        {
            CHAR* pNewFirstSeg = NEW CHAR[nFullSegSize * sizeof(ElemPos)];
            if (nSegSize)
            {
                // Reallocate
                memcpy(pNewFirstSeg, m_aPos.pSegs[nSeg], nSegSize * sizeof(ElemPos));
                delete[](CHAR*)m_aPos.pSegs[nSeg];
            }
            m_aPos.pSegs[nSeg] = (ElemPos*)pNewFirstSeg;
        }

        // New segment
        CHAR* pNewSeg = NEW CHAR[nNewSegSize * sizeof(ElemPos)];
        if (nNewSeg == nSeg && nSegSize)
        {
            // Reallocate
            memcpy(pNewSeg, m_aPos.pSegs[nSeg], nSegSize * sizeof(ElemPos));
            delete[](CHAR*)m_aPos.pSegs[nSeg];
        }
        m_aPos.pSegs[nNewSeg] = (ElemPos*)pNewSeg;
        m_aPos.nSize = nNewSize;
    }
    return True;
}

Boolean MsXmlReader::x_ParseDoc()
{
    // Preserve pre-parse result
    mstr strResult = m_strError;

    // Reset indexes
    ResetPos();
    m_mapSavedPos.RemoveAll();

    // Starting size of position array: 1 element per 64 bytes of document
    // Tight fit when parsing small doc, only 0 to 2 reallocs when parsing large doc
    // Start at 8 when creating NEW document
    m_iPosFree = 1;
    x_AllocPosArray((Int32)m_strDoc.size() / 64 + 8);
    m_iPosDeleted = 0;

    // Parse document
    m_aPos[0].ClearVirtualParent();
    if (m_strDoc.size())
    {
        TokenPos token(m_strDoc, m_nFlags);
        Int32 iPos = x_ParseElem(0, token);
        m_aPos[0].nLength = (Int32)m_strDoc.size();
        if (iPos > 0)
        {
            m_aPos[0].iElemChild = iPos;
            if (m_aPos[iPos].iElemNext)
                m_strError = "Root element has sibling";
        }
        else
            m_strError = "No root element";
    }
    else
        m_strError = "Empty document";

    ResetPos();

    // Combine preserved result with parse error
    if (!strResult.empty())
    {
        if (m_strError.empty())
            m_strError = strResult;
        else
            m_strError = strResult + ", " + m_strError;
    }

    return IsWellFormed();
};

Int32 MsXmlReader::x_ParseElem(Int32 iPosParent, TokenPos& token)
{
    // This is either called by x_ParseDoc or x_AddSubDoc or x_SetElemContent
    // Returns index of the first element encountered or zero if no elements
    //
    Int32 iElemRoot = 0;
    Int32 iPos = iPosParent;
    Int32 iVirtualParent = iPosParent;
    Int32 nRootDepth = m_aPos[iPos].Level();
    token.nNext = 0;
    m_strError.erase();

    // Loop through the nodes of the document
    NodeStack aNodes;
    aNodes.Add();
    Int32 nDepth = 0;
    Int32 nMatchDepth;
    Int32 iPosChild;
    Int32 iPosMatch;
    Int32 nTypeFound = 0;
    ElemPos* pElem;
    Int32 iElemFirst, iElemLast;
    while (1)
    {
        nTypeFound = x_ParseNode(token, aNodes.Top());
        nMatchDepth = 0;
        if (nTypeFound == MNT_ELEMENT) // start tag
        {
            iPos = x_GetFreePos();
            if (!iElemRoot)
                iElemRoot = iPos;
            pElem = &m_aPos[iPos];
            pElem->iElemParent = iPosParent;
            pElem->iElemNext = 0;
            if (m_aPos[iPosParent].iElemChild)
            {
                iElemFirst = m_aPos[iPosParent].iElemChild;
                iElemLast = m_aPos[iElemFirst].iElemPrev;
                m_aPos[iElemLast].iElemNext = iPos;
                pElem->iElemPrev = iElemLast;
                m_aPos[iElemFirst].iElemPrev = iPos;
                pElem->nFlags = 0;
            }
            else
            {
                m_aPos[iPosParent].iElemChild = iPos;
                pElem->iElemPrev = iPos;
                pElem->nFlags = MNF_FIRST;
            }
            pElem->SetLevel(nRootDepth + nDepth);
            pElem->iElemChild = 0;
            pElem->nStart = aNodes.Top().nStart;
            pElem->SetStartTagLen(aNodes.Top().nLength);
            if (aNodes.Top().nFlags & MNF_EMPTY)
            {
                iPos = iPosParent;
                pElem->SetEndTagLen(0);
                pElem->nLength = aNodes.Top().nLength;
            }
            else
            {
                iPosParent = iPos;
                ++nDepth;
                aNodes.Add();
            }
        }
        else if (nTypeFound == 0) // end tag
        {
            nMatchDepth = nDepth;
            iPosMatch = iPos;
            while (nMatchDepth && !token.Match(aNodes.At(nMatchDepth - 1).strMeta))
            {
                /*
                // Auto-switch case sensitivity
                if ( ! (token.nTokenFlags & MDF_IGNORECASE ) )
                {
                    token.nTokenFlags |= MDF_IGNORECASE;
                    if ( token.Match(aNodes.At(nMatchDepth-1).strMeta) )
                        break;
                    token.nTokenFlags |= MDF_IGNORECASE;
                }
                */
                --nMatchDepth;
                iPosMatch = m_aPos[iPosMatch].iElemParent;
            }
            if (nMatchDepth == 0)
            {
                // Not matched at all, it is a lone end tag, a non-element node
                m_aPos[iVirtualParent].nFlags |= MNF_ILLFORMED;
                m_aPos[iPos].nFlags |= MNF_ILLDATA;
                if (m_strError.empty())
                {
                    CHAR* szError = NEW CHAR[token.Length() + 100];
                    sprintf_s(szError, token.Length() + 100, "No start tag for end tag '%s' at offset %d",
                        x_GetToken(token).c_str(), aNodes.Top().nStart);
                    m_strError = szError;
                    delete[] szError;
                }
            }
            else
            {
                pElem = &m_aPos[iPosMatch];
                pElem->nLength = aNodes.Top().nStart - pElem->nStart + aNodes.Top().nLength;
                pElem->SetEndTagLen(aNodes.Top().nLength);
            }
        }
        else if (nTypeFound == -1)
        {
            m_aPos[iVirtualParent].nFlags |= MNF_ILLFORMED;
            m_aPos[iPos].nFlags |= MNF_ILLDATA;
            if (m_strError.empty())
                m_strError = aNodes.Top().strMeta;
        }

        // Matched end tag, or end of document
        if (nMatchDepth || nTypeFound == -2)
        {
            if (nDepth > nMatchDepth)
                m_aPos[iVirtualParent].nFlags |= MNF_ILLFORMED;

            // Process any non-ended elements
            while (nDepth > nMatchDepth)
            {
                // Element with no end tag
                pElem = &m_aPos[iPos];
                iPosChild = pElem->iElemChild;
                iPosParent = pElem->iElemParent;
                pElem->SetEndTagLen(0);
                pElem->nFlags |= MNF_NONENDED;
                pElem->iElemChild = 0;
                pElem->nLength = pElem->StartTagLen();
                if (pElem->nFlags & MNF_ILLDATA)
                {
                    pElem->nFlags ^= MNF_ILLDATA;
                    m_aPos[iPosParent].nFlags |= MNF_ILLDATA;
                }
                while (iPosChild)
                {
                    m_aPos[iPosChild].iElemParent = iPosParent;
                    m_aPos[iPosChild].iElemPrev = iPos;
                    m_aPos[iPos].iElemNext = iPosChild;
                    iPos = iPosChild;
                    iPosChild = m_aPos[iPosChild].iElemNext;
                }
                iPos = iPosParent;
                aNodes.Remove();
                --nDepth;

                // Error mstr
                // if end tag did not match, top node is end tag that did not match pElem
                // if end of document, any nodes below top have no end tag
                if (m_strError.empty())
                {
                    if (nTypeFound == 0)
                    {
                        CHAR* szError = NEW CHAR[aNodes.Top().strMeta.size() + token.Length() + 100];
                        sprintf_s(szError, aNodes.Top().strMeta.size() + token.Length() + 100, "End tag '%s' at offset %d does not match start tag '%s' at offset %d",
                            x_GetToken(token).c_str(), token.nL - 1, aNodes.Top().strMeta.c_str(), pElem->nStart);
                        m_strError = szError;
                        delete[] szError;
                    }
                    else
                    {
                        CHAR* szError = NEW CHAR[aNodes.Top().strMeta.size() + 100];
                        sprintf_s(szError, aNodes.Top().strMeta.size() + 100, "Element '%s' at offset %d not ended",
                            aNodes.Top().strMeta.c_str(), aNodes.Top().nStart);
                        m_strError = szError;
                        delete[] szError;
                    }
                }
            }
            if (nTypeFound == -2)
                break;
            iPosParent = m_aPos[iPos].iElemParent;
            iPos = iPosParent;
            aNodes.Remove();
            --nDepth;
        }
    }
    return iElemRoot;
}

Boolean MsXmlReader::x_FindAny(LPCSTR szDoc, Int32& nChar)
{
    // Starting at nChar, find a non-whitespace CHAR
    // return False if no non-whitespace before end of document, nChar points to end
    // otherwise return True and nChar points to non-whitespace CHAR
    static AscIISet as(" \t\n\r");
    while (szDoc[nChar] && as.Has(szDoc[nChar]))
        ++nChar;
    return szDoc[nChar] != '\0';
}

Boolean MsXmlReader::x_FindName(MsXmlReader::TokenPos& token)
{
    // Starting at token.nNext, bypass whitespace and find the next name
    // returns True on success, members of token point to token
    // returns False on end of document, members point to end of document
    LPCSTR szDoc = token.szDoc;
    Int32 nChar = token.nNext;

    // By-pass leading whitespace
    if (!x_FindAny(szDoc, nChar))
    {
        // No token was found before end of document
        token.nL = nChar;
        token.nR = nChar - 1;
        token.nNext = nChar;
        return False;
    }

    // Go until special CHAR or whitespace
    token.nL = nChar;
    static AscIISet as(" \t\n\r<>=\\/?!");
    while (szDoc[nChar] && !as.Has(szDoc[nChar]))
        ++nChar;

    // Adjust end position if it is one special CHAR
    if (nChar == token.nL)
        ++nChar; // it is a special CHAR
    token.nR = nChar - 1;

    // nNext points to one past last CHAR of token
    token.nNext = nChar;
    return True;
}

mstr MsXmlReader::x_GetToken(const MsXmlReader::TokenPos& token)
{
    // The token contains indexes into the document identifying a small substring
    // Build the substring from those indexes and return it
    if (token.nL > token.nR)
        return "";
    mstr strToken(&token.szDoc[token.nL], token.Length());
    return strToken;
}

Int32 MsXmlReader::x_FindElem(Int32 iPosParent, Int32 iPos, LPCSTR szPath) const
{
    // If szPath is nullptr or empty, go to next sibling element
    // Otherwise go to next sibling element with matching path
    //
    if (iPos)
        iPos = m_aPos[iPos].iElemNext;
    else
        iPos = m_aPos[iPosParent].iElemChild;

    // Finished here if szPath not specified
    if (szPath == nullptr || !szPath[0])
        return iPos;

    // Search
    TokenPos token(m_strDoc, m_nFlags);
    while (iPos)
    {
        // Compare tag name
        token.nNext = m_aPos[iPos].nStart + 1;
        x_FindName(token); // Locate tag name
        if (token.Match(szPath))
            return iPos;
        iPos = m_aPos[iPos].iElemNext;
    }
    return 0;

}

MarkupNodeType MsXmlReader::x_ParseNode(MsXmlReader::TokenPos& token, MsXmlReader::NodePos& node)
{
    // Call this with token.nNext set to the start of the node or tag
    // Upon return token.nNext points to the CHAR after the node or tag
    // 
    // <!--...--> comment
    // <!DOCTYPE ...> dtd
    // <?target ...?> processing instruction
    // <![CDATA[...]]> cdata section
    // <NAME ...> element start tag
    // </NAME ...> element end tag
    //
    // returns the nodetype or
    // 0 for end tag
    // -1 for bad node
    // -2 for end of document
    //
    enum ParseBits
    {
        PD_OPENTAG = 1,
        PD_BANG = 2,
        PD_DASH = 4,
        PD_BRACKET = 8,
        PD_TEXTORWS = 16,
        PD_DOCTYPE = 32,
        PD_INQUOTE_S = 64,
        PD_INQUOTE_D = 128,
    };
    Int32 nParseFlags = 0;

    LPCSTR szFindEnd = nullptr;
    MarkupNodeType nNodeType = MNT_DEFAULT_INVALID;
    Int32 nEndLen = 0;
    Int32 nName = 0;
    UInt32 cDminus1 = 0, cDminus2 = 0;
#define FINDNODETYPE(e,t,n) { szFindEnd=e; nEndLen=(sizeof(e)-1); nNodeType=t; if(n) nName=(Int32)(pDoc-token.szDoc)+n-1; }
#define FINDNODEBAD(e) { szFindEnd=">"; nEndLen=1; CHAR szE[100]; sprintf_s(szE,"Incorrect %s at offset %d",e,nR); node.strMeta=szE; nNodeType=MNT_DEFAULT_INVALID; }

    node.nStart = token.nNext;
    node.nFlags = 0;

    Int32 nR = token.nNext;
    LPCSTR pDoc = &token.szDoc[nR];
    register UInt32 cD = (UInt32)*pDoc;
    if (!cD)
    {
        node.nLength = 0;
        node.nNodeType = 0;
        return MNT_END_INVALID; // end of document
    }

    while (1)
    {
        cD = (UInt32)*pDoc;
        if (!cD)
        {
            nR = (Int32)(pDoc - token.szDoc) - 1;
            if (nNodeType != MNT_WHITESPACE && nNodeType != MNT_TEXT)
            {
                LPCSTR szType = "tag";
                if ((nParseFlags & PD_DOCTYPE) || nNodeType == MNT_DOCUMENT_TYPE)
                    szType = "Doctype";
                else if (nNodeType == MNT_ELEMENT)
                    szType = "Element tag";
                else if (nNodeType == 0)
                    szType = "Element end tag";
                else if (nNodeType == MNT_CDATA_SECTION)
                    szType = "CDATA Section";
                else if (nNodeType == MNT_PROCESSING_INSTRUCTION)
                    szType = "Processing instruction";
                else if (nNodeType == MNT_COMMENT)
                    szType = "Comment";
                nNodeType = MNT_DEFAULT_INVALID;
                CHAR szError[100];
                sprintf_s(szError, "%s at offset %d unterminated", szType, node.nStart);
                node.strMeta = szError;
            }
            break;
        }

        if (nName)
        {
            static AscIISet as(" \t\n\r/>");
            if (as.Has((CHAR)cD))
            {
                Int32 nNameLen = (Int32)(pDoc - token.szDoc) - nName;
                if (nNodeType == 0)
                {
                    token.nL = nName;
                    token.nR = nName + nNameLen - 1;
                }
                else
                {
                    node.strMeta.assign(&token.szDoc[nName], nNameLen);
                }
                nName = 0;
                cDminus2 = 0;
                cDminus1 = 0;
            }
            else
            {
                ++pDoc;
                continue;
            }
        }

        if (szFindEnd)
        {
            if (cD == '>' && !(nParseFlags & (PD_INQUOTE_S | PD_INQUOTE_D)))
            {
                nR = (Int32)(pDoc - token.szDoc);
                if (nEndLen == 1)
                {
                    szFindEnd = nullptr;
                    if (nNodeType == MNT_ELEMENT && cDminus1 == '/')
                    {
                        static AscIISet as(" \t\n\r\'\"");
                        if ((!cDminus2) || as.Has((CHAR)cDminus2))
                            node.nFlags |= MNF_EMPTY;
                    }
                }
                else if (nR > nEndLen)
                {
                    // Test for end of PI or comment
                    LPCSTR pEnd = pDoc - nEndLen + 1;
                    LPCSTR pFindEnd = szFindEnd;
                    Int32 nLen = nEndLen;
                    while (--nLen && *pEnd++ == *pFindEnd++);
                    if (nLen == 0)
                        szFindEnd = nullptr;
                }
                if (!szFindEnd && !(nParseFlags & PD_DOCTYPE))
                    break;
            }
            else if (cD == '<' && (nNodeType == MNT_TEXT || nNodeType == -1))
            {
                nR = (Int32)(pDoc - token.szDoc) - 1;
                break;
            }
            else if (nNodeType & (MNT_ELEMENT | MNT_DOCUMENT_TYPE))
            {
                if (cD == '\"' && !(nParseFlags&PD_INQUOTE_S))
                    nParseFlags ^= PD_INQUOTE_D;
                else if (cD == '\'' && !(nParseFlags&PD_INQUOTE_D))
                    nParseFlags ^= PD_INQUOTE_S;
                if (nNodeType == MNT_ELEMENT)
                {
                    cDminus2 = cDminus1;
                    cDminus1 = cD;
                }
            }
        }
        else if (nParseFlags)
        {
            if (nParseFlags & PD_TEXTORWS)
            {
                static AscIISet as(" \t\n\r");
                if (cD == '<')
                {
                    nR = (Int32)(pDoc - token.szDoc) - 1;
                    nNodeType = MNT_WHITESPACE;
                    break;
                }
                else if (!as.Has((CHAR)cD))
                {
                    nParseFlags ^= PD_TEXTORWS;
                    FINDNODETYPE("<", MNT_TEXT, 0)
                }
            }
            else if (nParseFlags & PD_OPENTAG)
            {
                nParseFlags ^= PD_OPENTAG;
                if (cD > 0x60 || (cD > 0x40 && cD < 0x5b) || cD == 0x5f || cD == 0x3a)
                    FINDNODETYPE(">", MNT_ELEMENT, 1)
                else if (cD == '/')
                    FINDNODETYPE(">", MNT_INVALID, 2)
                else if (cD == '!')
                    nParseFlags |= PD_BANG;
                else if (cD == '?')
                    FINDNODETYPE("?>", MNT_PROCESSING_INSTRUCTION, 2)
                else
                    FINDNODEBAD("tag name character")
            }
            else if (nParseFlags & PD_BANG)
            {
                nParseFlags ^= PD_BANG;
                if (cD == '-')
                    nParseFlags |= PD_DASH;
                else if (cD == '[' && !(nParseFlags & PD_DOCTYPE))
                    nParseFlags |= PD_BRACKET;
                else if (cD == 'D' && !(nParseFlags & PD_DOCTYPE))
                    nParseFlags |= PD_DOCTYPE;
                else if (strchr("EAN", (CHAR)cD)) // <!ELEMENT ATTLIST ENTITY NOTATION
                    FINDNODETYPE(">", MNT_DOCUMENT_TYPE, 0)
                else
                    FINDNODEBAD("! tag")
            }
            else if (nParseFlags & PD_DASH)
            {
                nParseFlags ^= PD_DASH;
                if (cD == '-')
                    FINDNODETYPE("-->", MNT_COMMENT, 0)
                else
                    FINDNODEBAD("comment tag")
            }
            else if (nParseFlags & PD_BRACKET)
            {
                nParseFlags ^= PD_BRACKET;
                if (cD == 'C')
                    FINDNODETYPE("]]>", MNT_CDATA_SECTION, 0)
                else
                    FINDNODEBAD("tag")
            }
            else if (nParseFlags & PD_DOCTYPE)
            {
                if (cD == '<')
                    nParseFlags |= PD_OPENTAG;
                else if (cD == '>')
                {
                    nR = (Int32)(pDoc - token.szDoc);
                    nNodeType = MNT_DOCUMENT_TYPE;
                    break;
                }
            }
        }
        else if (cD == '<')
        {
            nParseFlags |= PD_OPENTAG;
        }
        else
        {
            nNodeType = MNT_WHITESPACE;
            if (strchr(" \t\n\r", (CHAR)cD))
                nParseFlags |= PD_TEXTORWS;
            else
                FINDNODETYPE("<", MNT_TEXT, 0)
        }
        ++pDoc;
    }
    token.nNext = nR + 1;
    node.nLength = token.nNext - node.nStart;
    node.nNodeType = nNodeType;
    return nNodeType;
}

mstr MsXmlReader::x_GetPath(Int32 iPos) const
{
    mstr strPath;
    while (iPos)
    {
        mstr strTagName = x_GetTagName(iPos);
        Int32 iPosParent = m_aPos[iPos].iElemParent;
        Int32 iPosSib = 0;
        Int32 nCount = 0;
        while (iPosSib != iPos)
        {
            iPosSib = x_FindElem(iPosParent, iPosSib, strTagName.c_str());
            ++nCount;
        }
        if (nCount > 1)
        {
            CHAR szPred[25];
            sprintf_s(szPred, "[%d]", nCount);
            strPath = "/" + strTagName + szPred + strPath;
        }
        else
            strPath = "/" + strTagName + strPath;
        iPos = iPosParent;
    }
    return strPath;
}

mstr MsXmlReader::x_GetTagName(Int32 iPos) const
{
    // Return the tag name at specified element
    TokenPos token(m_strDoc, m_nFlags);
    token.nNext = m_aPos[iPos].nStart + 1;
    if (!iPos || !x_FindName(token))
        return "";

    // Return substring of document
    return x_GetToken(token);
}

Boolean MsXmlReader::x_FindAttrib(MsXmlReader::TokenPos& token, LPCSTR szAttrib, Int32 n/*=0*/)
{
    // Return True if found, otherwise False and token.nNext is NEW insertion point
    // If szAttrib is nullptr find attrib n and leave token at attrib name
    // If szAttrib is given, find matching attrib and leave token at value
    // support non-well-formed attributes e.g. href=/advanced_search?hl=en, nowrap
    // token also holds start and length of preceeding whitespace to support remove
    //
    Int32 nPreSpaceStart;
    Int32 nPreSpaceLength;
    Int32 nChar;
    CHAR cFirstChar;
    LPCSTR szDoc = token.szDoc;
    Int32 nAttrib = -1; // starts at tag name
    Int32 nFoundAttribNameR = 0;
    Boolean bAfterEqual = False;
    while (1)
    {
        // Starting at token.nNext, bypass whitespace and find the next token
        nChar = token.nNext;
        nPreSpaceStart = nChar;
        if (!x_FindAny(szDoc, nChar))
            break;
        nPreSpaceLength = nChar - nPreSpaceStart;

        // Is it an opening quote?
        cFirstChar = szDoc[nChar];
        if (cFirstChar == '\"' || cFirstChar == '\'')
        {
            token.nTokenFlags |= MNF_QUOTED;

            // Move past opening quote
            ++nChar;
            token.nL = nChar;

            // Look for closing quote
            while (szDoc[nChar] && szDoc[nChar] != cFirstChar)
                ++nChar;

            // Set right to before closing quote
            token.nR = nChar - 1;

            // Set nChar past closing quote unless at end of document
            if (szDoc[nChar])
                ++nChar;
        }
        else
        {
            token.nTokenFlags &= ~MNF_QUOTED;

            // Go until special CHAR or whitespace
            token.nL = nChar;
            if (bAfterEqual)
            {
                static AscIISet as(" \t\n\r>");
                while (szDoc[nChar] && !as.Has(szDoc[nChar]))
                    ++nChar;
            }
            else
            {
                static AscIISet as("= \t\n\r>/?");
                while (szDoc[nChar] && !as.Has(szDoc[nChar]))
                    ++nChar;
            }

            // Adjust end position if it is one special CHAR
            if (nChar == token.nL)
                ++nChar; // it is a special CHAR
            token.nR = nChar - 1;
        }

        // nNext points to one past last CHAR of token
        token.nNext = nChar;

        if (!bAfterEqual && !(token.nTokenFlags&MNF_QUOTED))
        {
            // Is it an equal sign?
            CHAR cChar = szDoc[token.nL];
            if (cChar == '=')
            {
                bAfterEqual = True;
                continue;
            }

            // Is it the right angle bracket?
            if (cChar == '>' || cChar == '/' || cChar == '?')
            {
                token.nNext = nPreSpaceStart;
                break; // attrib not found
            }

            if (nFoundAttribNameR)
                break;

            // Attribute name
            if (nAttrib != -1)
            {
                if (!szAttrib)
                {
                    if (nAttrib == n)
                        return True; // found by number
                }
                else if (token.Match(szAttrib))
                {
                    // Matched attrib name, go forward to value
                    nFoundAttribNameR = token.nR;
                    token.nPreSpaceStart = nPreSpaceStart;
                    token.nPreSpaceLength = nPreSpaceLength;
                }
            }
            ++nAttrib;
        }
        else if (nFoundAttribNameR)
            break;
        bAfterEqual = False;
    }

    if (nFoundAttribNameR)
    {
        if (!bAfterEqual)
        {
            // when attribute has no value the value is the attribute name
            token.nL = token.nPreSpaceStart + token.nPreSpaceLength;
            token.nR = nFoundAttribNameR;
            token.nNext = nFoundAttribNameR + 1;
        }
        return True; // found by name
    }
    return False; // not found
}

mstr MsXmlReader::x_GetAttrib(Int32 iPos, LPCSTR szAttrib) const
{
    // Return the value of the attrib
    TokenPos token(m_strDoc, m_nFlags);
    if (iPos && m_nNodeType == MNT_ELEMENT)
        token.nNext = m_aPos[iPos].nStart + 1;
    else if (iPos == m_iPos && m_nNodeLength && m_nNodeType == MNT_PROCESSING_INSTRUCTION)
        token.nNext = m_nNodeOffset + 2;
    else
        return "";

    if (szAttrib && x_FindAttrib(token, szAttrib))
        return UnescapeText(&token.szDoc[token.nL], token.Length());
    return "";
}

Boolean MsXmlReader::x_SetAttrib(Int32 iPos, LPCSTR szAttrib, Int32 nValue)
{
    // Convert integer to mstr and call SetChildAttrib
    CHAR szVal[25];
    sprintf_s(szVal, "%d", nValue);
    return x_SetAttrib(iPos, szAttrib, szVal);
}

Boolean MsXmlReader::x_SetAttrib(Int32 iPos, LPCSTR szAttrib, LPCSTR szValue)
{
    // Set attribute in iPos element
    TokenPos token(m_strDoc, m_nFlags);
    if (iPos && m_nNodeType == MNT_ELEMENT)
        token.nNext = m_aPos[iPos].nStart + 1;
    else if (iPos == m_iPos && m_nNodeLength && m_nNodeType == MNT_PROCESSING_INSTRUCTION)
        token.nNext = m_nNodeOffset + 2;
    else
        return False;

    // Create insertion text depending on whether attribute already exists
    // Decision: for empty value leaving attrib="" instead of removing attrib
    Int32 nReplace = 0;
    Int32 nInsertAt;
    mstr strInsert;
    strInsert += X_ATTRIBQUOTE;
    strInsert += EscapeText(szValue, MNF_ESCAPEQUOTES);
    strInsert += X_ATTRIBQUOTE;
    if (x_FindAttrib(token, szAttrib))
    {
        // Replace value
        nInsertAt = token.nL - ((token.nTokenFlags&MNF_QUOTED) ? 1 : 0);
        nReplace = token.Length() + ((token.nTokenFlags&MNF_QUOTED) ? 2 : 0);
    }
    else
    {
        // Insert mstr name value pair
        mstr strFormat;
        strFormat = " ";
        strFormat += szAttrib;
        strFormat += "=";
        strFormat += strInsert;
        strInsert = strFormat;
        nInsertAt = token.nNext;
    }

    x_DocChange(nInsertAt, nReplace, strInsert);
    Int32 nAdjust = (Int32)strInsert.size() - nReplace;
    if (m_nNodeType == MNT_PROCESSING_INSTRUCTION)
    {
        x_AdjustForNode(m_iPosParent, m_iPos, nAdjust);
        m_nNodeLength += nAdjust;
        ////MARKUP_SETDEBUGSTATE;
        return True;
    }
    m_aPos[iPos].AdjustStartTagLen(nAdjust);
    m_aPos[iPos].nLength += nAdjust;
    x_Adjust(iPos, nAdjust);
    //MARKUP_SETDEBUGSTATE;
    return True;
}


Boolean MsXmlReader::x_CreateNode(mstr& strNode, MarkupNodeType nNodeType, LPCSTR szText)
{
    // Set strNode based on nNodeType and szData
    // Return False if szData would jeopardize well-formed document
    //
    switch (nNodeType)
    {
        case MNT_PROCESSING_INSTRUCTION:
            strNode = "<?";
            strNode += szText;
            strNode += "?>";
            break;
        case MNT_COMMENT:
            strNode = "<!--";
            strNode += szText;
            strNode += "-->";
            break;
        case MNT_ELEMENT:
            strNode = "<";
            strNode += szText;
            strNode += "/>";
            break;
        case MNT_TEXT:
        case MNT_WHITESPACE:
            strNode = EscapeText(szText);
            break;
        case MNT_DOCUMENT_TYPE:
            strNode = szText;
            break;
        case MNT_LONE_END_TAG:
            return False;
        case MNT_CDATA_SECTION:
            if (strstr(szText, "]]>") != nullptr)
                return False;
            strNode = "<![CDATA[";
            strNode += szText;
            strNode += "]]>";
            break;
    }
    return True;
}

mstr MsXmlReader::x_EncodeCDATASection(LPCSTR szData)
{
    // Split CDATA Sections if there are any end delimiters
    mstr strData = "<![CDATA[";
    LPCSTR pszNextStart = szData;
    LPCSTR pszEnd = strstr(szData, "]]>");
    while (pszEnd)
    {
        strData += mstr(pszNextStart, (Int32)(pszEnd - pszNextStart));
        strData += "]]]]><![CDATA[>";
        pszNextStart = pszEnd + 3;
        pszEnd = strstr(pszNextStart, "]]>");
    }
    strData += pszNextStart;
    strData += "]]>";
    return strData;
}

Boolean MsXmlReader::x_SetData(Int32 iPos, Int32 nValue)
{
    // Convert integer to mstr
    CHAR szVal[25];
    sprintf_s(szVal, "%d", nValue);
    return x_SetData(iPos, szVal, MNF_DEFAULT);
}

Boolean MsXmlReader::x_SetData(Int32 iPos, LPCSTR szData, MarkupNodeFlagsInternal nFlags)
{
    // Set data at specified position
    // if nFlags==1, set content of element to a CDATA Section
    mstr strInsert;

    if (iPos == m_iPos && m_nNodeLength)
    {
        // Not an element
        if (!x_CreateNode(strInsert, m_nNodeType, szData))
            return False;
        x_DocChange(m_nNodeOffset, m_nNodeLength, strInsert);
        x_AdjustForNode(m_iPosParent, iPos, (Int32)strInsert.size() - m_nNodeLength);
        m_nNodeLength = (Int32)strInsert.size();
        //MARKUP_SETDEBUGSTATE;
        return True;
    }

    // Set data in iPos element
    if (!iPos || m_aPos[iPos].iElemChild)
        return False;

    // Build strInsert from szData based on nFlags
    if (nFlags & MNF_WITHCDATA)
        strInsert = x_EncodeCDATASection(szData);
    else
        strInsert = EscapeText(szData, nFlags);

    // Insert
    NodePos node(MNF_WITHNOLINES | MNF_REPLACE);
    node.strMeta = strInsert;
    Int32 iPosBefore = 0;
    Int32 nReplace = x_InsertNew(iPos, iPosBefore, node);
    Int32 nAdjust = (Int32)node.strMeta.size() - nReplace;
    x_Adjust(iPos, nAdjust);
    m_aPos[iPos].nLength += nAdjust;
    if (m_aPos[iPos].nFlags & MNF_ILLDATA)
        m_aPos[iPos].nFlags &= ~MNF_ILLDATA;
    //MARKUP_SETDEBUGSTATE;
    return True;
}

mstr MsXmlReader::x_GetData(Int32 iPos) const
{
    if (iPos == m_iPos && m_nNodeLength)
    {
        if (m_nNodeType == MNT_COMMENT)
            return m_strDoc.substr(m_nNodeOffset + 4, m_nNodeLength - 7);
        else if (m_nNodeType == MNT_PROCESSING_INSTRUCTION)
            return m_strDoc.substr(m_nNodeOffset + 2, m_nNodeLength - 4);
        else if (m_nNodeType == MNT_CDATA_SECTION)
            return m_strDoc.substr(m_nNodeOffset + 9, m_nNodeLength - 12);
        else if (m_nNodeType == MNT_TEXT)
            return UnescapeText(&(m_strDoc.c_str())[m_nNodeOffset], m_nNodeLength);
        else if (m_nNodeType == MNT_LONE_END_TAG)
            return m_strDoc.substr(m_nNodeOffset + 2, m_nNodeLength - 3);
        else
            return m_strDoc.substr(m_nNodeOffset, m_nNodeLength);
    }

    // Return a mstr representing data between start and end tag
    // Return empty mstr if there are any children elements
    mstr strData;
    if (!m_aPos[iPos].iElemChild && !m_aPos[iPos].IsEmptyElement())
    {
        // Quick scan for any tags inside content
        Int32 nContentLen = m_aPos[iPos].ContentLen();
        Int32 nStartContent = m_aPos[iPos].StartContent();
        LPCSTR pszContent = &(m_strDoc.c_str())[nStartContent];
        LPCSTR pszTag = strchr(pszContent, '<');
        if (pszTag && ((Int32)(pszTag - pszContent) < nContentLen))
        {
            // Concatenate all CDATA Sections and text nodes, ignore other nodes
            TokenPos token(m_strDoc, m_nFlags);
            token.nNext = nStartContent;
            NodePos node;
            while (token.nNext < nStartContent + nContentLen)
            {
                x_ParseNode(token, node);
                if (node.nNodeType == MNT_TEXT)
                    strData += UnescapeText(&token.szDoc[node.nStart], node.nLength);
                else if (node.nNodeType == MNT_CDATA_SECTION)
                    strData += m_strDoc.substr(node.nStart + 9, node.nLength - 12);
            }
        }
        else // no tags
            strData = UnescapeText(&(m_strDoc.c_str())[nStartContent], nContentLen);
    }
    return strData;
}

mstr MsXmlReader::x_GetElemContent(Int32 iPos) const
{
    if (iPos && m_aPos[iPos].ContentLen())
        return m_strDoc.substr(m_aPos[iPos].StartContent(), m_aPos[iPos].ContentLen());
    return "";
}

Boolean MsXmlReader::x_SetElemContent(LPCSTR szContent)
{
    // Set data in iPos element only
    if (!m_iPos)
        return False;

    if (m_nNodeLength)
        return False; // not an element

    // Unlink all children
    Int32 iPos = m_iPos;
    Int32 iPosChild = m_aPos[iPos].iElemChild;
    Boolean bHadChild = (iPosChild != 0);
    while (iPosChild)
        iPosChild = x_ReleaseSubDoc(iPosChild);
    if (bHadChild)
        x_CheckSavedPos();

    // Parse content
    Boolean bWellFormed = True;
    TokenPos token(szContent, m_nFlags);
    Int32 iPosVirtual = x_GetFreePos();
    m_aPos[iPosVirtual].ClearVirtualParent();
    m_aPos[iPosVirtual].SetLevel(m_aPos[iPos].Level() + 1);
    iPosChild = x_ParseElem(iPosVirtual, token);
    if (m_aPos[iPosVirtual].nFlags & MNF_ILLFORMED)
        bWellFormed = False;
    m_aPos[iPos].nFlags = (m_aPos[iPos].nFlags & ~MNF_ILLDATA) | (m_aPos[iPosVirtual].nFlags & MNF_ILLDATA);

    // Prepare insert and adjust offsets
    NodePos node(MNF_WITHNOLINES | MNF_REPLACE);
    node.strMeta = szContent;
    Int32 iPosBefore = 0;
    Int32 nReplace = x_InsertNew(iPos, iPosBefore, node);

    // Adjust and link in the inserted elements
    x_Adjust(iPosChild, node.nStart);
    m_aPos[iPosChild].nStart += node.nStart;
    m_aPos[iPos].iElemChild = iPosChild;
    while (iPosChild)
    {
        m_aPos[iPosChild].iElemParent = iPos;
        iPosChild = m_aPos[iPosChild].iElemNext;
    }
    x_ReleasePos(iPosVirtual);

    Int32 nAdjust = (Int32)node.strMeta.size() - nReplace;
    x_Adjust(iPos, nAdjust, True);
    m_aPos[iPos].nLength += nAdjust;

    _SetPos(m_iPosParent, m_iPos, 0);
    return bWellFormed;
}

void MsXmlReader::x_DocChange(Int32 nLeft, Int32 nReplace, const mstr& strInsert)
{
    // Insert strInsert Int32 m_strDoc at nLeft replacing nReplace chars
    // Do this with only one buffer reallocation if it grows
    //
    Int32 nDocLength = (Int32)m_strDoc.size();
    Int32 nInsLength = (Int32)strInsert.size();
    Int32 nNewLength = nInsLength + nDocLength - nReplace;

    // When creating a document, reduce reallocs by reserving mstr space
    // Allow for 1.5 times the current allocation
    Int32 nBufferLen = nNewLength;
    Int32 nAllocLen = (Int32)m_strDoc.capacity();
    if (nNewLength > nAllocLen)
    {
        nBufferLen += nBufferLen / 2 + 128;
        if (nBufferLen < nNewLength)
            nBufferLen = nNewLength;
        m_strDoc.reserve(nBufferLen);
    }

    m_strDoc.replace(nLeft, nReplace, strInsert);

}

void MsXmlReader::x_Adjust(Int32 iPos, Int32 nShift, Boolean bAfterPos /*=False*/)
{
    // Loop through affected elements and adjust indexes
    // Algorithm:
    // 1. update children unless bAfterPos
    //   (if no children or bAfterPos is True, length of iPos not affected)
    // 2. update starts of next siblings and their children
    // 3. go up until there is a next sibling of a parent and update starts
    // 4. step 2
    Int32 iPosTop = m_aPos[iPos].iElemParent;
    Boolean bPosFirst = bAfterPos; // mark as first to skip its children

    // Stop when we've reached the virtual parent (which has no tags)
    while (m_aPos[iPos].StartTagLen())
    {
        // Were we at containing parent of affected position?
        Boolean bPosTop = False;
        if (iPos == iPosTop)
        {
            // Move iPosTop up one towards root
            iPosTop = m_aPos[iPos].iElemParent;
            bPosTop = True;
        }

        // Traverse to the next update position
        if (!bPosTop && !bPosFirst && m_aPos[iPos].iElemChild)
        {
            // Depth first
            iPos = m_aPos[iPos].iElemChild;
        }
        else if (m_aPos[iPos].iElemNext)
        {
            iPos = m_aPos[iPos].iElemNext;
        }
        else
        {
            // Look for next sibling of a parent of iPos
            // When going back up, parents have already been done except iPosTop
            while (1)
            {
                iPos = m_aPos[iPos].iElemParent;
                if (iPos == iPosTop)
                    break;
                if (m_aPos[iPos].iElemNext)
                {
                    iPos = m_aPos[iPos].iElemNext;
                    break;
                }
            }
        }
        bPosFirst = False;

        // Shift indexes at iPos
        if (iPos != iPosTop)
            m_aPos[iPos].nStart += nShift;
        else
            m_aPos[iPos].nLength += nShift;
    }
}

Int32 MsXmlReader::x_InsertNew(Int32 iPosParent, Int32& iPosRel, MsXmlReader::NodePos& node)
{
    // Parent empty tag or tags with no content?
    Boolean bEmptyParentTag = iPosParent && m_aPos[iPosParent].IsEmptyElement();
    Boolean bNoContentParentTags = iPosParent && !m_aPos[iPosParent].ContentLen();
    if (node.nLength)
    {
        // Located at a non-element node
        if (!(node.nFlags & MNF_INSERT))
            node.nStart += node.nLength;
    }
    else if (iPosRel)
    {
        // Located at an element
        node.nStart = m_aPos[iPosRel].nStart;
        if (!(node.nFlags & MNF_INSERT)) // follow iPosRel
            node.nStart += m_aPos[iPosRel].nLength;
    }
    else if (bEmptyParentTag)
    {
        // Parent has no separate end tag, so split empty element
        if (m_aPos[iPosParent].nFlags & MNF_NONENDED)
            node.nStart = m_aPos[iPosParent].StartContent();
        else
            node.nStart = m_aPos[iPosParent].StartContent() - 1;
    }
    else
    {
        if (node.nFlags & (MNF_INSERT | MNF_REPLACE))
            node.nStart = m_aPos[iPosParent].StartContent();
        else // before end tag
            node.nStart = m_aPos[iPosParent].StartAfter() - m_aPos[iPosParent].EndTagLen();
    }

    // Go up to start of next node, unless its splitting an empty element
    if (!(node.nFlags&(MNF_WITHNOLINES | MNF_REPLACE)) && !bEmptyParentTag)
    {
        LPCSTR szDoc = m_strDoc.c_str();
        Int32 nChar = node.nStart;
        if (!x_FindAny(szDoc, nChar) || szDoc[nChar] == '<')
            node.nStart = nChar;
    }

    // Is insert relative to element position? (i.e. not other kind of node)
    if (!node.nLength)
    {
        // Modify iPosRel to reflect position before
        if (iPosRel)
        {
            if (node.nFlags & MNF_INSERT)
            {
                if (!(m_aPos[iPosRel].nFlags & MNF_FIRST))
                    iPosRel = m_aPos[iPosRel].iElemPrev;
                else
                    iPosRel = 0;
            }
        }
        else if (!(node.nFlags & MNF_INSERT))
        {
            // If parent has a child, add after last child
            if (m_aPos[iPosParent].iElemChild)
                iPosRel = m_aPos[m_aPos[iPosParent].iElemChild].iElemPrev;
        }
    }

    // Get node length (used only by x_AddNode)
    node.nLength = (Int32)node.strMeta.size();

    // Prepare end of lines
    if ((!(node.nFlags & MNF_WITHNOLINES)) && (bEmptyParentTag || bNoContentParentTags))
        node.nStart += X_EOLLEN;
    if (!(node.nFlags & MNF_WITHNOLINES))
        node.strMeta += X_EOL;

    // Calculate insert offset and replace length
    Int32 nReplace = 0;
    Int32 nInsertAt = node.nStart;
    if (bEmptyParentTag)
    {
        mstr strTagName = x_GetTagName(iPosParent);
        mstr strFormat;
        if (node.nFlags & MNF_WITHNOLINES)
            strFormat = ">";
        else
            strFormat = ">" X_EOL;
        strFormat += node.strMeta;
        strFormat += "</";
        strFormat += strTagName;
        node.strMeta = strFormat;
        if (m_aPos[iPosParent].nFlags & MNF_NONENDED)
        {
            nInsertAt = m_aPos[iPosParent].StartAfter() - 1;
            nReplace = 0;
            m_aPos[iPosParent].nFlags ^= MNF_NONENDED;
        }
        else
        {
            nInsertAt = m_aPos[iPosParent].StartAfter() - 2;
            nReplace = 1;
            m_aPos[iPosParent].AdjustStartTagLen(-1);
        }
        m_aPos[iPosParent].SetEndTagLen(3 + (Int32)strTagName.size());
    }
    else
    {
        if (node.nFlags & MNF_REPLACE)
        {
            nInsertAt = m_aPos[iPosParent].StartContent();
            nReplace = m_aPos[iPosParent].ContentLen();
        }
        else if (bNoContentParentTags)
        {
            node.strMeta = X_EOL + node.strMeta;
            nInsertAt = m_aPos[iPosParent].StartContent();
        }
    }
    x_DocChange(nInsertAt, nReplace, node.strMeta);
    return nReplace;
}

Boolean MsXmlReader::x_AddElem(LPCSTR szName, Int32 nValue, MarkupNodeFlagsInternal nFlags)
{
    // Convert integer to mstr
    CHAR szVal[25];
    sprintf_s(szVal, "%d", nValue);
    return x_AddElem(szName, szVal, nFlags);
}

Boolean MsXmlReader::x_AddElem(LPCSTR szName, LPCSTR szValue, MarkupNodeFlagsInternal nFlags)
{
    if (nFlags & MNF_CHILD)
    {
        // Adding a child element under main position
        if (!m_iPos)
            return False;
    }

    // Locate where to add element relative to current node
    NodePos node(nFlags);
    Int32 iPosParent, iPosBefore;
    if (nFlags & MNF_CHILD)
    {
        iPosParent = m_iPos;
        iPosBefore = m_iPosChild;
    }
    else
    {
        iPosParent = m_iPosParent;
        iPosBefore = m_iPos;
        node.nStart = m_nNodeOffset;
        node.nLength = m_nNodeLength;
    }

    // Cannot have data in non-ended element
    if ((nFlags&MNF_WITHNOEND) && szValue && szValue[0])
        return False;

    // Allocate ElemPos structure for this element
    Int32 iPos = x_GetFreePos();

    // Create mstr for insert
    // If no szValue is specified, an empty element is created
    // i.e. either <NAME>value</NAME> or <NAME/>
    //
    ElemPos* pElem = &m_aPos[iPos];
    Int32 nLenName = (Int32)strlen(szName);
    if (!szValue || !szValue[0])
    {
        // <NAME/> empty element
        node.strMeta = "<";
        node.strMeta += szName;
        if (nFlags & MNF_WITHNOEND)
        {
            node.strMeta += ">";
            pElem->SetStartTagLen(nLenName + 2);
            pElem->nLength = nLenName + 2;
        }
        else
        {
            if (nFlags & MNF_WITHXHTMLSPACE)
            {
                node.strMeta += " />";
                pElem->SetStartTagLen(nLenName + 4);
                pElem->nLength = nLenName + 4;
            }
            else
            {
                node.strMeta += "/>";
                pElem->SetStartTagLen(nLenName + 3);
                pElem->nLength = nLenName + 3;
            }
        }
        pElem->SetEndTagLen(0);
    }
    else
    {
        // <NAME>value</NAME>
        mstr strValue;
        if (nFlags & MNF_WITHCDATA)
            strValue = x_EncodeCDATASection(szValue);
        else
            strValue = EscapeText(szValue, nFlags);
        Int32 nLenValue = (Int32)strValue.size();
        node.strMeta = "<";
        node.strMeta += szName;
        node.strMeta += ">";
        node.strMeta += strValue;
        node.strMeta += "</";
        node.strMeta += szName;
        node.strMeta += ">";
        pElem->SetEndTagLen(nLenName + 3);
        pElem->nLength = nLenName * 2 + nLenValue + 5;
        pElem->SetStartTagLen(nLenName + 2);
    }

    // Insert
    Int32 nReplace = x_InsertNew(iPosParent, iPosBefore, node);

    pElem->nStart = node.nStart;
    pElem->iElemChild = 0;
    if (nFlags & MNF_WITHNOEND)
        pElem->nFlags = MNF_NONENDED;
    else
        pElem->nFlags = 0;
    x_LinkElem(iPosParent, iPosBefore, iPos);

    x_Adjust(iPos, (Int32)node.strMeta.size() - nReplace);

    if (nFlags & MNF_CHILD)
        _SetPos(m_iPosParent, iPosParent, iPos);
    else
        _SetPos(iPosParent, iPos, 0);
    return True;
}

mstr MsXmlReader::x_GetSubDoc(Int32 iPos) const
{
    if (iPos)
    {
        Int32 nStart = m_aPos[iPos].nStart;
        Int32 nNext = nStart + m_aPos[iPos].nLength;
        LPCSTR szDoc = m_strDoc.c_str();
        Int32 nChar = nNext;
        if (!x_FindAny(szDoc, nChar) || szDoc[nChar] == '<')
            nNext = nChar;
        return m_strDoc.substr(nStart, nNext - nStart);
    }
    return "";
}

Boolean MsXmlReader::x_AddSubDoc(LPCSTR szSubDoc, MarkupNodeFlagsInternal nFlags)
{
    // Add subdocument, parse, and modify positions of affected elements
    //
    NodePos node(nFlags);
    Int32 iPosParent, iPosBefore;
    if (nFlags & MNF_CHILD)
    {
        // Add a subdocument under main position, before or after child
        if (!m_iPos)
            return False;
        iPosParent = m_iPos;
        iPosBefore = m_iPosChild;
    }
    else
    {
        // Add a subdocument under parent position, before or after main
        iPosParent = m_iPosParent;
        iPosBefore = m_iPos;
        node.nStart = m_nNodeOffset;
        node.nLength = m_nNodeLength;
    }

    // Parse subdocument
    Boolean bWellFormed = True;
    TokenPos token(szSubDoc, m_nFlags);
    Int32 iPosVirtual = x_GetFreePos();
    m_aPos[iPosVirtual].ClearVirtualParent();
    m_aPos[iPosVirtual].SetLevel(m_aPos[iPosParent].Level() + 1);
    Int32 iPos = x_ParseElem(iPosVirtual, token);
    if ((!iPos) || m_aPos[iPosVirtual].nFlags & MNF_ILLFORMED)
        bWellFormed = False;
    if (m_aPos[iPosVirtual].nFlags & MNF_ILLDATA)
        m_aPos[iPosParent].nFlags |= MNF_ILLDATA;

    // Extract subdocument without leading/trailing nodes
    Int32 nExtractStart = 0;
    Int32 iPosLast = m_aPos[iPos].iElemPrev;
    if (bWellFormed)
    {
        nExtractStart = m_aPos[iPos].nStart;
        Int32 nExtractLength = m_aPos[iPos].nLength;
        if (iPos != iPosLast)
        {
            nExtractLength = m_aPos[iPosLast].nStart - nExtractStart + m_aPos[iPosLast].nLength;
            bWellFormed = False; // treat as subdoc here, but return not well-formed
        }
        node.strMeta.assign(&szSubDoc[nExtractStart], nExtractLength);
    }
    else
    {
        node.strMeta = szSubDoc;
        node.nFlags |= MNF_WITHNOLINES;
    }

    // Insert
    Int32 nReplace = x_InsertNew(iPosParent, iPosBefore, node);

    // Adjust and link in the inserted elements
    // iPosVirtual will stop it from affecting rest of document
    Int32 nAdjust = node.nStart - nExtractStart;
    if (iPos && nAdjust)
    {
        x_Adjust(iPos, nAdjust);
        m_aPos[iPos].nStart += nAdjust;
    }
    Int32 iPosChild = iPos;
    while (iPosChild)
    {
        Int32 iPosNext = m_aPos[iPosChild].iElemNext;
        x_LinkElem(iPosParent, iPosBefore, iPosChild);
        iPosBefore = iPosChild;
        iPosChild = iPosNext;
    }
    x_ReleasePos(iPosVirtual);

    // Now adjust remainder of document
    x_Adjust(iPosLast, (Int32)node.strMeta.size() - nReplace, True);

    // Set position to top element of subdocument
    if (nFlags & MNF_CHILD)
        _SetPos(m_iPosParent, iPosParent, iPos);
    else // Main
        _SetPos(m_iPosParent, iPos, 0);
    return bWellFormed;
}

Int32 MsXmlReader::x_RemoveElem(Int32 iPos)
{
    // Remove element and all contained elements
    // Return NEW position
    //
    if (!iPos)
        return 0;

    // Determine whether any whitespace up to next tag
    Int32 nAfterEnd = m_aPos[iPos].StartAfter();
    LPCSTR szDoc = m_strDoc.c_str();
    Int32 nChar = nAfterEnd;
    if (!x_FindAny(szDoc, nChar) || szDoc[nChar] == '<')
        nAfterEnd = nChar;

    // Remove from document, adjust affected indexes, and unlink
    Int32 nLen = nAfterEnd - m_aPos[iPos].nStart;
    x_DocChange(m_aPos[iPos].nStart, nLen, mstr());
    x_Adjust(iPos, -nLen, True);
    Int32 iPosPrev = x_UnlinkElem(iPos);
    x_CheckSavedPos();
    return iPosPrev;
}

void MsXmlReader::x_LinkElem(Int32 iPosParent, Int32 iPosBefore, Int32 iPos)
{
    // Link in element, and initialize nFlags, and iElem indexes
    ElemPos* pElem = &m_aPos[iPos];
    pElem->iElemParent = iPosParent;
    if (iPosBefore)
    {
        // Link in after iPosBefore
        pElem->nFlags &= ~MNF_FIRST;
        pElem->iElemNext = m_aPos[iPosBefore].iElemNext;
        if (pElem->iElemNext)
            m_aPos[pElem->iElemNext].iElemPrev = iPos;
        else
            m_aPos[m_aPos[iPosParent].iElemChild].iElemPrev = iPos;
        m_aPos[iPosBefore].iElemNext = iPos;
        pElem->iElemPrev = iPosBefore;
    }
    else
    {
        // Link in as first child
        pElem->nFlags |= MNF_FIRST;
        if (m_aPos[iPosParent].iElemChild)
        {
            pElem->iElemNext = m_aPos[iPosParent].iElemChild;
            pElem->iElemPrev = m_aPos[pElem->iElemNext].iElemPrev;
            m_aPos[pElem->iElemNext].iElemPrev = iPos;
            m_aPos[pElem->iElemNext].nFlags ^= MNF_FIRST;
        }
        else
        {
            pElem->iElemNext = 0;
            pElem->iElemPrev = iPos;
        }
        m_aPos[iPosParent].iElemChild = iPos;
    }
    if (iPosParent)
        pElem->SetLevel(m_aPos[iPosParent].Level() + 1);
}

Int32 MsXmlReader::x_UnlinkElem(Int32 iPos)
{
    // Fix links to remove element and mark as deleted
    // return previous position or zero if none
    ElemPos* pElem = &m_aPos[iPos];

    // Find previous sibling and bypass removed element
    Int32 iPosPrev = 0;
    if (pElem->nFlags & MNF_FIRST)
    {
        if (pElem->iElemNext) // set next as first child
        {
            m_aPos[pElem->iElemParent].iElemChild = pElem->iElemNext;
            m_aPos[pElem->iElemNext].iElemPrev = pElem->iElemPrev;
            m_aPos[pElem->iElemNext].nFlags |= MNF_FIRST;
        }
        else // no children remaining
            m_aPos[pElem->iElemParent].iElemChild = 0;
    }
    else
    {
        iPosPrev = pElem->iElemPrev;
        m_aPos[iPosPrev].iElemNext = pElem->iElemNext;
        if (pElem->iElemNext)
            m_aPos[pElem->iElemNext].iElemPrev = iPosPrev;
        else
            m_aPos[m_aPos[pElem->iElemParent].iElemChild].iElemPrev = iPosPrev;
    }
    x_ReleaseSubDoc(iPos);
    return iPosPrev;
}

Int32 MsXmlReader::x_ReleasePos(Int32 iPos)
{
    Int32 iPosNext = m_aPos[iPos].iElemNext;
    m_aPos[iPos].iElemNext = m_iPosDeleted;
    m_aPos[iPos].nFlags = MNF_DELETED;
    m_iPosDeleted = iPos;
    return iPosNext;
}

Int32 MsXmlReader::x_ReleaseSubDoc(Int32 iPos)
{
    // Mark position structures as deleted by depth first traversal
    // Tricky because iElemNext used in traversal is overwritten for linked list of deleted
    // Return value is what iElemNext was before being overwritten
    //
    Int32 iPosNext = 0, iPosTop = iPos;
    while (1)
    {
        if (m_aPos[iPos].iElemChild)
            iPos = m_aPos[iPos].iElemChild;
        else
        {
            while (1)
            {
                iPosNext = x_ReleasePos(iPos);
                if (iPos == iPosTop)
                    return iPosNext;
                if (iPosNext)
                    break;
                iPos = m_aPos[iPos].iElemParent;
            }
            iPos = iPosNext;
        }
    }
    return iPosNext;
}

void MsXmlReader::x_CheckSavedPos()
{
    // Remove any saved positions now pointing to deleted elements
    // Must be done as part of element removal before position reassigned
    if (m_mapSavedPos.pTable)
    {
        for (Int32 nSlot = 0; nSlot < SavedPosMap::SPM_SIZE; ++nSlot)
        {
            SavedPos* pSavedPos = m_mapSavedPos.pTable[nSlot];
            if (pSavedPos)
            {
                Int32 nOffset = 0;
                Int32 nSavedPosCount = 0;
                while (1)
                {
                    if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_USED)
                    {
                        Int32 iPos = pSavedPos[nOffset].iPos;
                        if (!(m_aPos[iPos].nFlags & MNF_DELETED))
                        {
                            if (nSavedPosCount < nOffset)
                            {
                                pSavedPos[nSavedPosCount] = pSavedPos[nOffset];
                                pSavedPos[nSavedPosCount].nSavedPosFlags &= ~SavedPosMap::SPM_LAST;
                            }
                            ++nSavedPosCount;
                        }
                    }
                    if (pSavedPos[nOffset].nSavedPosFlags & SavedPosMap::SPM_LAST)
                    {
                        while (nSavedPosCount <= nOffset)
                            pSavedPos[nSavedPosCount++].nSavedPosFlags &= ~SavedPosMap::SPM_USED;
                        break;
                    }
                    ++nOffset;
                }
            }
        }
    }
}

void MsXmlReader::x_AdjustForNode(Int32 iPosParent, Int32 iPos, Int32 nShift)
{
    // Adjust affected indexes
    Boolean bAfterPos = True;
    if (!iPos)
    {
        // Change happened before or at first element under iPosParent
        // If there are any children of iPosParent, adjust from there
        // otherwise start at parent and adjust from there
        iPos = m_aPos[iPosParent].iElemChild;
        if (iPos)
        {
            m_aPos[iPos].nStart += nShift;
            bAfterPos = False;
        }
        else
        {
            iPos = iPosParent;
            m_aPos[iPos].nLength += nShift;
        }
    }
    x_Adjust(iPos, nShift, bAfterPos);
}

Boolean MsXmlReader::x_AddNode(MarkupNodeType nNodeType, LPCSTR szText, MarkupNodeFlagsInternal nFlags)
{
    // Only comments, DTDs, and processing instructions are followed by CRLF
    // Other nodes are usually concerned with mixed content, so no CRLF
    if (!(nNodeType & (MNT_PROCESSING_INSTRUCTION | MNT_COMMENT | MNT_DOCUMENT_TYPE)))
    {
        Int32& xTempFlags = (Int32&)nFlags;
        xTempFlags |= MNF_WITHNOLINES;
    }

    // Add node of nNodeType after current node position
    NodePos node(nFlags);
    if (!x_CreateNode(node.strMeta, nNodeType, szText))
        return False;

    // Locate where to add node relative to current node
    Int32 iPosBefore = m_iPos;
    Int32 iPosParent = m_iPosParent;
    node.nStart = m_nNodeOffset;
    node.nLength = m_nNodeLength;
    node.nNodeType = nNodeType;

    Int32 nReplace = x_InsertNew(iPosParent, iPosBefore, node);

    // If its a NEW element, create an ElemPos
    Int32 iPos = iPosBefore;
    if (nNodeType == MNT_ELEMENT)
    {
        // Set indexes
        iPos = x_GetFreePos();
        ElemPos* pElem = &m_aPos[iPos];
        pElem->nStart = node.nStart;
        pElem->SetStartTagLen(node.nLength);
        pElem->SetEndTagLen(0);
        pElem->nLength = node.nLength;
        node.nStart = 0;
        node.nLength = 0;
        pElem->iElemChild = 0;
        pElem->nFlags = 0;
        x_LinkElem(iPosParent, iPosBefore, iPos);
    }

    // Need to adjust element positions after iPos
    x_AdjustForNode(iPosParent, iPos, (Int32)node.strMeta.size() - nReplace);

    // Set current position
    m_iPos = iPos;
    m_iPosChild = 0;
    m_nNodeOffset = node.nStart;
    m_nNodeLength = node.nLength;
    m_nNodeType = nNodeType;
    //MARKUP_SETDEBUGSTATE;
    return True;
}

void MsXmlReader::x_RemoveNode(Int32 iPosParent, Int32& iPos, MarkupNodeType& nNodeType, Int32& nNodeOffset, Int32& nNodeLength)
{
    // Remove node and return NEW position
    //
    Int32 iPosPrev = iPos;

    // Removing an element?
    if (nNodeType == MNT_ELEMENT)
    {
        nNodeOffset = m_aPos[iPos].nStart;
        nNodeLength = m_aPos[iPos].nLength;
        iPosPrev = x_UnlinkElem(iPos);
        x_CheckSavedPos();
    }

    // Find previous node type, offset and length
    Int32 nPrevOffset = 0;
    if (iPosPrev)
        nPrevOffset = m_aPos[iPosPrev].StartAfter();
    else if (iPosParent)
        nPrevOffset = m_aPos[iPosParent].StartContent();
    TokenPos token(m_strDoc, m_nFlags);
    NodePos node;
    token.nNext = nPrevOffset;
    MarkupNodeType nPrevType = MNT_INVALID;
    while (token.nNext < nNodeOffset)
    {
        nPrevOffset = token.nNext;
        nPrevType = x_ParseNode(token, node);
    }
    Int32 nPrevLength = nNodeOffset - nPrevOffset;
    if (!nPrevLength)
    {
        // Previous node is iPosPrev element
        nPrevOffset = 0;
        if (iPosPrev)
            nPrevType = MNT_ELEMENT;
    }

    // Remove node from document
    x_DocChange(nNodeOffset, nNodeLength, mstr());
    x_AdjustForNode(iPosParent, iPosPrev, -nNodeLength);

    // Was removed node a lone end tag?
    if (nNodeType == MNT_LONE_END_TAG)
    {
        // See if we can unset parent MNF_ILLDATA flag
        token.nNext = m_aPos[iPosParent].StartContent();
        Int32 nEndOfContent = token.nNext + m_aPos[iPosParent].ContentLen();
        Int32 iPosChild = m_aPos[iPosParent].iElemChild;
        while (token.nNext < nEndOfContent)
        {
            if (x_ParseNode(token, node) <= 0)
                break;
            if (node.nNodeType == MNT_ELEMENT)
            {
                token.nNext = m_aPos[iPosChild].StartAfter();
                iPosChild = m_aPos[iPosChild].iElemNext;
            }
        }
        if (token.nNext == nEndOfContent)
            m_aPos[iPosParent].nFlags &= ~MNF_ILLDATA;
    }

    nNodeType = nPrevType;
    nNodeOffset = nPrevOffset;
    nNodeLength = nPrevLength;
    iPos = iPosPrev;
}
//
//Boolean MsXmlReader::LoadToProto(LPBASEPROTO xProto)
//{
//    MsXmlReader& xMsXmlReader = *this;
//    const Reflection* xReflection = xProto->GetReflection();
//    const Descriptor* xDescriptor = xProto->GetDescriptor();
//    Int32 xFieldCount = xDescriptor->field_count();
//    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
//    {
//        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
//        const mstr& xName = xField->name();
//        switch (xField->type())
//        {
//            case ProtoType::TYPE_FLOAT:
//            {
//                if (xField->is_repeated())
//                {
//                    xMsXmlReader.FindChildElem(xName);
//                    do {
//                        xReflection->AddFloat(xProto, xField, (float)lua_tonumber(lua_state_, -1));
//
//                        mstr xAddr = xMsXmlReader.GetAttrib("addr");
//                        mstr xStrPort = xMsXmlReader.GetAttrib("port");
//                        WORD xPort = (WORD)atoi(xStrPort.c_str());
//                        m_ListCenterIntAddr.Add(MsBase::IPPP2Int(xAddr.c_str(), xPort, xPort));
//                    } while (xMsXmlReader.FindNextElem("node"));
//
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddFloat(xProto, xField, (float)lua_tonumber(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetFloat(xProto, xField, (float)lua_tonumber(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_DOUBLE:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddDouble(xProto, xField, lua_tonumber(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetDouble(xProto, xField, lua_tonumber(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_INT64:
//            case ProtoType::TYPE_SINT64:
//            case ProtoType::TYPE_FIXED64:
//            case ProtoType::TYPE_SFIXED64:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddInt64(xProto, xField, lua_tointeger(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetInt64(xProto, xField, lua_tointeger(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_UINT64:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_INT32:
//            case ProtoType::TYPE_SINT32:
//            case ProtoType::TYPE_FIXED32:
//            case ProtoType::TYPE_SFIXED32:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_UINT32:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_ENUM:
//            {
//                AssertNormal(False, "lua_table_config not supported enum");
//            }break;
//
//            case ProtoType::TYPE_BOOL:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
//                }
//            }break;
//
//            case ProtoType::TYPE_STRING:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_BYTES:
//            {
//                //AssertNormal(False, "lua_table_config not supported bytes");
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        lua_rawgeti(lua_state_, -1, i);
//                        xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
//                }
//            }break;
//
//            case ProtoType::TYPE_MESSAGE:
//            {
//                if (xField->is_repeated())
//                {
//                    int xLen = (int)lua_objlen(lua_state_, -1);
//                    for (int i = 1; i <= xLen; i++)
//                    {
//                        Message* xMsg = xReflection->AddMessage(xProto, xField);
//                        lua_rawgeti(lua_state_, -1, i);
//                        this->LoadParseLuaTableListData(xMsg);
//                        lua_pop(lua_state_, 1);
//                    }
//                }
//                else
//                {
//                    Message* xMsg = xReflection->MutableMessage(xProto, xField);
//                    this->LoadParseLuaTableListData(xMsg);
//                }
//            }break;
//        }
//    }
//
//}
//


//MsXmlReader& operator[](mstr& szName) { return (*this)[szName.c_str()]; }
MsXmlReader& MsXmlReader::operator[](LPCSTR szName)
{
    if (szName == nullptr) { this->SelectRootElem(szName); }
    else if (!m_IsOpenOK) { m_IsLastOK = this->Load(szName); }
    else if (!m_iPos) { this->SelectRootElem(szName); }
    else { m_IsLastOK = this->FindChildElem(szName); }
    return *this;
}


Boolean MsXmlReader::IsOK()
{
    return m_IsLastOK;
}
