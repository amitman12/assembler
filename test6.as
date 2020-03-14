;should succeed
LEFT3: prn r4

;should fail
LEFT8: prn 0


;should succeed
LEFT13: prn #-13


;should succeed
LEFT843450: prn LABEL


;should fail
LEFT84564500: prn 1LABEL


;should fail
prn *r4,r2

;should fail
LEFT2343: prn r4,#1

;should fail
LEFT82235: prn 0,13


;should fail
LEFT132346346: prn #-13


;should fail
LEFT843450234: prn LABEL,LABEL2


;should fail
LEFT84565474500: prn 1LABEL,LABEL


;should fail
prn *r4,r9


