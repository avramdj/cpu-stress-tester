.intel_syntax noprefix
.text
.global assemblyStress
assemblyStress:
    vzeroupper
    cvtsi2ss xmm0, rcx
    vbroadcastss ymm1, xmm0
    vbroadcastss ymm2, xmm0
    vbroadcastss ymm4, xmm0
    vbroadcastss ymm5, xmm0
stressLoop:
    vfmadd213ps ymm0, ymm1, ymm2
    vfmadd213ps ymm3, ymm4, ymm5
    vfmadd213ps ymm6, ymm1, ymm2
    vfmadd213ps ymm7, ymm1, ymm2
    vfmadd213ps ymm8, ymm1, ymm2
    vfmadd213ps ymm9, ymm1, ymm2
    vfmadd213ps ymm10, ymm1, ymm2
    vfmadd213ps ymm11, ymm1, ymm2
    vfmadd213ps ymm12, ymm1, ymm2
    vfmadd213ps ymm13, ymm1, ymm2
    vfmadd213ps ymm14, ymm1, ymm2
    vfmadd213ps ymm15, ymm1, ymm2
    cmp [rdi], dword ptr 0
    je end
    jmp stressLoop
end:
    ret
