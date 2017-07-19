#include "Precompiled.h"

void UD_Npc_Shop_Item::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc_Shop_Item::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_NPC_Shop_Item);

    //this->GetGameItem()->Load();

    return True;
}

void UD_Npc_Shop_Item::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_NPC_Shop_Item);
    FAST_READ_INT32_VALUE(�չ���);
    FAST_READ_INT32_VALUE(���ۼ�);
    FAST_READ_INT32_VALUE(��ʼ����);

    this->Set�Ƿ��������(Attribs["�Ƿ��������"]);
    this->GetGameItem()->LoadFromObviousXml(xMsXmlReader);
}








void UD_Npc_Shop::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc_Shop::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_NPC_Shop);

    this->Load�̵���Ʒ�б�();
    return True;
}

void UD_Npc_Shop::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_NPC_Shop);

    this->Set�ǳ�(Attribs["�ǳ�"]);
    this->Set����(Attribs["����"]);

    this->Load�̵���Ʒ�б�(xMsXmlReader);
    //xMsXmlReader["�̵���Ʒ�б�"];
    //universal_data* xUData = this->Get�̵���Ʒ�б�();
    //if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {
    //    do
    //    {
    //        UD_Npc_Shop_Item xPJC_Scene_NPC_Shop_Item(xMsXmlReader);
    //        xUData->add_data()->CopyFrom(xPJC_Scene_NPC_Shop_Item.GetUD());
    //    } while (xMsXmlReader.FindNextElem());
    //}
    //xMsXmlReader.SelectParentElem();
}






void UD_Npc_DlgText::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc_DlgText::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_Npc_DlgText);
    return True;
}

void UD_Npc_DlgText::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_Npc_DlgText);
    FAST_READ_STR_VALUE(����);
}







void UD_Npc_Dlg::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc_Dlg::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_Npc_Dlg);

    this->Load�ı�();

    return True;
}

void UD_Npc_Dlg::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_Npc_Dlg);
    this->Load�ı�(xMsXmlReader);
    //universal_data* xUData = this->Get�ı�();
    //xUData->set_key(INVALID_LID);
    //xUData->set_type("group");
    //xUData->set_name("group");
    //AssertLog("%s", xMsXmlReader.GetTagName().c_str());
    //if (xMsXmlReader.FindChildElem()) if (xMsXmlReader) {
    //    do
    //    {
    //        UD_Npc_DlgText xDlgText(xMsXmlReader);
    //        xUData->add_data()->CopyFrom(xDlgText.GetUD());
    //    } while (xMsXmlReader.FindNextElem());
    //}
    //xMsXmlReader.SelectParentElem();
}






void UD_Npc::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_NPC);

    this->Load�Ի��б�();
    this->Load�̵��б�();
    return True;
}

void UD_Npc::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_NPC);
    //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
    //this->SetName(Attribs.m_MsXmlReader->GetTagName());
    //this->SetType("NPC");

    this->Setת������(atoi(Attribs["ת������"].c_str()));
    this->Set�ȼ�(atoi(Attribs["�ȼ�"].c_str()));

    this->Set����X((Single)atof(Attribs["����X"].c_str()));
    this->Set����Y((Single)atof(Attribs["����Y"].c_str()));

    this->Set����(Attribs["����"]);
    this->Set�ǳ�(Attribs["�ǳ�"]);
    this->Set����(Attribs["����"]);
    this->Set�Ա�(Attribs["�Ա�"]);
    this->Set��ɫ����(Attribs["��ɫ����"]);

    this->Load�Ի��б�(xMsXmlReader);
    this->Load�̵��б�(xMsXmlReader);
}

