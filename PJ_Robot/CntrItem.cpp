
// CntrItem.cpp : CMFC_RobotCntrItem ���ʵ��
//

#include "Precompiled.h"

#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_RobotCntrItem ��ʵ��

IMPLEMENT_SERIAL(CMFC_RobotCntrItem, COleClientItem, 0)

CMFC_RobotCntrItem::CMFC_RobotCntrItem(RobotDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: �ڴ����һ���Թ������
}

CMFC_RobotCntrItem::~CMFC_RobotCntrItem()
{
	// TODO: �ڴ˴�����������
}

void CMFC_RobotCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// �ڱ༭ĳ��ʱ(�͵ر༭����ȫ�򿪽��б༭)��
	//  �����������״̬�ĸ���
	//  �������ݵĿ������״̬�ĸ��ķ��� OnChange ֪ͨ��

	// TODO: ͨ������ UpdateAllViews ʹ������Ч
	//  (�����ʺ�����Ӧ�ó������ʾ)

	GetDocument()->UpdateAllViews(nullptr);
		// ����ֻ����������ͼ/����ʾ
}

BOOL CMFC_RobotCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬���������� CMFC_RobotCntrItem::OnChangeItemPosition
	//  �Ը��ľ͵ش��ڵ�λ�á�
	//  ��ͨ�������ڷ�����
	//  �ĵ��е����ݸ��Ķ����·�Χ�ı����
	//  �͵ص�����С������ġ�
	//
	// �˴���Ĭ�����õ��û��࣬�û��ཫ����
	//  COleClientItem::SetItemRects �Խ�����
	//  �ƶ����µ�λ�á�

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: ���¸���ľ���/��Χ���ܴ��ڵ��κλ���

	return TRUE;
}

BOOL CMFC_RobotCntrItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow)
{
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, pFrameWnd);

	if (pMainFrame != nullptr)
	{
		ASSERT_VALID(pMainFrame);
		return pMainFrame->OnShowPanes(bShow);
	}

	return FALSE;
}

void CMFC_RobotCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬������ CMFC_RobotCntrItem::OnGetItemPosition
	//  �Ծ��������λ�á�  ͨ����
	//  �þ��λᷴӳ�뼤��������ͼ 
	//  �����ĵ�ǰλ�á�  ���� 
	//  CMFC_RobotCntrItem::GetActiveView �ɵõ���ͼ��

	// TODO: ���� rPosition �е���ȷ����(�����ر�ʾ)

	CSize size;
	rPosition.SetRectEmpty();
	if (GetExtent(&size, m_nDrawAspect))
	{
		RobotView* pView = GetActiveView();
		ASSERT_VALID(pView);
		if (!pView)
			return;
		CDC *pDC = pView->GetDC();
		ASSERT(pDC);
		if (!pDC)
			return;
		pDC->HIMETRICtoLP(&size);
		rPosition.SetRect(10, 10, size.cx + 10, size.cy + 10);
	}
	else
		rPosition.SetRect(10, 10, 210, 210);
}

void CMFC_RobotCntrItem::OnActivate()
{
    // ÿ�����ֻ����һ���͵ؼ�����
    RobotView* pView = GetActiveView();
    ASSERT_VALID(pView);
	if (!pView)
		return;
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != nullptr && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CMFC_RobotCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, nullptr);
}

void CMFC_RobotCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// ���ȵ��û����Զ�ȡ COleClientItem ���ݡ�
	// ��Ϊ���������ô� CMFC_RobotCntrItem::GetDocument ���ص� m_pDocument ָ�룬
	//  ��������ȵ���
	//  ���� Serialize��
	COleClientItem::Serialize(ar);

	// ���ڴ洢/�����ض��� CMFC_RobotCntrItem ������
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CMFC_RobotCntrItem ���

#ifdef _DEBUG
void CMFC_RobotCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CMFC_RobotCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

