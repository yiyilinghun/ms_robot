#pragma once
#define MAX_SQL_STR_LNE DEFAULT_BUFF_SIZE   // 最大SQL字符串长度
#define MAX_SQL_BIN_LNE DEFAULT_BUFF_SIZE   // 最大SQL二进制流长度

class MsBaseSql
{
public:
    MsBaseSql(LPCSTR szDBAddr, LPCSTR szDBUser, LPCSTR szDBPassword, LPCSTR szDBName);
    MsBaseSql(LPCSTR xConfigFileName);
    virtual ~MsBaseSql(void);
    virtual Boolean Connect() = 0;
    virtual void Disconnect() = 0;
    virtual Int64 GetInfluenceRowCount() = 0;   // 获取影响行数
    virtual Int64 GetInsertId() = 0;            // 获取插入后自增Id
    virtual mstr GetMysqlInfo() = 0;            // 获取Mysql信息


    static void P2BS(const LPBASEPROTO xProto, mstr& xOut);
    static mstr P2BS(const LPBASEPROTO xProto);
    static void P2BS(const BaseProto& xProto, mstr& xOut);
    static mstr P2BS(const BaseProto& xProto);
    static void B2S(LPBYTE buf, OUT LPSTR szStr, Int32 len);

    mstr GetExistsTableSqlByProto(LPBASEPROTO xProto); // 检查表是否存在
    mstr GetCreateSqlByProto(LPBASEPROTO xProto);   // 创建
    mstr GetDropSqlByProto(LPBASEPROTO xProto);     // 销毁
    mstr GetInsertSqlByProto(LPBASEPROTO xProto);   // 插入

    mstr GetSelectSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // 查询
    mstr GetUpdateSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // 更新
    mstr GetDeleteSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence);  // 删除

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
