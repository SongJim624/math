	.code
smul_double proc
	vbroadcastsd ymm1, qword ptr[rdx];
	mov rax, r9;
    SHR rax, 02h;

    jz sub_loop;
main_loop:
    vmovupd ymm0, [rcx];

    vmulpd ymm0, ymm0, ymm1;
    vmovupd [r8], ymm0;

    add rcx, 20h;
    add r8, 20h;

    sub r9, 4;
    dec rax;  
    jnz main_loop;

control:
    add r9, 0;    
    jz done; 

sub_loop:

	vmovsd xmm0,qword ptr[rcx];
    vmulsd xmm0, xmm0, xmm1;
    vmovsd qword ptr[r8], xmm0; 

	dec r9;
	jz done

	add rcx, 8h;
    add r8, 8h;
	jmp sub_loop;
done:
	ret
smul_double endp
end