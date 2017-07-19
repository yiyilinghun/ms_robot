#pragma once

//断言日志文本最大长度
#define ASSERTX_STR_MAX_LEN     (8*MB_SIZE)

enum E_ASSERT_LEVEL
{
    AL_SHOWMSG, // 提示
    AL_NORMAL,  // 常规
    AL_CATAST,  // 灾难
};

void AsserMessage(E_ASSERT_LEVEL eLevel, LPSTR msg);
void ASSERT_LOG(LPCSTR szTemp);
void ASSERTEX(E_ASSERT_LEVEL eLevel, LPCSTR file, DWORD  line, LPCSTR func, LPCSTR expr, LPCSTR str, ...);

#ifdef Assert
#undef Assert
#endif

#define AssertLog(str,...)              ((ASSERTEX(AL_SHOWMSG, __FILE__, __LINE__, __FUNCTION__, "", str, __VA_ARGS__)))

#define Assert(level,expr,str,...)      (((expr) ? False : (ASSERTEX(level, __FILE__, __LINE__, __FUNCTION__, #expr, str, __VA_ARGS__), True)))
#define AssertMsg(expr,str,...)         Assert(AL_SHOWMSG,expr,str,__VA_ARGS__)
#define AssertNormal(expr,str,...)      Assert(AL_NORMAL,expr,str,__VA_ARGS__)
#define DAssertNormal(expr,str,...)     DebugCode(Assert(AL_NORMAL,expr,str,__VA_ARGS__))
#define AssertCatast(expr,str,...)      Assert(AL_CATAST,expr,str,__VA_ARGS__)

#define RetAssertMsg(expr,str,...)      if(Assert(AL_SHOWMSG,expr,str,__VA_ARGS__)){return False;}
#define RetAssertNormal(expr,str,...)   if(Assert(AL_NORMAL,expr,str,__VA_ARGS__)){return False;}
#define RetAssertCatast(expr,str,...)   if(Assert(AL_CATAST,expr,str,__VA_ARGS__)){return False;}

#define ENTER_TRY   try{
#define LEAVE_TRY   }catch(...){}
