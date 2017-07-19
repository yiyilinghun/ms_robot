
// CntrItem.h : CMFC_RobotCntrItem 类的接口
//

#pragma once

class RobotDoc;
class RobotView;

class CMFC_RobotCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CMFC_RobotCntrItem)

// 构造函数
public:
	CMFC_RobotCntrItem(RobotDoc* pContainer = nullptr);
		// 注意: 允许 pContainer 为 nullptr 以启用 IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE 要求类具有带零
		//  参数的构造函数。  OLE 项通常是用
		//  非 nullptr 文档指针构造的

// 特性
public:
	RobotDoc* GetDocument()
		{ return reinterpret_cast<RobotDoc*>(COleClientItem::GetDocument()); }
	RobotView* GetActiveView()
		{ return reinterpret_cast<RobotView*>(COleClientItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

// 实现
public:
	~CMFC_RobotCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

