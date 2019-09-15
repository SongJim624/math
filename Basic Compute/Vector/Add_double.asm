    .code
Add_double proc
loop
    vmovupd ymm0, [rcx];
    vaddpd ymm1, ymm0, [rdx];
    vmovupd [r8], ymm1;
    sub r9, 8;
Add_double endp
end