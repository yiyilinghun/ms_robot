#include "Precompiled.h"
#include "ZipFunction.h"
#include <io.h>
#include <ShlObj.h>

namespace ZipUtils
{
    ZRESULT ExtractZipToDir(LPCSTR lpszZipFullName, MsList<mstr>& szFilePathArr, LPCSTR lpszUnZipPath)
    {
        CHAR buffer[MAX_PATH] = { 0 };
        mstr strUnZipPath = lpszUnZipPath;
        DWORD zResult = ZR_OK;

        if (strUnZipPath != "")
        {
            // 如果文件路径不存在先创建,存在不做任何修改
            SHCreateDirectoryExA(nullptr, lpszUnZipPath, nullptr);
        }
        else
        {
            GetCurrentDirectoryA(MAX_PATH, buffer);
            strUnZipPath = buffer;
            SHCreateDirectoryExA(nullptr, strUnZipPath.c_str(), nullptr);
        }

        HZIP hz = OpenZip(lpszZipFullName, 0);
        ZIPENTRY ze;

        GetZipItem(hz, -1, &ze);
        int numitems = ze.index;

        for (int zi = 0; zi < numitems; zi++)
        {
            ZIPENTRY ze_;
            GetZipItem(hz, zi, &ze_);
            zResult = UnzipItem(hz, zi, (strUnZipPath + "\\" + ze_.name).c_str());
            szFilePathArr.Add(ze_.name);
            if (zResult != ZR_OK)
            {
#ifndef _UNICODE 
                // 判断文件是否存在
                if (_access(szFilePathArr[zi], 0))
                {
                    // 文件不存在的时候
                    return zResult;
                }
#else
                if (_access((char*)(LPTSTR)(LPCTSTR)szFilePathArr[zi].c_str(), 0))
                {
                    // 文件不存在的时候
                    CloseZip(hz);
                    return zResult;
                }
#endif
            }
        }
        CloseZip(hz);
        return zResult;
    }

    Boolean ExtractZipToMemory(LPCSTR lpszZipFullName, MsUnorderedDictionary<mstr, ZIP_RES_INFO>& xDictZipRes)
    {
        HZIP hz = OpenZip(lpszZipFullName, 0);
        ZIPENTRY xZipEntry;
        GetZipItem(hz, -1, &xZipEntry);
        int numitems = xZipEntry.index;
        for (int zi = 0; zi < numitems; zi++)
        {
            if (ZR_OK == GetZipItem(hz, zi, &xZipEntry))
            {
                ZIP_RES_INFO zri;
                zri.m_DataAddr = NEW Byte[xZipEntry.comp_size];
                zri.m_DataLen = xZipEntry.comp_size;
                if (ZR_OK == UnzipItem(hz, zi, zri.m_DataAddr, zri.m_DataLen))
                {
                    xDictZipRes.Add(xZipEntry.name, zri);
                }
            }
        }
        CloseZip(hz);
        return True;
    }

    //Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes)
    //{
    //    return ExtractZipToMemoryByName(MsBase::M2W(lpszZipFullName).c_str(), MsBase::M2W(szZipItemName).c_str(), xZipRes);
    //}

    Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes, LPCSTR szPassword)
    {
        //CHAR buffer[MAX_PATH] = { 0 };
        HZIP hz = OpenZip(lpszZipFullName, szPassword);

        ZIPENTRY xZipEntry;
        int zi;
        if (ZR_OK == FindZipItem(hz, szZipItemName, true, &zi, &xZipEntry))
        {
            xZipRes.m_DataAddr = NEW Byte[xZipEntry.unc_size];
            xZipRes.m_DataLen = xZipEntry.unc_size;
            if (ZR_OK == UnzipItem(hz, zi, xZipRes.m_DataAddr, xZipRes.m_DataLen))
            {
                CloseZip(hz);
                return True;
            }
        }
        CloseZip(hz);
        return False;
    }

    ZRESULT DirToZip(LPCSTR lpszSrcPath, LPCSTR lpszZipName, HZIP& hz, LPCSTR lpszDestPath, Boolean IsCreateDir)
    {
        mstr strFileName;
        DWORD zResult = ZR_OK;
        CHAR buffer[MAX_PATH] = { 0 };

        mstr strDestPath = lpszDestPath;

        if (IsCreateDir)
        {        // 这里边的只执行一次
            if (strDestPath != "")
            {
                // 如果解压路径文件夹不存在 先创建,存在 不做任何修改
                ::SHCreateDirectoryExA(nullptr, lpszDestPath, nullptr);
            }
            else
            {
                ::GetCurrentDirectoryA(MAX_PATH, buffer);
                strDestPath = buffer;
                ::SHCreateDirectoryExA(nullptr, strDestPath.c_str(), nullptr);
            }
            hz = CreateZip((strDestPath + "\\" + lpszZipName).c_str(), 0);
        }

        HANDLE file;
        WIN32_FIND_DATAA fileData;
        file = ::FindFirstFileA((mstr(lpszSrcPath) + "\\*.*").c_str(), &fileData);
        ::FindNextFileA(file, &fileData);
        while (::FindNextFileA(file, &fileData))
        {
            // 如果是一个文件目录
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strFileName == "")
                {
                    ZipAddFolder(hz, fileData.cFileName);
                }
                else
                {
                    ZipAddFolder(hz, (strFileName + "\\" + fileData.cFileName).c_str());
                }

                strFileName = fileData.cFileName;
                // 存在子文件夹 递归调用
                DirToZip((mstr(lpszSrcPath) + "\\" + fileData.cFileName).c_str(), lpszZipName, hz, lpszDestPath, False);
                //strFileName.Empty();
                strFileName.clear();
            }
            else
            {
                mstr strTempPath = mstr(lpszSrcPath) + "\\" + fileData.cFileName;
                //strTempPath.Format(_T("%s\\%s"), (wstr)lpszSrcPath, fileData.cFileName);
                if (strFileName == "")
                {
                    ZipAdd(hz, fileData.cFileName, strTempPath.c_str());
                }
                else
                {
                    ZipAdd(hz, (strFileName + "\\" + fileData.cFileName).c_str(), strTempPath.c_str());
                }

                if (zResult != ZR_OK)
                {
                    return zResult;
                }
            }
        }
        strFileName.clear();
        return zResult;
    }

    ZRESULT CompressDirToZip(LPCSTR lpszSrcPath, LPCSTR lpszZipName, LPCSTR lpszDestPath)
    {
        HZIP hz;
        DWORD zResult = ZR_OK;
        zResult = DirToZip(lpszSrcPath, lpszZipName, hz, lpszDestPath, True);
        if (zResult == ZR_OK)
        {
            CloseZip(hz);
        }
        return zResult;
    }
}