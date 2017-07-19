#pragma once
#define MAX_CLUSTER_NODE_INDEX  900000
#define MAX_UNIT_INDEX          10000000000000
#define MAX_SHOW_UNIT_INDEX     60466176
#include "MsClusterTerminal.h"
#include "MsClusterNetServer.h"
#include "MsClusterNetClient.h"

// 节点信息
class UD_ClusterNodeInfo : public UD_Base
{
public:
    DEF_UD_PROTO_STRUCTURE(UD_ClusterNodeInfo);

    //DEF_U32_BEGIN_FIELD;
    //DEF_U32_FIELD(ClusterNodeIndex);

    DEF_U64_BEGIN_FIELD;
    DEF_U64_FIELD(UnitNextIndex);
    DEF_U64_FIELD(ClusterNodeIntaddr);

    DEF_B_BEGIN_FIELD;
    DEF_B_FIELD(IsOnline);
};

class MsClusterNode : public MsThreadIOCPManager
{
public:

    MsClusterNode(mstr xName, mstr xConfigFile, mstr xFileDBPath, Boolean IsExclusive, DWORD xClusterNodeIndex, Int32 xTickInterval, mstr xInsideAddr, WORD xNetPort);
    virtual ~MsClusterNode();
    virtual void TickOnce();

public:
    UD_ClusterNodeInfo m_ClusterNodeInfo;
    Boolean m_IsExclusive;
    MsNTFS m_FileDB;

    void CheckNextIndex();
    void ClusterOnline();
    void ClusterOffline();
    void UpdateClusterInfoToCenter(Boolean IsOnline);
    virtual void Close();
};
