
; fail
.data "HELLO"

; fail
	.data "HELLO" 

	; fail
 .data 

; succeed
BLEB:.data 	1

; fail 
.data 	"HELLO,THERE , GENERAL	,	KENOBI"

; fail
.data 	"HELLO,THERE , GENERAL ,    KENOBI"

; succeed
.data 	13,8,9,234 ,0		,6, 34

; fail
.data	asd

; fail - syntax
.data #9

; fail
.data *r4


; fail
.data r4

; fail
.data HELLO HELLO

; succeed
.data 	-13,+8,9,234 ,0		,6, 34

; fail
.data 	-13,+8,9,,0		,6, 34


