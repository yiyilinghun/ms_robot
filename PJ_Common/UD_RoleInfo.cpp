#include "Precompiled.h"
void UD_RoleInfo::FillRoleInfo(S2C_Sync_RoleInfo* xRoleInfoPacket)
{
    xRoleInfoPacket->set_role_index(this->GetRoleIndex());
    this->GetRolePublicMemory()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRolePrivateMemory()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRolePublic()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRolePrivate()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleAttr()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleBag()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleEquips()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleSkills()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleTasks()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleFriends()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetRoleConfig()->CopyTo(xRoleInfoPacket->add_universal_role_info());
    this->GetSummonpets()->CopyTo(xRoleInfoPacket->add_universal_role_info());
}
