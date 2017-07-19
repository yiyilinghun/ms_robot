#pragma once

#ifdef ASSIST_DLL_EXPORTS
#define ASSIST_DLL_API __declspec(dllexport)
#else
#define ASSIST_DLL_API __declspec(dllimport)
#endif

ASSIST_DLL_API Int32 Editor_GetBaseGamePath(LPBYTE xGameResBuff, Int32 xGameResBuffLen);
ASSIST_DLL_API Boolean Editor_CreateAssistWnd(LPVOID xParam);
ASSIST_DLL_API HBITMAP Editor_OpenTgaImage(LPBYTE xTGAStream, Int32 xStreamLen);

ASSIST_DLL_API HBITMAP Editor_LoadImage(LPBYTE xFileName, Int32 xKey, Int32 xNextIndex, Int32 xDirIndex);

ASSIST_DLL_API void Editor_Release();



class EditorRenderManager : public RenderManager
{
public:
    EditorRenderManager(mstr xUnitName, MsIOCPManager* xMsIOCPManager, MsIOCPFactory* xMsIOCPFactory, WND_POS_TYPE xWndPosType);
    virtual ~EditorRenderManager();

    virtual Boolean TickOnce();
    virtual Boolean Render();
    virtual Boolean LogicTick();

    void Release();

#pragma region ������Ϣ�¼�
    virtual Boolean OnKeyEvent(KEY_EVENT_TYPE xEventType, Int32 xKey);
    virtual void OnKeyEventFree(KEY_EVENT_TYPE xEventType, Int32 xKey);
    virtual Boolean OnMouseEvent(MOUSE_EVENT_TYPE xEventType, Int32 xMouseX, Int32 xMouseY);
    virtual void OnMouseEventFree(MOUSE_EVENT_TYPE xEventType, Int32 xMouseX, Int32 xMouseY);
    virtual Boolean OnButtonWheel(Int32 xMouseX, Int32 xMouseY, Int32 xParam);
    virtual void OnButtonWheelFree(Int32 xMouseX, Int32 xMouseY, Int32 xParam);
    virtual Int32 OnNchitTest(Int32, Int32);
#pragma endregion

    Boolean System_Initiate();
    Boolean System_Start();

    void LoadMouseCursor(mstr xCursorName, DWORD xWasKey);

    Boolean InitiateFont();

    Single m_NextZLevel;

    ResManager* m_lpResManager;
    Boolean m_IsInitiated;

    RenderFont m_TIPS_FONT;
    RenderFont m_����_8_16_300;
    RenderFont m_����_8_16_100;
    RenderFont m_����_8_16_100_������ʾ;
    RenderFont m_����_8_16_100_������ʾ2;
    RenderFont m_����_8_16_700;
    RenderFont m_����_8_16_600;
    RenderFont m_����_16_32_800;
    RenderFont m_��������_8_18_500;
    RenderFont m_΢���ź�_9_20_800;
    RenderFont m_����_9_20_800;
};
