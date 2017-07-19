#pragma once
class UD_RoleInfo;

class UD_RoleAttr : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_RoleAttr);

    void CreateInit();

    void InitLevel(MsList<UD_LevelInfo*> xListLevelConfig);

    void LevelUp(UD_LevelInfo* xLevelConfig);

    // ��������
    void Calculate(UD_RoleInfo* xRoleInfo, Boolean IsFullRefresh);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(TeamPos);
    DEF_I32_FIELD(MinAp);               // ��������
    DEF_I32_FIELD(MaxAp);               // ��������
    DEF_I32_FIELD(PhysicalDefense);     // �������
    DEF_I32_FIELD(PointHp);             // �����ѷ����
    DEF_I32_FIELD(ReadyPointHp);        // ����δȷ�ϵ�
    DEF_I32_FIELD(PointMp);             // �����ѷ����
    DEF_I32_FIELD(ReadyPointMp);        // ����δȷ�ϵ�
    DEF_I32_FIELD(PointAp);             // �����ѷ����
    DEF_I32_FIELD(ReadyPointAp);        // ����δȷ�ϵ�
    DEF_I32_FIELD(PointSp);             // �����ѷ����
    DEF_I32_FIELD(ReadyPointSp);        // ����δȷ�ϵ�
    DEF_I32_FIELD(SurplusPoint);        // ʣ��ɷ����

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(RoleIndex);           // ��ɫ����
    DEF_I64_FIELD(TeamIndex);           // ��������
    DEF_I64_FIELD(Level);               // ��ɫ�ȼ�
    DEF_I64_FIELD(BaseHp);              // ��������
    DEF_I64_FIELD(BaseMp);              // ��������
    DEF_I64_FIELD(BaseMinAp);           // ������͹���
    DEF_I64_FIELD(BaseMaxAp);           // ������߹���
    DEF_I64_FIELD(MaxHp);               // ���Ѫ��
    DEF_I64_FIELD(MaxMp);               // �����
    DEF_I64_FIELD(MaxExp);              // �����ֵ
    DEF_CHECK_I64_FIELD(CurMp,0, GetMaxMp());       // ��ǰ����
    DEF_CHECK_I64_FIELD(CurHp, 0, GetMaxHp());      // ��ǰѪ��
    DEF_CHECK_I64_FIELD(CurExp, 0, MAXINT64);       // ��ǰ����ֵ

    DEF_F_BEGIN_FIELD;
    DEF_F_FIELD(GrowthHp);              // ���ǳɳ���
    DEF_F_FIELD(GrowthMp);              // ���Գɳ���
    DEF_F_FIELD(GrowthAp);              // �����ɳ���
    DEF_F_FIELD(GrowthSp);              // ���ݳɳ���
    DEF_F_FIELD(BaseActionForce);       // �����ж���
    DEF_F_FIELD(CurrentActionForce);    // ��ǰ�ж���
    DEF_F_FIELD(FullActionForce);       // �ж�������
    DEF_F_FIELD(ActionPlusRate);        // �ж������ʼӳ�
    DEF_F_FIELD(BaseSp);                // �����ٶ�
    DEF_F_FIELD(CurSp);                 // �����ٶ�

    DEF_PROTO_BEGIN_FIELD;
    DEF_PROTO_FIELD(UD_RoleRewards, RoleRewards);   // ��������

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
