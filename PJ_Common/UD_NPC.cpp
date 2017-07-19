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
    FAST_READ_INT32_VALUE(收购价);
    FAST_READ_INT32_VALUE(销售价);
    FAST_READ_INT32_VALUE(初始数量);

    this->Set是否可以跑商(Attribs["是否可以跑商"]);
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

    this->Load商店商品列表();
    return True;
}

void UD_Npc_Shop::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_NPC_Shop);

    this->Set昵称(Attribs["昵称"]);
    this->Set类型(Attribs["类型"]);

    this->Load商店商品列表(xMsXmlReader);
    //xMsXmlReader["商店商品列表"];
    //universal_data* xUData = this->Get商店商品列表();
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
    FAST_READ_STR_VALUE(内容);
}







void UD_Npc_Dlg::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_Npc_Dlg::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);
    FAST_READ_TYPE(Scene_Npc_Dlg);

    this->Load文本();

    return True;
}

void UD_Npc_Dlg::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_Npc_Dlg);
    this->Load文本(xMsXmlReader);
    //universal_data* xUData = this->Get文本();
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

    this->Load对话列表();
    this->Load商店列表();
    return True;
}

void UD_Npc::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(Scene_NPC);
    //MsXmlAttribReader& Attribs = xMsXmlReader.Attribs;
    //this->SetName(Attribs.m_MsXmlReader->GetTagName());
    //this->SetType("NPC");

    this->Set转生次数(atoi(Attribs["转生次数"].c_str()));
    this->Set等级(atoi(Attribs["等级"].c_str()));

    this->Set坐标X((Single)atof(Attribs["坐标X"].c_str()));
    this->Set坐标Y((Single)atof(Attribs["坐标Y"].c_str()));

    this->Set朝向(Attribs["朝向"]);
    this->Set昵称(Attribs["昵称"]);
    this->Set种族(Attribs["种族"]);
    this->Set性别(Attribs["性别"]);
    this->Set角色类型(Attribs["角色类型"]);

    this->Load对话列表(xMsXmlReader);
    this->Load商店列表(xMsXmlReader);
}

