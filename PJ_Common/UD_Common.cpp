#include "Precompiled.h"

void UD_GameEquipBaseAttr::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_GameEquipBaseAttr::Load()
{
    FAST_READ_TYPE(GameEquipBaseAttr);
    return True;
}

void UD_GameEquipBaseAttr::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(GameEquipBaseAttr);
}





//
//UD_Item_Drugs* UD_GameItem::GetItemByDrugs()
//{
//    return UD_Base::GetFieldByIndex<UD_Item_Drugs>(0, True);
//}
//void UD_GameItem::SetItemToDrugs(UD_Item_Drugs* xItemDrugs)
//{
//    return UD_Base::SetFieldByIndex<UD_Item_Drugs>(0, xItemDrugs);
//}
//
//UD_Item_Equip* UD_GameItem::GetItemByEquip()
//{
//    return UD_Base::GetFieldByIndex<UD_Item_Equip>(0, True);
//}
//void UD_GameItem::SetItemToEquip(UD_Item_Equip* xItemEquip)
//{
//    return UD_Base::SetFieldByIndex<UD_Item_Equip>(0, xItemEquip);
//}
//








void UD_GameItem::SetName(mstr xName)
{
    return UD_Base::SetName(xName);
}

Boolean UD_GameItem::Load()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_Data);

    FAST_READ_TYPE(GameItem);
    return True;
}

void UD_GameItem::LoadFromObviousXml(MsXmlReader& xMsXmlReader)
{
    FAST_READ_ATTR_TYPE_NAME(GameItem);

    FAST_READ_INT32_VALUE(ItemPos);
    FAST_READ_INT32_VALUE(ItemCount);
    FAST_READ_INT64_VALUE(RoleIndex);
    FAST_READ_STR_VALUE(ItemName);
    FAST_READ_STR_VALUE(ItemType);
}

Int64 UD_GameItem::GetItemIndex()
{
    return m_prt->key();
}

