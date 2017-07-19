#pragma once

//struct MS_FILE_ID_128
//{
//    Byte Identifier[16];
//    inline void CopyFrom(LPVOID xData)
//    {
//        *((QWORD*)this) = *((QWORD*)xData);
//        *(((QWORD*)this) + 1) = *(((QWORD*)xData) + 1);
//    }
//
//    bool operator<(const MS_FILE_ID_128& b) const
//    {
//        if (*((QWORD*)this) < *((QWORD*)&b))
//        {
//            return true;
//        }
//        else
//        {
//            if (*(((QWORD*)this) + 1) < *(((QWORD*)&b) + 1))
//            {
//                return true;
//            }
//        }
//        return false;
//    }
//};

//struct MsNTFSNode
//{
//    mstr GetFullNameA() { if (m_ParentNode) { return m_ParentNode->GetFullNameA() + "\\" + MsBase::W2M(Name); } else { return MsBase::W2M(Name); } }
//    wstr GetFullNameW() { if (m_ParentNode) { return m_ParentNode->GetFullNameW() + L"\\" + Name; } else { return Name; } }
//    tstr GetFullName() { if (m_ParentNode) { return m_ParentNode->GetFullName() + _T("\\") + Name; } else { return Name; } }
//    MS_FILE_ID_128 FileRefNo;
//    MS_FILE_ID_128 ParentRefNo;
//    DWORD FileAttributes;
//    wstr Name;
//    MsNTFSNode* m_ParentNode = nullptr;
//};

class MsNTFS
{
public:
    MsNTFS(mstr xBaseDir);
    ~MsNTFS();

    //QWORD GetNodeCount();
    //Boolean ExistPathFile(mstr xPathFile);

    void CreateFullPath(LPCSTR xPath);
    QWORD GetFileSize(mstr xPathFile);

    Boolean CheckPathFile(mstr xPathFile);
    Boolean CheckSaveFile(mstr xPathFile, LPBYTE xData, DWORD dwDataLen);
    Boolean CheckSaveIndexFile(mstr xPathIndexFile, mstr& xPathFile);

    Boolean SaveIndexFile(mstr xPathIndexFile, mstr& xPathFile);
    Boolean ReadIndexFile(mstr xPathIndexFile, mstr& xPathFile);
    Boolean ReadFileFromIndex(mstr xPathIndexFile, LPBYTE xData, DWORD& dwMaxDataLen);

    Boolean SaveFile(mstr xPathFile, LPBYTE xData, DWORD dwDataLen, Boolean IsAppend = False);
    Boolean SaveProto(mstr xPathFile, BaseProto& xProto);
    Boolean CheckSaveProto(mstr xPathFile, BaseProto& xProto);

    Boolean LoadFile(mstr xPathFile);
    Boolean ReadFile(mstr xPathFile, LPBYTE xData, DWORD& dwMaxDataLen);
    Boolean ReadProto(mstr xPathFile, BaseProto& xProto);
    Boolean CheckLoadProto(mstr xPathFile, BaseProto& xProto);
    Boolean ReadProtoFromIndex(mstr xPathIndexFile, BaseProto& xProto);

    void EnumDirAllFiles(mstr xDirName, OUT MsVector<mstr>& xListFiles, Boolean xRetFullName = False);
private:
    WIN32_FIND_DATA m_NextInfo;
private:
    AutoRelease<HANDLE>::LP_AUTO_RELEASE_FUN1 m_AutoReleaseFun;

    wstr m_BaseDirW;
    mstr m_BaseDirA;

    tstr m_DrvName;
    tstr m_DrvFileName;
    HANDLE m_hVolDrvFile = INVALID_HANDLE_VALUE;

    BYTE m_BuffData[10 * MB_SIZE];
    DWORD m_dwDataLen;

    Int64 m_NextIndex = 0;
    MsDictionary<Int64, SafePtr<void>> m_Dict_LoadFiles;

    //MsDictionary<MS_FILE_ID_128, MsNTFSNode>    m_Dict_Nodes;
    //MsMulDictionary<mstr, MsNTFSNode*>          m_MDict_FileName;
    //MsUnorderedDictionary<mstr, MsNTFSNode*>    m_Dict_FileFullName;
};
