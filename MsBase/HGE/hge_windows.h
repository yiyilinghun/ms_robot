//#include "..\core\hge_plugin.h"

#include <windows.h>
#include <stdio.h>

struct CResourceList
{
	char				filename[_MAX_PATH];
	char				password[64];
	CResourceList*		next;
};

struct CInputEventList
{
	hgeInputEvent		event;
	CInputEventList*	next;
};

typedef BOOL (WINAPI *GetSystemPowerStatusFunc)(LPSYSTEM_POWER_STATUS);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/*
** HGE Plugin implementation
*/
class HGE_Windows : public HGE_Plugin
{
	public:
		virtual	void		CALL	Release();

		virtual bool		CALL	System_Initiate(HWND hWnd);
		virtual void		CALL	System_Shutdown();
		virtual bool		CALL	System_Start();
        virtual WNDPROC     CALL    System_GetWndProc() { return WindowProc; };

		virtual char*		CALL	System_GetErrorMessage();
		virtual	void		CALL	System_Log(const char *format, ...);
		virtual bool		CALL	System_Launch(const char *url);
		virtual void		CALL	System_Snapshot(const char *filename=0);

		virtual void*		CALL	Resource_Load(const char *filename, DWORD *size=0);
		virtual void		CALL	Resource_Free(void *res);
		virtual bool		CALL	Resource_AttachPack(const char *filename, const char *password=0);
		virtual void		CALL	Resource_RemovePack(const char *filename);
		virtual void		CALL	Resource_RemoveAllPacks();
		virtual char*		CALL	Resource_MakePath(const char *filename=0);
		virtual char*		CALL	Resource_EnumFiles(const char *wildcard=0);
		virtual char*		CALL	Resource_EnumFolders(const char *wildcard=0);

		virtual	void		CALL	Ini_SetInt(const char *section, const char *name, int value);
		virtual	int 		CALL	Ini_GetInt(const char *section, const char *name, int def_val);
		virtual	void		CALL	Ini_SetFloat(const char *section, const char *name, float value);
		virtual	float		CALL	Ini_GetFloat(const char *section, const char *name, float def_val);
		virtual	void		CALL	Ini_SetString(const char *section, const char *name, const char *value);
		virtual	char*		CALL	Ini_GetString(const char *section, const char *name, const char *def_val);

		virtual float		CALL	Timer_GetTime();
		virtual float		CALL	Timer_GetDelta();
		virtual int			CALL	Timer_GetFPS();

		virtual void		CALL	Input_GetMousePos(float *x, float *y);
		virtual void		CALL	Input_SetMousePos(float x, float y);
		virtual int			CALL	Input_GetMouseWheel();
		virtual bool		CALL	Input_IsMouseOver();
		virtual bool		CALL	Input_KeyDown(int key);
		virtual bool		CALL	Input_KeyUp(int key);
		virtual bool		CALL	Input_GetKeyState(int key);
		virtual char*		CALL	Input_GetKeyName(int key);
		virtual int			CALL	Input_GetKey();
		virtual int			CALL	Input_GetChar();
		virtual bool		CALL	Input_GetEvent(hgeInputEvent *event);

        virtual bool        CALL    Font_Create(D3DXFONT_DESCA* xD3dFont, LPD3DXFONT* lpID3DXFont);

		static HGE_Windows * _Interface_Get(HGE *lpHGE);

		void				_FocusChange(bool bAct);
		void				_Resize(int width, int height);

		void				_UpdateMouse();
		void				_InputInit();
		void				_ClearQueue();
		void				_BuildEvent(int type, int key, int scan, int flags, int x, int y);

		HINSTANCE			hInstance;

		bool				bActive;
		char				szError[256];
		char				szAppPath[_MAX_PATH];
		char				szIniString[256];

		RECT				rectW;
		LONG				styleW;

		RECT				rectFS;
		LONG				styleFS;

		// Input
		int					VKey;
		int					Char;
		int					Zpos;
		float				Xpos;
		float				Ypos;
		bool				bMouseOver;
		bool				bCaptured;
		char				keyz[256];
		CInputEventList*	queue;


		// Resources
		char				szTmpFilename[_MAX_PATH];
		CResourceList*		res;
		HANDLE				hSearch;
		WIN32_FIND_DATA		SearchData;

		// Power
		int							nPowerStatus;
		HMODULE						hKrnl32;
		GetSystemPowerStatusFunc	lpfnGetSystemPowerStatus;

		// Timer
		float				fTime;
		float				fDeltaTime;
		DWORD				nFixedDelta;
		int					nFPS;
		DWORD				t0, t0fps, dt;
		int					cfps;

	private:
		HGE_Windows(HGE *lpHGE);

		void _PostError(char *error);

		void _InitPowerStatus();
		void _UpdatePowerStatus();
		void _DonePowerStatus();
};

extern HGE_Windows * g_lpHGE;
