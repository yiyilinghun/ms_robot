#pragma once

#define DEF_B_BEGIN_FIELD const static Int32 BEGIN_FIELD_B = __LINE__;
#define DEF_B_FIELD(field) DINLINE Boolean Get##field() { this->Check##field(); return m_prt->b(__LINE__-BEGIN_FIELD_B-1); }\
DINLINE static Boolean Get##field(const universal_data& ud) { return ud.b(Get##field##Index()); }\
DINLINE static Boolean Get##field(universal_data* ud_ptr) { return ud_ptr->b(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_B-1; }\
DINLINE void Set##field(Boolean v) { this->Check##field(); v ? m_prt->set_b(__LINE__-BEGIN_FIELD_B-1, true) : m_prt->set_b(__LINE__-BEGIN_FIELD_B-1, false); }\
DINLINE void Check##field() { while (m_prt->b_size() <= (__LINE__ - BEGIN_FIELD_B - 1)) { m_prt->add_b(false); } }

#define DEF_F_BEGIN_FIELD const static Int32 BEGIN_FIELD_F = __LINE__;
#define DEF_F_FIELD(field) DINLINE Single Get##field() { this->Check##field(); return m_prt->f(__LINE__-BEGIN_FIELD_F-1); }\
DINLINE static Single Get##field(const universal_data& ud) { return ud.f(Get##field##Index()); }\
DINLINE static Single Get##field(universal_data* ud_ptr) { return ud_ptr->f(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_F-1; }\
DINLINE void Set##field(Single v) { this->Check##field(); m_prt->set_f(__LINE__-BEGIN_FIELD_F-1, v); }\
DINLINE void Increment##field(Single v) { this->Check##field(); m_prt->set_f(__LINE__-BEGIN_FIELD_F-1, this->Get##field() + v); }\
DINLINE void Decrement##field(Single v) { this->Check##field(); m_prt->set_f(__LINE__-BEGIN_FIELD_F-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->f_size() <= (__LINE__ - BEGIN_FIELD_F - 1)) { m_prt->add_f(0.0f); } }

#define DEF_D_BEGIN_FIELD const static Int32 BEGIN_FIELD_D = __LINE__;
#define DEF_D_FIELD(field) DINLINE Double Get##field() { this->Check##field(); return m_prt->d(__LINE__-BEGIN_FIELD_D-1); }\
DINLINE static Double Get##field(const universal_data& ud) { return ud.d(Get##field##Index()); }\
DINLINE static Double Get##field(universal_data* ud_ptr) { return ud_ptr->d(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_D-1; }\
DINLINE void Set##field(Double v) { this->Check##field(); m_prt->set_d(__LINE__-BEGIN_FIELD_D-1, v); }\
DINLINE void Increment##field(Double v) { this->Check##field(); m_prt->set_d(__LINE__-BEGIN_FIELD_D-1, this->Get##field() + v); }\
DINLINE void Decrement##field(Double v) { this->Check##field(); m_prt->set_d(__LINE__-BEGIN_FIELD_D-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->d_size() <= (__LINE__ - BEGIN_FIELD_D - 1)) { m_prt->add_d(0.0); } }

#define DEF_I32_ENUM_BEGIN_FIELD const static Int32 BEGIN_FIELD_I32 = __LINE__;
#define DEF_I32_FIELD(field) DINLINE Int32 Get##field() { this->Check##field(); return m_prt->i32(__LINE__-BEGIN_FIELD_I32-1); }\
DINLINE static Int32 Get##field(const universal_data& ud) { return ud.i32(Get##field##Index()); }\
DINLINE static Int32 Get##field(universal_data* ud_ptr) { return ud_ptr->i32(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_I32-1; }\
DINLINE void Set##field(Int32 v) { this->Check##field(); m_prt->set_i32(__LINE__-BEGIN_FIELD_I32-1, v); }\
DINLINE void Increment##field(Int32 v) { this->Check##field(); m_prt->set_i32(__LINE__-BEGIN_FIELD_I32-1, this->Get##field() + v); }\
DINLINE void Decrement##field(Int32 v) { this->Check##field(); m_prt->set_i32(__LINE__-BEGIN_FIELD_I32-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->i32_size() <= (__LINE__ - BEGIN_FIELD_I32 - 1)) { m_prt->add_i32(0); } }
#define DEF_ENUM_FIELD(_type_, field)  _type_ Get##field() { this->Check##field(); return (_type_)m_prt->i32(__LINE__-BEGIN_FIELD_I32-1); }\
DINLINE static _type_ Get##field(const universal_data& ud) { return (_type_)ud.i32(Get##field##Index()); }\
DINLINE static _type_ Get##field(universal_data* ud_ptr) { return (_type_)ud_ptr->i32(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_I32-1; }\
DINLINE void Set##field(_type_ v) { this->Check##field(); m_prt->set_i32(__LINE__-BEGIN_FIELD_I32-1, (Int32)v); }\
DINLINE void Check##field() { while (m_prt->i32_size() <= (__LINE__ - BEGIN_FIELD_I32 - 1)) { m_prt->add_i32(0); } }

#define DEF_I64_BEGIN_FIELD const static Int32 BEGIN_FIELD_I64 = __LINE__;
#define DEF_I64_FIELD(field) DINLINE Int64 Get##field() { this->Check##field(); return m_prt->i64(__LINE__-BEGIN_FIELD_I64-1); }\
DINLINE static Int64 Get##field(const universal_data& ud) { return ud.i64(Get##field##Index()); }\
DINLINE static Int64 Get##field(universal_data* ud_ptr) { return ud_ptr->i64(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_I64-1; }\
DINLINE void Set##field(Int64 v) { this->Check##field();  m_prt->set_i64(__LINE__-BEGIN_FIELD_I64-1, v); }\
DINLINE void Increment##field(Int64 v) { this->Check##field(); m_prt->set_i64(__LINE__-BEGIN_FIELD_I64-1, this->Get##field() + v); }\
DINLINE void Decrement##field(Int64 v) { this->Check##field(); m_prt->set_i64(__LINE__-BEGIN_FIELD_I64-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->i64_size() <= (__LINE__ - BEGIN_FIELD_I64 - 1)) { m_prt->add_i64(0); } }

#define DEF_U32_BEGIN_FIELD const static Int32 BEGIN_FIELD_U32 = __LINE__;
#define DEF_U32_FIELD(field) DINLINE  UInt32 Get##field() { this->Check##field(); return m_prt->u32(__LINE__-BEGIN_FIELD_U32-1); }\
DINLINE static UInt32 Get##field(const universal_data& ud) { return ud.u32(Get##field##Index()); }\
DINLINE static UInt32 Get##field(universal_data* ud_ptr) { return ud_ptr->u32(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_U32-1; }\
DINLINE void Set##field(UInt32 v) { this->Check##field(); m_prt->set_u32(__LINE__-BEGIN_FIELD_U32-1, v); }\
DINLINE void Increment##field(UInt32 v) { this->Check##field(); m_prt->set_u32(__LINE__-BEGIN_FIELD_U32-1, this->Get##field() + v); }\
DINLINE void Decrement##field(UInt32 v) { this->Check##field(); m_prt->set_u32(__LINE__-BEGIN_FIELD_U32-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->u32_size() <= (__LINE__ - BEGIN_FIELD_U32 - 1)) { m_prt->add_u32(0); } }

#define DEF_U64_BEGIN_FIELD const static Int32 BEGIN_FIELD_U64 = __LINE__;
#define DEF_U64_FIELD(field) DINLINE  UInt64 Get##field() { this->Check##field(); return m_prt->u64(__LINE__-BEGIN_FIELD_U64-1); }\
DINLINE static UInt64 Get##field(const universal_data& ud) { return ud.u64(Get##field##Index()); }\
DINLINE static UInt64 Get##field(universal_data* ud_ptr) { return ud_ptr->u64(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_U64-1; }\
DINLINE void Set##field(UInt64 v) { this->Check##field(); m_prt->set_u64(__LINE__-BEGIN_FIELD_U64-1, v); }\
DINLINE void Increment##field(UInt64 v) { this->Check##field(); m_prt->set_u64(__LINE__-BEGIN_FIELD_U64-1, this->Get##field() + v); }\
DINLINE void Decrement##field(UInt64 v) { this->Check##field(); m_prt->set_u64(__LINE__-BEGIN_FIELD_U64-1, this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->u64_size() <= (__LINE__ - BEGIN_FIELD_U64 - 1)) { m_prt->add_u64(0); } }











#define DEF_CHECK_F_FIELD(field,_min_,_max_) DINLINE Single Get##field() { this->Check##field(); return m_prt->f(__LINE__-BEGIN_FIELD_F-1); }\
DINLINE static Single Get##field(const universal_data& ud) { return ud.f(Get##field##Index()); }\
DINLINE static Single Get##field(universal_data* ud_ptr) { return ud_ptr->f(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_F-1; }\
DINLINE void Set##field(Single v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v; this->Check##field(); m_prt->set_f(__LINE__-BEGIN_FIELD_F-1, v); }\
DINLINE void Increment##field(Single v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(Single v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->f_size() <= (__LINE__ - BEGIN_FIELD_F - 1)) { m_prt->add_f(0.0f); } }

#define DEF_CHECK_D_FIELD(field,_min_,_max_) DINLINE Double Get##field() { this->Check##field(); return m_prt->d(__LINE__-BEGIN_FIELD_D-1); }\
DINLINE static Double Get##field(const universal_data& ud) { return ud.d(Get##field##Index()); }\
DINLINE static Double Get##field(universal_data* ud_ptr) { return ud_ptr->d(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_D-1; }\
DINLINE void Set##field(Double v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v;  this->Check##field(); m_prt->set_d(__LINE__-BEGIN_FIELD_D-1, v); }\
DINLINE void Increment##field(Double v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(Double v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->d_size() <= (__LINE__ - BEGIN_FIELD_D - 1)) { m_prt->add_d(0.0); } }

#define DEF_CHECK_I32_FIELD(field,_min_,_max_) DINLINE Int32 Get##field() { this->Check##field(); return m_prt->i32(__LINE__-BEGIN_FIELD_I32-1); }\
DINLINE static Int32 Get##field(const universal_data& ud) { return ud.i32(Get##field##Index()); }\
DINLINE static Int32 Get##field(universal_data* ud_ptr) { return ud_ptr->i32(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_I32-1; }\
DINLINE void Set##field(Int32 v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v;  this->Check##field(); m_prt->set_i32(__LINE__-BEGIN_FIELD_I32-1, v); }\
DINLINE void Increment##field(Int32 v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(Int32 v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->i32_size() <= (__LINE__ - BEGIN_FIELD_I32 - 1)) { m_prt->add_i32(0); } }

#define DEF_CHECK_I64_FIELD(field,_min_,_max_) DINLINE Int64 Get##field() { this->Check##field(); return m_prt->i64(__LINE__-BEGIN_FIELD_I64-1); }\
DINLINE static Int64 Get##field(const universal_data& ud) { return ud.i64(Get##field##Index()); }\
DINLINE static Int64 Get##field(universal_data* ud_ptr) { return ud_ptr->i64(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_I64-1; }\
DINLINE void Set##field(Int64 v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v;  this->Check##field();  m_prt->set_i64(__LINE__-BEGIN_FIELD_I64-1, v); }\
DINLINE void Increment##field(Int64 v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(Int64 v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->i64_size() <= (__LINE__ - BEGIN_FIELD_I64 - 1)) { m_prt->add_i64(0); } }

#define DEF_CHECK_U32_FIELD(field,_min_,_max_) DINLINE  UInt32 Get##field() { this->Check##field(); return m_prt->u32(__LINE__-BEGIN_FIELD_U32-1); }\
DINLINE static UInt32 Get##field(const universal_data& ud) { return ud.u32(Get##field##Index()); }\
DINLINE static UInt32 Get##field(universal_data* ud_ptr) { return ud_ptr->u32(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_U32-1; }\
DINLINE void Set##field(UInt32 v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v;  this->Check##field(); m_prt->set_u32(__LINE__-BEGIN_FIELD_U32-1, v); }\
DINLINE void Increment##field(UInt32 v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(UInt32 v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->u32_size() <= (__LINE__ - BEGIN_FIELD_U32 - 1)) { m_prt->add_u32(0); } }

#define DEF_CHECK_U64_FIELD(field,_min_,_max_) DINLINE  UInt64 Get##field() { this->Check##field(); return m_prt->u64(__LINE__-BEGIN_FIELD_U64-1); }\
DINLINE static UInt64 Get##field(const universal_data& ud) { return ud.u64(Get##field##Index()); }\
DINLINE static UInt64 Get##field(universal_data* ud_ptr) { return ud_ptr->u64(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_U64-1; }\
DINLINE void Set##field(UInt64 v) { v = v > _max_ ? _max_ : v; v = v < _min_ ? _min_ : v;  this->Check##field(); m_prt->set_u64(__LINE__-BEGIN_FIELD_U64-1, v); }\
DINLINE void Increment##field(UInt64 v) { this->Set##field(this->Get##field() + v); }\
DINLINE void Decrement##field(UInt64 v) { this->Set##field(this->Get##field() - v); }\
DINLINE void Check##field() { while (m_prt->u64_size() <= (__LINE__ - BEGIN_FIELD_U64 - 1)) { m_prt->add_u64(0); } }









#define DEF_STR_BEGIN_FIELD const static Int32 BEGIN_FIELD_STR = __LINE__;
#define DEF_STR_FIELD(field) DINLINE  mstr Get##field() { this->Check##field(); return m_prt->str(__LINE__-BEGIN_FIELD_STR-1); }\
DINLINE static mstr Get##field(const universal_data& ud) { return ud.str(Get##field##Index()); }\
DINLINE static mstr Get##field(universal_data* ud_ptr) { return ud_ptr->str(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_STR-1; }\
DINLINE void Set##field(mstr v) { this->Check##field();  m_prt->set_str(__LINE__-BEGIN_FIELD_STR-1, v); }\
DINLINE void Check##field() { while (m_prt->str_size() <= (__LINE__ - BEGIN_FIELD_STR - 1)) { m_prt->add_str(""); } }


#define DEF_PROTO_BEGIN_FIELD const static Int32 BEGIN_FIELD_PROTO = __LINE__;
#define DEF_PROTO_FIELD(_type_, field) DINLINE _type_* Get##field() { return Get##field(True); }\
DINLINE _type_* Get##field(Boolean IsAutoLoad)\
{\
    this->Check##field();\
    universal_data* xUData = m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1);\
    if (m_Dict_Data.ContainsKey(xUData))\
    {\
        return (_type_*)m_Dict_Data[xUData];\
    }\
    else\
    {\
        _type_* xTemp = NEW _type_(xUData);\
        IsAutoLoad ? xTemp->Load() : NULL;\
        m_Dict_Data[xUData] = xTemp;\
        return xTemp;\
    }\
}\
DINLINE void Load##field(MsXmlReader& xMsXmlReader)\
{\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        this->Get##field()->LoadFromObviousXml(xMsXmlReader);\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
}\
DINLINE void Load##field() { this->Get##field()->Load(); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
DINLINE void CopyTo##field(universal_data& xUD) { xUD.CopyFrom(*(m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1))); }\
DINLINE void CopyTo##field(universal_data* xUD) { xUD->CopyFrom(*(m_prt->mutable_sub(__LINE__ - BEGIN_FIELD_PROTO - 1))); }

#define DEF_UNIVERSAL_FIELD(_universalfield_index)  DEF_PROTO_FIELD(UD_UniversalField, UniversalField_##_universalfield_index);
#define DEF_UNIVERSAL_FIELD_INSTANCE(_type_, _universalfield_index) friend class _type_;\
_type_* Get##_type_(){return UD_Base::GetFieldByIndex<_type_>(_universalfield_index, True);}\
void Set##_type_(_type_* x##_type_){return UD_Base::SetFieldByIndex<_type_>(_universalfield_index, x##_type_);}













#define DEF_VECTOR_PROTO_FIELD(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
DINLINE universal_data* Get##field(Int32 xIndex) { this->Check##field(); return xIndex < m_Vect_##field.GetCount() ? m_Vect_##field[xIndex].GetUDPtr() : nullptr; }\
DINLINE void Set##field(Int32 xIndex, _type_& v) { this->Check##field(); m_Vect_##field[xIndex].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Add##field(_type_& v) { this->Check##field(); m_Vect_##field.Add(v); this->Save##field(); }\
MsVector<_type_> m_Vect_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader){\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do{\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
}\
DINLINE void Load##field(){\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    m_Vect_##field.Clear();\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub(); \
    FOREACH_PROTO_REPEATED(xRepeated) {\
        auto* xTemp = xCurrentValue; \
        _type_ x##_type_;\
        x##_type_.CopyFrom(xTemp);\
        x##_type_.Load();\
        m_Vect_##field.Add(x##_type_);\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FAST_FOREACH(m_Vect_##field)\
    {\
        xRepeated->Add()->CopyFrom(m_Vect_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}











#define DEF_LIST_PROTO_FIELD(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
DINLINE universal_data* Get##field(Int32 xIndex) { this->Check##field(); return xIndex < m_List_##field.GetCount() ? m_List_##field[xIndex].GetUDPtr() : nullptr; }\
DINLINE void Set##field(Int32 xIndex, _type_& v) { this->Check##field(); m_List_##field[xIndex].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Add##field(_type_& v) { this->Check##field(); m_List_##field.Add(v); this->Save##field(); }\
DINLINE void Remove##field(_type_& v) { this->Check##field(); m_List_##field.Remove(v); }\
MsList<_type_> m_List_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader){\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do{\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
}\
DINLINE void Load##field(){\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    m_List_##field.Clear();\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub(); \
    FOREACH_PROTO_REPEATED(xRepeated) {\
        auto* xTemp = xCurrentValue; \
        _type_ x##_type_;\
        x##_type_.CopyFrom(xTemp);\
        x##_type_.Load();\
        m_List_##field.Add(x##_type_);\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FAST_FOREACH(m_List_##field)\
    {\
        xRepeated->Add()->CopyFrom(m_List_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}












#define DEF_DICT_PROTO_FIELD_BY_KEY(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
DINLINE universal_data* Get##field(Int64 xKey) { this->Check##field(); return m_Dict_##field.ContainsKey(xKey) ? m_Dict_##field[xKey].GetUDPtr() : nullptr; }\
DINLINE void Set##field(Int64 xKey, _type_& v) { this->Check##field(); m_Dict_##field[xKey].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Add##field(Int64 xKey, _type_& v) { this->Check##field(); m_Dict_##field[xKey].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Remove##field(Int64 xKey) { this->Check##field(); m_Dict_##field.Remove(xKey); }\
MsUnorderedDictionary<Int64, _type_> m_Dict_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader)\
{\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do\
            {\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
}\
DINLINE void Load##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    FOREACH_PROTO_REPEATED(xRepeated)\
    {\
        auto* xTemp = xCurrentValue;\
        m_Dict_##field[xTemp->key()].CopyFrom(xTemp);\
        m_Dict_##field[xTemp->key()].Load();\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FAST_FOREACH(m_Dict_##field)\
    {\
        m_Dict_##field.Value().SetKey(m_Dict_##field.GetKey());\
        xRepeated->Add()->CopyFrom(m_Dict_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}












#define DEF_DICT_PROTO_FIELD_BY_KEY_NO_IF(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
private: DINLINE universal_data* Get##field(Int64 xKey) { this->Check##field(); return m_Dict_##field.ContainsKey(xKey) ? m_Dict_##field[xKey].GetUDPtr() : nullptr; }\
private: DINLINE void Set##field(Int64 xKey, _type_& v) { this->Check##field(); m_Dict_##field[xKey].CopyFrom(v.GetUD()); this->Save##field(); }\
private: DINLINE void Add##field(Int64 xKey, _type_& v) { this->Check##field(); m_Dict_##field[xKey].CopyFrom(v.GetUD()); this->Save##field(); }\
private: DINLINE void Remove##field(Int64 xKey) { this->Check##field(); m_Dict_##field.Remove(xKey); }\
public:  MsUnorderedDictionary<Int64, _type_> m_Dict_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader)\
{\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do\
            {\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
}\
DINLINE void Load##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    FOREACH_PROTO_REPEATED(xRepeated)\
    {\
        auto* xTemp = xCurrentValue;\
        m_Dict_##field[xTemp->key()].CopyFrom(xTemp);\
        m_Dict_##field[xTemp->key()].Load();\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FAST_FOREACH(m_Dict_##field)\
    {\
        m_Dict_##field.Value().SetKey(m_Dict_##field.GetKey());\
        xRepeated->Add()->CopyFrom(m_Dict_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}









#define DEF_DICT_PROTO_FIELD_BY_NAME(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
DINLINE universal_data* Get##field(mstr xName) { this->Check##field(); return m_Dict_##field.ContainsKey(xName) ? m_Dict_##field[xName] : nullptr; }\
DINLINE void Set##field(mstr xName, _type_& v) { this->Check##field(); m_Dict_##field[xName].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Add##field(mstr xName, _type_& v) { this->Check##field(); m_Dict_##field[xName].CopyFrom(v.GetUD()); this->Save##field(); }\
DINLINE void Remove##field(mstr xName) { this->Check##field(); m_Dict_##field.Remove(xName); }\
MsUnorderedDictionary<mstr, _type_> m_Dict_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader)\
{\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do\
            {\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
    Load##field();\
}\
DINLINE void Load##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    FOREACH_PROTO_REPEATED(xRepeated)\
    {\
        auto* xTemp = xCurrentValue;\
        m_Dict_##field[xTemp->name()].CopyFrom(xTemp);\
        m_Dict_##field[xTemp->name()].Load();\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FAST_FOREACH(m_Dict_##field)\
    {\
        m_Dict_##field.Value().SetName(m_Dict_##field.GetKey());\
        xRepeated->Add()->CopyFrom(m_Dict_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}









#define DEF_DICT_PROTO_FIELD_BY_NAME_NO_IF(_type_, field) universal_data* Get##field() { this->Check##field(); return m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1); }\
DINLINE static const universal_data& Get##field(const universal_data& ud) { return ud.sub(Get##field##Index()); }\
DINLINE static universal_data* Get##field(universal_data* ud_ptr) { return ud_ptr->mutable_sub(Get##field##Index()); }\
DINLINE static Int32 Get##field##Index() { return __LINE__-BEGIN_FIELD_PROTO-1; }\
DINLINE void Set##field(_type_& v) { this->Check##field();  m_prt->mutable_sub(__LINE__-BEGIN_FIELD_PROTO-1)->CopyFrom(*(v.m_prt)); }\
DINLINE void Check##field() { while (m_prt->sub_size() <= (__LINE__ - BEGIN_FIELD_PROTO - 1)) { m_prt->add_sub(); } }\
private: DINLINE universal_data* Get##field(mstr xName) { this->Check##field(); return m_Dict_##field.ContainsKey(xName) ? m_Dict_##field[xName] : nullptr; }\
private: DINLINE void Set##field(mstr xName, _type_& v) { this->Check##field(); m_Dict_##field[xName].CopyFrom(v.GetUD()); this->Save##field(); }\
private: DINLINE void Add##field(mstr xName, _type_& v) { this->Check##field(); m_Dict_##field[xName].CopyFrom(v.GetUD()); this->Save##field(); }\
private: DINLINE void Remove##field(mstr xName) { this->Check##field(); m_Dict_##field.Remove(xName); }\
public:  MsUnorderedDictionary<mstr, _type_> m_Dict_##field;\
DINLINE void Load##field(MsXmlReader& xMsXmlReader)\
{\
    Int32 iPosParent, iPos, iPosChild;\
    xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);\
    xMsXmlReader[""#field];\
    if (xMsXmlReader) {\
        universal_data* xUData = this->Get##field();\
        xUData->set_key(INVALID_LID);\
        xUData->set_type("group@"#_type_);\
        xUData->set_name("group@"#field);\
        if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {\
            do\
            {\
                _type_ x##_type_(xMsXmlReader);\
                xUData->add_sub()->CopyFrom(*(x##_type_.m_prt));\
            } while (xMsXmlReader.FindNextElem());\
        }\
    }\
    xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);\
    Load##field();\
}\
DINLINE void Load##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    FOREACH_PROTO_REPEATED(xRepeated)\
    {\
        auto* xTemp = xCurrentValue;\
        m_Dict_##field[xTemp->name()].CopyFrom(xTemp);\
        m_Dict_##field[xTemp->name()].Load();\
    }\
}\
DINLINE void Save##field(MsXmlReader& xMsXmlReader)\
{\
    ;\
}\
DINLINE void Save##field()\
{\
    universal_data* xUData = this->Get##field();\
    xUData->set_key(INVALID_LID);\
    xUData->set_type("group@"#_type_);\
    xUData->set_name("group@"#field);\
    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();\
    xRepeated->Clear();\
    FOREACH_CONTAINER_(m_Dict_##field)\
    {\
        xEnumValue.SetName(m_Dict_##field.GetKey());\
        xRepeated->Add()->CopyFrom(m_Dict_##field.Value().GetUD());\
    }\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message* from)\
{\
    this->Get##field()->CopyFrom(*from);\
    this->Load##field();\
}\
DINLINE void CopyFrom_Load##field(const ::google::protobuf::Message& from)\
{\
    this->Get##field()->CopyFrom(from);\
    this->Load##field();\
}








#define FAST_READ_HEX_VALUE(_name_)             this->Set##_name_(MsBase::atox(Attribs[""#_name_].c_str()));
#define FAST_READ_INT32_VALUE(_name_)           this->Set##_name_(atoi(Attribs[""#_name_].c_str()));
#define FAST_READ_UINT32_VALUE(_name_)          this->Set##_name_(strtoul(Attribs[""#_name_].c_str(), nullptr, 10));
#define FAST_READ_ENUM_VALUE(_type_, _name_)    this->Set##_name_((_type_)atoi(Attribs[""#_name_].c_str()));
#define FAST_READ_INT64_VALUE(_name_)           this->Set##_name_(atoll(Attribs[""#_name_].c_str()));
#define FAST_READ_UINT64_VALUE(_name_)          this->Set##_name_(strtoull(Attribs[""#_name_].c_str(), nullptr, 10));
#define FAST_READ_SINGLE_VALUE(_name_)          this->Set##_name_((Single)atof(Attribs[""#_name_].c_str()));
#define FAST_READ_STR_VALUE(_name_)             this->Set##_name_(Attribs[""#_name_]);


#define FAST_READ_ATTR_TYPE_NAME(_type_) MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;\
this->SetName(Attribs.m_MsXmlReader->GetTagName());\
this->SetType(""#_type_);

#define FAST_READ_TYPE(_type_) this->SetType(""#_type_);
#define FAST_READ_TYPE_NAME(_type_) this->SetType(""#_type_); this->SetName(""#_type_);


#define CALL_LOAD_FROM_XML_ATTR             if (m_IsAutoNew) { SAFE_DELETE(m_prt); } m_IsAutoNew = True; m_prt = m_prt ? m_prt : NEW universal_data();\
m_prt->set_key(INVALID_LID); this->LoadFromObviousXml(xMsXmlReader);


#define CALL_COPY_FROM_UNIVERSAL_DATA       if (m_IsAutoNew) { SAFE_DELETE(m_prt); } m_IsAutoNew = True; m_prt = m_prt ? m_prt : NEW universal_data();\
m_prt->CopyFrom(xProtoData); if(!m_prt->has_key()) { m_prt->set_key(INVALID_LID); }


#define CALL_COPY_FROM_MYSQL                if (m_IsAutoNew) { SAFE_DELETE(m_prt); } m_IsAutoNew = True;  m_prt = m_prt ? m_prt : NEW universal_data();\
m_prt->set_key(xKey); xMsMysql.LoadUniversalData(xKey, m_prt);


#define CALL_COPY_FROM_UNIVERSAL_DATA_PTR   if (m_IsAutoNew) { SAFE_DELETE(m_prt); } m_IsAutoNew = False; m_prt = xProtoData;\
if(!m_prt->has_key()) { m_prt->set_key(INVALID_LID); this->Load(); }


//#define INIT_ALL_PARSER_ADDR                if (m_prt) { for (Int32 i = 0; i < m_prt->data_size(); i++) { universal_data* xUData = m_prt->mutable_sub(i); xUData->clear_parser_addr(); } m_prt->set_parser_addr((Int64)this); }
#define INIT_ALL_PARSER_ADDR
//#define CALL_DESTRUCTOR_EXE                 if (m_prt) { for (Int32 i = 0; i < m_prt->data_size(); i++) { universal_data* xUData = m_prt->mutable_sub(i); if (xUData->has_parser_addr() && xUData->parser_addr() > 0) { UD_Base* xTempPtr = (UD_Base*)xUData->parser_addr(); SAFE_DELETE(xTempPtr); xUData->clear_parser_addr(); } } }
#define CALL_DESTRUCTOR_EXE


#define DEF_UD_PROTO_STRUCTURE(_class_)     _class_() { this->Load(); }\
_class_(MsXmlReader& xMsXmlReader)          : UD_Base(xMsXmlReader)     { CALL_LOAD_FROM_XML_ATTR; INIT_ALL_PARSER_ADDR; this->Load(); }\
_class_(const universal_data& xProtoData)   : UD_Base(xProtoData)       { CALL_COPY_FROM_UNIVERSAL_DATA; INIT_ALL_PARSER_ADDR; this->Load(); }\
_class_(universal_data* xProtoData)         : UD_Base(xProtoData)       { CALL_COPY_FROM_UNIVERSAL_DATA_PTR; INIT_ALL_PARSER_ADDR; this->Load(); }\
_class_(const _class_& xSrc)                : UD_Base(xSrc)             { universal_data& xProtoData = ((_class_*)(&xSrc))->GetUD(); CALL_COPY_FROM_UNIVERSAL_DATA; INIT_ALL_PARSER_ADDR; this->Load(); }\
void Re_##_class_(MsXmlReader& xMsXmlReader)        { CALL_LOAD_FROM_XML_ATTR; INIT_ALL_PARSER_ADDR; this->Load(); }\
void Re_##_class_(const universal_data& xProtoData) { CALL_COPY_FROM_UNIVERSAL_DATA; INIT_ALL_PARSER_ADDR; this->Load(); }\
void Re_##_class_(universal_data* xProtoData)       { CALL_COPY_FROM_UNIVERSAL_DATA_PTR; INIT_ALL_PARSER_ADDR; this->Load(); }\
void operator =(_class_ &xSrc) { universal_data& xProtoData = xSrc.GetUD(); CALL_COPY_FROM_UNIVERSAL_DATA; INIT_ALL_PARSER_ADDR; this->Load(); }

class UD_Base
{
public:
    DINLINE operator universal_data*() { return m_prt; }
    DINLINE operator universal_data& () { return *m_prt; }
    DINLINE universal_data* GetUDPtr() { return m_prt; }
    DINLINE universal_data& GetUD() { return *m_prt; }
    DINLINE void Clear() { return m_prt->Clear(); }

    DINLINE UD_Base() { m_IsAutoNew = True;  m_prt = NEW universal_data(); m_prt->set_key(INVALID_LID); }
    //DINLINE UD_Base(Int64 xKey, MsMysql& xMsMysql) { USE_PARAM(xKey, xMsMysql); }
    DINLINE UD_Base(MsXmlReader& xMsXmlReader) { USE_PARAM(xMsXmlReader); }
    DINLINE UD_Base(const universal_data& xProtoData) { USE_PARAM(xProtoData); }
    DINLINE UD_Base(universal_data* xProtoData) { USE_PARAM(xProtoData); }
    DINLINE UD_Base(const UD_Base& xBase) { USE_PARAM(xBase); }
    virtual ~UD_Base() { SAFE_ENUMERATOR_DELETE(m_Dict_Data); if (m_IsAutoNew) { SAFE_DELETE(m_prt); } }

    //void SetChanged() { m_IsChanged = True; }
    //Boolean m_IsChanged = False;
    DINLINE void CopyTo(universal_data& xProtoData) { xProtoData.CopyFrom(*m_prt); }
    DINLINE void CopyTo(universal_data* xProtoData) { xProtoData->CopyFrom(*m_prt); }
    DINLINE void CopyFrom(const universal_data& xProtoData) { m_prt->CopyFrom(xProtoData); }
    DINLINE void CopyFrom(universal_data* xProtoData) { m_prt->CopyFrom(*xProtoData); }
    DINLINE void CopyFrom(UD_Base& xProtoData) { m_prt->CopyFrom(xProtoData.GetUD()); }
    DINLINE void CopyFrom(UD_Base* xProtoData) { m_prt->CopyFrom(xProtoData->GetUD()); }

    template<typename T>
    DINLINE T* GetFieldByIndex(Int32 xIndex, Boolean IsAutoLoad = True)
    {
        while (m_prt->sub_size() <= xIndex) { m_prt->add_sub(); }
        universal_data* xUData = m_prt->mutable_sub(xIndex);
        if (m_Dict_Data.ContainsKey(xUData))
        {
            return (T*)m_Dict_Data[xUData];
        }
        else
        {
            T* xTemp = NEW T(xUData);
            IsAutoLoad ? xTemp->Load() : NULL;
            m_Dict_Data[xUData] = xTemp;
            return xTemp;
        }
    }

    template<typename T>
    DINLINE void SetFieldByIndex(Int32 xIndex, T* xT)
    {
        while (m_prt->sub_size() <= xIndex) { m_prt->add_sub(); }
        universal_data* xUData = m_prt->mutable_sub(xIndex);
        xUData->CopyFrom(xT->GetUD());
    }

    Boolean m_IsAutoNew = False;
    universal_data* m_prt = nullptr;

    DINLINE Int64 GetKey() { return m_prt->key(); }
    DINLINE void SetKey(Int64 xKey) { m_prt->set_key(xKey); }

    DINLINE mstr GetType() { return m_prt->type(); }
    DINLINE void SetType(mstr xType) { m_prt->set_type(xType); }

    DINLINE mstr GetName() { return m_prt->name(); }
    virtual void SetName(mstr xName) { m_prt->set_name(xName); }

    DINLINE void CopyFrom_Load(const ::google::protobuf::Message& from) { SAFE_ENUMERATOR_DELETE(m_Dict_Data); m_prt->CopyFrom(from); this->Load(); }
    DINLINE void CopyFrom_Load(const ::google::protobuf::Message* from) { SAFE_ENUMERATOR_DELETE(m_Dict_Data); m_prt->CopyFrom(*from);  this->Load(); }
    DINLINE void CopyFrom_Load(UD_Base& xProtoData) { SAFE_ENUMERATOR_DELETE(m_Dict_Data); this->CopyFrom(xProtoData); this->Load(); }
    DINLINE void CopyFrom_Load(UD_Base* xProtoData) { SAFE_ENUMERATOR_DELETE(m_Dict_Data); this->CopyFrom(xProtoData); this->Load(); }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader) { USE_PARAM(xMsXmlReader); AssertNormal(False, "未初始化的xml解析!"); }
    virtual void LoadFromUniversalXml(MsXmlReader& xMsXmlReader) { USE_PARAM(xMsXmlReader); }
    virtual Boolean Load() { return True; };

    //virtual Boolean CheckDBByTypeName(MsMysql* xMsMysql);
    //virtual Boolean CheckDBEqual(MsMysql* xMsMysq);
    //virtual Boolean LoadDB(MsMysql* xMsMysql);
    //virtual Boolean SaveDB(MsMysql* xMsMysql, Boolean canUpdate, MsIOCPManager* xMsIOCPManager);

    MsUnorderedDictionary<universal_data*, UD_Base*> m_Dict_Data;
};

class UD_UniversalField : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_UniversalField);
    virtual void SetName(mstr xName) { return UD_Base::SetName(xName); }
    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(UniversalField);
        return True;
    }
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader) { FAST_READ_ATTR_TYPE_NAME(UniversalField); }
};

#define MAX_UNIT_IN_ONE_PAGE    24
#define MAX_BAG_PAGE_COUNT      10
