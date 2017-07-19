#pragma once
#include "_zip.h"
#include "_unzip.h"


struct ZIP_RES_INFO
{
    ZIP_RES_INFO() :m_DataAddr(nullptr), m_DataLen(0), m_IsAutoFree(True) { ; }
    ZIP_RES_INFO(Boolean IsAutoFree) :m_DataAddr(nullptr), m_DataLen(0), m_IsAutoFree(IsAutoFree) { ; }
    ~ZIP_RES_INFO() { m_IsAutoFree ? SAFE_DELETE_ARRAY(m_DataAddr) : nullptr; }
    LPBYTE  m_DataAddr;
    Int32  m_DataLen;
    Boolean m_IsAutoFree;
};

namespace ZipUtils
{
    // ------------------------------------------------------------------------------------------------------------------------
    // Summary:
    //   解压zip文件到指定路径, 并返回解压文件路径和文件名。
    // Parameters:
    //   lpszZipFullName   - 待解压 zip压缩包所在文件夹路径和zip压缩名称; 如"D://00//1.zip"。
    //   szFilePathArr     - 保存的解压后文件的文件名；如"1.jpg"。	
    //   lpszUnZipPath     - 解压出来的文件 所存放位置的完整路径; 如 “D://01”
    //                       此参数省略时，默认解压到exe程序所在文件夹下。
    // Returns:
    //   解压成功返回ZR_OK，解压失败返回错误码。
    // ------------------------------------------------------------------------------------------------------------------------
    ZRESULT ExtractZipToDir(LPCSTR lpszZipFullName, MsList<mstr>& szFilePathArr, LPCSTR lpszUnZipPath = nullptr);
    Boolean ExtractZipToMemory(LPCSTR lpszZipFullName, MsUnorderedDictionary<mstr, ZIP_RES_INFO>& xDictZipRes);
    //Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes);
    Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes, LPCSTR szPassword = nullptr);

    // ------------------------------------------------------------------------------------------------------------------------
    // Summary:
    //   压缩指定路径下的文件，并保存压缩包到指定路径。
    // Parameters:
    //   lpszSrcPath        - 待压缩文件所在的路径; 如"D://00"。
    //   lpszDestPath       - 压缩完成后，存放压缩包的路径。
    //                        此参数省略时，默认存放路径为exe程序所在文件的路径。
    //   lpszZipName        - 压缩完成后，压缩的名称；如“MySkin.zip”。
    // Returns:
    //   压缩成功返回ZR_OK，压缩失败返回错误码。
    // ------------------------------------------------------------------------------------------------------------------------
    ZRESULT CompressDirToZip(LPCSTR lpszSrcPath, LPCSTR lpszZipName, LPCSTR lpszDestPath = nullptr);
}
