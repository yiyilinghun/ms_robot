#pragma once

static class GlobalPlayer
{
public:
    static mstr Create_Encrypt_AccountInfo(mstr& xUsername, mstr& xPassword);

    static DWORD GetNickNameColorByRebornSum(Int32 xRebornSum);

    //// ���ݱ������ݲ��ҵ�һ���ո�
    //static Int32 FindFirstFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xMaxPosCount);

    //// ���ݱ������ݲ���ָ��ǰ��ո���
    //static MsList<Int32> FindFrontFreeBagPos(MsProtoDictionary<Int64, item>& xProtoDict_Bag, Int32 xFindPosCount, Int32 xMaxPosCount);
};
