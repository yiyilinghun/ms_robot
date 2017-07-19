#pragma once

class UD_Scene : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_Scene);

    virtual void SetName(mstr xName)
    {
        return UD_Base::SetName(xName);
    }

    virtual Boolean Load();
    virtual void LoadFromObviousXml(MsXmlReader& xMsXmlReader);

    DEF_I64_BEGIN_FIELD;
    DEF_I64_FIELD(ID);

    DEF_I32_ENUM_BEGIN_FIELD;
    DEF_I32_FIELD(ս������);
    DEF_I32_FIELD(��������);
    DEF_I32_FIELD(С��ͼ��Դ);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(����);
    DEF_STR_FIELD(�ļ�);
    DEF_STR_FIELD(С��ͼ�ļ�);
    DEF_STR_FIELD(�����ļ�);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_TransferPoint, TransferPoint);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc, Npc);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Monster, Monster);
};

