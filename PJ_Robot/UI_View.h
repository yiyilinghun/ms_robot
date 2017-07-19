#pragma once

class UI_ViewToolBar : public CMFCToolBar
{
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
};


class UI_View : public CDockablePane
{
    // 构造
public:
    UI_View();

    void AdjustLayout();
    void OnChangeVisualStyle();

    // 特性
protected:

    CViewTree m_RobotListViewTree;
    CImageList m_RobotListImages;
    UI_ViewToolBar m_wndToolBar;

protected:
    void FillFileView();

    // 实现
public:
    virtual ~UI_View();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnProperties();
    afx_msg void OnFileOpen();
    afx_msg void OnFileOpenWith();
    afx_msg void OnDummyCompile();
    afx_msg void OnEditCut();
    afx_msg void OnEditCopy();
    afx_msg void OnEditClear();
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);

    DECLARE_MESSAGE_MAP()
public:
    void CreateRobot(Int32 xRobotCount);

    afx_msg void OnCreateRobot1();
    afx_msg void OnCreateRobot100();
    afx_msg void OnCreateRobot2();
    afx_msg void OnCreateRobot5();
    afx_msg void OnCreateRobot10();
    afx_msg void OnCreateRobot50();
    afx_msg void OnCreateRobot500();
    afx_msg void OnCreateRobot1000();
};

