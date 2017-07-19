#pragma once

class MsClusterNetServer : public MsNetServer
{
public:
    MsClusterNetServer(MsClusterNode* xMsClusterNode);
    virtual MsNetTerminal* GetNewNetTerminal(SOCKET xAcceptSocket)
    {
        return NEW MsClusterTerminal(m_MsIOCPManager, True, xAcceptSocket);
    }
};
