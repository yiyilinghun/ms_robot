#pragma once
//#include "UD_Base.h"
#include "UD_Common.h"

#include "UD_Born.h"
#include "UDC_Item_Drugs.h"
#include "UDC_Item_Equip.h"

#include "UDD_Account.h"

#include "UD_LevelInfo.h"
#include "UD_Npc.h"
#include "UD_Monster.h"
#include "UD_ObjectRes.h"
#include "PJC_Skill.h"

#include "UD_FindMapInfo.h"

#include "UD_Npc.h"
#include "UD_Monster.h"
#include "UD_TransferPoint.h"
#include "UD_Scene.h"

#include "UD_RoleInfo.h"
#include "UD_TeamInfo.h"

struct WEIGHT_RAND_ASSIST
{
    Int32  m_Weight;
    Boolean m_Invalid;
    Int32  m_Value;
};

struct ITEM_UNIT
{
    E_GOODS_TYPE m_Type;
    Int32  m_UnitId;
    Int32  m_UnitCount;
};
#pragma endregion

class PJCommonConfig : public MsXmlReader
{
public:
    PJCommonConfig(LPCSTR xXmlFileName);
    PJCommonConfig(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword);
    ~PJCommonConfig();

    void Init();

    // 对象资源配置
    void LoadObjectRes();
    MsUnorderedDictionary<mstr, UD_ObjectRes*> m_DictObjectRes;

    // 药品配置
    void LoadDrugs();
    MsUnorderedDictionary<mstr, UD_Item_Drugs*> m_DictItemDrugs;

    // 装备配置
    void LoadEquip();
    MsUnorderedDictionary<mstr, UD_Item_Equip*> m_DictItemEquip;

    // 技能配置
    void LoadSkill();
    MsUnorderedDictionary<mstr, PJC_Skill*> m_DictSkill;
    PJC_Skill* GetSkill(mstr xSkillName);


    // 场景配置
    void LoadScene();
    MsUnorderedDictionary<WORD, UD_Scene*> m_DictScene;
    MsUnorderedDictionary<WORD, UD_Scene*> m_DictBattleScene;
    UD_Scene* GetScene(WORD xSceneId);

    //// 出生配置
    //void LoadBorn();
    //UDC_CommonBorn* m_CommonBorn;
    //MsUnorderedDictionary<mstr, UD_Born*> m_DictBorn;

    //// 召唤兽配置
    //void LoadSummonpet();
    //MsUnorderedDictionary<mstr, UD_Summonpet*> m_DictSummonpet;

    //// 打开奖励列表
    //Boolean OpenReward_PackageList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// 打开指定礼包
    //Boolean OpenPackage(Int32 xPackageId, MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// 将奖励列表转换成单元(道具,装备,经验,金币,钻石)列表
    //Boolean RewardList2UnitList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    //// 根据背包道具填充Tips
    //Boolean FillItemTipsByItem(item* xItem);
};


class PJServerConfig : public MsXmlReader
{
public:
    PJServerConfig(LPCSTR xXmlFileName);
    PJServerConfig(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword);
    ~PJServerConfig();

    void Init();

    // 等级信息
    void LoadLevelInfo(mstr xType);
    //void LoadNormalLevelInfo();         // 普通
    //void LoadGoodLevelInfo();           // 优秀
    //void LoadSuperiorLevelInfo();       // 精良
    //void LoadEpicLevelInfo();           // 史诗
    //void LoadLegendaryLevelInfo();      // 传说
    MsUnorderedDictionary<mstr, MsUnorderedDictionary<Int64, UD_LevelInfo*>> m_Dict_Type_LevelInfo;

    // 出生配置
    void LoadBorn();
    UDC_CommonBorn* m_CommonBorn;
    MsUnorderedDictionary<mstr, UD_Born*> m_DictBorn;

    // 召唤兽配置
    void LoadSummonpet();
    MsUnorderedDictionary<mstr, UD_Summonpet*> m_DictSummonpet;

    //// 打开奖励列表
    //Boolean OpenReward_PackageList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// 打开指定礼包
    //Boolean OpenPackage(Int32 xPackageId, MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// 将奖励列表转换成单元(道具,装备,经验,金币,钻石)列表
    //Boolean RewardList2UnitList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    //// 根据背包道具填充Tips
    //Boolean FillItemTipsByItem(item* xItem);
};
