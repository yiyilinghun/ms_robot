#include "Precompiled.h"
#include "mainfrm.h"
#include "RobotList.h"
#include "Resource.h"
#include "RobotApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif

/////////////////////////////////////////////////////////////////////////////
// RobotList

RobotList::RobotList()
{
}

RobotList::~RobotList()
{
}

BEGIN_MESSAGE_MAP(RobotList, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_PROPERTIES, OnProperties)
    ON_COMMAND(ID_OPEN, OnFileOpen)
    ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
    ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_CREATE_ROBOT_1, &RobotList::OnCreateRobot1)
    ON_COMMAND(ID_CREATE_ROBOT_2, &RobotList::OnCreateRobot2)
    ON_COMMAND(ID_CREATE_ROBOT_5, &RobotList::OnCreateRobot5)
    ON_COMMAND(ID_CREATE_ROBOT_10, &RobotList::OnCreateRobot10)
    ON_COMMAND(ID_CREATE_ROBOT_50, &RobotList::OnCreateRobot50)
    ON_COMMAND(ID_CREATE_ROBOT_100, &RobotList::OnCreateRobot100)
    ON_COMMAND(ID_CREATE_ROBOT_500, &RobotList::OnCreateRobot500)
    ON_COMMAND(ID_CREATE_ROBOT_1000, &RobotList::OnCreateRobot1000)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int RobotList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建视图: 
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

    if (!m_RobotListViewTree.Create(dwViewStyle, rectDummy, this, 4))
    {
        TRACE0("未能创建文件视图\n");
        return -1;      // 未能创建
    }

    // 加载视图图像: 
    m_RobotListImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
    m_RobotListViewTree.SetImageList(&m_RobotListImages, TVSIL_NORMAL);

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
    m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

    OnChangeVisualStyle();

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由: 
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    // 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
    FillFileView();
    AdjustLayout();

    return 0;
}

void RobotList::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void RobotList::FillFileView()
{
    //HTREEITEM hRoot = m_RobotListViewTree.InsertItem(_T("FakeApp 文件"), 0, 0);
    //m_RobotListViewTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

    //HTREEITEM hSrc = m_RobotListViewTree.InsertItem(_T("FakeApp 源文件"), 0, 0, hRoot);

    //m_RobotListViewTree.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
    //m_RobotListViewTree.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
    //m_RobotListViewTree.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
    //m_RobotListViewTree.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
    //m_RobotListViewTree.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
    //m_RobotListViewTree.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

    //HTREEITEM hInc = m_RobotListViewTree.InsertItem(_T("FakeApp 头文件"), 0, 0, hRoot);

    //m_RobotListViewTree.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
    //m_RobotListViewTree.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
    //m_RobotListViewTree.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
    //m_RobotListViewTree.InsertItem(_T("Resource.h"), 2, 2, hInc);
    //m_RobotListViewTree.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
    //m_RobotListViewTree.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

    //HTREEITEM hRes = m_RobotListViewTree.InsertItem(_T("FakeApp 资源文件"), 0, 0, hRoot);

    //m_RobotListViewTree.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
    //m_RobotListViewTree.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
    //m_RobotListViewTree.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
    //m_RobotListViewTree.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

    //m_RobotListViewTree.Expand(hRoot, TVE_EXPAND);
    //m_RobotListViewTree.Expand(hSrc, TVE_EXPAND);
    //m_RobotListViewTree.Expand(hInc, TVE_EXPAND);
}

void RobotList::OnContextMenu(CWnd* pWnd, CPoint point)
{
    //CTreeCtrl* pWndTree = (CTreeCtrl*)&m_RobotListViewTree;
    //ASSERT_VALID(pWndTree);

    //if (pWnd != pWndTree)
    //{
    //    CDockablePane::OnContextMenu(pWnd, point);
    //    return;
    //}

    //if (point != CPoint(-1, -1))
    //{
    //    // 选择已单击的项: 
    //    CPoint ptTree = point;
    //    pWndTree->ScreenToClient(&ptTree);

    //    UINT flags = 0;
    //    HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
    //    if (hTreeItem != nullptr)
    //    {
    //        pWndTree->SelectItem(hTreeItem);
    //    }
    //}

    //pWndTree->SetFocus();
    //theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void RobotList::AdjustLayout()
{
    if (GetSafeHwnd() == nullptr)
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_RobotListViewTree.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void RobotList::OnProperties()
{
    AfxMessageBox(_T("属性...."));

}

void RobotList::OnFileOpen()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnFileOpenWith()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnDummyCompile()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnEditCut()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnEditCopy()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnEditClear()
{
    // TODO: 在此处添加命令处理程序代码
}

void RobotList::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文

    CRect rectTree;
    m_RobotListViewTree.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void RobotList::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);

    m_RobotListViewTree.SetFocus();
}

void RobotList::OnChangeVisualStyle()
{
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

    m_RobotListImages.DeleteImageList();

    UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

    CBitmap bmp;
    if (!bmp.LoadBitmap(uiBmpId))
    {
        TRACE(_T("无法加载位图: %x\n"), uiBmpId);
        ASSERT(FALSE);
        return;
    }

    BITMAP bmpObj;
    bmp.GetBitmap(&bmpObj);

    UINT nFlags = ILC_MASK;

    nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

    m_RobotListImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
    m_RobotListImages.Add(&bmp, RGB(255, 0, 255));

    m_RobotListViewTree.SetImageList(&m_RobotListImages, TVSIL_NORMAL);
}

void RobotList::CreateRobot(Int32 xRobotCount)
{
    CMDIFrameWndEx* xCMDIFrameWndEx = (CMDIFrameWndEx*)AfxGetMainWnd();
    if (xCMDIFrameWndEx)
    {
        CMDIChildWnd* xCMDIChildWnd = (CMDIChildWnd*)xCMDIFrameWndEx->GetActiveFrame();
        if (xCMDIChildWnd)
        {
            RobotView* xRobotView = (RobotView*)xCMDIChildWnd->GetActiveView();
            if (xRobotView)
            {
                for (Int32 i = 0; i < xRobotCount; i++)
                {
                    //if (!xRobotView->AddNewRobot(L"121.40.89.124", 10024))

                    if (!xRobotView->AddNewRobot(L"172.168.100.12", 40001 + R_WORD(0, 20))) {
                        break;
                    }
                    //if (!xRobotView->AddNewRobot(L"172.168.100.12", 40001))

                }
            }
        }
    }


    //POSITION xDocTemplatePosition = theApp.m_pDocManager->GetFirstDocTemplatePosition();
    //CDocTemplate* xDocTemplate = theApp.m_pDocManager->GetNextDocTemplate(xDocTemplatePosition);
    //if (xDocTemplate)
    //{
    //    POSITION xDocPosition = xDocTemplate->GetFirstDocPosition();
    //    if (xDocPosition)
    //    {
    //        RobotDoc* xDoc = (RobotDoc*)xDocTemplate->GetNextDoc(xDocPosition);
    //        POSITION xViewPosition = xDoc->GetFirstViewPosition();
    //        while (xViewPosition)
    //        {
    //            RobotView* xView = (RobotView*)xDoc->GetNextView(xViewPosition);
    //            if (xView->m_IsActivated)
    //            {
    //                for (Int32 i = 0; i < xRobotCount; i++)
    //                {

    //                    if (!xView->AddNewRobot(L"127.0.0.1", 40001))
    //                    {
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}

void RobotList::OnCreateRobot1()
{
    this->CreateRobot(1);
}

void RobotList::OnCreateRobot2()
{
    this->CreateRobot(2);
}


void RobotList::OnCreateRobot5()
{
    this->CreateRobot(5);
}


void RobotList::OnCreateRobot10()
{
    this->CreateRobot(10);
}


void RobotList::OnCreateRobot50()
{
    this->CreateRobot(50);
}

void RobotList::OnCreateRobot100()
{
    this->CreateRobot(100);
}


void RobotList::OnCreateRobot500()
{
    this->CreateRobot(500);
}


void RobotList::OnCreateRobot1000()
{
    this->CreateRobot(1000);
}
