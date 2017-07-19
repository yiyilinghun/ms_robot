
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "Precompiled.h"
#include "RobotApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
    ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
    ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // ״̬��ָʾ��
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
    // TODO: �ڴ���ӳ�Ա��ʼ������
    theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    BOOL bNameValid;

    CMDITabInfo mdiTabParams;
    mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
    mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
    mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
    mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
    mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
    EnableMDITabbedGroups(TRUE, mdiTabParams);

    if (!m_wndMenuBar.Create(this))
    {
        TRACE0("δ�ܴ����˵���\n");
        return -1;      // δ�ܴ���
    }

    m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

    // ��ֹ�˵����ڼ���ʱ��ý���
    CMFCPopupMenu::SetForceMenuFocus(FALSE);

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
    {
        TRACE0("δ�ܴ���������\n");
        return -1;      // δ�ܴ���
    }

    CString strToolBarName;
    bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
    ASSERT(bNameValid);
    m_wndToolBar.SetWindowText(strToolBarName);

    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);
    m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

    // �����û�����Ĺ���������: 
    InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("δ�ܴ���״̬��\n");
        return -1;      // δ�ܴ���
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

    // TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndMenuBar);
    DockPane(&m_wndToolBar);


    // ���� Visual Studio 2005 ��ʽͣ��������Ϊ
    CDockingManager::SetDockingMode(DT_SMART);
    // ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
    EnableAutoHidePanes(CBRS_ALIGN_ANY);

    // ���ز˵���ͼ��(�����κα�׼��������): 
    CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

    // ����ͣ������
    if (!CreateDockingWindows())
    {
        TRACE0("δ�ܴ���ͣ������\n");
        return -1;
    }

    m_RobotListViewTree.EnableDocking(CBRS_ALIGN_ANY);
    m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_RobotListViewTree);
    CDockablePane* pTabbedBar = nullptr;
    m_wndClassView.AttachToTabWnd(&m_RobotListViewTree, DM_SHOW, TRUE, &pTabbedBar);
    m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndOutput);
    m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndProperties);

    // ���ڳ־�ֵ�����Ӿ�����������ʽ
    OnApplicationLook(theApp.m_nAppLook);

    // ������ǿ�Ĵ��ڹ���Ի���
    EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

    // ���ù�������ͣ�����ڲ˵��滻
    EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

    // ���ÿ���(��ס Alt �϶�)�������Զ���
    CMFCToolBar::EnableQuickCustomization();

    if (CMFCToolBar::GetUserImages() == nullptr)
    {
        // �����û�����Ĺ�����ͼ��
        if (m_UserImages.Load(_T(".\\UserImages.bmp")))
        {
            CMFCToolBar::SetUserImages(&m_UserImages);
        }
    }

    // ���ò˵����Ի�(���ʹ�õ�����)
    // TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
    CList<UINT, UINT> lstBasicCommands;

    lstBasicCommands.AddTail(ID_FILE_NEW);
    lstBasicCommands.AddTail(ID_FILE_OPEN);
    lstBasicCommands.AddTail(ID_FILE_SAVE);
    lstBasicCommands.AddTail(ID_FILE_PRINT);
    lstBasicCommands.AddTail(ID_APP_EXIT);
    lstBasicCommands.AddTail(ID_EDIT_CUT);
    lstBasicCommands.AddTail(ID_EDIT_PASTE);
    lstBasicCommands.AddTail(ID_EDIT_UNDO);
    lstBasicCommands.AddTail(ID_APP_ABOUT);
    lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
    lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
    lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
    lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
    lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
    lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
    lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
    lstBasicCommands.AddTail(ID_OLE_INSERT_NEW);

    CMFCToolBar::SetBasicCommands(lstBasicCommands);

    // ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
    // ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
    ModifyStyle(0, FWS_PREFIXTITLE);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CMDIFrameWndEx::PreCreateWindow(cs))
        return FALSE;
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
    BOOL bNameValid;

    // ��������ͼ
    CString strClassView;
    bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
    ASSERT(bNameValid);
    if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ���������ͼ������\n");
        return FALSE; // δ�ܴ���
    }

    // �����ļ���ͼ
    CString strFileView;
    bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
    ASSERT(bNameValid);
    if (!m_RobotListViewTree.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ������ļ���ͼ������\n");
        return FALSE; // δ�ܴ���
    }

    // �����������
    CString strOutputWnd;
    bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
    ASSERT(bNameValid);
    if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ����������\n");
        return FALSE; // δ�ܴ���
    }

    // �������Դ���
    CString strPropertiesWnd;
    bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
    ASSERT(bNameValid);
    if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ��������ԡ�����\n");
        return FALSE; // δ�ܴ���
    }

    SetDockingWindowIcons(theApp.m_bHiColorIcons);
    return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
    HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_RobotListViewTree.SetIcon(hFileViewIcon, FALSE);

    HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndClassView.SetIcon(hClassViewIcon, FALSE);

    HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

    HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

    UpdateMDITabbedBarsIcons();
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
    ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
    CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
    pDlgCust->EnableUserDefinedToolbars();
    pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
    LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
    if (lres == 0)
    {
        return 0;
    }

    CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
    ASSERT_VALID(pUserToolbar);

    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
    return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
    CWaitCursor wait;

    theApp.m_nAppLook = id;

    switch (theApp.m_nAppLook)
    {
        case ID_VIEW_APPLOOK_WIN_2000:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
            break;

        case ID_VIEW_APPLOOK_OFF_XP:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
            break;

        case ID_VIEW_APPLOOK_WIN_XP:
            CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
            break;

        case ID_VIEW_APPLOOK_OFF_2003:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
            CDockingManager::SetDockingMode(DT_SMART);
            break;

        case ID_VIEW_APPLOOK_VS_2005:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
            CDockingManager::SetDockingMode(DT_SMART);
            break;

        case ID_VIEW_APPLOOK_VS_2008:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
            CDockingManager::SetDockingMode(DT_SMART);
            break;

        case ID_VIEW_APPLOOK_WINDOWS_7:
            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
            CDockingManager::SetDockingMode(DT_SMART);
            break;

        default:
            switch (theApp.m_nAppLook)
            {
                case ID_VIEW_APPLOOK_OFF_2007_BLUE:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_BLACK:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_SILVER:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
                    break;

                case ID_VIEW_APPLOOK_OFF_2007_AQUA:
                    CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
                    break;
            }

            CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
            CDockingManager::SetDockingMode(DT_SMART);
    }

    m_wndOutput.UpdateFonts();
    RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

    theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
    // ���ཫִ�������Ĺ���

    if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
    {
        return FALSE;
    }

    g_FaceId.Add(0);
    g_FaceId.Add(1);
    g_FaceId.Add(2);
    g_FaceId.Add(3);
    g_FaceId.Add(4);
    g_FaceId.Add(5);
    g_FaceId.Add(6);
    g_FaceId.Add(7);
    g_FaceId.Add(8);
    g_FaceId.Add(9);
    g_FaceId.Add(10);
    g_FaceId.Add(11);
    g_FaceId.Add(12);
    g_FaceId.Add(13);
    g_FaceId.Add(14);
    g_FaceId.Add(15);
    g_FaceId.Add(16);
    g_FaceId.Add(17);
    g_FaceId.Add(18);
    g_FaceId.Add(19);
    g_FaceId.Add(20);
    g_FaceId.Add(21);
    g_FaceId.Add(22);
    g_FaceId.Add(23);
    g_FaceId.Add(24);
    g_FaceId.Add(25);
    g_FaceId.Add(26);
    g_FaceId.Add(27);
    g_FaceId.Add(28);
    g_FaceId.Add(29);
    g_FaceId.Add(30);
    g_FaceId.Add(31);
    g_FaceId.Add(32);
    g_FaceId.Add(33);
    g_FaceId.Add(34);
    g_FaceId.Add(35);
    g_FaceId.Add(36);
    g_FaceId.Add(37);
    g_FaceId.Add(38);
    g_FaceId.Add(39);
    g_FaceId.Add(40);
    g_FaceId.Add(41);
    g_FaceId.Add(42);
    g_FaceId.Add(43);
    g_FaceId.Add(44);
    g_FaceId.Add(45);
    g_FaceId.Add(46);
    g_FaceId.Add(47);
    g_FaceId.Add(48);
    g_FaceId.Add(49);
    g_FaceId.Add(50);
    g_FaceId.Add(51);
    g_FaceId.Add(52);
    g_FaceId.Add(53);
    g_FaceId.Add(54);
    g_FaceId.Add(55);
    g_FaceId.Add(56);
    g_FaceId.Add(57);
    g_FaceId.Add(58);
    g_FaceId.Add(59);
    g_FaceId.Add(60);
    g_FaceId.Add(61);
    g_FaceId.Add(62);
    g_FaceId.Add(63);
    g_FaceId.Add(74);
    g_FaceId.Add(75);
    g_FaceId.Add(76);
    g_FaceId.Add(77);
    g_FaceId.Add(78);
    g_FaceId.Add(79);
    g_FaceId.Add(80);
    g_FaceId.Add(81);
    g_FaceId.Add(82);
    g_FaceId.Add(83);
    g_FaceId.Add(84);
    g_FaceId.Add(85);
    g_FaceId.Add(86);
    g_FaceId.Add(87);
    g_FaceId.Add(88);
    g_FaceId.Add(89);
    g_FaceId.Add(90);
    g_FaceId.Add(91);
    g_FaceId.Add(92);
    g_FaceId.Add(93);
    g_FaceId.Add(94);
    g_FaceId.Add(95);
    g_FaceId.Add(96);
    g_FaceId.Add(97);
    g_FaceId.Add(98);
    g_FaceId.Add(99);
    g_FaceId.Add(101);
    g_FaceId.Add(102);
    g_FaceId.Add(103);
    g_FaceId.Add(104);
    g_FaceId.Add(105);
    g_FaceId.Add(106);
    g_FaceId.Add(107);
    g_FaceId.Add(108);
    g_FaceId.Add(109);
    g_FaceId.Add(110);
    g_FaceId.Add(111);
    g_FaceId.Add(112);
    g_FaceId.Add(113);
    g_FaceId.Add(117);
    g_FaceId.Add(118);
    g_FaceId.Add(119);
    g_FaceId.Add(120);
    g_FaceId.Add(121);
    g_FaceId.Add(122);
    g_FaceId.Add(123);
    g_FaceId.Add(124);
    g_FaceId.Add(125);
    g_FaceId.Add(126);
    g_FaceId.Add(127);
    g_FaceId.Add(128);
    g_FaceId.Add(129);
    g_FaceId.Add(130);
    g_FaceId.Add(131);
    g_FaceId.Add(132);
    g_FaceId.Add(133);
    g_FaceId.Add(134);
    g_FaceId.Add(135);
    g_FaceId.Add(136);
    g_FaceId.Add(137);
    g_FaceId.Add(138);
    g_FaceId.Add(139);
    g_FaceId.Add(140);
    g_FaceId.Add(141);
    g_FaceId.Add(142);
    g_FaceId.Add(143);
    g_FaceId.Add(144);
    g_FaceId.Add(145);
    g_FaceId.Add(146);
    g_FaceId.Add(147);
    g_FaceId.Add(148);
    g_FaceId.Add(149);
    g_FaceId.Add(150);
    g_FaceId.Add(151);
    g_FaceId.Add(152);
    g_FaceId.Add(153);
    g_FaceId.Add(184);
    g_FaceId.Add(185);
    g_FaceId.Add(186);
    g_FaceId.Add(187);
    g_FaceId.Add(188);
    g_FaceId.Add(189);
    g_FaceId.Add(190);
    g_FaceId.Add(191);
    g_FaceId.Add(192);
    g_FaceId.Add(193);
    g_FaceId.Add(194);
    g_FaceId.Add(195);
    g_FaceId.Add(196);
    g_FaceId.Add(197);
    g_FaceId.Add(198);
    g_FaceId.Add(199);
    g_FaceId.Add(200);
    g_FaceId.Add(201);
    g_FaceId.Add(202);
    g_FaceId.Add(203);
    g_FaceId.Add(204);
    g_FaceId.Add(205);
    g_FaceId.Add(206);
    g_FaceId.Add(207);
    g_FaceId.Add(208);
    g_FaceId.Add(209);
    g_FaceId.Add(210);
    g_FaceId.Add(211);

    // Ϊ�����û������������Զ��尴ť
    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    for (int i = 0; i < iMaxUserToolbars; i++)
    {
        CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
        if (pUserToolbar != nullptr)
        {
            pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
        }
    }

    return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
    m_wndOutput.UpdateFonts();
}
