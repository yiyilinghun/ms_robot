#pragma once

class MsMemoryStream
{
public:
    MsMemoryStream(DWORD dwBufferSize = 8192)
        : m_spBuffer(NEW Byte[dwBufferSize], [](LPBYTE slpBuffer) { SAFE_DELETE_ARRAY(slpBuffer); })
        , m_BufferSize(dwBufferSize)
        , m_InOffect(0)
        , m_OutOffect(0)
        , m_IsNeedFree(True)
    {
        ;
    }

    MsMemoryStream(LPBYTE lpBuffer, DWORD dwBufferSize)
        : m_spBuffer(lpBuffer, [](LPBYTE slpBuffer) {; })
        , m_BufferSize(dwBufferSize)
        , m_InOffect(0)
        , m_OutOffect(0)
        , m_IsNeedFree(False)
    {
        ;
    }

    ~MsMemoryStream()
    {
        if (m_IsNeedFree)
        {
            //SAFE_DELETE_ARRAY(m_spBuffer);
        }
    }

    template<class TP>
    MsMemoryStream& operator >> (TP& xParam)
    {
        if (m_OutOffect + sizeof(TP) <= m_BufferSize)
        {
            xParam = *((TP*)(m_spBuffer.get() + m_OutOffect));
            m_OutOffect += sizeof(TP);
        }
        else { throw "内存流越界!"; }
        return *this;
    }

    MsMemoryStream& Read(LPVOID xData, DWORD xLen)
    {
        if (m_OutOffect + xLen <= m_BufferSize)
        {
            memcpy(xData, m_spBuffer.get() + m_OutOffect, xLen);
            m_OutOffect += xLen;
        }
        else { throw "内存流越界!"; }
        return *this;
    }

    template<class TP>
    MsMemoryStream& operator << (TP& xParam)
    {
        if (m_InOffect + sizeof(TP) <= m_BufferSize)
        {
            *((TP*)(m_spBuffer.get() + m_InOffect)) = xParam;
            m_InOffect += sizeof(TP);
        }
        else { throw "内存流越界!"; }
        return *this;
    }

    template<class TP>
    MsMemoryStream& operator << (TP&& xParam)
    {
        if (m_InOffect + sizeof(TP) <= m_BufferSize)
        {
            *((TP*)(m_spBuffer.get() + m_InOffect)) = xParam;
            m_InOffect += sizeof(TP);
        }
        else { throw "内存流越界!"; }
        return *this;
    }

    MsMemoryStream& WriteLenStr(mstr& xStr)
    {
        return WriteLenStr(MsBase::M2W(xStr));
    }

    MsMemoryStream& WriteLenStr(mstr&& xStr)
    {
        return WriteLenStr(MsBase::M2W(xStr));
    }
    MsMemoryStream& WriteLenStr(wstr& xStr)
    {
        DWORD xStrLen = (DWORD)xStr.size();
        BYTE xByteFlag = 0xFF;
        if (xStrLen < 0xFF)
        {
            BYTE xByte = (BYTE)xStrLen;
            this->Write(&xByte, sizeof(xByte));
        }
        else
        {
            this->Write(&xByteFlag, sizeof(xByteFlag));
            this->Write(&xStrLen, sizeof(xStrLen));
        }
        return this->Write(xStr.data(), (DWORD)xStr.size() * sizeof(wchar_t));
    }
    MsMemoryStream& WriteLenStr(wstr&& xStr)
    {
        DWORD xStrLen = (DWORD)xStr.size();
        BYTE xByteFlag = 0xFF;
        if (xStrLen < 0xFF)
        {
            BYTE xByte = (BYTE)xStrLen;
            this->Write(&xByte, sizeof(xByte));
        }
        else
        {
            this->Write(&xByteFlag, sizeof(xByteFlag));
            this->Write(&xStrLen, sizeof(xStrLen));
        }
        return this->Write(xStr.data(), (DWORD)xStr.size() * sizeof(wchar_t));
    }

    MsMemoryStream& Write(LPCVOID xData, DWORD xLen)
    {
        if (m_InOffect + xLen <= m_BufferSize)
        {
            memcpy(m_spBuffer.get() + m_InOffect, xData, xLen);
            m_InOffect += xLen;
        }
        else { throw "内存流越界!"; }
        return *this;
    }

    DWORD GetOutSurplusSize() { return m_BufferSize - m_OutOffect; }
    DWORD GetInSurplusSize() { return m_BufferSize - m_InOffect; }
    DWORD GetBuffSize() { return m_BufferSize; }
    LPBYTE GetBuff() { return m_spBuffer.get(); }
    LPBYTE GetBuff(DWORD& dwOutOffect, DWORD& dwInOffect) { dwOutOffect = m_OutOffect; dwInOffect = m_InOffect; return m_spBuffer.get(); }

    Boolean OutSeek(DWORD dwOffect)
    {
        if (dwOffect >= 0 && dwOffect < m_BufferSize)
        {
            m_OutOffect = dwOffect;
            return True;
        }
        return False;
    }

    Boolean InSeek(DWORD dwOffect)
    {
        if (dwOffect >= 0 && dwOffect < m_BufferSize)
        {
            m_InOffect = dwOffect;
            return True;
        }
        return False;
    }

    //private:
    Boolean m_IsNeedFree;
    DWORD   m_InOffect;
    DWORD   m_OutOffect;
    SafePtr<Byte> m_spBuffer;
    DWORD   m_BufferSize;
};
