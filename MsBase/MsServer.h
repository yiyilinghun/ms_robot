#pragma once

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT32 xMsgIndex, WPARAM wParam, LPARAM lParam);
INT_PTR DialogInit(HWND hDlg);
INT_PTR DialogClose(HWND hDlg);
INT_PTR DialogProcCommand(HWND hDlg, DWORD xControlId);
INT_PTR DialogProcCommand_Exit(HWND hDlg);
INT_PTR DialogProcCommand_Test(HWND hDlg);
