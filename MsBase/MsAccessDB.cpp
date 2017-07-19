#include "Precompiled.h"
//
//MsAccessDB::MsAccessDB(mstr xDBFileName)
//{
//    ::CoInitialize(nullptr);
//    try
//    {
//        //HRESULT hr;
//        if (SUCCEEDED(m_Connection.CreateInstance("ADODB.Connection")))
//        {
//            CHAR szBuff[KB_SIZE];
//            sprintf_s(szBuff, "Provider=Microsoft.ACE.OLEDB.14.0;Data Source=%s;", xDBFileName.c_str());
//            if (SUCCEEDED(m_Connection->Open(szBuff, "", "", adModeUnknown)))
//            //if (SUCCEEDED(m_Connection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=ServerConfigDB.accdb", "", "", adModeUnknown)))
//
//            {
//                AssertLog("打开数据库[%s]成功", xDBFileName.c_str());
//            }
//            else
//            {
//                AssertLog("打开数据库[%s]失败", xDBFileName.c_str());
//            }
//        }
//    }
//
//    // 捕捉异常
//    catch (_com_error e)
//    {
//        AssertLog("打开数据库[%s]失败[%s]", xDBFileName.c_str(), MsBaseDef::W2M(e.ErrorMessage()).c_str());
//    }
//}
//
//MsAccessDB::~MsAccessDB()
//{
//    if (m_Connection->State)
//    {
//        m_Recordset->Close();
//        m_Recordset = nullptr;
//        m_Connection->Close();
//        m_Connection = nullptr;
//    }
//    ::CoUninitialize();
//}
//
//Boolean MsAccessDB::ExeSql(mstr xSql)
//{
//    m_Recordset.CreateInstance(__uuidof(Recordset));
//    try
//    {
//        m_Recordset->Open(xSql.c_str(), m_Connection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
//        return True;
//    }
//    catch (_com_error* e)
//    {
//        wprintf(e->ErrorMessage());
//        if (m_Connection->State)
//        {
//            m_Connection->Close();
//            m_Connection = NULL;
//        }
//        ::CoUninitialize();
//        return False;
//    }
//}
//
//Boolean MsAccessDB::FindItemBySql(LPBASEPROTO lpBaseProto, mstr xSql)
//{
//    USE_PARAM(lpBaseProto);
//
//    if (this->ExeSql(xSql))
//    {
//        if (this->IsRecordsetBOF())
//        {
//            if (m_Connection->State)
//            {
//                m_Recordset->Close(); m_Recordset = nullptr;
//                return False;
//            }
//        }
//        else
//        {
//            // 游标定位到第一条记录
//            m_Recordset->MoveFirst();
//            _variant_t var;
//            while (!m_Recordset->adoEOF)
//            {
//                //const google::protobuf::Reflection* xReflection = lpBaseProto->GetReflection();
//                //const google::protobuf::Descriptor* xDescriptor = lpBaseProto->GetDescriptor();
//                //for (Int32 xFieldIndex = 0; xFieldIndex < xDescriptor->field_count(); xFieldIndex++)
//                //{
//                //    const google::protobuf::FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
//                //    if (xField->is_repeated())
//                //    {
//                //    }
//
//                //    var = m_Recordset->GetCollect("ID");
//                //}
//                const Reflection* xReflection = lpBaseProto->GetReflection();
//                const Descriptor* xDescriptor = lpBaseProto->GetDescriptor();
//                Int32 xFieldCount = xDescriptor->field_count();
//                for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
//                {
//                    const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
//                    mstr xName = xField->name();
//                    switch (xField->type())
//                    {
//                        //case ProtoType::TYPE_FLOAT:
//                        case ProtoType::TYPE_INT32:
//                        {
//                            if (xField->is_repeated())
//                            {
//                                var = m_Recordset->GetCollect(xName.c_str());
//                                MsList<mstr> xListItem;
//                                MsBaseDef::StringSplit((MsBaseDef::W2M((BSTR)(_bstr_t)var)).c_str(), ';', xListItem);
//
//                                MsList<mstr> xListProtoItem;
//                                BEGIN_ENUMERATOR(xListItem)
//                                {
//                                    MsBaseDef::StringSplit(xEnumValue.c_str(), ',', xListProtoItem);
//                                }END_ENUMERATOR;
//                            }
//                            else
//                            {
//                                var = m_Recordset->GetCollect(xName.c_str());
//                                xReflection->SetFloat(lpBaseProto, xField, (float)var);
//                            }
//                        }break;
//                    }
//
//                    //    case ProtoType::TYPE_DOUBLE:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddDouble(xProto, xField, lua_tonumber(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetDouble(xProto, xField, lua_tonumber(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_INT64:
//                    //    case ProtoType::TYPE_SINT64:
//                    //    case ProtoType::TYPE_FIXED64:
//                    //    case ProtoType::TYPE_SFIXED64:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddInt64(xProto, xField, lua_tointeger(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetInt64(xProto, xField, lua_tointeger(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_UINT64:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetUInt64(xProto, xField, (UInt64)lua_tointeger(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_INT32:
//                    //    case ProtoType::TYPE_SINT32:
//                    //    case ProtoType::TYPE_FIXED32:
//                    //    case ProtoType::TYPE_SFIXED32:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetInt32(xProto, xField, (Int32)lua_tointeger(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_UINT32:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetUInt32(xProto, xField, (UInt32)lua_tointeger(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_ENUM:
//                    //    {
//                    //        AssertNormal(False, "lua_table_config not supported enum");
//                    //    }break;
//
//                    //    case ProtoType::TYPE_BOOL:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetBool(xProto, xField, (lua_toboolean(lua_state_, -1) ? true : false));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_STRING:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_BYTES:
//                    //    {
//                    //        //AssertNormal(False, "lua_table_config not supported bytes");
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                xReflection->AddString(xProto, xField, lua_tostring(lua_state_, -1));
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            xReflection->SetString(xProto, xField, lua_tostring(lua_state_, -1));
//                    //        }
//                    //    }break;
//
//                    //    case ProtoType::TYPE_MESSAGE:
//                    //    {
//                    //        if (xField->is_repeated())
//                    //        {
//                    //            int xLen = (int)lua_objlen(lua_state_, -1);
//                    //            for (int i = 1; i <= xLen; i++)
//                    //            {
//                    //                Message* xMsg = xReflection->AddMessage(xProto, xField);
//                    //                lua_rawgeti(lua_state_, -1, i);
//                    //                this->LoadParseLuaTableListData(xMsg);
//                    //                lua_pop(lua_state_, 1);
//                    //            }
//                    //        }
//                    //        else
//                    //        {
//                    //            Message* xMsg = xReflection->MutableMessage(xProto, xField);
//                    //            this->LoadParseLuaTableListData(xMsg);
//                    //        }
//                    //    }break;
//                }
//                //lua_pop(lua_state_, 1);
//            }
//            return True;
//        }
//    }
//    return False;
//}
//
////
////void main()
////{
////    try
////    {
////
////
////        // 游标定位到第一条记录
////        m_Recordset->MoveFirst();
////        _variant_t var;
////        while (!m_Recordset->adoEOF)
////        {
////            var = m_Recordset->GetCollect("ID");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("初始场景ID");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("金币");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("药丸");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("棒棒糖");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("等级");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("技能点");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("VIP等级");
////            if (var.vt != VT_NULL) { printf("%d\t", (int)var); }
////
////            var = m_Recordset->GetCollect("物品列表");
////            if (var.vt != VT_NULL) { wprintf(L"%s\t", (BSTR)(_bstr_t)var); }
////
////            var = m_Recordset->GetCollect("表情列表");
////            if (var.vt != VT_NULL) { wprintf(L"%s\t", (BSTR)(_bstr_t)var); }
////
////            var = m_Recordset->GetCollect("套装列表");
////            if (var.vt != VT_NULL) { wprintf(L"%s\t", (BSTR)(_bstr_t)var); }
////
////            printf("\n");
////
////            m_Recordset->MoveNext();
////        }
////    }
////    catch (_com_error *e)
////    {
////        wprintf(e->ErrorMessage());
////    }
////
////
////
////    //cout << "---------------------------------" << endl;
////
////    system("Pause");
////    return;
////}
