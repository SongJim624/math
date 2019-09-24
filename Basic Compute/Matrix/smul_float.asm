	.code
smul_float proc
	vbroadcastss ymm1, dword ptr[rdx];
	mov rax, r9;
    SHR rax, 03h;

    jz sub_loop;
main_loop:
    vmovups ymm0, [rcx];

    vmulps ymm0, ymm0, ymm1;
    vmovups [r8], ymm0;

    add rcx, 20h;
    add r8, 20h;

    sub r9, 8;
    dec rax;  
    jnz main_loop;

control:
    add r9, 0;    
    jz done; 

sub_loop:

	vmovss xmm0,dword ptr[rcx];
    vmulss xmm0, xmm0, xmm1;
    vmovss dword ptr[r8], xmm0; 

	dec r9;
	jz done

	add rcx, 4h;
    add r8, 4h;
	jmp sub_loop;
done:
	ret
smul_float endp
end