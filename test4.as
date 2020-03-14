;should succeed
LEFT1: clr r4

;should succeed
LEFT2: inc r4

;should succeed
LEFT3: dec r4

;should succeed
LEFT4: red r4

;should succeed
LEFT5: not r4

;should fail
LEFT6: clr 9

;should fail
LEFT7: inc -13

;should fail
LEFT30: clr -13

;should fail
LEFT8: dec 0

;should fail
LEFT9: red 99

;should fail
LEFT10: not 777

;should fail - addressing
LEFT11: not #-13

;should fail addressing
LEFT12: inc #-13

;should fail addressing
LEFT13: dec #-13

;should fail addressing
LEFT14: red #-13

;should fail addressing
LEFT15: clr #-13

;should succeed
LEFT70000: inc LABEL

;should succeed
LEFT302456: clr LABEL

;should succeed
LEFT843450: dec LABEL

;should succeed
LEFT92340: red LABEL

;should succeed
LEFT104440: not LABEL

;should fail
LEFT706557: inc 1LABEL

;should fail
LEFT301240: clr 1LABEL

;should fail
LEFT84564500: dec 1LABEL

;should fail
LEFT9023426: red 1LABEL

;should fail
LEFT1003450: not 1LABEL


;should succeed
clr r4

;should succeed
inc *r4

;should succeed
dec *r4

;should succeed
red *r4

;should succeed
not *r4

;should succeed
clr r4

;should succeed
inc r4

;should succeed
dec r4

;should succeed
red r4

;should succeed
not r4
