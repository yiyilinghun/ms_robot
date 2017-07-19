#include "Precompiled.h"

#define FAST_LOAD_RES_FILE(filename) m_DictFileWdfDecoder[filename] = NEW WdfDecoder(m_RenderManager, filename)

ResManager::ResManager(RenderManager* xRenderManager)
    : m_RenderManager(xRenderManager)
{
    FAST_LOAD_RES_FILE("mh/music.wdf");
    FAST_LOAD_RES_FILE("mh/smap.wdf");
    //FAST_LOAD_RES_FILE("mh/scene.wdf");

    FAST_LOAD_RES_FILE("shape.wdf");
    FAST_LOAD_RES_FILE("item.wdf");
    FAST_LOAD_RES_FILE("music.wdf");
    FAST_LOAD_RES_FILE("addon.wdf");
    FAST_LOAD_RES_FILE("effect.wd1");
    FAST_LOAD_RES_FILE("gires.wd1");
    FAST_LOAD_RES_FILE("gires.wdf");
    FAST_LOAD_RES_FILE("gires2.wdf");
    FAST_LOAD_RES_FILE("gires3.wdf");
    FAST_LOAD_RES_FILE("gires4.wdf");
    FAST_LOAD_RES_FILE("magic.wd2");
    FAST_LOAD_RES_FILE("magic.wd3");
    FAST_LOAD_RES_FILE("magic.wdf");
    FAST_LOAD_RES_FILE("sound.wdf");
    FAST_LOAD_RES_FILE("photo.wdf");
    FAST_LOAD_RES_FILE("smap.wdf");


    FAST_LOAD_RES_FILE("smap.wd1");

    FAST_LOAD_RES_FILE("shape.wd1");
    FAST_LOAD_RES_FILE("shape.wd2");
    FAST_LOAD_RES_FILE("shape.wd3");
    FAST_LOAD_RES_FILE("shape.wd4");
    FAST_LOAD_RES_FILE("shape.wd5");
    FAST_LOAD_RES_FILE("shape.wd6");
    FAST_LOAD_RES_FILE("shape.wd7");
    FAST_LOAD_RES_FILE("shape.wd8");
    FAST_LOAD_RES_FILE("shape.wd9");
    FAST_LOAD_RES_FILE("shape.wda");
    FAST_LOAD_RES_FILE("shape.wdb");
    FAST_LOAD_RES_FILE("shape.wdc");
    FAST_LOAD_RES_FILE("shape.wdd");
    FAST_LOAD_RES_FILE("shape.wdk");
    FAST_LOAD_RES_FILE("shape.wdl");

    FAST_LOAD_RES_FILE("item.wd1");
    FAST_LOAD_RES_FILE("item.wd2");
    FAST_LOAD_RES_FILE("item.wd3");
    FAST_LOAD_RES_FILE("item.wd4");
    FAST_LOAD_RES_FILE("item.wd5");

    FAST_LOAD_RES_FILE("music.wd1");
    FAST_LOAD_RES_FILE("music.wd2");
    FAST_LOAD_RES_FILE("music.wd3");
    FAST_LOAD_RES_FILE("effect.wd2");
    FAST_LOAD_RES_FILE("effect.wdf");

    FAST_LOAD_RES_FILE("magic.wd1");
    FAST_LOAD_RES_FILE("magic.wd4");


    FAST_LOAD_RES_FILE("goods.wdf");
    FAST_LOAD_RES_FILE("welcome.wdf");

    FAST_LOAD_RES_FILE("audio.wd1");
    FAST_LOAD_RES_FILE("audio.wdf");
    FAST_LOAD_RES_FILE("sound.wd1");
    FAST_LOAD_RES_FILE("sound.wd2");

    FAST_LOAD_RES_FILE("card.wdf");

    FAST_LOAD_RES_FILE("photo.wd1");
    FAST_LOAD_RES_FILE("photo.wd2");
    FAST_LOAD_RES_FILE("photo.wd3");
    FAST_LOAD_RES_FILE("photo.wd4");

    FAST_LOAD_RES_FILE("resource.wd1");
    FAST_LOAD_RES_FILE("resource.wd2");
    FAST_LOAD_RES_FILE("resource.wd3");
    FAST_LOAD_RES_FILE("resource.wd4");
    FAST_LOAD_RES_FILE("resource.wdf");

    //FAST_LOAD_RES_FILE("chat.wdf");
    //FAST_LOAD_RES_FILE("info.wdf");
    //FAST_LOAD_RES_FILE("launch.wdf");
    //FAST_LOAD_RES_FILE("lcinfo.wdf");
    //FAST_LOAD_RES_FILE("mapani.wdf");
    //FAST_LOAD_RES_FILE("mbinfo.wd1");
    //FAST_LOAD_RES_FILE("mbinfo.wdf");
    //FAST_LOAD_RES_FILE("misc.wdf");
    //FAST_LOAD_RES_FILE("mypal.wd1");
    //FAST_LOAD_RES_FILE("mypal.wdf");
    //FAST_LOAD_RES_FILE("pimage.wdf");
    //FAST_LOAD_RES_FILE("pimage75.wdf");
    //FAST_LOAD_RES_FILE("ridepal.wdf");
    //FAST_LOAD_RES_FILE("rmap.wdf");
    //FAST_LOAD_RES_FILE("script.wdf");
    //FAST_LOAD_RES_FILE("shape.wdftest");
    //FAST_LOAD_RES_FILE("shape.wdm");
    //FAST_LOAD_RES_FILE("tab.wdf");
    //FAST_LOAD_RES_FILE("xy2dat.wdf");
}

ResManager::~ResManager()
{
    SAFE_ENUMERATOR_DELETE(m_DictFileWdfDecoder);
}

void ResManager::ResReset()
{
    FAST_FOREACH(m_DictFileWdfDecoder)
    {
        xEnumValue->Reset();
    }
}

//WdfDecoder* ResManager::GetWdfDecoderByKey(DWORD xKey)
//{
//    FAST_FOREACH(m_DictFileWdfDecoder)
//    {
//        var* xWdfDecoder = xEnumerator.Value();
//        if (xWdfDecoder->ContainsKey(xKey))
//        {
//            return xWdfDecoder;
//        }
//    }
//    return nullptr;
//}

E_UNIT_TYPE ResManager::GetResType(mstr xFile, DWORD xKey)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->GetUnitType(xKey);
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", xFile.c_str(), xKey).c_str(), MB_OK);
    return E_UNIT_TYPE::EUT_UNKNOW;
    //else
    //{
    //    return m_DictFileWdfDecoder[xFile]->GetUnitType(xKey);
    //}
}

//E_UNIT_TYPE ResManager::GetResType(DWORD xKey)
//{
//    var xWdfDecoder = this->GetWdfDecoderByKey(xKey);
//    if (xWdfDecoder)
//    {
//        return xWdfDecoder->GetUnitType(xKey);
//    }
//    return E_UNIT_TYPE::EUT_UNKNOW;
//}

WasDecoder* ResManager::AnalysisWas(mstr xFile, DWORD xKey, Boolean IsSaveFile)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->AnalysisWas(xKey, IsSaveFile);
    }
    return nullptr;
}

WasDecoder* ResManager::LoadWas(mstr xFile, DWORD xKey)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->LoadWas(xKey);
        //if (!x)
        //{
        //    return this->LoadWas(xFile, xKey);
        //}
        //return x;
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", xFile.c_str(), xKey).c_str(), MB_OK);
    return nullptr;
    //else
    //{
    //    return this->LoadWas(xKey);
    //}
}

//WasDecoder* ResManager::LoadWas(DWORD xKey)
//{
//    var xWdfDecoder = this->GetWdfDecoderByKey(xKey);
//    if (xWdfDecoder)
//    {
//        return xWdfDecoder->LoadWas(xKey);
//    }
//    return nullptr;
//}

GameSprite* ResManager::LoadTga(mstr xFile, DWORD xKey)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->LoadTga(xKey);
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", xFile.c_str(), xKey).c_str(), MB_OK);
    return nullptr;
    //else
    //{
    //    return this->LoadTga(xKey);
    //}
}

//GameSprite* ResManager::LoadTga(DWORD xKey)
//{
//    var xWdfDecoder = this->GetWdfDecoderByKey(xKey);
//    if (xWdfDecoder)
//    {
//        return xWdfDecoder->LoadTga(xKey);
//    }
//    return nullptr;
//}

LPBYTE ResManager::LoadMp3(mstr xFile, DWORD xKey, Int32& xSize)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->LoadMp3(xKey, xSize);
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", xFile.c_str(), xKey).c_str(), MB_OK);
    return nullptr;
}

//LPBYTE ResManager::LoadMp3(DWORD xKey, Int32& xSize)
//{
//    var xWdfDecoder = this->GetWdfDecoderByKey(xKey);
//    if (xWdfDecoder)
//    {
//        return xWdfDecoder->LoadMp3(xKey, xSize);
//    }
//    return nullptr;
//}

LPBYTE ResManager::LoadWav(mstr xFile, DWORD xKey, Int32& xSize)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile]->LoadWav(xKey, xSize);
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s:0x%x]未找到", xFile.c_str(), xKey).c_str(), MB_OK);
    return nullptr;
}

//LPBYTE ResManager::LoadWav(DWORD xKey, Int32& xSize)
//{
//    var xWdfDecoder = this->GetWdfDecoderByKey(xKey);
//    if (xWdfDecoder)
//    {
//        return xWdfDecoder->LoadWav(xKey, xSize);
//    }
//    return nullptr;
//}

WasDecoder* ResManager::GetMoveFlagWas(mstr xFile, DWORD xdwResKey)
{
    return this->LoadWas(xFile, xdwResKey);
}

WdfDecoder* ResManager::GetWdf(mstr xFile)
{
    if (m_DictFileWdfDecoder.ContainsKey(xFile))
    {
        return m_DictFileWdfDecoder[xFile];
    }
    MessageBoxA(g_lpMainWndRender->m_StateHWND.m_hWindow, "资源未找到,确认大话II官方端已经更新到最新", MsBase::Format("资源文件[%s]未找到", xFile.c_str()).c_str(), MB_OK);
    return nullptr;
}


//WasDecoder* ResManager::GetWas(GameObjectActionInfo& xGameObjectResInfo)
//{
//    if (m_DictFileWdfDecoder.ContainsKey(xGameObjectResInfo.m_szFile))
//    {
//        return m_DictFileWdfDecoder[xGameObjectResInfo.m_szFile]->GetWas(hge, xGameObjectResInfo);
//    }
//    return nullptr;
//}
//

//MsUnorderedDictionary<HTEXTURE, Int32> GameSprite::ms_DictHtexture;
GameSprite::GameSprite(RenderManager* xRenderManager, HTEXTURE tex, float x, float y, float w, float h)
    : RenderSprite(xRenderManager, tex, x, y, w, h)
{
    //if (!ms_DictHtexture.ContainsKey(tex))
    //{
    //    ms_DictHtexture.Add(tex, 0);
    //}

    //if (ms_DictHtexture.ContainsKey(tex))
    //{
    //    ms_DictHtexture[tex]++;
    //}
}

GameSprite::~GameSprite()
{
    m_RenderManager->Texture_Free(quad.tex);

    //if (ms_DictHtexture.ContainsKey(quad.tex))
    //{
    //    ms_DictHtexture[quad.tex]--;
    //}
    //if (ms_DictHtexture[quad.tex] == 0)
    //{
    //    m_RenderManager->Texture_Free(quad.tex);
    //    ms_DictHtexture.Remove(quad.tex);
    //}
}

Boolean GameSprite::CheckPixelPickup(Int64 xNow, Int32 xMouseX, Int32 xMouseY, Single X, Single Y)
{
    RenderManagerRect xRect(X, Y, X + width, Y + height);
    return xRect.Contains(xMouseX, xMouseY);
}

void GameSprite::SaveToFile(mstr xFileName)
{
    m_RenderManager->Texture_SaveToFile(this->GetTexture(), xFileName.c_str());
}
