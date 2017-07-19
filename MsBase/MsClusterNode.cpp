#include "Precompiled.h"

MsClusterNode::MsClusterNode(mstr xName, mstr xConfigFile, mstr xFileDBPath, Boolean IsExclusive, DWORD xClusterNodeIndex, Int32 xTickInterval, mstr xInsideAddr, WORD xNetPort)
    : MsThreadIOCPManager(xName, xTickInterval, False)
    , m_IsExclusive(IsExclusive)
    , m_FileDB(xFileDBPath)
{
    AssertNormal(xClusterNodeIndex < MAX_CLUSTER_NODE_INDEX, "节点索引超过最大值[%d/%d]", xClusterNodeIndex, MAX_CLUSTER_NODE_INDEX);
    m_NodeIndex = xClusterNodeIndex;

    m_InsideAddr = xInsideAddr;
    m_InsideNetPort = xNetPort;

    m_FileDB.CreateFullPath("ClusterNode");

    // 中心单元
    MsXmlReader xMsXmlReader;
    xMsXmlReader[xConfigFile.c_str()]["root"]["center"]["node"];
    if (xMsXmlReader)
    {
        do
        {
            mstr xAddr = xMsXmlReader.GetAttrib("addr");
            mstr xStrPort = xMsXmlReader.GetAttrib("port");
            WORD xPort = (WORD)atoi(xStrPort.c_str());
            m_ListCenterIntAddr.Add(MsBase::IPPP2Int(xAddr.c_str(), xPort, xPort));
        } while (xMsXmlReader.FindNextElem("node"));
    }

    m_InsideIntAddr = MsBase::IPPP2Int(m_InsideAddr.c_str(), m_InsideNetPort, m_InsideNetPort);
    for (Int32 i = 0; i < 1000; i++)
    {
        if (MsBase::CheckPortUsedTCP(m_InsideNetPort))
        {
            AssertLog("端口[%d]占用!", m_InsideNetPort);
            ::Sleep(1000);
            continue;
        }
        else
        {
            m_ClusterNetServer = NEW MsClusterNetServer(this);
            m_ClusterNetServer->ServerListen(m_InsideAddr.c_str(), m_InsideNetPort);
            m_ClusterNetClient = NEW MsClusterNetClient(this);
            break;
        }
    }
}

MsClusterNode::~MsClusterNode()
{
    this->Close();
    this->Stop();
}

void MsClusterNode::TickOnce()
{
    // 更新压力
    MsBase::TimeSpanCheckPressure(m_LastUpdatePressureTime, m_Pressure);
    m_CPU_Pressure = m_Pressure;

    if (m_IsClose)
    {
        this->Close();
        return;
    }
    else
    {
        if (!m_IsExclusive && MsBase::TimeSpanCheck(m_LastUpdateClusterDBInfoTime, 5000000))
        {
            this->UpdateClusterInfoToCenter(True);
        }
        m_ClusterNetServer->TickOnce();
    }
    return MsIOCPManager::TickOnce();
}


void MsClusterNode::Close()
{
    if (!m_IsClosed)
    {
        this->ClusterOffline();

        MsUnorderedDictionary<Int64, MsClusterTerminal*> xTemp_Dict_ServerNode = m_Dict_ServerNode;
        FAST_FOREACH(xTemp_Dict_ServerNode)
        {
            auto* xTemp = xTemp_Dict_ServerNode.Value();
            xTemp->Close();
        }
        xTemp_Dict_ServerNode.Clear();

        MsUnorderedDictionary<Int64, MsClusterTerminal*> xTemp_Dict_ClientNode = m_Dict_ClientNode;
        FAST_FOREACH(xTemp_Dict_ClientNode)
        {
            auto* xTemp = xTemp_Dict_ClientNode.Value();
            xTemp->Close();
        }
        xTemp_Dict_ClientNode.Clear();

        //m_ClusterNetServer->OnDetach();
        SAFE_DELETE(m_ClusterNetServer);
        SAFE_DELETE(m_ClusterNetClient);

        m_IsClosed = True;
    }
}

void MsClusterNode::CheckNextIndex()
{
    //while (True)
    //{
    //    if (m_MsMysql.ExeSQLFormat("SELECT COUNT(*) FROM `main` WHERE `key` = %lld LIMIT 1;", m_UnitNextIndex) && m_MsMysql.m_MysqlRes)
    //    {
    //        MysqlAutoRow xRow = m_MsMysql.NextRow();
    //        if (xRow)
    //        {
    //            Int32 xCount = xRow.GetInt32(0);
    //            if (xCount != 0)
    //            {
    //                m_UnitNextIndex++;
    //                continue;
    //            }
    //            else
    //            {
    //                break;
    //            }
    //        }
    //    }
    //}

    //while (True)
    //{
    //    if (m_MsMysql.ExeSQLFormat("SELECT COUNT(*) FROM `value` WHERE `key` = %lld LIMIT 1;", m_UnitNextIndex) && m_MsMysql.m_MysqlRes)
    //    {
    //        MysqlAutoRow xRow = m_MsMysql.NextRow();
    //        if (xRow)
    //        {
    //            Int32 xCount = xRow.GetInt32(0);
    //            if (xCount != 0)
    //            {
    //                m_UnitNextIndex++;
    //                continue;
    //            }
    //            else
    //            {
    //                break;
    //            }
    //        }
    //    }
    //}
    //m_MsMysql.ExeSQLFormat("UPDATE `cluster` SET `unit_next_index` = %lld WHERE `node_index` = %d;", m_UnitNextIndex, m_NodeIndex);
}

void MsClusterNode::ClusterOnline()
{
    mstr xTempName = "ClusterNode\\" + MsBase::ToString(m_NodeIndex);
    if (m_FileDB.CheckLoadProto(xTempName, m_ClusterNodeInfo))
    {
        m_UnitNextIndex = m_ClusterNodeInfo.GetUnitNextIndex();
        m_ClusterNodeInfo.SetIsOnline(True);
        m_ClusterNodeInfo.SetClusterNodeIntaddr(m_InsideIntAddr);
        if (!m_FileDB.SaveProto(xTempName, m_ClusterNodeInfo))
        {
            AssertNormal(False, "节点上线失败[%d][%lld]", m_NodeIndex, m_UnitNextIndex);
            return;
        }
    }
    else
    {
        m_UnitNextIndex = m_NodeIndex;
        m_UnitNextIndex *= MAX_UNIT_INDEX;
        m_ClusterNodeInfo.SetUnitNextIndex(m_UnitNextIndex);
        m_ClusterNodeInfo.SetIsOnline(True);
        m_ClusterNodeInfo.SetClusterNodeIntaddr(m_InsideIntAddr);
        if (!m_FileDB.CheckSaveProto(xTempName, m_ClusterNodeInfo))
        {
            AssertNormal(False, "节点上线失败[%d][%lld]", m_NodeIndex, m_UnitNextIndex);
            return;
        }
    }

    //while (True)
    //{
    //    if (m_MsMysql.ExeSQL(" SELECT `node_index`, `unit_next_index` FROM cluster WHERE `node_index` = (SELECT MIN(`node_index`) FROM cluster WHERE `is_online` = FALSE ORDER BY `node_index` LIMIT 1) LIMIT 1;") && m_MsMysql.m_MysqlRes)
    //    {
    //        MysqlAutoRow xRow = m_MsMysql.NextRow();
    //        if (xRow)
    //        {
    //            Int32 xTemp_NodeIndex = xRow.GetInt32(0);
    //            Int64 xTemp_UnitNextIndex = xRow.GetInt64(1);

    //            m_MsMysql.ExeSQLFormat(" UPDATE `cluster` SET `is_online` = TRUE, `intaddr` = %lld WHERE node_index = %d AND `is_online` = FALSE;", m_InsideIntAddr, xTemp_NodeIndex);
    //            if (m_MsMysql.GetInfluenceRowCount() == 1)
    //            {
    //                m_NodeIndex = xTemp_NodeIndex;
    //                m_UnitNextIndex = xTemp_UnitNextIndex;
    //                //mstr xShowIndex = MsBase::CI2SI(m_UnitNextIndex);
    //                //Int64 xClusterIndex = MsBase::SI2CI(xShowIndex, 0);
    //                //AssertLog("\no:%lld\ns:%s\nn:%lld\n", m_UnitNextIndex, xShowIndex.c_str(), xClusterIndex);
    //            }
    //            else
    //            {
    //                continue;
    //            }
    //        }
    //    }
    //    break;
    //}

    //if (m_NodeIndex == INVALID_NID && m_UnitNextIndex == INVALID_LID)
    //{
    //    m_MsMysql.ExeSQL(" INSERT INTO `cluster`(`intaddr`, `unit_next_index`, `is_online`) VALUES (0, 0, FALSE);");
    //    m_NodeIndex = (Int32)m_MsMysql.GetInsertId();
    //    m_UnitNextIndex = m_NodeIndex;
    //    m_UnitNextIndex *= MAX_UNIT_INDEX;

    //    //AssertLog(MsBase::CI2SI(m_UnitNextIndex).c_str());

    //    m_MsMysql.ExeSQLFormat(" UPDATE `cluster` SET `intaddr` = %lld , `unit_next_index` = %lld, `is_online` = TRUE WHERE node_index = %d;", m_InsideIntAddr, m_UnitNextIndex, m_NodeIndex);
    //    if (m_MsMysql.GetInfluenceRowCount() == 1)
    //    {
    //        // 成功
    //    }
    //}
    this->CheckNextIndex();
}


void MsClusterNode::ClusterOffline()
{
    if (!m_IsExclusive)
    {
        this->UpdateClusterInfoToCenter(False);
    }

    mstr xTempName = "ClusterNode\\" + MsBase::ToString(m_NodeIndex);
    m_ClusterNodeInfo.SetUnitNextIndex(m_UnitNextIndex);
    m_ClusterNodeInfo.SetIsOnline(False);
    if (!m_FileDB.SaveProto(xTempName, m_ClusterNodeInfo))
    {
        AssertNormal(False, "节点下线失败[%d][%lld]", m_NodeIndex, m_UnitNextIndex);
    }
    //if (m_MsMysql.ExeSQLFormat("UPDATE `cluster` SET `intaddr` = %lld , `unit_next_index` = %lld , `is_online` = FALSE WHERE `node_index` = %d;",
    //    m_InsideIntAddr, m_UnitNextIndex, m_NodeIndex) &&
    //    m_MsMysql.GetInfluenceRowCount() == 1)
    //{
    //    // 成功
    //}
}

void MsClusterNode::UpdateClusterInfoToCenter(Boolean IsOnline)
{
    S2S_RegNodeInfo xAsk;
    WORD xCenterPort = GetCenterPortByUnitIndex(m_NodeIndex);
    Int64 xIntAddr = MsBase::IPPP2Int(GetCenterAddrByUnitIndex(m_NodeIndex).c_str(), xCenterPort, xCenterPort);

    sql_ret_cluster_node_info* xNodeInfo = xAsk.mutable_cluster_node_info();
    xNodeInfo->set_node_index(m_NodeIndex);             // 节点索引
    xNodeInfo->set_intaddr(m_InsideIntAddr);            // 节点地址
    xNodeInfo->set_unit_next_index(m_UnitNextIndex);    // 下个索引
    xNodeInfo->set_cpu_pressure(m_CPU_Pressure);        // CPU压力
    xNodeInfo->set_is_online(IsOnline ? true : false);  // 是否在线
    this->SendPacketToUnit(xIntAddr, xAsk, CENTER_NODEMANAGER_IOCP_UNIT_KEY);
}
