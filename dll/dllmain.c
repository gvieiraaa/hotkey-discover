/*!
 * \file dllmain.c
 * \brief This is a DLL that keeps all hook procedures.
 * \author Krzysztof "Itachi" Bąk  (https://github.com/ITachiLab/hotkey-detective)
 * \modified by github.com/gvieiraaa
 */

#include <stdio.h>
#include "dllmain.h"

HANDLE mapping_handle;
HWND *listening_thread_id;
HINSTANCE dll_hinst;
HWND process;
//LPCSTR afdsa;

BOOL WINAPI DllMain(HINSTANCE h_inst, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            dll_hinst = h_inst;

            mapping_handle = OpenFileMappingW(FILE_MAP_READ, FALSE, MMF_NAME);
            if (!mapping_handle) {
                OutputDebugString("Couldn't open memory file mapping.");
                break;
            }
            char s[256];
            listening_thread_id = MapViewOfFile(
                    mapping_handle, FILE_MAP_READ, 0, 0, sizeof(DWORD));
            sprintf(s, "listening_thread_id: %p", listening_thread_id);
            OutputDebugString((LPCSTR)s);
            if (!listening_thread_id) {
                OutputDebugString("Couldn't create a view of file.");
                return FALSE;
            }
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            UnmapViewOfFile(listening_thread_id);
            CloseHandle(mapping_handle);
            break;
    }
    return TRUE;
}

LRESULT CALLBACK getmessage_hook(int n_code, WPARAM w_param, LPARAM l_param) {
    if (n_code == HC_ACTION) {
        MSG *msg = (MSG *)l_param;

        if (LOWORD(msg->message) == WM_HOTKEY) {
            //OutputDebugString("WH_GETMESSAGE");
            char s[256];
            sprintf(s, "listening_thread_id: %lld / hwnd: %Id / lParam: %I64d", *listening_thread_id, (WPARAM)msg->hwnd, msg->lParam);
            OutputDebugString((LPCSTR)s);
            //PostMessageW(*listening_thread_id, 0x6666, (WPARAM)msg->hwnd, msg->lParam);
            PostMessageW(*listening_thread_id, 0x6666, (WPARAM)msg->hwnd, msg->lParam);
            msg->message = WM_NULL;
        }
    }

    return CallNextHookEx(NULL, n_code, w_param, l_param);
}

HHOOK set_hook(HWND p) {
    process = p;
    HOOKPROC hook_proc;
    hook_proc = getmessage_hook;
    
    HHOOK hook_handle = SetWindowsHookExW(WH_GETMESSAGE, hook_proc, dll_hinst, 0);
    if (!hook_handle) {
        printf("Couldn't apply hook: %lu\n", GetLastError());
    }

    return hook_handle;
}