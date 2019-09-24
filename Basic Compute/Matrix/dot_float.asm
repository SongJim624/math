    .code
dot_float proc
	XORPS xmm1, xmm1;
	
	mov rax, r8;
    SHR rax, 03h;
	
    jz sub_loop;
main_loop:

    vmovups ymm0, ymmword ptr[rcx];
    vdpps ymm0, ymm0, ymmword ptr[rdx], 255;
	
	vaddps ymm1, ymm1, ymm0;
	vperm2f128 ymm0, ymm0, ymm0, 7;
	vaddps ymm1, ymm1, ymm0;

    add rcx, 20h;
    add rdx, 20h;

    sub r8, 8;
    dec rax;  
    jnz main_loop;

control:
    add r8, 0;    
    jz done; 

sub_loop:

	vmovss xmm0,dword ptr[rcx];
    vmulss xmm0, xmm0, dword ptr[rdx];
    vaddss xmm1, xmm1, xmm0;

	dec r8;
	jz done

	add rcx, 4h;
    add rdx, 4h;
	jmp sub_loop;
done:
    vmovss dword ptr[r9], xmm1;
	ret
dot_float endp
end