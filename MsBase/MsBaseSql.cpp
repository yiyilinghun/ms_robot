#include "Precompiled.h"
using namespace google::protobuf;
typedef FieldDescriptor::Type ProtoType;

MsBaseSql::MsBaseSql(LPCSTR szDBAddr, LPCSTR szDBUser, LPCSTR szDBPassword, LPCSTR szDBName)
    : m_DBAddr(szDBAddr)
    , m_DBUser(szDBUser)
    , m_DBPassword(szDBPassword)
    , m_DBName(szDBName)
    , m_ConfigFileName("")
    , m_DefaultVarCharLength(128)
    , m_DefaultVarByteLength(128)
    , m_Charset("gbk")
{
    ;
}

MsBaseSql::MsBaseSql(LPCSTR xConfigFileName)
    : m_ConfigFileName(xConfigFileName)
    , m_DefaultVarCharLength(128)
    , m_DefaultVarByteLength(128)
    , m_Charset("gbk")
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

MsBaseSql::~MsBaseSql(void)
{
    ;
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

void MsBaseSql::P2BS(const LPBASEPROTO xProto, mstr& xOut)
{
    return MsBaseSql::P2BS(*xProto, xOut);
}

mstr MsBaseSql::P2BS(const LPBASEPROTO xProto)
{
    return MsBaseSql::P2BS(*xProto);
}

void MsBaseSql::P2BS(const BaseProto& xProto, mstr& xOut)
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
    MsBaseSql::B2S(xProtoData, lpStrData + 2, xProtoLen);
    lpStrData[0] = '0'; lpStrData[1] = 'x'; xOut = lpStrData;
    SAFE_DELETE_ARRAY(lpStrData);
    SAFE_DELETE_ARRAY(xProtoData);
}

mstr MsBaseSql::P2BS(const BaseProto& xProto)
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
    MsBaseSql::B2S(xProtoData, lpStrData + 2, xProtoLen);
    lpStrData[0] = '0'; lpStrData[1] = 'x'; xStrData = lpStrData;
    SAFE_DELETE_ARRAY(lpStrData);
    SAFE_DELETE_ARRAY(xProtoData);
    return xStrData;
}

void MsBaseSql::B2S(LPBYTE buf, OUT LPSTR szStr, Int32 len)
{
    for (Int32 i = 0; i < len; i++)
    {
        ((LPWORD)szStr)[i] = *(LPWORD)ByteChar[buf[i]];
    }
    szStr[len * 2] = 0;
}

mstr MsBaseSql::GetExistsTableSqlByProto(LPBASEPROTO xProto)
{
    CHAR szTemp[MAX_SQL_STR_LNE];
    sprintf_s(szTemp
        , "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='%s' AND TABLE_NAME='%s';"
        , m_DBName.c_str()
        , xProto->GetTypeName().c_str()
        );
    return szTemp;
}

mstr MsBaseSql::GetCreateSqlByProto(LPBASEPROTO xProto)
{
    mstr xCreateSql = "CREATE TABLE IF NOT EXISTS `" + xProto->GetTypeName() + "` (";
    CHAR szTemp[MAX_SQL_STR_LNE];

    const Descriptor* xDescriptor = xProto->GetDescriptor();
    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xFieldIndex > 0) { xCreateSql += ','; }
        switch (xField->type())
        {
            case ProtoType::TYPE_FLOAT:
            {
                xCreateSql += " `" + xField->name() + "` FLOAT NOT nullptr ";
            }break;

            case ProtoType::TYPE_DOUBLE:
            {
                xCreateSql += " `" + xField->name() + "` DOUBLE NOT nullptr ";
            }break;

            case ProtoType::TYPE_INT64:
            case ProtoType::TYPE_SINT64:
            case ProtoType::TYPE_FIXED64:
            case ProtoType::TYPE_SFIXED64:
            case ProtoType::TYPE_UINT64:
            {
                xCreateSql += " `" + xField->name() + "` BIGINT NOT nullptr ";
            }break;

            case ProtoType::TYPE_INT32:
            case ProtoType::TYPE_SINT32:
            case ProtoType::TYPE_FIXED32:
            case ProtoType::TYPE_SFIXED32:
            case ProtoType::TYPE_UINT32:
            {
                xCreateSql += " `" + xField->name() + "` INT NOT nullptr ";
            }break;

            case ProtoType::TYPE_ENUM:
            {
                xCreateSql += " `" + xField->name() + "` ENUM(";
                const google::protobuf::EnumDescriptor* lpEnumDescriptor = google::protobuf::DescriptorPool::generated_pool()->FindEnumTypeByName(xField->enum_type()->name());
                Int32 xEnumValueCount = lpEnumDescriptor->value_count();
                for (Int32 xEnumValueIndex = 0; xEnumValueIndex < xEnumValueCount; xEnumValueIndex++)
                {
                    const google::protobuf::EnumValueDescriptor* lpEnumValueDescriptor = lpEnumDescriptor->FindValueByNumber(xEnumValueIndex);
                    if (lpEnumValueDescriptor)
                    {
                        sprintf_s(szTemp, "'%d'", lpEnumValueDescriptor->number());
                        if (xEnumValueIndex > 0) { xCreateSql += ','; }
                        xCreateSql += szTemp;
                    }
                }
                xCreateSql += ") NOT nullptr ";
            }break;

            case ProtoType::TYPE_BOOL:
            {
                xCreateSql += " `" + xField->name() + "` BOOL NOT nullptr ";
            }break;

            case ProtoType::TYPE_STRING:
            {
                sprintf_s(szTemp, "` VARCHAR(%d) NOT nullptr ", m_DefaultVarCharLength);
                xCreateSql += " `" + xField->name() + szTemp;
            }break;

            case ProtoType::TYPE_BYTES:
            {
                sprintf_s(szTemp, "` VARBINARY(%d) NOT nullptr ", m_DefaultVarByteLength);
                xCreateSql += " `" + xField->name() + szTemp;
            }break;

            default: {; }
        }
    }
    if (m_PrimaryFieldName != "")
    {
        sprintf_s(szTemp, ", PRIMARY KEY(`%s`)", m_PrimaryFieldName.c_str());
        xCreateSql += szTemp;
    }
    BEGIN_ENUMERATOR(m_UniqueField)
    {
        sprintf_s(szTemp, ", UNIQUE KEY `UNIQUE_%s_%s` (`%s`)", xProto->GetTypeName().c_str(), xEnumValue.c_str(), xEnumValue.c_str());
        xCreateSql += szTemp;
    }END_ENUMERATOR;

    sprintf_s(szTemp, ") ENGINE=INNODB DEFAULT CHARSET=%s;", m_Charset.c_str());
    xCreateSql += szTemp;

    return xCreateSql;
}

mstr MsBaseSql::GetDropSqlByProto(LPBASEPROTO xProto)
{
    CHAR szTemp[MAX_SQL_STR_LNE];
    sprintf_s(szTemp, "DROP TABLE `%s`;", xProto->GetTypeName().c_str());
    return szTemp;
}

mstr MsBaseSql::GetInsertSqlByProto(LPBASEPROTO xProto)
{
    mstr xInsertSql = "INSERT INTO `" + xProto->GetTypeName() + "` (";
    CHAR szTemp[MAX_SQL_STR_LNE];

    const Reflection* xReflection = xProto->GetReflection();
    const Descriptor* xDescriptor = xProto->GetDescriptor();

    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xFieldIndex > 0) { xInsertSql += ", "; }
        xInsertSql += '`' + xField->name() + '`';
    }

    xInsertSql += ")VALUES(";

    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xFieldIndex > 0) { xInsertSql += ','; }
        switch (xField->type())
        {
            case ProtoType::TYPE_FLOAT:
            {
                sprintf_s(szTemp, "%f", xReflection->GetFloat(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_DOUBLE:
            {
                sprintf_s(szTemp, "%lf", xReflection->GetDouble(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_INT64:
            case ProtoType::TYPE_SINT64:
            case ProtoType::TYPE_FIXED64:
            case ProtoType::TYPE_SFIXED64:
            {
                sprintf_s(szTemp, "%lld", xReflection->GetInt64(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_UINT64:
            {
                sprintf_s(szTemp, "%llud", xReflection->GetUInt64(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_INT32:
            case ProtoType::TYPE_SINT32:
            case ProtoType::TYPE_FIXED32:
            case ProtoType::TYPE_SFIXED32:
            {
                sprintf_s(szTemp, "%d", xReflection->GetInt32(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_UINT32:
            {
                sprintf_s(szTemp, "%ud", xReflection->GetUInt32(*xProto, xField));
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_ENUM:
            {
                sprintf_s(szTemp, "%d", xReflection->GetEnum(*xProto, xField)->number());
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_BOOL:
            {
                if (xReflection->GetBool(*xProto, xField))
                {
                    xInsertSql += "TRUE";
                }
                else
                {
                    xInsertSql += "FALSE";
                }
            }break;

            case ProtoType::TYPE_STRING:
            {
                sprintf_s(szTemp, "'%s'", xReflection->GetString(*xProto, xField).c_str());
                xInsertSql += szTemp;
            }break;

            case ProtoType::TYPE_BYTES:
            {
                mstr xBinary = xReflection->GetString(*xProto, xField);
                LPSTR xStrBinary = NEW CHAR[xBinary.size() * 2 + 1];
                B2S((LPBYTE)xBinary.data(), xStrBinary, (Int32)xBinary.size());
                xInsertSql += "0x";
                xInsertSql += xStrBinary;
                SAFE_DELETE_ARRAY(xStrBinary);
            }break;

            default: {; }
        }
    }
    xInsertSql += ");";
    return xInsertSql;
}

mstr MsBaseSql::GetSelectSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence)
{
    mstr xSelectSql = "SELECT ";
    const Descriptor* xDescriptor = xProto->GetDescriptor();

    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xFieldIndex > 0) { xSelectSql += ", "; }
        xSelectSql += '`' + xField->name() + '`';
    }
    xSelectSql += "FROM ";
    xSelectSql += '`' + xProto->GetTypeName() + '`';
    if (xWhereSentence != "")
    {
        xSelectSql += "WHERE ";
        xSelectSql += xWhereSentence;
    }
    xSelectSql += ';';
    return xSelectSql;
}

mstr MsBaseSql::GetUpdateSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence)
{
    CHAR szTemp[MAX_SQL_STR_LNE];

    mstr xUpdateSql = "UPDATE ";
    xUpdateSql += '`' + xProto->GetTypeName() + '`';
    xUpdateSql += "SET ";

    const Reflection* xReflection = xProto->GetReflection();
    const Descriptor* xDescriptor = xProto->GetDescriptor();
    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xFieldIndex > 0) { xUpdateSql += ','; }
        switch (xField->type())
        {
            case ProtoType::TYPE_FLOAT:
            {
                sprintf_s(szTemp, "`%s` = %f", xField->name().c_str(), xReflection->GetFloat(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_DOUBLE:
            {
                sprintf_s(szTemp, "`%s` = %lf", xField->name().c_str(), xReflection->GetDouble(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_INT64:
            case ProtoType::TYPE_SINT64:
            case ProtoType::TYPE_FIXED64:
            case ProtoType::TYPE_SFIXED64:
            {
                sprintf_s(szTemp, "`%s` = %lld", xField->name().c_str(), xReflection->GetInt64(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_UINT64:
            {
                sprintf_s(szTemp, "`%s` = %llud", xField->name().c_str(), xReflection->GetUInt64(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_INT32:
            case ProtoType::TYPE_SINT32:
            case ProtoType::TYPE_FIXED32:
            case ProtoType::TYPE_SFIXED32:
            {
                sprintf_s(szTemp, "`%s` = %d", xField->name().c_str(), xReflection->GetInt32(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_UINT32:
            {
                sprintf_s(szTemp, "`%s` = %ud", xField->name().c_str(), xReflection->GetUInt32(*xProto, xField));
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_ENUM:
            {
                sprintf_s(szTemp, "`%s` = %d", xField->name().c_str(), xReflection->GetEnum(*xProto, xField)->number());
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_BOOL:
            {
                if (xReflection->GetBool(*xProto, xField))
                {
                    sprintf_s(szTemp, "`%s` = TRUE", xField->name().c_str());
                    xUpdateSql += szTemp;
                }
                else
                {
                    sprintf_s(szTemp, "`%s` = FALSE", xField->name().c_str());
                    xUpdateSql += szTemp;
                }
            }break;

            case ProtoType::TYPE_STRING:
            {
                sprintf_s(szTemp, "`%s` = '%s'", xField->name().c_str(), xReflection->GetString(*xProto, xField).c_str());
                xUpdateSql += szTemp;
            }break;

            case ProtoType::TYPE_BYTES:
            {
                mstr xBinary = xReflection->GetString(*xProto, xField);
                LPSTR xStrBinary = NEW CHAR[xBinary.size() * 2 + 1];
                B2S((LPBYTE)xBinary.data(), xStrBinary, (Int32)xBinary.size());
                sprintf_s(szTemp, "`%s` = 0x%s", xField->name().c_str(), xStrBinary);
                xUpdateSql += szTemp;
                SAFE_DELETE_ARRAY(xStrBinary);
            }break;

            default: {; }
        }
    }
    if (xWhereSentence != "")
    {
        xUpdateSql += " WHERE ";
        xUpdateSql += xWhereSentence;
    }
    xUpdateSql += ';';
    return xUpdateSql;
}

mstr MsBaseSql::GetDeleteSqlByProto(LPBASEPROTO xProto, mstr xWhereSentence)
{
    mstr xDeleteSql = "DELETE FROM `" + xProto->GetTypeName() + "`";
    if (xWhereSentence != "")
    {
        xDeleteSql += " WHERE ";
        xDeleteSql += xWhereSentence;
    }
    return xDeleteSql;
}
