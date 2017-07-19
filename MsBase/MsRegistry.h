#pragma once                        //��
// �������������������������������� //��
//�ļ����ƣ�MsRegistry.h            //��
//����������ע���                  //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

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
