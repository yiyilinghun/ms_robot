#pragma once
class MsClusterNetClient : public MsNetClient
{
public:
    MsClusterNetClient(MsClusterNode* xMsClusterNode) : m_lpMsClusterNode(xMsClusterNode) { ; }
    MsClusterNode* m_lpMsClusterNode;
};
