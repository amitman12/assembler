; should fail
cmp *r4,	+3

; should succeed
 cmp *r1,*r2

; should succeed
 cmp *r1, HELLOTHERE

; should succeed
	cmp *r0,#-2	

; should fail
cmp r4,	+3

; should succeed
 cmp r1,*r2

; should succeed
 cmp r1, HELLOTHERE

; should succeed
	cmp r0,#-2	

; should fail
cmp -6,	+3

; should fail
 cmp +9,*r2

; should fail
 cmp 17, HELLOTHERE

; should fail
	cmp 4,#-2	

; should fail
cmp #4,	+3

; should succeed
 cmp #1,*r2

; should succeed
 cmp #1, HELLOTHERE

; should succeed
	cmp #0,#-2	

; the next line is a label that starts after some spaces and tabs
    TEST: cmp r1,r4

; should accept different case
    test: cmp r1,r4

; test empty label should fail - empty label
  : cmp r1, r4

; should fail - command expected
SOMELABEL:

; the next line should fail - illegal label space after label
    TEST : cmp r1,r4

; should fail with a long label
A23456789_123456789_123456789012: cmp r1, r4

; should accept a valid label
A234567893123456789t12345678901: cmp r1, r4

; should fail with already exists
TEST: cmp r1,r4

; should fail with illegal label
3TEST: cmp r1,r4

; should fail with reserved word
r2: cmp r1, r5

; should fail with reserved word
add: cmp r1, r5

; should fail with with reserved word
.entry: cmp r1, r5

; both should be accepted
LIST1: cmp	r1,r4
LIST2: cmp	r1,r2

; should fail
	cmp *r0,
	