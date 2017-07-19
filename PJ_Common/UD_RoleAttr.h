#pragma once
class UD_RoleInfo;

class UD_RoleAttr : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleAttr);

    void CreateInit();

    void InitLevel(MsList<UD_LevelInfo*> xListLevelConfig);

    void LevelUp(UD_LevelInfo* xLevelConfig);

    // 计算属性
    void Calculate(UD_RoleInfo* xRoleInfo, Boolean IsFullRefresh);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(TeamPos);
    DEF_I32_FIELD(MinAp);               // 攻击下限
    DEF_I32_FIELD(MaxAp);               // 攻击上限
    DEF_I32_FIELD(PhysicalDefense);     // 物理防御
    DEF_I32_FIELD(PointHp);             // 根骨已分配点
    DEF_I32_FIELD(ReadyPointHp);        // 根骨未确认点
    DEF_I32_FIELD(PointMp);             // 灵性已分配点
    DEF_I32_FIELD(ReadyPointMp);        // 灵性未确认点
    DEF_I32_FIELD(PointAp);             // 力量已分配点
    DEF_I32_FIELD(ReadyPointAp);        // 力量未确认点
    DEF_I32_FIELD(PointSp);             // 敏捷已分配点
    DEF_I32_FIELD(ReadyPointSp);        // 敏捷未确认点
    DEF_I32_FIELD(SurplusPoint);        // 剩余可分配点

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);           // 角色索引
    DEF_I64_FIELD(TeamIndex);           // 队伍索引
    DEF_I64_FIELD(Level);               // 角色等级
    DEF_I64_FIELD(BaseHp);              // 基础生命
    DEF_I64_FIELD(BaseMp);              // 基础法力
    DEF_I64_FIELD(BaseMinAp);           // 基础最低攻击
    DEF_I64_FIELD(BaseMaxAp);           // 基础最高攻击
    DEF_I64_FIELD(MaxHp);               // 最大血量
    DEF_I64_FIELD(MaxMp);               // 最大法力
    DEF_I64_FIELD(MaxExp);              // 最大经验值
    DEF_CHECK_I64_FIELD(CurMp,0, GetMaxMp());       // 当前法力
    DEF_CHECK_I64_FIELD(CurHp, 0, GetMaxHp());      // 当前血量
    DEF_CHECK_I64_FIELD(CurExp, 0, MAXINT64);       // 当前经验值

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(GrowthHp);              // 根骨成长率
    DEF_F_FIELD(GrowthMp);              // 灵性成长率
    DEF_F_FIELD(GrowthAp);              // 力量成长率
    DEF_F_FIELD(GrowthSp);              // 敏捷成长率
    DEF_F_FIELD(BaseActionForce);       // 基础行动力
    DEF_F_FIELD(CurrentActionForce);    // 当前行动力
    DEF_F_FIELD(FullActionForce);       // 行动力上限
    DEF_F_FIELD(ActionPlusRate);        // 行动力速率加成
    DEF_F_FIELD(BaseSp);                // 基础速度
    DEF_F_FIELD(CurSp);                 // 敏捷速度

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_RoleRewards, RoleRewards);   // 奖励容器

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load()
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Data);

        FAST_READ_TYPE(RoleAttr);
        return True;
    }

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(RoleAttr);
    }
    //const static Int64 GROWTH_PRECISION = 1000;
    //const static Int64 BASE_HP = 300;
    //const static Int64 BASE_MP = 300;
    //const static Int64 BASE_AP = 70;
    //const static Int64 BASE_SP = 10;
};
