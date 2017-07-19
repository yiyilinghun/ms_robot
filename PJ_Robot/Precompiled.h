#pragma once
#define __PJ_ROBOT__

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类

#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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
