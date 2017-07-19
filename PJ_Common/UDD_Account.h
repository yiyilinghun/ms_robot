#pragma once

class UDD_Account : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UDD_Account);

    DEF_I32_ENUM_BEGIN_FIELD;

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(Password);

    //virtual Boolean LoadDB(MsMysql* xMsMysql)
    //{
    //    var xRet = UD_Base::LoadDB(xMsMysql);
    //    return xRet;
    //}

    //virtual Boolean SaveDB(MsMysql* xMsMysql, Boolean canUpdate, MsIOCPManager* xMsIOCPManager)
    //{
    //    var xRet = UD_Base::SaveDB(xMsMysql, canUpdate, xMsIOCPManager);
    //    return xRet;
    //}

    virtual void SetName(mstr xName) { return UD_Base::SetName(xName); }
    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(UDD_Account);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader) { FAST_READ_ATTR_TYPE_NAME(UDD_Account); }
};
