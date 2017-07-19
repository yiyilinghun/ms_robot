#include "Precompiled.h"

mstr MsRegistry::MsRegistry::GetRegistryStringA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
    HKEY hkResult = nullptr;
    Byte xData[DEFAULT_REGISTRY_VALUE_LEN] = { 0 };
    if (ERROR_SUCCESS == ::RegOpenKeyExA(hKey, lpSubKey, 0, KEY_READ, &hkResult))
    {
        DWORD xLen = DEFAULT_REGISTRY_VALUE_LEN;
        DWORD xType = REG_SZ;
        if (ERROR_SUCCESS == ::RegQueryValueExA(hkResult, lpValueName, nullptr, &xType, xData, &xLen))
        {
            ::RegCloseKey(hkResult);
            return mstr((LPSTR)xData);
        }
    }
    //AssertLog(False, MsBase::W2M(MsBase::GetLastErrorString()).c_str());
    return "";
}

void MsRegistry::MsRegistry::SetRegistryStringA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue)
{
    HKEY hkResult = nullptr;
    if (ERROR_SUCCESS == ::RegOpenKeyExA(hKey, lpSubKey, 0, KEY_WRITE, &hkResult))
    {
        DWORD xType = REG_SZ;
        if (ERROR_SUCCESS == ::RegSetValueExA(hkResult, lpValueName, 0, xType, (LPBYTE)lpValue, (DWORD)strlen(lpValue)))
        {
            ::RegCloseKey(hkResult);
        }
    }
}

wstr MsRegistry::MsRegistry::GetRegistryStringW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName)
{
    HKEY hkResult = nullptr;
    Byte xData[DEFAULT_REGISTRY_VALUE_LEN] = { 0 };
    if (ERROR_SUCCESS == ::RegOpenKeyExW(hKey, lpSubKey, 0, KEY_READ, &hkResult))
    {
        DWORD xLen = DEFAULT_REGISTRY_VALUE_LEN;
        DWORD xType = REG_SZ;
        if (ERROR_SUCCESS == ::RegQueryValueExW(hkResult, lpValueName, nullptr, &xType, xData, &xLen))
        {
            ::RegCloseKey(hkResult);
            return wstr((LPWSTR)xData);
        }
    }
    //AssertLog(False, MsBase::W2M(MsBase::GetLastErrorString()).c_str());
    return L"";
}

DWORD MsRegistry::GetRegistryDWORD(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
    HKEY hkResult = nullptr;
    DWORD xValue;
    if (ERROR_SUCCESS == ::RegOpenKeyExA(hKey, lpSubKey, 0, KEY_READ, &hkResult))
    {
        DWORD xLen = sizeof(xValue);
        DWORD xType = REG_DWORD;
        if (ERROR_SUCCESS == ::RegQueryValueExA(hkResult, lpValueName, nullptr, &xType, (LPBYTE)&xValue, &xLen))
        {
            ::RegCloseKey(hkResult);
            return xValue;
        }
    }
    //AssertLog(False, MsBase::W2M(MsBase::GetLastErrorString()).c_str());
    return INVALID_UID;
}


Boolean MsRegistry::GetRegistryBool(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName)
{
    if (GetRegistryDWORD(hKey, lpSubKey, lpValueName) == 1)
    {
        return True;
    }
    else
    {
        return False;
    }
}
