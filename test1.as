; the next line is a label that starts after some spaces and tabs
    TEST: add r1,r4

; should accept different case
    test: add r1,r4

; test empty label
  : add r1, r4

; should accept a label without a command
SOME_LABEL:

; the next line should fail
    TEST : add r1,r4

; should fail with a long label
A23456789_123456789_123456789012: add r1, r4

; should accept a valid label
A23456789_123456789_12345678901: add r1, r4

; should fail with already exists
TEST: add r1,r4

; should fail with invalid characters
3TEST: add r1,r4

; should fail with reserved word
r2: add r1, r5

; should fail with reserved word
add: add r1, r5

; should fail with reserved word
.entry: add r1, r5


LIST1: add	r1,r4
LIST2: add	r1,r2
