#pragma once
// 高级钩子消息索引
#define NET_HOOK_PACKET_INDEX_HIGHER ((Int64)-1)

// 低级钩子消息索引
#define NET_HOOK_PACKET_INDEX_LOWER ((Int64)-2)

class MsNetTerminal;
class MsNetPacket;
typedef Boolean(*LP_NET_EXECUTE_PACKET)(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket);

#define AUTO_NET_REGISTER(factory, pid, proto) Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket);\
    MsNetPacketRegisterAssist g_ms_auto_##proto##pid(factory, pid, _fun_##pid); \
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, proto& x##proto); \
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket){proto x##proto; if (xBasePacket.ToProto(x##proto, False)){ return _fun_##pid(xMsNetTerminal, x##proto); }return False; }\
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, proto& xPacket)

#define AUTO_NET_REGISTER_AUTO_ID(factory, proto)   AUTO_NET_REGISTER(factory, proto, Msg_##proto)

#define AUTO_NET_REGISTER_FORWARD_KEY(factory, pid, proto) Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket);\
    MsNetPacketRegisterAssist g_ms_auto_##proto##pid(factory, pid, _fun_##pid); \
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, proto& x##proto, Int64 xForwardKey); \
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket){proto x##proto; if (xBasePacket.ToProto(x##proto, True)){ return _fun_##pid(xMsNetTerminal, x##proto, xBasePacket.GetForwardKey()); }return False; }\
    DINLINE Boolean _fun_##pid(MsNetTerminal* xMsNetTerminal, proto& x##proto, Int64 xForwardKey)

#define AUTO_NET_REGISTER_HIGHE_HOOK(factory) Boolean _fun_##factory##NET_HOOK_PACKET_INDEX_HIGHER(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket);\
    MsNetPacketRegisterAssist g_ms_auto_##factory##NET_HOOK_PACKET_INDEX_HIGHER(factory, NET_HOOK_PACKET_INDEX_HIGHER, _fun_##factory##NET_HOOK_PACKET_INDEX_HIGHER); \
    DINLINE Boolean _fun_##factory##NET_HOOK_PACKET_INDEX_HIGHER(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket)

#define AUTO_NET_REGISTER_LOWER_HOOK(factory) Boolean _fun_##factory##NET_HOOK_PACKET_INDEX_LOWER(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket);\
    MsNetPacketRegisterAssist g_ms_auto_##factory##NET_HOOK_PACKET_INDEX_LOWER(factory, NET_HOOK_PACKET_INDEX_LOWER, _fun_##factory##NET_HOOK_PACKET_INDEX_LOWER); \
    DINLINE Boolean _fun_##factory##NET_HOOK_PACKET_INDEX_LOWER(MsNetTerminal* xMsNetTerminal, MsNetPacket& xBasePacket)

class MsNetPacketFactory : public MsIoCompletionPort
{
public:
    DINLINE MsNetPacketFactory() :m_Hook_Higher_NetExecutePacket(nullptr), m_Hook_Lower_NetExecutePacket(nullptr)
    {
        ;
    }

    DINLINE ~MsNetPacketFactory()
    {
        ;
    }

private:
    Boolean RegisterPacket(Int64 xPacketIndex, LP_NET_EXECUTE_PACKET xNetExecutePacket)
    {
        if (xPacketIndex == NET_HOOK_PACKET_INDEX_HIGHER && m_Hook_Higher_NetExecutePacket == nullptr)
        {
            m_Hook_Higher_NetExecutePacket = xNetExecutePacket;
            return True;
        }
        else if (xPacketIndex == NET_HOOK_PACKET_INDEX_LOWER && m_Hook_Lower_NetExecutePacket == nullptr)
        {
            m_Hook_Lower_NetExecutePacket = xNetExecutePacket;
            return True;
        }
        else
        {
            if (m_Dict_NetExecutePacket.ContainsKey(xPacketIndex))
            {
                return False;
            }
            else
            {
                m_Dict_NetExecutePacket.Add(xPacketIndex, xNetExecutePacket);
                return True;
            }
        }
    }

    Boolean ExecutePacket(MsNetTerminal* xMsNetTerminal, MsNetPacket& xMsNetPacket);

    LP_NET_EXECUTE_PACKET m_Hook_Higher_NetExecutePacket;
    LP_NET_EXECUTE_PACKET m_Hook_Lower_NetExecutePacket;
    MsDictionary<Int64, LP_NET_EXECUTE_PACKET> m_Dict_NetExecutePacket;
    friend class MsNetPacketRegisterAssist;
    friend class MsNetTerminal;
};

class MsNetPacketRegisterAssist
{
public: MsNetPacketRegisterAssist(MsNetPacketFactory* xMsNetPacketFactory, Int64 xPacketIndex, LP_NET_EXECUTE_PACKET xNetExecutePacket)
{
    xMsNetPacketFactory->RegisterPacket(xPacketIndex, xNetExecutePacket);
}
};
