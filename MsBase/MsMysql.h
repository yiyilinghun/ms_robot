#pragma once
#include "Mysql/mysql.h"

#define MAX_SQL_STR_LNE DEFAULT_BUFF_SIZE   // 最大SQL字符串长度
#define MAX_SQL_BIN_LNE DEFAULT_BUFF_SIZE   // 最大SQL二进制流长度

class MysqlAutoRes
{
public:
    MysqlAutoRes();
    MysqlAutoRes(MYSQL_RES* xRes);
    MysqlAutoRes(LPVOID xRes);
    MysqlAutoRes::~MysqlAutoRes();
    void operator =(MYSQL_RES* xRes);
    void operator =(LPVOID xRes);
    operator MYSQL_RES*();
    operator bool();
    Boolean FillResult(LPBASEPROTO xResult);
    DWORD GetResultColumnNum();
    MYSQL_ROW GetResultRowData();
    void Reset();
    MYSQL_RES* m_Res;
};

class MysqlAutoRow
{
public:
    MysqlAutoRow();
    MysqlAutoRow(MYSQL_ROW xRow);
    MysqlAutoRow::~MysqlAutoRow();
    void operator=(MYSQL_ROW xRow);
    bool operator!();
    operator bool();
    operator MYSQL_ROW();
    operator MYSQL_ROW*();
    Boolean IsNull(DWORD xColumnIndex);
    Single  GetSingle(DWORD xColumnIndex);
    Double  GetDouble(DWORD xColumnIndex);
    Int64   GetInt64(DWORD xColumnIndex);
    UInt64  GetUInt64(DWORD xColumnIndex);
    Int32   GetInt32(DWORD xColumnIndex);
    UInt32  GetUInt32(DWORD xColumnIndex);
    Boolean GetBoolean(DWORD xColumnIndex);
    mstr    GetString(DWORD xColumnIndex);
    LPBYTE  GetBytes(MYSQL_RES* xMySqlRes, DWORD xColumnIndex, OUT DWORD& dwSize);

    MYSQL_ROW m_Row;
};

class MsMysql
{
public:
    MsMysql(LPCSTR szDBAddr, LPCSTR szDBUser, LPCSTR szDBPassword, LPCSTR szDBName);
    MsMysql(LPCSTR xConfigFileName);
    ~MsMysql(void);
    Boolean Connect();
    void Disconnect();
    Boolean ExeSQL(mstr xSql);
    Boolean ExeSQLFormat(LPCSTR xSqlFormat, ...);
    MYSQL_ROW NextRow() { return m_MysqlRes.GetResultRowData(); };

    Int64 GetInfluenceRowCount();                   // 获取影响行数
    Int64 GetInsertId();                            // 获取插入后自增Id
    mstr GetMysqlInfo();                            // 获取Mysql信息
    Boolean CheckTableExist(mstr xTableName);               // 检查指定表是否存在

    Int64 CheckKey(universal_data* xUniversalData, MsIOCPManager* xMsIOCPManager);
    Boolean GetUniversalDataKeys(Int64 xMainKey, MsSet<Int64>& xSetKeys);
    void RemoveUniversalDatas(Int64 xMainKey, Boolean IsMain = False);
    void RemoveUniversalDatas(Int64 xMainKey, MsSet<Int64>& xSetKeys, Boolean IsMain = False);

    Boolean UpdateUniversalData(universal_data* xUniversalData, MsIOCPManager* xMsIOCPManager) { return UpdateUniversalData(*xUniversalData, xMsIOCPManager); }
    Boolean LoadUniversalData(Int64 xKey, universal_data* xUniversalData, Boolean IsMain = True) { return LoadUniversalData(xKey, *xUniversalData, IsMain); }
    Boolean SaveUniversalData(universal_data* xUniversalData, Boolean canUpdate, MsIOCPManager* xMsIOCPManager) { return SaveUniversalData(*xUniversalData, canUpdate, xMsIOCPManager); }

    Boolean UpdateUniversalData(const universal_data& xUniversalData, MsIOCPManager* xMsIOCPManager);
    Boolean LoadUniversalData(Int64 xKey, universal_data& xUniversalData, Boolean IsMain = True);
    Boolean SaveUniversalData(const universal_data& xUniversalData, Boolean canUpdate, MsIOCPManager* xMsIOCPManager);

    //Boolean EqualUniversalData(universal_data& xUniversalData, Boolean IsMain = True);

    void MakeSubValueSql(const universal_data& xUniversalData, Int32 xIndex, mstr& xValueSql, MsIOCPManager* xMsIOCPManager);
    void MakeValueSql(const universal_data& xUniversalData, MsList<mstr>& xListValueSql, MsIOCPManager* xMsIOCPManager);
    Boolean SaveValue(const universal_data& xUniversalData, MsIOCPManager* xMsIOCPManager);

    Boolean static CheckSqlKeyword(mstr xSqlText);
    Boolean IsPing();

    static void P2BS(const LPBASEPROTO xProto, mstr& xOut);
    static mstr P2BS(const LPBASEPROTO xProto);
    static void P2BS(const BaseProto& xProto, mstr& xOut);
    static mstr P2BS(const BaseProto& xProto);
    static void B2S(LPBYTE buf, OUT LPSTR szStr, Int32 len);

    // 填充结果
    static void FreeResult(MYSQL_RES* xMySqlRes);
    static Boolean FillResult(MYSQL_RES* xMySqlRes, LPBASEPROTO xResult);
    static Boolean FillResult(MYSQL_RES* xMySqlRes, MYSQL_ROW* lpSqlRow, LPBASEPROTO xResult);
    static DWORD GetResultColumnNum(MYSQL_RES* xMySqlRes);
    static MYSQL_ROW GetResultRowData(MYSQL_RES* xMySqlRes);

    static Boolean IsNull(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static Single GetSingle(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static Double GetDouble(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static Int64 GetInt64(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static UInt64 GetUInt64(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static Int32 GetInt32(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static UInt32 GetUInt32(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static Boolean GetBoolean(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static mstr GetString(MYSQL_ROW xRowData, DWORD xColumnIndex);
    static LPBYTE GetBytes(MYSQL_RES* xMySqlRes, MYSQL_ROW xRowData, DWORD xColumnIndex, OUT DWORD& dwSize);

    MysqlAutoRes    m_MysqlRes;
    mstr            m_Error;
protected:
    MYSQL*          m_MySql = nullptr;
    mstr            m_ConfigFileName;
    mstr            m_DBAddr;
    mstr            m_DBUser;
    mstr            m_DBPassword;
    mstr            m_DBName;
};


