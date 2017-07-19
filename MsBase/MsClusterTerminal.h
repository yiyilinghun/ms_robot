#pragma once

class MsClusterTerminal : public MsNetTerminal
{
public:
    MsClusterTerminal(MsIOCPManager* xMsIOCPManager, Boolean IsInServer, SOCKET xSocket, Int64 xUnitIndex = INVALID_LID);
    virtual ~MsClusterTerminal();

    virtual void OnAttach();
    virtual void Close();
    Int64 m_IntAddr;
};
