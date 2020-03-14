;should fail
LEFT3: stop r4

;should fail
LEFT8: stop 0


;should fail
LEFT13: stop #-13


;should fail
LEFT843450: stop LABEL


;should fail
LEFT84564500: stop 1LABEL


;should fail
stop *r4,r2

;should fail
LEFT3: stop r4,#1

;should fail
LEFT8: stop 0,


;should fail
LEFT13: stop #-13


;should fail
LEFT843450: stop LABEL,LABEL2


;should fail
LEFT84564500: stop 1LABEL


;should fail
stop *r4

;should succeed
stop 

;should succeed
rts 
