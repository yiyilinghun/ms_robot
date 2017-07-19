#pragma once
#define __PJ_ROBOT__

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����

#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4307)
#pragma warning(disable:4127)
#pragma warning(disable:4996)
#include "EnumDef.pb.h"
#include "Common.pb.h"
#include "Server.pb.h"
#include "NetPacket.pb.h"
#include "Battle.pb.h"
#include "Database.pb.h"
#include "Mail.pb.h"
typedef google::protobuf::Message   BaseProto;
typedef google::protobuf::Message*  LPBASEPROTO;
#pragma warning(pop)

#include "MsBase.h"

//// Hge
//#include "..\HGE\hge.h"
//#include "..\HGE\hgefader.h"
//#include "..\HGE\hgesprite.h"
//#include "..\HGE\hgegui.h"
//#include "..\HGE\hgeguictrls.h"

#ifndef var
#define var auto
#endif

#include "CommonResDef.h"
#include "PJConfig.h"

#define DLL_EXPORTS __declspec(dllimport)

#include "RobotPlayer.h"
#include "ZipFunction.h"

#include "MainFrm.h"
#include "RobotApp.h"
#include "ChildFrm.h"
#include "RobotDoc.h"
#include "RobotView.h"

extern MsList<Int32> g_FaceId;
