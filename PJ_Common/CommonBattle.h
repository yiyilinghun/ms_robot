#pragma once
//class BattleCommonAttr;
//class BattleCommonBuff;
class BattleCommonUnit;
class BattleCommonSkill;

// 战斗状态
class BattleCommonBuff
{
public:
    BattleCommonBuff(BattleCommonUnit* xSrcBattleCommonUnit, Int64 xSurplusTickNum);
    virtual ~BattleCommonBuff();

    virtual void OnBegin(Int64 xSumTickNum) = 0;
    virtual void OnEnd(Int64 xSumTickNum) = 0;
    virtual void Update(Int64 xNow) = 0;
    virtual void FrameTick(Int64 xSumTickNum) = 0;
    virtual Boolean IsComplete(Int64 xSumTickNum) = 0;

    virtual Boolean IsNoPlayAnimation(Int64 xNow) { return False; };        // 是否禁止播放动画
    virtual Boolean IsNoSelect(Int64 xSumTickNum) { return False; };        // 是否禁止被选中
    virtual Boolean IsNoAction(Int64 xSumTickNum) { return False; };        // 是否禁止行动
    virtual Boolean IsNoHurt(Int64 xSumTickNum) { return False; };          // 是否禁止受伤
    virtual Boolean IsNoIncreaseAction(Int64 xSumTickNum) { return False; };// 是否禁止增长行动力
    virtual Boolean IsNoUseItem(Int64 xSumTickNum) { return False; };       // 是否禁止使用道具
    virtual Boolean IsNoUnderAttack(Int64 xSumTickNum) { return False; };   // 是否禁止遭受物理攻击
    virtual Boolean IsNoProtectOther(Int64 xSumTickNum) { return False; };  // 是否禁止保护其他人
    virtual Boolean IsNoEscape(Int64 xSumTickNum) { return False; };        // 是否禁止逃跑
    virtual Boolean IsNoUseSkill(Int64 xSumTickNum) { return False; };      // 是否禁用使用法术
    virtual Boolean IsNoActionCheck(Int64 xSumTickNum) { return False; };   // 是否禁止行动检测

    // 事件
    virtual Boolean OnAttack(Int64 xSumTickNum, Int32& xOutNewValue) { return True; };                   // 物理攻击时
    virtual Boolean OnUseSkill(Int64 xSumTickNum, BattleCommonSkill* xBattleSkill) { return True; };     // 使用法术时
    virtual Boolean OnCauseDamage(Int64 xSumTickNum, Int32& xOutNewValue) { return True; };              // 造成伤害时
    virtual Boolean OnActionBegin(Int64 xSumTickNum) { return True; };                                   // 行动开始时
    virtual Boolean OnActionEnd(Int64 xSumTickNum) { return True; };                                     // 行动结束时

    // 受伤时
    virtual Boolean OnHurt(Int64 xSumTickNum, BUFF_ID xBuffId, mstr xSkillName, Int32& xDamage_HP, Int32& xDamage_MP) { return True; };
    virtual Boolean OnGetPlusRatio(Int64 xSumTickNum, Single& xOutNewValue) { return True; };            // 获取加成比例时
    virtual Boolean OnGetDefense(Int64 xSumTickNum, Int32& xOutNewValue) { return True; };               // 获取防御值时
    virtual Boolean OnGetAttr(Int64 xSumTickNum, Int32& xOutNewValue) { return True; };                  // 获取属性时

public:
    BUFF_TYPE           m_BuffType;
    BUFF_ID             m_BuffId;
    Boolean             m_IsReplace;
    Boolean             m_IsComplete;
    Boolean             m_NeedInstantlyRemove;
    Int64               m_BeginTickNum;
    Int64               m_SurplusTickNum;
    BattleCommonUnit*   m_SrcBattleCommonUnit;
};






// 战斗单元
class BattleCommonUnit
{
public:
    BattleCommonUnit(UD_RoleAttr& xRoleAttr);
    virtual ~BattleCommonUnit();

    inline Boolean CanPlayAnimation(Int64 xNow) { return !this->IsNoPlayAnimation(xNow); }
    inline Boolean CanSelect(Int64 xSumTickNum) { return !this->IsNoSelect(xSumTickNum); }
    inline Boolean CanAction(Int64 xSumTickNum) { return !this->IsNoAction(xSumTickNum); }
    inline Boolean CanHurt(Int64 xSumTickNum) { return !this->IsNoHurt(xSumTickNum); }
    inline Boolean CanIncreaseAction(Int64 xSumTickNum) { return !this->IsNoIncreaseAction(xSumTickNum); }
    inline Boolean CanUseItem(Int64 xSumTickNum) { return !this->IsNoUseItem(xSumTickNum); }
    inline Boolean CanUnderAttack(Int64 xSumTickNum) { return !this->IsNoUnderAttack(xSumTickNum); }
    inline Boolean CanProtectOther(Int64 xSumTickNum) { return !this->IsNoProtectOther(xSumTickNum); }
    inline Boolean CanEscape(Int64 xSumTickNum) { return !this->IsNoEscape(xSumTickNum); }
    inline Boolean CanUseSkill(Int64 xSumTickNum) { return !this->IsNoUseSkill(xSumTickNum); }
    inline Boolean CanActionCheck(Int64 xSumTickNum) { return !this->IsNoActionCheck(xSumTickNum); }

    virtual Boolean IsNoPlayAnimation(Int64 xNow);        // 是否禁止播放动画
    virtual Boolean IsNoSelect(Int64 xSumTickNum);        // 是否禁止被选中
    virtual Boolean IsNoAction(Int64 xSumTickNum);        // 是否禁止行动
    virtual Boolean IsNoHurt(Int64 xSumTickNum);          // 是否禁止受伤
    virtual Boolean IsNoIncreaseAction(Int64 xSumTickNum);// 是否禁止增长行动力
    virtual Boolean IsNoUseItem(Int64 xSumTickNum);       // 是否禁止使用道具
    virtual Boolean IsNoUnderAttack(Int64 xSumTickNum);   // 是否禁止遭受物理攻击
    virtual Boolean IsNoProtectOther(Int64 xSumTickNum);  // 是否禁止保护其他人
    virtual Boolean IsNoEscape(Int64 xSumTickNum);        // 是否禁止逃跑
    virtual Boolean IsNoUseSkill(Int64 xSumTickNum);      // 是否禁用使用法术
    virtual Boolean IsNoActionCheck(Int64 xSumTickNum);   // 是否禁止行动检测

    // 事件
    virtual Boolean OnAttack(Int64 xSumTickNum, Int32& xOutNewValue);                   // 物理攻击时
    virtual Boolean OnUseSkill(Int64 xSumTickNum, BattleCommonSkill* xBattleSkill);     // 使用法术时
    virtual Boolean OnCauseDamage(Int64 xSumTickNum, Int32& xOutNewValue);              // 造成伤害时
    virtual Boolean OnActionBegin(Int64 xSumTickNum);                                   // 行动开始时
    virtual Boolean OnActionEnd(Int64 xSumTickNum);                                     // 行动结束时
    // 受伤时
    virtual Boolean OnHurt(Int64 xSumTickNum, BUFF_ID xBuffId, mstr xSkillName, Int32& xDamage_HP, Int32& xDamage_MP);
    virtual Boolean OnGetPlusRatio(Int64 xSumTickNum, Single& xOutNewValue);            // 获取加成比例时
    virtual Boolean OnGetDefense(Int64 xSumTickNum, Int32& xOutNewValue);               // 获取防御值时
    virtual Boolean OnGetAttr(Int64 xSumTickNum, Int32& xOutNewValue);                  // 获取属性时






    static Boolean IsFriend(BattleCommonUnit* xSrcUnit, BattleCommonUnit* xTarUnit);

    virtual Boolean Damage(S2C_BattleCommandDamage& xPacket, BUFF_ID xBuffId, LPCSTR xSkillName, Int32& DamageHP, Int32& DamageMP, Int64 xSumTickNum) = 0;
    virtual Boolean Damage(BUFF_ID xBuffId, LPCSTR xSkillName, Int32& DamageHP, Int32& DamageMP, Int64 xSumTickNum) = 0;
    virtual Boolean Defend(S2C_BattleCommandDefend& xPacket, Int64 xSumTickNum) = 0;
    virtual Boolean Attack(S2C_BattleCommandAttack& xPacket, BattleCommonUnit* xTargetarUnit, Int64 xSumTickNum) = 0;
    virtual Boolean UseSkill(LPCSTR xSkillName, S2C_BattleCommandUseSkill& xPacket, BattleCommonUnit* xTargetarUnit, Int64 xSumTickNum) = 0;

    UD_RoleAttr& m_RoleAttr;
    MsDictionary <BUFF_ID, BattleCommonBuff*> m_Dict_BattleBuff;
    mstr m_Default_SkillName;
    MsDictionary <mstr, BattleCommonSkill*> m_Dict_BattleSkill;
    Int32 m_ReserveActionForce;
    BattleCommonBuff* m_BuffControl;

    virtual void UpdateBuff(Int64 xSumTickNum) = 0;
    virtual void AddSkill(LPCSTR xSkillName, BattleCommonSkill* xBattleCommonSkill);

    virtual Boolean AddBuff(Int64 xSumTickNum, BattleCommonBuff* xBuff);
    virtual Boolean RemoveBuff(Int64 xSumTickNum, BUFF_ID xBuffId);
    virtual void RemoveAllBuff(Int64 xSumTickNum, BUFF_TYPE xBuffType);
    virtual void RemoveAllBuff(Int64 xSumTickNum);
    virtual void CheckNeedInstantlyRemoveBuff(Int64 xNewTickNum, Int64 xSumTickNum);

    virtual Boolean IsDead();
    virtual void AddRewardExp(Int64 xRewardExp);
    virtual void AddRewardMoney(Int64 xRewardMoney);
    virtual void AddRewardItem(Int64 xRewardItemIndex, item& xItem);

    Boolean CalculationActionForce(Int64 xSumTickNum);
    Boolean CalculationActionForce(UD_RoleAttr& xRoleAttr, Int64 xSumTickNum);
};






// 战斗技能
class PJC_Skill;
class BattleCommonSkill
{
public:
    BattleCommonSkill(BattleCommonUnit* xSrcObject, PJC_Skill* xSkill, LPCSTR xSkillName);
    virtual ~BattleCommonSkill();

    virtual Boolean CheckConsume(Int64 xNow) = 0;
    virtual Boolean UseSkill(Int64 xNow, Int64 xSumTickNum, S2C_BattleCommandUseSkill& xPacket, BattleCommonUnit* xSelectTarget) = 0;
    virtual SKILL_TARGET_TYPE GetTargetType() = 0;
    virtual MsVector<BattleCommonUnit*> FlagSelectTargetList(Int64 xNow, BattleCommonUnit* xSelectTarget, MsUnorderedDictionary<Int64, UI_Key>& xDict_TargetFlag);
    virtual MsVector<BattleCommonUnit*> RealSelectTargetList(Int64 xSumTickNum, BattleCommonUnit* xSelectTarget);
    virtual void FilterObject(Int64 xSumTickNum, MsVector<BattleCommonUnit*>& xTargetList);
public:
    BattleCommonUnit* m_SrcObject;
    PJC_Skill* m_Skill;
    MsList<BattleCommonUnit*> m_TargetList;
    mstr m_SkillName;
};

