/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: ini file
*/


#include "hge.h"

void CALL HGE::Ini_SetInt(const char* section, const char* name, int value)
{
    char buf[256];

    if (this->System_GetState(HGE_INIFILE)) {
        sprintf(buf, "%d", value);
        WritePrivateProfileStringA(section, name, buf, this->System_GetState(HGE_INIFILE));
    }
}


int CALL HGE::Ini_GetInt(const char* section, const char* name, int def_val)
{
    char buf[256];

    if (this->System_GetState(HGE_INIFILE)) {
        if (GetPrivateProfileStringA(section, name, "", buf, sizeof(buf), this->System_GetState(HGE_INIFILE)))
        {
            return atoi(buf);
        }
        else { return def_val; }
    }
    return def_val;
}


void CALL HGE::Ini_SetFloat(const char* section, const char* name, float value)
{
    char buf[256];

    if (this->System_GetState(HGE_INIFILE)) {
        sprintf(buf, "%f", value);
        WritePrivateProfileStringA(section, name, buf, this->System_GetState(HGE_INIFILE));
    }
}


float CALL HGE::Ini_GetFloat(const char* section, const char* name, float def_val)
{
    char buf[256];

    if (this->System_GetState(HGE_INIFILE)) {
        if (GetPrivateProfileStringA(section, name, "", buf, sizeof(buf), this->System_GetState(HGE_INIFILE)))
        {
            return (float)atof(buf);
        }
        else { return def_val; }
    }
    return def_val;
}


void CALL HGE::Ini_SetString(const char* section, const char* name, const char* value)
{
    if (this->System_GetState(HGE_INIFILE))
        WritePrivateProfileStringA(section, name, value, this->System_GetState(HGE_INIFILE));
}


char* CALL HGE::Ini_GetString(const char* section, const char* name, const char* def_val)
{
    if (this->System_GetState(HGE_INIFILE))
        GetPrivateProfileStringA(section, name, def_val, szIniString, sizeof(szIniString), this->System_GetState(HGE_INIFILE));
    else
        strcpy(szIniString, def_val);

    return szIniString;
}

