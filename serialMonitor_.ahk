;RControl::Return
WinGetClass, class, A
if WinActive("ahk_class" class)

RControl ::

SetKeyDelay, 20

;ih := InputHook("L1 M")
;ih.Start()
;ih.Wait()

;SendInput {ih.Input()}
;SendInput {LControl down}{Enter down}
;SendInput {Enter up}{LControl up}


MsgBox KeyWaitCombo()

KeyWaitCombo(Options:="")
{
    ih := InputHook(Options)
    if !InStr(Options, "V")
        ih.VisibleNonText := false
    ih.KeyOpt("{All}", "E")  ; End
    ; Exclude the modifiers
    ih.KeyOpt("{RCtrl}", "-E")
    ih.Start()
    ih.Wait()
	ih.
    ;return ih.EndMods . ih.EndKey  ; Return a string like <^<+Esc

	SendInput {ih.EndKey}
	SendInput {LControl down}{Enter down}
	SendInput {Enter up}{LControl up}
}


// https://www.autohotkey.com/docs/commands/InputHook.htm#OnChar
