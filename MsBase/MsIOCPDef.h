#pragma once
#define MAX_ENTRIES_COUNT   2048

// 完成端口传递对象类型枚举
enum MS_NET_TERMINAL_OL_TYPE
{
    SEND_OL,
    RECV_OL,
};

// 完成端口传递对象类型枚举
enum IOCP_OL_TYPE
{
    IOT_BASE,
    IOT_NET,
    IOT_PACKET,
    IOT_WEB,
    IOT_DIY,
};

#define MONITOR_SERVER_IOCP_UNIT_KEY            0x7FFFFFFFFFFFF001
#define CLUSTER_NETSERVER_IOCP_UNIT_KEY         0x7FFFFFFFFFFFD002

#define SERVER_LOGINMANAGER_IOCP_UNIT_KEY       0x7FFFFFFFFFFFE001
#define SERVER_SCENEMANAGER_IOCP_UNIT_KEY       0x7FFFFFFFFFFFE002

#define CENTER_WEBMANAGER_IOCP_UNIT_KEY         0x7FFFFFFFFFFFF001
#define CENTER_NODEMANAGER_IOCP_UNIT_KEY        0x7FFFFFFFFFFFF002
#define CENTER_SCENEMANAGER_IOCP_UNIT_KEY       0x7FFFFFFFFFFFF003
#define CENTER_ROLEDATAMANAGER_IOCP_UNIT_KEY    0x7FFFFFFFFFFFF004
#define CENTER_MAILMANAGER_IOCP_UNIT_KEY        0x7FFFFFFFFFFFF005


#define AR_UID_EXE(_class_, _fun_) class c##_class_##_fun_ : public _class_ { public:Boolean _class_##_fun_(iocp_base_ol*); }; AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_fun_(ERT_##_class_, MsBase::GetProtoIntKey(""#_fun_), (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_fun_::_class_##_fun_, GetIocpUnitRoute(ERT_##_class_)); Boolean c##_class_##_fun_::_class_##_fun_(iocp_base_ol* lpIocpol)

#define AR_STR_EXE(_class_, _fun_) class c##_class_##_fun_ : public _class_ { public:Boolean _class_##_fun_(iocp_base_ol*); }; AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_fun_(ERT_##_class_, ""#_fun_, (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_fun_::_class_##_fun_, GetIocpUnitRoute(ERT_##_class_)); Boolean c##_class_##_fun_::_class_##_fun_(iocp_base_ol* lpIocpol)

#define AR_STR_EXE_DIY(_class_, _fun_, _key_fun_)   class c##_class_##_key_fun_ : public _class_ { public:Boolean _class_##_key_fun_(iocp_base_ol*); }; AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_key_fun_(ERT_##_class_, ""#_fun_, (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_key_fun_::_class_##_key_fun_, GetIocpUnitRoute(ERT_##_class_)); Boolean c##_class_##_key_fun_::_class_##_key_fun_(iocp_base_ol* lpIocpol)

#define AR_PACKET_EXE(_class_, _packet_) class c##_class_##_packet_ : public _class_ { public:Boolean _class_##_packet_(iocp_packet_ol*);Boolean _class_##packet##_packet_(MsNetTerminal*, _packet_&);};AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_packet_(ERT_##_class_, MsBase::GetProtoIntKey(""#_packet_), (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_packet_::_class_##_packet_, GetIocpUnitRoute(ERT_##_class_));\
Boolean c##_class_##_packet_::_class_##_packet_(iocp_packet_ol* xIocpPacketol){_packet_ xPacket; xIocpPacketol->m_Packet->ToProto(xPacket, xIocpPacketol->m_MsNetTerminal->m_IsUTF8);return _class_##packet##_packet_(xIocpPacketol->m_MsNetTerminal, xPacket);}Boolean c##_class_##_packet_::_class_##packet##_packet_(MsNetTerminal* xMsNetTerminal, _packet_& xPacket)

#define AR_WEB_EXE(_class_, _fun_) class c##_class_##_fun_ : public _class_{ public:Boolean _class_##_fun_(iocp_web_ol*); Boolean _class_##packet##_fun_(iocp_web_ol*, MsVector<mstr>&, HTTP_RESPONSE*); }; AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_fun_(ERT_##_class_, ""#_fun_, (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_fun_::_class_##_fun_, GetIocpUnitRoute(ERT_##_class_)); \
Boolean c##_class_##_fun_::_class_##_fun_(iocp_web_ol* xIocpWebol)

#define AR_WEB_EXE_DIY(_class_, _fun_, _key_fun_)   class c##_class_##_key_fun_ : public _class_{ public:Boolean _class_##_key_fun_(iocp_web_ol*); Boolean _class_##packet##_key_fun_(iocp_web_ol*, MsVector<mstr>&, HTTP_RESPONSE*); }; AutoRegExeFunIocpUnitAssist ms_AutoRegExeFunIocpUnitAssist_##_class_##_key_fun_(ERT_##_class_, ""#_fun_, (UnitRoute<LogicUnit>::LPEXEFUN)&c##_class_##_key_fun_::_class_##_key_fun_, GetIocpUnitRoute(ERT_##_class_)); \
Boolean c##_class_##_key_fun_::_class_##_key_fun_(iocp_web_ol* xIocpWebol)

#define LAMTA_UID_CB(_fun_) GetRoute()->m_FunLamtaId[EventId] = [this](iocp_base_ol* lpIocpol)
#define LAMTA_STR_CB(_fun_) GetRoute()->m_FunLamtaStr[EventId] = [this](iocp_base_ol* lpIocpol)
#define LAMTA_TIMER(EventId, xInterval, xInstantly)  this->SetTimer(EventId, xInterval, 0, xInstantly); GetRoute()->m_FunLamtaId[EventId] = [this](iocp_base_ol* lpIocpol)
#define LAMTA_TIMER_SPECIFY(xUnit, EventId, xInterval, xParam, xInstantly)  xUnit->SetTimer(EventId, xInterval, xParam, xInstantly); xUnit->GetRoute()->m_FunLamtaId[EventId] = [this](iocp_base_ol* lpIocpol)

enum E_ROUTE_TYPE
{
    ERT_IocpUnit,
    ERT_LogicUnit,
    ERT_MsNetTerminal,
    ERT_WebMonitorServer,

    ERT_RenderManager,
    ERT_MsWebServer,
    ERT_MsNetServer,
    ERT_CenterWebManager,
    ERT_CenterSceneManager,
    ERT_CenterRoleDataManager,
    ERT_CenterNodeManager,
    ERT_CenterMailManager,

    ERT_LoginManager,
    ERT_LoginPlayer,

    ERT_SceneManager,
    ERT_ScenePlayer,

    ERT_RobotPlayer,
    ERT_MainPlayer,
    ERT_ServerBaseScene,

    ERT_MAX_COUNT,
};

#define DEF_IOCPUNIT_TYPE(c) m_IocpUnitType = E_ROUTE_TYPE::ERT_##c;
