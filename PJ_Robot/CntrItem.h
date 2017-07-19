
// CntrItem.h : CMFC_RobotCntrItem ��Ľӿ�
//

#pragma once

class RobotDoc;
class RobotView;

class CMFC_RobotCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CMFC_RobotCntrItem)

// ���캯��
public:
	CMFC_RobotCntrItem(RobotDoc* pContainer = nullptr);
		// ע��: ���� pContainer Ϊ nullptr ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����  OLE ��ͨ������
		//  �� nullptr �ĵ�ָ�빹���

// ����
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

// ʵ��
public:
	~CMFC_RobotCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

