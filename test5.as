;should fail
LEFT3: jmp r4

;should fail
LEFT4: bne r4

;should fail
LEFT5: jsr r4

;should fail
LEFT8: jsr 0

;should fail
LEFT9: bne 99

;should fail
LEFT10: jmp 777

;should fail addressing
LEFT13: jmp #-13

;should fail addressing
LEFT14: jsr #-13

;should fail addressing
LEFT15: bne #-13

;should succeed
LEFT843450: bne LABEL

;should succeed
LEFT92340: jmp LABEL

;should succeed
LEFT104440: jsr LABEL

;should fail
LEFT84564500: jmp 1LABEL

;should fail
LEFT9023426: jsr 1LABEL

;should fail
LEFT1003450: bne 1LABEL

;should succeed
jmp *r4

;should succeed
jsr *r4

;should succeed
bne *r4
