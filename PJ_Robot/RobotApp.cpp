
// MFC_Robot.cpp : ����Ӧ�ó��������Ϊ��
//

#include "Precompiled.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "RobotApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// main����֮ǰ
BEFORE_MAIN_RUN
{
    BEGIN_INIT;
}

// main����֮��
AFTER_MAIN_RUN
{
    END_CLEAR;
}


// RobotApp

BEGIN_MESSAGE_MAP(RobotApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &RobotApp::OnAppAbout)
    // �����ļ��ı�׼�ĵ�����
    ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
    // ��׼��ӡ��������
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// RobotApp ����
RobotApp::RobotApp()
{
    m_bHiColorIcons = TRUE;

    // ֧����������������
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
    // ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
    //     1) �����д˸������ã�������������������֧�ֲ�������������
    //     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
    System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

    // TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
    //Ϊ CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID(_T("MFC_Robot.AppID.NoVersion"));

    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� RobotApp ����

RobotApp theApp;


// RobotApp ��ʼ��

BOOL RobotApp::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();


    // ��ʼ�� OLE ��
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();

    EnableTaskbarInteraction();

    // ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
    // AfxInitRichEdit2();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    LoadStdProfileSettings(10);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


    InitContextMenuManager();

    InitKeyboardManager();

    InitTooltipManager();
    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    // ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
    // �������ĵ�����ܴ��ں���ͼ֮�������
    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(IDR_MFC_RobotTYPE,
        RUNTIME_CLASS(RobotDoc),
        RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
        RUNTIME_CLASS(RobotView));
    if (!pDocTemplate)
        return FALSE;
    pDocTemplate->SetContainerInfo(IDR_MFC_RobotTYPE_CNTR_IP);
    AddDocTemplate(pDocTemplate);

    // ������ MDI ��ܴ���
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
        delete pMainFrame;
        return FALSE;
    }
    m_pMainWnd = pMainFrame;

    // �������к�׺ʱ�ŵ��� DragAcceptFiles
    //  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
    // ������/��
    m_pMainWnd->DragAcceptFiles();

    // ������׼ shell ���DDE�����ļ�������������
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // ���á�DDE ִ�С�
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);


    // ��������������ָ�������  ���
    // �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;
    // �������ѳ�ʼ���������ʾ����������и���
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();


    this->LoadCommand("protocol_list.txt");
    return TRUE;
}

int RobotApp::ExitInstance()
{
    //TODO: �����������ӵĸ�����Դ
    AfxOleTerm(FALSE);

    return CWinAppEx::ExitInstance();
}

// RobotApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void RobotApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

// RobotApp �Զ������/���淽��

void RobotApp::PreLoadState()
{
    BOOL bNameValid;
    CString strName;
    bNameValid = strName.LoadString(IDS_EDIT_MENU);
    ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
    bNameValid = strName.LoadString(IDS_EXPLORER);
    ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void RobotApp::LoadCustomState()
{
}

void RobotApp::SaveCustomState()
{
}

// RobotApp ��Ϣ�������
void RobotApp::LoadCommand(mstr xCommandFileName)
{
    Char xBuffer[16 * KB_SIZE];
    std::ifstream xFileStream(xCommandFileName);
    if (!xFileStream.is_open())
    {
        //LOG_ERROR("file is open!");
        return;
    }
    while (!xFileStream.eof())
    {
        xFileStream.getline(xBuffer, 1024);
        std::string strSrc(xBuffer);
        if (!strSrc.empty())
        {
            MsList<mstr> xParams;
            MsBase::StringSplit(strSrc.c_str(), ':', xParams);
            if (xParams.GetCount() == 3)
            {
                MsgStruct msg;
                if (MsBase::stoi(xParams[0].c_str(), msg.ID))
                {
                    if (GetBuffFromBase64(xParams[1], msg.sBuffer))
                    {
                        msg.fTime = atof(xParams[2].c_str());
                        g_CommandList.Add(msg);
                    }
                }
            }
        }
    }
}

Boolean RobotApp::GetBuffFromBase64(mstr& strSrc, mstr& strResult)
{
    //LOG_TRACE("src[" << strSrc << "] len : " << strSrc.size());
    int maxLen = (Int32)strSrc.size() / 4 * 3;
    strResult.resize(maxLen);
    int tarIndex(0), srcIndex(0);
    int emptyNum(0);
    while (srcIndex < strSrc.size())
    {
        int iVal(0);
        for (int i = 0; i < 4; i++)
        {
            char cSrc = strSrc[srcIndex + i];
            int iNow(0);
            if (cSrc >= 'A' && cSrc <= 'Z')
            {
                iNow = cSrc - 'A';
            }
            else if (cSrc >= 'a' && cSrc <= 'z')
            {
                iNow = cSrc - 'a' + 26;
            }
            else if (cSrc >= '0' && cSrc <= '9')
            {
                iNow = cSrc - '0' + 52;
            }
            else if (cSrc == '+')
            {
                iNow = 62;
            }
            else if (cSrc == '/')
            {
                iNow = 63;
            }
            else if (cSrc == '=')
            {
                emptyNum++;
            }
            iVal = (iVal << 6) + iNow;
        }
        srcIndex += 4;
        for (int i = 2; i >= 0; i--)
        {
            int iNow = (iVal >> (i * 8)) & 0xFF;
            strResult[tarIndex] = iNow;
            tarIndex++;
        }
    }
    if (emptyNum > 0)
    {
        strResult.resize(maxLen - emptyNum);
    }
    return True;
}

