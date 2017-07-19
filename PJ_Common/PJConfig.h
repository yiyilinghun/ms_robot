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

    // ������Դ����
    void LoadObjectRes();
    MsUnorderedDictionary<mstr, UD_ObjectRes*> m_DictObjectRes;

    // ҩƷ����
    void LoadDrugs();
    MsUnorderedDictionary<mstr, UD_Item_Drugs*> m_DictItemDrugs;

    // װ������
    void LoadEquip();
    MsUnorderedDictionary<mstr, UD_Item_Equip*> m_DictItemEquip;

    // ��������
    void LoadSkill();
    MsUnorderedDictionary<mstr, PJC_Skill*> m_DictSkill;
    PJC_Skill* GetSkill(mstr xSkillName);


    // ��������
    void LoadScene();
    MsUnorderedDictionary<WORD, UD_Scene*> m_DictScene;
    MsUnorderedDictionary<WORD, UD_Scene*> m_DictBattleScene;
    UD_Scene* GetScene(WORD xSceneId);

    //// ��������
    //void LoadBorn();
    //UDC_CommonBorn* m_CommonBorn;
    //MsUnorderedDictionary<mstr, UD_Born*> m_DictBorn;

    //// �ٻ�������
    //void LoadSummonpet();
    //MsUnorderedDictionary<mstr, UD_Summonpet*> m_DictSummonpet;

    //// �򿪽����б�
    //Boolean OpenReward_PackageList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// ��ָ�����
    //Boolean OpenPackage(Int32 xPackageId, MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// �������б�ת���ɵ�Ԫ(����,װ��,����,���,��ʯ)�б�
    //Boolean RewardList2UnitList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    //// ���ݱ����������Tips
    //Boolean FillItemTipsByItem(item* xItem);
};


class PJServerConfig : public MsXmlReader
{
public:
    PJServerConfig(LPCSTR xXmlFileName);
    PJServerConfig(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword);
    ~PJServerConfig();

    void Init();

    // �ȼ���Ϣ
    void LoadLevelInfo(mstr xType);
    //void LoadNormalLevelInfo();         // ��ͨ
    //void LoadGoodLevelInfo();           // ����
    //void LoadSuperiorLevelInfo();       // ����
    //void LoadEpicLevelInfo();           // ʷʫ
    //void LoadLegendaryLevelInfo();      // ��˵
    MsUnorderedDictionary<mstr, MsUnorderedDictionary<Int64, UD_LevelInfo*>> m_Dict_Type_LevelInfo;

    // ��������
    void LoadBorn();
    UDC_CommonBorn* m_CommonBorn;
    MsUnorderedDictionary<mstr, UD_Born*> m_DictBorn;

    // �ٻ�������
    void LoadSummonpet();
    MsUnorderedDictionary<mstr, UD_Summonpet*> m_DictSummonpet;

    //// �򿪽����б�
    //Boolean OpenReward_PackageList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// ��ָ�����
    //Boolean OpenPackage(Int32 xPackageId, MsUnorderedDictionary<Int32, Int32>& xDictReward_Package);

    //// �������б�ת���ɵ�Ԫ(����,װ��,����,���,��ʯ)�б�
    //Boolean RewardList2UnitList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package, MsList<ITEM_UNIT>& xListUnit);

    //// ���ݱ����������Tips
    //Boolean FillItemTipsByItem(item* xItem);
};
