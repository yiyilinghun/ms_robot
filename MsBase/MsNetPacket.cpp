#include "Precompiled.h"
using namespace google::protobuf;
#undef GetMessage

MsNetPacket::MsNetPacket()
    : m_dwCompleteBytes(0)
{
    ZeroObject(m_MsNetPacketHead);
}

MsNetPacket::~MsNetPacket()
{
    ;
}


void MsNetPacket::ProtoStr2M(const BaseProto& xSrcProto, LPBASEPROTO xTarProto)
{
    const Reflection* xReflection = xSrcProto.GetReflection();
    const Descriptor* xDescriptor = xSrcProto.GetDescriptor();
    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        mstr xName = xField->name();
        if (FieldDescriptor::Type::TYPE_STRING == xField->type() || FieldDescriptor::Type::TYPE_BYTES == xField->type())
        {
            if (xField->is_repeated())
            {
                Int32 xRepeatedSize = xReflection->FieldSize(xSrcProto, xField);
                MsVector<mstr> xTempList;
                for (int i = 0; i < xRepeatedSize; i++)
                {
                    wstr x = MsBase::U82W(xReflection->GetRepeatedString(xSrcProto, xField, i));
                    xTempList.Add(MsBase::W2M(x));
                }
                xReflection->ClearField(xTarProto, xField);
                FAST_FOREACH(xTempList)
                {
                    xReflection->AddString(xTarProto, xField, xTempList.Value());
                }
            }
            else
            {
                wstr xTemp = MsBase::U82W(xReflection->GetString(xSrcProto, xField));
                xReflection->SetString(xTarProto, xField, MsBase::W2M(xTemp));
            }
        }
        else if (FieldDescriptor::Type::TYPE_MESSAGE == xField->type())
        {
            if (xField->is_repeated())
            {
                Int32 xRepeatedSize = xReflection->FieldSize(xSrcProto, xField);
                for (int i = 0; i < xRepeatedSize; i++)
                {
                    const BaseProto& xSrcTemp = xReflection->GetRepeatedMessage(xSrcProto, xField, i);
                    BaseProto* xTarTemp = xReflection->MutableRepeatedMessage(xTarProto, xField, i);
                    this->ProtoStr2U8(xSrcTemp, xTarTemp);
                }
            }
            else
            {
                if (xReflection->HasField(xSrcProto, xField))
                {
                    const BaseProto& xSrcTemp = xReflection->GetMessage(xSrcProto, xField);
                    BaseProto* xTarTemp = xReflection->MutableMessage(xTarProto, xField);
                    this->ProtoStr2U8(xSrcTemp, xTarTemp);
                }
            }
        }
    }
}

void MsNetPacket::ProtoStr2U8(const BaseProto& xSrcProto, LPBASEPROTO xTarProto)
{
    const Reflection* xReflection = xSrcProto.GetReflection();
    const Descriptor* xDescriptor = xSrcProto.GetDescriptor();
    Int32 xFieldCount = xDescriptor->field_count();
    for (Int32 xFieldIndex = 0; xFieldIndex < xFieldCount; xFieldIndex++)
    {
        const FieldDescriptor* xField = xDescriptor->field(xFieldIndex);
        mstr xName = xField->name();
        if (FieldDescriptor::Type::TYPE_STRING == xField->type() || FieldDescriptor::Type::TYPE_BYTES == xField->type())
        {
            if (xField->is_repeated())
            {
                Int32 xRepeatedSize = xReflection->FieldSize(xSrcProto, xField);
                MsList<utf8str> xTempList;
                for (int i = 0; i < xRepeatedSize; i++)
                {
                    wstr x = MsBase::M2W(xReflection->GetRepeatedString(xSrcProto, xField, i));
                    xTempList.Add(MsBase::W2U8(x));
                }
                xReflection->ClearField(xTarProto, xField);
                FAST_FOREACH(xTempList)
                {
                    xReflection->AddString(xTarProto, xField, xTempList.Value());
                }
            }
            else
            {
                wstr x = MsBase::M2W(xReflection->GetString(xSrcProto, xField));
                utf8str xTemp = MsBase::W2U8(x);
                xReflection->SetString(xTarProto, xField, xTemp);
            }
        }
        else if (FieldDescriptor::Type::TYPE_MESSAGE == xField->type())
        {
            if (xField->is_repeated())
            {
                Int32 xRepeatedSize = xReflection->FieldSize(xSrcProto, xField);
                for (int i = 0; i < xRepeatedSize; i++)
                {
                    const BaseProto& xSrcTemp = xReflection->GetRepeatedMessage(xSrcProto, xField, i);
                    LPBASEPROTO xTarTemp = xReflection->MutableRepeatedMessage(xTarProto, xField, i);
                    this->ProtoStr2U8(xSrcTemp, xTarTemp);
                }
            }
            else
            {
                if (xReflection->HasField(xSrcProto, xField))
                {
                    const BaseProto& xSrcTemp = xReflection->GetMessage(xSrcProto, xField);
                    LPBASEPROTO xTarTemp = xReflection->MutableMessage(xTarProto, xField);
                    this->ProtoStr2U8(xSrcTemp, xTarTemp);
                }
            }
        }
    }
}

Boolean MsNetPacket::FromStream(WORD xPacketIndex, MsMemoryStream& xMsMemoryStream, QWORD xTonke)
{
    if (xTonke == INVALID_QID)
    {
        m_MsNetPacketHead.m_PacketLen = xMsMemoryStream.m_InOffect;
        if (m_MsNetPacketHead.m_PacketLen <= MAX_PACKET_DATA_LEN)
        {
            m_MsNetPacketHead.m_PacketIndex = xPacketIndex;
            memcpy(m_PacketData, xMsMemoryStream.GetBuff(), m_MsNetPacketHead.m_PacketLen);
            return True;
        }
    }
    else
    {
        m_MsNetPacketHead.m_PacketLen = xMsMemoryStream.m_InOffect + sizeof(xTonke);
        if (m_MsNetPacketHead.m_PacketLen <= MAX_PACKET_DATA_LEN)
        {
            m_MsNetPacketHead.m_PacketIndex = xPacketIndex;
            memcpy(m_PacketData, &xTonke, sizeof(xTonke));
            memcpy(m_PacketData + sizeof(xTonke), xMsMemoryStream.GetBuff(), m_MsNetPacketHead.m_PacketLen - sizeof(xTonke));
            return True;
        }
    }
    return False;
}

Boolean MsNetPacket::FromProto(WORD xPacketIndex, BaseProto& xBaseProto, Boolean IsUTF8)
{
    LPBASEPROTO lpTemp = &xBaseProto;
    if (IsUTF8)
    {
        lpTemp = xBaseProto.New();
        lpTemp->CopyFrom(xBaseProto);
        this->ProtoStr2U8(xBaseProto, lpTemp);
    }
    m_MsNetPacketHead.m_PacketLen = lpTemp->ByteSize();
    if (lpTemp->IsInitialized())
    {
        if (m_MsNetPacketHead.m_PacketLen <= MAX_PACKET_DATA_LEN && lpTemp->SerializeToArray(m_PacketData, m_MsNetPacketHead.m_PacketLen))
        {
            m_MsNetPacketHead.m_PacketIndex = xPacketIndex;
            if (IsUTF8) { SAFE_DELETE(lpTemp); }
            return True;
        }
        if (IsUTF8) { SAFE_DELETE(lpTemp); }
        return False;
    }
    else
    {
        MsBase::ShowNotExistField(xBaseProto);
        if (IsUTF8) { SAFE_DELETE(lpTemp); }
        return False;
    }
}

Boolean MsNetPacket::ToProto(BaseProto& xBaseProto, Boolean IsUTF8)
{
    Boolean xRet = xBaseProto.ParseFromArray(m_PacketData, m_MsNetPacketHead.m_PacketLen);
    if (xRet && IsUTF8)
    {
        BaseProto* u8_Proto = xBaseProto.New();
        u8_Proto->CopyFrom(xBaseProto);
        this->ProtoStr2M(xBaseProto, u8_Proto);
        xBaseProto.CopyFrom(*u8_Proto);
        SAFE_DELETE(u8_Proto);
    }
    return xRet;
}
