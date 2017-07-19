#include "Precompiled.h"

Int64 g_RobotNextIndex = 1000;
RobotPlayer::RobotPlayer(MsIOCPManager* xMsIOCPManager, RobotView* xRobotView)
    : MsNetTerminal(xMsIOCPManager, False, INVALID_SOCKET)
    , m_RoleIndex(INVALID_LID)
    , m_SceneInstanceId(INVALID_LID)
    , m_IsConnected(False)
    , m_State(ERS_STOP)
    , m_RobotView(xRobotView)
{
    CHAR szBuff[100];
    sprintf_s(szBuff, "msRobot%lld", (Int64)::MsInterlockedAdd(&g_RobotNextIndex, 1));
    //sprintf_s(szBuff, "%d", R_WORD(100, 1000));
    m_Account = szBuff;
    m_Password = szBuff;
    m_NickName = szBuff;
}

RobotPlayer::~RobotPlayer()
{
    ;
}

Boolean RobotPlayer::TickOnce()
{
    return True;
}

void RobotPlayer::OnAttach()
{
    m_IsExeClose = False;
    return MsNetTerminal::OnAttach();
}

void RobotPlayer::Close()
{
    this->m_State = E_ROBOT_STATE::ERS_STOP;
    this->m_IsChange = True;
    m_IsConnect = False;
    return MsNetTerminal::Close();
}

void RobotPlayer::OnMsg(OVERLAPPED_ENTRY& xEntry)
{
    m_IsChange = True;
    return MsNetTerminal::OnMsg(xEntry);
}

void RobotPlayer::Log(mstr xLog)
{
    AssertLog("%s", xLog.c_str());
}

void RobotPlayer::FillLog(LPBASEPROTO xProto, Boolean IsSend)
{
    if (IsSend)
    {
        m_RobotLogs.Add("Send:[" + xProto->GetTypeName() + "]" + xProto->DebugString());
    }
    else
    {
        m_IsChange = True;
        m_RobotLogs.Add("Recv:[" + xProto->GetTypeName() + "]" + xProto->DebugString());
    }
    if (m_RobotLogs.GetCount() > 1000) { m_RobotLogs.RemoveAtFirst(); }
}

void RobotPlayer::RobotSendPacket(BaseProto& xBaseProto)
{
    m_IsChange = True;
    FillLog(&xBaseProto, True);
    m_State = ERS_WAIT;

    iocp_diy_ol* lpIocpol = new iocp_diy_ol(this, ROBOT_CB_SEND_PACKET);
    MsNetPacket* lpMsNetPacket = new MsNetPacket();
    if (lpMsNetPacket->FromProto(MsBase::GetProtoIntKey(xBaseProto), xBaseProto, m_IsUTF8))
    {
        lpIocpol->Pointer = lpMsNetPacket;
        m_MsIOCPManager->PostOLMsg(m_UnitIndex, lpIocpol);
    }
    else
    {
        MsBase::ShowNotExistField(xBaseProto);
    }
}

Boolean RobotPlayer::IsChange()
{
    if (m_IsChange)
    {
        m_IsChange = False;
        return True;
    }
    else
    {
        m_IsChange = False;
        return False;
    }
}

AR_UID_EXE(RobotPlayer, ROBOT_CB_SEND_PACKET)
{
    MsNetPacket* lpMsNetPacket = (MsNetPacket*)lpIocpol->Pointer;
    this->SendPacket(*lpMsNetPacket);
    SAFE_DELETE(lpMsNetPacket);
    return True;
};

ROBOT_PACKET_EXE(S2C_Sync_Dir)
{
    FillLog(&xPacket, False);
    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_Chat)
{
    FillLog(&xPacket, False);
    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_RoleCreate)
{
    FillLog(&xPacket, False);

    if (xPacket.ret_code() == "")
    {
        C2S_Ask_RoleSum xAsk;
        xAsk.set_account(xPacket.account());
        this->RobotSendPacket(xAsk);
    }
    //switch (xPacket.ret_code())
    //{
    //    case Msg_Ret_Code_OK:
    //    {
    //        C2S_RoleSum xAsk;
    //        xAsk.set_account(xPacket.account());
    //        this->RobotSendPacket(xAsk);
    //    }break;

    //    //case Msg_Ret_Code_Nickname_Exist: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "昵称已存在", "提示", MB_OK); }break;

    //    //case Msg_Ret_Code_CreateRole_Failed:
    //    //default: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "创建角色失败", "提示", MB_OK); }break;
    //}
    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_Login)
{
    FillLog(&xPacket, False);
    if (xPacket.ret_code() == "")
    {
        C2S_Ask_RoleSum xAsk;
        xAsk.set_account(xPacket.account());
        this->RobotSendPacket(xAsk);
    }
    //switch (xPacket.ret_code() == "")
    //{
    //    case Msg_Ret_Code_OK:
    //    {
    //        C2S_RoleSum xAsk;
    //        xAsk.set_account(xPacket.account());
    //        this->RobotSendPacket(xAsk);
    //    }break;

    //    case Msg_Ret_Code_Login_Failed:
    //    {
    //        //g_Account = "";
    //        //g_Password = "";
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "账号或密码错误", "提示", MB_OK);
    //    }break;

    //    case Msg_Ret_Code_Username_To_Short:
    //    case Msg_Ret_Code_Username_To_Long:
    //    case Msg_Ret_Code_Username_Exist:
    //    {
    //        //g_Account = "";
    //        //g_Password = "";
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "用户名错误", "提示", MB_OK);
    //    }break;

    //    default:
    //    {
    //        //g_Account = "";
    //        //g_Password = "";
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "登录失败,未知错误!", "提示", MB_OK);
    //    }break;
    //}
    return True;
}

ROBOT_PACKET_EXE(S2C_Sync_RoleMove)
{
    FillLog(&xPacket, False);
    if (xPacket.role_index() == m_RoleIndex)
    {
        Position xPos = { xPacket.end_x(), xPacket.end_y() };
        this->m_RoleInfo.GetRolePublic()->SetPos(xPos);
        this->m_State = E_ROBOT_STATE::ERS_READY;
    }

    //if (g_lpCurrentScene && m_JourneyRender->m_lpClientGameScene)
    //{
    //    // 如果是主角自己
    //    if (xPacket.role_index() == g_lpMainPlayer->m_lpGamePlayer->m_RoleIndex)
    //    {
    //        if (xPacket.is_run() == True)
    //        {
    //            g_lpMainPlayer->m_lpGamePlayer->SetRun();
    //        }
    //        else
    //        {
    //            g_lpMainPlayer->m_lpGamePlayer->SetWalk();
    //        }
    //        //g_CurrentMainPlayer->TransferTo(g_lpCurrentScene, xPacket.begin_x(), xPacket.begin_y());
    //        //g_lpMainPlayer->m_lpGamePlayer->MoveTo(xPacket.end_x(), xPacket.end_y());
    //    }
    //    else if (m_JourneyRender->m_lpClientGameScene->m_Dict_Id_OtherGameObject.ContainsKey(xPacket.role_index()))
    //    {
    //        GameObject* xGameObject = m_JourneyRender->m_lpClientGameScene->m_Dict_Id_OtherGameObject[xPacket.role_index()];
    //        if (xPacket.is_run() == True)
    //        {
    //            xGameObject->SetRun();
    //        }
    //        else
    //        {
    //            xGameObject->SetWalk();
    //        }
    //        //xGameObject->TransferTo(g_lpCurrentScene, xPacket.begin_x(), xPacket.begin_y());
    //        xGameObject->MoveTo(xPacket.end_x(), xPacket.end_y());
    //    }

    //    //g_GameNetTerminal->GetAddr()
    //    //if (g_CurrentMainPlayer->in)
    //    //{
    //    //}
    //    //m_TargetPosition = m_TargetPath[m_TargetPath.GetCount() - 1];

    //    //g_CurrentMainPlayer->FindTo()
    //}
    ////g_CurrentMainPlayer
    //// xPacket.role_id = 1;
    //// xPacket.scene_instance_id = 2;
    //// xPacket.begin_x = 3;
    //// xPacket.begin_y = 4;
    ////  = 5;
    //// xPacket.end_y = 6;

    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_RoleInfo)
{
    FillLog(&xPacket, False);

    m_RoleIndex = xPacket.role_index();

    if (!xPacket.has_role_info())
    {
        C2S_Ask_RoleInfo xAsk;
        xAsk.set_account(m_Account);
        xAsk.set_role_index(m_LoginRoleIndex);
        xAsk.set_force_apply_role_info(true);
        this->RobotSendPacket(xAsk);
    }
    else
    {
        C2S_Ask_Ready_EnterScene xAsk;
        xAsk.set_role_index(xPacket.role_index());
        this->RobotSendPacket(xAsk);
    }
    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_RoleSum)
{
    FillLog(&xPacket, False);

    if (xPacket.list_role_index_size() > 0)
    {
        m_LoginRoleIndex = xPacket.list_role_index(0);
        C2S_Ask_RoleInfo xAsk;
        xAsk.set_account(xPacket.account());
        xAsk.set_role_index(m_LoginRoleIndex);
        this->RobotSendPacket(xAsk);
        // 请求登录第一个角色, 进入游戏流程
    }
    else
    {
        mstr xArray[] = {
            "逍遥生","剑侠客","莽壮士","英女侠","俏千金","飞燕女",
            "神天兵","智圣仙","龙战将","舞天姬","玄剑娥","精灵仙",
            "虎头怪","夺命妖","巨魔王","狐美人","骨精灵","小蛮妖"
        };

        Int32 x = R_WORD(0, 18);
        mstr x种族;
        if (x >= 0 && x <= 5) { x种族 = "人族"; }
        if (x >= 6 && x <= 11) { x种族 = "仙族"; }
        if (x >= 12 && x <= 17) { x种族 = "魔族"; }

        mstr x性别 = "女";
        switch (x)
        {
            case 0:
            case 1:
            case 2:
            case 6:
            case 7:
            case 8:
            case 12:
            case 13:
            case 14:
            {
                x性别 = "男";
            }break;
        }
        C2S_Ask_RoleCreate xPacket;
        xPacket.set_account(m_Account);         // 账号
        xPacket.set_nick_name(m_NickName);      // 昵称
        xPacket.set_device_key(m_Account);      // 设备唯一标示
        xPacket.set_device_token(m_Account);    // 设备推送标示
        xPacket.set_device_type(m_Account);     // 设备类型
        xPacket.set_platform_type("PC");        // 平台类型
                                                //xPacket.set_racial(x种族);              // 种族
                                                //xPacket.set_gender(x性别);               // 性别
        xPacket.set_role_type(xArray[x]);         // 角色类型
        this->RobotSendPacket(xPacket);
    }
    return True;
}

//ROBOTPLAYER_AUTO_REGISTER(S2C_SelfHiAct)
//{
//    FillLog(&xPacket, False);
//
//    //if (xPacket.list_role_index_size() > 0)
//    //{
//    //    C2S_RoleInfo xAsk;
//    //    xAsk.set_account_key(xPacket.account());
//    //    xAsk.set_role_index(xPacket.list_role_index(0));
//    //    this->RobotSendPacket(Msg_C2S_RoleInfo, xAsk);
//    //    // 请求登录第一个角色, 进入游戏流程
//    //}
//    //else
//    //{
//    //    C2S_CreateRole xPacket;
//    //    xPacket.set_account_key(m_Account);         // 账号
//    //    xPacket.set_nick_name(m_NickName);      // 昵称
//    //    xPacket.set_device_key(m_Account);      // 设备唯一标示
//    //    xPacket.set_device_token(m_Account);    // 设备推送标示
//    //    xPacket.set_device_type(m_Account);     // 设备类型
//    //    xPacket.set_platform_type("PC");        // 平台类型
//    //    //xPacket.set_racial("仙族");              // 种族
//    //    //xPacket.set_gender("女");               // 性别
//    //    //xPacket.set_role_type("舞天姬");         // 角色类型
//    //    xPacket.set_is_test_vip(true);          // 是否VIP测试
//    //    this->RobotSendPacket(Msg_C2S_CreateRole, xPacket);
//    //}
//    return True;
//}

//ROBOT_PACKET_EXE(S2C_RoleInfo_EnterScene)
//{
//    FillLog(&xPacket, False);
//    if (xPacket.role_index() == m_RoleIndex)
//    {
//        m_RoleInfo.CopyFrom_Load(xPacket.role_info());
//        this->m_State = E_ROBOT_STATE::ERS_READY;
//    }
//    return True;
//}

ROBOT_PACKET_EXE(S2C_Notice_Role_LeaveScene)
{
    FillLog(&xPacket, False);
    return True;
}

ROBOT_PACKET_EXE(S2C_Notice_Role_EnterScene)
{
    FillLog(&xPacket, False);
    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_Register)
{
    FillLog(&xPacket, False);
    ////S2C_Register xPacket; RET_TERMINAL_ANALYTIC_PROTO(xPacket);
    //switch (xPacket.ret_code())
    //{
    //    case Msg_Ret_Code_OK: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "注册成功", "提示", MB_OK); }break;
    //    case Msg_Ret_Code_Username_To_Short: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "用户名错误(长度应为5-100)", "提示", MB_OK); }break;
    //    case Msg_Ret_Code_Username_To_Long: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "用户名错误(长度应为5-100)", "提示", MB_OK); }break;
    //    case Msg_Ret_Code_Username_Exist: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "用户名已存在", "提示", MB_OK); }break;
    //    default: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "注册失败,未知错误!", "提示", MB_OK); }break;
    //}

    return True;
}

ROBOT_PACKET_EXE(S2C_Ret_Ready_EnterScene)
{
    FillLog(&xPacket, False);

    //C2S_EnterScene xAsk;
    //xAsk.set_role_index(xPacket.role_index());
    //xAsk.set_scene_instance_id(xPacket.scene_instance_id());
    //this->RobotSendPacket(xAsk);

    return True;
}

ROBOT_PACKET_EXE(S2C_Notice_OffLine)
{
    FillLog(&xPacket, False);
    return True;
}

//ROBOT_PACKET_EXE(S2C_Mail_Have_New)
//{
//    FillLog(&xPacket, False);
//    return True;
//}
