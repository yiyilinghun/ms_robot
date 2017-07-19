#include "Precompiled.h"

void UD_TeamIndex::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_TeamIndex::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE_NAME(TeamIndex);
    return True;
}

void UD_TeamIndex::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(TeamIndex);
}










void UD_TeamInfo::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_TeamInfo::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE_NAME(TeamInfo);

    this->LoadTeamIndex();
    this->LoadTeamApply();
    this->LoadTeamInvite();
    return True;
}

Int64 UD_TeamInfo::GetTeamCaptain()
{
    if (m_List_TeamIndex.GetCount() > 0)
    {
        return m_List_TeamIndex[0].GetRoleIndex();
    }
    return INVALID_LID;
}

void UD_TeamInfo::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(TeamIndex);

}

Int32 UD_TeamInfo::FindInTeamPos(Int64 xRoleIndex)
{
    for (Int32 i = 0; i < m_List_TeamIndex.GetCount(); i++)
    {
        if (m_List_TeamIndex[i].GetRoleIndex() == xRoleIndex) { return i + 1; }
    }
    return INVALID_NID;
}

Boolean UD_TeamInfo::LeaveTeam(Int64 xRoleIndex) { return LeaveTeam(xRoleIndex, this->FindInTeamPos(xRoleIndex)); }
Boolean UD_TeamInfo::LeaveTeam(Int64 xRoleIndex, Int32 xPos)
{
    if (xPos > 0 && xPos <= MAX_TEAM_OBJECT_COUNT)
    {
        m_List_TeamIndex.RemoveAt(xPos - 1);
        this->SaveTeamIndex();
        return True;
    }
    return False;
}

void UD_TeamInfo::ClearApplyList()
{
    m_List_TeamApply.Clear();
}

Boolean UD_TeamInfo::CheckInApplyList(Int64 xRoleIndex)
{
    for (Int32 i = 0; i < m_List_TeamApply.GetCount(); i++)
    {
        if (m_List_TeamApply[i].GetRoleIndex() == xRoleIndex)
        {
            return True;
        }
    }
    return False;
}

Boolean UD_TeamInfo::EnterApplyList(Int64 xRoleIndex)
{
    if (!CheckInApplyList(xRoleIndex))
    {
        UD_TeamIndex xTeamIndex;
        xTeamIndex.SetRoleIndex(xRoleIndex);
        this->AddTeamApply(xTeamIndex);
        return True;
    }
    return False;
}

Boolean UD_TeamInfo::RemoveApplyList(Int64 xRoleIndex)
{
    SAFE_FOREACH(m_List_TeamApply)
    {
        if (m_List_TeamApply.Value().GetRoleIndex() == xRoleIndex)
        {
            m_List_TeamApply.RemoveCur();
            this->SaveTeamApply();
            return True;
        }
    }
    return False;
}

void UD_TeamInfo::CreateTeamInfo(Int64 xRoleIndex)
{
    m_List_TeamApply.Clear();
    UD_TeamIndex xTeamIndex;
    xTeamIndex.SetRoleIndex(xRoleIndex);
    this->AddTeamIndex(xTeamIndex);
    this->Load();
}

