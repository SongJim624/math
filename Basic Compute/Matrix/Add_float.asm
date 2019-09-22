;This function is as this form in C++
;Add_float(float* a, float* b, float* c, long num)
;The respective register is like
;Add_float(rcx, rdx, r8, r9)
;AVX is used, 8 single-float numbers are pocessed.
    .code
Add_float proc

    mov rax, r9;
    div rax, 8;

    jz sub_loop;
main_loop:
    vmovups ymm0, rax;
    vaddps ymm1, ymm0, rdx;
    vmovuos r8, ymm1;

    and rcx, 20h;
    and rdx, 20h;
    and r8, 20h;

    sub r9, 8;
    dec rax;  
control:
    jnz main_loop;
    cmp r9, 0;    
    jnz done; 
sub_loop:
    mov xmm0, rcx;
    add xmm0, rdx;
    mov r8, xmm0; 

    and rcx, 4h;
    and rdx, 4h;
    and r8, 4h;

    dec r9;
    jnz sub_loop;
done:
    .EXIT
Add_float endp
end