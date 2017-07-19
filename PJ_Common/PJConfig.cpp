#include "Precompiled.h"

mstr Getװ����λ�ı�(EQUIP_POS_TYPE xEquipPosType)
{
    switch (xEquipPosType)
    {
        case EPT_PI_FENG: { return "����"; }
        case EPT_MAO_ZI: { return "ñ��"; }
        case EPT_WU_QI: { return "����"; }
        case EPT_YI_FU: { return "�·�"; }
        case EPT_GUA_JIAN: { return "�Ҽ�"; }
        case EPT_XIE_ZI: { return "Ь��"; }
        case EPT_MIAN_JU: { return "���"; }
        case EPT_XIANG_LIAN: { return "����"; }
        case EPT_JIE_ZHI: { return "��ָ"; }
        case EPT_YAO_DAI: { return "����"; }
        default: {return ""; }
    }
}

mstr Getװ�������ı�(EQUIP_TYPE xEquipType)
{
    switch (xEquipType)
    {
        case ET_PI_FENG_NAN: {return "����_��"; }
        case ET_PI_FENG_NV: {return "����_Ů"; }
        case ET_MAO_ZI_NAN: {return "ñ��_��"; }
        case ET_MAO_ZI_NV: {return "ñ��_Ů"; }
        case ET_YI_FU_NAN: {return "�·�_��"; }
        case ET_YI_FU_NV: {return "�·�_Ů"; }
        case ET_GUA_JIAN_NAN: {return "�Ҽ�_��"; }
        case ET_GUA_JIAN_NV: {return "�Ҽ�_Ů"; }
        case ET_MIAN_JU_NAN: {return "���_��"; }
        case ET_MIAN_JU_NV: {return "���_Ů"; }
        case ET_WU_QI_JIAN: {return "����_��"; }
        case ET_WU_QI_DAO: {return "����_��"; }
        case ET_WU_QI_BIAN: {return "����_��"; }
        case ET_WU_QI_CHUI: {return "����_��"; }
        case ET_WU_QI_GUN: {return "����_��"; }
        case ET_WU_QI_ZHUA: {return "����_צ"; }
        case ET_WU_QI_QIANG: {return "����_ǹ"; }
        case ET_WU_QI_FU: {return "����_��"; }
        case ET_WU_QI_SHAN: {return "����_��"; }
        case ET_WU_QI_GOU: {return "����_��"; }
        case ET_WU_QI_QUAN: {return "����_ȭ"; }
        case ET_WU_QI_CHEN: {return "����_��"; }
        case ET_WU_QI_FAN: {return "����_�"; }
        case ET_WU_QI_DAI: {return "����_��"; }
        case ET_XIANG_LIAN: {return "����"; }
        case ET_XIE_ZI: {return "Ь��"; }
        case ET_JIE_ZHI: {return "��ָ"; }
        case ET_YAO_DAI: {return "����"; }
        default: {return ""; }
    }
}

// ����ָ��������Ʒ���ֵ�
Boolean AddItem(MsUnorderedDictionary<Int32, Int32>& xDictReward, Int32 xItemId, Int32 xItemCount)
{
    if (xDictReward.ContainsKey(xItemId))
    {
        xDictReward[xItemId] += xItemCount;
    }
    else
    {
        xDictReward[xItemId] = xItemCount;
    }
    return True;
}

// �Ƴ�ָ��������Ʒ���ֵ�
Boolean EraseItem(MsUnorderedDictionary<Int32, Int32>& xDictReward, Int32 xItemId, Int32 xItemCount)
{
    if (xDictReward.ContainsKey(xItemId) && xDictReward[xItemId] >= xItemCount)
    {
        xDictReward[xItemId] -= xItemCount;

        if (xDictReward[xItemId] == 0)
        {
            xDictReward.Remove(xItemId);
        }
        return True;
    }
    return False;
}



PJCommonConfig::PJCommonConfig(LPCSTR xXmlFileName)
{
    this->Load(xXmlFileName);
    this->Init();
}

PJCommonConfig::PJCommonConfig(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword)
{
    this->LoadFromZip(xZipName, szXmlPathName, szPassword);
    this->Init();
}

PJCommonConfig::~PJCommonConfig()
{
    SAFE_ENUMERATOR_DELETE(m_DictObjectRes);
    SAFE_ENUMERATOR_DELETE(m_DictItemDrugs);
    SAFE_ENUMERATOR_DELETE(m_DictItemEquip);
    SAFE_ENUMERATOR_DELETE(m_DictSkill);
    SAFE_ENUMERATOR_DELETE(m_DictScene);
    SAFE_ENUMERATOR_DELETE(m_DictBattleScene);

    //SAFE_ENUMERATOR_DELETE(m_DictRoleLevel);
    //SAFE_ENUMERATOR_DELETE(m_DictBorn);
    //SAFE_DELETE(m_CommonBorn);
    //SAFE_ENUMERATOR_DELETE(m_DictSummonpet);
}

void PJCommonConfig::Init()
{
    this->LoadObjectRes();
    this->LoadDrugs();
    this->LoadEquip();
    this->LoadSkill();
    this->LoadScene();
}


void PJCommonConfig::LoadObjectRes()
{
    MsXmlReader& xXmlReader = *this;

    m_DictObjectRes.Clear();
    xXmlReader[nullptr]["������Դ"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_ObjectRes* xPJC_ObjectRes = NEW UD_ObjectRes(xXmlReader);
                xPJC_ObjectRes->Load();
                if (m_DictObjectRes.ContainsKey(xPJC_ObjectRes->GetName()))
                {
                    AssertLog("��Դ���ظ�[%s]", xPJC_ObjectRes->GetName().c_str());
                }
                else
                {
                    m_DictObjectRes.Add(xPJC_ObjectRes->GetName(), xPJC_ObjectRes);
                }
            } while (xXmlReader.FindNextElem());
        }
    }
}

void PJCommonConfig::LoadDrugs()
{
    MsXmlReader& xXmlReader = *this;

    m_DictItemDrugs.Clear();
    xXmlReader[nullptr]["ҩƷ�б�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_Item_Drugs* xItemDrugs = NEW UD_Item_Drugs(xXmlReader);
                xItemDrugs->Load();
                AssertNormal(!m_DictItemDrugs.ContainsKey(xItemDrugs->GetName()), "ҩƷ��key");
                m_DictItemDrugs.Add(xItemDrugs->GetName(), xItemDrugs);
            } while (xXmlReader.FindNextElem());
        }
    }
}

void PJCommonConfig::LoadEquip()
{
    MsXmlReader& xXmlReader = *this;

    m_DictItemEquip.Clear();
    xXmlReader[nullptr]["װ���б�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_Item_Equip* xItemEquip = NEW UD_Item_Equip(xXmlReader);
                xItemEquip->Load();
                AssertNormal(!m_DictItemEquip.ContainsKey(xItemEquip->GetName()), "װ����key");
                m_DictItemEquip.Add(xItemEquip->GetName(), xItemEquip);
            } while (xXmlReader.FindNextElem());
        }
    }
}


void PJCommonConfig::LoadScene()
{
    MsXmlReader& xXmlReader = *this;

    xXmlReader[nullptr]["�����б�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_Scene* xPJC_Scene = NEW UD_Scene(xXmlReader);
                xPJC_Scene->Load();
                AssertNormal(!m_DictScene.ContainsKey((WORD)xPJC_Scene->GetID()), "������key");
                m_DictScene.Add((WORD)xPJC_Scene->GetID(), xPJC_Scene);
            } while (xXmlReader.FindNextElem());
        }
    }

    xXmlReader[nullptr]["�����б�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem("ս��")) {
            do
            {
                UD_Scene* xPJC_Scene = NEW UD_Scene(xXmlReader);
                xPJC_Scene->Load();
                AssertNormal(!m_DictBattleScene.ContainsKey((WORD)xPJC_Scene->GetID()), "������key");
                m_DictBattleScene.Add((WORD)xPJC_Scene->GetID(), xPJC_Scene);
            } while (xXmlReader.FindNextElem("ս��"));
        }
    }
}

UD_Scene* PJCommonConfig::GetScene(WORD xSceneId)
{
    if (m_DictScene.ContainsKey(xSceneId))
    {
        return m_DictScene[xSceneId];
    }
    return nullptr;
}


//void PJServerConfig::LoadScene_NPC(UD_Scene* xPJC_Scene, MsUnorderedDictionary<mstr, PJC_Scene_NPC*>& xDictScene_NPC)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_Scene_NPC* xPJC_NPC = NEW PJC_Scene_NPC(xXmlReader);
//                //xPJC_NPC->LoadFromObviousXml();
//                AssertNormal(!xDictScene_NPC.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC.Add(xPJC_NPC->GetName(), xPJC_NPC);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//// ���س�����NPC���̵�����
//void PJServerConfig::LoadScene_NPC_Shop(UD_Scene* xPJC_Scene, PJC_NPC* xPJC_NPC, MsUnorderedDictionary<mstr, PJC_NPC_Shop*>& xDictScene_NPC_Shop)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"][xPJC_NPC->name().c_str()]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_NPC_Shop* xPJC_NPC_Shop = NEW PJC_NPC_Shop(xXmlReader.Attribs);
//                AssertNormal(!xDictScene_NPC_Shop.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC_Shop.Add(xPJC_NPC_Shop->GetName(), xPJC_NPC_Shop);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//// ���س�����NPC���̵�����Ʒ����
//void PJServerConfig::LoadScene_NPC_Shop_Item(UD_Scene* xPJC_Scene, PJC_NPC* xPJC_NPC, PJC_NPC_Shop* xPJC_NPC_Shop, MsUnorderedDictionary<mstr, PJC_NPC_Shop_Item*>& xDictScene_NPC_Shop_Item)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"][xPJC_NPC->name().c_str()][xPJC_NPC_Shop->name().c_str()]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_NPC_Shop_Item* xPJC_NPC_Shop_Item = NEW PJC_NPC_Shop_Item(xXmlReader.Attribs);
//                AssertNormal(!xDictScene_NPC_Shop_Item.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC_Shop_Item.Add(xPJC_NPC_Shop_Item->GetName(), xPJC_NPC_Shop_Item);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}

//// ��������
//void PJServerConfig::LoadScene_Monster(UD_Scene* xPJC_Scene, MsList<UD_Monster*>& xListSceneMonster)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["����"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                UD_Monster* xPJC_Monster = NEW UD_Monster(xXmlReader);
//                //xPJC_Monster->LoadFromObviousXml();
//                xListSceneMonster.Add(xPJC_Monster);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//void PJServerConfig::LoadScene_TransferPoint(UD_Scene* xPJC_Scene, MsUnorderedDictionary<mstr, UD_TransferPoint*>& xDictSceneTransferPoint)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["���͵�"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                UD_TransferPoint* xPJC_TransferPoint = NEW UD_TransferPoint(xXmlReader);
//                //xPJC_TransferPoint->LoadFromObviousXml();
//                AssertNormal(!xDictSceneTransferPoint.ContainsKey(xPJC_TransferPoint->GetName()), "");
//                xDictSceneTransferPoint.Add(xPJC_TransferPoint->GetName(), xPJC_TransferPoint);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}


void PJCommonConfig::LoadSkill()
{
    MsXmlReader& xXmlReader = *this;

    xXmlReader[nullptr]["�����б�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                PJC_Skill* xPJC_Skill = NEW PJC_Skill(xXmlReader);
                xPJC_Skill->Load();
                AssertNormal(!m_DictSkill.ContainsKey(xPJC_Skill->Get��������()), "������key");
                m_DictSkill.Add(xPJC_Skill->Get��������(), xPJC_Skill);
            } while (xXmlReader.FindNextElem());
        }
    }
}

PJC_Skill* PJCommonConfig::GetSkill(mstr xSkillName)
{
    if (m_DictSkill.ContainsKey(xSkillName))
    {
        return m_DictSkill[xSkillName];
    }
    return nullptr;
}

Boolean PJC_Skill::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(Skill);

    if (this->Get����˳��() == "��������") { m_SelectOrder = E_SelectOrder::��������; }
    else if (this->Get����˳��() == "��������") { m_SelectOrder = E_SelectOrder::��������; }
    else if (this->Get����˳��() == "��������") { m_SelectOrder = E_SelectOrder::��������; }
    else if (this->Get����˳��() == "��������") { m_SelectOrder = E_SelectOrder::��������; }
    else if (this->Get����˳��() == "Ѫ������") { m_SelectOrder = E_SelectOrder::Ѫ������; }
    else if (this->Get����˳��() == "Ѫ������") { m_SelectOrder = E_SelectOrder::Ѫ������; }
    else { AssertLog("δ֪����˳��%s", this->Get����˳��().c_str()); }

    if (this->GetĿ������() == "�ѷ�") { m_TargetType = E_TargetType::�ѷ�; }
    else if (this->GetĿ������() == "�з�") { m_TargetType = E_TargetType::�з�; }
    else if (this->GetĿ������() == "����") { m_TargetType = E_TargetType::����; }
    else { AssertLog("δ֪Ŀ������%s", this->Get����˳��().c_str()); }

    MsVector<mstr> xTemp;
    MsBase::StringSplit(this->Get���ķ����б�().c_str(), ',', xTemp);
    FAST_FOREACH(xTemp)
    {
        DWORD xValueA;
        DWORD xValueB;
        sscanf_s(xTemp.Value().c_str(), "%u:%u", &xValueA, &xValueB);
        m_ListConsume.Add({ xValueA ,xValueB });
    }

    xTemp.Clear();
    MsBase::StringSplit(this->GetЧ��ǿ���б�().c_str(), ',', xTemp);
    FAST_FOREACH(xTemp)
    {
        DWORD xValueA;
        Single xValueB;
        sscanf_s(xTemp.Value().c_str(), "%u:%f", &xValueA, &xValueB);
        m_ListEffectPower.Add({ xValueA ,xValueB });
    }

    xTemp.Clear();
    MsBase::StringSplit(this->GetĿ�������б�().c_str(), ',', xTemp);
    FAST_FOREACH(xTemp)
    {
        DWORD xValueA;
        DWORD xValueB;
        sscanf_s(xTemp.Value().c_str(), "%u:%u", &xValueA, &xValueB);
        m_ListTargetCount.Add({ xValueA, xValueB });
    }

    return True;
}

Int32 PJC_Skill::GetConsume()
{
    S_Consume xTempBegin;
    S_Consume xTempEnd;
    DWORD xTempSkilled = (DWORD)this->Get������();
    FAST_FOREACH(m_ListConsume)
    {
        S_Consume& xEnum = m_ListConsume.Value();
        if (xEnum.m_Skilld <= xTempSkilled)
        {
            xTempBegin = xEnum;
        }
        else
        {
            xTempEnd = xEnum;
            break;
        }
    }

    Single xTemp���� = ((Single)xTempSkilled - (Single)xTempBegin.m_Skilld) / ((Single)xTempEnd.m_Skilld - (Single)xTempBegin.m_Skilld);
    return (Int32)((xTempEnd.m_ConsumeValue - xTempBegin.m_ConsumeValue) * xTemp����) + xTempBegin.m_ConsumeValue;
}

Single PJC_Skill::GetPower()
{
    S_EffectPower xTempBegin;
    S_EffectPower xTempEnd;
    DWORD xTempSkilled = (DWORD)this->Get������();
    FAST_FOREACH(m_ListEffectPower)
    {
        S_EffectPower& xEnum = m_ListEffectPower.Value();
        if (xEnum.m_Skilld <= xTempSkilled)
        {
            xTempBegin = xEnum;
        }
        else
        {
            xTempEnd = xEnum;
            break;
        }
    }

    Single xTemp���� = ((Single)xTempSkilled - (Single)xTempBegin.m_Skilld) / ((Single)xTempEnd.m_Skilld - (Single)xTempBegin.m_Skilld);
    return ((xTempEnd.m_EffectRatio - xTempBegin.m_EffectRatio) * xTemp����) + xTempBegin.m_EffectRatio;
}

Int32 PJC_Skill::GetTarCount()
{
    Int32 xTempTarCount = 0;
    DWORD xTempSkilled = (DWORD)this->Get������();
    FAST_FOREACH(m_ListTargetCount)
    {
        S_TargetCount& xEnum = m_ListTargetCount.Value();
        if (xEnum.m_Skilld <= xTempSkilled)
        {
            xTempTarCount = xEnum.m_TarCount;
        }
        else
        {
            break;
        }
    }
    return xTempTarCount;
}












PJServerConfig::PJServerConfig(LPCSTR xXmlFileName)
{
    this->Load(xXmlFileName);
    this->Init();
}

PJServerConfig::PJServerConfig(LPCSTR xZipName, LPCSTR szXmlPathName, LPCSTR szPassword)
{
    this->LoadFromZip(xZipName, szXmlPathName, szPassword);
    this->Init();
}

void PJServerConfig::Init()
{
    this->LoadLevelInfo("����");
    this->LoadLevelInfo("��ͨ");
    this->LoadLevelInfo("����");
    this->LoadLevelInfo("����");
    this->LoadLevelInfo("ʷʫ");
    this->LoadLevelInfo("��˵");

    this->LoadBorn();
    this->LoadSummonpet();
}

void PJServerConfig::LoadBorn()
{
    MsXmlReader& xXmlReader = *this;

    m_DictBorn.Clear();
    xXmlReader[nullptr]["������Ϣ"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_Born* xPJC_Born = NEW UD_Born(xXmlReader);
                xPJC_Born->Load();
                AssertNormal(!m_DictBorn.ContainsKey(xPJC_Born->GetName()), "������Ϣ��key");
                m_DictBorn.Add(xPJC_Born->GetName(), xPJC_Born);
            } while (xXmlReader.FindNextElem());
        }
    }

    xXmlReader[nullptr]["���������Ϣ"]; if (xXmlReader)
    {
        m_CommonBorn = NEW UDC_CommonBorn(xXmlReader);
        m_CommonBorn->Load();
    }
}

void PJServerConfig::LoadSummonpet()
{
    MsXmlReader& xXmlReader = *this;

    m_DictSummonpet.Clear();
    xXmlReader[nullptr]["�ٻ��ޱ�"]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_Summonpet* xPJC_Summons = NEW UD_Summonpet(xXmlReader);
                xPJC_Summons->Load();
                AssertNormal(!m_DictSummonpet.ContainsKey(xPJC_Summons->GetName()), "�ٻ�����key");
                m_DictSummonpet.Add(xPJC_Summons->GetName(), xPJC_Summons);
            } while (xXmlReader.FindNextElem());
        }
    }
}

//
//// �򿪽����б�
//Boolean PJServerConfig::OpenReward_PackageList(MsUnorderedDictionary<Int32, Int32>& xDictReward_Package)
//{
//    //MsUnorderedDictionary<Int32, Int32> xTempReward = xDictReward_Package;
//    //FAST_FOREACH(xTempReward)
//    //{
//    //    lua_reward* xReward = this->Getreward(xEnumKey);
//    //    if (!xReward) { return False; }
//
//    //    if (PJBase::GetUnitTypeById(xReward->itemid()) == EGT_PACKAGE)
//    //    {
//    //        if (EraseItem(xDictReward_Package, xEnumKey, 1))
//    //        {
//    //            Boolean xRet = this->OpenPackage(xReward->itemid(), xDictReward_Package);
//    //            if (!xRet)
//    //            {
//    //                return False;
//    //            }
//    //            else
//    //            {
//    //                continue;
//    //            }
//    //        }
//    //    }
//    //}
//    return True;
//}

//void PJServerConfig::LoadScene_NPC(UD_Scene* xPJC_Scene, MsDictionary<mstr, PJC_Scene_NPC*>& xDictScene_NPC)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_Scene_NPC* xPJC_NPC = NEW PJC_Scene_NPC(xXmlReader);
//                //xPJC_NPC->LoadFromObviousXml();
//                AssertNormal(!xDictScene_NPC.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC.Add(xPJC_NPC->GetName(), xPJC_NPC);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//// ���س�����NPC���̵�����
//void PJServerConfig::LoadScene_NPC_Shop(UD_Scene* xPJC_Scene, PJC_NPC* xPJC_NPC, MsDictionary<mstr, PJC_NPC_Shop*>& xDictScene_NPC_Shop)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"][xPJC_NPC->name().c_str()]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_NPC_Shop* xPJC_NPC_Shop = NEW PJC_NPC_Shop(xXmlReader.Attribs);
//                AssertNormal(!xDictScene_NPC_Shop.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC_Shop.Add(xPJC_NPC_Shop->GetName(), xPJC_NPC_Shop);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//// ���س�����NPC���̵�����Ʒ����
//void PJServerConfig::LoadScene_NPC_Shop_Item(UD_Scene* xPJC_Scene, PJC_NPC* xPJC_NPC, PJC_NPC_Shop* xPJC_NPC_Shop, MsDictionary<mstr, PJC_NPC_Shop_Item*>& xDictScene_NPC_Shop_Item)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["NPC"][xPJC_NPC->name().c_str()][xPJC_NPC_Shop->name().c_str()]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                PJC_NPC_Shop_Item* xPJC_NPC_Shop_Item = NEW PJC_NPC_Shop_Item(xXmlReader.Attribs);
//                AssertNormal(!xDictScene_NPC_Shop_Item.ContainsKey(xPJC_NPC->GetName()), "");
//                xDictScene_NPC_Shop_Item.Add(xPJC_NPC_Shop_Item->GetName(), xPJC_NPC_Shop_Item);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}

//// ��������
//void PJServerConfig::LoadScene_Monster(UD_Scene* xPJC_Scene, MsList<UD_Monster*>& xListSceneMonster)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["����"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                UD_Monster* xPJC_Monster = NEW UD_Monster(xXmlReader);
//                //xPJC_Monster->LoadFromObviousXml();
//                xListSceneMonster.Add(xPJC_Monster);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}
//
//void PJServerConfig::LoadScene_TransferPoint(UD_Scene* xPJC_Scene, MsDictionary<mstr, UD_TransferPoint*>& xDictSceneTransferPoint)
//{
//    MsXmlReader& xXmlReader = *this;
//
//    xXmlReader[nullptr]["�����б�"][xPJC_Scene->name().c_str()]["���͵�"]; if (xXmlReader) {
//        if (xXmlReader.FindChildElem()) {
//            do
//            {
//                UD_TransferPoint* xPJC_TransferPoint = NEW UD_TransferPoint(xXmlReader);
//                //xPJC_TransferPoint->LoadFromObviousXml();
//                AssertNormal(!xDictSceneTransferPoint.ContainsKey(xPJC_TransferPoint->GetName()), "");
//                xDictSceneTransferPoint.Add(xPJC_TransferPoint->GetName(), xPJC_TransferPoint);
//            } while (xXmlReader.FindNextElem());
//        }
//    }
//}

void PJServerConfig::LoadLevelInfo(mstr xType)
{
    MsXmlReader& xXmlReader = *this;
    var& xDict = m_Dict_Type_LevelInfo[xType];
    xXmlReader[nullptr]["�ȼ��б�"][xType.c_str()]; if (xXmlReader) {
        if (xXmlReader.FindChildElem()) {
            do
            {
                UD_LevelInfo* xPJC_RoleLevel = NEW UD_LevelInfo(xXmlReader);
                xPJC_RoleLevel->Load();
                AssertNormal(!xDict.ContainsKey(xPJC_RoleLevel->Get�ȼ�()), "�ȼ��б���key");
                xDict.Add(xPJC_RoleLevel->Get�ȼ�(), xPJC_RoleLevel);
            } while (xXmlReader.FindNextElem());
        }
    }
}

//
//// ��ָ�����
//Boolean PJServerConfig::OpenPackage(Int32 xPackageId, MsDictionary<Int32, Int32>& xDictReward)
//{
//    //// ��ʼ�����
//    //lua_package* xPackage = this->Getpackage(xPackageId);
//    //if (xPackage)
//    //{
//    //    MsList<WEIGHT_RAND_ASSIST> xListWeightReward;
//    //    if (this->GetPackageWeightRewardList(xPackage, xListWeightReward))
//    //    {
//    //        Int32 xRewardCount = xPackage->randomcount();
//    //        for (Int32 i = 0; i < xRewardCount; i++)
//    //        {
//    //            Int64 xMaxWeight = this->CalculateMaxWeight(xListWeightReward);
//    //            Int64 xRand = R_INT64(0, xMaxWeight) + 1;
//    //            Int32 xRewardKey = this->GetRewardKeyByRandProbability(xRand, xListWeightReward);
//    //            if (xRewardKey == INVALID_NID)
//    //            {
//    //                return False;
//    //            }
//    //            else
//    //            {
//    //                AddItem(xDictReward, xRewardKey, 1);
//    //                if (xDictReward.GetCount() >= MAX_REWARD_COUNT_IN_ONE_PACKAGE)
//    //                {
//    //                    return False;
//    //                }
//    //                else
//    //                {
//    //                    continue;
//    //                }
//    //            }
//    //        }
//    //        return OpenReward_PackageList(xDictReward);
//    //        //MsList<Int32> xTempReward = xListReward;
//    //        //FAST_FOREACH(xTempReward)
//    //        //{
//    //        //    lua_reward* xReward = this->Getreward(xEnumValue);
//    //        //    if (!xReward) { return False; }
//
//    //        //    if (PJBase::GetUnitTypeById(xReward->itemid()) == EGT_PACKAGE)
//    //        //    {
//    //        //        ERASE_VALUE(xListReward, xEnumValue);
//    //        //        Boolean xRet = this->OpenPackage(xReward->itemid(), xListReward);
//    //        //        if (!xRet)
//    //        //        {
//    //        //            return False;
//    //        //        }
//    //        //        else
//    //        //        {
//    //        //            continue;
//    //        //        }
//    //        //    }
//    //        //}
//    //        //return True;
//    //    }
//    //}
//    return False;
//}
//
//// �������б�ת���ɵ�Ԫ(����,װ��,����,���,��ʯ)�б�
//Boolean PJServerConfig::RewardList2UnitList(MsDictionary<Int32, Int32>& xDictReward, MsList<ITEM_UNIT>& xListUnit)
//{
//    //FAST_FOREACH(xDictReward)
//    //{
//    //    lua_reward* xReward = this->Getreward(xEnumKey);
//    //    if (!xReward) { return False; }
//
//    //    E_GOODS_TYPE xUnitType = PJBase::GetUnitTypeById(xReward->itemid());
//    //    switch (xUnitType)
//    //    {
//    //        // ���
//    //        case EGT_GOLD:
//    //        {
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//
//    //        // ����
//    //        case EGT_STAMINA:
//    //        {
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//
//    //        // ��ʯ
//    //        case EGT_DIAMOND_COIN:
//    //        {
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//
//    //        // ����
//    //        case EGT_EXP:
//    //        {
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//
//    //        // (������Ӧ���Ѿ����������,��Ӧ���������)���(�Ƿ�)
//    //        case EGT_PACKAGE:
//    //        {
//    //            AssertLog("������Ӧ���Ѿ����������,��Ӧ���������");
//    //            return False;
//    //            //xListUnit.Add({ xUnitType, xReward->itemid(),xReward->count() });
//    //        }break;
//
//    //        case EGT_ITEM:
//    //        {
//    //            lua_item* xItem = this->Getitem(xReward->itemid());
//    //            if (!xItem)
//    //            {
//    //                AssertLog("������[%d]���߲�����!", xReward->itemid());
//    //                return False;
//    //            }
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//
//    //        case EGT_EQUIP:
//    //        {
//    //            lua_equip* xEquip = this->Getequip(xReward->itemid());
//    //            if (!xEquip)
//    //            {
//    //                AssertLog("������[%d]װ��������!", xReward->itemid());
//    //                return False;
//    //            }
//    //            xListUnit.Add({ xUnitType, xReward->itemid(), (xReward->count() * xEnumValue) });
//    //        }break;
//    //    }
//
//    //}
//
//    return True;
//}

//// �����������õ���������
//Int32 PJServerConfig::GetRewardKeyByRandProbability(Int64 xRand, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
//{
//    Int32 xTempMaxWeigth = 0;
//    Int32 xTempRewardKey = INVALID_NID;
//    FAST_FOREACH(xListWeightReward)
//    {
//        if (xEnumValue.m_Invalid)
//        {
//            if (xTempMaxWeigth < xRand)
//            {
//                xTempRewardKey = xEnumValue.m_Value;
//            }
//            xTempMaxWeigth += xEnumValue.m_Weight;
//            if (xTempMaxWeigth >= xRand)
//            {
//                xEnumValue.m_Invalid = False;
//                break;
//            }
//        }
//        else
//        {
//            continue;
//        }
//    }
//    return xTempRewardKey;
//}
//
//// �������Ȩ��ֵ
//Int64 PJServerConfig::CalculateMaxWeight(MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
//{
//    Int64 xMaxWeight = 0;
//    FAST_FOREACH(xListWeightReward)
//    {
//        if (xEnumValue.m_Invalid)
//        {
//            xMaxWeight += xEnumValue.m_Weight;
//        }
//    }
//    return xMaxWeight;
//}
//
//// ��ȡָ�����Ȩ�ؽ�������
//Boolean PJServerConfig::GetPackageWeightRewardList(lua_package* xPackage, MsList<WEIGHT_RAND_ASSIST>& xListWeightReward)
//{
//    // �������Ƿ����
//    if (xPackage)
//    {
//        // ���������������Ƿ�Ϸ�
//        Int32 xRewardCount = xPackage->rewardlist_size();
//        if (xRewardCount > 0 && xRewardCount == xPackage->weightlist_size())
//        {
//            xListWeightReward.Clear();
//            for (Int32 i = 0; i < xRewardCount; i++)
//            {
//                xListWeightReward.Add({ xPackage->weightlist(i) , True, xPackage->rewardlist(i) });
//            }
//            return xRewardCount == xListWeightReward.GetCount();
//        }
//    }
//    return False;
//}

PJServerConfig::~PJServerConfig()
{
    //SAFE_ENUMERATOR_DELETE(m_DictObjectRes);
    //SAFE_ENUMERATOR_DELETE(m_DictItemDrugs);
    //SAFE_ENUMERATOR_DELETE(m_DictItemEquip);


    //SAFE_ENUMERATOR_DELETE(m_DictSkill);
    FAST_FOREACH(m_Dict_Type_LevelInfo)
    {
        SAFE_ENUMERATOR_DELETE(m_Dict_Type_LevelInfo.Value());
    }
    SAFE_ENUMERATOR_DELETE(m_DictBorn);
    SAFE_DELETE(m_CommonBorn);
    SAFE_ENUMERATOR_DELETE(m_DictSummonpet);
}

//
//// ���ݱ����������Tips
//Boolean PJServerConfig::FillItemTipsByItem(item* xItem)
//{
//    //mstr xTips = "";
//    //CHAR szBuff[KB_SIZE];
//    //if (m_Dict_item.ContainsKey(xItem->item_id()))
//    //{
//    //    lua_item* xItemConfig = m_Dict_item[xItem->item_id()];
//    //    if (xItemConfig && xItemConfig->type() == "ҩƷ")
//    //    {
//    //        DrugsItem xDrugs(xItem, xItemConfig);
//
//    //        xItem->set_item_res_file(xItemConfig->res_file());
//    //        xItem->set_item_res_key(xItemConfig->res_key());
//    //        xItem->set_item_name(xItemConfig->name());
//
//    //        sprintf_s(szBuff, "�ܼ�%d��\n", xItem->item_count());
//    //        xTips += szBuff;
//
//    //        if (xDrugs.GetRecoveryHP() > 0)
//    //        {
//    //            sprintf_s(szBuff, "�ָ�����%d\n", xDrugs.GetRecoveryHP());
//    //            xTips += szBuff;
//    //        }
//
//    //        if (xDrugs.GetRecoveryMP() > 0)
//    //        {
//    //            sprintf_s(szBuff, "�ָ�����%d\n", xDrugs.GetRecoveryMP());
//    //            xTips += szBuff;
//    //        }
//
//    //        xItem->set_item_tips(xTips);
//    //    }
//    //    else if (xItemConfig && xItemConfig->type() == "װ��")
//    //    {
//    //        EquipItem xEquip(xItem, xItemConfig);
//
//    //        xItem->set_item_res_file(xItemConfig->res_file());
//    //        xItem->set_item_res_key(xItemConfig->res_key());
//    //        xItem->set_item_name(xItemConfig->name());
//
//    //        sprintf_s(szBuff, "�ܼ�%d��\n", xItem->item_count());
//    //        xTips += szBuff;
//
//    //        if (xEquip.GetHpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetHpPoint()); xTips += szBuff; }
//    //        if (xEquip.GetMpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetMpPoint()); xTips += szBuff; }
//    //        if (xEquip.GetApPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetApPoint()); xTips += szBuff; }
//    //        if (xEquip.GetSpPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetSpPoint()); xTips += szBuff; }
//    //        if (xEquip.GetSurplusPoint() > 0) { sprintf_s(szBuff, "����+%d\n", xEquip.GetSurplusPoint()); xTips += szBuff; }
//
//    //        xItem->set_item_tips(xTips);
//    //    }
//    //    return True;
//    //}
//    return False;
//}

