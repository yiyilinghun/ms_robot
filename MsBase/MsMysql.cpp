#include "Precompiled.h"
using namespace google::protobuf;
typedef FieldDescriptor::Type ProtoType;

#define ONE_UNIVERSAL_ROW_COLUMN_COUNT 10

Boolean MysqlAutoRow::IsNull(DWORD xColumnIndex) { return MsMysql::IsNull(m_Row, xColumnIndex); }
Single  MysqlAutoRow::GetSingle(DWORD xColumnIndex) { return MsMysql::GetSingle(m_Row, xColumnIndex); }
Double  MysqlAutoRow::GetDouble(DWORD xColumnIndex) { return MsMysql::GetDouble(m_Row, xColumnIndex); }
Int64   MysqlAutoRow::GetInt64(DWORD xColumnIndex) { return MsMysql::GetInt64(m_Row, xColumnIndex); }
UInt64  MysqlAutoRow::GetUInt64(DWORD xColumnIndex) { return MsMysql::GetUInt64(m_Row, xColumnIndex); }
Int32   MysqlAutoRow::GetInt32(DWORD xColumnIndex) { return MsMysql::GetInt32(m_Row, xColumnIndex); }
UInt32  MysqlAutoRow::GetUInt32(DWORD xColumnIndex) { return MsMysql::GetUInt32(m_Row, xColumnIndex); }
Boolean MysqlAutoRow::GetBoolean(DWORD xColumnIndex) { return MsMysql::GetBoolean(m_Row, xColumnIndex); }
mstr    MysqlAutoRow::GetString(DWORD xColumnIndex) { return MsMysql::GetString(m_Row, xColumnIndex); }
LPBYTE  MysqlAutoRow::GetBytes(MYSQL_RES* xMySqlRes, DWORD xColumnIndex, OUT DWORD& dwSize) { return MsMysql::GetBytes(xMySqlRes, m_Row, xColumnIndex, dwSize); }

MysqlAutoRes::MysqlAutoRes()
    : m_Res(nullptr)
{
    ;
}

MysqlAutoRes::MysqlAutoRes(MYSQL_RES* xRes)
    : m_Res(xRes)
{
    ;
}

MysqlAutoRes::MysqlAutoRes(LPVOID xRes)
    : m_Res((MYSQL_RES*)xRes)
{
    ;
}

MysqlAutoRes::MysqlAutoRes::~MysqlAutoRes()
{
    this->Reset();
}

void MysqlAutoRes::operator =(MYSQL_RES* xRes)
{
    this->Reset();
    m_Res = xRes;
}

void MysqlAutoRes::operator =(LPVOID xRes)
{
    this->Reset();
    m_Res = (MYSQL_RES*)xRes;
}

MysqlAutoRes::operator MYSQL_RES*()
{
    return m_Res;
}

MysqlAutoRes::operator bool()
{
    return m_Res ? true : false;
}


Boolean MysqlAutoRes::FillResult(LPBASEPROTO xResult)
{
    return MsMysql::FillResult(m_Res, xResult);
}

DWORD MysqlAutoRes::GetResultColumnNum()
{
    return MsMysql::GetResultColumnNum(m_Res);
}

MYSQL_ROW MysqlAutoRes::GetResultRowData()
{
    return MsMysql::GetResultRowData(m_Res);
}

void MysqlAutoRes::Reset()
{
    if (m_Res)
    {
        MsMysql::FreeResult(m_Res);
        m_Res = nullptr;
    }
}

MysqlAutoRow::MysqlAutoRow()
    : m_Row(nullptr)
{
    ;
}

MysqlAutoRow::MysqlAutoRow(MYSQL_ROW xRow)
    : m_Row(xRow)
{
    ;
}

MysqlAutoRow::~MysqlAutoRow()
{

}

void MysqlAutoRow::operator=(MYSQL_ROW xRow)
{
    m_Row = xRow;
}

bool MysqlAutoRow::operator!()
{
    return !m_Row;
}

MysqlAutoRow::operator bool()
{
    return m_Row ? true : false;
}

MysqlAutoRow::operator MYSQL_ROW()
{
    return m_Row;
}

MysqlAutoRow::operator MYSQL_ROW*()
{
    return &m_Row;
}

LPCSTR ByteChar[] = {
    "00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
    "10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
    "20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
    "30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
    "40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
    "50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
    "60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
    "70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
    "80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
    "90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
    "A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
    "B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
    "C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
    "D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
    "E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
    "F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF",
};

void MsMysql::P2BS(const LPBASEPROTO xProto, mstr& xOut)
{
    return MsMysql::P2BS(*xProto, xOut);
}

mstr MsMysql::P2BS(const LPBASEPROTO xProto)
{
    return MsMysql::P2BS(*xProto);
}

void MsMysql::P2BS(const BaseProto& xProto, mstr& xOut)
{
    Int32 xProtoLen = xProto.ByteSize();
    if (xProtoLen == 0) {
        xOut = "0"; return;
    }

    LPBYTE xProtoData = NEW Byte[xProtoLen];
    if (!xProto.SerializeToArray(xProtoData, xProtoLen))
    {
        SAFE_DELETE_ARRAY(xProtoData);
        xOut = "0"; return;
    }
    LPSTR lpStrData = NEW CHAR[xProtoLen * 2 + 3];
    MsMysql::B2S(xProtoData, lpStrData + 2, xProtoLen);
    lpStrData[0] = '0'; lpStrData[1] = 'x'; xOut = lpStrData;
    SAFE_DELETE_ARRAY(lpStrData);
    SAFE_DELETE_ARRAY(xProtoData);
}

mstr MsMysql::P2BS(const BaseProto& xProto)
{
    Int32 xProtoLen = xProto.ByteSize();
    if (xProtoLen == 0) { return "0"; }

    LPBYTE xProtoData = NEW Byte[xProtoLen];
    if (!xProto.SerializeToArray(xProtoData, xProtoLen))
    {
        SAFE_DELETE_ARRAY(xProtoData);
        return "0";
    }
    LPSTR lpStrData = NEW CHAR[xProtoLen * 2 + 3];
    mstr xStrData;
    MsMysql::B2S(xProtoData, lpStrData + 2, xProtoLen);
    lpStrData[0] = '0'; lpStrData[1] = 'x'; xStrData = lpStrData;
    SAFE_DELETE_ARRAY(lpStrData);
    SAFE_DELETE_ARRAY(xProtoData);
    return xStrData;
}

void MsMysql::B2S(LPBYTE buf, OUT LPSTR szStr, Int32 len)
{
    for (Int32 i = 0; i < len; i++)
    {
        ((LPWORD)szStr)[i] = *(LPWORD)ByteChar[buf[i]];
    }
    szStr[len * 2] = 0;
}


MsMysql::MsMysql(LPCSTR szDBAddr, LPCSTR szDBUser, LPCSTR szDBPassword, LPCSTR szDBName)
    : m_MySql(mysql_init(nullptr))
    , m_DBAddr(szDBAddr)
    , m_DBUser(szDBUser)
    , m_DBPassword(szDBPassword)
    , m_DBName(szDBName)
{
    ;
}

MsMysql::MsMysql(LPCSTR xConfigFileName)
    : m_MySql(mysql_init(nullptr))
    , m_ConfigFileName(xConfigFileName)
{
    MsXmlReader xMsXmlReader;
    xMsXmlReader[xConfigFileName]["root"]["db"];
    if (xMsXmlReader)
    {
        m_DBAddr = xMsXmlReader.GetAttrib("db_addr");
        m_DBUser = xMsXmlReader.GetAttrib("db_user");
        m_DBPassword = xMsXmlReader.GetAttrib("db_password");
        m_DBName = xMsXmlReader.GetAttrib("db_name");
    }
}

MsMysql::~MsMysql(void)
{
    this->Disconnect();
}

Boolean MsMysql::IsPing()
{
    return (0 == mysql_ping(m_MySql));
}

// 执行格式化SQL语句
Boolean MsMysql::ExeSQLFormat(LPCSTR xSqlFormat, ...)
{
    va_list ap;
    CHAR szStrBuff[DEFAULT_BUFF_SIZE] = { 0 };
    va_start(ap, xSqlFormat);
    _vsnprintf_s(szStrBuff, DEFAULT_BUFF_SIZE, xSqlFormat, ap);
    va_end(ap);
    return this->ExeSQL(szStrBuff);
}

//MYSQL_RES* MsMysql::ExeSQLFormat(mstr& xError, MysqlAutoRes& xRes, LPCSTR xSqlFormat, ...)
//{
//    va_list ap;
//    CHAR szStrBuff[DEFAULT_BUFF_SIZE] = { 0 };
//    va_start(ap, xSqlFormat);
//    _vsnprintf_s(szStrBuff, DEFAULT_BUFF_SIZE, xSqlFormat, ap);
//    va_end(ap);
//    return this->ExeSQL(xError, xRes, szStrBuff);
//}

Boolean MsMysql::Connect()
{
    if (mysql_real_connect(m_MySql, m_DBAddr.c_str(), m_DBUser.c_str(), m_DBPassword.c_str(), m_DBName.c_str(), 0, nullptr, CLIENT_MULTI_RESULTS || CLIENT_INTERACTIVE))
    {
        ExeSQL("set interactive_timeout=2880000");
        ExeSQL("set wait_timeout=2880000");
        Byte xParam = 1;
        mysql_options(m_MySql, MYSQL_OPT_RECONNECT, &xParam);
        mysql_set_character_set(m_MySql, "gbk");
        return True;
    }
    AssertLog("MsMysql::Connect,失败[%s]", mysql_error(m_MySql));
    return False;
}

void MsMysql::Disconnect()
{
    if (m_MysqlRes)
    {
        m_MysqlRes.Reset();
    }
    if (m_MySql)
    {
        mysql_close(m_MySql);
        m_MySql = nullptr;
    }
}

void MsMysql::FreeResult(MYSQL_RES* xMySqlRes)
{
    if (xMySqlRes)
    {
        mysql_free_result(xMySqlRes);
    }
}

Boolean MsMysql::ExeSQL(mstr xSql)
{
    m_Error = "";
    if (!m_MySql) { return False; }
    if (m_MysqlRes) { m_MysqlRes.Reset(); }

    Int32 xPingRet = mysql_ping(m_MySql);
    if (xPingRet != 0)
    {
        m_Error = mysql_error(m_MySql);
    }

    while (mysql_real_query(m_MySql, xSql.c_str(), (ULONG)xSql.size()))
    {
        m_Error = mysql_error(m_MySql);
        if (m_Error == "Deadlock found when trying to get lock; try restarting transaction")
        {
            ::SleepEx(5, True);
            continue;
        }
        else
        {
            AssertLog("%s", xSql.c_str());
            AssertLog("%s", m_Error.c_str());
            return False;
        }
    }
    m_MysqlRes = mysql_use_result(m_MySql);
    return True;
}

//MYSQL_RES* MsMysql::ExeSQL(mstr& xError, MysqlAutoRes& xRes, mstr xSql)
//{
//    xRes.Reset();
//    if (!m_MySql) { return False; }
//
//    MYSQL_RES* xMySqlRes = nullptr;
//
//    Int32 xPingRet = mysql_ping(m_MySql);
//    if (xPingRet != 0)
//    {
//        xError = mysql_error(m_MySql);
//    }
//
//    if (mysql_real_query(m_MySql, xSql.c_str(), (ULONG)xSql.size()))
//    {
//        xError = mysql_error(m_MySql);
//        AssertLog("%s", xSql.c_str());
//        AssertLog("%s", xError.c_str());
//        return nullptr;
//    }
//    else
//    {
//        xMySqlRes = mysql_use_result(m_MySql);
//    }
//
//    xRes = xMySqlRes;
//    return xMySqlRes;
//}

Boolean MsMysql::FillResult(MYSQL_RES* xMySqlRes, LPBASEPROTO xResult)
{
    if (!xMySqlRes) { return False; }
    MYSQL_ROW xSqlRow = mysql_fetch_row(xMySqlRes);
    if (xSqlRow)
    {
        return MsMysql::FillResult(xMySqlRes, &xSqlRow, xResult);
    }
    return False;
}

Boolean MsMysql::FillResult(MYSQL_RES* xMySqlRes, MYSQL_ROW* lpSqlRow, LPBASEPROTO xResult)
{
    if (!xMySqlRes) { return False; }
    LPDWORD xSqlFieldLen = mysql_fetch_lengths(xMySqlRes);
    DWORD xFieldCount = mysql_num_fields(xMySqlRes);
    if (lpSqlRow)
    {
        MYSQL_ROW& xSqlRow = *lpSqlRow;
        const Reflection* xReflection = xResult->GetReflection();
        const Descriptor* xDescriptor = xResult->GetDescriptor();
        for (DWORD xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
        {
            LPSTR xFieldName = xMySqlRes->fields[xFieldIndex].name;
            const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByName(xFieldName);
            if (xField)
            {
                switch (xField->type())
                {
                    case ProtoType::TYPE_MESSAGE:
                    {
                        ULONG xBinaryLen = xSqlFieldLen[xFieldIndex];
                        if (xBinaryLen > 0)
                        {
                            Message* xMsg = xReflection->MutableMessage(xResult, xField);
                            xMsg->ParseFromArray(xSqlRow[xFieldIndex], xBinaryLen);
                        }
                    }break;

                    case ProtoType::TYPE_FLOAT:
                    {
                        xReflection->SetFloat(xResult, xField, google::protobuf::strtof(xSqlRow[xFieldIndex], nullptr));
                    }break;

                    case ProtoType::TYPE_DOUBLE:
                    {
                        xReflection->SetDouble(xResult, xField, google::protobuf::strtod(xSqlRow[xFieldIndex], nullptr));
                    }break;

                    case ProtoType::TYPE_INT64:
                    case ProtoType::TYPE_SINT64:
                    case ProtoType::TYPE_FIXED64:
                    case ProtoType::TYPE_SFIXED64:
                    {
                        xReflection->SetInt64(xResult, xField, google::protobuf::strto64(xSqlRow[xFieldIndex], nullptr, 10));
                    }break;

                    case ProtoType::TYPE_UINT64:
                    {
                        xReflection->SetUInt64(xResult, xField, google::protobuf::strtou64(xSqlRow[xFieldIndex], nullptr, 10));
                    }break;

                    case ProtoType::TYPE_INT32:
                    case ProtoType::TYPE_SINT32:
                    case ProtoType::TYPE_FIXED32:
                    case ProtoType::TYPE_SFIXED32:
                    {
                        xReflection->SetInt32(xResult, xField, google::protobuf::strto32(xSqlRow[xFieldIndex], nullptr, 10));
                    }break;

                    case ProtoType::TYPE_UINT32:
                    {
                        xReflection->SetUInt32(xResult, xField, google::protobuf::strtou32(xSqlRow[xFieldIndex], nullptr, 10));
                    }break;
                    case ProtoType::TYPE_ENUM:
                    {
                        Int32 xValue = google::protobuf::strto32(xSqlRow[xFieldIndex], nullptr, 10);
                        const google::protobuf::EnumDescriptor* lpEnumDescriptor = google::protobuf::DescriptorPool::generated_pool()->FindEnumTypeByName(xField->enum_type()->name());
                        for (Int32 i = 0; i < lpEnumDescriptor->value_count(); i++)
                        {
                            const google::protobuf::EnumValueDescriptor* lpEnumValueDescriptor = lpEnumDescriptor->FindValueByNumber(i);
                            if (lpEnumValueDescriptor && lpEnumValueDescriptor->number() == xValue)
                            {
                                xReflection->SetEnum(xResult, xField, lpEnumValueDescriptor);
                                break;
                            }
                        }
                    }break;

                    case ProtoType::TYPE_BOOL:
                    {
                        xReflection->SetBool(xResult, xField, *((CHAR*)(xSqlRow[xFieldIndex])) == '1' ? true : false);
                    }break;

                    case ProtoType::TYPE_STRING:
                    {
                        xReflection->SetString(xResult, xField, xSqlRow[xFieldIndex]);
                    }break;

                    case ProtoType::TYPE_BYTES:
                    {
                        ULONG xBinaryLen = xSqlFieldLen[xFieldIndex];
                        if (xBinaryLen > 0 && xBinaryLen < MAX_SQL_BIN_LNE)
                        {
                            xReflection->SetString(xResult, xField, mstr(xSqlRow[xFieldIndex], xBinaryLen));
                        }
                    }break;

                    default: {; }
                }
            }

        }
        return True;
    }
    return False;
}

Int64 MsMysql::GetInfluenceRowCount()
{
    return (Int64)mysql_affected_rows(m_MySql);
}

Int64 MsMysql::GetInsertId()
{
    return (Int64)mysql_insert_id(m_MySql);
}

Boolean MsMysql::GetUniversalDataKeys(Int64 xMainKey, MsSet<Int64>& xSetKeys)
{
    mstr xError;
    MysqlAutoRow xRow;
    MsSet<Int64> xTempKeys;
    if (this->ExeSQLFormat("SELECT `sub_0`, `sub_1`, `sub_2`, `sub_3`, `sub_4`, `sub_5`, `sub_6`, `sub_7`, `sub_8`, `sub_9` FROM `value` WHERE `key` = %lld", xMainKey) && m_MysqlRes)
    {
        xRow = this->NextRow();
        while (xRow)
        {
            if (!xRow.IsNull(0)) { xTempKeys.Add(xRow.GetInt64(0)); }
            if (!xRow.IsNull(1)) { xTempKeys.Add(xRow.GetInt64(1)); }
            if (!xRow.IsNull(2)) { xTempKeys.Add(xRow.GetInt64(2)); }
            if (!xRow.IsNull(3)) { xTempKeys.Add(xRow.GetInt64(3)); }
            if (!xRow.IsNull(4)) { xTempKeys.Add(xRow.GetInt64(4)); }
            if (!xRow.IsNull(5)) { xTempKeys.Add(xRow.GetInt64(5)); }
            if (!xRow.IsNull(6)) { xTempKeys.Add(xRow.GetInt64(6)); }
            if (!xRow.IsNull(7)) { xTempKeys.Add(xRow.GetInt64(7)); }
            if (!xRow.IsNull(8)) { xTempKeys.Add(xRow.GetInt64(8)); }
            if (!xRow.IsNull(9)) { xTempKeys.Add(xRow.GetInt64(9)); }
            xRow = this->NextRow();
        }
    }
    BEGIN_ENUMERATOR(xTempKeys)
    {
        this->GetUniversalDataKeys(xEnumKey, xSetKeys); xSetKeys.Add(xEnumKey);
    }END_ENUMERATOR;
    return True;
}

void MsMysql::RemoveUniversalDatas(Int64 xMainKey, Boolean IsMain)
{
    MsSet<Int64> xSetKeys;
    GetUniversalDataKeys(xMainKey, xSetKeys);
    RemoveUniversalDatas(xMainKey, xSetKeys, IsMain);
}

void MsMysql::RemoveUniversalDatas(Int64 xMainKey, MsSet<Int64>& xSetKeys, Boolean IsMain)
{
    if (IsMain)
    {
        this->ExeSQLFormat("DELETE FROM `main` WHERE `key` = %lld ", xMainKey);
    }
    this->ExeSQLFormat("DELETE FROM `value` WHERE `key` = %lld ", xMainKey);
    BEGIN_ENUMERATOR(xSetKeys)
    {
        this->ExeSQLFormat("DELETE FROM `value` WHERE `key` = %lld ", xEnumKey);
    }END_ENUMERATOR;
}

Boolean MsMysql::LoadUniversalData(Int64 xKey, universal_data& xUniversalData, Boolean IsMain)
{
    MysqlAutoRow xRow;
    if (IsMain)
    {
        if (this->ExeSQLFormat("SELECT `type`, `name` FROM `main` WHERE `key` = %lld LIMIT 1", xKey) && m_MysqlRes)
        {
            xRow = this->NextRow();
            if (xRow)
            {
                xUniversalData.set_key(xKey);
                xUniversalData.set_type(xRow.GetString(0));
                xUniversalData.set_name(xRow.GetString(1));
            }
            else
            {
                AssertLog("fadsfdsafadsfdsa");
            }
        }
        else
        {
            AssertLog("fadsfdsafadsfdsa");
        }
    }
    else
    {
        if (this->ExeSQLFormat("SELECT `type`, `name` FROM `value` WHERE `key` = %lld LIMIT 1", xKey) && m_MysqlRes)
        {
            xRow = this->NextRow();
            if (xRow)
            {
                xUniversalData.set_key(xKey);
                xUniversalData.set_type(xRow.GetString(0));
                xUniversalData.set_name(xRow.GetString(1));
            }
            else
            {
                AssertLog("fadsfdsafadsfdsa");
            }
        }
        else
        {
            AssertLog("fadsfdsafadsfdsa");
        }
    }

    // value
    MsDictionary<Int64, universal_data*> xDictSubData;
    if (this->ExeSQLFormat("SELECT * FROM `value` WHERE `key` = %lld", xKey) && m_MysqlRes)
    {
        xRow = m_MysqlRes.GetResultRowData();
        while (xRow)
        {
            Int32 xBeginColumnIndex = 4;
            for (Int32 i = 0; i < ONE_UNIVERSAL_ROW_COLUMN_COUNT; i++)
            {
                Int32 xIndex = i + (xRow.GetInt32(1) * ONE_UNIVERSAL_ROW_COLUMN_COUNT);

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto b = xRow.GetBoolean(xBeginColumnIndex);
                    while (xUniversalData.b_size() <= xIndex) { xUniversalData.add_b(false); }
                    xUniversalData.set_b(xIndex, b ? true : false);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto f = xRow.GetSingle(xBeginColumnIndex);
                    while (xUniversalData.f_size() <= xIndex) { xUniversalData.add_f(0.0f); }
                    xUniversalData.set_f(xIndex, f);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto d = xRow.GetDouble(xBeginColumnIndex);
                    while (xUniversalData.d_size() <= xIndex) { xUniversalData.add_d(0.0); }
                    xUniversalData.set_d(xIndex, d);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto e = xRow.GetInt32(xBeginColumnIndex);
                    while (xUniversalData.e_size() <= xIndex) { xUniversalData.add_e(0); }
                    xUniversalData.set_e(xIndex, e);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto i32 = xRow.GetInt32(xBeginColumnIndex);
                    while (xUniversalData.i32_size() <= xIndex) { xUniversalData.add_i32(0); }
                    xUniversalData.set_i32(xIndex, i32);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto i64 = xRow.GetInt64(xBeginColumnIndex);
                    while (xUniversalData.i64_size() <= xIndex) { xUniversalData.add_i64(0i64); }
                    xUniversalData.set_i64(xIndex, i64);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto u32 = xRow.GetUInt32(xBeginColumnIndex);
                    while (xUniversalData.u32_size() <= xIndex) { xUniversalData.add_u32(0); }
                    xUniversalData.set_u32(xIndex, u32);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto u64 = xRow.GetUInt64(xBeginColumnIndex);
                    while (xUniversalData.u64_size() <= xIndex) { xUniversalData.add_u64(0i64); }
                    xUniversalData.set_u64(xIndex, u64);
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto str = xRow.GetString(xBeginColumnIndex);
                    while (xUniversalData.str_size() <= xIndex) { xUniversalData.add_str(""); }
                    xUniversalData.set_str(xIndex, str.c_str());
                }xBeginColumnIndex++;

                if (!xRow.IsNull(xBeginColumnIndex)) {
                    auto sub = xRow.GetInt64(xBeginColumnIndex);
                    while (xUniversalData.sub_size() <= xIndex) { xUniversalData.add_sub(); }
                    xDictSubData.Add(sub, xUniversalData.mutable_sub(xIndex));
                }xBeginColumnIndex++;
            }
            xRow = this->NextRow();
        }
        BEGIN_ENUMERATOR(xDictSubData)
        {
            this->LoadUniversalData(xEnumKey, xEnumValue, False);
        }END_ENUMERATOR;

        if (!xUniversalData.has_name())
        {
            AssertLog("fadsfdsafadsfdsa");
        }
        return True;
    }
    return False;
}

Boolean MsMysql::SaveUniversalData(const universal_data& xUniversalData, Boolean canUpdate, MsIOCPManager* xMsIOCPManager)
{
    mstr xError;

    if (INVALID_LID == this->CheckKey((universal_data*)&xUniversalData, xMsIOCPManager))
    {
        return False;
    }

    if (MsMysql::CheckSqlKeyword(xUniversalData.type()))
    {
        AssertNormal("字段[type]存在非法字符:[%s]", xUniversalData.type().c_str());
        return False;
    }

    if (MsMysql::CheckSqlKeyword(xUniversalData.name()))
    {
        AssertNormal("字段[name]存在非法字符:[%s]", xUniversalData.name().c_str());
        return False;
    }

    if (canUpdate)
    {
        // 插入或更新主记录
        this->ExeSQLFormat("INSERT INTO `main` VALUES(%lld, '%s', '%s') ON DUPLICATE KEY UPDATE "
            "`key` = %lld, `type`='%s',`name`='%s'",
            xUniversalData.key(),
            xUniversalData.type().c_str(),
            xUniversalData.name().c_str(),
            xUniversalData.key(),
            xUniversalData.type().c_str(),
            xUniversalData.name().c_str()
        );
    }
    else
    {
        // 尝试插入主记录
        this->ExeSQLFormat("INSERT INTO `main` VALUES(%lld, '%s', '%s')",
            xUniversalData.key(),
            xUniversalData.type().c_str(),
            xUniversalData.name().c_str()
        );
    }

    if (m_Error != "")
    {
        AssertNormal("插入主记录失败[%s]", xError.c_str());
        return False;
    }

    this->SaveValue(xUniversalData, xMsIOCPManager);
    return True;
}

//Boolean MsMysql::EqualUniversalData(universal_data& xUniversalData, Boolean IsMain)
//{
//    Int64 xKey = xUniversalData.key();
//    mstr xError;
//    MysqlAutoRow xRow;
//    if (IsMain)
//    {
//        if (this->ExeSQLFormat(xError, "SELECT `type`, `name` FROM `main` WHERE `key` = %lld", xKey) && m_MysqlRes)
//        {
//            xRow = this->NextRow();
//            if (xRow)
//            {
//                if (xUniversalData.type() != xRow.GetString(0)) { return False; }
//                if (xUniversalData.name() != xRow.GetString(1)) { return False; }
//            }
//            else
//            {
//                return False;
//            }
//        }
//        else
//        {
//            return False;
//        }
//    }
//
//    // b
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `b` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetBoolean(1);
//            while (xUniversalData.b_size() <= xIndex) { return False; }
//            if ((Boolean)(xUniversalData.b(xIndex)) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // d
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `d` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetDouble(1);
//            while (xUniversalData.d_size() <= xIndex) { return False; }
//            if (xUniversalData.d(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // f
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `f` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetSingle(1);
//            while (xUniversalData.f_size() <= xIndex) { return False; }
//            if (xUniversalData.f(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // i32
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `i32` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetInt32(1);
//            while (xUniversalData.i32_size() <= xIndex) { return False; }
//            if (xUniversalData.i32(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // i64
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `i64` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetInt64(1);
//            while (xUniversalData.i64_size() <= xIndex) { return False; }
//            if (xUniversalData.i64(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // str
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `str` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetString(1);
//            while (xUniversalData.str_size() <= xIndex) { return False; }
//            if (xUniversalData.str(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // u32
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `u32` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetUInt32(1);
//            while (xUniversalData.u32_size() <= xIndex) { return False; }
//            if (xUniversalData.u32(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // u64
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `value` FROM `u64` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            auto xValue = xRow.GetUInt64(1);
//            while (xUniversalData.u64_size() <= xIndex) { return False; }
//            if (xUniversalData.u64(xIndex) != xValue) { return False; }
//            xRow = this->NextRow();
//        }
//    }
//
//    // sub
//    if (this->ExeSQLFormat(xError, "SELECT `index`, `type`, `name`, `value` FROM `sub` WHERE `key` = %lld", xKey) && m_MysqlRes)
//    {
//        xRow = this->NextRow();
//        MsDictionary<Int64, universal_data*> xTempDictData;
//        while (xRow)
//        {
//            Int32 xIndex = xRow.GetInt32(0);
//            mstr xType = xRow.GetString(1);
//            mstr xName = xRow.GetString(2);
//            Int64 xValueKey = xRow.GetInt64(3);
//            while (xUniversalData.sub_size() <= xIndex) { xUniversalData.add_sub(); }
//            xTempDictData[xValueKey] = xUniversalData.mutable_sub(xIndex);
//            xTempDictData[xValueKey]->set_key(xValueKey);
//            xTempDictData[xValueKey]->set_type(xType);
//            xTempDictData[xValueKey]->set_name(xName);
//
//            xRow = this->NextRow();
//        }
//
//        BEGIN_ENUMERATOR(xTempDictData)
//        {
//            if (!this->EqualUniversalData(*(xEnumValue), False))
//            {
//                return False;
//            }
//        }END_ENUMERATOR;
//    }
//
//    return True;
//}

Boolean MsMysql::CheckSqlKeyword(mstr xSqlText)
{
    mstr StrRegex = " select | insert | delete | from | count | drop | table | update |"
        " truncate | asc | mid | char | xp_cmdshell | exec | master |"
        " netlocalgroup | administrators | : | net | user | or | and |\t|\n|`|"
        "\\||&|-|;|\\|\\(|\\)|\\[|\\]|\\{|\\}|%|\\*|!|\'|\"";
    //"\\||&|-|;|\\|\\(|\\)|\\[|\\]|\\{|\\}|%|@|\\*|!|\'|\"";
    //"\\||&|-|;|,|\\|\\(|\\)|\\[|\\]|\\{|\\}|%|@|\\*|!|\'|\"";
    //"\\||&|-|;|,|/|\\|\\(|\\)|\\[|\\]|\\{|\\}|%|@|\\*|!|\'|\"";

    return MsBase::RegexCheck(xSqlText, StrRegex);
}

Int64 MsMysql::CheckKey(universal_data* xUniversalData, MsIOCPManager* xMsIOCPManager)
{
    if (!xUniversalData->has_key() || xUniversalData->key() == INVALID_LID)
    {
        xUniversalData->set_key(xMsIOCPManager->GetNextUnitIndex());
    }
    return xUniversalData->key();
}

void MsMysql::MakeSubValueSql(const universal_data& xUniversalData, Int32 xIndex, mstr& xValueSql, MsIOCPManager* xMsIOCPManager)
{
    //Int32 xColumnIndex = xMaxCount % ONE_UNIVERSAL_ROW_COLUMN_COUNT; Int32 xRowIndex = xMaxCount / ONE_UNIVERSAL_ROW_COLUMN_COUNT;
    // , %s, %s, %s, %s, %s, %s, %s, %s, %s
    FFORMAT(xBeginSql, "(%lld, %d, '%s', '%s' ",
        this->CheckKey((universal_data*)&xUniversalData, xMsIOCPManager),
        xIndex,
        xUniversalData.type().c_str(),
        xUniversalData.name().c_str()
    );
    mstr xEndSql = "),\n";

    mstr xTempValueSql;
    for (Int32 i = 0; i < ONE_UNIVERSAL_ROW_COLUMN_COUNT; i++)
    {
        Int32 xRealIndex = i + (xIndex * ONE_UNIVERSAL_ROW_COLUMN_COUNT);
        mstr b = xUniversalData.b_size() > xRealIndex ? MsBase::ToString(xUniversalData.b(xRealIndex)) : "NULL";
        mstr f = xUniversalData.f_size() > xRealIndex ? MsBase::ToString(xUniversalData.f(xRealIndex)) : "NULL";
        mstr d = xUniversalData.d_size() > xRealIndex ? MsBase::ToString(xUniversalData.d(xRealIndex)) : "NULL";
        mstr e = xUniversalData.e_size() > xRealIndex ? MsBase::ToString(xUniversalData.e(xRealIndex)) : "NULL";
        mstr i32 = xUniversalData.i32_size() > xRealIndex ? MsBase::ToString(xUniversalData.i32(xRealIndex)) : "NULL";
        mstr i64 = xUniversalData.i64_size() > xRealIndex ? MsBase::ToString(xUniversalData.i64(xRealIndex)) : "NULL";
        mstr u32 = xUniversalData.u32_size() > xRealIndex ? MsBase::ToString(xUniversalData.u32(xRealIndex)) : "NULL";
        mstr u64 = xUniversalData.u64_size() > xRealIndex ? MsBase::ToString(xUniversalData.u64(xRealIndex)) : "NULL";
        mstr str = xUniversalData.str_size() > xRealIndex ? xUniversalData.str(xRealIndex) : "NULL";
        mstr sub = xUniversalData.sub_size() > xRealIndex ? MsBase::ToString(this->CheckKey((universal_data*)&xUniversalData.sub(xRealIndex), xMsIOCPManager)) : "NULL";
        if (MsMysql::CheckSqlKeyword(str))
        {
            AssertLog("字段[str][%d]存在非法字符:[%s]", i, str.c_str());
            return;
        }
        else if (str != "NULL")
        {
            str = MsBase::Format("'%s'", str.c_str());
        }
        // b   f   d   e   i32 i64 u32 u64 str sub
        FFORMAT(xTempSql, ", %s, %s, %s, %s, %s, %s, %s, %s, %s, %s",
            b.c_str(), f.c_str(), d.c_str(), e.c_str(), i32.c_str(), i64.c_str(), u32.c_str(), u64.c_str(), str.c_str(), sub.c_str()
        );
        xTempValueSql += xTempSql;
    }

    xValueSql += xBeginSql;
    xValueSql += xTempValueSql;
    xValueSql += xEndSql;
}

void MsMysql::MakeValueSql(const universal_data& xUniversalData, MsList<mstr>& xListValueSql, MsIOCPManager* xMsIOCPManager)
{
    Int32 xMaxCount = 0;
    xMaxCount = xUniversalData.b_size() > xMaxCount ? xUniversalData.b_size() : xMaxCount;
    xMaxCount = xUniversalData.f_size() > xMaxCount ? xUniversalData.f_size() : xMaxCount;
    xMaxCount = xUniversalData.d_size() > xMaxCount ? xUniversalData.d_size() : xMaxCount;
    xMaxCount = xUniversalData.e_size() > xMaxCount ? xUniversalData.e_size() : xMaxCount;
    xMaxCount = xUniversalData.i32_size() > xMaxCount ? xUniversalData.i32_size() : xMaxCount;
    xMaxCount = xUniversalData.i64_size() > xMaxCount ? xUniversalData.i64_size() : xMaxCount;
    xMaxCount = xUniversalData.u32_size() > xMaxCount ? xUniversalData.u32_size() : xMaxCount;
    xMaxCount = xUniversalData.u64_size() > xMaxCount ? xUniversalData.u64_size() : xMaxCount;
    xMaxCount = xUniversalData.str_size() > xMaxCount ? xUniversalData.str_size() : xMaxCount;
    xMaxCount = xUniversalData.sub_size() > xMaxCount ? xUniversalData.sub_size() : xMaxCount;

    if (xMaxCount == 0)
    {
        FFORMAT(xTempSql, "(%lld, 0, '%s', '%s'", this->CheckKey((universal_data*)&xUniversalData, xMsIOCPManager), xUniversalData.type().c_str(), xUniversalData.name().c_str());
        for (Int32 i = 0; i < ONE_UNIVERSAL_ROW_COLUMN_COUNT; i++)
        {
            xTempSql += ", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL";
        }
        xTempSql += "),\n";
        xListValueSql.Add(xTempSql);
    }
    else
    {
        for (Int32 i = 0; i <= xMaxCount / ONE_UNIVERSAL_ROW_COLUMN_COUNT; i++)
        {
            mstr xTempSql;
            this->MakeSubValueSql(xUniversalData, i, xTempSql, xMsIOCPManager);
            xListValueSql.Add(xTempSql);
        }

        for (Int32 i = 0; i < xUniversalData.sub_size(); i++)
        {
            this->MakeValueSql(xUniversalData.sub(i), xListValueSql, xMsIOCPManager);
        }
    }
}

Boolean MsMysql::SaveValue(const universal_data& xUniversalData, MsIOCPManager* xMsIOCPManager)
{
    mstr xBeginSql = "INSERT INTO `value`(`key`, `index`, `type`, `name`, "
        "`b_0`, `f_0`, `d_0`, `e_0`, `i32_0`, `i64_0`, `u32_0`, `u64_0`, `str_0`, `sub_0`, "
        "`b_1`, `f_1`, `d_1`, `e_1`, `i32_1`, `i64_1`, `u32_1`, `u64_1`, `str_1`, `sub_1`, "
        "`b_2`, `f_2`, `d_2`, `e_2`, `i32_2`, `i64_2`, `u32_2`, `u64_2`, `str_2`, `sub_2`, "
        "`b_3`, `f_3`, `d_3`, `e_3`, `i32_3`, `i64_3`, `u32_3`, `u64_3`, `str_3`, `sub_3`, "
        "`b_4`, `f_4`, `d_4`, `e_4`, `i32_4`, `i64_4`, `u32_4`, `u64_4`, `str_4`, `sub_4`, "
        "`b_5`, `f_5`, `d_5`, `e_5`, `i32_5`, `i64_5`, `u32_5`, `u64_5`, `str_5`, `sub_5`, "
        "`b_6`, `f_6`, `d_6`, `e_6`, `i32_6`, `i64_6`, `u32_6`, `u64_6`, `str_6`, `sub_6`, "
        "`b_7`, `f_7`, `d_7`, `e_7`, `i32_7`, `i64_7`, `u32_7`, `u64_7`, `str_7`, `sub_7`, "
        "`b_8`, `f_8`, `d_8`, `e_8`, `i32_8`, `i64_8`, `u32_8`, `u64_8`, `str_8`, `sub_8`, "
        "`b_9`, `f_9`, `d_9`, `e_9`, `i32_9`, `i64_9`, `u32_9`, `u64_9`, `str_9`, `sub_9` "
        ") VALUES";
    //mstr xBeginSql = "INSERT INTO `value`(`key`, `index`, `type`, `name`, `b`, `f`, `d`, `e`, `i32`, `i64`, `u32`, `u64`, `str`, `data`) VALUES";
    mstr xEndSql = "ON DUPLICATE KEY UPDATE `type`=VALUES(`type`), `name`=VALUES(`name`), "
        " `b_0`=VALUES(`b_0`), `f_0`=VALUES(`f_0`), `d_0`=VALUES(`d_0`), `e_0`=VALUES(`e_0`), `i32_0`=VALUES(`i32_0`), `i64_0`=VALUES(`i64_0`), `u32_0`=VALUES(`u32_0`), `u64_0`=VALUES(`u64_0`), `str_0`=VALUES(`str_0`), `sub_0`=VALUES(`sub_0`),"
        " `b_1`=VALUES(`b_1`), `f_1`=VALUES(`f_1`), `d_1`=VALUES(`d_1`), `e_1`=VALUES(`e_1`), `i32_1`=VALUES(`i32_1`), `i64_1`=VALUES(`i64_1`), `u32_1`=VALUES(`u32_1`), `u64_1`=VALUES(`u64_1`), `str_1`=VALUES(`str_1`), `sub_1`=VALUES(`sub_1`),"
        " `b_2`=VALUES(`b_2`), `f_2`=VALUES(`f_2`), `d_2`=VALUES(`d_2`), `e_2`=VALUES(`e_2`), `i32_2`=VALUES(`i32_2`), `i64_2`=VALUES(`i64_2`), `u32_2`=VALUES(`u32_2`), `u64_2`=VALUES(`u64_2`), `str_2`=VALUES(`str_2`), `sub_2`=VALUES(`sub_2`),"
        " `b_3`=VALUES(`b_3`), `f_3`=VALUES(`f_3`), `d_3`=VALUES(`d_3`), `e_3`=VALUES(`e_3`), `i32_3`=VALUES(`i32_3`), `i64_3`=VALUES(`i64_3`), `u32_3`=VALUES(`u32_3`), `u64_3`=VALUES(`u64_3`), `str_3`=VALUES(`str_3`), `sub_3`=VALUES(`sub_3`),"
        " `b_4`=VALUES(`b_4`), `f_4`=VALUES(`f_4`), `d_4`=VALUES(`d_4`), `e_4`=VALUES(`e_4`), `i32_4`=VALUES(`i32_4`), `i64_4`=VALUES(`i64_4`), `u32_4`=VALUES(`u32_4`), `u64_4`=VALUES(`u64_4`), `str_4`=VALUES(`str_4`), `sub_4`=VALUES(`sub_4`),"
        " `b_5`=VALUES(`b_5`), `f_5`=VALUES(`f_5`), `d_5`=VALUES(`d_5`), `e_5`=VALUES(`e_5`), `i32_5`=VALUES(`i32_5`), `i64_5`=VALUES(`i64_5`), `u32_5`=VALUES(`u32_5`), `u64_5`=VALUES(`u64_5`), `str_5`=VALUES(`str_5`), `sub_5`=VALUES(`sub_5`),"
        " `b_6`=VALUES(`b_6`), `f_6`=VALUES(`f_6`), `d_6`=VALUES(`d_6`), `e_6`=VALUES(`e_6`), `i32_6`=VALUES(`i32_6`), `i64_6`=VALUES(`i64_6`), `u32_6`=VALUES(`u32_6`), `u64_6`=VALUES(`u64_6`), `str_6`=VALUES(`str_6`), `sub_6`=VALUES(`sub_6`),"
        " `b_7`=VALUES(`b_7`), `f_7`=VALUES(`f_7`), `d_7`=VALUES(`d_7`), `e_7`=VALUES(`e_7`), `i32_7`=VALUES(`i32_7`), `i64_7`=VALUES(`i64_7`), `u32_7`=VALUES(`u32_7`), `u64_7`=VALUES(`u64_7`), `str_7`=VALUES(`str_7`), `sub_7`=VALUES(`sub_7`),"
        " `b_8`=VALUES(`b_8`), `f_8`=VALUES(`f_8`), `d_8`=VALUES(`d_8`), `e_8`=VALUES(`e_8`), `i32_8`=VALUES(`i32_8`), `i64_8`=VALUES(`i64_8`), `u32_8`=VALUES(`u32_8`), `u64_8`=VALUES(`u64_8`), `str_8`=VALUES(`str_8`), `sub_8`=VALUES(`sub_8`),"
        " `b_9`=VALUES(`b_9`), `f_9`=VALUES(`f_9`), `d_9`=VALUES(`d_9`), `e_9`=VALUES(`e_9`), `i32_9`=VALUES(`i32_9`), `i64_9`=VALUES(`i64_9`), `u32_9`=VALUES(`u32_9`), `u64_9`=VALUES(`u64_9`), `str_9`=VALUES(`str_9`), `sub_9`=VALUES(`sub_9`)";

    MsList<mstr> xListValueSql;
    this->MakeValueSql(xUniversalData, xListValueSql, xMsIOCPManager);

    if (xListValueSql.GetCount() > 0)
    {
        mstr& xStr = xListValueSql[xListValueSql.GetCount() - 1];
        xStr = xStr.substr(0, xStr.size() - 2);
        xStr += '\n';
    }

    mstr xSql = xBeginSql;
    BEGIN_ENUMERATOR(xListValueSql)
    {
        xSql += xEnumValue;
    }END_ENUMERATOR;
    xSql += xEndSql;

    this->ExeSQL(xSql.c_str());

    //for (Int32 i = 0; i < xUniversalData.data_size(); i++)
    //{
    //    this->SaveValue(xUniversalData.data(i), xMsIOCPManager);
    //}

    return True;
}

// 检查指定表是否存在
Boolean MsMysql::CheckTableExist(mstr xTableName)
{
    CHAR szTemp[MAX_SQL_STR_LNE];
    sprintf_s(szTemp
        , "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='%s' AND TABLE_NAME='%s';"
        , m_DBName.c_str()
        , xTableName.c_str()
    );

    if (this->ExeSQL(szTemp) && m_MysqlRes)
    {
        MYSQL_ROW xRowData = this->NextRow();
        DWORD xColumnCount = this->GetResultColumnNum(m_MysqlRes);
        if (xRowData && xColumnCount == 1)
        {
            mstr xRetName = this->GetString(xRowData, 0);
            if (xRetName == xTableName)
            {
                return True;
            }
        }
    }
    return False;
}

mstr MsMysql::GetMysqlInfo()
{
    LPCSTR x = mysql_error(m_MySql);
    return mstr(x);
}

DWORD MsMysql::GetResultColumnNum(MYSQL_RES* xMySqlRes)
{
    return mysql_num_fields(xMySqlRes);
}

MYSQL_ROW MsMysql::GetResultRowData(MYSQL_RES* xMySqlRes)
{
    return mysql_fetch_row(xMySqlRes);
}

Boolean MsMysql::IsNull(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return xRowData[xColumnIndex] == nullptr;
}

Single MsMysql::GetSingle(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strtof(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr);
}

Double MsMysql::GetDouble(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strtod(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr);
}

Int64 MsMysql::GetInt64(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strto64(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr, 10);
}

UInt64 MsMysql::GetUInt64(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strtou64(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr, 10);
}

Int32 MsMysql::GetInt32(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strto32(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr, 10);
}

UInt32 MsMysql::GetUInt32(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return google::protobuf::strtou32(((MYSQL_ROW)xRowData)[xColumnIndex], nullptr, 10);
}

Boolean MsMysql::GetBoolean(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    mstr xRet = GetString(xRowData, xColumnIndex);
    //AssertLog("%s", xRet.c_str());
    transform(xRet.begin(), xRet.end(), xRet.begin(), ::tolower);
    if (xRet == "true" || xRet == "1")
    {
        return True;
    }
    return False;
}

mstr MsMysql::GetString(MYSQL_ROW xRowData, DWORD xColumnIndex)
{
    return ((MYSQL_ROW)xRowData)[xColumnIndex];
}

LPBYTE MsMysql::GetBytes(MYSQL_RES* xMySqlRes, MYSQL_ROW xRowData, DWORD xColumnIndex, OUT DWORD& dwSize)
{
    LPDWORD xSqlFieldLen = mysql_fetch_lengths(xMySqlRes);
    dwSize = xSqlFieldLen[xColumnIndex];
    return (LPBYTE)((MYSQL_ROW)xRowData)[xColumnIndex];
}
