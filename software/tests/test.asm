% segment code 
mov r1 2 
stw ds r1 z 0 
mov r1 10 
stb ds r1 z 2 
mov r1 6 
mov r1 r1 
mov r2 10 
subr1 r1 r2 
stb ds r1 z 2 
% segment data 
: first 
% dw 0 
: sec 
% db 0 
% db 0 
% db 0 
% db 0 
