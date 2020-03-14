; the next line is a label that starts after some spaces and tabs
    TEST: add r1,r4

; should accept different case
    test: add r1,r4

; test empty label should fail - empty label
  : add r1, r4

; should fail - command expected
SOMELABEL:

; the next line should fail - illegal label space after label
    TEST : add r1,r4

; should fail with a long label
A23456789_123456789_123456789012: add r1, r4

; should accept a valid label
A234567893123456789t12345678901: add r1, r4

; should fail with already exists
TEST: add r1,r4

; should fail with illegal label
3TEST: add r1,r4

; should fail with reserved word
r2: add r1, r5

; should fail with reserved word
add: add r1, r5

; should fail with illegal label/reserved word
.entry: add r1, r5

; both should be accepted
LIST1: add	r1,r4
LIST2: add	r1,r2

; should succeed
sub r3 ,r8

; should succeed
sub #-1,	r2

; should succeed
 add #-1,r2

; should succeed
	mov #-1, r2
	
; should succeed
sub #-1,	HELLO

; should succeed
 add #-1,HELLO

; should succeed
	mov #-1, HELLO

; should succeed
sub #-1,	*r2

; should succeed
 add #-1,*r1

; should succeed
	mov #-1, *r0
	
; should fail
sub -1,	*r2

; should fail
 add -1,*r1

; should fail
	mov -1, *r0

; should succeed
sub *r4,	HELLO

; should succeed
 add *r2,HELLO

; should succeed
	mov *r3, HELLO
	
; should fail
sub -1,	HELLO

; should fail
 add -1,HELLO

; should fail
	mov -1, HELLO
	
; should fail
sub -1,	+3

; should fail
 add -1,-5

; should fail
	mov -1, 	17
	
; should fail
sub #-1,	+3

; should fail
 add #-1,-5

; should fail
	mov #-1, 	17

; should fail
sub *r4,	+3

; should fail
 add *r1,-5

; should fail
	mov *r0, 	17

; should fail
	mov *r0, 	17

; should fail
	mov *r0,
; should fail
	mov *r0
		
;should faill
	add 7

;should fail
	add r2




