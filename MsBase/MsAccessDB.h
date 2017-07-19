//#import "./Mysql/msado15.dll" no_namespace rename("EOF", "adoEOF")  
////#ifdef _WIN64
////#import "./Mysql/msado15_x64.dll" no_namespace rename("EOF", "adoEOF")  
////#else
////#import "./Mysql/msado15_x86.dll" no_namespace rename("EOF", "adoEOF")  
////#endif
//
//class MsAccessDB
//{
//public:
//    MsAccessDB(mstr xDBFileName);
//    ~MsAccessDB();
//
//    Boolean FindItemBySql(LPBASEPROTO lpBaseProto, mstr xSql);
//private:
//    Boolean ExeSql(mstr xSql);
//    Boolean IsRecordsetBOF() { return m_Recordset->BOF; }
//
//    _ConnectionPtr m_Connection;
//    _RecordsetPtr m_Recordset;
//};
//
