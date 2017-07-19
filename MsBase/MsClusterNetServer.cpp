#include "Precompiled.h"

MsClusterNetServer::MsClusterNetServer(MsClusterNode* xMsClusterNode) : MsNetServer(xMsClusterNode, CLUSTER_NETSERVER_IOCP_UNIT_KEY)
{
    ;
}

