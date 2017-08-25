#pragma once

extern MsList<mstr> g_List;
extern Int32 g_RobotCellNextIndex;

enum DL_Packet_Idx
{
    METHOD_2_OnCreateResult = 2,
    METHOD_96_OnRoomIP = 96,
    METHOD_101_OnRoomDetail = 101,

    INF = 60000,
    CONNECT_START,
    CONNECT_END,
    EXEC_ACK,
    EXEC_RESULT,
    EXEC_EXCEPTION,
    EXEC_BUSSY,
    VERSION,
    VERSION_RESULT,
    LOGIN,
    LOGIN_RESULT,
    LICENCE_UPDATE,
    GAME_START,
    GAME_TIME_UPDATE,
    CREATE_SELF,
    ENTITY_EMERGE,
    ENTITY_VANISH,
    ENTITY_LIST_EMERGE,
    ENTITY_LIST_VANISH,
    ENTITY_PROPERTY_UPDATE_CHANNEL_INF,
    ENTITY_PROPERTY_UPDATE_CHANNEL_SUP = ENTITY_PROPERTY_UPDATE_CHANNEL_INF + 15,
    ENTITY_GM,
    ENTITY_MESSAGE,
    SUP,
};

enum DL_PACK_RET_CODE
{
    OK,
    ERROR_VERSION,
    ERROR_ACCOUNT,
    NEW_LOGIN,
    WAITING,
    INDULGE,
    DISABLE_ACCOUNT,
    DISABLE_IP,
    EMPTY_PASSWD,
    REGISTER_OK,
    RECONNECT_FAILED,
    TOKEN_INVALID,		//token不一致
    SAME_PKT,			//检测到相同数据包
    SAME_ENTITY_ID,		//相同的ID
    DISABLE_ACCOUNT_FORCE,	//封号强制玩家下线
    RECONNECT_SUCCESS,		//断线重连成功

    //SDK错误提示
    THIRD_CHANNEL_MKSDK_ERROR_ACCOUNT, //MISDK登陆验证错误
};

enum DL_LoginType
{
    STRAT,
    LOGIN_REGISTER,   // 注册
    LOGIN_ENGERGAME,  // 登录
    LOGIN_TOURISTS,   // 游客登录
    LOGIN_IN,		  // 按钮登录
    LOGIN_RECONNECT,  // 断线重连
    LOGIN_REGISGER_MISDK, //小米SDK
    END,
};

enum AccountClientCellMethod
{
    METHOD_0_RandomName = 0,
    METHOD_1_CreateRole = 1,

};

enum PlayerClientRoomMethod
{
    METHOD_0_Ping = 0,
    METHOD_2_SpaceLoadFinish = 2,
    METHOD_3_FireSkill = 3,
};

enum PlayerClientCellMethod
{
    METHOD_11_SetTask = 11,
    METHOD_14_SetVip = 14,
    METHOD_19_IAmGod = 19,
    METHOD_38_SyncPos = 38,
    METHOD_39_GotoSpace = 39,
    METHOD_40_SpaceLoadFinish = 40,
    //METHOD_79_SelectPvpTargets = 79,
    METHOD_148_StartGVEMatch = 148,
    METHOD_152_ChangeRoomState = 152,
    METHOD_153_ChangeRoomHero = 153,
    METHOD_160_PlayerRequestSkyMonterMessage = 160,

};


struct MsgStruct
{
    Int32 ID;
    mstr sBuffer;
    Double fTime;
};

class RobotCellPlayer;
class RobotDoc;
class RobotView;
class NetRobot : public MsNetClient
{
public:
    NetRobot(RobotView* xRobotView) :m_RobotView(xRobotView) { ; }
    RobotView* m_RobotView;
protected:
    virtual MsNetTerminal* GetNewNetTerminal(MsIOCPManager* xMsIOCPManager);
};

class CMFC_RobotCntrItem;

class RobotView : public CView
{
protected: // 仅从序列化创建
    RobotView();
    DECLARE_DYNCREATE(RobotView)

    // 特性
public:
    RobotDoc* GetDocument() const;
    // m_pSelection 将所选内容保存在当前的 CMFC_RobotCntrItem 中。
    // 对于许多应用程序，这种成员变量不足以
    //  表示某个选择，例如在不属于 CMFC_RobotCntrItem 的对象中
    //  选定的一个或多个对象。  提供这种选择
    //  机制的目的只是帮助您入门

    // TODO: 用适合应用程序的选择机制替换此选择机制
    CMFC_RobotCntrItem* m_pSelection;

    // 操作
public:

    // 重写
public:
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void OnInitialUpdate(); // 构造后第一次调用
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual BOOL IsSelected(const CObject* pDocItem) const;// 容器支持

// 实现
public:
    virtual ~RobotView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    Boolean AddNewRobot(wstr xAddr, WORD xPort);
    void CreateRobot(Int32 xRobotCount);

    Boolean m_IsActivated;
    MsList<RobotCellPlayer*> m_ListRobot;
    NetRobot m_NetRobot;

//protected:

    MsThreadIOCPManager m_MsThreadIOCPManager;

    CBrush m_RedBrush;
    CBrush m_YellowBrush;
    CBrush m_GreenBrush;
    CPen m_Pen;

    //CImage m_RedImage;
    //CImage m_YellowImage;
    //CImage m_GreenImage;

    Boolean m_IsRegister = False;
    Boolean m_IsLogin = False;
    Boolean m_IsMoving = False;


    Int32 m_SelectRobotUnitIndex;

    Int64 m_LastMoveTime;
    Boolean m_IsMove;

    void ShowRobotLog();
    // 生成的消息映射函数
//protected:
    afx_msg void OnDestroy();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnInsertObject();
    afx_msg void OnCancelEditCntr();
    afx_msg void OnFilePrint();
    afx_msg void OnFilePrintPreview();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    afx_msg void OnCreateRobot1();
    afx_msg void OnCreateRobot100();
    afx_msg void OnCreateRobot2();
    afx_msg void OnCreateRobot5();
    afx_msg void OnCreateRobot10();
    afx_msg void OnCreateRobot50();
    afx_msg void OnCreateRobot500();
    afx_msg void OnCreateRobot1000();
    //afx_msg void OnRobotTimer();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEdit_C2S_Register();
    afx_msg void OnEdit_C2S_Login();
    afx_msg void OnEdit_C2S_CreateRole();
    afx_msg void OnEdit_C2S_EnterScene();
    afx_msg void OnEdit_GM_IAmGod();
    afx_msg void OnEdit_Go_SkyMonter();

    afx_msg void OnRobotMove();
    afx_msg void OnRobotChangeScene();
};

#ifndef _DEBUG  // MFC_RobotView.cpp 中的调试版本
inline RobotDoc* RobotView::GetDocument() const
{
    return reinterpret_cast<RobotDoc*>(m_pDocument);
}
#endif

