#pragma once
#include "GlobalPlayer.h"

#define ROBOT_PACKET_EXE(_packet_)   AR_PACKET_EXE(RobotPlayer, _packet_)


//#define ROBOTPLAYER_AUTO_REGISTER_PROTO(id)     AUTO_REGISTER_REALIZE_PROTO(RobotPlayer, GET_PJ_ROBOT_FACTORY(), id)
//#define ROBOTPLAYER_AUTO_REGISTER(id)           ARCB(RobotPlayer, GET_PJ_ROBOT_FACTORY(), id)
//#define ROBOT_ASYN_EXE_MSG(msg)                 ASYN_EXE_MSG(RobotPlayer, msg, ROBOTPLAYER_AUTO_REGISTER)

enum E_ROBOT_STATE
{
    ERS_INVALID,
    ERS_STOP,
    ERS_READY,
    ERS_WAIT,
};

enum ROBOT_CB_MSG
{
    ROBOT_CB_SEND_PACKET = 0x10000000,
    ROBOT_CB_C2S_LOGIN,


};

class RobotView;
class RobotPlayer : public MsNetTerminal
{
public:
    RobotPlayer(MsIOCPManager* xMsIOCPManager, RobotView* xRobotView);
    virtual ~RobotPlayer();
    virtual Boolean TickOnce();
    virtual void OnAttach();
    virtual void Close();
    void OnMsg(OVERLAPPED_ENTRY& xEntry);

    void RobotSendPacket(BaseProto& xBaseProto);

    Boolean IsChange();
    E_ROBOT_STATE GetState() { return m_State; }
    void ReadyState() { m_State = ERS_READY; }
    void WatiState() { m_State = ERS_WAIT; }

    void Roboto_C2S_Login();

    void Log(mstr xLog);
    void FillLog(LPBASEPROTO xProto, Boolean IsSend);
    Boolean m_IsConnected;
    mstr m_Account;
    mstr m_Password;
    mstr m_NickName;
    Int64 m_RoleIndex;
    UD_RoleInfo m_RoleInfo;
    Int64 m_SceneInstanceId;
    Int64 m_LoginRoleIndex;

    RobotView* m_RobotView;
    MsList<mstr> m_RobotLogs;
protected:
    E_ROBOT_STATE m_State;
    Boolean m_IsChange;
};
