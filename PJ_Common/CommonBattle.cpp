#include "Precompiled.h"


//SKILL_GROUP_TYPE GetSkillGroupTypeBySkillType(mstr xSkillName)
//{
//    switch (xSkillType)
//    {
//        case EST_�����Ը�:
//        case EST_���߲�˿:
//        case EST_���޵���:
//        case EST_�����Ϲ�:
//        case EST_�������: {return SGT_��ӡ; }break;
//
//        case EST_������:
//        case EST_�˯��:
//        case EST_�����:
//        case EST_�Ի���:
//        case EST_������: {return SGT_��˯; }break;
//
//        case EST_��Ы����:
//        case EST_׷������:
//        case EST_�ϳ���ɢ:
//        case EST_�׶����:
//        case EST_�򶾹���: {return SGT_�ж�; }break;
//
//        case EST_����֮��:
//        case EST_��������:
//        case EST_�������:
//        case EST_�赶ɱ��:
//        case EST_ʧ�Ŀ���: {return SGT_����; }break;
//
//        case EST_��������:
//        case EST_׷������:
//        case EST_ħ������:
//        case EST_����ʴ��:
//        case EST_����׷��: {return SGT_����; }break;
//
//        case EST_��֮ħ��:
//        case EST_������:
//        case EST_ʨ��֮ŭ:
//        case EST_��������:
//        case EST_ħ����: {return SGT_��ţ; }break;
//
//        case EST_��������:
//        case EST_��������:
//        case EST_��������:
//        case EST_ħ����:
//        case EST_��������: {return SGT_����; }break;
//
//        case EST_ħ֮�ɲ�:
//        case EST_����֮ħ:
//        case EST_ħ�����:
//        case EST_�����ħ:
//        case EST_Ǭ������: {return SGT_����; }break;
//
//        case EST_��������:
//        case EST_���չ⻪:
//        case EST_����ŭ��:
//        case EST_��������:
//        case EST_�������: {return SGT_�׷�; }break;
//
//        case EST_�������:
//        case EST_����ˮ��:
//        case EST_��Х����:
//        case EST_��������:
//        case EST_��������: {return SGT_ˮ��; }break;
//
//        case EST_�����һ�:
//        case EST_����ŭ��:
//        case EST_��ζ���:
//        case EST_�һ���:
//        case EST_��������: {return SGT_��; }break;
//
//        case EST_��ɰ��ʯ:
//        case EST_�˷�����:
//        case EST_̫������:
//        case EST_����ӿ��:
//        case EST_����Ǭ��: {return SGT_�編; }break;
//
//        case EST_��Ч:
//        default: {return SGT_��Ч; }break;
//    }
//}
//
//SKILL_RACIAL_GROUP_TYPE GetSkillRacialGroupTypeBySkillType(mstr xSkillName)
//{
//    switch (xSkillType)
//    {
//        case EST_�����Ը�:
//        case EST_���߲�˿:
//        case EST_���޵���:
//        case EST_�����Ϲ�:
//        case EST_�������:
//        case EST_������:
//        case EST_�˯��:
//        case EST_�����:
//        case EST_�Ի���:
//        case EST_������:
//        case EST_��Ы����:
//        case EST_׷������:
//        case EST_�ϳ���ɢ:
//        case EST_�׶����:
//        case EST_�򶾹���:
//        case EST_����֮��:
//        case EST_��������:
//        case EST_�������:
//        case EST_�赶ɱ��:
//        case EST_ʧ�Ŀ���: {return SRGT_��; }break;
//
//        case EST_��������:
//        case EST_׷������:
//        case EST_ħ������:
//        case EST_����ʴ��:
//        case EST_����׷��:
//        case EST_��֮ħ��:
//        case EST_������:
//        case EST_ʨ��֮ŭ:
//        case EST_��������:
//        case EST_ħ����:
//        case EST_��������:
//        case EST_��������:
//        case EST_��������:
//        case EST_ħ����:
//        case EST_��������:
//        case EST_ħ֮�ɲ�:
//        case EST_����֮ħ:
//        case EST_ħ�����:
//        case EST_�����ħ:
//        case EST_Ǭ������: {return SRGT_ħ; }break;
//
//        case EST_��������:
//        case EST_���չ⻪:
//        case EST_����ŭ��:
//        case EST_��������:
//        case EST_�������:
//        case EST_�������:
//        case EST_����ˮ��:
//        case EST_��Х����:
//        case EST_��������:
//        case EST_��������:
//        case EST_�����һ�:
//        case EST_����ŭ��:
//        case EST_��ζ���:
//        case EST_�һ���:
//        case EST_��������:
//        case EST_��ɰ��ʯ:
//        case EST_�˷�����:
//        case EST_̫������:
//        case EST_����ӿ��:
//        case EST_����Ǭ��: {return SRGT_��; }break;
//
//        case EST_��Ч:
//        default: {return SRGT_��Ч; }break;
//    }
//}

#pragma message("δ֪��Ϣ")
//
//BattleCommonAttr::BattleCommonAttr(role_info_attr* xAttr)
//{
//    m_Attr.CopyFrom(*xAttr);
//}
//
//
//// �ȼ�
//Int32 BattleCommonAttr::Get_Level() { return m_Attr.level(); };
//Int32 BattleCommonAttr::Update_Level(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.level() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_level(xNew); return m_Attr.level();
//};
//void BattleCommonAttr::Set_Level(Int32 xNew) { m_Attr.set_level(xNew); };
//
//
//// ��ǰѪ��
//Int32 BattleCommonAttr::Get_Cur_HP() { return m_Attr.cur_hp(); };
//Int32 BattleCommonAttr::Update_Cur_HP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.cur_hp() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_cur_hp(xNew); return m_Attr.cur_hp();
//};
//void BattleCommonAttr::Set_Cur_HP(Int32 xNew) { m_Attr.set_cur_hp(xNew); };
//
//
//// ���Ѫ��
//Int32 BattleCommonAttr::Get_Max_HP() { return m_Attr.max_hp(); };
//Int32 BattleCommonAttr::Update_Max_HP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.max_hp() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_max_hp(xNew); return m_Attr.max_hp();
//};
//void BattleCommonAttr::Set_Max_HP(Int32 xNew) { m_Attr.set_max_hp(xNew); };
//
//
//// ��ǰ����
//Int32 BattleCommonAttr::Get_Cur_MP() { return m_Attr.cur_mp(); };
//Int32 BattleCommonAttr::Update_Cur_MP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.cur_mp() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_cur_mp(xNew); return m_Attr.cur_mp();
//};
//void BattleCommonAttr::Set_Cur_MP(Int32 xNew) { m_Attr.set_cur_mp(xNew); };
//
//
//// �����
//Int32 BattleCommonAttr::Get_Max_MP() { return m_Attr.max_mp(); };
//Int32 BattleCommonAttr::Update_Max_MP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.max_mp() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_max_mp(xNew); return m_Attr.max_mp();
//};
//void BattleCommonAttr::Set_Max_MP(Int32 xNew) { m_Attr.set_max_mp(xNew); };
//
//
//// ����������
//Int32 BattleCommonAttr::Get_Min_AP() { return m_Attr.min_ap(); };
//Int32 BattleCommonAttr::Update_Min_AP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.min_ap() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_min_ap(xNew); return m_Attr.min_ap();
//};
//void BattleCommonAttr::Set_Min_AP(Int32 xNew) { m_Attr.set_min_ap(xNew); };
//
//
//// ����������
//Int32 BattleCommonAttr::Get_Max_AP() { return m_Attr.max_ap(); };
//Int32 BattleCommonAttr::Update_Max_AP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.max_ap() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_max_ap(xNew); return m_Attr.max_ap();
//};
//void BattleCommonAttr::Set_Max_AP(Int32 xNew) { m_Attr.set_max_ap(xNew); };
//
//
//// ����(�ٶ�)
//Int32 BattleCommonAttr::Get_SP() { return m_Attr.sp(); };
//Int32 BattleCommonAttr::Update_SP(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.sp() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_sp(xNew); return m_Attr.sp();
//};
//void BattleCommonAttr::Set_SP(Int32 xNew) { m_Attr.set_sp(xNew); };
//
//
//// �������
//Int32 BattleCommonAttr::Get_PhysicalDefense() { return m_Attr.physical_defense(); };
//Int32 BattleCommonAttr::Update_PhysicalDefense(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.physical_defense() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_physical_defense(xNew); return m_Attr.physical_defense();
//};
//void BattleCommonAttr::Set_PhysicalDefense(Int32 xNew) { m_Attr.set_physical_defense(xNew); };
//
//
//// �����ж���
//Int32 BattleCommonAttr::Get_BaseActionForce() { return m_Attr.base_action_force(); };
//Int32 BattleCommonAttr::Update_BaseActionForce(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.base_action_force() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_base_action_force(xNew); return m_Attr.base_action_force();
//};
//void BattleCommonAttr::Set_BaseActionForce(Int32 xNew) { m_Attr.set_base_action_force(xNew); };
//
//
//// ��ǰ�ж���
//Int32 BattleCommonAttr::Get_CurrentActionForce() { return m_Attr.current_action_force(); };
//Int32 BattleCommonAttr::Update_CurrentActionForce(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.current_action_force() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_current_action_force(xNew); return m_Attr.current_action_force();
//};
//void BattleCommonAttr::Set_CurrentActionForce(Int32 xNew) { m_Attr.set_current_action_force(xNew); };
//
//
//// �ж�������
//Int32 BattleCommonAttr::Get_FullActionForce() { return m_Attr.full_action_force(); };
//Int32 BattleCommonAttr::Update_FullActionForce(Int32 xDifference, Int32 xLimitMax, Int32 xLimitMin)
//{
//    Int32 xNew = m_Attr.full_action_force() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_full_action_force(xNew); return m_Attr.full_action_force();
//};
//void BattleCommonAttr::Set_FullActionForce(Int32 xNew) { m_Attr.set_full_action_force(xNew); };
//
//
//// �ж������ʼӳ�
//Single BattleCommonAttr::Get_ActionPlusRate() { return m_Attr.action_plus_rate(); };
//Single BattleCommonAttr::Update_ActionPlusRate(Single xDifference, Single xLimitMax, Single xLimitMin)
//{
//    Single xNew = m_Attr.action_plus_rate() + xDifference;
//    if (xNew < xLimitMin) { xNew = xLimitMin; }
//    if (xNew > xLimitMax) { xNew = xLimitMax; }
//    m_Attr.set_action_plus_rate(xNew); return m_Attr.action_plus_rate();
//};
//void BattleCommonAttr::Set_ActionPlusRate(Single xNew) { m_Attr.set_action_plus_rate(xNew); };
//
//Int32 BattleCommonAttr::AddRewardExp(Int64 xRewardExp)
//{
//    m_Attr.add_list_reward_exp(xRewardExp);
//    return m_Attr.list_reward_exp_size();
//}
//
//Int32 BattleCommonAttr::AddRewardMoney(Int64 xRewardMoney)
//{
//    m_Attr.add_list_reward_money(xRewardMoney);
//    return m_Attr.list_reward_money_size();
//}
//
//void BattleCommonAttr::AddRewardItem(Int64 xRewardItemIndex, item& xItem)
//{
//    var* xTemp = m_Attr.add_list_reward_item();
//    xTemp->CopyFrom(xItem);
//    xTemp->set_item_index(xRewardItemIndex);
//}

BattleCommonSkill::BattleCommonSkill(BattleCommonUnit* xSrcObject, PJC_Skill* xSkill, LPCSTR xSkillName)
    :m_SrcObject(xSrcObject)
    , m_Skill(xSkill)
    , m_SkillName(xSkillName)
{
    //m_SkillActionInfo;
    //m_TargetList;
}

BattleCommonSkill::~BattleCommonSkill()
{
    ;
}



//void BattleCommonAttr::Init(void)
//{
//    m_ActionPlusRate = 1.0f;
//}
//
//void BattleCommonAttr::LoadAttr()
//{
//    this->SetAttr(EOAT_MAX_HP, RAND_INT32(5000, 16000), 1, 10000000);
//    this->SetAttr(EOAT_CUR_HP, this->GetAttr(EOAT_MAX_HP), 0, this->GetAttr(EOAT_MAX_HP));
//
//    this->SetAttr(EOAT_LEVEL, RAND_INT32(1, 8), 0, 60);
//
//    this->SetAttr(EOAT_MAX_MP, 150, 0, 150);
//    this->SetAttr(EOAT_CUR_MP, 150, 0, 150);
//
//    this->SetAttr(EOAT_MIN_AP, RAND_INT32(100, 900), 1, 100000);
//    this->SetAttr(EOAT_MAX_AP, this->GetAttr(EOAT_MIN_AP) + RAND_INT32(100, 200), 1, 100000);
//
//    this->SetAttr(EOAT_SP, RAND_INT32(1, 100), 0, 2000);
//
//    this->SetAttr(EOAT_PHYSICAL_DEFENSE, RAND_INT32(50, 500), 0, 50000);
//
//    this->SetAttr(EOAT_BASE_ACTION_FORCE, 100, 100, 100);
//    this->SetAttr(EOAT_CURRENT_ACTION_FORCE, 0, 0, 3000);
//    this->SetAttr(EOAT_FULL_ACTION_FORCE, 3000, 3000, 3000);
//}
//
//Int32 BattleCommonAttr::GetAttr(E_OBJECT_ATTR_TYPE xAttrType)
//{
//    return m_Attr[xAttrType];
//}
//
//Int32 BattleCommonAttr::AddAttr(E_OBJECT_ATTR_TYPE xAttrType, Int32 xNewValue, Int32 xMinValue, Int32 xMaxValue)
//{
//    m_Attr[xAttrType] += xNewValue;
//    if (m_Attr[xAttrType] < xMinValue) { m_Attr[xAttrType] = xMinValue; }
//    if (m_Attr[xAttrType] > xMaxValue) { m_Attr[xAttrType] = xMaxValue; }
//    return this->GetAttr(xAttrType);
//}
//
//Int32 BattleCommonAttr::SubAttr(E_OBJECT_ATTR_TYPE xAttrType, Int32 xNewValue, Int32 xMinValue, Int32 xMaxValue)
//{
//    m_Attr[xAttrType] -= xNewValue;
//    if (m_Attr[xAttrType] < xMinValue) { m_Attr[xAttrType] = xMinValue; }
//    if (m_Attr[xAttrType] > xMaxValue) { m_Attr[xAttrType] = xMaxValue; }
//    return this->GetAttr(xAttrType);
//}
//
//Int32 BattleCommonAttr::SetAttr(E_OBJECT_ATTR_TYPE xAttrType, Int32 xNewValue, Int32 xMinValue, Int32 xMaxValue)
//{
//    m_Attr[xAttrType] = xNewValue;
//    if (m_Attr[xAttrType] < xMinValue) { m_Attr[xAttrType] = xMinValue; }
//    if (m_Attr[xAttrType] > xMaxValue) { m_Attr[xAttrType] = xMaxValue; }
//    return this->GetAttr(xAttrType);
//}
//
//Int32 BattleCommonAttr::UpdateAttr(E_OBJECT_ATTR_TYPE xAttrType, Int32 xNewValue)
//{
//    m_Attr[xAttrType] = xNewValue;
//    return this->GetAttr(xAttrType);
//}
//
//







//
//
//void CommonBattleBuffInfo::Init(void)
//{
//
//}
//
//
//void CommonBattleBuffInfo::OnBegin(Int64 xSumTickNum)
//{
//    ;
//}
//
//void CommonBattleBuffInfo::FrameTick(Int64 xNewTickNum, Int64 xSumTickNum)
//{
//    ;
//}
//
//void CommonBattleBuffInfo::Update(Int64 xNow)
//{
//    ;
//}
//
//void CommonBattleBuffInfo::OnEnd(Int64 xSumTickNum)
//{
//    ;
//}
//
//Boolean CommonBattleBuffInfo::IsComplete(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ���Բ��Ŷ���
//Boolean CommonBattleBuffInfo::OnCanPlayAnimation(Int64 xNow)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ���Ա�ѡ��
//Boolean CommonBattleBuffInfo::OnCanSelect(Int64 xNow)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ������
//Boolean CommonBattleBuffInfo::OnCanHurt(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ���������ж���
//Boolean CommonBattleBuffInfo::OnCanIncreaseAction(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ����ʹ�õ���
//Boolean CommonBattleBuffInfo::OnCanUseItem(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ����������
//Boolean CommonBattleBuffInfo::OnCanAttack(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ���Ա���������
//Boolean CommonBattleBuffInfo::OnCanProtectOther(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ��������
//Boolean CommonBattleBuffInfo::OnCanEscape(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// �жϼ���¼�,�Ƿ����ʹ�÷���
//Boolean CommonBattleBuffInfo::OnCanUseSkill(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// ���¼�,������ʱ
//Boolean CommonBattleBuffInfo::OnContinueAttack(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// ���¼�,ʹ�÷���ʱ
//Boolean CommonBattleBuffInfo::OnContinueUseSkill(Int64 xNow, BattleCommonSkill* xBattleSkill)
//{
//    return True;
//}
//
//// ���¼�,����˺�ʱ
//Boolean CommonBattleBuffInfo::OnContinueCauseDamage(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// ���¼�,�ж���ʼʱ
//Boolean CommonBattleBuffInfo::OnContinueActionBegin(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// ���¼�,�ж�����ʱ
//Boolean CommonBattleBuffInfo::OnContinueActionEnd(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// ���¼�,����ʱ
//Boolean CommonBattleBuffInfo::OnContinueDamage(Int64 xSumTickNum, Int32& xDamage_HP, Int32& xDamage_MP)
//{
//    return True;
//}
//
//// ���¼�,��ȡ�ӳɱ���ʱ
//Boolean CommonBattleBuffInfo::OnContinueGetPlusRatio(Int64 xNow, Single& xOutNewValue)
//{
//    return True;
//}
//
//// ���¼�,��ȡ����ֵʱ
//Boolean CommonBattleBuffInfo::OnContinueGetDefense(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// ���¼�,��ȡ����ʱ
//Boolean CommonBattleBuffInfo::OnContinueGetAttr(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//




BattleCommonBuff::BattleCommonBuff(BattleCommonUnit* xSrcBattleCommonUnit, Int64 xSurplusTickNum)
    : m_SrcBattleCommonUnit(xSrcBattleCommonUnit)
    , m_SurplusTickNum(xSurplusTickNum)
    , m_IsComplete(False)
{
    ;
};

BattleCommonBuff::~BattleCommonBuff()
{
    ;
};



//void CommonBattleEventInfo::Init(void)
//{
//
//}
//
//
//
//void BattleCommonSkill::Init(void)
//{
//
//}



BattleCommonUnit::BattleCommonUnit(UD_RoleAttr& xRoleAttr)
    : m_RoleAttr(xRoleAttr)
    , m_ReserveActionForce(0)
{
    //m_RoleInfo = NEW ProtoAssist_RoleInfo();
    //m_RoleInfo->UpdateProtoRole(xRoleInfo);
}

BattleCommonUnit::~BattleCommonUnit()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_BattleBuff);
    SAFE_ENUMERATOR_DELETE(m_Dict_BattleSkill);
    //SAFE_DELETE(m_RoleInfo);
}

// �Ƿ��ֹ���Ŷ���
Boolean BattleCommonUnit::IsNoPlayAnimation(Int64 xNow) {
    return False;
};

// �Ƿ��ֹ��ѡ��
Boolean BattleCommonUnit::IsNoSelect(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ�ж�
Boolean BattleCommonUnit::IsNoAction(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ����
Boolean BattleCommonUnit::IsNoHurt(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ�����ж���
Boolean BattleCommonUnit::IsNoIncreaseAction(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹʹ�õ���
Boolean BattleCommonUnit::IsNoUseItem(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ����������
Boolean BattleCommonUnit::IsNoUnderAttack(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ����������
Boolean BattleCommonUnit::IsNoProtectOther(Int64 xSumTickNum) {
    return False;
};

// �Ƿ��ֹ����
Boolean BattleCommonUnit::IsNoEscape(Int64 xSumTickNum) {
    return False;
};

// �Ƿ����ʹ�÷���
Boolean BattleCommonUnit::IsNoUseSkill(Int64 xSumTickNum) {
    return False;
};

// �Ƿ������ж����
Boolean BattleCommonUnit::IsNoActionCheck(Int64 xSumTickNum) {
    return False;
};

// ������ʱ
Boolean BattleCommonUnit::OnAttack(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// ʹ�÷���ʱ
Boolean BattleCommonUnit::OnUseSkill(Int64 xSumTickNum, BattleCommonSkill* xBattleSkill) {
    return True;
};

// ����˺�ʱ
Boolean BattleCommonUnit::OnCauseDamage(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// �ж���ʼʱ
Boolean BattleCommonUnit::OnActionBegin(Int64 xSumTickNum) {
    return True;
};

// �ж�����ʱ
Boolean BattleCommonUnit::OnActionEnd(Int64 xSumTickNum) {
    return True;
};

// ����ʱ
Boolean BattleCommonUnit::OnHurt(Int64 xSumTickNum, BUFF_ID xBuffId, mstr xSkillName, Int32& xDamage_HP, Int32& xDamage_MP) {
    return True;
};

// ��ȡ�ӳɱ���ʱ
Boolean BattleCommonUnit::OnGetPlusRatio(Int64 xSumTickNum, Single& xOutNewValue) {
    return True;
};

// ��ȡ����ֵʱ
Boolean BattleCommonUnit::OnGetDefense(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// ��ȡ����ʱ
Boolean BattleCommonUnit::OnGetAttr(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

//
//// �Ƿ���Բ��Ŷ���
//Boolean BattleCommonUnit::CanPlayAnimation(Int64 xNow)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanPlayAnimation(xNow))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ���Ա�ѡ��
//Boolean BattleCommonUnit::CanSelect(Int64 xNow)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanSelect(xNow))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ������ж�
//Boolean BattleCommonUnit::IsJumpAction(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (xBattleCommonBuff->OnCanJumpAction(xSumTickNum))
//        {
//            return True;
//        }
//    }
//    return False;
//}
//
//// �Ƿ�����ж�
//Boolean BattleCommonUnit::IsCanAction(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanAction(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ������
//Boolean BattleCommonUnit::CanHurt(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanHurt(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ���������ж���
//Boolean BattleCommonUnit::IsCanIncreaseAction(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanIncreaseAction(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//Boolean BattleCommonUnit::OnContinueActionBegin(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnContinueActionBegin(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ����ʹ�õ���
//Boolean BattleCommonUnit::CanUseItem(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanUseItem(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ����������
//Boolean BattleCommonUnit::CanUnderAttack(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanAttack(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// ����������
//Boolean BattleCommonUnit::OnContinueAttack(Int64 xSumTickNum, Int32& xSumOutNewValue)
//{
//#pragma message("δ֪��Ϣ")
//    //xSumOutNewValue = RAND_INT32(m_Attr.Get_Min_AP(), m_Attr.Get_Max_AP());
//    //FAST_FOREACH(m_Dict_BattleBuff)
//    //{
//    //    BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//    //    Int32 xOutNewValue = 0;
//    //    if (xBattleCommonBuff->OnContinueAttack(xSumTickNum, xOutNewValue))
//    //    {
//    //        xSumOutNewValue += xOutNewValue;
//    //    }
//    //}
//    return True;
//}
//
//// �Ƿ���Ա���������
//Boolean BattleCommonUnit::IsCanProtectOther(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanProtectOther(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}

Boolean BattleCommonUnit::IsFriend(BattleCommonUnit* xSrcUnit, BattleCommonUnit* xTarUnit)
{
    if (xSrcUnit && xTarUnit)
    {
        return xTarUnit->m_RoleAttr.GetTeamIndex() == xSrcUnit->m_RoleAttr.GetTeamIndex();
    }
    return False;
}
//
//// �Ƿ��������
//Boolean BattleCommonUnit::IsCanEscape(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanEscape(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}
//
//// �Ƿ����ʹ�÷���
//Boolean BattleCommonUnit::CanUseSkill(Int64 xSumTickNum)
//{
//    FAST_FOREACH(m_Dict_BattleBuff)
//    {
//        BattleCommonBuff* xBattleCommonBuff = xEnumValue;
//        if (!xBattleCommonBuff->OnCanUseSkill(xSumTickNum))
//        {
//            return False;
//        }
//    }
//    return True;
//}

Boolean BattleCommonUnit::CalculationActionForce(Int64 xSumTickNum)
{
#pragma message("δ֪��Ϣ")
    //// ���������ж�ֵ
    //Single xActionValue = (Single)((m_Attr.Get_BaseActionForce() + m_Attr.Get_SP()) * 1);
    //xActionValue *= m_Attr.Get_ActionPlusRate();

    //if (this->IsCanIncreaseAction(xSumTickNum))
    //{
    //    //if (!this->IsCanAction(xSumTickNum))
    //    {
    //        m_Attr.Update_CurrentActionForce(m_ReserveActionForce + (Int32)xActionValue, INT32_MAX);
    //        m_ReserveActionForce = 0;
    //        //m_Attr.Update_CurrentActionForce((Int32)xActionValue, INT32_MAX m_Attr.Get_FullActionForce() * 2);
    //    }
    //}

    //// ����Ƿ���Ҫ�ж�
    //return (m_Attr.Get_CurrentActionForce() >= m_Attr.Get_FullActionForce());
    return False;
}

Boolean BattleCommonUnit::CalculationActionForce(UD_RoleAttr& xRoleAttr, Int64 xSumTickNum)
{
#pragma message("δ֪��Ϣ")
    //// ���������ж�ֵ
    //Single xActionValue = (Single)((xRoleInfoCommonAttr.Get_BaseActionForce() + xRoleInfoCommonAttr.Get_SP()) * 1);
    //xActionValue *= xRoleInfoCommonAttr.Get_ActionPlusRate();

    //if (this->IsCanIncreaseAction(xSumTickNum))
    //{
    //    //if (!this->IsCanAction(xSumTickNum))
    //    {
    //        m_Attr.Update_CurrentActionForce((Int32)xActionValue, INT32_MAX);
    //        //xRoleInfoCommonAttr.Update_CurrentActionForce((Int32)xActionValue, xRoleInfoCommonAttr.Get_FullActionForce() * 2);
    //    }
    //}
    //return xRoleInfoCommonAttr.Get_CurrentActionForce() >= xRoleInfoCommonAttr.Get_FullActionForce();
    return False;
}



Boolean BattleCommonUnit::AddBuff(Int64 xSumTickNum, BattleCommonBuff* xBuff)
{
    this->RemoveBuff(xSumTickNum, xBuff->m_BuffId);
    m_Dict_BattleBuff.Add(xBuff->m_BuffId, xBuff);
    xBuff->OnBegin(xSumTickNum);
    return True;
}

Boolean BattleCommonUnit::RemoveBuff(Int64 xSumTickNum, BUFF_ID xBuffId)
{
    if (m_Dict_BattleBuff.ContainsKey(xBuffId))
    {
        var* xBuff = m_Dict_BattleBuff[xBuffId];
        m_Dict_BattleBuff.Remove(xBuffId);
        xBuff->OnEnd(xSumTickNum);
        SAFE_DELETE(xBuff);
        return True;
    }
    return False;
}

void BattleCommonUnit::RemoveAllBuff(Int64 xSumTickNum, BUFF_TYPE xBuffType)
{
    var xTemp = m_Dict_BattleBuff;
    FAST_FOREACH(xTemp)
    {
        if (xTemp.Value()->m_BuffType == xBuffType)
        {
            this->RemoveBuff(xSumTickNum, xTemp.GetKey());
        }
    }
}

void BattleCommonUnit::RemoveAllBuff(Int64 xSumTickNum)
{
    var xTemp = m_Dict_BattleBuff;
    FAST_FOREACH(xTemp)
    {
        this->RemoveBuff(xSumTickNum, xTemp.GetKey());
    }
}


void BattleCommonUnit::CheckNeedInstantlyRemoveBuff(Int64 xNewTickNum, Int64 xSumTickNum)
{
    MsList<BattleCommonBuff*> xTemp;
    FAST_FOREACH(m_Dict_BattleBuff) { if (m_Dict_BattleBuff.Value()->m_NeedInstantlyRemove) { xTemp.Add(m_Dict_BattleBuff.Value()); } }
    FAST_FOREACH(xTemp) { this->RemoveBuff(xSumTickNum, xTemp.Value()->m_BuffId); }
}

Boolean BattleCommonUnit::IsDead()
{
#pragma message("δ֪��Ϣ")
    return False;
    //return m_Attr.Get_Cur_HP() <= 0;
}


void BattleCommonUnit::AddRewardExp(Int64 xRewardExp)
{
#pragma message("δ֪��Ϣ")
    //m_Attr.AddRewardExp(xRewardExp);
}

void BattleCommonUnit::AddRewardMoney(Int64 xRewardMoney)
{
#pragma message("δ֪��Ϣ")
    //m_Attr.AddRewardMoney(xRewardMoney);
}

void BattleCommonUnit::AddRewardItem(Int64 xRewardItemIndex, item& xItem)
{
#pragma message("δ֪��Ϣ")
    //m_Attr.AddRewardItem(xRewardItemIndex, xItem);
}

void BattleCommonUnit::AddSkill(LPCSTR xSkillName, BattleCommonSkill* xBattleCommonSkill)
{
    if (!m_Dict_BattleSkill.ContainsKey(xSkillName))
    {
        m_Dict_BattleSkill[xSkillName] = xBattleCommonSkill;
    }
    else
    {
        AssertLog("�Ѿ�����");
        SAFE_DELETE(xBattleCommonSkill);
    }
}

MsVector<BattleCommonUnit*> BattleCommonSkill::FlagSelectTargetList(Int64 xNow, BattleCommonUnit* xSelectTarget, MsUnorderedDictionary<Int64, UI_Key>& xDict_TargetFlag)
{
    return MsVector<BattleCommonUnit*>();
}

MsVector<BattleCommonUnit*> BattleCommonSkill::RealSelectTargetList(Int64 xSumTickNum, BattleCommonUnit* xSelectTarget)
{
    return MsVector<BattleCommonUnit*>();
}

void BattleCommonSkill::FilterObject(Int64 xSumTickNum, MsVector<BattleCommonUnit*>& xTargetList)
{
    MsVector<BattleCommonUnit*> xTempTargetList;
    FAST_FOREACH(xTargetList)
    {
        if (!xTargetList.Value()->IsDead() && !xTargetList.Value()->IsNoSelect(xSumTickNum))
        {
            xTempTargetList.Add(xTargetList.Value());
        }
    }
    xTargetList = xTempTargetList;
}

