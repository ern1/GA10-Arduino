RControl::Return

SetKeyDelay, 20

RControl & q::
SendInput {q}
PressCtrlEnter()
return

RControl & w::
SendInput {w}
PressCtrlEnter()
return

RControl & e::
SendInput {e}
PressCtrlEnter()
return

RControl & r::
SendInput {r}
PressCtrlEnter()
return


RControl & t::
SendInput {t}
PressCtrlEnter()
return


RControl & o::
SendInput {o}
PressCtrlEnter()
return

RControl & p::
SendInput {p}
PressCtrlEnter()
return

RControl & l::
SendInput {l}
PressCtrlEnter()
return


PressCtrlEnter()
{
	SendInput {LControl down}{Enter down}
	SendInput {Enter up}{LControl up}
}
