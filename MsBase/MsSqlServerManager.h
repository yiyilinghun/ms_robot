#pragma once
#include <sql.h>

// ���Ԥ��SQL��䳤��
#define MAX_PREPARE_SQL_SENTENCE 1024

// ���Ԥ��SQL������
#define MAX_PREPARE_SQL_COUNT 1024

// Ԥ��SQL�������������
#define PREPARE_MAX_PARAMETER_COUNT 100

#ifndef SQL_TYPE_DEF
#define SQL_TYPE_DEF
// SQL���ݿ�����
enum SQL_TYPE
{
    MS_SQL_TYPE_UNKNOW = SQL_UNKNOWN_TYPE,          // δ֪����
    MS_SQL_TYPE_INT = SQL_INTEGER,                  // ����
    MS_SQL_TYPE_TCHAR = SQL_C_TCHAR,                // �ַ���
    MS_SQL_TYPE_FLOAT = SQL_REAL,                   // ������
    MS_SQL_TYPE_DATETIME = SQL_C_TYPE_TIMESTAMP,    // ����ʱ����
    MS_SQL_TYPE_MAX_COUNT,
    MS_SQL_TYPE_TO_DWORD = 0x7FFFFFFF,
};
#endif

// ���ļ�����SQL�ű���δ���
typedef enum LOAD_SQL_FROM_FILE_FLAG
{
    LSFFF_LOAD_AND_IMMEDIATELY_EXECUTE, // ����ִ��
    LSFFF_LOAD_AND_ADD_TO_PREPARE,      // ���뵽׼�������
    LSFFF_LOAD_ONLY,                    // ����ȡ����ִ��״̬
}LSFFF;

typedef enum SQL_SENTENCE_TYPE
{
    SSET_CREATE, // ����
    SSET_SELECT, // ��ѯ
    SSET_INSERT, // ����
    SSET_UPDATA, // ����
    SSET_DELETE, // ɾ��
}SSET;

// ��ѯ���������ϵ
typedef enum SQL_SELECT_WHERE_NEXUS
{
    SSWN_UNKNOW = 0,    // δ֪
    SSWN_EQUAL,         // ���
    SSWN_NOTEQUAL,      // ����
    SSWN_BIG,           // ����
    SSWN_SMALL,         // С��
    SSWN_BIG_EQUAL,     // ���ڵ���
    SSWN_SMALL_EQUAL,   // С�ڵ���
    SSWN_LIKE,          // LIKE
    SSWN_TO_DWORD = 0x7FFFFFFF,
}SSWN;

// SQL����������ִ�з��ؽ��
typedef enum SQL_RESULT
{
    SR_OK,              // ִ������
    SR_DB_NAME_ERROR,   // ���ݿ����ִ���
    SR_CODE_THROW,      // ִ���м�⵽�쳣
    SR_ERROR,
}SR;

// SQL������ʹ�õ�׼�����ṹ��
typedef struct SQL_PREPARE_SENTENCE
{
    inline VOID Release(VOID)
    {
        ZeroMemoryArray(m_szSQLPrepare);
        ZeroMemoryArray(m_SqlType);
        m_ParameterNumber = 0;
    }
    TCHAR           m_szSQLPrepare[MAX_PREPARE_SQL_SENTENCE];   // ׼���������
    SQL_TYPE        m_SqlType[MAX_PREPARE_SQL_SENTENCE];        // ׼������в������͵�����
    SQLUSMALLINT    m_ParameterNumber;                          // ׼�����ʹ�õĲ�������
}SPS;

// ���������Ϣ�ṹ��
typedef struct SQL_TABLE_ITEM_ATTRIBUTE
{
    TCHAR       m_szTableItemName[MAX_PATH];// ��������
    SQL_TYPE    m_SqlType;                  // ���ݿ�����
    BOOL        m_IsCanNull;                // �Ƿ�����Ϊ��
    BOOL        m_IsAutoAdd;                // �Ƿ�������
    TCHAR       m_strLen[RADIX_10];         // ���г������͵ĳ���
}STIA;

// �����������ʹ�ýṹ��
typedef struct STIA_STRUCT
{
    STIA*   m_pStia;                    // ���������ָ��
    INT     m_Count;                    // ��������
    TCHAR   m_szTableKeyName[MAX_PATH]; // ������������
}STIAC;

// ��ѯ��������
typedef enum SELECT_TYPE
{
    SELECT_TYPE_COUNT,      // ��ѯ����
    SELECT_TYPE_SUM,        // ��ѯ�ܺ�
    SELECT_TYPE_AVG,        // ��ѯƽ��ֵ
    SELECT_TYPE_SUM_TO_AVG, // ��ѯ��ֵ,�Լ���ȡƽ��ֵ
    SELECT_TYPE_ALL_ITEM,   // ��ѯ������
    SELECT_TYPE_ONE_ITEM,   // ��ѯָ��һ��
}ST;

// ��ѯ������Ϣ�ṹ��
class CAutoTableManager;
typedef struct SQL_SELECT_ITEM_ATTRIBUTE
{
    TCHAR*  m_szItemName;               // ��������
    TCHAR*  m_szSelectItemName;         // ��������
    TCHAR*  m_strTableName;             // ��ѯ������
    SSWN    m_WhereAttrNexus;           // ��ѯ��ϵ
    TCHAR*  m_szWhereAttrTeam;          // �����ѯ����
    CAutoTableManager*  m_pTable;       // ���ؽ�����ָ��
    ST      m_SelectType;               // ��ѯ����
}SSIA;

// ���������Ϣ�ṹ��
typedef struct SQL_INSERT_ITEM_ATTRIBUTE
{
    TCHAR       m_szItemName[MAX_PATH]; // ��������
    SQL_TYPE    m_SqlType;              // ��������
    TCHAR       m_szItemValue[MAX_PATH];// ����ֵ
}SIIA;

// ���������Ϣ����ʹ�ýṹ��
typedef struct SIIA_STRUCT
{
    SIIA*   m_pSiia;        // �����������ָ��
    INT     m_Count;        // ����������
}SIIAC;

class MsSqlServerManager
{
public: // ���졢����
    MsSqlServerManager(VOID);           // ���ݿ���������캯��
    virtual ~MsSqlServerManager(VOID);  // ���ݿ��������������

public: // �ӿ�
    BOOL Init(VOID);    // ��ʼ�����ݿ������
    BOOL Release(VOID); // �ͷ����ݿ������

    // �������ݿ�
    BOOL ConnectW(CONST WCHAR* szDBName, CONST WCHAR* szUserName, CONST WCHAR* szPassword);
    BOOL ConnectA(CONST CHAR* szDBName, CONST CHAR* szUserName, CONST CHAR* szPassword);
    BOOL Connect(CONST TCHAR* szDBName, CONST TCHAR* szUserName, CONST TCHAR* szPassword);

    // ִ��SQL���
    SR ExecuteSQLSentence(TCHAR* szSQLSentence, BOOL IsNeedResult = FALSE, CAutoTableManager* pTable = NULL);

    // ִ��Ԥ��SQL���
    SR ExecuteSQLPrepare(INT nIndex, TCHAR* szSQLParameter, ...);

    // ���ļ�����SQL���
    BOOL LoadSqlSentenceFromFile(TCHAR* szFileName, LSFFF nFlag = LSFFF_LOAD_AND_IMMEDIATELY_EXECUTE);

    //// ���Ԥ��SQL����Ƿ�ƥ��
    //BOOL CheckSQLPrepare(INT nIndex, TCHAR* szSQLParameter);

    // ���Ԥ��SQL���
    INT AddSQLPrepare(TCHAR* szSQLSentence);

    // ɾ��Ԥ��SQL���
    BOOL RemoveSQLPrepare(INT nIndex);

    // ����SQL���
    BOOL CreateSQLSentence(OUT TCHAR* szSQLSentence, SIZE_T MaxSQLLen, SSET sset, TCHAR* szTableName, LPVOID pData);

    // ��ȡ���ִ�н��
    FLOAT GetSQLSentenceResult(VOID);


    // ��¼�洢����
    BOOL SQLProcedures_Login(IN LPSTR xUsername, IN Int32 xDeIndex);

private:// ˽��

    inline VOID CheckFileSQL();

    // ����SQL���֮�������
    BOOL CreateSQLSentenceAtCreateTable(OUT TCHAR* szSQLSentence, SIZE_T MaxSQLLen, TCHAR* szTableName, STIAC* pStiac);

    // ����SQL���֮��ѯ��
    BOOL CreateSQLSentenceAtInsertItem(OUT TCHAR* szSQLSentence, SIZE_T MaxSQLLen, TCHAR* szTableName, SIIAC* pSiiac);

    // ����SQL���֮��ѯ��
    BOOL CreateSQLSentenceAtSelectItem(OUT TCHAR* szSQLSentence, SIZE_T MaxSQLLen, SSIA* pSsia);


    BOOL        m_IsInit;       // ��ǰ�����Ƿ��Ѿ���ʼ�����
    SQLHENV     m_hSqlHenv;     // SQL���ݿ⻷�����
    SQLHDBC     m_hDbc;         // SQL���ݿ����Ӿ��
    SQLHSTMT    m_hStmt;        // SQL���ݿ������
    BOOL        m_bConnectOK;   // ���ݿ��Ƿ�ɹ�����

    // SQL���ݿ�Ԥ�����
    SPS         m_SQLPrepare[MAX_PREPARE_SQL_COUNT];
    TCHAR       m_szSQLSentenceT[MAX_PREPARE_SQL_SENTENCE];
    SQLCHAR     m_szSQLSentenceA[MAX_PREPARE_SQL_SENTENCE];
    TCHAR       m_SQLMessageText[MAX_PREPARE_SQL_SENTENCE];
    INT         m_SQLPrepareCount;      // Ԥ��������
    FLOAT       m_fSQLSentenceResult;   // SQL���ִ�з���ֵ
};
