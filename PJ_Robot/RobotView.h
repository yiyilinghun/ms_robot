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
    // 生成的消息映射函数
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

#ifndef _DEBUG  // MFC_RobotView.cpp 中的调试版本
inline RobotDoc* RobotView::GetDocument() const
{
    return reinterpret_cast<RobotDoc*>(m_pDocument);
}
#endif

