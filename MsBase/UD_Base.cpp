#include "Precompiled.h"

//Boolean UD_Base::CheckDBByTypeName(MsMysql* xMsMysql)
//{
//    MysqlAutoRow xRow;
//    if (xMsMysql->ExeSQLFormat("SELECT `key` FROM `main` WHERE `type` = '%s' AND `name` = '%s'", this->GetType().c_str(), this->GetName().c_str()) && xMsMysql->m_MysqlRes)
//    {
//        xRow = xMsMysql->NextRow();
//        if (xRow)
//        {
//            this->SetKey(xRow.GetInt64(0));
//            return True;
//        }
//    }
//    return False;
//}

//Boolean UD_Base::CheckDBEqual(MsMysql* xMsMysql)
//{
//    return xMsMysql->EqualUniversalData(*m_prt);
//}
//
//Boolean UD_Base::LoadDB(MsMysql* xMsMysql)
//{
//    if (xMsMysql->LoadUniversalData(this->GetKey(), m_prt))
//    {
//        this->Load();
//        return True;
//    }
//    return False;
//}
//
//Boolean UD_Base::SaveDB(MsMysql* xMsMysql, Boolean canUpdate, MsIOCPManager* xMsIOCPManager)
//{
//    return xMsMysql->SaveUniversalData(m_prt, canUpdate, xMsIOCPManager);
//}
