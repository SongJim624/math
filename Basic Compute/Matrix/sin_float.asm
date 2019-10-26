;function in C:
;extern "C" void sin_float(float* a, float* b, unsigned long num)
;extern "C" void sin_float(rcx, rdx, r8);
;The fomula is sin(x) = x(1 - x^2 / 3! + x^4 / 5! - x^6 / 7! + x^8 / 9! - x^10 / 11!)

    .code
dword first 1;1!
dword second 6;3!
dword third 120;5!
dwrod fourth 5040;7!
dowrd fifth 362880;9!

dword
table

sin_float proc
    mov rax, r8;
    SHR rax 03h;
    jz sub_loop;
main_loop;
;ymm0 stores the x
;ymm1 stores the x^2
;ymm2 stores the result
    vmulps ymm1, ymm0, ymm0
    vsubps ymm2, ymm1, ymm2;

    vumlps ymm2, ymm2, ymm2;    
   

control:
sub_loop:

sin_float endp