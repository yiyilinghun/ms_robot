#include "Precompiled.h"

mstr GetBaseGamePath()
{
    MsXmlReader xMsXmlReader;
    xMsXmlReader[CLIENT_CONFIG_NAME]["root"]["client"];
    if (xMsXmlReader)
    {
        mstr xTemp = xMsXmlReader.GetAttrib("res_path");
        return MsBase::FillPathEndFlag(xTemp);
    }
    return "";
}

WORD GetServerPort()
{
    MsXmlReader xMsXmlReader;
    xMsXmlReader[CLIENT_CONFIG_NAME]["root"]["client"];
    if (xMsXmlReader)
    {
        Int32 xServerNumber;
        mstr xServerSum = xMsXmlReader.GetAttrib("server_port");
        if (MsBase::stoi(xServerSum.c_str(), xServerNumber))
        {
            return (WORD)xServerNumber;
        }
    }
    return 1;
}

mstr GetServerAddr()
{
    MsXmlReader xMsXmlReader;
    xMsXmlReader[CLIENT_CONFIG_NAME]["root"]["client"];
    if (xMsXmlReader)
    {
        return xMsXmlReader.GetAttrib("server_addr");
    }
    return "0.0.0.0";
}

DWORD GetServerSum()
{
    MsXmlReader xMsXmlReader;
    xMsXmlReader[CLIENT_CONFIG_NAME]["root"]["client"];
    if (xMsXmlReader)
    {
        Int32 xServerNumber;
        mstr xServerSum = xMsXmlReader.GetAttrib("server_sum");
        if (MsBase::stoi(xServerSum.c_str(), xServerNumber))
        {
            return xServerNumber;
        }
    }
    return 1;
}

void SaveCurrentConfig(RenderManager* xRenderManager)
{
    if (!xRenderManager->m_IsReConfig)
    {
        MsXmlReader xMsXmlReader;
        xMsXmlReader[CLIENT_CONFIG_NAME]["root"]["client"];
        if (xMsXmlReader)
        {
            FFORMAT(xTempStr, "%d*%d", xRenderManager->m_ScreenWidth, xRenderManager->m_ScreenHeight);
            xMsXmlReader.SetAttrib("resolution", xTempStr);
            xMsXmlReader.SetAttrib("isfullscreen", xRenderManager->m_StateBoolean.m_bFullScreen ? "1" : "0");
            xMsXmlReader.SetAttrib("ismusic", xRenderManager->m_StateBoolean.m_bMusic ? "1" : "0");
            xMsXmlReader.SetAttrib("issound", xRenderManager->m_StateBoolean.m_bSound ? "1" : "0");
            xMsXmlReader.Save(CLIENT_CONFIG_NAME);
        }
    }
}
