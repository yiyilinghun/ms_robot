#include "Precompiled.h"

MsClusterTerminal::MsClusterTerminal(MsIOCPManager* xMsIOCPManager, Boolean IsInServer, SOCKET xSocket, Int64 xUnitIndex)
    : MsNetTerminal(xMsIOCPManager, IsInServer, xSocket, xUnitIndex)
{
    ;
}


MsClusterTerminal::~MsClusterTerminal()
{
    ;
}

void MsClusterTerminal::OnAttach()
{
    if (m_IsInServer) { this->m_MsIOCPManager->m_Dict_ServerNode.Add(m_UnitIndex, this); }
    m_IsExeClose = False;
    MsNetTerminal::OnAttach();
}

void MsClusterTerminal::Close()
{
    if (m_IsInServer)
    {
        this->m_MsIOCPManager->m_Dict_ServerNode.Remove(m_UnitIndex);
    }
    else
    {
        this->m_MsIOCPManager->m_Dict_ClientNode.Remove(m_IntAddr);
    }
    MsNetTerminal::Close();
    AssertLog("MsClusterTerminal::Close");
}
