# Example COIL Configuration File for x86-64 (Version 0.1.0)

```
; COIL Configuration File for x86-64
; Version 0.1.0

[metadata]
version = 0.1.0
timestamp = 1614556800
flags = system_default, autodetected

;===============================================================
; Processing Unit Definition
;===============================================================

[cpu:0]
name = Generic x86-64 CPU
vendor = Generic
model = x86-64 Compatible
type = 0  ; CPU type

;===============================================================
; Feature Definitions
;===============================================================

[core_features]
; Word sizes
8bit_support = true
16bit_support = true
32bit_support = true
64bit_support = true
128bit_support = false

; Addressing
physical_32bit = false
physical_36bit = false
physical_40bit = false
physical_48bit = true
physical_52bit = false
physical_64bit = false

virtual_32bit = false
virtual_48bit = true
virtual_57bit = false
virtual_64bit = false

; Core capabilities
branch_pred = true
speculative_exec = true
out_of_order = true
multithread = false
multicore = true
multiprocessor = false
numa = false
cache_coherent = true

[integer]
; Basic operations
int_add = true
int_sub = true
int_mul = true
int_div = true
int_rem = true

; Extended operations
int_popcnt = true
int_clz = true
int_ctz = true
int_bswap = true
int_addc = true
int_subc = true
int_mulh = true
int_mulhsu = false

; Bit manipulation
int_and = true
int_or = true
int_xor = true
int_not = true
int_shl = true
int_shr = true
int_sar = true
int_rol = true
int_ror = true
int_bext = false
int_bins = false

; Advanced bit manipulation
int_bitfield = false
int_bitperm = false
int_crc = false
int_gfmul = false

[floating_point]
; Supported formats
fp16 = false
fp32 = true
fp64 = true
fp80 = true
fp128 = false
bfloat16 = false

; Basic operations
fp_add = true
fp_sub = true
fp_mul = true
fp_div = true
fp_rem = true
fp_sqrt = true
fp_fma = true

; Extended operations
fp_exp = false
fp_log = false
fp_sin = false
fp_cos = false
fp_tan = false
fp_pow = false

; Rounding modes
fp_round_near = true
fp_round_zero = true
fp_round_up = true
fp_round_down = true

; IEEE compliance
fp_ieee754 = true
fp_denormals = true
fp_nan = true
fp_inf = true

[simd]
; Vector widths
simd_128bit = true  ; SSE
simd_256bit = true  ; AVX
simd_512bit = false ; AVX-512

; Data types
simd_int8 = true
simd_int16 = true
simd_int32 = true
simd_int64 = true
simd_float32 = true
simd_float64 = true

; Operations
simd_add = true
simd_sub = true
simd_mul = true
simd_div = true
simd_and = true
simd_or = true
simd_xor = true
simd_shift = true
simd_cmp = true
simd_blend = true
simd_shuffle = true
simd_insert = true
simd_extract = true
simd_fma = true

[memory]
; Basic operations
mem_load = true
mem_store = true
mem_prefetch = true
mem_fence = true
mem_barrier = true

; Atomic operations
atomic_load = true
atomic_store = true
atomic_exchange = true
atomic_compare_exchange = true
atomic_add = true
atomic_sub = true
atomic_and = true
atomic_or = true
atomic_xor = true

; Memory model
memory_sequential = true
memory_relaxed = true
memory_acquire = true
memory_release = true
memory_acq_rel = true

[security]
; CPU security features
nx_bit = true
smep = true
smap = true
aslr_support = true
stack_protector = true
shadow_stack = false
control_flow_guard = false
memory_tagging = false
secure_enclave = false

;===============================================================
; Resource Definitions
;===============================================================

[resources]
; General purpose registers
gpr = 16, 64bit, general_purpose
fpr = 16, 80bit, floating_point
vr = 16, 256bit, vector

; Caches
cache_l1 = 32KiB, data, 64B_line, 8_way
cache_l1 = 32KiB, instruction, 64B_line, 8_way
cache_l2 = 256KiB, unified, 64B_line, 8_way
cache_l3 = 8MiB, unified, 64B_line, 16_way

; Threads and cores
thread = 1, per_core
core = 4, per_cpu
cpu = 1, per_system

; Memory
memory = 16GiB, main

;===============================================================
; ABI Definitions
;===============================================================

[abi:system_v_x64]
arg_count = 6
flags = preserves_sp, caller_cleanup

[system_v_x64.mapping]
0 = gpr:0            ; Return value in RAX (RQ0)
1 = gpr:4            ; First integer argument in RDI (RQ4)
2 = gpr:5            ; Second integer argument in RSI (RQ5)
3 = gpr:2            ; Third integer argument in RDX (RQ2)
4 = gpr:8            ; Fourth integer argument in RCX (RQ8)
5 = gpr:9            ; Fifth integer argument in R8 (RQ9)
6 = gpr:10           ; Sixth integer argument in R9 (RQ10)
7 = stack:0          ; Additional arguments on stack
1f = fpr:0           ; First floating point argument in XMM0
2f = fpr:1           ; Second floating point argument in XMM1
3f = fpr:2           ; Third floating point argument in XMM2
4f = fpr:3           ; Fourth floating point argument in XMM3
5f = fpr:4           ; Fifth floating point argument in XMM4
6f = fpr:5           ; Sixth floating point argument in XMM5
7f = fpr:6           ; Seventh floating point argument in XMM6
8f = fpr:7           ; Eighth floating point argument in XMM7
9f = stack:0         ; Additional floating point arguments on stack
0f = fpr:0           ; Floating point return value in XMM0

[abi:windows_x64]
arg_count = 4
flags = preserves_sp, uses_red_zone

[windows_x64.mapping]
0 = gpr:0            ; Return value in RAX (RQ0)
1 = gpr:2            ; First argument in RCX (RQ2)
2 = gpr:3            ; Second argument in RDX (RQ3)
3 = gpr:8            ; Third argument in R8 (RQ8)
4 = gpr:9            ; Fourth argument in R9 (RQ9)
5 = stack:32         ; Additional arguments on stack (32-byte shadow space)
1f = fpr:0           ; First floating point argument in XMM0
2f = fpr:1           ; Second floating point argument in XMM1
3f = fpr:2           ; Third floating point argument in XMM2
4f = fpr:3           ; Fourth floating point argument in XMM3
5f = stack:32        ; Additional floating point arguments on stack
0f = fpr:0           ; Floating point return value in XMM0

[abi:linux_x64_syscall]
arg_count = 6
flags = preserves_sp

[linux_x64_syscall.mapping]
0 = gpr:0            ; Syscall number in RAX (RQ0)
1 = gpr:4            ; First argument in RDI (RQ4)
2 = gpr:5            ; Second argument in RSI (RQ5)
3 = gpr:2            ; Third argument in RDX (RQ2)
4 = gpr:8            ; Fourth argument in R10 (RQ8)
5 = gpr:9            ; Fifth argument in R8 (RQ9)
6 = gpr:10           ; Sixth argument in R9 (RQ10)
r = gpr:0            ; Return value in RAX (RQ0)

;===============================================================
; Architecture-Specific Extensions
;===============================================================

[arch:x86_64]
; x86-64 specific extensions
sse = true
sse2 = true
sse3 = true
ssse3 = true
sse4.1 = true
sse4.2 = true
avx = true
avx2 = true
f16c = true
fma = true
aes = true
sha = false
pclmulqdq = true
popcnt = true
lzcnt = true
bmi1 = true
bmi2 = true
adx = true
mpx = false
sgx = false
```

This example configuration defines a generic x86-64 CPU with typical features found in modern processors. It includes detailed information about:

1. Core features - Supported word sizes, addressing modes, and basic capabilities
2. Integer operations - Supported integer arithmetic and bit manipulation operations
3. Floating-point operations - Supported floating-point formats and operations
4. SIMD features - Vector processing capabilities
5. Memory operations - Memory access patterns and atomic operations
6. Security features - CPU security mechanisms
7. Resources - Available registers, caches, cores, and memory
8. ABIs - Calling conventions for different environments
9. Architecture-specific extensions - Features specific to x86-64

COIL assemblers use this information to generate optimized code for the target platform. The configuration can be automatically generated by the `coilcfg-detect` tool or manually created/edited using the text format shown here.