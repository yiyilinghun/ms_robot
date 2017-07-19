#include "Precompiled.h"

void UD_RoleAttr::CreateInit()
{
    this->SetRoleIndex(INVALID_LID);    // ��������
    this->SetTeamIndex(INVALID_LID);    // ��������
    this->SetLevel(1);      // ��ɫ�ȼ�
    this->SetCurHp(1);      // ��ǰѪ��
    this->SetMaxHp(1);      // ���Ѫ��
    this->SetCurMp(1);      // ��ǰ����
    this->SetMaxMp(1);      // �����
    this->SetMinAp(1);      // ����������
    this->SetMaxAp(1);      // ����������
    this->SetPhysicalDefense(0);    // �������

    this->SetCurSp(1.0f);       // �����ٶ�
    this->SetGrowthHp(1.0f);    // ���ǳɳ���
    this->SetGrowthMp(1.0f);    // ���Գɳ���
    this->SetGrowthAp(1.0f);    // �����ɳ���
    this->SetGrowthSp(1.0f);    // ���ݳɳ���
    this->SetBaseActionForce(0.0f);     // �����ж���
    this->SetCurrentActionForce(0.0f);  // ��ǰ�ж���
    this->SetFullActionForce(3000.0f);  // �ж�������
    this->SetActionPlusRate(1000.0f);   // �ж������ʼӳ�

    this->SetPointHp(0);        // �����ѷ����
    this->SetReadyPointHp(0);   // ����δȷ�ϵ�

    this->SetPointMp(0);        // �����ѷ����
    this->SetReadyPointMp(0);   // ����δȷ�ϵ�

    this->SetPointAp(0);        // �����ѷ����
    this->SetReadyPointAp(0);   // ����δȷ�ϵ�

    this->SetPointSp(0);        // �����ѷ����
    this->SetReadyPointSp(0);   // ����δȷ�ϵ�

    this->SetSurplusPoint(0);   // ʣ��ɷ����

    this->SetCurExp(0);             // ��ǰ����ֵ
    this->SetMaxExp(100000000000);  // �����ֵ
}

void UD_RoleAttr::InitLevel(MsList<UD_LevelInfo*> xListLevelConfig)
{
    this->CreateInit();
    FAST_FOREACH(xListLevelConfig)
    {
        var* xRoleLevel = xListLevelConfig.Value();
        this->IncrementSurplusPoint(xRoleLevel->Get���Ե�());
        this->SetLevel(xRoleLevel->Get�ȼ�());
        this->SetCurExp(0);
        this->SetMaxExp(xRoleLevel->Get����());
    }
}

void UD_RoleAttr::LevelUp(UD_LevelInfo* xLevelConfig)
{
    if ((this->GetLevel() + 1) == xLevelConfig->Get�ȼ�())
    {
        this->IncrementSurplusPoint(xLevelConfig->Get���Ե�());
        this->SetLevel(xLevelConfig->Get�ȼ�());
        this->SetMaxExp(xLevelConfig->Get����());
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

    this->SetMaxHp(Int64((Int64(((xLevel + E) * xHPPoint * xHPGrowth) + xTempBaseHP) /*����װ��������Ѫֵ*/)/* ��������*/));
    this->SetMaxMp(Int64((Int64(((xLevel + E) * xMPPoint * xMPGrowth) + xTempBaseMP) /*����װ��������Ѫֵ*/)/* ��������*/));
    this->SetMinAp(Int64((Int64(((xLevel + E) * xAPPoint * xAPGrowth) + xTempBaseMinAP) /*װ�����ӹ���ֵ*/)/* ��������*/));
    this->SetMaxAp(Int64((Int64(((xLevel + E) * xAPPoint * xAPGrowth) + xTempBaseMaxAP) /*װ�����ӹ���ֵ*/)/* ��������*/));
    this->SetCurSp(((xTempBaseSP + xSPPoint * xSPGrowth) + xTempBaseSP)              /*װ�������ٶ�ֵ*//* ��������*/);

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
