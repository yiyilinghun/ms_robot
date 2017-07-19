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
    DEF_I32_FIELD(战斗场景);
    DEF_I32_FIELD(音乐索引);
    DEF_I32_FIELD(小地图资源);

    DEF_STR_BEGIN_FIELD;
    DEF_STR_FIELD(名称);
    DEF_STR_FIELD(类型);
    DEF_STR_FIELD(文件);
    DEF_STR_FIELD(小地图文件);
    DEF_STR_FIELD(音乐文件);

    DEF_PROTO_BEGIN_FIELD;
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_TransferPoint, TransferPoint);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Npc, Npc);
    DEF_DICT_PROTO_FIELD_BY_NAME(UD_Monster, Monster);
};

