#include "Precompiled.h"


//SKILL_GROUP_TYPE GetSkillGroupTypeBySkillType(mstr xSkillName)
//{
//    switch (xSkillType)
//    {
//        case EST_作茧自缚:
//        case EST_金蛇缠丝:
//        case EST_天罗地网:
//        case EST_作壁上观:
//        case EST_四面楚歌: {return SGT_封印; }break;
//
//        case EST_催眠咒:
//        case EST_瞌睡咒:
//        case EST_离魂咒:
//        case EST_迷魂醉:
//        case EST_百日眠: {return SGT_昏睡; }break;
//
//        case EST_蛇蝎美人:
//        case EST_追魂迷香:
//        case EST_断肠烈散:
//        case EST_鹤顶红粉:
//        case EST_万毒攻心: {return SGT_中毒; }break;
//
//        case EST_反间之计:
//        case EST_情真意切:
//        case EST_谗言相加:
//        case EST_借刀杀人:
//        case EST_失心狂乱: {return SGT_混乱; }break;
//
//        case EST_夺命勾魂:
//        case EST_追神摄魄:
//        case EST_魔音摄心:
//        case EST_销魂蚀骨:
//        case EST_阎罗追命: {return SGT_震摄; }break;
//
//        case EST_妖之魔力:
//        case EST_力神复苏:
//        case EST_狮王之怒:
//        case EST_兽王神力:
//        case EST_魔神附身: {return SGT_加牛; }break;
//
//        case EST_红袖添香:
//        case EST_莲步轻舞:
//        case EST_楚楚可怜:
//        case EST_魔神护体:
//        case EST_含情脉脉: {return SGT_伤盘; }break;
//
//        case EST_魔之飞步:
//        case EST_急速之魔:
//        case EST_魔神飞舞:
//        case EST_天外飞魔:
//        case EST_乾坤借速: {return SGT_提速; }break;
//
//        case EST_雷霆霹雳:
//        case EST_日照光华:
//        case EST_雷神怒击:
//        case EST_电闪雷鸣:
//        case EST_天诛地灭: {return SGT_雷法; }break;
//
//        case EST_龙卷雨击:
//        case EST_龙腾水溅:
//        case EST_龙啸九天:
//        case EST_蛟龙出海:
//        case EST_九龙冰封: {return SGT_水法; }break;
//
//        case EST_地狱烈火:
//        case EST_天雷怒火:
//        case EST_三味真火:
//        case EST_烈火骄阳:
//        case EST_九阴纯火: {return SGT_火法; }break;
//
//        case EST_飞砂走石:
//        case EST_乘风破浪:
//        case EST_太乙生风:
//        case EST_风雷涌动:
//        case EST_袖里乾坤: {return SGT_风法; }break;
//
//        case EST_无效:
//        default: {return SGT_无效; }break;
//    }
//}
//
//SKILL_RACIAL_GROUP_TYPE GetSkillRacialGroupTypeBySkillType(mstr xSkillName)
//{
//    switch (xSkillType)
//    {
//        case EST_作茧自缚:
//        case EST_金蛇缠丝:
//        case EST_天罗地网:
//        case EST_作壁上观:
//        case EST_四面楚歌:
//        case EST_催眠咒:
//        case EST_瞌睡咒:
//        case EST_离魂咒:
//        case EST_迷魂醉:
//        case EST_百日眠:
//        case EST_蛇蝎美人:
//        case EST_追魂迷香:
//        case EST_断肠烈散:
//        case EST_鹤顶红粉:
//        case EST_万毒攻心:
//        case EST_反间之计:
//        case EST_情真意切:
//        case EST_谗言相加:
//        case EST_借刀杀人:
//        case EST_失心狂乱: {return SRGT_人; }break;
//
//        case EST_夺命勾魂:
//        case EST_追神摄魄:
//        case EST_魔音摄心:
//        case EST_销魂蚀骨:
//        case EST_阎罗追命:
//        case EST_妖之魔力:
//        case EST_力神复苏:
//        case EST_狮王之怒:
//        case EST_兽王神力:
//        case EST_魔神附身:
//        case EST_红袖添香:
//        case EST_莲步轻舞:
//        case EST_楚楚可怜:
//        case EST_魔神护体:
//        case EST_含情脉脉:
//        case EST_魔之飞步:
//        case EST_急速之魔:
//        case EST_魔神飞舞:
//        case EST_天外飞魔:
//        case EST_乾坤借速: {return SRGT_魔; }break;
//
//        case EST_雷霆霹雳:
//        case EST_日照光华:
//        case EST_雷神怒击:
//        case EST_电闪雷鸣:
//        case EST_天诛地灭:
//        case EST_龙卷雨击:
//        case EST_龙腾水溅:
//        case EST_龙啸九天:
//        case EST_蛟龙出海:
//        case EST_九龙冰封:
//        case EST_地狱烈火:
//        case EST_天雷怒火:
//        case EST_三味真火:
//        case EST_烈火骄阳:
//        case EST_九阴纯火:
//        case EST_飞砂走石:
//        case EST_乘风破浪:
//        case EST_太乙生风:
//        case EST_风雷涌动:
//        case EST_袖里乾坤: {return SRGT_仙; }break;
//
//        case EST_无效:
//        default: {return SRGT_无效; }break;
//    }
//}

#pragma message("未知消息")
//
//BattleCommonAttr::BattleCommonAttr(role_info_attr* xAttr)
//{
//    m_Attr.CopyFrom(*xAttr);
//}
//
//
//// 等级
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
//// 当前血量
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
//// 最大血量
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
//// 当前法力
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
//// 最大法力
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
//// 物理攻击下限
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
//// 物理攻击上限
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
//// 敏捷(速度)
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
//// 物理防御
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
//// 基础行动力
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
//// 当前行动力
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
//// 行动力上限
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
//// 行动力速率加成
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
//// 判断检测事件,是否可以播放动画
//Boolean CommonBattleBuffInfo::OnCanPlayAnimation(Int64 xNow)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以被选中
//Boolean CommonBattleBuffInfo::OnCanSelect(Int64 xNow)
//{
//    return True;
//}
//
//// 判断检测事件,是否会受伤
//Boolean CommonBattleBuffInfo::OnCanHurt(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以增长行动力
//Boolean CommonBattleBuffInfo::OnCanIncreaseAction(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以使用道具
//Boolean CommonBattleBuffInfo::OnCanUseItem(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以物理攻击
//Boolean CommonBattleBuffInfo::OnCanAttack(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以保护其他人
//Boolean CommonBattleBuffInfo::OnCanProtectOther(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以逃跑
//Boolean CommonBattleBuffInfo::OnCanEscape(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 判断检测事件,是否可以使用法术
//Boolean CommonBattleBuffInfo::OnCanUseSkill(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 纯事件,物理攻击时
//Boolean CommonBattleBuffInfo::OnContinueAttack(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// 纯事件,使用法术时
//Boolean CommonBattleBuffInfo::OnContinueUseSkill(Int64 xNow, BattleCommonSkill* xBattleSkill)
//{
//    return True;
//}
//
//// 纯事件,造成伤害时
//Boolean CommonBattleBuffInfo::OnContinueCauseDamage(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// 纯事件,行动开始时
//Boolean CommonBattleBuffInfo::OnContinueActionBegin(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 纯事件,行动结束时
//Boolean CommonBattleBuffInfo::OnContinueActionEnd(Int64 xSumTickNum)
//{
//    return True;
//}
//
//// 纯事件,受伤时
//Boolean CommonBattleBuffInfo::OnContinueDamage(Int64 xSumTickNum, Int32& xDamage_HP, Int32& xDamage_MP)
//{
//    return True;
//}
//
//// 纯事件,获取加成比例时
//Boolean CommonBattleBuffInfo::OnContinueGetPlusRatio(Int64 xNow, Single& xOutNewValue)
//{
//    return True;
//}
//
//// 纯事件,获取防御值时
//Boolean CommonBattleBuffInfo::OnContinueGetDefense(Int64 xNow, Int32& xOutNewValue)
//{
//    return True;
//}
//
//// 纯事件,获取属性时
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

// 是否禁止播放动画
Boolean BattleCommonUnit::IsNoPlayAnimation(Int64 xNow) {
    return False;
};

// 是否禁止被选中
Boolean BattleCommonUnit::IsNoSelect(Int64 xSumTickNum) {
    return False;
};

// 是否禁止行动
Boolean BattleCommonUnit::IsNoAction(Int64 xSumTickNum) {
    return False;
};

// 是否禁止受伤
Boolean BattleCommonUnit::IsNoHurt(Int64 xSumTickNum) {
    return False;
};

// 是否禁止增长行动力
Boolean BattleCommonUnit::IsNoIncreaseAction(Int64 xSumTickNum) {
    return False;
};

// 是否禁止使用道具
Boolean BattleCommonUnit::IsNoUseItem(Int64 xSumTickNum) {
    return False;
};

// 是否禁止遭受物理攻击
Boolean BattleCommonUnit::IsNoUnderAttack(Int64 xSumTickNum) {
    return False;
};

// 是否禁止保护其他人
Boolean BattleCommonUnit::IsNoProtectOther(Int64 xSumTickNum) {
    return False;
};

// 是否禁止逃跑
Boolean BattleCommonUnit::IsNoEscape(Int64 xSumTickNum) {
    return False;
};

// 是否禁用使用法术
Boolean BattleCommonUnit::IsNoUseSkill(Int64 xSumTickNum) {
    return False;
};

// 是否跳过行动检测
Boolean BattleCommonUnit::IsNoActionCheck(Int64 xSumTickNum) {
    return False;
};

// 物理攻击时
Boolean BattleCommonUnit::OnAttack(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// 使用法术时
Boolean BattleCommonUnit::OnUseSkill(Int64 xSumTickNum, BattleCommonSkill* xBattleSkill) {
    return True;
};

// 造成伤害时
Boolean BattleCommonUnit::OnCauseDamage(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// 行动开始时
Boolean BattleCommonUnit::OnActionBegin(Int64 xSumTickNum) {
    return True;
};

// 行动结束时
Boolean BattleCommonUnit::OnActionEnd(Int64 xSumTickNum) {
    return True;
};

// 受伤时
Boolean BattleCommonUnit::OnHurt(Int64 xSumTickNum, BUFF_ID xBuffId, mstr xSkillName, Int32& xDamage_HP, Int32& xDamage_MP) {
    return True;
};

// 获取加成比例时
Boolean BattleCommonUnit::OnGetPlusRatio(Int64 xSumTickNum, Single& xOutNewValue) {
    return True;
};

// 获取防御值时
Boolean BattleCommonUnit::OnGetDefense(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

// 获取属性时
Boolean BattleCommonUnit::OnGetAttr(Int64 xSumTickNum, Int32& xOutNewValue) {
    return True;
};

//
//// 是否可以播放动画
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
//// 是否可以被选中
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
//// 是否跳过行动
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
//// 是否可以行动
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
//// 是否会受伤
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
//// 是否可以增长行动力
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
//// 是否可以使用道具
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
//// 是否可以物理攻击
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
//// 修正物理攻击
//Boolean BattleCommonUnit::OnContinueAttack(Int64 xSumTickNum, Int32& xSumOutNewValue)
//{
//#pragma message("未知消息")
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
//// 是否可以保护其他人
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
//// 是否可以逃跑
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
//// 是否可以使用法术
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
#pragma message("未知消息")
    //// 计算新增行动值
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

    //// 检测是否需要行动
    //return (m_Attr.Get_CurrentActionForce() >= m_Attr.Get_FullActionForce());
    return False;
}

Boolean BattleCommonUnit::CalculationActionForce(UD_RoleAttr& xRoleAttr, Int64 xSumTickNum)
{
#pragma message("未知消息")
    //// 计算新增行动值
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
#pragma message("未知消息")
    return False;
    //return m_Attr.Get_Cur_HP() <= 0;
}


void BattleCommonUnit::AddRewardExp(Int64 xRewardExp)
{
#pragma message("未知消息")
    //m_Attr.AddRewardExp(xRewardExp);
}

void BattleCommonUnit::AddRewardMoney(Int64 xRewardMoney)
{
#pragma message("未知消息")
    //m_Attr.AddRewardMoney(xRewardMoney);
}

void BattleCommonUnit::AddRewardItem(Int64 xRewardItemIndex, item& xItem)
{
#pragma message("未知消息")
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
        AssertLog("已经存在");
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

