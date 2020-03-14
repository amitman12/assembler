
; succeed
.string "HELLO"

; succeed
	.string "HELLO" 

	; succeed
 .string "HELLO"	

; succeed
BLEB:.string 	"HELLO_THERE"

; fail - \t isnt printable
.string 	"HELLO,THERE , GENERAL	,	KENOBI"

; fail - \t isnt printable
.string 	"HELLO,THERE , GENERAL ,    KENOBI"

; succeed
.string 	"HE"LLO"

; fail
.string	asd

; fail
.string #9

; fail
.string *r4


; fail
.string r4

; fail
.string HELLO HELLO

