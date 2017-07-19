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
    //   ��ѹzip�ļ���ָ��·��, �����ؽ�ѹ�ļ�·�����ļ�����
    // Parameters:
    //   lpszZipFullName   - ����ѹ zipѹ���������ļ���·����zipѹ������; ��"D://00//1.zip"��
    //   szFilePathArr     - ����Ľ�ѹ���ļ����ļ�������"1.jpg"��	
    //   lpszUnZipPath     - ��ѹ�������ļ� �����λ�õ�����·��; �� ��D://01��
    //                       �˲���ʡ��ʱ��Ĭ�Ͻ�ѹ��exe���������ļ����¡�
    // Returns:
    //   ��ѹ�ɹ�����ZR_OK����ѹʧ�ܷ��ش����롣
    // ------------------------------------------------------------------------------------------------------------------------
    ZRESULT ExtractZipToDir(LPCSTR lpszZipFullName, MsList<mstr>& szFilePathArr, LPCSTR lpszUnZipPath = nullptr);
    Boolean ExtractZipToMemory(LPCSTR lpszZipFullName, MsUnorderedDictionary<mstr, ZIP_RES_INFO>& xDictZipRes);
    //Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes);
    Boolean ExtractZipToMemoryByName(LPCSTR lpszZipFullName, LPCSTR szZipItemName, ZIP_RES_INFO& xZipRes, LPCSTR szPassword = nullptr);

    // ------------------------------------------------------------------------------------------------------------------------
    // Summary:
    //   ѹ��ָ��·���µ��ļ���������ѹ������ָ��·����
    // Parameters:
    //   lpszSrcPath        - ��ѹ���ļ����ڵ�·��; ��"D://00"��
    //   lpszDestPath       - ѹ����ɺ󣬴��ѹ������·����
    //                        �˲���ʡ��ʱ��Ĭ�ϴ��·��Ϊexe���������ļ���·����
    //   lpszZipName        - ѹ����ɺ�ѹ�������ƣ��硰MySkin.zip����
    // Returns:
    //   ѹ���ɹ�����ZR_OK��ѹ��ʧ�ܷ��ش����롣
    // ------------------------------------------------------------------------------------------------------------------------
    ZRESULT CompressDirToZip(LPCSTR lpszSrcPath, LPCSTR lpszZipName, LPCSTR lpszDestPath = nullptr);
}
