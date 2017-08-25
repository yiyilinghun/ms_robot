
// MFC_RobotView.cpp : RobotView ���ʵ��
//

#include "Precompiled.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
    // TODO: �ڴ˴���ӹ������

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

    //LoadImageFromZip(m_RedImage, "..\\..\\res.zip", "res/Robot���.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());
    //LoadImageFromZip(m_YellowImage, "..\\..\\res.zip", "res/Robot�ƿ�.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());
    //LoadImageFromZip(m_GreenImage, "..\\..\\res.zip", "res/Robot�̿�.png", MsBase::CreateZipPassword(xList1, xList2, xList3, xList4).c_str());

    //mstr xRegistryPath = "SOFTWARE\\";
    //xRegistryPath += PJ_CLIENT_REGISTRY_FIELD_NAME;
    //mstr xBaseGamePath = MsRegistry::GetRegistryStringA(HKEY_CURRENT_USER, xRegistryPath.c_str(), "BASEDIR");

    //m_RedImage.Load(MsBase::M2W(xBaseGamePath + "\\Robot���.png").c_str());
    //.Load(MsBase::M2W(xBaseGamePath + "\\Robot�ƿ�.png").c_str());
    //m_GreenImage.Load(MsBase::M2W(xBaseGamePath + "\\Robot�̿�.png").c_str());
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
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return CView::PreCreateWindow(cs);
}

// RobotView ����

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

    // TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
    // TODO: ͬʱ�����ĵ��е����� OLE ��

    // ������λ�û���ѡ���  һ��
    //  ʵ���������Ļ��ƴ����Ӧ�Ƴ��˴��롣  ��λ��
    //  �� CMFC_RobotCntrItem ���صľ�����ȫ��Ӧ��
    //  �Ӷ������͵ر༭��Ч����

    // TODO: ���ջ��ƴ�����ɺ��Ƴ��˴��롣
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


    // TODO: д������ѡ��ģʽ����֮���Ƴ��˴���
    m_pSelection = nullptr;    // ��ʼ����ѡ����

}


// RobotView ��ӡ


void RobotView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL RobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Ĭ��׼��
    return DoPreparePrinting(pInfo);
}

void RobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void RobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӵ�ӡ����е��������
}

void RobotView::OnDestroy()
{
    // ����ʱͣ�ô������
    // ʹ�ò����ͼʱ�ǳ���Ҫ 
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr && pActiveItem->GetActiveView() == this)
    {
        pActiveItem->Deactivate();
        ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
    }
    CView::OnDestroy();
}



// OLE �ͻ���֧�ֺ�����

BOOL RobotView::IsSelected(const CObject* pDocItem) const
{
    //�����ʵ�����㹻�ģ������ѡ���ݰ���
    //  CMFC_RobotCntrItem ����������ʵ�־��㹻�ˡ���Ҫ��������ѡ����ƣ�
    //  ��Ӧ�滻�˴���ʵ��

    // TODO: ʵ�ֶ���ѡ OLE �ͻ�������в��Եĺ���

    return pDocItem == m_pSelection;
}

void RobotView::OnInsertObject()
{
    // ���ñ�׼�ġ�������󡱶Ի����Ի���й�
    //  �� CMFC_RobotCntrItem �������Ϣ
    COleInsertDialog dlg;
    if (dlg.DoModal() != IDOK)
        return;

    BeginWaitCursor();

    CMFC_RobotCntrItem* pItem = nullptr;
    TRY
    {
        // ��������ĵ������ӵ�����
        RobotDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        pItem = new CMFC_RobotCntrItem(pDoc);
        ASSERT_VALID(pItem);

        // ͨ���Ի������ݳ�ʼ������
        if (!dlg.CreateItem(pItem))
            AfxThrowMemoryException();  // �κ��쳣�������¸ý��
        ASSERT_VALID(pItem);

        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
            pItem->DoVerb(OLEIVERB_SHOW, this);

        ASSERT_VALID(pItem);
        // ��Ϊ�����û�������ƣ���Ὣ��ѡ����
        //  ����Ϊ��������һ��

        // TODO: ����ʵ����ѡ���ݣ�ʹ���ʺ�������Ӧ�ó���
        m_pSelection = pItem;   // ����ѡ��������Ϊ��������һ��
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

// �������������ṩ�˱�׼����
//  �û�������ȡ���͵ر༭�Ự��  �˴���
//  ����(�����Ƿ�����)������ͣ��
void RobotView::OnCancelEditCntr()
{
    // �رմ���ͼ�е��κξ͵ػ�
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr)
    {
        pActiveItem->Close();
    }
    ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
}

// �ھ͵ر༭һ������ʱ��������Ҫ�� OnSetFocus �� OnSize 
//  �������⴦��
void RobotView::OnSetFocus(CWnd* pOldWnd)
{
    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
    if (pActiveItem != nullptr &&
        pActiveItem->GetItemState() == COleClientItem::activeUIState)
    {
        // ��������ͬһ��ͼ�У�����Ҫ���������õ�����
        CWnd* pWnd = pActiveItem->GetInPlaceWindow();
        if (pWnd != nullptr)
        {
            pWnd->SetFocus();   // ��Ҫ���û���
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
    //Ĭ������£�Ҫ���ĵ�ʹ�� IOleCommandTarget ��ӡ����
    //�������Ҫ����Ϊ��
    //���Ƴ��� COleDocObjectItem::DoDefaultPrinting ���á�
    //�������ĳ��ԭ�����ʧ�ܣ��򽫳���ʹ�� IPrint �ӿڴ�ӡ
    //���ĵ�����
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


// RobotView ���

#ifdef _DEBUG
void RobotView::AssertValid() const
{
    CView::AssertValid();
}

void RobotView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

RobotDoc* RobotView::GetDocument() const // �ǵ��԰汾��������
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RobotDoc)));
    return (RobotDoc*)m_pDocument;
}
#endif //_DEBUG


// RobotView ��Ϣ�������
Boolean RobotView::AddNewRobot(wstr xAddr, WORD xPort)
{
    ::SetTimer(m_hWnd, 100, 100, nullptr);

    if (m_ListRobot.GetCount() >= 2000)
    {
        this->MessageBox(L"��ҳ�������Ѵ�����[2000]!");
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
        AfxMessageBox(L"�����������Ѿ���¼����!");
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
        AfxMessageBox(L"���������˵�ǰ���ɽ���ע��!");
    }
}

void RobotView::OnRobotMove()
{
    if (m_IsMove)
    {
        m_IsMove = False;
        AfxMessageBox(L"�Ѿ��ر��ƶ�!");
    }
    else
    {
        if (!m_IsLogin)
        {
            AfxMessageBox(L"��������δ��¼!");
            return;
        }
        FAST_FOREACH(m_ListRobot)
        {
            if (xEnumValue->GetState() == E_ROBOT_STATE::ERS_WAIT)
            {
                AfxMessageBox(L"��������δ��¼���!");
                return;
            }
        }
        m_IsMove = True;
        AfxMessageBox(L"�Ѿ������ƶ�!");
    }
}

void RobotView::OnRobotChangeScene()
{
    C2S_Change_Scene xPacket;
    FAST_FOREACH(m_ListRobot)
    {
        if (xEnumValue->GetState() == E_ROBOT_STATE::ERS_WAIT)
        {
            AfxMessageBox(L"��������δ��¼���!");
            return;
        }

        xPacket.set_role_index(xEnumValue->m_RoleIndex);
        if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1478)  // ���Ʊ߾�
        {
            xPacket.set_tar_scene_id(1464);
            xPacket.set_pos_x(39);
            xPacket.set_pos_y(16);
        }
        else if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1464)   // ���ƾ���
        {
            xPacket.set_tar_scene_id(1477);
            xPacket.set_pos_x(39);
            xPacket.set_pos_y(16);
        }
        else if (xEnumValue->m_RoleInfo.GetRolePublic()->GetSceneId() == 1477)   // ���ƾ���
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

