#pragma once                        //┃
// ━━━━━━━━━━━━━━━━ //┃
//文件名称：MsRegistry.h            //┃
//功能描述：注册表                  //┃
//平台相关：Windows                 //┃
//━━━━━━━━━━━━━━━━━━┛

#define DEFAULT_REGISTRY_VALUE_LEN 1024

class MsRegistry
{
public:
    // HKEY_CLASSES_ROOT
    // HKEY_CURRENT_CONFIG
    // HKEY_CURRENT_USER
    // HKEY_LOCAL_MACHINE
    // HKEY_USERS
    static mstr MsRegistry::GetRegistryStringA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName);
    static void MsRegistry::SetRegistryStringA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue);

    static wstr MsRegistry::GetRegistryStringW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName);

    static DWORD MsRegistry::GetRegistryDWORD(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName);

    static Boolean GetRegistryBool(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName);
};
