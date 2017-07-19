#include "Precompiled.h"
#include <Sddl.h>
#undef GetMessage

MsUnorderedDictionary<DWORD, mstr>* MsBase::m_lpDebugPacketCode = nullptr;

void* __CRTDECL operator new(size_t const size, int const block_use, char const* file_name, int const line_number, bool is)
{
    USE_PARAM(block_use, file_name, line_number);
    for (; is;)
    {
        if (void* const block = _malloc_dbg(size, block_use, file_name, line_number))
        {
            if (block)
            {
                return block;
            }
        }
    }
    AssertNormal(False, "ÄÚ´æ²»×ã");
    return nullptr;
}

mstr CreateStr(mstr xStr, Int32 xCount)
{
    mstr x;
    for (Int32 i = 0; i < xCount; i++)
    {
        x += xStr;
    }
    return x;
}

void AppendStr(mstr& xSrcText, mstr xStr, Int32 xCount)
{
    for (Int32 i = 0; i < xCount; i++)
    {
        xSrcText += xStr;
    }
}

mstr MsBase::MakeDefaultXmlRoot(LPCSTR xRootName, Int32& xOutSize)
{
    FFORMAT(szBuff, "<?xml version=\"1.0\" encoding=\"gb2312\"?>\n<%s>\n</%s>\n", xRootName, xRootName);
    xOutSize = (Int32)szBuff.size();
    return szBuff;
}

void MsBase::ProtoData2Xml(const google::protobuf::Message& xBaseProto, MsXmlReader& xMsXmlReader, Boolean xFillNoHasField)
{
    Int32 xRootSize = 0;
    xMsXmlReader.LoadFromMem(MakeDefaultXmlRoot(xBaseProto.GetTypeName().c_str(), xRootSize).c_str(), xRootSize);
    xMsXmlReader[xBaseProto.GetTypeName().c_str()];
    return _ProtoMsg2Xml(xBaseProto, xMsXmlReader, xFillNoHasField);
}

void MsBase::_ProtoMsg2Xml(const google::protobuf::Message& xBaseProto, MsXmlReader& xMsXmlReader, Boolean xFillNoHasField)
{
    const google::protobuf::Reflection* xReflection = xBaseProto.GetReflection();
    const google::protobuf::Descriptor* xDescriptor = xBaseProto.GetDescriptor();
    for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xField->is_repeated())
        {
            Int32 xRepeatedSize = xReflection->FieldSize(xBaseProto, xField);
            Int32 iRepPosParent, iRepPos, iRepPosChild;
            xMsXmlReader.BackupPos(iRepPosParent, iRepPos, iRepPosChild);
            xMsXmlReader.x_AddElem(xField->name().c_str(), "", MNF_CHILD);
            xMsXmlReader[xField->name().c_str()];
            FFORMAT(szFlag, "repeated[%d]", xRepeatedSize);
            xMsXmlReader.SetAttrib("flag", szFlag.c_str());
            for (int i = 0; i < xRepeatedSize; i++)
            {
                CHAR szValue[1024];
                switch (xField->type())
                {
                    case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                    {
                        const google::protobuf::Message& xMsg = xReflection->GetRepeatedMessage(xBaseProto, xField, i);
                        FFORMAT(szRepName, "repeated_%d", i + 1);
                        Int32 iPosParent, iPos, iPosChild;
                        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
                        xMsXmlReader.x_AddElem(szRepName.c_str(), "", MNF_CHILD);
                        xMsXmlReader[szRepName.c_str()];
                        _ProtoMsg2Xml(xMsg, xMsXmlReader, xFillNoHasField);
                        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
                        continue;
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                    {
                        sprintf_s(szValue, "%lf", xReflection->GetRepeatedDouble(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                    {
                        sprintf_s(szValue, "%f", xReflection->GetRepeatedFloat(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64:
                    {
                        sprintf_s(szValue, "%lld", xReflection->GetRepeatedInt64(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                    {
                        sprintf_s(szValue, "%d", xReflection->GetRepeatedEnum(xBaseProto, xField, i)->number());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED32:
                    {
                        sprintf_s(szValue, "%d", xReflection->GetRepeatedInt32(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                    {
                        if (xReflection->GetRepeatedBool(xBaseProto, xField, i))
                        {
                            sprintf_s(szValue, "%s", "true");
                        }
                        else
                        {
                            sprintf_s(szValue, "%s", "false");
                        }
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
                    case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                    {
                        sprintf_s(szValue, "%s", xReflection->GetRepeatedString(xBaseProto, xField, i).c_str());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                    {
                        sprintf_s(szValue, "%u", xReflection->GetRepeatedUInt32(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                    {
                        sprintf_s(szValue, "%llu", xReflection->GetRepeatedUInt64(xBaseProto, xField, i));
                    }break;
                }

                FFORMAT(szRepName, "repeated_%d", i + 1);
                Int32 iPosParent, iPos, iPosChild;
                xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
                xMsXmlReader.x_AddElem(szRepName.c_str(), "", MNF_CHILD);
                xMsXmlReader[szRepName.c_str()];
                xMsXmlReader.SetAttrib("type", xField->type_name());
                xMsXmlReader.SetAttrib("value", szValue);
                xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
            }
            xMsXmlReader._SetPos(iRepPosParent, iRepPos, iRepPosChild);
        }
        else
        {
            if (xReflection->HasField(xBaseProto, xField))
            {
                CHAR szValue[1024];
                switch (xField->type())
                {
                    case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                    {
                        const google::protobuf::Message& xMsg = xReflection->GetMessage(xBaseProto, xField);
                        Int32 iPosParent, iPos, iPosChild;
                        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
                        xMsXmlReader.x_AddElem(xField->name().c_str(), "", MNF_CHILD);
                        xMsXmlReader[xField->name().c_str()];
                        _ProtoMsg2Xml(xMsg, xMsXmlReader, xFillNoHasField);
                        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
                    }break;
                    case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                    {
                        sprintf_s(szValue, "%lf", xReflection->GetDouble(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                    {
                        sprintf_s(szValue, "%f", xReflection->GetFloat(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64:
                    {
                        sprintf_s(szValue, "%lld", xReflection->GetInt64(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                    {
                        sprintf_s(szValue, "%d", xReflection->GetEnum(xBaseProto, xField)->number());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED32:
                    {
                        sprintf_s(szValue, "%d", xReflection->GetInt32(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                    {
                        if (xReflection->GetBool(xBaseProto, xField))
                        {
                            sprintf_s(szValue, "%s", "true");
                        }
                        else
                        {
                            sprintf_s(szValue, "%s", "false");
                        }
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
                    case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                    {
                        sprintf_s(szValue, "%s", xReflection->GetString(xBaseProto, xField).c_str());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                    {
                        sprintf_s(szValue, "%u", xReflection->GetUInt32(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                    {
                        sprintf_s(szValue, "%llu", xReflection->GetUInt64(xBaseProto, xField));
                    }break;
                }

                Int32 iPosParent, iPos, iPosChild;
                xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
                xMsXmlReader.x_AddElem(xField->name().c_str(), "", MNF_CHILD);
                xMsXmlReader[xField->name().c_str()];

                if (xField->is_required()) { xMsXmlReader.SetAttrib("flag", "required"); }
                else if (xField->is_optional()) { xMsXmlReader.SetAttrib("flag", "optional"); }

                xMsXmlReader.SetAttrib("type", xField->type_name());
                xMsXmlReader.SetAttrib("value", szValue);

                xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
            }
            else if (xFillNoHasField)
            {
                Int32 iPosParent, iPos, iPosChild;
                xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
                xMsXmlReader.x_AddElem(xField->name().c_str(), "", MNF_CHILD);
                xMsXmlReader[xField->name().c_str()];

                if (xField->is_required()) { xMsXmlReader.SetAttrib("flag", "required"); }
                else if (xField->is_optional()) { xMsXmlReader.SetAttrib("flag", "optional"); }

                xMsXmlReader.SetAttrib("type", xField->type_name());
                xMsXmlReader.SetAttrib("value", "--------no data--------");

                xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
            }
        }
    }
}

void MsBase::ShowUD(const universal_data& xUniversalData)
{
    Int32 xRootSize = 0;
    MsXmlReader xMsXmlReader;
    xMsXmlReader.LoadFromMem(MakeDefaultXmlRoot("universal_data", xRootSize).c_str(), xRootSize);
    xMsXmlReader["universal_data"];
    MsBase::UD2Xml(xUniversalData, xMsXmlReader);
    MsVector<mstr> xTemp;
    MsBase::StringSplit(xMsXmlReader.m_strDoc.c_str(), '\n', xTemp);
    FAST_FOREACH(xTemp)
    {
        DebugStr(xTemp.Value().c_str());
    }
}

void MsBase::UD2Xml(const universal_data& xUniversalData, MsXmlReader& xMsXmlReader)
{
    if (xUniversalData.name() == "")
    {
        xMsXmlReader.x_AddElem("no_name", "", MNF_CHILD);
        xMsXmlReader["no_name"];
    }
    else
    {
        xMsXmlReader.x_AddElem(xUniversalData.name().c_str(), "", MNF_CHILD);
        xMsXmlReader[xUniversalData.name().c_str()];
    }

    if (xUniversalData.type() == "") { xMsXmlReader.AddAttrib("type", "no_type"); }
    else { xMsXmlReader.AddAttrib("type", xUniversalData.type().c_str()); }
    xMsXmlReader.AddAttrib("key", MsBase::ToString(xUniversalData.key()).c_str());

    Int32 iPosParent, iPos, iPosChild;
    for (Int32 i = 0; i < xUniversalData.b_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        if (xUniversalData.b(i))
        {
            xMsXmlReader.x_AddElem("b", "true", MNF_CHILD);
        }
        else
        {
            xMsXmlReader.x_AddElem("b", "false", MNF_CHILD);
        }
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.d_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("d", MsBase::ToString(xUniversalData.d(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.f_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("f", MsBase::ToString(xUniversalData.f(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.e_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("e", MsBase::ToString(xUniversalData.e(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.i32_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("i32", MsBase::ToString(xUniversalData.i32(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.i64_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("i64", MsBase::ToString(xUniversalData.i64(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.u32_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("u32", MsBase::ToString(xUniversalData.u32(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.u64_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("u64", MsBase::ToString(xUniversalData.u64(i)).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.str_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        xMsXmlReader.x_AddElem("str", xUniversalData.str(i).c_str(), MNF_CHILD);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }

    for (Int32 i = 0; i < xUniversalData.sub_size(); i++)
    {
        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
        UD2Xml(xUniversalData.sub(i), xMsXmlReader);
        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    }
}

void MsBase::ShowAllField(const google::protobuf::Message& xBaseProto, Boolean xFillNoHasField)
{
    MsXmlReader xMsXmlReader;
    MsBase::ProtoData2Xml(xBaseProto, xMsXmlReader, xFillNoHasField);
    MsVector<mstr> xTemp;
    MsBase::StringSplit(xMsXmlReader.m_strDoc.c_str(), '\n', xTemp);
    FAST_FOREACH(xTemp)
    {
        DebugStr(xTemp.Value().c_str());
    }
}

//void MsBase::ShowNotExistField(const google::protobuf::Message& xBaseProto)
//{
//    MsList<mstr> xFieldFullName;
//    MsBase::GetNotExistField(xFieldFullName, xBaseProto, 0);
//    FAST_FOREACH(xFieldFullName)
//    {
//        DebugStr(xEnumValue.c_str());
//    }
//}
//
//void MsBase::GetNotExistField(MsList<mstr>& xFieldFullName, const google::protobuf::Message& xBaseProto, Int32 xLevelCount)
//{
//    mstr xTempFlag;
//    AppendStr(xTempFlag, "    ", xLevelCount);
//    if (xLevelCount == 0)
//    {
//        xFieldFullName.Add(xBaseProto.GetTypeName() + ":{\n");
//    }
//    const google::protobuf::Reflection* xReflection = xBaseProto.GetReflection();
//    const google::protobuf::Descriptor* xDescriptor = xBaseProto.GetDescriptor();
//    for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
//    {
//        const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
//        if (xField->is_repeated())
//        {
//            MsList<mstr> xTempFieldFullName;
//            Int32 xRepeatedSize = xReflection->FieldSize(xBaseProto, xField);
//            CHAR szBuff[100];
//            sprintf_s(szBuff, "%d", xRepeatedSize);
//            xTempFieldFullName.Add(xTempFlag + "  repeated:  (" + szBuff + ")" + xField->name() + "\n");
//            if (xField->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
//            {
//                for (int i = 0; i < xRepeatedSize; i++)
//                {
//                    const google::protobuf::Message& xMsg = xReflection->GetRepeatedMessage(xBaseProto, xField, i);
//
//                    GetNotExistField(xTempFieldFullName, xMsg, xLevelCount + 1);
//                    if (xTempFieldFullName.GetCount() > 1)
//                    {
//                        xFieldFullName.Add(xField->full_name() + ":{\n");
//                        FAST_FOREACH(xTempFieldFullName)
//                        {
//                            xFieldFullName.Add(xEnumValue);
//                        }
//                        xFieldFullName.Add(xTempFlag + "  }\n");
//                    }
//                }
//            }
//        }
//        else if (!xReflection->HasField(xBaseProto, xField))
//        {
//            if (xField->is_required())
//            {
//                xFieldFullName.Add(xTempFlag + "  required:  " + xField->name() + "\n");
//            }
//            else if (xField->is_optional())
//            {
//                //xFieldFullName.Add(xTempFlag + "  optional:  " + xField->name() + "\n");
//            }
//        }
//        else
//        {
//            if (xField->type() == google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE)
//            {
//                const google::protobuf::Message& xMsg = xReflection->GetMessage(xBaseProto, xField);
//
//                MsList<mstr> xTempFieldFullName;
//                GetNotExistField(xTempFieldFullName, xMsg, xLevelCount + 1);
//                if (xTempFieldFullName.GetCount() > 0)
//                {
//                    xFieldFullName.Add(xField->full_name() + ":{\n");
//                    FAST_FOREACH(xTempFieldFullName)
//                    {
//                        xFieldFullName.Add(xEnumValue);
//                    }
//                    xFieldFullName.Add(xTempFlag + "  }\n");
//                }
//            }
//        }
//    }
//    if (xLevelCount == 0) { xFieldFullName.Add("}\n"); }
//}
//
//void MsBase::ProtoLogNotExistField(const google::protobuf::Message& xBaseProto)
//{
//    MsList<mstr> xFieldFullName;
//    MsBase::GetNotExistField(xFieldFullName, xBaseProto, 0);
//    mstr xTempLog;
//    FAST_FOREACH(xFieldFullName)
//    {
//        xTempLog += xEnumValue;
//    }
//    if (xFieldFullName.GetCount() > 0)
//    {
//        AssertNormal(False, "PROTO´æÔÚÎ´Ìî³ä×Ö¶Î:[%s]", xTempLog.c_str());
//    }
//}

google::protobuf::FieldDescriptor::Type MsBase::ConformityProtoType(google::protobuf::FieldDescriptor::Type xType)
{
    switch (xType)
    {
        case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
        case google::protobuf::FieldDescriptor::Type::TYPE_SINT32:
        case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32:
        case google::protobuf::FieldDescriptor::Type::TYPE_FIXED32:
        {
            return google::protobuf::FieldDescriptor::Type::TYPE_INT32;
        }break;

        case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
        case google::protobuf::FieldDescriptor::Type::TYPE_SINT64:
        case google::protobuf::FieldDescriptor::Type::TYPE_FIXED64:
        case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64:
        {
            return google::protobuf::FieldDescriptor::Type::TYPE_INT64;
        }break;

        case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
        case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
        {
            return google::protobuf::FieldDescriptor::Type::TYPE_STRING;
        }break;

        default:
        {
            return xType;
        }
    }
}

mstr MsBase::ConformityProtoTypeName(const google::protobuf::FieldDescriptor* xField)
{
    switch (xField->type())
    {
        case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
        case google::protobuf::FieldDescriptor::Type::TYPE_SINT32:
        case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32:
        case google::protobuf::FieldDescriptor::Type::TYPE_FIXED32:
        {
            return "int32";
        }break;

        case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
        case google::protobuf::FieldDescriptor::Type::TYPE_SINT64:
        case google::protobuf::FieldDescriptor::Type::TYPE_FIXED64:
        case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64:
        {
            return "int64";
        }break;

        case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
        case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
        {
            return"string";
        }break;

        default:
        {
            return xField->type_name();
        }break;
    }
}


Boolean MsBase::IsEmptyUD(const universal_data& xUniversalData)
{
    MsVector<mstr> xTemp;
    MsBase::StringSplit(xUniversalData.name().c_str(), '@', xTemp);
    if (xTemp.GetCount() >= 2 && xTemp[0] == "group") { return False; }
    return xUniversalData.b_size() == 0 &&
        xUniversalData.f_size() == 0 &&
        xUniversalData.d_size() == 0 &&
        xUniversalData.e_size() == 0 &&
        xUniversalData.i32_size() == 0 &&
        xUniversalData.i64_size() == 0 &&
        xUniversalData.u32_size() == 0 &&
        xUniversalData.u64_size() == 0 &&
        xUniversalData.str_size() == 0 &&
        xUniversalData.sub_size() == 0;
}

void MsBase::UD2Proto(const universal_data& xUniversalData, google::protobuf::Message* xBaseProto)
{
    const google::protobuf::Reflection* xReflection = xBaseProto->GetReflection();
    const google::protobuf::Descriptor* xDescriptor = xBaseProto->GetDescriptor();

    MsUnorderedDictionary<google::protobuf::FieldDescriptor::Type, MsUnorderedDictionary<Int32, Int32>> xTempDict;
    MsUnorderedDictionary<google::protobuf::FieldDescriptor::Type, Int32> xTempNum;
    for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);

        google::protobuf::FieldDescriptor::Type xType = MsBase::ConformityProtoType(xField->type());
        if (xField->is_repeated())
        {
            xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE][xTempNum[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE]] = xField->number();
            xTempNum[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE]++;
        }
        else
        {
            xTempDict[xType][xTempNum[xType]] = xField->number();
            xTempNum[xType]++;
        }
    }

    for (Int32 i = 0; i < xUniversalData.b_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_BOOL][i]);
        xReflection->SetBool(xBaseProto, xField, xUniversalData.b(i));
    }

    for (Int32 i = 0; i < xUniversalData.f_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_FLOAT][i]);
        xReflection->SetFloat(xBaseProto, xField, xUniversalData.f(i));
    }

    for (Int32 i = 0; i < xUniversalData.d_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE][i]);
        xReflection->SetDouble(xBaseProto, xField, xUniversalData.d(i));
    }

    for (Int32 i = 0; i < xUniversalData.e_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_ENUM][i]);
        const google::protobuf::EnumDescriptor* lpEnumDescriptor = google::protobuf::DescriptorPool::generated_pool()->FindEnumTypeByName(xField->enum_type()->name());
        for (Int32 xEnumId = 0; xEnumId < lpEnumDescriptor->value_count(); xEnumId++)
        {
            const google::protobuf::EnumValueDescriptor* lpEnumValueDescriptor = lpEnumDescriptor->value(xEnumId);
            if (lpEnumValueDescriptor && lpEnumValueDescriptor->number() == xUniversalData.e(i))
            {
                xReflection->SetEnum(xBaseProto, xField, lpEnumValueDescriptor);
                break;
            }
        }
    }

    for (Int32 i = 0; i < xUniversalData.i32_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_INT32][i]);
        if (xField)
        {
            xReflection->SetInt32(xBaseProto, xField, xUniversalData.i32(i));
        }
        else
        {
            AssertLog("this is nullptr");
        }
    }

    for (Int32 i = 0; i < xUniversalData.i64_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_INT64][i]);
        xReflection->SetInt64(xBaseProto, xField, xUniversalData.i64(i));
    }

    for (Int32 i = 0; i < xUniversalData.u32_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_UINT32][i]);
        xReflection->SetUInt32(xBaseProto, xField, xUniversalData.u32(i));
    }

    for (Int32 i = 0; i < xUniversalData.u64_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_UINT64][i]);
        xReflection->SetUInt64(xBaseProto, xField, xUniversalData.u64(i));
    }

    for (Int32 i = 0; i < xUniversalData.str_size(); i++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_STRING][i]);
        xReflection->SetString(xBaseProto, xField, xUniversalData.str(i));
    }

    for (Int32 i = 0; i < xUniversalData.sub_size(); i++)
    {
        if (xTempDict.ContainsKey(google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE))
        {
            if (xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE].ContainsKey(i))
            {
                const google::protobuf::FieldDescriptor* xField = xDescriptor->FindFieldByNumber(xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE][i]);
                if (xField->is_repeated())
                {
                    MsVector<mstr> xTemp;
                    MsBase::StringSplit(xUniversalData.sub(i).name().c_str(), '@', xTemp);
                    if (xTemp.GetCount() >= 2 && xTemp[0] == "group")
                    {
                        const universal_data& xGroup = xUniversalData.sub(i);
                        if (!IsEmptyUD(xGroup))
                        {
                            switch (MsBase::ConformityProtoType(xField->type()))
                            {
                                case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.sub_size(); xRepeatedIndex++)
                                    {
                                        auto& xSub = xGroup.sub(xRepeatedIndex);
                                        if (!IsEmptyUD(xSub))
                                        {
                                            MsBase::UD2Proto(xSub, xReflection->AddMessage(xBaseProto, xField));
                                        }
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.d_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddDouble(xBaseProto, xField, xGroup.d(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.f_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddFloat(xBaseProto, xField, xGroup.f(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.i64_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddInt64(xBaseProto, xField, xGroup.i64(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.e_size(); xRepeatedIndex++)
                                    {
                                        const google::protobuf::EnumDescriptor* lpEnumDescriptor = google::protobuf::DescriptorPool::generated_pool()->FindEnumTypeByName(xField->enum_type()->name());
                                        for (Int32 xEnumId = 0; xEnumId < lpEnumDescriptor->value_count(); xEnumId++)
                                        {
                                            const google::protobuf::EnumValueDescriptor* lpEnumValueDescriptor = lpEnumDescriptor->value(xEnumId);
                                            if (lpEnumValueDescriptor && lpEnumValueDescriptor->number() == xGroup.e(xRepeatedIndex))
                                            {
                                                xReflection->AddEnum(xBaseProto, xField, lpEnumValueDescriptor);
                                                break;
                                            }
                                        }
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.i32_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddInt32(xBaseProto, xField, xGroup.i32(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.b_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddBool(xBaseProto, xField, xGroup.b(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.str_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddString(xBaseProto, xField, xGroup.str(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.u32_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddUInt32(xBaseProto, xField, xGroup.u32(xRepeatedIndex));
                                    }
                                }break;

                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                                {
                                    for (Int32 xRepeatedIndex = 0; xRepeatedIndex < xGroup.u64_size(); xRepeatedIndex++)
                                    {
                                        xReflection->AddUInt64(xBaseProto, xField, xGroup.u64(xRepeatedIndex));
                                    }
                                }break;
                            }
                        }
                    }
                }
                else
                {
                    if (!IsEmptyUD(xUniversalData.sub(i)))
                    {
                        MsBase::UD2Proto(xUniversalData.sub(i), xReflection->MutableMessage(xBaseProto, xField));
                    }
                    else
                    {
                        AssertLog("IsEmptyUD");
                    }
                }
            }
        }
    }
}

void MsBase::Proto2UD(const google::protobuf::Message& xBaseProto, mstr xName, universal_data* xUniversalData)
{
    const google::protobuf::Reflection* xReflection = xBaseProto.GetReflection();
    const google::protobuf::Descriptor* xDescriptor = xBaseProto.GetDescriptor();
    xUniversalData->set_type(xBaseProto.GetTypeName());
    xUniversalData->set_name(xName);

    MsUnorderedDictionary<google::protobuf::FieldDescriptor::Type, MsUnorderedDictionary<const google::protobuf::FieldDescriptor*, Int32>> xTempDict;
    MsUnorderedDictionary<google::protobuf::FieldDescriptor::Type, Int32> xTempNum;
    for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        if (xField->is_repeated())
        {
            xTempDict[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE][xField] = xTempNum[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE];
            xTempNum[google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE]++;
        }
        else
        {
            google::protobuf::FieldDescriptor::Type xType = MsBase::ConformityProtoType(xField->type());
            xTempDict[xType][xField] = xTempNum[xType];
            xTempNum[xType]++;
        }
    }

    for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
    {
        const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        Int32 xFieldId = xTempDict[MsBase::ConformityProtoType(xField->type())][xField];

        if (xField->is_repeated())
        {
            mstr xGroupType = "group@";
            mstr xGroupName = "group@";
            xGroupType += MsBase::ConformityProtoTypeName(xField);

            Int32 xRepeatedSize = xReflection->FieldSize(xBaseProto, xField);

            while (xUniversalData->sub_size() <= xFieldId) { xUniversalData->add_sub(); }
            universal_data* xSub = xUniversalData->mutable_sub(xFieldId);
            xSub->set_type(xGroupType);
            xSub->set_name(xGroupName + xField->full_name());
            for (int i = 0; i < xRepeatedSize; i++)
            {
                switch (MsBase::ConformityProtoType(xField->type()))
                {
                    case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                    {
                        const google::protobuf::Message& xMsg = xReflection->GetRepeatedMessage(xBaseProto, xField, i);
                        MsBase::Proto2UD(xMsg, xField->full_name(), xSub->add_sub());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                    {
                        xSub->add_d(xReflection->GetRepeatedDouble(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                    {
                        xSub->add_f(xReflection->GetRepeatedFloat(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                    {
                        xSub->add_i64(xReflection->GetRepeatedInt64(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                    {
                        xSub->add_e(xReflection->GetRepeatedEnum(xBaseProto, xField, i)->number());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                    {
                        xSub->add_i32(xReflection->GetRepeatedInt32(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                    {
                        xSub->add_b(xReflection->GetRepeatedBool(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                    {
                        xSub->add_str(xReflection->GetRepeatedString(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                    {
                        xSub->add_u32(xReflection->GetRepeatedUInt32(xBaseProto, xField, i));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                    {
                        xSub->add_u64(xReflection->GetRepeatedUInt64(xBaseProto, xField, i));
                    }break;
                }
            }
        }
        else
        {
            if (xReflection->HasField(xBaseProto, xField))
            {
                switch (xField->type())
                {
                    case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                    {
                        while (xUniversalData->sub_size() <= xFieldId) { xUniversalData->add_sub(); }
                        const google::protobuf::Message& xMsg = xReflection->GetMessage(xBaseProto, xField);
                        MsBase::Proto2UD(xMsg, xField->full_name(), xUniversalData->mutable_sub(xFieldId));
                    }break;
                    case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                    {
                        while (xUniversalData->d_size() <= xFieldId) { xUniversalData->add_d(0.0); }
                        xUniversalData->set_d(xFieldId, xReflection->GetDouble(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                    {
                        while (xUniversalData->f_size() <= xFieldId) { xUniversalData->add_f(0.0f); }
                        xUniversalData->set_f(xFieldId, xReflection->GetFloat(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED64:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED64:
                    {
                        while (xUniversalData->i64_size() <= xFieldId) { xUniversalData->add_i64(0i64); }
                        xUniversalData->set_i64(xFieldId, xReflection->GetInt64(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                    {
                        while (xUniversalData->e_size() <= xFieldId) { xUniversalData->add_e(0); }
                        xUniversalData->set_e(xFieldId, xReflection->GetEnum(xBaseProto, xField)->number());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SINT32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_SFIXED32:
                    case google::protobuf::FieldDescriptor::Type::TYPE_FIXED32:
                    {
                        while (xUniversalData->i32_size() <= xFieldId) { xUniversalData->add_i32(0); }
                        xUniversalData->set_i32(xFieldId, xReflection->GetInt32(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                    {
                        while (xUniversalData->b_size() <= xFieldId) { xUniversalData->add_b(false); }
                        if (xReflection->GetBool(xBaseProto, xField))
                        {
                            xUniversalData->set_b(xFieldId, true);
                        }
                        else
                        {
                            xUniversalData->set_b(xFieldId, false);
                        }
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
                    case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                    {
                        while (xUniversalData->str_size() <= xFieldId) { xUniversalData->add_str(""); }
                        xUniversalData->set_str(xFieldId, xReflection->GetString(xBaseProto, xField).c_str());
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                    {
                        while (xUniversalData->u32_size() <= xFieldId) { xUniversalData->add_u32(0); }
                        xUniversalData->set_u32(xFieldId, xReflection->GetUInt32(xBaseProto, xField));
                    }break;

                    case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                    {
                        while (xUniversalData->u64_size() <= xFieldId) { xUniversalData->add_u64(0i64); }
                        xUniversalData->set_u64(xFieldId, xReflection->GetUInt64(xBaseProto, xField));
                    }break;
                }
            }
        }
    }
}

mstr MsBase::GetUserSID()
{
    mstr xUserSID = "";
    Boolean bRet = True;
    HANDLE hToken = nullptr;
    LPBYTE xBuffer = nullptr;
    LPSTR pszSID = nullptr;
    do
    {
        if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken))
        {
            bRet = False;
            break;
        }
        DWORD dwBufferSize = 0;
        if (!::GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize) && (GetLastError() != ERROR_INSUFFICIENT_BUFFER))
        {
            bRet = False;
            break;
        }
        xBuffer = NEW BYTE[dwBufferSize];
        SecureZeroMemory(xBuffer, dwBufferSize * sizeof(BYTE));
        PTOKEN_USER pTokenUser = (PTOKEN_USER)xBuffer;
        if (!::GetTokenInformation(hToken, TokenUser, pTokenUser, dwBufferSize, &dwBufferSize))
        {
            bRet = False;
            break;
        }
        if (!::IsValidSid(pTokenUser->User.Sid))
        {
            bRet = False;
            break;
        }
        PSID pSid = pTokenUser->User.Sid;
        if (!::ConvertSidToStringSidA(pSid, &pszSID))
        {
            bRet = False;
            break;
        }
        xUserSID = pszSID;
    } while (False);

    SAFE_DELETE_ARRAY(xBuffer);
    if (pszSID) { ::LocalFree(pszSID); pszSID = nullptr; }
    if (hToken) { ::CloseHandle(hToken); hToken = nullptr; }
    return xUserSID;
}

mstr MsBase::CI2SI(Int64 xClusterIndex)
{
    Int64 xNodeIndex = xClusterIndex / MAX_UNIT_INDEX;
    Int64 xIndex = xClusterIndex % MAX_UNIT_INDEX;
    Int64 xTempIndex = xNodeIndex * MAX_SHOW_UNIT_INDEX + xIndex;
    CHAR szBuff[100];
    _ui64toa_s(xTempIndex, szBuff, CountOf(szBuff), 36);
    _strupr_s(szBuff, CountOf(szBuff));
    switch (strlen(szBuff))
    {
        case 0: return "000000000" + mstr(szBuff);
        case 1: return "00000000" + mstr(szBuff);
        case 2: return "0000000" + mstr(szBuff);
        case 3: return "000000" + mstr(szBuff);
        case 4: return "00000" + mstr(szBuff);
        case 5: return "0000" + mstr(szBuff);
        case 6: return "000" + mstr(szBuff);
        case 7: return "00" + mstr(szBuff);
        case 8: return "0" + mstr(szBuff);
        default: return "";
    }
}

Int64 MsBase::SI2CI(mstr xShowIndex, Int32 xBeginPos)
{
    CHAR szBuff[100];
    Int32 xSize = (Int32)(xShowIndex.size() - xBeginPos);
    memcpy(szBuff, xShowIndex.data() + xBeginPos, xSize);
    szBuff[xSize] = 0;

    LPSTR xBegin = (LPSTR)(xShowIndex.data() + xBeginPos);
    LPSTR xEnd = xBegin + 4;
    CHAR xBak = *(xBegin + 4); *(xBegin + 4) = 0;
    Int64 xNodeIndex = strtoll(xBegin, &xEnd, 36);
    *(xBegin + 4) = xBak;

    xBegin += 4; xEnd += 5;
    Int64 xIndex = strtoll(xBegin, (LPSTR*)(&xEnd), 36);

    Int64 xClusterIndex = (xNodeIndex * MAX_UNIT_INDEX) + xIndex;
    return xClusterIndex;
}

MsVector<mstr> MsBase::GetMacAddr()
{
    MsVector<mstr> xVecMacAddr;
    ULONG unAddrBuffSize = 0;

    ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST, nullptr, nullptr, &unAddrBuffSize);
    SafePtr<BYTE> xBuff(NEW BYTE[unAddrBuffSize], [](LPBYTE xData) { SAFE_DELETE_ARRAY(xData); });
    PIP_ADAPTER_ADDRESSES pAdapterSet = (PIP_ADAPTER_ADDRESSES)xBuff.get();
    ZeroMemory(pAdapterSet, unAddrBuffSize);

    if (ERROR_SUCCESS == ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST, nullptr, pAdapterSet, &unAddrBuffSize))
    {
        mstr sTmp;
        PIP_ADAPTER_ADDRESSES pAdapter = pAdapterSet;
        while (pAdapter)
        {
            sTmp = pAdapter->AdapterName;
            mstr xTemp = MsBase::Format("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                pAdapter->PhysicalAddress[0], pAdapter->PhysicalAddress[1],
                pAdapter->PhysicalAddress[2], pAdapter->PhysicalAddress[3],
                pAdapter->PhysicalAddress[4], pAdapter->PhysicalAddress[5]);
            if (xTemp != "00-00-00-00-00-00")
            {
                xVecMacAddr.Add(xTemp);
            }
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        Int32 x = ::GetLastError();
    }
    return xVecMacAddr;
}