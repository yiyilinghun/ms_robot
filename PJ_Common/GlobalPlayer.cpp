#include "Precompiled.h"

mstr GlobalPlayer::Create_Encrypt_AccountInfo(mstr& xUsername, mstr& xPassword)
{
    mstr x = GETSTRMD5(xUsername);
    x += xPassword;
    return GETSTRMD5(x);
}

DWORD GlobalPlayer::GetNickNameColorByRebornSum(Int32 xRebornSum)
{
    switch (xRebornSum)
    {
        case 0x00: { return 0xFFFFFFFF; }break;
        case 0x01: { return 0xFF00BB00; }break;
        case 0x02: { return 0xFF008800; }break;
        case 0x03: { return 0xFFBBBB00; }break;
        case 0x04: { return 0xFF888800; }break;
        case 0x05: { return 0xFF00BBBB; }break;
        case 0x06: { return 0xFF008888; }break;
        case 0x07: { return 0xFF0000BB; }break;
        case 0x08: { return 0xFF000088; }break;
        case 0x09: { return 0xFFBB00BB; }break;
        case 0x0A: { return 0xFF880088; }break;
        case 0x0B: { return 0xFFBB0000; }break;
        case 0x0C: { return 0xFF880000; }break;

        default: { return 0xFF000000; }break;
    }
}

//Int32 GlobalPlayer::FindFirstFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xMaxPosCount)
//{
//    MsSet<Int32> xSetPos;
//    FAST_FOREACH(xProtoDict_Bag) { xSetPos.Add(xEnumValue.item_pos()); }
//
//    for (Int32 i = 0; i < xMaxPosCount; i++)
//    {
//        if (!xSetPos.Contains(i))
//        {
//            return i;
//        }
//    }
//    return INVALID_NID;
//}

//// 根据背包数据查找指定前面空格数
//MsList<Int32> GlobalPlayer::FindFrontFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xFindPosCount, Int32 xMaxPosCount)
//{
//    MsSet<Int32> xSetPos;
//    FAST_FOREACH(xProtoDict_Bag) { xSetPos.Add(xEnumValue.item_pos()); }
//
//    MsList<Int32> xList;
//    for (Int32 i = 0; i < xMaxPosCount; i++)
//    {
//        if (!xSetPos.Contains(i))
//        {
//            xList.Add(i);
//            if (xList.GetCount() == xFindPosCount)
//            {
//                return xList;
//            }
//        }
//    }
//    return xList;
//}
