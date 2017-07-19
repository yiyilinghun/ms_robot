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

    //    //case Msg_Ret_Code_Nickname_Exist: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�ǳ��Ѵ���", "��ʾ", MB_OK); }break;

    //    //case Msg_Ret_Code_CreateRole_Failed:
    //    //default: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "������ɫʧ��", "��ʾ", MB_OK); }break;
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
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�˺Ż��������", "��ʾ", MB_OK);
    //    }break;

    //    case Msg_Ret_Code_Username_To_Short:
    //    case Msg_Ret_Code_Username_To_Long:
    //    case Msg_Ret_Code_Username_Exist:
    //    {
    //        //g_Account = "";
    //        //g_Password = "";
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�û�������", "��ʾ", MB_OK);
    //    }break;

    //    default:
    //    {
    //        //g_Account = "";
    //        //g_Password = "";
    //        //::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "��¼ʧ��,δ֪����!", "��ʾ", MB_OK);
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
    //    // ����������Լ�
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
        // �����¼��һ����ɫ, ������Ϸ����
    }
    else
    {
        mstr xArray[] = {
            "��ң��","������","ç׳ʿ","ӢŮ��","��ǧ��","����Ů",
            "�����","��ʥ��","��ս��","���켧","������","������",
            "��ͷ��","������","��ħ��","������","�Ǿ���","С����"
        };

        Int32 x = R_WORD(0, 18);
        mstr x����;
        if (x >= 0 && x <= 5) { x���� = "����"; }
        if (x >= 6 && x <= 11) { x���� = "����"; }
        if (x >= 12 && x <= 17) { x���� = "ħ��"; }

        mstr x�Ա� = "Ů";
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
                x�Ա� = "��";
            }break;
        }
        C2S_Ask_RoleCreate xPacket;
        xPacket.set_account(m_Account);         // �˺�
        xPacket.set_nick_name(m_NickName);      // �ǳ�
        xPacket.set_device_key(m_Account);      // �豸Ψһ��ʾ
        xPacket.set_device_token(m_Account);    // �豸���ͱ�ʾ
        xPacket.set_device_type(m_Account);     // �豸����
        xPacket.set_platform_type("PC");        // ƽ̨����
                                                //xPacket.set_racial(x����);              // ����
                                                //xPacket.set_gender(x�Ա�);               // �Ա�
        xPacket.set_role_type(xArray[x]);         // ��ɫ����
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
//    //    // �����¼��һ����ɫ, ������Ϸ����
//    //}
//    //else
//    //{
//    //    C2S_CreateRole xPacket;
//    //    xPacket.set_account_key(m_Account);         // �˺�
//    //    xPacket.set_nick_name(m_NickName);      // �ǳ�
//    //    xPacket.set_device_key(m_Account);      // �豸Ψһ��ʾ
//    //    xPacket.set_device_token(m_Account);    // �豸���ͱ�ʾ
//    //    xPacket.set_device_type(m_Account);     // �豸����
//    //    xPacket.set_platform_type("PC");        // ƽ̨����
//    //    //xPacket.set_racial("����");              // ����
//    //    //xPacket.set_gender("Ů");               // �Ա�
//    //    //xPacket.set_role_type("���켧");         // ��ɫ����
//    //    xPacket.set_is_test_vip(true);          // �Ƿ�VIP����
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
    //    case Msg_Ret_Code_OK: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "ע��ɹ�", "��ʾ", MB_OK); }break;
    //    case Msg_Ret_Code_Username_To_Short: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�û�������(����ӦΪ5-100)", "��ʾ", MB_OK); }break;
    //    case Msg_Ret_Code_Username_To_Long: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�û�������(����ӦΪ5-100)", "��ʾ", MB_OK); }break;
    //    case Msg_Ret_Code_Username_Exist: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "�û����Ѵ���", "��ʾ", MB_OK); }break;
    //    default: { ::MessageBoxA(G_HGE->System_GetState(HGE_HWND), "ע��ʧ��,δ֪����!", "��ʾ", MB_OK); }break;
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
