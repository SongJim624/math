;This function is as this form in C++
;Add_float(float* a, float* b, float* c, long num)
;The respective register is like
;Add_double(rcx, rdx, r8, r9)
;AVX is used, 4 single-float numbers are pocessed.
    .code
add_double proc
    mov rax, r9;
    SHR rax, 02h;
	
    jz sub_loop;
main_loop:
    vmovupd ymm0, [rcx];
    vaddpd ymm0, ymm0, [rdx];
    vmovupd [r8], ymm0;

    add rcx, 20h;
    add rdx, 20h;
    add r8, 20h;

    sub r9, 4;
    dec rax;  
    jnz main_loop;

control:
    add r9, 0;    
    jz done; 
sub_loop:

	vmovsd xmm0,qword ptr[rcx];
    vaddsd xmm0, xmm0, qword ptr[rdx];
    vmovsd qword ptr[r8], xmm0; 
	
	dec r9;
	jz done;

    add rcx, 8h;
    add rdx, 8h;
    add r8, 8h;

	jmp sub_loop;
done:
	ret
add_double endp
end