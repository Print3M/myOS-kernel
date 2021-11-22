[bits 64]

section .text:
    global _enable_cpu_4_level_paging

_enable_cpu_4_level_paging:
    ; void _enable_cpu_4_level_paging()
    ; cr0.PG   = 1
    mov rax, 1
    shl rax, 31
    mov rbx, cr4
    or  rbx, rax
    mov cr4, rbx  

    ; cr4.PAE  = 1
    mov rax, 1
    shl rax, 5
    mov rbx, cr4
    or  rbx, rax
    mov cr4, rbx
; 
    ; cr4.LA57 = 0
    mov rax, 1
    shl rax, 57
    mov rbx, cr4
    not rbx
    or  rbx, rax
    not rbx
    mov cr4, rbx

    xor rax, rax
    ret
