#include "Precompiled.h"

void UD_RoleAttr::CreateInit()
{
    this->SetRoleIndex(INVALID_LID);    // 队伍索引
    this->SetTeamIndex(INVALID_LID);    // 队伍索引
    this->SetLevel(1);      // 角色等级
    this->SetCurHp(1);      // 当前血量
    this->SetMaxHp(1);      // 最大血量
    this->SetCurMp(1);      // 当前法力
    this->SetMaxMp(1);      // 最大法力
    this->SetMinAp(1);      // 物理攻击下限
    this->SetMaxAp(1);      // 物理攻击上限
    this->SetPhysicalDefense(0);    // 物理防御

    this->SetCurSp(1.0f);       // 敏捷速度
    this->SetGrowthHp(1.0f);    // 根骨成长率
    this->SetGrowthMp(1.0f);    // 灵性成长率
    this->SetGrowthAp(1.0f);    // 力量成长率
    this->SetGrowthSp(1.0f);    // 敏捷成长率
    this->SetBaseActionForce(0.0f);     // 基础行动力
    this->SetCurrentActionForce(0.0f);  // 当前行动力
    this->SetFullActionForce(3000.0f);  // 行动力上限
    this->SetActionPlusRate(1000.0f);   // 行动力速率加成

    this->SetPointHp(0);        // 根骨已分配点
    this->SetReadyPointHp(0);   // 根骨未确认点

    this->SetPointMp(0);        // 灵性已分配点
    this->SetReadyPointMp(0);   // 灵性未确认点

    this->SetPointAp(0);        // 力量已分配点
    this->SetReadyPointAp(0);   // 力量未确认点

    this->SetPointSp(0);        // 敏捷已分配点
    this->SetReadyPointSp(0);   // 敏捷未确认点

    this->SetSurplusPoint(0);   // 剩余可分配点

    this->SetCurExp(0);             // 当前经验值
    this->SetMaxExp(100000000000);  // 最大经验值
}

void UD_RoleAttr::InitLevel(MsList<UD_LevelInfo*> xListLevelConfig)
{
    this->CreateInit();
    FAST_FOREACH(xListLevelConfig)
    {
        var* xRoleLevel = xListLevelConfig.Value();
        this->IncrementSurplusPoint(xRoleLevel->Get属性点());
        this->SetLevel(xRoleLevel->Get等级());
        this->SetCurExp(0);
        this->SetMaxExp(xRoleLevel->Get经验());
    }
}

void UD_RoleAttr::LevelUp(UD_LevelInfo* xLevelConfig)
{
    if ((this->GetLevel() + 1) == xLevelConfig->Get等级())
    {
        this->IncrementSurplusPoint(xLevelConfig->Get属性点());
        this->SetLevel(xLevelConfig->Get等级());
        this->SetMaxExp(xLevelConfig->Get经验());
    }
}


void UD_RoleAttr::Calculate(UD_RoleInfo* xRoleInfo, Boolean IsFullRefresh)
{
    Int64 xTempBaseHP = this->GetBaseHp();
    Int64 xTempBaseMP = this->GetBaseMp();
    Int64 xTempBaseMinAP = this->GetBaseMinAp();
    Int64 xTempBaseMaxAP = this->GetBaseMaxAp();
    Single xTempBaseSP = this->GetBaseSp();

    Int64 xLevel = this->GetLevel();
    Int64 E = Int64((100 - xLevel) / 5);

    Single xHPGrowth = this->GetGrowthHp();
    Int64 xHPPoint = this->GetPointHp();

    Single xMPGrowth = this->GetGrowthMp();
    Int64 xMPPoint = this->GetPointMp();

    Single xAPGrowth = this->GetGrowthAp();
    Int64 xAPPoint = this->GetPointAp();

    Single xSPGrowth = this->GetGrowthSp();
    Int64 xSPPoint = this->GetPointSp();

    if (xRoleInfo)
    {
        xHPPoint += xRoleInfo->GetRoleEquips()->GetAllEquipHPPoint();
        xMPPoint += xRoleInfo->GetRoleEquips()->GetAllEquipMPPoint();
        xAPPoint += xRoleInfo->GetRoleEquips()->GetAllEquipAPPoint();
        xSPPoint += xRoleInfo->GetRoleEquips()->GetAllEquipSPPoint();
    }

    //Int64 xBaseHp = (Int64)(xTempBaseHP * xHPGrowth);
    //Int64 xBaseMp = (Int64)(xTempBaseMP * xMPGrowth);
    //Int64 xBaseMinAp = (Int64)(xTempBaseAP * xAPGrowth);
    //Int64 xBaseMaxAp = (Int64)(xTempBaseAP * xAPGrowth);
    //Int64 xBaseSp = (Int64)(xTempBaseSP * xSPGrowth);

    this->SetMaxHp(Int64((Int64(((xLevel + E) * xHPPoint * xHPGrowth) + xTempBaseHP) /*加上装备附加气血值*/)/* 乘以修正*/));
    this->SetMaxMp(Int64((Int64(((xLevel + E) * xMPPoint * xMPGrowth) + xTempBaseMP) /*加上装备附加气血值*/)/* 乘以修正*/));
    this->SetMinAp(Int64((Int64(((xLevel + E) * xAPPoint * xAPGrowth) + xTempBaseMinAP) /*装备附加攻击值*/)/* 乘以修正*/));
    this->SetMaxAp(Int64((Int64(((xLevel + E) * xAPPoint * xAPGrowth) + xTempBaseMaxAP) /*装备附加攻击值*/)/* 乘以修正*/));
    this->SetCurSp(((xTempBaseSP + xSPPoint * xSPGrowth) + xTempBaseSP)              /*装备附加速度值*//* 乘以修正*/);

    if (IsFullRefresh)
    {
        this->SetCurHp(this->GetMaxHp());
        this->SetCurMp(this->GetMaxMp());
    }
    else
    {
        if (this->GetCurHp() > this->GetMaxHp()) { this->SetCurHp(this->GetMaxHp()); }
        if (this->GetCurMp() > this->GetMaxMp()) { this->SetCurMp(this->GetMaxMp()); }
    }
}
