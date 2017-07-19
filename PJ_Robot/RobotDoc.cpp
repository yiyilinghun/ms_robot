
// MFC_RobotDoc.cpp : RobotDoc ���ʵ��
//

#include "Precompiled.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS

#endif

#include "CntrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RobotDoc

IMPLEMENT_DYNCREATE(RobotDoc, COleDocument)

BEGIN_MESSAGE_MAP(RobotDoc, COleDocument)
    // ����Ĭ�ϵ� OLE ����ʵ��
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &COleDocument::OnUpdatePasteMenu)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, &COleDocument::OnUpdatePasteLinkMenu)
    ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, &COleDocument::OnUpdateObjectVerbMenu)
    ON_COMMAND(ID_OLE_EDIT_CONVERT, &COleDocument::OnEditConvert)
    ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &COleDocument::OnUpdateEditLinksMenu)
    ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &RobotDoc::OnUpdateObjectVerbPopup)
    ON_COMMAND(ID_OLE_EDIT_LINKS, &COleDocument::OnEditLinks)
    ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// RobotDoc ����/����

RobotDoc::RobotDoc()
{
    // ʹ�� OLE �����ļ�
    EnableCompoundFile();

    // TODO: �ڴ����һ���Թ������

}

RobotDoc::~RobotDoc()
{
}

BOOL RobotDoc::OnNewDocument()
{
    if (!COleDocument::OnNewDocument())
        return FALSE;

    // TODO: �ڴ�������³�ʼ������
    // (SDI �ĵ������ø��ĵ�)

    return TRUE;
}




// RobotDoc ���л�

void RobotDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: �ڴ���Ӵ洢����
    }
    else
    {
        // TODO: �ڴ���Ӽ��ش���
    }

    // ���û��� COleDocument ������
    //  �����ĵ��� COleClientItem ��������л���
    COleDocument::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void RobotDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // �޸Ĵ˴����Ի����ĵ�����
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// ������������֧��
void RobotDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // ���ĵ����������������ݡ�
    // ���ݲ���Ӧ�ɡ�;���ָ�

    // ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
    SetSearchContent(strSearchContent);
}

void RobotDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl *pChunk = nullptr;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != nullptr)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// RobotDoc ���

#ifdef _DEBUG
void RobotDoc::AssertValid() const
{
    COleDocument::AssertValid();
}

void RobotDoc::Dump(CDumpContext& dc) const
{
    COleDocument::Dump(dc);
}
#endif //_DEBUG


// RobotDoc ����
