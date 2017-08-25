
// MFC_Robot.h : MFC_Robot Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������Precompiled.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// RobotApp:
// �йش����ʵ�֣������ MFC_Robot.cpp
//

extern MsList<MsgStruct> g_CommandList;

class RobotApp : public CWinAppEx
{
public:
    RobotApp();


    // ��д
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // ʵ��
    UINT  m_nAppLook;
    BOOL  m_bHiColorIcons;

    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();


    void LoadCommand(mstr xCommandFileName);
    Boolean GetBuffFromBase64(std::string& strSrc, std::string& strResult);

    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern RobotApp theApp;
