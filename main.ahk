#Include <Start>
#Include <Admin>

; https://github.com/ITachiLab/hotkey-detective
DetectHiddenWindows, On
; hotkey_hook.dll
MOD_WIN := 0x8
;msgbox % Format("0x{:x}",GetKeyVK(".") << 16 | MOD_WIN)


OutputDebug, % "ahk pID: " DllCall("GetCurrentProcessId")
hDll := DllCall( "LoadLibrary", "Str", "hotkey_hook.dll" )
x := DllCall("hotkey_hook.dll\set_hook", "Ptr", DllCall("GetCurrentProcessId"))
if !x
    Msgbox failed to load

OnMessage(0x6666, "msg")
return

msg(n_code, wParam, lParam) {
    msgbox % Format("n_code: {1}`twParam: {2}`tlParam: {3}"
        , n_code
        , wParam
        , lParam)
}



; F3::
; PostMessage, 0x312, % ++count,,, % "ahk_id " WinExist("ahk_class Shell_TrayWnd")
; clipboard := count
; return
;PostMessage, 0x312, 0x204, % GetKeyVK(".") << 16 | MOD_WIN,, % "ahk_id " WinExist("ahk_class Shell_TrayWnd")
;PostMessage, 0x312, 0x204, % GetKeyVK(".") << 16 | MOD_WIN,, % "ahk_id " WinExist("ahk_class Shell_TrayWnd")

; wparam 00000240
; lparam: 00BE0008
; handle: 00000000000202F0
; Shell_TrayWnd
;501 554

F12::ExitApp