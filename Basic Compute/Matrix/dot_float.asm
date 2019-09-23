    .code
dot_float proc
    vmovss xmm2, dword ptr[r9];

	vsubss xmm2, xmm2, xmm2;
	
	mov rax, r8;
    SHR rax, 03h;
	
    jz sub_loop;
main_loop:

    vmovups ymm0, ymmword ptr[rcx];
    vdpps ymm1, ymm0, ymmword ptr[rdx], 255;
	
	vmovups ymmword ptr [r9], ymm1;

	mov rbx, 8;
	vbroadcasti128 ymm3, ymmword rbx;

	vpermps ymm1, ymm3, ymm1;

	vaddss xmm2, xmm2, xmm1;
	vaddss xmm2, xmm2, dword ptr[r9];

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
    vmulss xmm1, xmm0, dword ptr[rdx];
    vaddss xmm2, xmm2, xmm1;

	dec r8;
	jz done

	add rcx, 4h;
    add rdx, 4h;
	jmp sub_loop;
done:
    vmovss dword ptr[r9], xmm2;
	ret
dot_float endp
end