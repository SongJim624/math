;because the AVX assembly only support the xmm resigster
;to use the instruction : vdppd
;so it would be much more slower than floating point
;may in the future it would be updated
    .code
dot_double proc
	XORPD xmm1, xmm1;
	
	mov rax, r8;
    SHR rax, 01h;
	
    jz sub_loop;
main_loop:

    vmovupd xmm0, xmmword ptr[rcx];
    vdppd xmm0, xmm0, xmmword ptr[rdx], 127;
	
	vaddpd xmm1, xmm1, xmm0;

    add rcx, 10h;
    add rdx, 10h;

    sub r8, 2;
    dec rax;  
    jnz main_loop;

control:
    add r8, 0;    
    jz done; 

sub_loop:

	vmovsd xmm0,qword ptr[rcx];
    vmulsd xmm0, xmm0, qword ptr[rdx];
    vaddsd xmm1, xmm1, xmm0;

	dec r8;
	jz done

	add rcx, 8h;
    add rdx, 8h;
	jmp sub_loop;
done:
    vmovsd qword ptr[r9], xmm1;
	ret
dot_double endp
end