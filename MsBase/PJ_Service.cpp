#include "Precompiled.h"

//MsIOCPFactory   ms_MainPlayer_IOCPFactory;
//MsNetRouter     ms_MainPlayer_NetRouter;

DEF_GLOBAL_OBJECT_POINTER(MsIOCPFactory, LP_GW_FACTORY);
DEF_GLOBAL_OBJECT_POINTER(MsNetRouter, LP_CLUSTER_IN_ROUTER);
DEF_GLOBAL_OBJECT_POINTER(MsNetRouter, LP_CLUSTER_OUT_ROUTER);

//DEF_GLOBAL_OBJECT_POINTER(MsThreadIOCPManager, LP_DB_IOCP);

//PJ_DBManager* g_PJ_Service.m_lpDBAsynManager = nullptr;

PJ_Service::PJ_Service()
    : MsBaseService(L"PJ_Service")
    , m_NetGateway(nullptr)
{
#ifdef _DEBUG
    this->m_IsAutoRun = True;
#else
    this->m_IsAutoRun = False;
#endif
}

PJ_Service::~PJ_Service()
{
    ;
}

void BeginTest()
{

}

void PJ_Service::CloseAllScene()
{
    BEGIN_ENUMERATOR(m_Dict_ServerSceneCity)
    {
        xEnumValue->Post_ClearTimer();
        xEnumValue->Wait_ClearTimer();
        xEnumValue->PostClose();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerBattleScene)
    {
        xEnumValue->Post_ClearTimer();
        xEnumValue->Wait_ClearTimer();
        xEnumValue->PostClose();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerSceneWild)
    {
        xEnumValue->Post_ClearTimer();
        xEnumValue->Wait_ClearTimer();
        xEnumValue->PostClose();
    }END_ENUMERATOR;
}

void PJ_Service::WaitAllScene()
{
    BEGIN_ENUMERATOR(m_Dict_ServerSceneCity)
    {
        xEnumValue->WaitClose();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerBattleScene)
    {
        xEnumValue->WaitClose();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerSceneWild)
    {
        xEnumValue->WaitClose();
    }END_ENUMERATOR;
}

void PJ_Service::WaitAllSceneSave()
{
    BEGIN_ENUMERATOR(m_Dict_ServerSceneCity)
    {
        xEnumValue->WaitSave();
        xEnumValue->WaitSql();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerBattleScene)
    {
        xEnumValue->WaitSave();
        xEnumValue->WaitSql();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerSceneWild)
    {
        xEnumValue->WaitSave();
        xEnumValue->WaitSql();
    }END_ENUMERATOR;
}

void PJ_Service::WaitSceneOffline()
{
    BEGIN_ENUMERATOR(m_Dict_ServerSceneCity)
    {
        xEnumValue->GetClusterManager()->ObjectOffline(xEnumValue, ECOT_SCENE);
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerBattleScene)
    {
        xEnumValue->GetClusterManager()->ObjectOffline(xEnumValue, ECOT_SCENE);
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerSceneWild)
    {
        xEnumValue->GetClusterManager()->ObjectOffline(xEnumValue, ECOT_SCENE);
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_Dict_ServerSceneCity) { xEnumValue->WaitSql(); }END_ENUMERATOR;
    BEGIN_ENUMERATOR(m_Dict_ServerBattleScene) { xEnumValue->WaitSql(); }END_ENUMERATOR;
    BEGIN_ENUMERATOR(m_Dict_ServerSceneWild) { xEnumValue->WaitSql(); }END_ENUMERATOR;
}

void PJ_Service::ReleaseAllScene()
{
    SAFE_ENUMERATOR_DELETE(m_Dict_ServerSceneCity);
    SAFE_ENUMERATOR_DELETE(m_Dict_ServerBattleScene);
    SAFE_ENUMERATOR_DELETE(m_Dict_ServerSceneWild);
}

WORD PJ_Service::GetSceneIdByInstanceId(Int64 qwInstanceId)
{
    return (WORD)((qwInstanceId >> 32) & 0x0000FFFF);
}

WORD PJ_Service::GetMapIdByInstanceId(Int64 qwInstanceId)
{
    return (WORD)((qwInstanceId >> 16) & 0x0000FFFF);
}

Int64 PJ_Service::CreateSceneInstanceId(WORD wSceneId, WORD wMapId, WORD dwInstanceId)
{
    return (((Int64)MAKELONG(wMapId, wSceneId)) << 16) + dwInstanceId;
}

ServerCityScene* PJ_Service::GetCityScene(Int64 qwSceneInstanceId)
{
    if (m_Dict_ServerSceneCity.ContainsKey(qwSceneInstanceId))
    {
        return m_Dict_ServerSceneCity[qwSceneInstanceId];
    }
    return nullptr;
}

void PJ_Service::LoadCityScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName)
{
    Int64 xSceneInstanceId = this->CreateSceneInstanceId(wSceneId, wMapId, wInstanceId);

    MsClusterNode* xMsClusterManager = m_ListMsClusterManager[wInstanceId % m_ListMsClusterManager.GetCount()];
    ServerCityScene* xServerSceneCity = NEW ServerCityScene(xMsClusterManager);
    xServerSceneCity->Load(wSceneId, wMapId, xSceneInstanceId);
    xServerSceneCity->Init(xMapName);
    m_Dict_ServerSceneCity.Add(xSceneInstanceId, xServerSceneCity);
}

ServerWildScene* PJ_Service::GetWildScene(Int64 qwSceneInstanceId)
{
    if (m_Dict_ServerSceneWild.ContainsKey(qwSceneInstanceId))
    {
        return m_Dict_ServerSceneWild[qwSceneInstanceId];
    }
    return nullptr;
}

void PJ_Service::LoadWildScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName)
{
    Int64 xSceneInstanceId = this->CreateSceneInstanceId(wSceneId, wMapId, wInstanceId);

    MsClusterNode* xMsClusterManager = m_ListMsClusterManager[wInstanceId % m_ListMsClusterManager.GetCount()];
    ServerWildScene* xServerSceneWild = NEW ServerWildScene(xMsClusterManager);
    xServerSceneWild->Load(wSceneId, wMapId, xSceneInstanceId);
    xServerSceneWild->Init(xMapName);
    m_Dict_ServerSceneWild.Add(xSceneInstanceId, xServerSceneWild);
}

ServerBattleScene* PJ_Service::GetBattleScene(Int64 qwSceneInstanceId)
{
    if (m_Dict_ServerBattleScene.ContainsKey(qwSceneInstanceId))
    {
        return m_Dict_ServerBattleScene[qwSceneInstanceId];
    }
    return nullptr;
}

Int64 PJ_Service::LoadBattleScene(WORD wSceneId, WORD wMapId, WORD wInstanceId, LPCSTR xMapName)
{
    Int64 xSceneInstanceId = this->CreateSceneInstanceId(wSceneId, wMapId, wInstanceId);

    MsClusterNode* xMsClusterManager = m_ListMsClusterManager[wInstanceId % m_ListMsClusterManager.GetCount()];
    ServerBattleScene* xServerBattleScene = NEW ServerBattleScene(xMsClusterManager);
    xServerBattleScene->Load(wSceneId, wMapId, xSceneInstanceId);
    xServerBattleScene->Init(xMapName);
    m_Dict_ServerBattleScene.Add(xSceneInstanceId, xServerBattleScene);
    return xSceneInstanceId;
}

Boolean PJ_Service::OnStart()
{
    try
    {
        if (MsService::Token_IsSystem()) { g_AssertIsMessage = False; }

        //MsDictionary<DWORD, DWORD> xDict;
        //while (True)
        //{
        //    DWORD dw = R_DWORD(0, 100);
        //    if (xDict.ContainsKey(dw))
        //    {
        //        xDict[dw]++;
        //    }
        //    else
        //    {
        //        xDict.Add(dw, 0);
        //    }
        //}

        //BeginTest();

        DWORD dwNumberOfProcessors = MsBaseDef::GetCPUNumberOfProcessors();
        for (DWORD i = 0; i < dwNumberOfProcessors; i++)
        {
            Char szBuff[100];
            sprintf_s(szBuff, "逻辑线程[%d]", i);
            m_ListMsClusterManager.Add(NEW MsClusterNode(szBuff, PJ_SERVER_REGISTRY_FIELD_NAME));
        }

        mstr xRegistryPath = "SYSTEM\\";
        xRegistryPath += PJ_SERVER_REGISTRY_FIELD_NAME;
        mstr xServiceAddr = MsRegistry::GetRegistryStringA(HKEY_LOCAL_MACHINE, xRegistryPath.c_str(), "ThisComputerAddr");
        m_NetGateway = NEW PJ_GW(m_ListMsClusterManager[0]);
        m_NetGateway->ServerListen(xServiceAddr.c_str(), 9998);

        AssertNormal(m_MinosLuaConfig.Init(".\\src\\serverMain.lua"), "脚本配置初始化失败!");

        this->LoadCityScene(1464, 1464, 1, "新手村");
        this->LoadBattleScene(1, 1, 1, "小黑屋");

        //// 加载套装商店配置表
        //m_MinosLuaConfig.GetAllsuit_shop();

        //lua_suit_shop* x0 = m_MinosLuaConfig.Getsuit_shop(4);

        //lua_born* x1 = m_MinosLuaConfig.Getborn(1);

        //lua_scene* x2 = m_MinosLuaConfig.Getscene(x1->sceneid());


        return True;
    }
    catch (...)
    {
        return False;
    }
}

Boolean PJ_Service::OnStop()
{
    this->Release();
    ::PostQuitMessage(0);
    return True;
}

Boolean PJ_Service::OnShutdown()
{
    this->Release();
    return True;
}

void PJ_Service::Release()
{
    if (m_NetGateway)
    {
        m_NetGateway->Post_ClearTimer();
        m_NetGateway->Wait_ClearTimer();
        m_NetGateway->WaitSql();
    }

    this->CloseAllScene();
    this->WaitAllScene();

    this->WaitSceneOffline();
    this->WaitAllSceneSave();

    BEGIN_ENUMERATOR(m_ListMsClusterManager)
    {
        xEnumValue->PostClusterOfflineMsg();
    }END_ENUMERATOR;

    BEGIN_ENUMERATOR(m_ListMsClusterManager)
    {
        xEnumValue->WatiClusterNode();
        xEnumValue->Stop();
        xEnumValue->Wait();
    }END_ENUMERATOR;

    SAFE_ENUMERATOR_DELETE(m_ListMsClusterManager);


    this->ReleaseAllScene();


    SAFE_DELETE(m_NetGateway);

    FREE_LP_CLUSTER_IN_ROUTER();
    FREE_LP_CLUSTER_OUT_ROUTER();
    FREE_LP_SCENEPLAYER_FACTORY();
    FREE_LP_GW_FACTORY();
    FREE_LP_GW_ROUTER();
    FREE_LP_SCENE_FACTORY();
}

Int32 APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ Int32 nCmdShow)
{
    g_AssertIsMessage = False;
    __try { return g_PJ_Service.ServiceRun(); }
    __except (MsBaseDef::TryDump(GetExceptionCode(), GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
    return -1;
}
