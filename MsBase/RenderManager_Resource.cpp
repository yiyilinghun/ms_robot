#include "Precompiled.h"
#include "ZLIB\unzip.h"
#pragma warning(disable:4996)

Boolean RenderManager::Resource_AttachPack(LPCSTR filename, LPCSTR password)
{
    LPSTR szName;
    CResourceList *resItem = m_Res;
    unzFile zip;

    szName = Resource_MakePath(filename);
    _strupr(szName);

    while (resItem)
    {
        if (!strcmp(szName, resItem->filename)) return False;
        resItem = resItem->next;
    }

    zip = unzOpen(szName);
    if (!zip) return False;
    unzClose(zip);

    resItem = NEW CResourceList;
    strcpy(resItem->filename, szName);
    if (password) strcpy(resItem->password, password);
    else resItem->password[0] = 0;
    resItem->next = m_Res;
    m_Res = resItem;

    return True;
}

void RenderManager::Resource_RemovePack(LPCSTR filename)
{
    LPSTR szName;
    CResourceList *resItem = m_Res, *resPrev = 0;

    szName = Resource_MakePath(filename);
    _strupr(szName);

    while (resItem)
    {
        if (!strcmp(szName, resItem->filename))
        {
            if (resPrev) resPrev->next = resItem->next;
            else m_Res = resItem->next;
            delete resItem;
            break;
        }

        resPrev = resItem;
        resItem = resItem->next;
    }
}

void RenderManager::Resource_RemoveAllPacks()
{
    CResourceList *resItem = m_Res, *resNextItem;

    while (resItem)
    {
        resNextItem = resItem->next;
        delete resItem;
        resItem = resNextItem;
    }

    m_Res = 0;
}

void* RenderManager::Resource_Load(LPCSTR filename, LPDWORD size)
{
    static LPSTR res_err = "Can't load resource: %s";

    CResourceList *resItem = m_Res;
    char szName[_MAX_PATH];
    char szZipName[_MAX_PATH];
    unzFile zip;
    unz_file_info file_info;
    int done, i;
    LPVOID ptr;
    HANDLE hF;

    if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':') goto _fromfile; // skip absolute paths

    // Load from pack

    strcpy(szName, filename);
    _strupr(szName);
    for (i = 0; szName[i]; i++) { if (szName[i] == '/') szName[i] = '\\'; }

    while (resItem)
    {
        zip = unzOpen(resItem->filename);
        done = unzGoToFirstFile(zip);
        while (done == UNZ_OK)
        {
            unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), nullptr, 0, nullptr, 0);
            _strupr(szZipName);
            for (i = 0; szZipName[i]; i++) { if (szZipName[i] == '/') szZipName[i] = '\\'; }
            if (!strcmp(szName, szZipName))
            {
                if (unzOpenCurrentFilePassword(zip, resItem->password[0] ? resItem->password : 0) != UNZ_OK)
                {
                    unzClose(zip);
                    sprintf(szName, res_err, filename);
                    _PostError(szName);
                    return 0;
                }

                ptr = malloc(file_info.uncompressed_size);
                if (!ptr)
                {
                    unzCloseCurrentFile(zip);
                    unzClose(zip);
                    sprintf(szName, res_err, filename);
                    _PostError(szName);
                    return 0;
                }

                if (unzReadCurrentFile(zip, ptr, file_info.uncompressed_size) < 0)
                {
                    unzCloseCurrentFile(zip);
                    unzClose(zip);
                    free(ptr);
                    sprintf(szName, res_err, filename);
                    _PostError(szName);
                    return 0;
                }
                unzCloseCurrentFile(zip);
                unzClose(zip);
                if (size) *size = file_info.uncompressed_size;
                return ptr;
            }

            done = unzGoToNextFile(zip);
        }

        unzClose(zip);
        resItem = resItem->next;
    }

    // Load from file
_fromfile:

    hF = CreateFileA(Resource_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, nullptr);
    if (hF == INVALID_HANDLE_VALUE)
    {
        sprintf(szName, res_err, filename);
        _PostError(szName);
        return 0;
    }
    file_info.uncompressed_size = GetFileSize(hF, nullptr);
    ptr = malloc(file_info.uncompressed_size);
    if (!ptr)
    {
        CloseHandle(hF);
        sprintf(szName, res_err, filename);
        _PostError(szName);
        return 0;
    }
    if (ReadFile(hF, ptr, file_info.uncompressed_size, &file_info.uncompressed_size, nullptr) == 0)
    {
        CloseHandle(hF);
        free(ptr);
        sprintf(szName, res_err, filename);
        _PostError(szName);
        return 0;
    }

    CloseHandle(hF);
    if (size) *size = file_info.uncompressed_size;
    return ptr;
}


void RenderManager::Resource_Free(LPVOID xRes)
{
    if (xRes) free(xRes);
}


//char* RenderManager::Resource_MakePath(LPCSTR filename)
//{
//    int i;
//
//    if (!filename)
//        strcpy(szTmpFilename, m_szAppPath);
//    else if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':')
//        strcpy(szTmpFilename, filename);
//    else
//    {
//        strcpy(szTmpFilename, m_szAppPath);
//        if (filename) strcat(szTmpFilename, filename);
//    }
//
//    for (i = 0; szTmpFilename[i]; i++) { if (szTmpFilename[i] == '/') szTmpFilename[i] = '\\'; }
//    return szTmpFilename;
//}

char* RenderManager::Resource_EnumFiles(LPCSTR wildcard)
{
    if (wildcard)
    {
        if (hSearch) { FindClose(hSearch); hSearch = 0; }
        hSearch = FindFirstFileA(Resource_MakePath(wildcard), &SearchData);
        if (hSearch == INVALID_HANDLE_VALUE) { hSearch = 0; return 0; }

        if (!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
        else return Resource_EnumFiles();
    }
    else
    {
        if (!hSearch) return 0;
        for (;;)
        {
            if (!FindNextFileA(hSearch, &SearchData)) { FindClose(hSearch); hSearch = 0; return 0; }
            if (!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
        }
    }
}

char* RenderManager::Resource_EnumFolders(LPCSTR wildcard)
{
    if (wildcard)
    {
        if (hSearch) { FindClose(hSearch); hSearch = 0; }
        hSearch = FindFirstFileA(Resource_MakePath(wildcard), &SearchData);
        if (hSearch == INVALID_HANDLE_VALUE) { hSearch = 0; return 0; }

        if ((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            strcmp(SearchData.cFileName, ".") && strcmp(SearchData.cFileName, ".."))
            return SearchData.cFileName;
        else return Resource_EnumFolders();
    }
    else
    {
        if (!hSearch) return 0;
        for (;;)
        {
            if (!FindNextFileA(hSearch, &SearchData)) { FindClose(hSearch); hSearch = 0; return 0; }
            if ((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                strcmp(SearchData.cFileName, ".") && strcmp(SearchData.cFileName, ".."))
                return SearchData.cFileName;
        }
    }
}
