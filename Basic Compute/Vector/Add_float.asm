    .code
Add_float proc
loop
    vmovups ymm0, [rcx];
    vaddps ymm1, ymm0, [rdx];
    vmovups [r8], ymm1;
    sub r9, 8;
Add_float endp
end