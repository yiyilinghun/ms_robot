#pragma once
#include "PJ_GW_NetRouter.h"

#define PJ_SERVER_REGISTRY_FIELD_NAME "PJService"

class ServerMap;
class ServerCityScene;
class ServerWildScene;
class ServerBattleScene;
class PJ_Service : public MsBaseService
{
public:
    PJ_Service();
    ~PJ_Service();

    void Release();
protected:
    virtual Boolean OnStart();
    virtual Boolean OnStop();
    virtual Boolean OnShutdown();

public:
    void CloseAllScene();
    void WaitAllScene();
    void WaitAllSceneSave();
    void WaitSceneOffline();
    void ReleaseAllScene();

    static WORD GetSceneIdByInstanceId(Int64 qwInstanceId);
    static WORD GetMapIdByInstanceId(Int64 qwInstanceId);
    static Int64 CreateSceneInstanceId(WORD wSceneId, WORD wMapId, WORD dwInstanceId);

    ServerCityScene* GetCityScene(Int64 qwSceneInstanceId);
    void LoadCityScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName);

    ServerWildScene* GetWildScene(Int64 qwSceneInstanceId);
    void LoadWildScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName);

    ServerBattleScene* GetBattleScene(Int64 qwSceneInstanceId);
    Int64 LoadBattleScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName);

    //MsClusterNode m_MsClusterNode;

    MsDictionary<Int64, ServerCityScene*> m_Dict_ServerSceneCity;
    MsDictionary<Int64, ServerWildScene*> m_Dict_ServerSceneWild;
    MsDictionary<Int64, ServerBattleScene*> m_Dict_ServerBattleScene;
    //void Minos_ServiceThreadTick();

    // Ω≈±æ≈‰÷√
    MinosLuaConfig m_MinosLuaConfig;

    // DB
    //PJ_DBManager* m_lpDBAsynManager;
    //MsThreadIOCPManager* m_lpDBMsThreadIOCPManager;

    // GW
    PJ_GW* m_NetGateway;
    //MsNetServer<ScenePlayer>* m_NetGateServer;

    //MsThreadIOCPManager* GetRelaxedIOCP();
private:
    MsList<MsClusterNode*> m_ListMsClusterManager;

    //static void _Minos_ServiceThreadTick(LPVOID lpThreadParameter)
    //{
    //    PJ_Service* lpMinos_Service = (PJ_Service*)lpThreadParameter;
    //    lpMinos_Service->Minos_ServiceThreadTick();
    //}
};

#define AUTO_WEB_REGISTER_USERCENTER_SERVER(rawurl)    AUTO_WEB_REGISTER(PJ_Service::ms_MsWebPacketFactory, rawurl)

extern PJ_Service g_PJ_Service;
//extern MsThreadPacketFactory g_MsThreadPacketFactory;

//extern MsWebResultFactory g_MsWebResultFactory;

//DECLARE_GLOBAL_OBJECT_POINTER(MsIOCPManager, LP_GW_IOCP);

//DECLARE_GLOBAL_OBJECT_POINTER(MsIOCPFactory, LP_GW_FACTORY);
DECLARE_GLOBAL_OBJECT_POINTER(PJ_GW_NetRouter, LP_GW_ROUTER);


DECLARE_GLOBAL_OBJECT_POINTER(MsIOCPFactory, LP_SCENE_FACTORY);


DECLARE_GLOBAL_OBJECT_POINTER(MsNetRouter, LP_CLUSTER_IN_ROUTER);
DECLARE_GLOBAL_OBJECT_POINTER(MsNetRouter, LP_CLUSTER_OUT_ROUTER);

//DECLARE_GLOBAL_OBJECT_POINTER(MsThreadIOCPManager, LP_DB_IOCP);


//MsIOCPFactory* GetMsIOCPFactory();
//PJ_ClusterListenRouter* GetPJ_ClusterListenRouter();
//PJ_ClusterBroadcastRouter* GetPJ_ClusterBroadcastRouter();

//MsNetPacketFactory*     GetMsNetPacketFactory();
//MsSqlResultFactory*     GetMsSqlResultFactory();

//PJ_GW_NetRouter*    GetPJ_Server_NetRouter();

