#pragma once

enum E_TargetType { 友方, 敌方, 任意, };
enum E_SelectOrder { 敏高优先, 敏低优先, 攻高优先, 攻低优先, 血高优先, 血低优先, };

// 效果强度
struct S_EffectPower
{
    bool operator==(const S_EffectPower& xTar) const { return xTar.m_Skilld == m_Skilld && xTar.m_EffectRatio == m_EffectRatio; }
    DWORD m_Skilld;
    Single m_EffectRatio;
};

// 消耗
struct S_Consume
{
    bool operator==(const S_Consume& xTar) const { return xTar.m_Skilld == m_Skilld && xTar.m_ConsumeValue == m_ConsumeValue; }
    DWORD m_Skilld = INVALID_UID;
    DWORD m_ConsumeValue = INVALID_UID;
};

struct S_TargetCount
{
    bool operator==(const S_TargetCount& xTar) const { return xTar.m_Skilld == m_Skilld && xTar.m_TarCount == m_TarCount; }
    DWORD m_Skilld;
    DWORD m_TarCount;
};

class PJC_Skill : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(PJC_Skill);

    Int32 GetConsume();
    Single GetPower();
    Int32 GetTarCount();

    E_TargetType m_TargetType;
    E_SelectOrder m_SelectOrder;
    MsList<S_EffectPower> m_ListEffectPower;
    MsList<S_Consume> m_ListConsume;
    MsList<S_TargetCount> m_ListTargetCount;

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(图标索引);
    DEF_I32_FIELD(资源索引);
    DEF_I32_FIELD(等级);
    DEF_I32_FIELD(熟练度);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(技能名称);
    DEF_STR_FIELD(分组);
    DEF_STR_FIELD(图标文件);
    DEF_STR_FIELD(资源文件);
    DEF_STR_FIELD(过滤顺序);
    DEF_STR_FIELD(附带效果);
    DEF_STR_FIELD(目标类型);
    DEF_STR_FIELD(消耗法力列表);
    DEF_STR_FIELD(效果强度列表);
    DEF_STR_FIELD(目标数量列表);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load();

    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader)
    {
        FAST_READ_ATTR_TYPE_NAME(Skill);
        //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
        //this->SetName(Attribs.m_MsXmlReader->GetTagName());
        //this->SetType("技能");

        FAST_READ_HEX_VALUE(资源索引);
        FAST_READ_HEX_VALUE(图标索引);

        this->Set等级(atoi(Attribs["等级"].c_str()));
        this->Set熟练度(atoi(Attribs["熟练度"].c_str()));

        this->Set技能名称(Attribs["技能名称"]);
        this->Set分组(Attribs["分组"]);
        this->Set图标文件(Attribs["图标文件"]);
        this->Set资源文件(Attribs["资源文件"]);
        this->Set过滤顺序(Attribs["过滤顺序"]);
        this->Set附带效果(Attribs["附带效果"]);
        this->Set目标类型(Attribs["目标类型"]);
        this->Set消耗法力列表(Attribs["消耗法力列表"]);
        this->Set效果强度列表(Attribs["效果强度列表"]);
        this->Set目标数量列表(Attribs["目标数量列表"]);
    }
};
