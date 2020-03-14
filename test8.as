
; succeed
.extern HELLO

; succeed
	.extern HELLO

	; succeed
 .extern HELLO

; succeed
BLEB:.extern 	HELLO

; succeed
.extern 	HELLO,THERE , GENERAL	,	KENOBI

; succeed
.extern 	HELLO

; fail
.extern	9

; fail
.extern #9

; fail
.extern *r4


; fail
.extern r4

; fail
.extern HELLO HELLO

