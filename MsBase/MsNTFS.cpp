#include "Precompiled.h"
//MsNTFS::MsNTFS(mstr xBaseDir)
//    : m_AutoReleaseFun([](HANDLE hFile) { ::CloseHandle(hFile); })
//{
//#ifdef _WIN64
//    CHAR szTemp[MAX_PATH];
//    ::GetFullPathNameA(xBaseDir.c_str(), MAX_PATH, szTemp, nullptr);
//    m_BaseDirA = szTemp;
//    m_BaseDirW = MsBase::M2W(m_BaseDirA);
//    m_DrvName = _T("?:\\");
//    m_DrvFileName = _T("\\\\.\\?:");
//    ((LPWCH)(m_DrvName.data()))[0] = szTemp[0];
//    ((LPWCH)(m_DrvFileName.data()))[4] = szTemp[0];
//
//    WCHAR FileSystemName[MAX_PATH];
//    DWORD xMaximumComponentLength;
//    if (::GetVolumeInformation(m_DrvName.c_str(), 0, 0, 0, &xMaximumComponentLength, 0, FileSystemName, MAX_PATH))
//    {
//        // 检查是否为 NTFS或ReFS 格式
//        //if (0 == _tcscmp(FileSystemName, _T("NTFS")) || 0 == _tcscmp(FileSystemName, _T("ReFS")))
//
//        // 检查是否为 NTFS 格式
//        if (0 == wcscmp(FileSystemName, L"NTFS"))
//        {
//            m_hVolDrvFile = ::CreateFile(m_DrvFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
//            if (m_hVolDrvFile != INVALID_HANDLE_VALUE)
//            {
//                AutoRelease<LPTSTR> xBuffer(True);
//                xBuffer = NEW TCHAR[100 * MB_SIZE];
//                DWORD xBytesReturned = 0;
//
//                // 如果创建过，且没有用FSCTL_DELETE_USN_JOURNAL关闭，则可以跳过这一步
//                CREATE_USN_JOURNAL_DATA xCreateUSNUSNJournalData = { 0, 0 };
//                if (::DeviceIoControl(m_hVolDrvFile, FSCTL_CREATE_USN_JOURNAL, &xCreateUSNUSNJournalData, sizeof(xCreateUSNUSNJournalData), nullptr, 0, &xBytesReturned, nullptr))
//                {
//                    USN_JOURNAL_DATA xUSNJournalData;
//                    if (::DeviceIoControl(m_hVolDrvFile, FSCTL_QUERY_USN_JOURNAL, nullptr, 0, &xUSNJournalData, sizeof(xUSNJournalData), &xBytesReturned, nullptr))
//                    {
//                        // 使用FSCTL_ENUM_USN_DATA可以列出所有存在的文件信息，但UsnRecord->Reason等信息是无效的
//                        MFT_ENUM_DATA med = { 0, xUSNJournalData.FirstUsn, xUSNJournalData.NextUsn, 2, 4 };
//                        for (; DeviceIoControl(m_hVolDrvFile, FSCTL_ENUM_USN_DATA, &med, sizeof(med), xBuffer.m_RealPtr, (100 * MB_SIZE), &xBytesReturned, nullptr);
//                            med.StartFileReferenceNumber = *(USN*)&(xBuffer.m_RealPtr))
//                        {
//                            DWORD dwRetBytes = xBytesReturned - sizeof(USN);
//                            USN_RECORD_UNION* xUSNRecord = (USN_RECORD_UNION*)((LPSTR)(xBuffer.m_RealPtr) + sizeof(USN));
//                            while (dwRetBytes > 0)
//                            {
//                                MsNTFSNode xFileNode;
//                                xFileNode.FileRefNo.CopyFrom(&(xUSNRecord->V3.FileReferenceNumber));
//                                xFileNode.ParentRefNo.CopyFrom(&(xUSNRecord->V3.ParentFileReferenceNumber));
//                                xFileNode.FileAttributes = xUSNRecord->V3.FileAttributes;
//                                WCHAR szNameTemp[MAX_PATH];
//                                memcpy(szNameTemp, xUSNRecord->V3.FileName, xUSNRecord->V3.FileNameLength); szNameTemp[xUSNRecord->V3.FileNameLength / 2] = L'\0';
//                                xFileNode.Name = szNameTemp;
//
//                                m_Dict_Nodes[xFileNode.FileRefNo] = xFileNode;
//                                m_MDict_FileName.Add(MsBase::W2M(xFileNode.Name), &m_Dict_Nodes[xFileNode.FileRefNo]);
//
//                                dwRetBytes -= xUSNRecord->V3.RecordLength;
//                                xUSNRecord = (PUSN_RECORD_UNION)((LPSTR)xUSNRecord + xUSNRecord->V3.RecordLength);
//                            }
//                        }
//
//                        FAST_FOREACH(m_Dict_Nodes)
//                        {
//                            auto& xFileNode = m_Dict_Nodes.Value();
//                            if (m_Dict_Nodes.ContainsKey(xFileNode.ParentRefNo))
//                            {
//                                auto& xParentFileNode = m_Dict_Nodes[xFileNode.ParentRefNo];
//                                xFileNode.m_ParentNode = &xParentFileNode;
//                            }
//                        }
//
//                        FAST_FOREACH(m_Dict_Nodes)
//                        {
//                            auto& xFileNode = m_Dict_Nodes.Value();
//                            m_Dict_FileFullName.Add(xFileNode.GetFullNameA(), &xFileNode);
//                        }
//
//                        if (GetLastError() != ERROR_HANDLE_EOF)
//                        {
//                            AssertLog("%s读取失败ErrorCode[%d]", MsBase::W2M(m_DrvFileName).c_str(), ::GetLastError());
//                        }
//                    }
//                }
//            }
//            else
//            {
//                AssertLog("%s打开失败ErrorCode[%d]", MsBase::W2M(m_DrvFileName).c_str(), ::GetLastError());
//            }
//        }
//    }
//#endif
//}

MsNTFS::MsNTFS(mstr xBaseDir)
    : m_AutoReleaseFun([](HANDLE hFile) { ::CloseHandle(hFile); })
{
    CHAR szTemp[MAX_PATH];
    ::GetFullPathNameA(xBaseDir.c_str(), MAX_PATH, szTemp, nullptr);
    m_BaseDirA = szTemp;
    m_BaseDirW = MsBase::M2W(m_BaseDirA);
}

MsNTFS::~MsNTFS()
{
    if (m_hVolDrvFile != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hVolDrvFile);
        m_hVolDrvFile = INVALID_HANDLE_VALUE;
    }
}

//QWORD MsNTFS::GetNodeCount()
//{
//    return m_MDict_FileName.GetBigCount();
//}
//
//Boolean MsNTFS::ExistPathFile(mstr xPathFile)
//{
//    return m_Dict_FileFullName.ContainsKey(xPathFile);
//}

void MsNTFS::CreateFullPath(LPCSTR xPath)
{
    MsVector<mstr> xPathList;
    MsBase::StringSplit(xPath, '\\', xPathList);
    mstr xTemp;
    for (Int32 i = 0; i < xPathList.GetCount(); i++)
    {
        xTemp += xPathList[i];
        if (!this->CheckPathFile(xTemp))
        {
            ::CreateDirectoryA((m_BaseDirA + xTemp).c_str(), nullptr);
            //m_Dict_FileFullName.Add(xTemp, nullptr);
        }
        xTemp += '\\';
    }
}

QWORD MsNTFS::GetFileSize(mstr xPathFile)
{
    xPathFile = m_BaseDirA + xPathFile;
    AutoRelease<HANDLE> hFile(::CreateFileA(xPathFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE)
    {
        AssertNormal(False, "读取文件[%s]尺寸失败[%d]", xPathFile.c_str(), ::GetLastError());
        return 0;
    }

    LARGE_INTEGER xFileSize;
    if (!::GetFileSizeEx(hFile.m_RealPtr, &xFileSize)) { xFileSize.QuadPart = 0; }
    return xFileSize.QuadPart;
}

Boolean MsNTFS::CheckPathFile(mstr xPathFile)
{
    xPathFile = m_BaseDirA + xPathFile;

    return ::PathFileExistsA(xPathFile.c_str());
}

Boolean MsNTFS::CheckSaveFile(mstr xPathFile, LPBYTE xData, DWORD dwDataLen)
{
    xPathFile = m_BaseDirA + xPathFile;

    AutoRelease<HANDLE> hFile(::CreateFileA(xPathFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE)
    {
        Int32 xError = ::GetLastError();
        if (ERROR_FILE_EXISTS == xError)
        {
            return False;
        }
        else
        {
            AssertLog("检查文件[%s]失败[%d]", xPathFile.c_str(), xError); return False;
            return False;
        }
    }
    return ::WriteFile(hFile.m_RealPtr, xData, dwDataLen, &dwDataLen, nullptr);
}

Boolean MsNTFS::CheckSaveIndexFile(mstr xPathIndexFile, mstr& xPathFile)
{
    xPathIndexFile = m_BaseDirA + xPathIndexFile;

    AutoRelease<HANDLE> hFile(::CreateFileA(xPathIndexFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE)
    {
        Int32 xError = ::GetLastError();
        if (ERROR_FILE_EXISTS == xError)
        {
            return False;
        }
        else
        {
            AssertLog("检查文件[%s]失败[%d]", xPathIndexFile.c_str(), xError); return False;
            return False;
        }
    }
    DWORD dwDataLen = (DWORD)xPathFile.size();
    return ::WriteFile(hFile.m_RealPtr, xPathFile.data(), dwDataLen, &dwDataLen, nullptr);
}

Boolean MsNTFS::SaveIndexFile(mstr xPathIndexFile, mstr& xPathFile)
{
    xPathIndexFile = m_BaseDirA + xPathIndexFile;

    AutoRelease<HANDLE> hFile(::CreateFileA(xPathIndexFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE) { AssertLog("创建文件[%s]失败[%d]", xPathIndexFile.c_str(), ::GetLastError()); return False; }
    DWORD dwDataLen = (DWORD)xPathFile.size();
    return ::WriteFile(hFile.m_RealPtr, xPathFile.data(), dwDataLen, &dwDataLen, nullptr);
}

Boolean MsNTFS::ReadIndexFile(mstr xPathIndexFile, mstr& xPathFile)
{
    xPathIndexFile = m_BaseDirA + xPathIndexFile;

    AutoRelease<HANDLE> hFile(::CreateFileA(xPathIndexFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE)
    {
        AssertNormal(False, "读取文件[%s]失败[%d]", xPathIndexFile.c_str(), ::GetLastError());
        return False;
    }
    CHAR xData[MAX_PATH * 10];
    LARGE_INTEGER xFileSize;
    if (!::GetFileSizeEx(hFile.m_RealPtr, &xFileSize)) { xFileSize.QuadPart = 0; }
    if (::ReadFile(hFile.m_RealPtr, xData, xFileSize.LowPart, &xFileSize.LowPart, nullptr))
    {
        xData[xFileSize.LowPart] = 0;
        xPathFile = xData;
        return True;
    }
    return False;
}

Boolean MsNTFS::ReadFileFromIndex(mstr xPathIndexFile, LPBYTE xData, DWORD& dwMaxDataLen)
{
    mstr xPathFile;
    if (this->ReadIndexFile(xPathIndexFile, xPathFile))
    {
        return this->ReadFile(xPathFile, xData, dwMaxDataLen);
    }
    return False;
}

Boolean MsNTFS::SaveFile(mstr xPathFile, LPBYTE xData, DWORD dwDataLen, Boolean IsAppend)
{
    xPathFile = m_BaseDirA + xPathFile;

    AutoRelease<HANDLE> hFile(m_AutoReleaseFun);
    if (IsAppend)
    {
        hFile = ::CreateFileA(xPathFile.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hFile.m_RealPtr == INVALID_HANDLE_VALUE) { AssertLog("追加文件[%s]失败[%d]", xPathFile.c_str(), ::GetLastError()); return False; }
        ::SetFilePointer(hFile.m_RealPtr, 0, 0, FILE_END);
    }
    else
    {
        hFile = ::CreateFileA(xPathFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
        if (hFile.m_RealPtr == INVALID_HANDLE_VALUE) { AssertLog("创建文件[%s]失败[%d]", xPathFile.c_str(), ::GetLastError()); return False; }
    }
    return ::WriteFile(hFile.m_RealPtr, xData, dwDataLen, &dwDataLen, nullptr);
}

Boolean MsNTFS::LoadFile(mstr xPathFile)
{
    xPathFile = m_BaseDirA + xPathFile;

    SafePtr<void> hFile(::CreateFileA(xPathFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr), m_AutoReleaseFun);
    if (hFile.get() == INVALID_HANDLE_VALUE)
    {
        AssertNormal(False, "加载文件[%s]失败[%d]", xPathFile.c_str(), ::GetLastError());
        return False;
    }
    m_Dict_LoadFiles.Add(m_NextIndex++, hFile);
    DWORD dwHiDataLen = 0;
    LARGE_INTEGER xFileSize;
    if (!::GetFileSizeEx(hFile.get(), &xFileSize)) { xFileSize.QuadPart = 0; }
    return True;
}

Boolean MsNTFS::ReadFile(mstr xPathFile, LPBYTE xData, DWORD& dwMaxDataLen)
{
    xPathFile = m_BaseDirA + xPathFile;

    AutoRelease<HANDLE> hFile(::CreateFileA(xPathFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr), m_AutoReleaseFun);
    if (hFile.m_RealPtr == INVALID_HANDLE_VALUE)
    {
        AssertNormal(False, "读取文件[%s]失败[%d]", xPathFile.c_str(), ::GetLastError());
        return False;
    }
    //DWORD dwHiDataLen = 0;
    LARGE_INTEGER xFileSize;
    if (!::GetFileSizeEx(hFile.m_RealPtr, &xFileSize)) { xFileSize.QuadPart = 0; }
    dwMaxDataLen = xFileSize.LowPart;
    return ::ReadFile(hFile.m_RealPtr, xData, xFileSize.LowPart, &xFileSize.LowPart, nullptr);
}

Boolean MsNTFS::SaveProto(mstr xPathFile, BaseProto& xProto)
{
    m_dwDataLen = xProto.ByteSize();
    xProto.SerializeToArray(m_BuffData, m_dwDataLen);
    return this->SaveFile(xPathFile, m_BuffData, m_dwDataLen);
}

Boolean MsNTFS::CheckSaveProto(mstr xPathFile, BaseProto& xProto)
{
    m_dwDataLen = xProto.ByteSize();
    xProto.SerializeToArray(m_BuffData, m_dwDataLen);
    return this->CheckSaveFile(xPathFile, m_BuffData, m_dwDataLen);
}

Boolean MsNTFS::ReadProto(mstr xPathFile, BaseProto& xProto)
{
    m_dwDataLen = CountOf(m_BuffData);
    this->ReadFile(xPathFile, m_BuffData, m_dwDataLen);
    return xProto.ParseFromArray(m_BuffData, m_dwDataLen);
}

Boolean MsNTFS::CheckLoadProto(mstr xPathFile, BaseProto& xProto)
{
    if (this->CheckPathFile(xPathFile))
    {
        return this->ReadProto(xPathFile, xProto);
    }
    return False;
}

Boolean MsNTFS::ReadProtoFromIndex(mstr xPathIndexFile, BaseProto& xProto)
{
    m_dwDataLen = CountOf(m_BuffData);
    this->ReadFileFromIndex(xPathIndexFile, m_BuffData, m_dwDataLen);
    return xProto.ParseFromArray(m_BuffData, m_dwDataLen);
}

void MsNTFS::EnumDirAllFiles(mstr xDirNameA, OUT MsVector<mstr>& xListFiles, Boolean xRetFullName)
{
    wstr xDirName = MsBase::M2W(xDirNameA);
    wstr xDo = L".";
    wstr xDoo = L"..";
    HANDLE hContext = ::FindFirstFile((m_BaseDirW + xDirName + L"\\*.*").c_str(), &m_NextInfo);
    if (hContext == INVALID_HANDLE_VALUE)
    {
        AssertLog("遍历路径失败[%d]", ::GetLastError());
        return;
    }

    do
    {
        if (m_NextInfo.cFileName == xDo || m_NextInfo.cFileName == xDoo)
        {
            continue;
        }
        if (xRetFullName)
        {
            xListFiles.Add(MsBase::W2M(m_BaseDirW + xDirName + L"\\" + m_NextInfo.cFileName));
        }
        else
        {
            xListFiles.Add(MsBase::W2M(m_NextInfo.cFileName));
        }
    } while (FindNextFile(hContext, &m_NextInfo));
    ::FindClose(hContext);
}

















