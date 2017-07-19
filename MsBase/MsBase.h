#pragma once

#if defined (_DEBUG)
#define _HAS_ITERATOR_DEBUGGING 1
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !_WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef _WIN64
//#pragma message ("本代码库针对64位程序设计,32位仅可调试研究使用不可发布！")
#endif

#define CLIENT_CONFIG_NAME   "../../ClientConfig.xml"
//#define SERVER_CONFIG_NAME   "../../ServerConfig.xml"
//#define COMMON_CONFIG_NAME   "../../CommonConfig.xml"
//#define PJ_SERVER_NAME        "../../ServerConfig.xml"

#pragma warning(disable:4235)
#pragma warning(disable:4127)
#pragma warning(disable:4100)
#pragma warning(disable:4091)

// 去除包含msxml com组件的引用
#define __msxml_h__
#define __ISoftDistExt_INTERFACE_DEFINED__
#define __IPublishingWizard_INTERFACE_DEFINED__


#define SECURITY_WIN32
#include <SDKDDKVer.h>
#include <intsafe.h>
#include <WinSock2.h>
#include <windows.h>
#include <lm.h>
#include <Shlwapi.h >
#include <ws2tcpip.h>
#include <mswsock.h>
#include <winbase.h>
#include <Sspi.h>
#include <tcpmib.h>
#include <IPHlpApi.h>
#include <Psapi.h>
#include <tlhelp32.h>
//#include <llimits.h>
#pragma comment(lib, "Iphlpapi.lib")
#include <winnt.h>
#include <ShlObj.h>

#include <time.h>
#include <MMSystem.h>
#include <stdio.h>
#include <regex>

#include <math.h>
#include <iomanip>
#include <utility>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <istream>
#include <sstream>
#include <memory>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <shellapi.h>
#include <Dbghelp.h>
#include <winhttp.h>
#include <http.h>


#include <sql.h>
#include <sqlucode.h>

#pragma warning(push)
#pragma warning(disable:4127)
#include "./Proto/src/google/protobuf/stubs/strutil.h"
#pragma warning(pop)

#include "MsNetPing.h"

#define CountOf _countof

#ifdef _WIN64
#define MsInterlockedAdd InterlockedAdd64
#else
#define MsInterlockedAdd(_Addend_, _Value_) InterlockedExchangeAdd((LONG volatile *)_Addend_, _Value_)
#endif



#define BYTE_SIZE   (sizeof(Byte))
#define KB_SIZE     (BYTE_SIZE*1024)
#define MB_SIZE     (KB_SIZE*1024)
#define GB_SIZE     (MB_SIZE*1024)

#define USE_PARAM(...)(__VA_ARGS__)

#define DEFAULT_BUFF_SIZE (64*KB_SIZE)

#if defined(_DEBUG)
#include <crtdbg.h>
#define BEGIN_INIT _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //MsBase::MY_RAND_INIT();
#define END_CLEAR ::WSACleanup();google::protobuf::ShutdownProtobufLibrary(); //MsBase::MY_RAND_CLEAR();
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__, true)
#define DINLINE
#else
#define BEGIN_INIT //MsBase::MY_RAND_INIT()
#define END_CLEAR ::WSACleanup(); google::protobuf::ShutdownProtobufLibrary(); //MsBase::MY_RAND_CLEAR();
#define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__, true)
//#define NEW new
#define DINLINE inline
#endif

//#include "json/json.h"
//typedef Json::Reader        JsonReader;
//typedef Json::FastWriter    JsonWriter;
//typedef Json::Value         JsonValue;

// 根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)                      ((p) ? (delete (p), (p)=nullptr) : ((p) = (p)))
#endif

#ifndef SAFE_DETACH
#define SAFE_DETACH(p)                      ((p) ? (p->m_MsIOCPManager->DetachObjectAttachedDeputy(p->m_UnitIndex), (p) = nullptr) : NULL)
#define SAFE_DETACH_THIS                    (this ? (this->m_MsIOCPManager->DetachObjectAttachedDeputy(this->m_UnitIndex)) : NULL)
#define SAFE_DETACH_OBJ(o)                  ((o).m_MsIOCPManager->DetachObjectAttachedDeputy((o).m_UnitIndex))
#endif


// 根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)                ((p)?(delete[] (p),(p)=nullptr):((p)=(p)))
#endif

// 根据指针值释放对象
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)                     ((p)?((p)->Release(),(p)=nullptr):((p) = (p)))
#endif

// 根据迭代器释放内存
#ifndef BEGIN_ENUMERATOR

#define BEGIN_MULCONTAINER(key,container)       for (auto xIts = container.GetValues(key); xIts.first != container.GetEnd() && xIts.first != xIts.second; ++(xIts.first))
#define xMulKey                                 (xIts.first->first)
#define xMulValue                               (xIts.first->second)

#define xEnumValue                              (xContainer.Value())
#define xEnumKey                                (xContainer.Key())
//#define FAST_FOREACH(container)             {for (auto xEnumerator = (container).GetEnumerator(); xEnumerator.MoveNext();)
//#define FAST_FOREACH(container)             {for (auto xEnumerator = (container).GetEnumerator(); xEnumerator.MoveNext();)
//#define END_ENUMERATOR                          }
#define FOREACH_KEY_CALL(container,fun,...)     FAST_FOREACH(container){xEnumKey->fun(__VA_ARGS__);}
#define FOREACH_VALUE_CALL(container,fun,...)   FAST_FOREACH(container){xEnumValue->fun(__VA_ARGS__);}
#define ERASE_VALUE(container, value)           FAST_FOREACH(container) {if (value == xEnumValue){container.Erase(xEnumerator); break;}}
#define FOREACH_PROTO_REPEATED(repeated)        auto& _xTemp = repeated; for (Int32 i = 0; i < repeated->size(); i++)
#define xCurrentValue                           _xTemp->Mutable(i)
#define FOR_EACH_RANGE(it,pair_range)           for (auto it = pair_range.first; it != pair_range.second; ++it)

#endif

// 快速迭代容器
#ifndef FAST_FOREACH
#define FOREACH(container)                          for ((container).ToFirst(); (container).IsValid(); (container).DirectMoveNext())
#define NAME_FOREACH(name,container)                for (auto& name = (container).ForFirst(); name.IsValid(); name.DirectMoveNext())
#define FAST_FOREACH(container)                     for (auto& xContainer = (container).ForFirst(); xContainer.IsValid(); xContainer.DirectMoveNext())
#define SAFE_FOREACH(container)                     for (auto& xContainer = (container).ForFirst(); xContainer.IsValid(); xContainer.SafeMoveNext())

#define FOREACH_BACKWARD(container)                 for ((container).ToLast(); (container).IsValid(); (container).DirectMoveUp())
#define NAME_FOREACH_BACKWARD(name,container)       for (auto& name = (container).ForLast(); name.IsValid(); name.DirectMoveUp())
#define FAST_FOREACH_BACKWARD(container)            for (auto& xContainer = (container).ForLast(); xContainer.IsValid(); xContainer.DirectMoveUp())
#define SAFE_FOREACH_BACKWARD(container)            for (auto& xContainer = (container).ForLast(); xContainer.IsValid(); xContainer.SafeMoveUp())
#endif

// 根据迭代器释放内存
#ifndef SAFE_ENUMERATOR_DELETE
//#define SAFE_ENUMERATOR_DELETE__(container)         FAST_FOREACH(xEnumerator##container, container) {auto lpTemp = xEnumerator##container.Value(); SAFE_DELETE(lpTemp);} container.Clear();
//#define SAFE_ENUMERATOR_DELETE_ARRAY__(container)   FAST_FOREACH(xEnumerator##container, container) {auto lpTemp = xEnumerator##container.Value(); SAFE_DELETE_ARRAY(lpTemp);} container.Clear();
#define SAFE_ENUMERATOR_DELETE(container)           FAST_FOREACH(container) {auto lpTemp = container.Value(); SAFE_DELETE(lpTemp);} container.Clear();
#define SAFE_ENUMERATOR_DELETE_ARRAY(container)     FAST_FOREACH(container) {auto lpTemp = container.Value(); SAFE_DELETE_ARRAY(lpTemp);} container.Clear();
#endif

// 交换数据
#ifndef EXCHANGE_DATA
#define EXCHANGE_DATA(a,b)          (a) ^= (b), (b) ^= (a), (a) ^= (b);
#endif

#define ZeroObject(p)   ZeroMemory(&(p), sizeof(p))
#define ZeroArray(a)    ZeroMemory(&(a), sizeof(a))
#define ZeroThis        ZeroMemory(this, sizeof(*this))


// 进制宏定义
#define RADIX_2 2       // 2进制的宏定义
#define RADIX_8 8       // 8进制的宏定义
#define RADIX_10 10     // 10进制的宏定义
#define RADIX_16 16     // 16进制的宏定义

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          ((p) ? (delete (p), (p)=nullptr) : ((p) = (p)))
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    ((p)?(delete[] (p),(p)=nullptr):((p)=(p)))
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         ((p)?((p)->Release(),(p)=nullptr):((p) = (p)))
#endif
#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p)           ((p) ? (::CloseHandle(p), (p)=nullptr) : ((p) = (p)))
#endif

#define EXPORT_API __declspec(dllexport)

typedef BYTE    Byte;
typedef CHAR    Char;

typedef INT16   Int16;
typedef UINT16  UInt16;

typedef INT32   Int32;
typedef UINT32  UInt32;

typedef INT64   Int64;
typedef UINT64  UInt64;

typedef BOOL    Boolean;
#define True    TRUE
#define False   FALSE

typedef float   Single;
typedef double  Double;

typedef unsigned __int64 QWORD;

typedef HANDLE  IntPtr;

typedef std::wstring    wstr;
typedef std::string     mstr;
typedef std::string     utf8str;
#ifdef UNICODE
typedef std::wstring    tstr;
#else
typedef std::string     tstr;
#endif

#define SafePtr std::shared_ptr
#define AutoLeaveRun(p1)        SafePtr<void> xTemp##__LINE__(nullptr, [](void*) { p1 })
#define AutoLeaveRunThis(p1)    SafePtr<void> xTemp##__LINE__(nullptr, [this](void*) { p1 })
#define AutoBuff(buff, size)    SafePtr<BYTE> xTemp##__LINE__(NEW BYTE[size], [](LPBYTE x) { SAFE_DELETE_ARRAY(x); }); LPBYTE buff = xTemp##__LINE__.get()

template<typename T>
class AutoRelease
{
public:
    typedef void(*LP_AUTO_RELEASE_FUN1)(T xType);
    typedef void(*LP_AUTO_RELEASE_FUN2)(T& xType);
    DINLINE typedef AutoRelease(Boolean IsDefaultArray = False) : m_RealPtr(NULL), m_IsDefaultArray(IsDefaultArray), m_ReleaseFun1(nullptr), m_ReleaseFun2(nullptr) { ; }
    DINLINE typedef AutoRelease(LP_AUTO_RELEASE_FUN1 xFun) : m_RealPtr(NULL), m_ReleaseFun1(xFun), m_ReleaseFun2(nullptr) { ; }
    DINLINE typedef AutoRelease(LP_AUTO_RELEASE_FUN2 xFun) : m_RealPtr(NULL), m_ReleaseFun1(nullptr), m_ReleaseFun2(xFun) { ; }
    DINLINE typedef AutoRelease(T xType, LP_AUTO_RELEASE_FUN1 xFun) : m_RealPtr(xType), m_ReleaseFun1(xFun), m_ReleaseFun2(nullptr) { ; }
    DINLINE typedef AutoRelease(T xType, LP_AUTO_RELEASE_FUN2 xFun) : m_RealPtr(xType), m_ReleaseFun1(nullptr), m_ReleaseFun2(xFun) { ; }
    DINLINE ~AutoRelease()
    {
        if (m_ReleaseFun1)
        {
            this->m_ReleaseFun1(m_RealPtr); m_RealPtr = NULL;
        }
        else if (m_ReleaseFun2)
        {
            this->m_ReleaseFun2(m_RealPtr);
        }
        else
        {
            if (m_IsDefaultArray)
            {
                SAFE_DELETE_ARRAY(m_RealPtr);
            }
            else
            {
                SAFE_DELETE(m_RealPtr);
            }
        }
    }
    DINLINE void operator=(T xType)
    {
        if (m_RealPtr)
        {
            if (m_ReleaseFun1) { this->m_ReleaseFun1(m_RealPtr); m_RealPtr = NULL; }
            else if (m_ReleaseFun2) { this->m_ReleaseFun2(m_RealPtr); }
            else if (m_IsDefaultArray) { SAFE_DELETE_ARRAY(m_RealPtr); }
            else { SAFE_DELETE(m_RealPtr); }
        }
        m_RealPtr = xType;
    }

    DINLINE void operator=(T& xType)
    {
        if (m_RealPtr)
        {
            if (m_ReleaseFun1) { this->m_ReleaseFun1(m_RealPtr); m_RealPtr = NULL; }
            else if (m_ReleaseFun2) { this->m_ReleaseFun2(m_RealPtr); }
            else { SAFE_DELETE(m_RealPtr); }
        }
        m_RealPtr = xType;
    }

    DINLINE T& get() { return m_RealPtr; }
    DINLINE T& operator *() { return m_RealPtr; }
    DINLINE operator T() { return m_RealPtr; }
    DINLINE operator T&() { return m_RealPtr; }
public:
    LP_AUTO_RELEASE_FUN1 m_ReleaseFun1;
    LP_AUTO_RELEASE_FUN2 m_ReleaseFun2;
    Boolean m_IsDefaultArray;
    T m_RealPtr;
};

#ifdef _DEBUG
#define DebugCode(code) code
#define DebugStr(msg)       ::OutputDebugStringA(msg)
#define DebugStrf(msg,...)  ::OutputDebugStringA(MsBase::Format(msg,__VA_ARGS__).c_str())
#else
#define DebugCode(code)
#define DebugStr(msg)
#define DebugStrf(msg)
#endif

#ifdef NDEBUG
#define NDebugCode(code) code
#else
#define NDebugCode(code)
#endif

//无效的ID值
#define INVALID_LID     (Int64)-1
#define INVALID_QID     (QWORD)-1
#define INVALID_NID     -1
#define INVALID_UID     (DWORD)-1
#define INVALID_WID     (WORD)-1
#define INVALID_BID     (BYTE)-1
#define INVALID_KEY     (LPVOID)-1
#define INVALID_PUL     (ULONG_PTR)-1
#define INVALID_PNL     (LONG_PTR)-1

#define CURRENT_LOCALTIME(st) ::GetLocalTime(&(st))
#define CURRENT_FILETIME(ft)  {SYSTEMTIME temp_local_time; CURRENT_FILETIME(temp_local_time) ::SystemTimeToFileTime(&temp_local_time, &(ft));}


#include "MsAssertx.h"
#include "MD5.h"


#define INVALID_VALUE (-1)
#define INVALID_DVALUE (DWORD)(-1)
#define INVALID_PTR (IntPtr)(-1)



#pragma warning(disable:4291)
void* __CRTDECL operator new(size_t const size, int const block_use, char const* file_name, int const line_number, bool is);


#define UEP(ph, arglist) UEPHelper ph(arglist);
class UEPHelper
{
public: UEPHelper(va_list arglist) { m_arglist = arglist; }
public: template<typename T> operator T() { return va_arg(m_arglist, T); }
private:va_list m_arglist;
};

class MsLock
{
public:
    DINLINE MsLock(void) { ::InitializeCriticalSection(&m_Lock); }
    DINLINE ~MsLock(void) { ::DeleteCriticalSection(&m_Lock); }
    DINLINE void Lock(void) { ::EnterCriticalSection(&m_Lock); }
    DINLINE void UnLock(void) { ::LeaveCriticalSection(&m_Lock); }
private:
    CRITICAL_SECTION m_Lock;
};

class AutoLock
{
public:
    DINLINE AutoLock(MsLock& xMsLock) :m_lpMsLock(&xMsLock) { m_lpMsLock->Lock(); }
    DINLINE ~AutoLock(void) { m_lpMsLock->UnLock(); }
private:
    MsLock* m_lpMsLock;
};
#define AUTO_LOCK(lock) AutoLock AutoLock##__FILE__##__FUNCTION__##__LINE__##lock(lock)

#define MAX_SID_LEN 256
struct MsSid
{
    DINLINE MsSid()
    {
        m_SidLen = 0;
        ZeroMemory(m_SidData, sizeof(m_SidData));
    }

    DINLINE MsSid(PSID pSid)
    {
        m_SidLen = ::GetLengthSid(pSid);
        memcpy(m_SidData, pSid, m_SidLen);
    }

    DINLINE void operator=(PSID pSid)
    {
        m_SidLen = ::GetLengthSid(pSid);
        memcpy(m_SidData, pSid, m_SidLen);
    }

    DINLINE void operator=(DWORD Id)
    {
        m_Id = Id;
    }

    DINLINE bool operator==(const DWORD Id) const
    {
        return m_Id == Id;
    }

    DINLINE bool operator==(const PSID pSid) const
    {
        DWORD xSidLen = ::GetLengthSid(pSid);
        if (m_SidLen != xSidLen)
        {
            return false;
        }
        else
        {
            return memcmp(m_SidData, pSid, m_SidLen) == 0;
        }
    }

    DINLINE bool operator==(const MsSid xMsSid) const
    {
        if (m_SidLen != xMsSid.m_SidLen)
        {
            return false;
        }
        else
        {
            return memcmp(m_SidData, xMsSid.m_SidData, m_SidLen) == 0;
        }
    }

    DINLINE bool operator<(const MsSid xMsSid) const
    {
        if (m_SidLen < xMsSid.m_SidLen)
        {
            return true;
        }
        else if (m_SidLen == xMsSid.m_SidLen)
        {
            Int32 xInt64ComparisonCount = m_SidLen / sizeof(Int64);
            Int32 xByteComparisonCount = m_SidLen % sizeof(Int64);

            Int64* lpInt64Src = (Int64*)m_SidData;
            Int64* lpInt64Tar = (Int64*)xMsSid.m_SidData;
            for (Int32 i = 0; i < xInt64ComparisonCount; i++)
            {
                if ((*lpInt64Src) < (*lpInt64Tar))
                {
                    return true;
                }
                else if ((*lpInt64Src) == (*lpInt64Tar))
                {
                    lpInt64Src++;
                    lpInt64Tar++;
                    continue;
                }
                else
                {
                    return false;
                }
            }

            Byte* lpByteSrc = ((Byte*)m_SidData) + (xInt64ComparisonCount * sizeof(Int64));
            Byte* lpByteTar = ((Byte*)xMsSid.m_SidData) + (xInt64ComparisonCount * sizeof(Int64));
            for (Int32 i = 0; i < xByteComparisonCount; i++)
            {
                if ((*lpByteSrc) < (*lpByteTar))
                {
                    return true;
                }
                else if ((*lpByteSrc) == (*lpByteTar))
                {
                    lpByteSrc++;
                    lpByteTar++;
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }

    DINLINE LPCSTR GetBytesAddr() { return (LPCSTR)this; }

    DWORD m_Id;
    DWORD m_SidLen;
    Byte m_SidData[MAX_SID_LEN];
};

struct Int128
{
    DINLINE Int128()
    {
        Hi64 = 0;
        Lo64 = 0;
    }

    DINLINE Int128(LPCSTR lpNumberData)
    {
        Hi64 = *((Int64*)lpNumberData);
        Lo64 = *((Int64*)(lpNumberData + sizeof(Int64)));
    }

    DINLINE void operator=(LPCSTR lpNumberData)
    {
        Hi64 = *((Int64*)lpNumberData);
        Lo64 = *((Int64*)(lpNumberData + sizeof(Int64)));
    }

    DINLINE bool operator<(const Int128 xInt128) const
    {
        if (Hi64 < xInt128.Hi64)
        {
            return true;
        }
        else if (Hi64 == xInt128.Hi64)
        {
            return Lo64 < xInt128.Lo64;
        }
        else
        {
            return false;
        }
    }

    DINLINE LPCSTR GetBytesAddr() { return (LPCSTR)this; }

    Int64 Hi64;
    Int64 Lo64;
};

//产生浮点随机数
#define DOUBLE_RAND ((DOUBLE)rand())
#define RFREQUENCY 4096

// 根据新生成种子产生随机数
#define RADIXN  MsBase::MY_RAND()

// 根据种子在指定范围内随机ACHAR
#define R_BYTE(ri,ra)       (BYTE)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_BYTE(ri,ra)    R_BYTE((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机WORD
#define R_WORD(ri,ra)       (WORD)(RADIXN*((ra)-(ri))+(ri)) // 不包含最大值
#define RAND_WORD(ri,ra)    R_WORD((ri),((ra)+1))           // 包含最大值

//根据种子在指定范围内随机DWORD
#define R_DWORD(ri,ra)      (DWORD)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_DWORD(ri,ra)   R_DWORD((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机QWORD
#define R_QWORD(ri,ra)      (QWORD)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_QWORD(ri,ra)   R_QWORD((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机FLOAT
#define R_FLOAT(ri,ra)      (FLOAT)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_FLOAT(ri,ra)   R_FLOAT((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机DOUBLE
#define R_DOUBLE(ri,ra)     (DOUBLE)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_DOUBLE(ri,ra)  R_DOUBLE((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机INT32
#define R_INT32(ri,ra)      (INT32)(RADIXN*((ra)-(ri))+(ri))// 不包含最大值
#define RAND_INT32(ri,ra)   R_INT32((ri),((ra)+1))          // 包含最大值

//根据种子在指定范围内随机INT64
#define R_INT64(ri,ra)      (INT64)(RADIXN*(ra-ri)+(ri))    // 不包含最大值
#define RAND_INT64(ri,ra)   R_INT64((ri),((ra)+1))          // 包含最大值

//一定几率等于指定值
#define R_ODDS(ri,ra,va,vb) ((RADIXN*(ra))<=(ri))?(va):(vb)


typedef struct tagTHREADNAME_INFO
{
    DWORD   dwType;     // must be 0x1000 
    LPCSTR szName;     // pointer to name (in same addr space) 
    DWORD   dwThreadID; // thread ID (-1 caller thread) 
    DWORD   dwFlags;    // reserved for future use, most be zero 
}THREADNAME_INFO;

#include <functional>
#include "MsSet.hpp"
#include "MsUnorderedSet.hpp"
#include "MsList.hpp"
#include "MsVector.hpp"
#include "MsQueue.hpp"
#include "MsDictionary.hpp"
#include "MsUnorderedDictionary.hpp"
#include "MsMulDictionary.hpp"
#include "MsBlockDict.hpp"

class LogicUnit;
class IocpUnit;
struct iocp_base_ol;
struct MsTimer
{
public:
    MsTimer(IocpUnit* xIocpUnit, DWORD xEventId, Int64 xInterval, Int64 xParam, Boolean xInstantlyTrigger)
    {
        SYSTEMTIME st;
        ::GetLocalTime(&st);
        ::SystemTimeToFileTime(&st, (LPFILETIME)&m_LastTime);
        if (xInstantlyTrigger) { m_LastTime -= (Int64)xInterval * 1000i64 * 10000i64; }
        m_Interval = xInterval;
        m_EventId = xEventId;
        m_IocpUnit = xIocpUnit;
        m_Param = xParam;
        m_NextTriggerTime = m_LastTime + m_Interval * 10000i64;
    }

    bool operator<(MsTimer& xMsTimer)
    {
        if (m_EventId == xMsTimer.m_EventId)
        {
            if (m_LastTime == xMsTimer.m_LastTime)
            {
                if (m_Interval == xMsTimer.m_Interval)
                {
                    if (m_Param == xMsTimer.m_Param)
                    {
                        return false;
                    }
                    return m_Param > xMsTimer.m_Param;
                }
                return m_Interval > xMsTimer.m_Interval;
            }
            return m_LastTime > xMsTimer.m_LastTime;
        }
        return m_EventId > xMsTimer.m_EventId;
    }

    DWORD       m_EventId;
    Int64       m_LastTime;
    Int64       m_Interval;
    Int64       m_NextTriggerTime = 0;
    Int64       m_Param;
    IocpUnit*   m_IocpUnit = nullptr;
};

#define FFORMAT(b,f,...)    mstr b; MsBase::Format(b,f, __VA_ARGS__);


#define UD_PROTO_REPEATED_TYPE (google::protobuf::FieldDescriptor::Type)(google::protobuf::FieldDescriptor::Type::MAX_TYPE + 1)

class MsXmlReader;
class MsBase
{
public:

    static MsVector<mstr> GetMacAddr();

    DINLINE static BYTE ToHex(BYTE x)
    {
        return x > 9 ? x + 55 : x + 48;
    }

    DINLINE static BYTE FromHex(BYTE x)
    {
        unsigned char y;
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
        else if (x >= '0' && x <= '9') y = x - '0';
        else assert(0);
        return y;
    }

    DINLINE static mstr UrlEncode(const mstr& str)
    {
        mstr strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (isalnum((unsigned char)str[i]) ||
                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~'))
                strTemp += str[i];
            else if (str[i] == ' ')
                strTemp += "+";
            else
            {
                strTemp += '%';
                strTemp += ToHex((unsigned char)str[i] >> 4);
                strTemp += ToHex((unsigned char)str[i] % 16);
            }
        }
        return strTemp;
    }

    DINLINE static mstr UrlDecode(const mstr& str)
    {
        mstr strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (str[i] == '+') strTemp += ' ';
            else if (str[i] == '%')
            {
                assert(i + 2 < length);
                unsigned char high = FromHex((unsigned char)str[++i]);
                unsigned char low = FromHex((unsigned char)str[++i]);
                strTemp += high * 16 + low;
            }
            else strTemp += str[i];
        }
        return strTemp;
    }

    static Boolean CALLBACK GetCurrentThreadWnd_EnumWindowsCB(HWND hWnd, LPARAM lParam) { *((HWND*)lParam) = hWnd; return True; }
    DINLINE static HWND GetCurrentThreadWnd() { HWND hWnd; ::EnumThreadWindows(::GetCurrentThreadId(), GetCurrentThreadWnd_EnumWindowsCB, (LPARAM)&hWnd); return hWnd; }

    DINLINE static wstr GetFullPath(wstr xPath)
    {
        WCHAR szTemp[MAX_PATH * 10];
        ::GetFullPathNameW(xPath.c_str(), MAX_PATH * 10, szTemp, nullptr);
        return szTemp;
    }

    DINLINE static mstr GetFullPath(mstr xPath)
    {
        CHAR szTemp[MAX_PATH * 10];
        ::GetFullPathNameA(xPath.c_str(), MAX_PATH * 10, szTemp, nullptr);
        return szTemp;
    }

    DINLINE static mstr FillPathEndFlag(mstr xPath)
    {
        CHAR xChar = xPath.data()[xPath.size() - 1];
        if (xChar != '\\' && xChar != '/')
        {
            xPath += '\\';
        }
        return xPath;
    }

    DINLINE static Boolean stoi(LPCWSTR xSrc, OUT Int32& xNumber)
    {
        Int32 xTempNumber = _wtoi(xSrc);
        if (xTempNumber == 0 && xSrc[0] != L'0')
        {
            return False;
        }
        xNumber = xTempNumber;
        return True;
    }

    DINLINE static Boolean stoi(LPCSTR xSrc, OUT Int32& xNumber)
    {
        Int32 xTempNumber = ::atoi(xSrc);
        if (xTempNumber == 0 && xSrc[0] != L'0')
        {
            return False;
        }
        xNumber = xTempNumber;
        return True;
    }

    DINLINE static DWORD atox(LPCWSTR xSrc)
    {
        DWORD dwTemp;
        swscanf_s(xSrc, L"%x", &dwTemp);
        return dwTemp;
    }

    DINLINE static DWORD atox(LPCSTR xSrc)
    {
        DWORD dwTemp;
        sscanf_s(xSrc, "%x", &dwTemp);
        return dwTemp;
    }

    DINLINE static mstr GetMD5(mstr& xSrc)
    {
        return GETSTRMD5(xSrc);
    }

    DINLINE static mstr GetMD5(LPCVOID input, size_t length)
    {
        return GETDATAMD5(input, length);
    }

    DINLINE static mstr CreateZipPassword(MsVector<DWORD>& xList1, MsVector<DWORD>& xList2, MsVector<DWORD>& xList3, MsVector<DWORD>& xList4)
    {
        mstr xPassword;
        DWORD xTemp = 0;
        FAST_FOREACH(xList1)
        {
            xTemp += xList1.Value();
        }
        xPassword += MsBase::Format("%08x", xTemp);

        xTemp = 0;
        FAST_FOREACH(xList2)
        {
            xTemp += xList1.Value();
        }
        xPassword += MsBase::Format("%08x", xTemp);

        xTemp = 0;
        FAST_FOREACH(xList3)
        {
            xTemp += xList1.Value();
        }
        xPassword += MsBase::Format("%08x", xTemp);

        xTemp = 0;
        FAST_FOREACH(xList4)
        {
            xTemp += xList1.Value();
        }
        xPassword += MsBase::Format("%08x", xTemp);
        return xPassword;
    }

    DINLINE static void FastFillMemory(LPDWORD _Dst, DWORD _Val, DWORD _Count)
    {
        DWORD _Size = _Count * sizeof(DWORD);
        if (_Size > 1024)
        {
            DWORD aBackColor[16] = { _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val, _Val };
            DWORD xWholeMaxLen = (_Size) / sizeof(aBackColor);
            DWORD xOverMaxLen = ((_Size) % sizeof(aBackColor)) / sizeof(_Val);
            for (DWORD i = 0; i < xWholeMaxLen; i++)
            {
                CopyMemory(&(_Dst[(sizeof(aBackColor)*i) / sizeof(_Val)]), aBackColor, sizeof(aBackColor));
            }
            for (DWORD i = 0; i < xOverMaxLen; i++)
            {
                (_Dst + (sizeof(aBackColor)*xWholeMaxLen) / sizeof(_Val))[i] = _Val;
            }
        }
        else
        {
            for (DWORD i = 0; i < _Size; i++)
            {
                _Dst[i] = _Val;
            }
        }
    }

    DINLINE static Int64 StrReplace(wstr& s1, const wstr s2, const wstr s3, Boolean IsOnce = False)
    {
        Int64 xLastPos = INVALID_LID;
        wstr::size_type pos = 0;
        wstr::size_type a = s2.size();
        wstr::size_type b = s3.size();
        while ((pos = s1.find(s2, pos)) != wstr::npos)
        {
            xLastPos = (Int64)pos;
            s1.erase(pos, a);
            s1.insert(pos, s3);
            pos += b;
            if (IsOnce) { return xLastPos; }
            else { continue; }
        }
        return xLastPos;
    }

    DINLINE static Int64 StrReplace(mstr& s1, const mstr s2, const mstr s3, Boolean IsOnce = False)
    {
        Int64 xLastPos = INVALID_LID;
        mstr::size_type pos = 0;
        mstr::size_type a = s2.size();
        mstr::size_type b = s3.size();
        while ((pos = s1.find(s2, pos)) != mstr::npos)
        {
            xLastPos = (Int64)pos;
            s1.erase(pos, a);
            s1.insert(pos, s3);
            pos += b;
            if (IsOnce) { return xLastPos; }
            else { continue; }
        }
        return xLastPos;
    }

    DINLINE static Boolean RegexCheck(mstr xSrc, mstr xRegex)
    {
        std::regex xTempRegex(xRegex.c_str());
        std::smatch results;
        return regex_search(xSrc, results, xTempRegex);
    }

    template<typename TKey, typename TValue>
    DINLINE static Boolean Dict2Proto(MsUnorderedDictionary<TKey, TValue>& xDict, BaseProto& xProto)
    {
        FAST_FOREACH(xDict)
        {
            xProto.add_keys(xEnumKey);
            xProto.add_values(xEnumValue);
        }
        return True;
    }

    template<typename TKey, typename TValue>
    DINLINE static Boolean Proto2Dict(MsUnorderedDictionary<TKey, TValue>& xDict, BaseProto& xProto)
    {
        Int32 xCount = xProto.keys_size();
        for (Int32 i = 0; i < xCount; i++)
        {
            xDict[xProto.keys(i)] = xProto.values(i);
        }
        return True;
    }

    DINLINE static mstr ToString(Single xData) { FFORMAT(xRet, "%f", xData); return xRet; }
    DINLINE static mstr ToString(Double xData) { FFORMAT(xRet, "%lf", xData); return xRet; }
    DINLINE static mstr ToString(Int32 xData) { FFORMAT(xRet, "%d", xData); return xRet; }
    DINLINE static mstr ToString(Int64 xData) { FFORMAT(xRet, "%lld", xData); return xRet; }
    DINLINE static mstr ToString(UInt32 xData) { FFORMAT(xRet, "%u", xData); return xRet; }
    DINLINE static mstr ToString(UInt64 xData) { FFORMAT(xRet, "%llu", xData); return xRet; }
    DINLINE static mstr ToString(bool xData) { return xData ? "true" : "false"; }

    DINLINE static void Format(mstr& xOutTar, LPCSTR xFormat, ...)
    {
        CHAR szBaseFormat[KB_SIZE * 10];
        va_list ap;
        va_start(ap, xFormat);
        vsprintf_s(szBaseFormat, xFormat, ap);
        va_end(ap);
        xOutTar = szBaseFormat;
    }

    DINLINE static mstr Format(LPCSTR xFormat, ...)
    {
        CHAR szBaseFormat[KB_SIZE * 10];
        va_list ap;
        va_start(ap, xFormat);
        vsprintf_s(szBaseFormat, xFormat, ap);
        va_end(ap);
        return szBaseFormat;
    }

    DINLINE static Boolean FilePathExists(mstr xPath)
    {
        WIN32_FIND_DATAA fd;
        HANDLE hFind = ::FindFirstFileA(xPath.c_str(), &fd);
        BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE)) ? TRUE : FALSE;
        ::FindClose(hFind);
        return RetValue;
    }

    static MsUnorderedDictionary<DWORD, mstr>* m_lpDebugPacketCode;
    DINLINE static DWORD GetProtoIntKey(LPCSTR xBaseProtoName)
    {
        static MsUnorderedDictionary<DWORD, mstr> m_DebugPacketCode;
        m_lpDebugPacketCode = &m_DebugPacketCode;
        mstr xStr = GETSTRMD5(xBaseProtoName); LPSTR lpStr = (LPSTR)xStr.c_str();
        lpStr[8] = '\0';
        DWORD xKey;
        sscanf_s(lpStr, "%x", &xKey);
        m_DebugPacketCode[xKey] = xBaseProtoName;
        AssertLog("Proto2Int:[%s][%08x]", xBaseProtoName, xKey);
        return xKey;
    }

    DINLINE static DWORD GetProtoIntKey(BaseProto& xBaseProto)
    {
        return GetProtoIntKey(xBaseProto.GetTypeName().c_str());
    }

    DINLINE const static Boolean CheckPortUsedTCP(WORD wPort)
    {
        MIB_TCPTABLE TcpTable[KB_SIZE * 100];
        DWORD xSize = sizeof(TcpTable);
        if (NO_ERROR == ::GetTcpTable(TcpTable, &xSize, True))
        {
            DWORD xCount = TcpTable[0].dwNumEntries;
            if (xCount > 0)
            {
                for (DWORD i = 0; i < xCount; i++)
                {
                    MIB_TCPROW TcpRow = TcpTable[0].table[i];
                    DWORD temp1 = TcpRow.dwLocalPort;
                    int temp2 = temp1 / 256 + (temp1 % 256) * 256;
                    if (temp2 == wPort)
                    {
                        return True;
                    }
                }
            }
            return False;
        }
        return False;
    }

    static google::protobuf::FieldDescriptor::Type ConformityProtoType(google::protobuf::FieldDescriptor::Type xType);
    static mstr ConformityProtoTypeName(const google::protobuf::FieldDescriptor* xField);

    static Boolean IsEmptyUD(const universal_data& xUniversalData);

    static void Proto2UD(const google::protobuf::Message& xBaseProto, mstr xName, universal_data* xUniversalData);
    static void UD2Proto(const universal_data& xUniversalData, google::protobuf::Message* xBaseProto);

    static mstr MakeDefaultXmlRoot(LPCSTR xRootName, Int32& xOutSize);
    static void ProtoData2Xml(const google::protobuf::Message& xBaseProto, MsXmlReader& xMsXmlReader, Boolean xFillNoHasField);
    static void _ProtoMsg2Xml(const google::protobuf::Message& xBaseProto, MsXmlReader& xMsXmlReader, Boolean xFillNoHasField);

    static void UD2Xml(const universal_data& xUniversalData, MsXmlReader& xMsXmlReader);
    static void ShowUD(const universal_data& xUniversalData);

    static void ShowAllField(const google::protobuf::Message& xBaseProto, Boolean xFillNoHasField = True);

    static void ShowNotExistField(const google::protobuf::Message& xBaseProto)
    {
        ShowAllField(xBaseProto);
    }

    DINLINE static Double MY_RAND()
    {
        Double Radix;
        SYSTEMTIME st;
        ::GetLocalTime(&st);
        Radix = (DOUBLE_RAND + st.wMilliseconds + st.wSecond) / (RAND_MAX + DOUBLE_RAND / (RAND_MAX + st.wMilliseconds + st.wSecond));
        Radix *= 100;
        Radix = Radix - (Int64)Radix;
        return Radix;
    }

    DINLINE static DWORD GetCPUNumberOfProcessors()
    {
        SYSTEM_INFO si;
        ZeroObject(si);
        ::GetSystemInfo(&si);
        return si.dwNumberOfProcessors;
    }

    DINLINE static LPSTR STR_NEW_LPSZ(mstr xStr)
    {
        LPSTR szRet = NEW CHAR[xStr.size() + 1];
        strncpy_s(szRet, xStr.size() + 1, xStr.c_str(), xStr.size());
        return szRet;
    }

    DINLINE static void CheckAndCreatePath(mstr xPath) { return CheckAndCreatePath(MsBase::M2W(xPath)); }
    DINLINE static void CheckAndCreatePath(wstr xPath)
    {
        wstr::size_type pos = xPath.find(L'/');
        while (pos != wstr::npos)
        {
            xPath.replace(pos, 1, 1, L'\\');
            pos = xPath.find(L'/');
        }

        LPCWSTR lpPath = xPath.c_str();
        LPWSTR lpTemp = (LPWSTR)lpPath;
        while (lpTemp)
        {
            lpTemp = (LPWSTR)wcschr(lpTemp, L'\\');
            if (!lpTemp)
            {
                break;
            }
            else
            {
                *lpTemp = L'\0';
                if (!::PathFileExists(lpPath))
                {
                    ::CreateDirectory(lpPath, nullptr);
                }
                *lpTemp = L'\\';
                lpTemp++;
            }
        }
    }

    DINLINE static Int64 MysqlTime2FileTime(Int64 xMysqlTime)
    {
        Int64 xYear = xMysqlTime / 10000000000;
        xMysqlTime %= 10000000000;

        Int64 xMonth = xMysqlTime / 100000000;
        xMysqlTime %= 100000000;

        Int64 xDay = xMysqlTime / 1000000;
        xMysqlTime %= 1000000;

        Int64 xHour = xMysqlTime / 10000;
        xMysqlTime %= 10000;

        Int64 xMinute = xMysqlTime / 100;
        xMysqlTime %= 100;

        Int64 xSecond = xMysqlTime;

        SYSTEMTIME st;
        st.wYear = (WORD)xYear;
        st.wMonth = (WORD)xMonth;
        st.wDay = (WORD)xDay;
        st.wHour = (WORD)xHour;
        st.wMinute = (WORD)xMinute;
        st.wSecond = (WORD)xSecond;
        st.wMilliseconds = 0;
        st.wDayOfWeek = 0;

        Int64 xFileTime;
        ::SystemTimeToFileTime(&st, (LPFILETIME)&xFileTime);

        return xFileTime;
    }

    DINLINE static Int64 FileTime2MysqlTime(Int64 xFileTime)
    {
        SYSTEMTIME st;
        ::FileTimeToSystemTime((LPFILETIME)&xFileTime, &st);
        Int64 xYear = st.wYear;
        Int64 xMonth = st.wMonth;
        Int64 xDay = st.wDay;
        Int64 xHour = st.wHour;
        Int64 xMinute = st.wMinute;
        Int64 xSecond = st.wSecond;
        return xYear * 10000000000 + xMonth * 100000000 + xDay * 1000000 + xHour * 10000 + xMinute * 100 + xSecond;
    }

    DINLINE static Int64 Now()
    {
        Int64 xNow;
        SYSTEMTIME st;
        ::GetLocalTime(&st);
        ::SystemTimeToFileTime(&st, (LPFILETIME)&xNow);
        return xNow;
    }

    DINLINE static mstr FullExePathA()
    {
        CHAR szPath[MAX_PATH];
        ::GetModuleFileNameA(nullptr, szPath, MAX_PATH);
        CHAR* pLastSlash = strrchr(szPath, '\\');
        pLastSlash[0] = '\0';
        return szPath;
    }

    DINLINE static wstr FullExePathW()
    {
        WCHAR szPath[MAX_PATH];
        ::GetModuleFileNameW(nullptr, szPath, MAX_PATH);
        WCHAR* pLastSlash = wcsrchr(szPath, L'\\');
        pLastSlash[0] = L'\0';
        return szPath;
    }

    DINLINE static __time64_t FileTime2UnixTime(Int64 ft)
    {
        return (ft / 10000000) - 11644473600;
    }

    DINLINE static Int64 UnixTime2FileTime(__time64_t ut)
    {
        return (ut + 11644473600) * 10000000;
    }

    DINLINE static Boolean TimeSpanCheck(Int64 xNow, Int64& xLastTime, Int64 nTimeMillisecondLen)
    {
        if (xNow > xLastTime + nTimeMillisecondLen * 10000)
        {
            xLastTime = xNow;
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static Boolean TimeSpanCheckOnce(Int64 xTime, Int64 nTimeMillisecondLen)
    {
        if (MsBase::Now() > xTime + nTimeMillisecondLen * 10000)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static Boolean TimeSpanCheck(Int64& xLastTime, Int64 nTimeMillisecondLen)
    {
        Int64 xNow = MsBase::Now();
        if (xNow > xLastTime + nTimeMillisecondLen * 10000)
        {
            xLastTime = xNow;
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static Boolean TimeSpanCheck(Int64 xNow, MsTimer* xTimer)
    {
        if (xNow > xTimer->m_LastTime + xTimer->m_Interval * 10000)
        {
            xTimer->m_LastTime = xNow;
            xTimer->m_NextTriggerTime = xNow + (xTimer->m_Interval * 10000);
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static Boolean TimeSpanCheck(MsTimer* xTimer)
    {
        Int64 xNow = MsBase::Now();
        if (xNow > xTimer->m_LastTime + xTimer->m_Interval * 10000)
        {
            xTimer->m_LastTime = xNow;
            xTimer->m_NextTriggerTime = xNow + (xTimer->m_Interval * 10000);
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static Boolean TimeSpanCheck(MsTimer& xTimer)
    {
        Int64 xNow = MsBase::Now();
        if (xNow > xTimer.m_LastTime + xTimer.m_Interval * 10000)
        {
            xTimer.m_LastTime = xNow;
            xTimer.m_NextTriggerTime = xNow + (xTimer.m_Interval * 10000);
            return true;
        }
        else
        {
            return false;
        }
    }

    DINLINE static void TimeSpanCheckPressure(Int64& xLastTime, Int64& xPressure)
    {
        Int64 xNow = MsBase::Now();
        xPressure = (xNow - xLastTime) / 10000;
        xLastTime = xNow;
    }

    DINLINE static Boolean MsInetPtonA(mstr xAddr, sockaddr_in& xSockaddrIn, Int32& xAddrLen)
    {
#if defined(_WIN64)
        USE_PARAM(xAddrLen);
        if (!::InetPtonA(AF_INET, xAddr.c_str(), &xSockaddrIn.sin_addr.s_addr))
        {
            return False;
        }
#else
#pragma warning(disable:4996)
        if (SOCKET_ERROR == ::WSAStringToAddressA((LPSTR)xAddr.c_str(), AF_INET, nullptr, (LPSOCKADDR)&xSockaddrIn, &xAddrLen))
        {
            return False;
        }
#endif
        return True;
    }

    DINLINE static Boolean Ping(mstr xAddr)
    {
        MsNetPing xMsNetPing;
        return xMsNetPing.Ping(xAddr.c_str(), 1000);
    }

    DINLINE static void WaitAll(HANDLE* xList, Int32 xTotalCount)
    {
        if (xList)
        {
            Int32 xBatch = (xTotalCount / MAXIMUM_WAIT_OBJECTS) + 1;
            for (Int32 i = 0; i < xBatch; i++)
            {
                Int32 xNextCount = xTotalCount - (i * MAXIMUM_WAIT_OBJECTS);
                if (xNextCount > 0)
                {
                    xNextCount = xNextCount > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : xNextCount;
                    ::WaitForMultipleObjects(xNextCount, xList, True, INFINITE);
                    xList += xNextCount;
                }
            }
        }
    }

#ifdef _WIN64
    DINLINE static mstr GetIPFromHost(LPCSTR lpHost)
    {
        addrinfo* xAddrInfo = nullptr;
        getaddrinfo(lpHost, "", nullptr, &xAddrInfo);
        MsVector<mstr> xList;
        for (addrinfo* xCur = xAddrInfo; xCur != nullptr; xCur = xCur->ai_next)
        {
            CHAR ipbuf[512];
            sockaddr_in* xAddr = (struct sockaddr_in*)xCur->ai_addr;
            ::inet_ntop(AF_INET, &xAddr->sin_addr, ipbuf, 512);
            xList.Add(ipbuf);
        }
        freeaddrinfo(xAddrInfo);
        if (xList.GetCount() > 0) { return xList[0]; }
        return "";
    }
#else
#pragma warning(disable:4996)
    DINLINE static mstr GetIPFromHost(LPCSTR lpHost)
    {
        hostent* xHostent = gethostbyname(lpHost);
        if (xHostent->h_addr_list[0])
        {
            struct in_addr addr;
            addr.s_addr = *(LPDWORD)(xHostent->h_addr_list[0]);
            return inet_ntoa(addr);
    }
        return "";
}
#endif

    DINLINE static mstr dwIP2csIP(DWORD dwIP)
    {
        CHAR szBuff[100];
        WORD add1, add2, add3, add4;
        add1 = (WORD)(dwIP & 255);
        add2 = (WORD)((dwIP >> 8) & 255);
        add3 = (WORD)((dwIP >> 16) & 255);
        add4 = (WORD)((dwIP >> 24) & 255);
        sprintf_s(szBuff, "%d.%d.%d.%d", add1, add2, add3, add4);
        return szBuff;
    }

    //DINLINE static DWORD GetIPDwordFromHost(mstr lpHost)
    //{
    //    hostent* xHostent = gethostbyname(lpHost.c_str());
    //    if (xHostent->h_addr_list[0])
    //    {
    //        struct in_addr addr;
    //        addr.s_addr = *(LPDWORD)(xHostent->h_addr_list[0]);
    //        inet_ntoa(addr);
    //    }

    //    MsList<mstr> xIPList;

    //    USE_PARAM(xHostent);

    //    StringSplit(lpHost.c_str(), '.', xIPList);
    //    if (xIPList.GetCount() == 4)
    //    {
    //        DWORD xA = (DWORD)atoi(xIPList[0].c_str());
    //        DWORD xB = (DWORD)atoi(xIPList[1].c_str());
    //        DWORD xC = (DWORD)atoi(xIPList[2].c_str());
    //        DWORD xD = (DWORD)atoi(xIPList[3].c_str());
    //        xD <<= 24;
    //        xC <<= 16;
    //        xB <<= 8;
    //        return xA + xB + xC + xD;
    //    }
    //    return INVALID_DVALUE;
    //}

    DINLINE static mstr GetLastErrorStringA()
    {
        CHAR xMsgBuf[KB_SIZE];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static mstr GetLastErrorStringA(Int32 xErrorCode)
    {
        CHAR xMsgBuf[KB_SIZE];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, xErrorCode, 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static mstr GetWSALastErrorStringA()
    {
        CHAR xMsgBuf[KB_SIZE];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(), 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static mstr GetWSALastErrorStringA(Int32 xErrorCode)
    {
        CHAR xMsgBuf[KB_SIZE];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, xErrorCode, 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static void MsFormatMessageBoxA(LPSTR szFormatCaption, LPSTR lpFormatText, ...)
    {
        va_list arglist;
        va_start(arglist, lpFormatText);

        CHAR szText[KB_SIZE * 100] = { 0 };
        vsprintf_s(szText, lpFormatText, arglist);
        CHAR szCaption[KB_SIZE * 100] = { 0 };
        vsprintf_s(szCaption, szFormatCaption, arglist);

        ::MessageBoxA(nullptr, szText, szCaption, MB_OK);
    }













    DINLINE static wstr GetLastErrorStringW()
    {
        WCHAR xMsgBuf[KB_SIZE];
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static wstr GetLastErrorStringW(Int32 xErrorCode)
    {
        WCHAR xMsgBuf[KB_SIZE];
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, xErrorCode, 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static wstr GetWSALastErrorStringW()
    {
        WCHAR xMsgBuf[KB_SIZE];
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(), 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static wstr GetWSALastErrorStringW(Int32 xErrorCode)
    {
        WCHAR xMsgBuf[KB_SIZE];
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, xErrorCode, 0, xMsgBuf, KB_SIZE, nullptr);
        return xMsgBuf;
    }

    DINLINE static void MsFormatMessageBoxW(LPWSTR szFormatCaption, LPWSTR lpFormatText, ...)
    {
        va_list arglist;
        va_start(arglist, lpFormatText);

        WCHAR szText[KB_SIZE * 100] = { 0 };
        vswprintf_s(szText, lpFormatText, arglist);
        WCHAR szCaption[KB_SIZE * 100] = { 0 };
        vswprintf_s(szCaption, szFormatCaption, arglist);

        ::MessageBoxW(nullptr, szText, szCaption, MB_OK);
    }

    //static MsLock ms_DumpLock;
    DINLINE static void TryDump(DWORD xExceptionCode, LPEXCEPTION_POINTERS lpExceptionPointers)
    {
        USE_PARAM(xExceptionCode, lpExceptionPointers);
        //        AUTO_LOCK(ms_DumpLock);
        //
        //        WCHAR szTime[100] = { 0 };
        //        SYSTEMTIME st;
        //        ::GetLocalTime(&st);
        //        Int64 xNow = 0;
        //        ::SystemTimeToFileTime(&st, (LPFILETIME)&xNow);
        //        swprintf_s(szTime, 100, L"%lld_%u%d.dmp", xNow, xExceptionCode, ::GetCurrentThreadId());
        //
        //        HANDLE hDumpFile = ::CreateFile(szTime, GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
        //        if (hDumpFile)
        //        {
        //            MINIDUMP_EXCEPTION_INFORMATION xMinidumpExceptionInfo;
        //            xMinidumpExceptionInfo.ThreadId = ::GetCurrentThreadId();
        //            xMinidumpExceptionInfo.ExceptionPointers = lpExceptionPointers;
        //            xMinidumpExceptionInfo.ClientPointers = True;
        //#ifdef _DEBUG
        //            ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory, &xMinidumpExceptionInfo, nullptr, nullptr);
        //#else
        //            ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, MiniDumpFilterMemory, &xMinidumpExceptionInfo, nullptr, nullptr);
        //#endif
        //            ::CloseHandle(hDumpFile);
        //        }
    }

    DINLINE static void StringSplit(LPCWSTR lpSrc, WCHAR xSeparator, MsVector<wstr>& xParams)
    {
        LPCWSTR lpParam = (LPCWSTR)lpSrc;
        LPWSTR lpTemp = (LPWSTR)wcschr(lpParam, xSeparator);
        while (lpTemp)
        {
            WCHAR x = *lpTemp;
            (*lpTemp) = L'\0';
            if ((*lpParam) != L'\0')
            {
                xParams.Add(lpParam);
            }
            (*lpTemp) = x;
            lpParam = lpTemp + 1;
            lpTemp = (LPWSTR)wcschr(lpParam, xSeparator);
        }
        if ((*lpParam) != L'\0')
        {
            xParams.Add(lpParam);
        }
    }

    DINLINE static void StringSplit(LPCWSTR lpSrc, WCHAR xSeparator, MsList<wstr>& xParams)
    {
        LPCWSTR lpParam = (LPCWSTR)lpSrc;
        LPWSTR lpTemp = (LPWSTR)wcschr(lpParam, xSeparator);
        while (lpTemp)
        {
            WCHAR x = *lpTemp;
            (*lpTemp) = L'\0';
            if ((*lpParam) != L'\0')
            {
                xParams.Add(lpParam);
            }
            (*lpTemp) = x;
            lpParam = lpTemp + 1;
            lpTemp = (LPWSTR)wcschr(lpParam, xSeparator);
        }
        if ((*lpParam) != L'\0')
        {
            xParams.Add(lpParam);
        }
    }

    DINLINE static void StringSplit(LPCSTR lpCSrc, CHAR xSeparator, MsVector<mstr>& xParams)
    {
        mstr xTemp = lpCSrc; LPSTR lpSrc = (LPSTR)xTemp.c_str();
        LPSTR lpParam = (LPSTR)lpSrc;
        LPSTR lpTemp = strchr(lpParam, xSeparator);
        while (lpTemp)
        {
            CHAR x = *lpTemp;
            (*lpTemp) = '\0';
            if ((*lpParam) != '\0')
            {
                xParams.Add(lpParam);
            }
            (*lpTemp) = x;
            lpParam = lpTemp + 1;
            lpTemp = strchr(lpParam, xSeparator);
        }
        if ((*lpParam) != '\0')
        {
            xParams.Add(lpParam);
        }
    }

    DINLINE static void StringSplit(LPCSTR lpSrc, CHAR xSeparator, MsList<mstr>& xParams)
    {
        LPSTR lpParam = (LPSTR)lpSrc;
        LPSTR lpTemp = strchr(lpParam, xSeparator);
        while (lpTemp)
        {
            CHAR x = *lpTemp;
            (*lpTemp) = '\0';
            if ((*lpParam) != '\0')
            {
                xParams.Add(lpParam);
            }
            (*lpTemp) = x;
            lpParam = lpTemp + 1;
            lpTemp = strchr(lpParam, xSeparator);
        }
        if ((*lpParam) != '\0')
        {
            xParams.Add(lpParam);
        }
    }

    DINLINE static wstr M2W(CONST mstr& xmstr)
    {
        WCHAR szBuff[KB_SIZE * 100] = { 0 };
        ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)xmstr.c_str(), (Int32)xmstr.length(), szBuff, (Int32)xmstr.length());
        return wstr(szBuff);
    }

    DINLINE static mstr& RemoveAllChar(mstr& xSrc, CHAR xChar)
    {
        xSrc.erase(std::remove(xSrc.begin(), xSrc.end(), xChar), xSrc.end());
        return xSrc;
    }

    DINLINE static wstr& RemoveAllChar(wstr& xSrc, WCHAR xChar)
    {
        xSrc.erase(std::remove(xSrc.begin(), xSrc.end(), xChar), xSrc.end());
        return xSrc;
    }

    DINLINE static mstr W2M(CONST wstr& xwstr)
    {
        CHAR szBuff[KB_SIZE * 100] = { 0 };
        ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)xwstr.c_str(), (Int32)xwstr.length(), szBuff, (Int32)xwstr.length()*(sizeof(WCHAR) / sizeof(CHAR)), nullptr, nullptr);
        return mstr(szBuff);
    }

    DINLINE static Boolean Str2Proto(BaseProto& xProto, mstr xStr)
    {
        return xProto.ParseFromString(xStr);
    }

    DINLINE static mstr Proto2Str(BaseProto& xProto)
    {
        mstr xStr;
        xProto.SerializeToString(&xStr);
        return xStr;
    }

    DINLINE static DWORD IP2Int(LPCSTR xIp)
    {
        MsVector<mstr> xParamList;
        MsBase::StringSplit(xIp, '.', xParamList);
        if (xParamList.GetCount() == 4)
        {
            DWORD x0 = atoi(xParamList[0].c_str());
            DWORD x1 = atoi(xParamList[1].c_str());
            DWORD x2 = atoi(xParamList[2].c_str());
            DWORD x3 = atoi(xParamList[3].c_str());
            return x3 + (x2 << 8) + (x1 << 16) + (x0 << 24);
        }
        return INVALID_UID;
    }

    DINLINE static mstr Int2IP(DWORD xIp)
    {
        DWORD x0 = xIp & 0xFF000000;
        DWORD x1 = xIp & 0x00FF0000;
        DWORD x2 = xIp & 0x0000FF00;
        DWORD x3 = xIp & 0x000000FF;
        CHAR szBuff[100];
        sprintf_s(szBuff, "%d.%d.%d.%d", x0 >> 24, x1 >> 16, x2 >> 8, x3);
        return szBuff;
    }


    DINLINE static Int64 IPPP2Int(LPCSTR xIp, WORD xPortA, WORD xPortB)
    {
        Int64 xIntIpA = xPortA;
        xIntIpA <<= 48;

        Int64 xIntIpB = xPortB;
        xIntIpB <<= 32;

        Int64 xIntIp = xIntIpA + xIntIpB;
        xIntIp += IP2Int(xIp);
        return xIntIp;
    }

    DINLINE static mstr Int2IPPP(Int64 xIp, WORD& xPortA, WORD& xPortB)
    {
        Int64 xTempPort = xIp & 0xFFFF000000000000;
        xPortA = (WORD)(xTempPort >> 48);

        xTempPort = xIp & 0x0000FFFF00000000;
        xPortB = (WORD)(xTempPort >> 32);

        DWORD xIntIp = (DWORD)(xIp & 0x00000000FFFFFFFF);
        return Int2IP(xIntIp);
    }

    DINLINE static Int64 IPP2Int(LPCSTR xIp, WORD xPort)
    {
        Int64 xIntIp = 0;
        MsVector<mstr> xParamList;
        MsBase::StringSplit(xIp, '.', xParamList);
        if (xParamList.GetCount() == 4)
        {
            Int64 x0 = atoi(xParamList[0].c_str());
            xIntIp += x0 * 100000000000000;

            Int64 x1 = atoi(xParamList[1].c_str());
            xIntIp += x1 * 100000000000;

            Int64 x2 = atoi(xParamList[2].c_str());
            xIntIp += x2 * 100000000;

            Int64 x3 = atoi(xParamList[3].c_str());
            xIntIp += x3 * 100000;
        }
        xIntIp += xPort;
        return xIntIp;
    }

    DINLINE static mstr Int2IPP(Int64 xIp, WORD& xPort)
    {
        Int64 x0 = xIp / 100000000000000;
        xIp %= 100000000000000;

        Int64 x1 = xIp / 100000000000;
        xIp %= 100000000000;

        Int64 x2 = xIp / 100000000;
        xIp %= 100000000;

        Int64 x3 = xIp / 100000;
        xIp %= 100000;

        xPort = (WORD)xIp;

        CHAR szBuff[100];
        sprintf_s(szBuff, "%lld.%lld.%lld.%lld", x0, x1, x2, x3);
        return szBuff;
    }

    static mstr GetUserSID();

    DINLINE static mstr CI2SI(Int64 xClusterIndex);
    DINLINE static Int64 SI2CI(mstr xShowIndex, Int32 xBeginPos);

    DINLINE static mstr MADDC(CONST mstr& xwstr, CHAR c)
    {
        CHAR szBuff[KB_SIZE * 100] = { 0 };
        strncpy_s(szBuff, xwstr.c_str(), xwstr.size());
        Int32 xSize = (Int32)strlen(szBuff);
        szBuff[xSize] = c;
        szBuff[xSize + 1] = 0;
        return szBuff;
    }

    DINLINE static wstr WADDC(CONST wstr& xwstr, WCHAR c)
    {
        WCHAR szBuff[KB_SIZE * 100] = { 0 };
        wcsncpy_s(szBuff, xwstr.c_str(), xwstr.size());
        Int32 xSize = (Int32)wcslen(szBuff);
        szBuff[xSize] = c;
        szBuff[xSize + 1] = 0;
        return szBuff;
    }

    DINLINE static utf8str M2U8(mstr& xmstr) { return MsBase::W2U8(MsBase::M2W(xmstr)); }
    DINLINE static mstr U82M(utf8str& xu8str) { return MsBase::W2M(MsBase::U82W(xu8str)); }
    DINLINE static utf8str W2U8(wstr& xwstr)
    {
        CHAR szRes[KB_SIZE * 100];
        Int32 xSize = ::WideCharToMultiByte(CP_UTF8, 0, xwstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        ::WideCharToMultiByte(CP_UTF8, 0, xwstr.c_str(), -1, szRes, xSize, nullptr, nullptr);
        return utf8str(szRes);
    }

    DINLINE static wstr U82W(mstr xmstr)
    {
        WCHAR szRes[KB_SIZE * 100];
        Int32 xSize = ::MultiByteToWideChar(CP_UTF8, 0, xmstr.c_str(), -1, nullptr, 0);
        ::MultiByteToWideChar(CP_UTF8, 0, xmstr.c_str(), -1, szRes, xSize);
        return wstr(szRes);
    }

#define MS_VC_EXCEPTION 0x406d1388 
    DINLINE static void SetThreadName(LPCSTR xNewThreadName)
    {
        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = xNewThreadName;
        info.dwThreadID = ::GetCurrentThreadId();
        info.dwFlags = 0;
        __try { ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info); }
        __except (EXCEPTION_CONTINUE_EXECUTION) {}
    }
};

#define DECLARE_GLOBAL_OBJECT_POINTER(type, pointer_name) extern type* pointer_name; extern type* GET_##pointer_name(); extern void FREE_##pointer_name();
#define DEF_GLOBAL_OBJECT_POINTER(type, pointer_name) type* pointer_name = nullptr; type* GET_##pointer_name(){if (!pointer_name) { pointer_name = NEW type(); } return pointer_name;} void FREE_##pointer_name() { SAFE_DELETE(pointer_name); }

struct StaticData { StaticData(); ~StaticData(); }; extern StaticData g_StaticData;
#define BEFORE_MAIN_RUN StaticData g_StaticData; StaticData::StaticData()
#define AFTER_MAIN_RUN  StaticData::~StaticData()


#include "Resource.h"
#include "MsService.h"
#include "MsThread.h"
#include "MsRegistry.h"
#include "MsBaseService.h"
#include "MsMemoryStream.hpp"
#include "MsLuaInterface.h"

#include "MsJson.h"

#include "MsIOCPUnit.h"

#include "MsNetPacket.h"
#include "MsNetTerminal.h"
#include "MsNetClient.h"
#include "MsNetServer.h"

#include "MsWebServer.h"
#include "MsWebClient.h"

#include "MsXmlReader.h"
#include "TinyXml.h"
#include "MsAccessDB.h"
#include "MsNTFS.h"
#include "UD_Base.h"

#include "MsIOCPDef.h"
#include "MsIOCP_OL.h"
#include "MsIOCPManager.h"
#include "MsClusterNode.h"

#include "RenderManagerDef.h"
#include "RenderManagerRect.h"
#include "RenderManager_Sprite.h"
#include "RenderManager.h"

#include "ZipFunction.h"
