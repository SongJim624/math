    .code
dot_float proc
    mov rax, r9;
    SHR rax, 03h;
	vmovss ymm2, 0;

    jz sub_loop;
main_loop:

    vmovups ymm0, [rcx];
    vdpps ymm1, ymm0, [rdx];
    vaddss ymm2, ymm1;

    add rcx, 20h;
    add rdx, 20h;
    add r8, 20h;

    sub r9, 8;
    dec rax;  
    jnz main_loop;

control:
    add r9, 0;    
    jz done; 

sub_loop:

	vmovss xmm0,dword ptr[rcx];
    vmulss xmm1, xmm0, dword ptr[rdx];
    vaddss xmm2, xmm1;

	dec r9;
	jz done

	add rcx, 4h;
    add rdx, 4h;
    add r8, 4h;
	jmp sub_loop;
done:
    mov rax, xmm2;
	ret
dot_float endp
end