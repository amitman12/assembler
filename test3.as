; should fail with reserved word
add: lea #1, r5

; should fail 
blob: lea 1, r5

; should fail
	lea #4,#-2

; should fail
	lea 4,#-2	

; should fail
lea #4,	+3

; should fail
 lea #1,*r2

; should fail
 lea #1, HELLOTHERE

; should fail
	lea #0,#-2	

; should succeed
    TEST: lea BLEB,r4

; should accept different case
    test: lea BLEB,r4

; test empty label should fail - empty label
  : lea #1, r4

; should fail - command expected
SOMELABEL:

; the next line should fail - illegal label space after label
    TEST : lea #1,r4

; should fail with a long label
A23456789_123456789_123456789012: lea #BLEB, r4

; should accept a valid label
A234567893123456789t12345678901: lea #1, r4

; should fail with already exists
TEST: lea #1,r4

; should fail with illegal label
3TEST: lea #1,r4

; should fail with reserved word
r2: lea #1, r5


; should fail with illegal label or reserved word
.entry: lea #1, r5

; both should be accepted
LIST1: lea	BLEB,r4
LIST2: lea	PLEB,r2

; should fail
	lea #1,
	
; should fail
lea *r4,	#+3

; should fail
 lea *r1,*r2

; should fail
 lea *r1, HELLOTHERE

; should fail
	lea *r0,#-2	

; should fail
lea r4,	+3

; should fail
lea -2,	r4

; should fail
 lea r1,*r2

; should fail
 lea r1, HELLOTHERE

; should fail
	lea r0,#-2	
	

; should fail
lea -6,	+3

; should fail
 lea +9,*r2

; should succeed
 lea PEEP, HELLOTHERE
 
 ; should succeed
 lea PEEP, *r4
	