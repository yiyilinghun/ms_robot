#include "Precompiled.h"

// ���ݿ���������캯��
MsSqlServerManager::MsSqlServerManager(VOID)
    : m_IsInit(FALSE) // ��ǰ���󴴽�ʱ��δ��ʼ��
    , m_hSqlHenv(NULL)
    , m_hDbc(NULL)
    , m_hStmt(NULL)
    , m_SQLPrepareCount(NULL)
    , m_bConnectOK(FALSE)
    , m_fSQLSentenceResult(-1.0f)
{
    ZeroMemoryArray(m_SQLPrepare);
    ZeroMemoryArray(m_szSQLSentenceT);
    ZeroMemoryArray(m_szSQLSentenceA);
    ZeroMemoryArray(m_SQLMessageText);
}

// ���ݿ��������������
MsSqlServerManager::~MsSqlServerManager(VOID)
{

}

// ��ʼ�����ݿ������
BOOL MsSqlServerManager::Init(VOID)
{
    INT nRet = SQL_SUCCESS;

    nRet = ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hSqlHenv);
    //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));

    nRet = ::SQLSetEnvAttr(m_hSqlHenv, SQL_ATTR_ODBC_VERSION, (LPVOID)SQL_OV_ODBC3, 0);
    //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLSetEnvAttrʧ��,����ʧ��"));

    nRet = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hSqlHenv, &m_hDbc);
    //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));

    nRet = ::SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (LPVOID)5, 0);
    //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLSetConnectAttrʧ��,����ʧ��"));

    m_IsInit = TRUE;
    // ���سɹ�
    return TRUE;
}

// �ͷ����ݿ������
BOOL MsSqlServerManager::Release(VOID)
{
    INT nRet = SQL_SUCCESS;


    if (m_hStmt)
    {
        nRet = ::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLFreeHandleʧ��,����ʧ��"));
        m_hStmt = NULL;
    }
    if (m_bConnectOK)
    {
        nRet = ::SQLDisconnect(m_hDbc);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLDisconnectʧ��,����ʧ��"));
        m_bConnectOK = FALSE;
    }
    if (m_hDbc)
    {
        nRet = ::SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLFreeHandleʧ��,����ʧ��"));
        m_hDbc = NULL;
    }
    if (m_hSqlHenv)
    {
        nRet = ::SQLFreeHandle(SQL_HANDLE_ENV, m_hSqlHenv);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLFreeHandleʧ��,����ʧ��"));
        m_hSqlHenv = NULL;
    }

    m_IsInit = FALSE;
    // ���سɹ�
    return TRUE;
}

// �������ݿ�
BOOL MsSqlServerManager::ConnectW(CONST WCHAR* szDBName, CONST WCHAR* szUserName, CONST WCHAR* szPassword)
{

    INT nRet = SQL_SUCCESS;

    if (szDBName && szUserName && szPassword)
    {
        nRet = ::SQLConnectW(m_hDbc, (SQLWCHAR*)szDBName, (SQLSMALLINT)wcslen(szDBName), (SQLWCHAR*)szUserName, (SQLSMALLINT)wcslen(szUserName), (SQLWCHAR*)szPassword, (SQLSMALLINT)wcslen(szPassword));
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLConnectʧ��,����ʧ��"));
        m_bConnectOK = TRUE;

        nRet = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));

        // ���سɹ�
        return TRUE;
    }
    else
    {
        //AssertEx(AL_NORMAL, szDBName, _T("szDBNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szUserName, _T("szUserNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szPassword, _T("szPasswordΪ��,����ʧ��"));
    }


    return FALSE;
}

// �������ݿ�
BOOL MsSqlServerManager::ConnectA(CONST CHAR* szDBName, CONST CHAR* szUserName, CONST CHAR* szPassword)
{

    INT nRet = SQL_SUCCESS;

    if (szDBName && szUserName && szPassword)
    {
        nRet = ::SQLConnectA(m_hDbc, (SQLCHAR*)szDBName, (SQLSMALLINT)strlen(szDBName), (SQLCHAR*)szUserName, (SQLSMALLINT)strlen(szUserName), (SQLCHAR*)szPassword, (SQLSMALLINT)strlen(szPassword));
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLConnectʧ��,����ʧ��"));
        m_bConnectOK = TRUE;

        nRet = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));

        // ���سɹ�
        return TRUE;
    }
    else
    {
        //AssertEx(AL_NORMAL, szDBName, _T("szDBNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szUserName, _T("szUserNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szPassword, _T("szPasswordΪ��,����ʧ��"));
    }


    return FALSE;
}

// �������ݿ�
BOOL MsSqlServerManager::Connect(CONST TCHAR* szDBName, CONST TCHAR* szUserName, CONST TCHAR* szPassword)
{

    INT nRet = SQL_SUCCESS;

    if (szDBName && szUserName && szPassword)
    {
        nRet = ::SQLConnect(m_hDbc, (SQLTCHAR*)szDBName, (SQLSMALLINT)_tcslen(szDBName), (SQLTCHAR*)szUserName, (SQLSMALLINT)_tcslen(szUserName), (SQLTCHAR*)szPassword, (SQLSMALLINT)_tcslen(szPassword));
        Assert(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), "SQLConnectʧ��,����ʧ��");
        m_bConnectOK = TRUE;

        nRet = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
        Assert(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), "SQLAllocHandleʧ��,����ʧ��");

        // ���سɹ� 
        return TRUE;
    }
    else
    {
        //AssertEx(AL_NORMAL, szDBName, _T("szDBNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szUserName, _T("szUserNameΪ��,����ʧ��"));
        //AssertEx(AL_NORMAL, szPassword, _T("szPasswordΪ��,����ʧ��"));
    }


    return FALSE;
}

// ִ��SQL���
SR MsSqlServerManager::ExecuteSQLSentence(TCHAR* szSQLSentence, BOOL IsNeedResult, CAutoTableManager* pTable)
{
    INT nRet = SQL_SUCCESS;

    if (!szSQLSentence)
    {
        szSQLSentence = m_szSQLSentenceT;
    }
    if (pTable)
    {
        pTable->Release();
        pTable->Init();
    }

    nRet = ::SQLExecDirect(m_hStmt, (SQLTCHAR*)szSQLSentence, (SQLSMALLINT)_tcslen(szSQLSentence) + 1);
    if (nRet == SQL_SUCCESS)
    {
        if ((IsNeedResult && !pTable) || (m_fSQLSentenceResult == 0.0f))
        {
            SQLLEN Size = sizeof(m_fSQLSentenceResult);
            if (::SQLFetch(m_hStmt) == SQL_SUCCESS)
            {
                ::SQLGetData(
                    m_hStmt,
                    1,
                    MS_SQL_TYPE_FLOAT,
                    &m_fSQLSentenceResult,
                    Size,
                    &Size
                );
            }
        }
        else if (IsNeedResult)
        {
            //AssertEx(AL_NORMAL, pTable, _T("pTableΪ��,���󴫲�"));
            UINT    uTableItemCount;
            UINT    uTableLineCount;
            SQLLEN  nSizeOfItem;
            SQL_TYPE SqlType;
            LPVOID lpData = NULL;
            pTable->GetTableSize(uTableItemCount, uTableLineCount);
            for (UINT j = 0; j < uTableLineCount; j++)
            {
                if (::SQLFetch(m_hStmt) != SQL_SUCCESS)
                {
                    break;
                }
                for (UINT i = 0; i < uTableItemCount; i++)
                {
                    SqlType = pTable->GetTableItemType(i);
                    lpData = pTable->GetTableItemInLine(i, j);
                    nSizeOfItem = pTable->GetTableItemSize(i, j, TRUE);
                    ::SQLGetData(
                        m_hStmt,
                        SQLUSMALLINT(i + 1),
                        (SQLSMALLINT)SqlType,
                        lpData,
                        nSizeOfItem,
                        &nSizeOfItem
                    );
                    pTable->SetValidLine(j + 1);
                }
            }
        }
        nRet = ::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLFreeHandleʧ��,����ʧ��"));
        nRet = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
        //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));
        return SR_OK;
    }

    else
    {
        SQLTCHAR SQLState[MAX_PATH] = { 0 }; SQLINTEGER NativeErrorPtr;
        nRet = ::SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, 1, SQLState, &NativeErrorPtr, (SQLTCHAR*)m_SQLMessageText, 1024, NULL);
        if ((nRet != SQL_SUCCESS) && (nRet != SQL_SUCCESS_WITH_INFO))
        {
            ::MessageBox(NULL, _T("��ȡSQL״̬����"), _T("��������"), MB_OK);
        }
        else
        {
            nRet = ::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
            //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLFreeHandleʧ��,����ʧ��"));
            nRet = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
            //AssertEx(AL_NORMAL, (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO), _T("SQLAllocHandleʧ��,����ʧ��"));
            if (_tcsstr(m_SQLMessageText, _T("�����ڡ���ȷ����ȷ�������˸����ơ�")))
            {
                return SR_DB_NAME_ERROR;
            }
            else if (_tcsstr(m_SQLMessageText, _T("�ѽ����ݿ������ĸ���Ϊ")))
            {
                return SR_OK;
            }
            {
                return SR_ERROR;
            }
        }
    }
    return SR_CODE_THROW;
}

// ִ��׼���õ�SQL���
SR MsSqlServerManager::ExecuteSQLPrepare(INT nIndex, TCHAR* szSQLParameter, ...)
{
    va_list vaList;
    va_start(vaList, szSQLParameter);


    //if (!this->CheckSQLPrepare(nIndex, szSQLParameter))
    //{
    //    AssertEx(AL_NORMAL, FALSE, _T("����SQL�������Ƿ�,�����߼�"));
    //}
    ZeroMemoryArray(m_szSQLSentenceT);

    TCHAR* strFindNextParameter = m_SQLPrepare[nIndex].m_szSQLPrepare;
    TCHAR* strSQLSentence = m_szSQLSentenceT;
    while (*strFindNextParameter)
    {
        if (_T('%') == *strFindNextParameter)
        {
            *strSQLSentence++ = *strFindNextParameter++;
            switch (*strFindNextParameter)
            {
                case _T('d'):
                {
                    *(--strSQLSentence) = _T('\0');
                    _itot_s(va_arg(vaList, INT), strSQLSentence, MAX_PREPARE_SQL_SENTENCE, RADIX_10);
                    strSQLSentence += _tcslen(strSQLSentence);
                    strFindNextParameter++;
                }break;
                case _T('s'):
                {
                    *(--strSQLSentence) = _T('\0');
                    _tcscat_s(strSQLSentence, MAX_PREPARE_SQL_SENTENCE, va_arg(vaList, TCHAR*));
                    strSQLSentence += _tcslen(strSQLSentence);
                    strFindNextParameter++;
                }break;
                case _T('f'):
                {
                    *(--strSQLSentence) = _T('\0');
                    _stprintf_s(strSQLSentence, MAX_PREPARE_SQL_SENTENCE, _T("%06f"), va_arg(vaList, FLOAT));
                    strSQLSentence += _tcslen(strSQLSentence);
                    strFindNextParameter++;
                }break;
                case _T('D'):
                {
                    *(--strSQLSentence) = _T('\0');
                    SQL_DATE_STRUCT date = va_arg(vaList, SQL_DATE_STRUCT);
                    _stprintf_s(strSQLSentence, MAX_PREPARE_SQL_SENTENCE, _T("%d-%d-%d"), date.year, date.month, date.day);
                    strSQLSentence += _tcslen(strSQLSentence);
                    strFindNextParameter++;
                }break;
                case _T('T'):
                {
                    *(--strSQLSentence) = _T('\0');
                    SQL_TIME_STRUCT time = va_arg(vaList, SQL_TIME_STRUCT);
                    _stprintf_s(strSQLSentence, MAX_PREPARE_SQL_SENTENCE, _T("%d-%d-%d"), time.hour, time.minute, time.second);
                    strSQLSentence += _tcslen(strSQLSentence);
                    strFindNextParameter++;
                }break;
                default:
                {
                    *strSQLSentence++ = *strFindNextParameter++;
                    continue;
                }break;
            }
        }
        else
        {
            *strSQLSentence++ = *strFindNextParameter++;
            continue;
        }
    }
    this->ExecuteSQLSentence(m_szSQLSentenceT);

    va_end(vaList);
    return SR_OK;
}

//// ���Ԥ��SQL����Ƿ�ƥ��
//BOOL MsSqlServerManager::CheckSQLPrepare(INT nIndex, TCHAR* szSQLParameter)
//{
//    
//
//    TCHAR* pSrc = m_SQLPrepare[nIndex].m_szSQLPrepare;
//    TCHAR* pDes = szSQLParameter;
//
//    for (;;)
//    {
//        TCHAR* pSrcTemp = _tcschr(pSrc, _T('%'));
//        TCHAR* pDesTemp = _tcschr(pDes, _T('%'));
//        if (!pSrcTemp && !pDesTemp)
//        {
//            return TRUE;
//        }
//        if (pSrcTemp && pDesTemp)
//        {
//            switch (*pSrcTemp)
//            {
//            case _T('d'):
//            case _T('s'):
//            case _T('f'):
//            case _T('D'):
//            case _T('T'):
//                {
//                    return FALSE;
//                }break;
//            default:
//                {
//                    return TRUE;
//                }break;
//            }
//        }
//        pSrcTemp++;pDesTemp++;
//        if (*pSrcTemp != *pDesTemp)
//        {
//            switch (*pSrcTemp)
//            {
//            case _T('d'):
//            case _T('s'):
//            case _T('f'):
//            case _T('D'):
//            case _T('T'):
//                {
//                    return FALSE;
//                }break;
//            default:
//                {
//                    continue;
//                }break;
//            }
//        }
//        pSrc = pSrcTemp; pDes = pDesTemp;
//    }
//
//    
//    return FALSE;
//}

// ���׼���õ�SQL���
INT MsSqlServerManager::AddSQLPrepare(TCHAR* szSQLSentence)
{

    //AssertEx(AL_NORMAL, szSQLSentence, _T("szSQLSentenceΪ��,���󴫲�"));

    //AssertEx(AL_NORMAL, m_SQLPrepareCount < MAX_PREPARE_SQL_COUNT, _T("�ѳ���SQL������ֵ,�����߼�"));

    TCHAR* strFindNextParameter = szSQLSentence;

    while (strFindNextParameter && *strFindNextParameter)
    {
        if (_T('%') == *strFindNextParameter)
        {
            strFindNextParameter++;
            switch (*strFindNextParameter)
            {
                case _T('d'):
                {
                    m_SQLPrepare[m_SQLPrepareCount].m_SqlType[m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber] = MS_SQL_TYPE_INT;
                    m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber++;
                }break;
                case _T('s'):
                {
                    m_SQLPrepare[m_SQLPrepareCount].m_SqlType[m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber] = MS_SQL_TYPE_TCHAR;
                    m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber++;
                }break;
                case _T('f'):
                {
                    m_SQLPrepare[m_SQLPrepareCount].m_SqlType[m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber] = MS_SQL_TYPE_FLOAT;
                    m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber++;
                }break;
                case _T('D'):
                {
                    m_SQLPrepare[m_SQLPrepareCount].m_SqlType[m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber] = MS_SQL_TYPE_DATETIME;
                    m_SQLPrepare[m_SQLPrepareCount].m_ParameterNumber++;
                }break;
                default:
                {
                    strFindNextParameter++;
                }break;
            }
        }
        else
        {
            strFindNextParameter++;
        }
    }
    if (szSQLSentence && m_SQLPrepareCount < MAX_PREPARE_SQL_COUNT)
    {
        _tcscpy_s(m_SQLPrepare[m_SQLPrepareCount].m_szSQLPrepare, MAX_PREPARE_SQL_SENTENCE, szSQLSentence);
        m_SQLPrepareCount++;
        return m_SQLPrepareCount - 1;
    }


    return INVALID_NID;
}

// ɾ��׼���õ�SQL���
BOOL MsSqlServerManager::RemoveSQLPrepare(INT nIndex)
{

    //AssertEx(AL_NORMAL, nIndex < m_SQLPrepareCount, _T("Ҫɾ����SQL��䲢������,�����߼�"));

    if (m_SQLPrepareCount < MAX_PREPARE_SQL_COUNT)
    {
        if (nIndex != m_SQLPrepareCount)
        {
            memcpy(&m_SQLPrepare[m_SQLPrepareCount], &m_SQLPrepare[nIndex], sizeof(m_SQLPrepare[m_SQLPrepareCount]));
        }
        m_SQLPrepare[m_SQLPrepareCount].Release();
        m_SQLPrepareCount--;
        return TRUE;
    }

    return FALSE;
}

BOOL MsSqlServerManager::LoadSqlSentenceFromFile(TCHAR* szFileName, LSFFF nFlag)
{


    if (!szFileName)
    {
        return FALSE;
    }

    BOOL bRet = TRUE;
    //AssertEx(AL_NORMAL, szFileName, _T("�����ļ���Ϊ��,�������"));
    HANDLE hSqlFile = ::CreateFile(
        szFileName,
        FILE_READ_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    //AssertEx(AL_NORMAL, hSqlFile != INVALID_HANDLE, _T("���ļ�ʧ��,����ʧ��"));

    DWORD SqlFileLen = ::GetFileSize(hSqlFile, NULL);
    //AssertEx(AL_NORMAL, SqlFileLen, _T("�ļ�Ϊ��,����ʧ��"));
    //AssertEx(AL_NORMAL, SqlFileLen < MAX_PREPARE_SQL_SENTENCE, _T("SQL�ļ�����,�����߼�"));

    ZeroMemoryArray(m_szSQLSentenceA);
    ZeroMemoryArray(m_szSQLSentenceT);

#ifdef UNICODE
    bRet = ::ReadFile(
        hSqlFile,
        m_szSQLSentenceA,
        SqlFileLen,
        &SqlFileLen,
        NULL);
    //AssertEx(AL_NORMAL, bRet, _T("��ȡ�ļ�����ʧ��,����ʧ��"));
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)m_szSQLSentenceA, MAX_PREPARE_SQL_SENTENCE, m_szSQLSentenceT, MAX_PREPARE_SQL_SENTENCE);
#else
    bRet = ::ReadFile(
        hSqlFile,
        m_szSQLSentenceT,
        SqlFileLen,
        &SqlFileLen,
        NULL);
    AssertEx(AL_NORMAL, bRet, _T("��ȡ�ļ�����ʧ��,����ʧ��"));
#endif
    this->CheckFileSQL();
    if (nFlag == LSFFF_LOAD_AND_IMMEDIATELY_EXECUTE)
    {
        return this->ExecuteSQLSentence(m_szSQLSentenceT);
    }
    else if (nFlag == LSFFF_LOAD_AND_ADD_TO_PREPARE)
    {
        return this->AddSQLPrepare(m_szSQLSentenceT);
    }

    ::CloseHandle(hSqlFile);
    return TRUE;
}

inline VOID MsSqlServerManager::CheckFileSQL()
{
    TCHAR* pFindChr = m_szSQLSentenceT;
    TCHAR* pFindChrBak = pFindChr;
    for (;;)
    {
        pFindChr = _tcschr(pFindChr, _T('G'));
        if (!pFindChr)
        {
            pFindChr = pFindChrBak;
            pFindChr = _tcschr(pFindChr, _T('g'));
            if (!pFindChr)
            {
                break;
            }
        }
        // �����һ���ַ�Ϊ��ĸ'O'
        if ((_T('O') == (*(pFindChr + 1))) || (_T('o') == (*(pFindChr + 1))))
        {
            if (0x0d == (*(pFindChr + 2)))
            {
                *pFindChr = _T(' ');
                *(pFindChr + 1) = _T(' ');
                pFindChr += 2;
                continue;
            }
            else if (0x00 == (*(pFindChr + 2)))
            {
                *pFindChr = _T(' ');
                *(pFindChr + 1) = _T('\0');
                break;
            }
        }
        pFindChr++;
        pFindChrBak = pFindChr;
    }
}

// ����SQL���
BOOL MsSqlServerManager::CreateSQLSentence(OUT TCHAR* szSQLSentence, SIZE_T MaxSQLLen, SSET sset, TCHAR* szTableName, LPVOID pData)
{


    if (!szSQLSentence)
    {
        szSQLSentence = m_szSQLSentenceT;
        MaxSQLLen = MAX_PREPARE_SQL_SENTENCE;
    }
    ZeroMemory(szSQLSentence, (sizeof(TCHAR) * MaxSQLLen));

    switch (sset)
    {
        case SSET_CREATE:
            return this->CreateSQLSentenceAtCreateTable(szSQLSentence, MaxSQLLen, szTableName, (STIAC*)pData);
            break;
        case SSET_SELECT:
            return this->CreateSQLSentenceAtSelectItem(szSQLSentence, MaxSQLLen, (SSIA*)pData);
            break;
        case SSET_INSERT:
            return this->CreateSQLSentenceAtInsertItem(szSQLSentence, MaxSQLLen, szTableName, (SIIAC*)pData);
            break;
    }

    return TRUE;
}

// ����SQL���֮�������
BOOL MsSqlServerManager::CreateSQLSentenceAtCreateTable(OUT TCHAR* szSQLSentence, SIZE_T sMaxSQLLen, TCHAR* szTableName, STIAC* pStiac)
{


    _sntprintf_s(
        szSQLSentence, sMaxSQLLen, _TRUNCATE,
        //_T("USE [%s]\n ")
        _T("CREATE TABLE [dbo].[%s]\n")
        _T("("),
        /*szDBName, */szTableName
    );

    STIA* pStia = pStiac->m_pStia;
    TCHAR* strSqlType = NULL;       // ����SQL��������
    TCHAR* strSqlCharLen = NULL;    // ����SQL���ַ����
    TCHAR* strCanNull = NULL;       // ����SQL�����Ƿ�����Ϊ��
    SIZE_T TempSize = 0;
    for (INT i = 0; i < pStiac->m_Count; i++, pStia++)
    {
        TempSize = _tcslen(szSQLSentence);
        szSQLSentence += TempSize;
        sMaxSQLLen -= TempSize;

        switch (pStia->m_SqlType)
        {
            case MS_SQL_TYPE_INT:
                if (pStia->m_IsAutoAdd)
                {
                    strSqlType = _T("[int] IDENTITY(1,1)");
                }
                else
                {
                    strSqlType = _T("[int]");
                }
                strSqlCharLen = _T("");
                strCanNull = pStia->m_IsCanNull ? _T("") : _T("NOT ");
                break;
            case MS_SQL_TYPE_TCHAR:
                strSqlType = _T("[nvarchar]");
                strSqlCharLen = pStia->m_strLen;
                strCanNull = pStia->m_IsCanNull ? _T("") : _T("NOT ");
                break;
            case MS_SQL_TYPE_FLOAT:
                strSqlType = _T("[float]");
                strSqlCharLen = _T("");
                strCanNull = pStia->m_IsCanNull ? _T("") : _T("NOT ");
                break;
            case MS_SQL_TYPE_DATETIME:
                strSqlType = _T("[datetime]");
                strSqlCharLen = _T("");
                strCanNull = pStia->m_IsCanNull ? _T("") : _T("NOT ");
                break;
            default:
            {
                continue;
            }
        }
        _sntprintf_s(
            szSQLSentence, sMaxSQLLen, _TRUNCATE,
            _T("[%s]%s %s %s NULL,\0"),
            pStia->m_szTableItemName,
            strSqlType,
            strSqlCharLen,
            strCanNull
        );
    }
    szSQLSentence[sMaxSQLLen - 1] = 0;

    TempSize = _tcsnlen(szSQLSentence, sMaxSQLLen);
    szSQLSentence += TempSize;
    sMaxSQLLen -= TempSize;
    _sntprintf_s(
        szSQLSentence, sMaxSQLLen, _TRUNCATE,
        _T("CONSTRAINT [PK_%s] PRIMARY KEY CLUSTERED")
        _T("([%s] ASC)")
        _T("WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]")
        _T(") ON [PRIMARY]"),
        szTableName,
        pStiac->m_szTableKeyName
    );

    return TRUE;
}

// ����SQL���֮��ѯ��
BOOL MsSqlServerManager::CreateSQLSentenceAtInsertItem(OUT TCHAR* szSQLSentence, SIZE_T sMaxSQLLen, TCHAR* szTableName, SIIAC* pSiiac)
{
    _sntprintf_s(
        szSQLSentence, sMaxSQLLen, _TRUNCATE,
        //_T("USE [%s]\n ")
        _T("INSERT INTO [%s]\n")
        _T("("),
        /*szDBName, */szTableName
    );

    SIIA*  pSiia = pSiiac->m_pSiia;
    SIZE_T TempSize = 0;

    for (INT i = 0; i < pSiiac->m_Count; i++, pSiia++)
    {
        TempSize = _tcslen(szSQLSentence);
        szSQLSentence += TempSize;
        sMaxSQLLen -= TempSize;
        _sntprintf_s(
            szSQLSentence, sMaxSQLLen, _TRUNCATE,
            _T("[%s],"),
            pSiia->m_szItemName
        );
    }

    szSQLSentence[sMaxSQLLen - 1] = 0;

    TempSize = _tcsnlen(szSQLSentence, sMaxSQLLen);
    szSQLSentence += TempSize;
    sMaxSQLLen -= TempSize;
    *(--szSQLSentence) = _T(')');
    *(++szSQLSentence) = _T('\n');
    _tcscpy_s(szSQLSentence, sMaxSQLLen, _T("VALUES \n("));

    pSiia = pSiiac->m_pSiia;
    for (INT i = 0; i < pSiiac->m_Count; i++, pSiia++)
    {
        TempSize = _tcslen(szSQLSentence);
        szSQLSentence += TempSize;
        sMaxSQLLen -= TempSize;
        switch (pSiia->m_SqlType)
        {
            case MS_SQL_TYPE_INT:
            {
                _sntprintf_s(
                    szSQLSentence, sMaxSQLLen, _TRUNCATE,
                    _T("%s,"),
                    pSiia->m_szItemValue
                );
            }break;
            case MS_SQL_TYPE_TCHAR:
            {
                _sntprintf_s(
                    szSQLSentence, sMaxSQLLen, _TRUNCATE,
                    _T("N'%s',"),
                    pSiia->m_szItemValue
                );
            }break;
            case MS_SQL_TYPE_FLOAT:
            {
                _sntprintf_s(
                    szSQLSentence, sMaxSQLLen, _TRUNCATE,
                    _T("%s,"),
                    pSiia->m_szItemValue
                );
            }break;
            case MS_SQL_TYPE_DATETIME:
            {
                _sntprintf_s(
                    szSQLSentence, sMaxSQLLen, _TRUNCATE,
                    _T("CONVERT(DATETIME, '%s', 102),"),
                    pSiia->m_szItemValue
                );
            }break;
        }
    }
    TempSize = _tcslen(szSQLSentence);
    szSQLSentence += TempSize;
    sMaxSQLLen -= TempSize;
    *(--szSQLSentence) = _T(')');
    *(++szSQLSentence) = _T('\n');
    return TRUE;
}

// ����SQL���֮��ѯ��
BOOL MsSqlServerManager::CreateSQLSentenceAtSelectItem(OUT TCHAR* szSQLSentence, SIZE_T sMaxSQLLen, SSIA* pSsia)
{


    // ����SELECT
    SIZE_T TempSize = 0;
    switch (pSsia->m_SelectType)
    {
        case SELECT_TYPE_COUNT:
        {
            _sntprintf_s(
                szSQLSentence, sMaxSQLLen, _TRUNCATE,
                _T("SELECT COUNT(%s) \n"),
                pSsia->m_szSelectItemName
            );
            m_fSQLSentenceResult = 0.0f;
        }break;
        case SELECT_TYPE_SUM:
        {
            _sntprintf_s(
                szSQLSentence, sMaxSQLLen, _TRUNCATE,
                _T("SELECT SUM(%s) \n"),
                pSsia->m_szSelectItemName
            );
            m_fSQLSentenceResult = 0.0f;
        }break;
        case SELECT_TYPE_AVG:
        {
            _sntprintf_s(
                szSQLSentence, sMaxSQLLen, _TRUNCATE,
                _T("SELECT AVG(%s) \n"),
                pSsia->m_szSelectItemName
            );
            m_fSQLSentenceResult = 0.0f;
        }break;
        case SELECT_TYPE_ALL_ITEM:
        {
            _tcscpy_s(szSQLSentence, sMaxSQLLen, _T("SELECT * \n"));
        }break;
        case SELECT_TYPE_ONE_ITEM:
        {
            _sntprintf_s(
                szSQLSentence, sMaxSQLLen, _TRUNCATE,
                _T("SELECT [%s] \n"),
                pSsia->m_szItemName
            );
        }break;
        default:
        {
            //AssertEx(AL_NORMAL, FALSE, _T("���ʹ���,���󴫲�"));
        }
    }

    // ����FROM
    TempSize = _tcsnlen(szSQLSentence, sMaxSQLLen);
    szSQLSentence += TempSize;
    sMaxSQLLen -= TempSize;
    //AssertEx(AL_NORMAL, pSsia->m_strTableName, _T("pSsia->m_strTableNameΪ��,���󴫲�"));
    _sntprintf_s(
        szSQLSentence, sMaxSQLLen, _TRUNCATE,
        _T("FROM [%s]\n"),
        pSsia->m_strTableName
    );


    if (pSsia->m_szWhereAttrTeam && _tcslen(pSsia->m_szWhereAttrTeam))
    {
        // ����WHERE
        TempSize = _tcslen(szSQLSentence);
        szSQLSentence += TempSize;
        sMaxSQLLen -= TempSize;
        _tcscpy_s(szSQLSentence, sMaxSQLLen, _T("WHERE ("));

        TempSize = 0;

        if (pSsia->m_szWhereAttrTeam)
        {
            TempSize = _tcslen(szSQLSentence);
            szSQLSentence += TempSize;
            sMaxSQLLen -= TempSize;

            switch (pSsia->m_WhereAttrNexus)
            {
                case SSWN_UNKNOW:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("%s)"),
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_EQUAL:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] = N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_NOTEQUAL:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] != N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_BIG:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] > N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_SMALL:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] < N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_BIG_EQUAL:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] >= N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_SMALL_EQUAL:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] <= N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
                case SSWN_LIKE:
                {
                    _sntprintf_s(
                        szSQLSentence, sMaxSQLLen, _TRUNCATE,
                        _T("[%s] LIKE N'%s' )"),
                        pSsia->m_szItemName,
                        pSsia->m_szWhereAttrTeam
                    );
                }break;
            }
        }
    }
    return TRUE;
}

// ��ȡ���ִ�н��
FLOAT MsSqlServerManager::GetSQLSentenceResult(VOID)
{
    FLOAT fCount = m_fSQLSentenceResult;
    m_fSQLSentenceResult = -1.0f;
    return fCount;
}

BOOL MsSqlServerManager::SQLProcedures_Login(IN LPSTR xUsername, IN Int32 xDeIndex)
{
    INT nRet = SQL_SUCCESS;

    SQLLEN cbEmployeeID = SQL_NTS;
    SQLLEN cbCustID = 0;
    nRet = ::SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(xUsername), 0, xUsername, 0, &cbEmployeeID);
    nRet = ::SQLBindParameter(m_hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &xDeIndex, 0, &cbCustID);

    nRet = ::SQLExecDirect(m_hStmt, _T("exec tf_Login ?, ?"), (SQLSMALLINT)_tcslen(_T("exec tf_Login ?, ?")));

    //nRet = ::SQLProcedures(m_hStmt, L"", 0, L"", 0, _T("tf_Login"), (SQLSMALLINT)_tcslen(_T("tf_Login")));
    if (nRet == SQL_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

