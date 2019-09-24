;function in C:
;extern "C" void sin_float(float* a, float* b, unsigned long num)
;extern "C" void sin_float(rcx, rdx, r8);
 
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
    vbroadcastss ymm2, dword first;
    vmovups ymm0, ymmword ptr[rcx];
    vmulps ymm1, ymm0, ymm0;

comtrol:
sub_loop:

sin_float endp