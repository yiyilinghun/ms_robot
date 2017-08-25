
// MFC_RobotView.cpp : RobotView 类的实现
//

#include "Precompiled.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS

#endif

#include "CntrItem.h"
#include "resource.h"
#include "RobotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


MsList<MsgStruct> g_CommandList;
MsList<Int32> g_FaceId;
// RobotView

IMPLEMENT_DYNCREATE(RobotView, CView)

BEGIN_MESSAGE_MAP(RobotView, CView)
    ON_WM_DESTROY()
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
    ON_COMMAND(ID_OLE_INSERT_NEW, &RobotView::OnInsertObject)
    ON_COMMAND(ID_CANCEL_EDIT_CNTR, &RobotView::OnCancelEditCntr)
    ON_COMMAND(ID_FILE_PRINT, &RobotView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &RobotView::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_COMMAND(ID_EDIT_C2S_REGISTER, &RobotView::OnEdit_C2S_Register)
    ON_COMMAND(ID_EDIT_C2S_LOGIN, &RobotView::OnEdit_C2S_Login)
    ON_COMMAND(ID_ROBOT_CREATE_ROLE, &RobotView::OnEdit_C2S_CreateRole)
    ON_COMMAND(ID_ROBOT_ENTER_SCENE, &RobotView::OnEdit_C2S_EnterScene)
    ON_COMMAND(ID_GM_I_AM_GOD, &RobotView::OnEdit_GM_IAmGod)
    ON_COMMAND(ID_ENTER_SKY_MONTER, &RobotView::OnEdit_Go_SkyMonter)

    ON_COMMAND(ID_ROBOT_MOVE, &RobotView::OnRobotMove)
    ON_COMMAND(ID_CREATE_ROBOT_1, &RobotView::OnCreateRobot1)
    ON_COMMAND(ID_CREATE_ROBOT_2, &RobotView::OnCreateRobot2)
    ON_COMMAND(ID_CREATE_ROBOT_5, &RobotView::OnCreateRobot5)
    ON_COMMAND(ID_CREATE_ROBOT_10, &RobotView::OnCreateRobot10)
    ON_COMMAND(ID_CREATE_ROBOT_50, &RobotView::OnCreateRobot50)
    ON_COMMAND(ID_CREATE_ROBOT_100, &RobotView::OnCreateRobot100)
    ON_COMMAND(ID_CREATE_ROBOT_500, &RobotView::OnCreateRobot500)
    ON_COMMAND(ID_CREATE_ROBOT_1000, &RobotView::OnCreateRobot1000)
    ON_COMMAND(ID_ROBOT_CHANGE_SCENE, &RobotView::OnRobotChangeScene)

    //ON_WM_TIMER()

END_MESSAGE_MAP()


void LoadImageFromZip(CImage& xImage, LPCSTR lpZipFileName, LPCSTR lpResName, LPCSTR lpPassword)
{
    ZIP_RES_INFO xResInfo;
    if (ZipUtils::ExtractZipToMemoryByName(lpZipFileName, lpResName, xResInfo, lpPassword))
    {
        HGLOBAL hMemImage = GlobalAlloc(GMEM_MOVEABLE, 0);
        if (hMemImage)
        {
            IStream* pStmImage = nullptr;
            CreateStreamOnHGlobal(hMemImage, TRUE, &pStmImage);
            if (pStmImage)
            {
                ULONG xWriteLen = 0;
                pStmImage->Write(xResInfo.m_DataAddr, xResInfo.m_DataLen, &xWriteLen);
                xImage.Load(pStmImage);
            }
        }
    }
}

MsNetTerminal* NetRobot::GetNewNetTerminal(MsIOCPManager* xMsIOCPManager)
{
    return new RobotCellPlayer(xMsIOCPManager, m_RobotView, g_CommandList);
}

RobotView* g_RobotView = nullptr;
MsList<mstr> g_List;
Int32 g_RobotCellNextIndex;

void CALLBACK Timer_CreateRobot(HWND hWnd, UINT xId, UINT_PTR, DWORD)
{
    if (g_RobotView)
    {
        static bool s_is = false;
        if (!s_is)
        {
            g_RobotView->OnCreateRobot500();
        }
        s_is = true;
    }
}

void CALLBACK Timer_Login(HWND hWnd, UINT xId, UINT_PTR, DWORD)
{
    if (g_RobotView)
    {
        static bool s_is = false;
        if (!s_is)
        {
            g_RobotView->OnEdit_C2S_Login();
        }
        s_is = true;
    }
}

void CALLBACK Timer_SkyMonter(HWND hWnd, UINT xId, UINT_PTR, DWORD)
{
    if (g_RobotView)
    {
        static bool s_is = false;
        if (!s_is)
        {
            g_RobotView->OnEdit_Go_SkyMonter();
        }
        s_is = true;
    }
}

void CALLBACK Timer_StopRobot(HWND hWnd, UINT xId, UINT_PTR, DWORD)
{
    exit(0);
}


RobotView::RobotView()
    : m_MsThreadIOCPManager("RobotThread", 5, True)
    , m_IsLogin(False)
    , m_IsMove(False)
    , m_NetRobot(this)
    , m_RedBrush(0x00FF0000)
    , m_YellowBrush(0x00FFFF00)
    , m_GreenBrush(0x0000FF00)
    , m_Pen(PS_SOLID, 1, (COLORREF)0x0000000)
{
    m_pSelection = nullptr;
    m_IsActivated = False;
    m_SelectRobotUnitIndex = INVALID_NID;
    m_LastMoveTime = MsBase::Now();
    // TODO: 在此处添加构造代码

    MsVector<DWORD> xList1; xList1.Add(3000000000); xList1.Add(577919341);
    MsVector<DWORD> xList2; xList2.Add(0x1730d4d0); xList2.Add(10);
    MsVector<DWORD> xList3; xList3.Add(0xeeff2000); xList3.Add(0xf);
    MsVector<DWORD> xList4; xList4.Add(0xc9103900); xList4.Add(0x79);

    g_RobotView = this;

    ifstream xFileIn("server.txt", ios_base::in);
    char szBuff[200];
    xFileIn.getline(szBuff, 200);
    MsBase::StringSplit(szBuff, ':', g_List);
    xFileIn.close();

    MsBase::stoi(g_List[2].c_str(), g_RobotCellNextIndex);

    //::SetTimer(m_hWnd, 100, 1000, Timer_CreateRobot);
    //::SetTimer(m_hWnd, 200, 1000 + 10000, Timer_Login);
    //::SetTimer(m_hWnd, 300, 1000 + 10000 + 10000, Timer_SkyMonter);
    //::SetTimer(m_hWnd, 300, 1000 + 10000 + 10000 + 600000, Timer_StopRobot);



    //::SetTimer(m_hWnd, 100, 10, Timer_CreateRobot);
    //::SetTimer(m_hWnd, 200, 10 + 300, Timer_Login);
    //::SetTimer(m_hWnd, 300, 10 + 300 + 300, Timer_SkyMonter);
    //::SetTimer(m_hWnd, 300, 10 + 300 + 300 + 600, Timer_StopRobot);

    //LoadImageFromZip(m_RedImage, "..\\..\\res.zip", "res/Robot红框.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());
    //LoadImageFromZip(m_YellowImage, "..\\..\\res.zip", "res/Robot黄框.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());
    //LoadImageFromZip(m_GreenImage, "..\\..\\res.zip", "res/Robot绿框.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());

    //mstr xRegistryPath = "SOFTWARE\\";
    //xRegistryPath += PJ_CLIENT_REGISTRY_FIELD_NAME;
    //mstr xBaseGamePath = MsRegistry::GetRegistryStringA(HKEY_CURRENT_USER, xRegistryPath.c_str(), "BASEDIR");

    //m_RedImage.Load(MsBase::M2W(xBaseGamePath + "\\Robot红框.png").c_str());
    //.Load(MsBase::M2W(xBaseGamePath + "\\Robot黄框.png").c_str());
    //m_GreenImage.Load(MsBase::M2W(xBaseGamePath + "\\Robot绿框.png").c_str());
}

RobotView::~RobotView()
{
    ::KillTimer(m_hWnd, 100);

    m_MsThreadIOCPManager.Stop();
    m_MsThreadIOCPManager.WaitThread();

    FAST_FOREACH(m_ListRobot)
    {
        RobotCellPlayer* xRobotPlayer1 = m_ListRobot.Value();
        RobotCellPlayer* xRobotPlayer2 = m_ListRobot.Value();
        xRobotPlayer1->Close();
        SAFE_DETACH(xRobotPlayer1);
        SAFE_DELETE(xRobotPlayer2);
    }
    m_ListRobot.Clear();
}

BOOL RobotView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// RobotView 绘制

void RobotView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    if (bActivate)
    {
        if (pActivateView == this)
        {
            m_IsActivated = True;
        }
    }
    else
    {
        m_IsActivated = False;
    }

    return CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void RobotView::OnDraw(CDC* pDC)
{
    if (!pDC) { return; }

    RobotDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    // TODO: 同时绘制文档中的所有 OLE 项

    // 在任意位置绘制选定项。  一旦
    //  实现了真正的绘制代码后，应移除此代码。  此位置
    //  与 CMFC_RobotCntrItem 返回的矩形完全对应，
    //  从而产生就地编辑的效果。

    // TODO: 最终绘制代码完成后移除此代码。
    //CString xText = L"123456";
    //RECT rc = { 100,100,200,200 };
    //pDC->DrawText(xText, &rc, 0);

    CRect xTempClientRect;
    this->GetClientRect(&xTempClientRect);
    Int32 xMaxWidth = xTempClientRect.Width();
    Int32 xMaxHeight = xTempClientRect.Height();

    Int32 xUnitWidth = 20;
    Int32 xUnitHeight = 20;

    Int32 xTempIndex = 0;
    FAST_FOREACH(m_ListRobot)
    {
        Int32 X = xTempIndex % (xMaxWidth / xUnitWidth);
        Int32 Y = xTempIndex / (xMaxWidth / xUnitWidth);
        RECT xTemp = { X * xUnitWidth, Y * xUnitHeight, X * xUnitWidth + xUnitWidth, Y * xUnitHeight + xUnitHeight };
        switch (m_ListRobot.Value()->GetState())
        {
            case E_ROBOT_STATE::ERS_READY: { pDC->FillRect(&xTemp, &m_GreenBrush); pDC->Draw3dRect(&xTemp, 0, 0); }break;
            case E_ROBOT_STATE::ERS_WAIT: { pDC->FillRect(&xTemp, &m_YellowBrush); pDC->Draw3dRect(&xTemp, 0, 0); }break;
            case E_ROBOT_STATE::ERS_WARNING: { pDC->FillRect(&xTemp, &m_YellowBrush); pDC->Draw3dRect(&xTemp, 0, 0); }break;
            case E_ROBOT_STATE::ERS_STOP:
            case E_ROBOT_STATE::ERS_INVALID: { pDC->FillRect(&xTemp, &m_RedBrush); pDC->Draw3dRect(&xTemp, 0, 0); }break;
        }
        xTempIndex++;
    }

    if (m_SelectRobotUnitIndex != INVALID_LID)
    {
        Int32 X = m_SelectRobotUnitIndex % (xMaxWidth / xUnitWidth);
        Int32 Y = m_SelectRobotUnitIndex / (xMaxWidth / xUnitWidth);
        CRect xTempRect(X * xUnitWidth, Y * xUnitHeight, (X * xUnitWidth) + 20, (Y * xUnitHeight) + 20);
        this->GetDC()->DrawFocusRect(&xTempRect);
    }

    if (m_pSelection != nullptr)
    {
        CSize size;
        CRect rect(10, 10, 210, 210);

        if (m_pSelection->GetExtent(&size, m_pSelection->m_nDrawAspect))
        {
            pDC->HIMETRICtoLP(&size);
            rect.right = size.cx + 10;
            rect.bottom = size.cy + 10;
        }
        m_pSelection->Draw(pDC, rect);
    }
}

void RobotView::OnInitialUpdate()
{
    CView::OnInitialUpdate();


    // TODO: 写入最终选择模式代码之后移除此代码
    m_pSelection = nullptr;    // 初始化所选内容

}


// RobotView 打印


void RobotView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL RobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // 默认准备
    return DoPreparePrinting(pInfo);
}

void RobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加额外的打印前进行的初始化过程
}

void RobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加打印后进行的清理过程
}

void RobotView::OnDestroy()
{
    // 析构时停用此项；这在
    // 使用拆分视图时非常重要 
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr && pActiveItem->GetActiveView() == this)
    {
        pActiveItem->Deactivate();
        ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
    }
    CView::OnDestroy();
}



// OLE 客户端支持和命令

BOOL RobotView::IsSelected(const CObject* pDocItem) const
{
    //下面的实现是足够的，如果所选内容包括
    //  CMFC_RobotCntrItem 对象，则以下实现就足够了。若要处理其他选择机制，
    //  则应替换此处的实现

    // TODO: 实现对所选 OLE 客户端项进行测试的函数

    return pDocItem == m_pSelection;
}

void RobotView::OnInsertObject()
{
    // 调用标准的“插入对象”对话框以获得有关
    //  新 CMFC_RobotCntrItem 对象的信息
    COleInsertDialog dlg;
    if (dlg.DoModal() != IDOK)
        return;

    BeginWaitCursor();

    CMFC_RobotCntrItem* pItem = nullptr;
    TRY
    {
        // 创建与此文档相连接的新项
        RobotDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        pItem = new CMFC_RobotCntrItem(pDoc);
        ASSERT_VALID(pItem);

        // 通过对话框数据初始化该项
        if (!dlg.CreateItem(pItem))
            AfxThrowMemoryException();  // 任何异常都将导致该结果
        ASSERT_VALID(pItem);

        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
            pItem->DoVerb(OLEIVERB_SHOW, this);

        ASSERT_VALID(pItem);
        // 作为任意用户界面设计，这会将所选内容
        //  设置为插入的最后一项

        // TODO: 重新实现所选内容，使其适合于您的应用程序
        m_pSelection = pItem;   // 将所选内容设置为插入的最后一项
        pDoc->UpdateAllViews(nullptr);
    }
        CATCH(CException, e)
    {
        if (pItem != nullptr)
        {
            ASSERT_VALID(pItem);
            pItem->Delete();
        }
        AfxMessageBox(IDP_FAILED_TO_CREATE);
    }
    END_CATCH

        EndWaitCursor();
}

// 以下命令处理程序提供了标准键盘
//  用户界面以取消就地编辑会话。  此处，
//  容器(而不是服务器)将导致停用
void RobotView::OnCancelEditCntr()
{
    // 关闭此视图中的任何就地活动项。
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr)
    {
        pActiveItem->Close();
    }
    ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
}

// 在就地编辑一个对象时，容器需要对 OnSetFocus 和 OnSize 
//  进行特殊处理
void RobotView::OnSetFocus(CWnd* pOldWnd)
{
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr &&
        pActiveItem->GetItemState() == COleClientItem::activeUIState)
    {
        // 如果该项处于同一视图中，则需要将焦点设置到该项
        CWnd* pWnd = pActiveItem->GetInPlaceWindow();
        if (pWnd != nullptr)
        {
            pWnd->SetFocus();   // 不要调用基类
            return;
        }
    }

    CView::OnSetFocus(pOldWnd);
}

void RobotView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr)
        pActiveItem->SetItemRects();
}

void RobotView::OnFilePrint()
{
    //默认情况下，要求活动文档使用 IOleCommandTarget 打印自身。
    //如果不需要此行为，
    //请移除对 COleDocObjectItem::DoDefaultPrinting 调用。
    //如果由于某种原因调用失败，则将尝试使用 IPrint 接口打印
    //该文档对象。
    CPrintInfo printInfo;
    ASSERT(printInfo.m_pPD != nullptr);
    if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
        return;

    CView::OnFilePrint();

}

void RobotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CRect xTempClientRect;
    this->GetClientRect(&xTempClientRect);
    Int32 xMaxWidth = xTempClientRect.Width();
    Int32 xMaxHeight = xTempClientRect.Height();
    Int32 xUnitWidth = 20;
    Int32 xUnitHeight = 20;

    Int32 X = m_SelectRobotUnitIndex % (xMaxWidth / xUnitWidth);
    Int32 Y = m_SelectRobotUnitIndex / (xMaxWidth / xUnitWidth);
    CRect xTempRect(X * xUnitWidth, Y * xUnitHeight, (X * xUnitWidth) + 20, (Y * xUnitHeight) + 20);
    this->InvalidateRect(&xTempRect, TRUE);
    if (nChar == VK_LEFT)
    {
        if (m_SelectRobotUnitIndex > 0)
        {
            m_SelectRobotUnitIndex--;
        }
    }
    else if (nChar == VK_RIGHT)
    {
        if (m_SelectRobotUnitIndex < m_ListRobot.GetCount() - 1)
        {
            m_SelectRobotUnitIndex++;
        }
    }
    if (m_SelectRobotUnitIndex > 0 && m_SelectRobotUnitIndex < m_ListRobot.GetCount())
    {
        this->ShowRobotLog();
    }
}

void RobotView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect xTempClientRect;
    this->GetClientRect(&xTempClientRect);
    Int32 xMaxWidth = xTempClientRect.Width();
    Int32 xMaxHeight = xTempClientRect.Height();
    Int32 xUnitWidth = 20;
    Int32 xUnitHeight = 20;

    Int32 xNewSelectRobotUnitIndex = (point.y / xUnitHeight)* (xMaxWidth / xUnitWidth) + (point.x / xUnitWidth);
    if (xNewSelectRobotUnitIndex < m_ListRobot.GetCount())
    {
        Int32 X = m_SelectRobotUnitIndex % (xMaxWidth / xUnitWidth);
        Int32 Y = m_SelectRobotUnitIndex / (xMaxWidth / xUnitWidth);
        CRect xTempRect(X * xUnitWidth, Y * xUnitHeight, (X * xUnitWidth) + 20, (Y * xUnitHeight) + 20);
        this->InvalidateRect(&xTempRect, TRUE);
        m_SelectRobotUnitIndex = xNewSelectRobotUnitIndex;
        this->ShowRobotLog();
    }
}

void RobotView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void RobotView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    if (m_ListRobot.Empty())
    {
        theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_ROBOT_LIST, point.x, point.y, this, TRUE);
    }
    else
    {
        theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
    }
#endif
}


// RobotView 诊断

#ifdef _DEBUG
void RobotView::AssertValid() const
{
    CView::AssertValid();
}

void RobotView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

RobotDoc* RobotView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RobotDoc)));
    return (RobotDoc*)m_pDocument;
}
#endif //_DEBUG


// RobotView 消息处理程序
Boolean RobotView::AddNewRobot(wstr xAddr, WORD xPort)
{
    ::SetTimer(m_hWnd, 100, 100, nullptr);

    if (m_ListRobot.GetCount() >= 2000)
    {
        this->MessageBox(L"本页机器人已达上限[2000]!");
        return False;
    }
    Int64 xNewRobotKey = INVALID_LID;

    RobotCellPlayer* xRobotPlayer = new RobotCellPlayer(&m_MsThreadIOCPManager, this, g_CommandList);
    m_ListRobot.Add(xRobotPlayer);

    //if (MsBase::Ping(MsBase::W2M(xAddr)))
    //{
    if (m_NetRobot.BlockCreateClient(MsBase::W2M(xAddr), xPort, *xRobotPlayer))
    {
        xRobotPlayer->m_IsConnected = True;
        xRobotPlayer->ReadyState();
    }
    //}
    return True;
}

void RobotView::OnTimer(UINT_PTR nIDEvent)
{
    this->Invalidate(False);

    if (ID_ROBOT_TIMER == nIDEvent)
    {
    }

    //CRect xTempClientRect;
    //this->GetClientRect(&xTempClientRect);
    //Int32 xMaxWidth = xTempClientRect.Width();
    //Int32 xMaxHeight = xTempClientRect.Height();
    //Int32 xUnitWidth = 20;
    //Int32 xUnitHeight = 20;

    //Int32 xTempIndex = 0;
    //FAST_FOREACH(m_ListRobot)
    //{
    //    if (xEnumValue->IsChange())
    //    {
    //        Int32 X = xTempIndex % (xMaxWidth / xUnitWidth);
    //        Int32 Y = xTempIndex / (xMaxWidth / xUnitWidth);
    //        switch (xEnumValue->GetState())
    //        {
    //            case E_ROBOT_STATE::ERS_READY: { m_GreenImage.Draw(this->GetDC()->m_hDC, X * xUnitWidth, Y * xUnitHeight); }break;
    //            case E_ROBOT_STATE::ERS_WAIT: { m_YellowImage.Draw(this->GetDC()->m_hDC, X * xUnitWidth, Y * xUnitHeight); }break;
    //            case E_ROBOT_STATE::ERS_STOP:
    //            case E_ROBOT_STATE::ERS_INVALID: { m_RedImage.Draw(this->GetDC()->m_hDC, X * xUnitWidth, Y * xUnitHeight); }break;
    //        }
    //    }
    //    xTempIndex++;
    //}

    if (m_IsMove && MsBase::TimeSpanCheck(m_LastMoveTime, 100))
    {
        FAST_FOREACH(m_ListRobot)
        {
            var& xValue = m_ListRobot.Value();
            if (xValue->GetState() == E_ROBOT_STATE::ERS_READY)
            {
                if (R_DWORD(0, 10) == 0)
                {
                    C2S_Sync_RoleMove xAsk;
                    xAsk.set_role_index(xValue->m_RoleIndex);
                    xAsk.set_begin_x(xValue->m_RoleInfo.GetRolePublic()->GetScenePosX());
                    xAsk.set_begin_y(xValue->m_RoleInfo.GetRolePublic()->GetScenePosY());

                    var xCurPos = xValue->m_RoleInfo.GetRolePublic()->GetPos();
                    xCurPos.X += R_FLOAT(-50, 50); xCurPos.X = abs(xCurPos.X);
                    xCurPos.Y += R_FLOAT(-50, 50); xCurPos.Y = abs(xCurPos.Y);

                    //C2S_Chat xChat;
                    //xChat.set_role_index(xEnumValue->m_RoleIndex);
                    //DWORD dwBackColor = 0;
                    ////DWORD dwBackColor = ((DWORD)R_BYTE(0x2F, 0xFF)) << 24;
                    ////dwBackColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 16;
                    ////dwBackColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 8;
                    ////dwBackColor += ((DWORD)R_BYTE(0x2F, 0xFF));
                    //xChat.set_backcolor(dwBackColor);

                    ////DWORD dwBoxColor = 0;
                    //DWORD dwBoxColor = ((DWORD)R_BYTE(0x2F, 0xFF)) << 24;
                    //dwBoxColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 16;
                    //dwBoxColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 8;
                    //dwBoxColor += ((DWORD)R_BYTE(0x2F, 0xFF));
                    //xChat.set_boxcolor(dwBoxColor);

                    //DWORD dwShadowColor = 0;
                    ////DWORD dwShadowColor = ((DWORD)R_BYTE(0x2F, 0xFF)) << 24;
                    ////dwShadowColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 16;
                    ////dwShadowColor += ((DWORD)R_BYTE(0x2F, 0xFF)) << 8;
                    ////dwShadowColor += ((DWORD)R_BYTE(0x2F, 0xFF));
                    //xChat.set_shadowcolor(dwShadowColor);

                    //xChat.set_msgcolor(R_DWORD(0xFFF00000, 0xFFFFFFFF));
                    //xChat.set_chat_type(E_CHAT_TYPE::ECT_CURRENT);
                    //CHAR szText[1024];

                    //sprintf_s(szText, "[%f],[%f]#%d", xTarX, xTarY, g_FaceId[R_DWORD(0, g_FaceId.GetCount())]);
                    //xChat.set_msg(szText);
                    //xEnumValue->RobotSendPacket(xChat);

                    xAsk.set_end_x(xCurPos.X);
                    xAsk.set_end_y(xCurPos.Y);
                    xAsk.set_is_run(true);
                    //xEnumValue->m_RoleInfo.GetRolePublic()->SetScenePosX(xTarX);
                    //xEnumValue->m_RoleInfo.GetRolePublic()->SetScenePosY(xTarY);
                    //xValue->RobotSendPacket(xAsk);
                    xValue->WatiState();
                }
            }
        }
    }
}

void RobotView::OnEdit_C2S_Login()
{
    if (!m_IsLogin)
    {
        FAST_FOREACH(m_ListRobot)
        {
            xEnumValue->Roboto_C2S_Login();
        }
        m_IsLogin = True;
    }
    else
    {
        AfxMessageBox(L"这批机器人已经登录过了!");
    }
}

void RobotView::OnEdit_C2S_CreateRole()
{
    FAST_FOREACH(m_ListRobot)
    {
        xEnumValue->Roboto_C2S_CreateRole();
    }
}


void RobotView::OnEdit_C2S_EnterScene()
{
    FAST_FOREACH(m_ListRobot)
    {
        xEnumValue->Roboto_C2S_EnterScene();
    }
}


void RobotView::OnEdit_GM_IAmGod()
{
    FAST_FOREACH(m_ListRobot)
    {
        xEnumValue->Roboto_GM_IAmGod();
    }
}

void RobotView::OnEdit_Go_SkyMonter()
{
    FAST_FOREACH(m_ListRobot)
    {
        xEnumValue->GoToSkyMonter();
    }
}

void RobotView::OnEdit_C2S_Register()
{
    if (!m_IsLogin && !m_IsRegister)
    {
        FAST_FOREACH(m_ListRobot)
        {
            xEnumValue->Roboto_C2S_Register();
        }
        m_IsRegister = True;
    }
    else
    {
        AfxMessageBox(L"这批机器人当前不可进行注册!");
    }
}

void RobotView::OnRobotMove()
{
    if (m_IsMove)
    {
        m_IsMove = False;
        AfxMessageBox(L"已经关闭移动!");
    }
    else
    {
        if (!m_IsLogin)
        {
            AfxMessageBox(L"机器人尚未登录!");
            return;
        }
        FAST_FOREACH(m_ListRobot)
        {
            if (xEnumValue->GetState() == E_ROBOT_STATE::ERS_WAIT)
            {
                AfxMessageBox(L"机器人尚未登录完毕!");
                return;
            }
        }
        m_IsMove = True;
        AfxMessageBox(L"已经开启移动!");
    }
}

void RobotView::OnRobotChangeScene()
{
    C2S_Change_Scene xPacket;
    FAST_FOREACH(m_ListRobot)
    {
        if (xEnumValue->GetState() == E_ROBOT_STATE::ERS_WAIT)
        {
            AfxMessageBox(L"机器人尚未登录完毕!");
            return;
        }

        xPacket.set_role_index(xEnumValue->m_RoleIndex);
        if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1478)  // 大唐边境
        {
            xPacket.set_tar_scene_id(1464);
            xPacket.set_pos_x(39);
            xPacket.set_pos_y(16);
        }
        else if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1464)   // 大唐境内
        {
            xPacket.set_tar_scene_id(1477);
            xPacket.set_pos_x(39);
            xPacket.set_pos_y(16);
        }
        else if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1477)   // 大唐境内
        {
            xPacket.set_tar_scene_id(1478);
            xPacket.set_pos_x(25);
            xPacket.set_pos_y(15);
        }
        //xEnumValue->RobotSendPacket(xPacket);
    }
}

void RobotView::CreateRobot(Int32 xRobotCount)
{
    //wstr xRegistryPath = L"SOFTWARE\\";
    //xRegistryPath += _T(PJ_CLIENT_REGISTRY_FIELD_NAME);
    wstr xServerAddr = MsBase::M2W(GetServerAddr());


    Int32 xPort = 0;

    if (MsBase::stoi(g_List[1].c_str(), xPort))
    {
        for (Int32 i = 0; i < xRobotCount; i++)
        {
            if (!this->AddNewRobot(MsBase::M2W(g_List[0]), xPort))
            {
                break;
            }
        }
    }
}

void RobotView::OnCreateRobot1()
{
    this->CreateRobot(1);
}

void RobotView::OnCreateRobot2()
{
    this->CreateRobot(2);
}


void RobotView::OnCreateRobot5()
{
    this->CreateRobot(5);
}


void RobotView::OnCreateRobot10()
{
    this->CreateRobot(10);
}


void RobotView::OnCreateRobot50()
{
    this->CreateRobot(50);
}

void RobotView::OnCreateRobot100()
{
    this->CreateRobot(100);
}


void RobotView::OnCreateRobot500()
{
    this->CreateRobot(500);
}


void RobotView::OnCreateRobot1000()
{
    this->CreateRobot(1000);
}


void RobotView::ShowRobotLog()
{
    if (m_SelectRobotUnitIndex != INVALID_LID)
    {
        CMainFrame* xCMainFrame = (CMainFrame*)AfxGetMainWnd();
        if (xCMainFrame)
        {
            xCMainFrame->m_wndOutput.ClearRobotLogWindow();
            auto& xLogs = m_ListRobot[m_SelectRobotUnitIndex]->m_RobotLogs;
            FAST_FOREACH(xLogs)
            {
                xCMainFrame->m_wndOutput.FillRobotLogWindow(MsBase::M2W(xEnumValue).c_str());
            }

            CHAR szBuff[100];
            sprintf_s(szBuff, "-------LOG SHOW FINISH[%d]-------", xLogs.GetCount());
            xCMainFrame->m_wndOutput.FillRobotLogWindow(MsBase::M2W(szBuff).c_str());
        }
    }
}

