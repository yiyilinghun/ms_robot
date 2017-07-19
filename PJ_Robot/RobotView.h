#pragma once

class NetRobot : public MsNetClient
{
public:
    NetRobot(RobotView* xRobotView) :m_RobotView(xRobotView) { ; }
    RobotView* m_RobotView;
protected:
    virtual MsNetTerminal* GetNewNetTerminal(MsIOCPManager* xMsIOCPManager)
    {
        return new RobotPlayer(xMsIOCPManager, m_RobotView);
    }
};

class CMFC_RobotCntrItem;

class RobotView : public CView
{
protected: // �������л�����
    RobotView();
    DECLARE_DYNCREATE(RobotView)

    // ����
public:
    RobotDoc* GetDocument() const;
    // m_pSelection ����ѡ���ݱ����ڵ�ǰ�� CMFC_RobotCntrItem �С�
    // �������Ӧ�ó������ֳ�Ա����������
    //  ��ʾĳ��ѡ�������ڲ����� CMFC_RobotCntrItem �Ķ�����
    //  ѡ����һ����������  �ṩ����ѡ��
    //  ���Ƶ�Ŀ��ֻ�ǰ���������

    // TODO: ���ʺ�Ӧ�ó����ѡ������滻��ѡ�����
    CMFC_RobotCntrItem* m_pSelection;

    // ����
public:

    // ��д
public:
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void OnInitialUpdate(); // ������һ�ε���
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual BOOL IsSelected(const CObject* pDocItem) const;// ����֧��

// ʵ��
public:
    virtual ~RobotView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif


    Boolean AddNewRobot(wstr xAddr, WORD xPort);
    void CreateRobot(Int32 xRobotCount);

    Boolean m_IsActivated;
    MsList<RobotPlayer*> m_ListRobot;

protected:

    NetRobot m_NetRobot;
    MsThreadIOCPManager m_MsThreadIOCPManager;

    CBrush m_RedBrush;
    CBrush m_YellowBrush;
    CBrush m_GreenBrush;
    CPen m_Pen;

    //CImage m_RedImage;
    //CImage m_YellowImage;
    //CImage m_GreenImage;

    Boolean m_IsLogin;
    Boolean m_IsMoving;


    Int32 m_SelectRobotUnitIndex;

    Int64 m_LastMoveTime;
    Boolean m_IsMove;

    void ShowRobotLog();
    // ���ɵ���Ϣӳ�亯��
protected:
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

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEdit_C2S_Login();
    afx_msg void OnRobotMove();
    afx_msg void OnRobotChangeScene();
};

#ifndef _DEBUG  // MFC_RobotView.cpp �еĵ��԰汾
inline RobotDoc* RobotView::GetDocument() const
{
    return reinterpret_cast<RobotDoc*>(m_pDocument);
}
#endif

