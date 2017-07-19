#pragma once

class UD_TeamIndex : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_TeamIndex);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);

    bool operator==(UD_TeamIndex xSrc) { return xSrc.GetRoleIndex() == GetRoleIndex(); }
    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);
};

class UD_TeamInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_TeamInfo);

    DEF_PROTO_BEGIN_FIELD;
    DEF_LIST_PROTO_FIELD(UD_TeamIndex, TeamIndex);
    DEF_LIST_PROTO_FIELD(UD_TeamIndex, TeamApply);
    DEF_LIST_PROTO_FIELD(UD_TeamIndex, TeamInvite);

    //DINLINE void AddTeamApply_(UD_TeamIndex& v)
    //{
    //    this->CheckTeamApply(); m_List_TeamApply.Add(v); this->SaveTeamApply_();
    //}

    //DINLINE void SaveTeamApply_()
    //{
    //    universal_data* xUData = this->GetTeamApply();
    //    xUData->set_key(INVALID_LID);
    //    xUData->set_type("group@UD_TeamIndex");
    //    xUData->set_name("group@TeamIndex");
    //    RepeatedPtrField<universal_data>* xRepeated = xUData->mutable_sub();
    //    xRepeated->Clear();
    //    FAST_FOREACH(m_List_TeamApply)
    //    {
    //        xRepeated->Add()->CopyFrom(xEnumValue.GetUD());
    //    }
    //}
    //DEF_DICT_PROTO_FIELD_BY_KEY(UD_TeamIndex, TeamIndex);

    virtual void SetName(mstr xName);
    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);







public:
    Int64 GetTeamCaptain();

    Int32 FindInTeamPos(Int64 xRoleIndex);

    Boolean LeaveTeam(Int64 xRoleIndex);
    Boolean LeaveTeam(Int64 xRoleIndex, Int32 xPos);

    void ClearApplyList();

    Boolean CheckInApplyList(Int64 xRoleIndex);
    Boolean EnterApplyList(Int64 xRoleIndex);
    Boolean RemoveApplyList(Int64 xRoleIndex);

    void CreateTeamInfo(Int64 xRoleIndex);
};

