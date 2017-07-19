#include "Precompiled.h"

#if defined(_WIN64)
#pragma comment(lib, R"(..\MsBase\HGE\Lib\x64\d3d9.lib)")
#pragma comment(lib, R"(..\MsBase\HGE\Lib\x64\d3dx9.lib)")
#else
#pragma comment(lib, R"(..\MsBase\HGE\Lib\x86\d3d9.lib)")
#pragma comment(lib, R"(..\MsBase\HGE\Lib\x86\d3dx9.lib)")
#endif

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "httpapi.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")

#pragma comment(lib, "Protobuf.lib")
#pragma comment(lib, "ProtoC.lib")
#pragma comment(lib, "MsLib.lib")
#pragma comment(lib, "Common.lib")
