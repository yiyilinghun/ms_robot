#pragma once
#define MAX_SQL_STR_LNE DEFAULT_BUFF_SIZE   // ���SQL�ַ�������
#define MAX_SQL_BIN_LNE DEFAULT_BUFF_SIZE   // ���SQL������������

class MsBaseSql
{
public:
    MsBaseSql(LPCSTR szDBAddr, LPCSTR szDBUser, LPCSTR szDBPassword, LPCSTR szDBName);
    MsBaseSql(LPCSTR xConfigFileName);
    virtual ~MsBaseSql(void);
    virtual Boolean Connect() = 0;
    virtual void Disconnect() = 0;
    virtual Int64 GetInfluenceRowCount() = 0;   // ��ȡӰ������
    virtual Int64 GetInsertId() = 0;            // ��ȡ���������Id
    virtual mstr GetMysqlInfo() = 0;            // ��ȡMysql��Ϣ


    static void P2BS(const LPBASEPROTO xProto, mstr& xOut);
    static mstr P2BS(const LPBASEPROTO xProto);
    static void P2BS(const BaseProto& xProto, mstr& xOut);
    static mstr P2BS(const BaseProto& xProto);
    static void B2S(LPBYTE buf, OUT LPSTR szStr, Int32 len);

    mstr GetExistsTableSqlByProto(LPBASEPROTO xProto); // �����Ƿ����
    mstr GetCreateSqlByProto(LPBASEPROTO xProto);   // ����
    mstr GetDropSqlByProto(LPBASEPROTO xProto);     // ����
    mstr GetInsertSqlByProto(LPBASEPROTO xProto);   // ����

    mstr GetSelectSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // ��ѯ
    mstr GetUpdateSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // ����
    mstr GetDeleteSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // ɾ��

    void SetVarCharLength(Int32 xDefaultVarCharLength) { m_DefaultVarCharLength = xDefaultVarCharLength; }
    void SetVarByteLength(Int32 xDefaultVarByteLength) { m_DefaultVarByteLength = xDefaultVarByteLength; }
    void ClearUniqueField() { m_UniqueField.Clear(); }
    void AddUniqueField(mstr xUniqueFieldName) { m_UniqueField.Add(xUniqueFieldName); }
    void SetPrimaryField(mstr xPrimaryFieldName) { m_PrimaryFieldName = xPrimaryFieldName; }
    void SetTableCharset(mstr xCharset) { m_Charset = xCharset; }

public:
    mstr            m_ConfigFileName;
    mstr            m_DBAddr;
    mstr            m_DBUser;
    mstr            m_DBPassword;
    mstr            m_DBName;

    mstr            m_PrimaryFieldName;
    MsList<mstr>    m_UniqueField;
    Int32          m_DefaultVarCharLength;
    Int32          m_DefaultVarByteLength;
    mstr            m_Charset;
};

#include "MsMysql.h"
#include "MsAutoTableManager.h"
#include "MsSqlServerManager.h"
