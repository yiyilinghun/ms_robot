#pragma once
#define MAX_PACKET_DATA_LEN (512*KB_SIZE)
//#define MAX_PACKET_DATA_LEN 20*MB_SIZE

#pragma pack(push,1)
struct MsNetPacketHead
{
    //#define MNPH_FLAG_FORWARD       0x0000FFFF
    //#define MNPH_FLAG_TO_UNIT_KEY   0x0000FFFE
        //DWORD m_Flag;
    WORD m_PacketIndex;
    DWORD m_PacketLen;
    //WORD m_FunctionId;
    //QWORD m_Param;
};


class MsNetPacket
{
public:
    MsNetPacket();
    ~MsNetPacket();

    void ProtoStr2M(const BaseProto& xSrcProto, LPBASEPROTO xTarProto);
    void ProtoStr2U8(const BaseProto& xSrcProto, LPBASEPROTO xTarProto);

    Boolean FromStream(WORD xPacketIndex, MsMemoryStream& xMsMemoryStream, QWORD xTonke = INVALID_QID);
    Boolean FromProto(WORD xPacketIndex, BaseProto& xBaseProto, Boolean IsUTF8);
    Boolean ToProto(BaseProto& xBaseProto, Boolean IsUTF8);

public:
    MsNetPacketHead m_MsNetPacketHead;
    BYTE m_PacketData[MAX_PACKET_DATA_LEN];
    DWORD m_dwCompleteBytes;
    Boolean m_IsMergePacket;
};
#pragma pack(pop)
