#pragma once

enum E_TargetType { �ѷ�, �з�, ����, };
enum E_SelectOrder { ��������, ��������, ��������, ��������, Ѫ������, Ѫ������, };

// Ч��ǿ��
struct S_EffectPower
{
    bool operator==(const S_EffectPower& xTar) const { return xTar.m_Skilld == m_Skilld && xTar.m_EffectRatio == m_EffectRatio; }
    DWORD m_Skilld;
    Single m_EffectRatio;
};

// ����
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
    DEF_I32_FIELD(ͼ������);
    DEF_I32_FIELD(��Դ����);
    DEF_I32_FIELD(�ȼ�);
    DEF_I32_FIELD(������);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(��������);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(ͼ���ļ�);
    DEF_STR_FIELD(��Դ�ļ�);
    DEF_STR_FIELD(����˳��);
    DEF_STR_FIELD(����Ч��);
    DEF_STR_FIELD(Ŀ������);
    DEF_STR_FIELD(���ķ����б�);
    DEF_STR_FIELD(Ч��ǿ���б�);
    DEF_STR_FIELD(Ŀ�������б�);

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
        //this->SetType("����");

        FAST_READ_HEX_VALUE(��Դ����);
        FAST_READ_HEX_VALUE(ͼ������);

        this->Set�ȼ�(atoi(Attribs["�ȼ�"].c_str()));
        this->Set������(atoi(Attribs["������"].c_str()));

        this->Set��������(Attribs["��������"]);
        this->Set����(Attribs["����"]);
        this->Setͼ���ļ�(Attribs["ͼ���ļ�"]);
        this->Set��Դ�ļ�(Attribs["��Դ�ļ�"]);
        this->Set����˳��(Attribs["����˳��"]);
        this->Set����Ч��(Attribs["����Ч��"]);
        this->SetĿ������(Attribs["Ŀ������"]);
        this->Set���ķ����б�(Attribs["���ķ����б�"]);
        this->SetЧ��ǿ���б�(Attribs["Ч��ǿ���б�"]);
        this->SetĿ�������б�(Attribs["Ŀ�������б�"]);
    }
};
