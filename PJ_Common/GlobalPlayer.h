#pragma once

static class GlobalPlayer
{
public:
    static mstr Create_Encrypt_AccountInfo(mstr& xUsername, mstr& xPassword);

    static DWORD GetNickNameColorByRebornSum(Int32 xRebornSum);

    //// 根据背包数据查找第一个空格
    //static Int32 FindFirstFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xMaxPosCount);

    //// 根据背包数据查找指定前面空格数
    //static MsList<Int32> FindFrontFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xFindPosCount, Int32 xMaxPosCount);
};
