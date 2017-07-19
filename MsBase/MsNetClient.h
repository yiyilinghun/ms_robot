#pragma once

class MsNetClient
{
public:

    virtual MsNetTerminal* GetNewNetTerminal(MsIOCPManager* xMsIOCPManager)
    {
        return NEW MsNetTerminal(xMsIOCPManager, True, INVALID_SOCKET);
    }

    MsNetTerminal* BlockCreateClient(mstr xAddr, UInt16 xPort, MsIOCPManager* xMsIOCPManager);
    Boolean BlockCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal* xMsNetTerminal);
    Boolean BlockCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal& xMsNetTerminal);

    MsNetTerminal* TryCreateClient(mstr xAddr, UInt16 xPort, MsIOCPManager* xMsIOCPManager);
    Boolean TryCreateClient(mstr xAddr, UInt16 xPort, MsNetTerminal* xMsNetTerminal);
};
