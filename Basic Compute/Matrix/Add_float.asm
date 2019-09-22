;This function is as this form in C++
;Add_float(float* a, float* b, float* c, long num)
;The respective register is like
;Add_float(rcx, rdx, r8, r9)
;AVX is used, 8 single-float numbers are pocessed.
    .code
Add_float proc
    mov rax, r9;
    SHR rax, 03h;
	
    jz sub_loop;
main_loop:
    vmovups ymm0, [rcx];
    vaddps ymm1, ymm0, [rdx];
    vmovups [r8], ymm1;

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
    vaddss xmm1, xmm0, dword ptr[rdx];
    vmovss dword ptr[r8], xmm1; 

	dec r9;
	jz done

	add rcx, 4h;
    add rdx, 4h;
    add r8, 4h;
	jmp sub_loop;
done:
	ret
Add_float endp
end