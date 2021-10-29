[bits 64]

section .text:
    global __cpuid
    global is_long_mode

__cpuid:
    ; void __cpuid(uint32_t cpuid_eax, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
    mov eax, edi
    cpuid

    .check_eax_param:
        cmp esi, 0x0 ; If :eax is not NULL ptr
        jne .ret_eax_param
    .check_ebx_param:
        cmp edx, 0x0 ; If :ebx is not NULL ptr
        jne .ret_ebx_param
    .check_ecx_param:
        cmp ecx, 0x0 ; If :ecx is not NULL ptr
        jne .ret_ecx_param
    .check_edx_param:
        cmp r8, 0x0 ;If :edx is not NULL ptr
        jne .ret_edx_param

    .return:
    ret

    .ret_eax_param:
        mov dword [esi], eax
        jmp .check_ebx_param          
    .ret_ebx_param:
        mov dword [edx], ebx
        jmp .check_edx_param
    .ret_ecx_param:
        mov dword [ecx], ecx
        jmp .check_edx_param
    .ret_edx_param:
        mov dword [r8], edx
        jmp .return

is_long_mode:
    ; bool is_long_mode(void)
    mov eax, 80000001h
    cpuid
    test edx, 1 << 29 ; Check if Long Mode bit is set    
    xor eax, eax
    jz .true
    ret ; Return false (0)

    .true:
        mov eax, 1 ; Return true (1)
        ret