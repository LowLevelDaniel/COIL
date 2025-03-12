# COIL Memory Model Specification

## 1. Introduction

The COIL Memory Model defines how memory is represented, accessed, and managed in the COIL intermediate representation. This model supports diverse processor architectures and memory hierarchies while providing safety, performance, and flexibility, without requiring a runtime system for core functionality.

## 2. Memory Spaces

### 2.1 Primary Memory Spaces

COIL defines a set of abstract memory spaces that map to the memory hierarchies of various processing units:

| Space | ID | Description | Typical Hardware Mapping |
|-------|----|----|-------------|
| GLOBAL | 0x0 | Global memory visible to all processing units | System RAM, shared GPU memory |
| DEVICE | 0x1 | Device-specific memory | GPU VRAM, accelerator memory |
| SHARED | 0x2 | Memory shared within a group of threads/processors | GPU shared memory, CPU L2/L3 cache |
| LOCAL | 0x3 | Thread-local memory | Thread-local storage, register spill |
| CONSTANT | 0x4 | Read-only memory with fast access | Constant caches, ROM |
| TEXTURE | 0x5 | Specialized for spatial locality and filtering | Texture caches, texture units |
| REGISTER | 0x6 | Direct fast storage, not addressable as memory | CPU/GPU registers |
| STACK | 0x7 | Automatic memory with stack allocation semantics | CPU stack |

### 2.2 Memory Space Attributes

Each memory space has attributes that define its characteristics:

```c
/**
 * @struct coil_memory_space_attrs
 * @brief Attributes defining memory space characteristics
 */
typedef struct {
  uint32_t space_id;        /* Memory space identifier */
  uint32_t access_flags;    /* Read/write/atomic access capabilities */
  uint32_t coherence_model; /* Memory coherence model */
  uint32_t addressable;     /* 1 if directly addressable, 0 if not */
  uint32_t cacheable;       /* 1 if cacheable, 0 if not */
  uint32_t shared_scope;    /* Level at which memory is shared */
  uint32_t min_align;       /* Minimum required alignment */
  uint32_t max_alloc;       /* Maximum allocation size (0 = unlimited) */
  uint32_t alloc_gran;      /* Allocation granularity */
  uint32_t features;        /* Special features bitfield */
} coil_memory_space_attrs_t;
```

### 2.3 Access Flags

| Flag | Value | Description |
|------|-------|-------------|
| READ | 0x01 | Memory space supports reading |
| WRITE | 0x02 | Memory space supports writing |
| ATOMIC | 0x04 | Memory space supports atomic operations |
| CONCURRENT | 0x08 | Memory space supports concurrent access |
| COHERENT | 0x10 | Memory space is coherent |
| VOLATILE | 0x20 | Memory accesses are volatile |
| CACHED | 0x40 | Memory accesses are cached |
| UNCACHED | 0x80 | Memory accesses bypass cache |

### 2.4 Shared Scope Levels

| Scope | Value | Description |
|-------|-------|-------------|
| NONE | 0x0 | Not shared |
| THREAD | 0x1 | Shared within a thread |
| WARP | 0x2 | Shared within a warp/wavefront |
| BLOCK | 0x3 | Shared within a thread block |
| GRID | 0x4 | Shared within a grid/kernel |
| DEVICE | 0x5 | Shared within a device |
| SYSTEM | 0x6 | Shared across the system |

## 3. Memory Addressing

### 3.1 Address Format

COIL uses a structured address format that includes the memory space and address within that space:

```c
/**
 * @struct coil_address
 * @brief Address representation in COIL
 */
typedef struct {
  uint8_t  space;      /* Memory space ID */
  uint8_t  flags;      /* Address flags */
  uint16_t reserved;   /* Reserved for future use */
  uint64_t address;    /* Actual address within the space */
} coil_address_t;
```

### 3.2 Address Flags

| Flag | Value | Description |
|------|-------|-------------|
| ALIGNED | 0x01 | Address is aligned to natural boundary |
| RESTRICTED | 0x02 | Address doesn't alias with other pointers |
| VOLATILE | 0x04 | Address accesses are volatile |
| READONLY | 0x08 | Address is read-only |
| WRITEONLY | 0x10 | Address is write-only |
| ATOMIC | 0x20 | Address supports atomic operations |
| COHERENT | 0x40 | Address maintains coherence |
| CACHED | 0x80 | Address is cached |

### 3.3 Address Encoding

In the COIL binary format, addresses are encoded as:

```
[space:uint8][flags:uint8][address:uint64]
```

For 32-bit addressing modes, the upper 32 bits of the address are set to zero.

## 4. Memory Operations

### 4.1 Allocation Operation

COIL provides memory allocation operations:

```
ALLOC dst, size, [alignment, space, flags, name]
```

Where:
- `dst`: Destination register to receive the allocated pointer
- `size`: Size of the allocation in bytes
- `alignment`: Optional alignment requirement (power of 2)
- `space`: Optional memory space for allocation
- `flags`: Optional allocation flags
- `name`: Optional symbolic name for debugging

### 4.2 Allocation Flags

| Flag | Value | Description |
|------|-------|-------------|
| ZEROED | 0x01 | Memory should be zeroed after allocation |
| NOFAIL | 0x02 | Allocation must not fail (may reserve) |
| TEMP | 0x04 | Temporary allocation (short lifetime hint) |
| PERSIST | 0x08 | Persistent allocation (long lifetime hint) |
| DEVICELOCAL | 0x10 | Allocate in device-local memory if possible |
| HOSTLOCAL | 0x20 | Allocate in host-local memory if possible |
| SHARED | 0x40 | Allocate in memory shared between host and device |
| UNIFIED | 0x80 | Allocate in unified memory (automatic migration) |

### 4.3 Deallocation Operation

```
FREE addr
```

Where `addr` is the pointer to be deallocated.

### 4.4 Stack Allocation

For local stack-based allocation:

```
ALLOCA dst, size, [alignment]
```

## 5. Memory Access Patterns

### 5.1 Standard Memory Access

For standard memory access, COIL provides LOAD and STORE operations:

```
LOAD dst, [addr]
STORE [addr], src
```

### 5.2 Typed Memory Access

For typed memory access with explicit size:

```
LOAD_I8  dst, [addr]   ; Load 8-bit signed integer
LOAD_I16 dst, [addr]   ; Load 16-bit signed integer
LOAD_I32 dst, [addr]   ; Load 32-bit signed integer
LOAD_I64 dst, [addr]   ; Load 64-bit signed integer
LOAD_U8  dst, [addr]   ; Load 8-bit unsigned integer
LOAD_U16 dst, [addr]   ; Load 16-bit unsigned integer
LOAD_U32 dst, [addr]   ; Load 32-bit unsigned integer
LOAD_U64 dst, [addr]   ; Load 64-bit unsigned integer
LOAD_F32 dst, [addr]   ; Load 32-bit float
LOAD_F64 dst, [addr]   ; Load 64-bit float
```

Corresponding STORE operations are also available.

### 5.3 Vector Memory Access

For accessing vectors:

```
VLOAD  dst, [addr], length
VSTORE [addr], src, length
```

### 5.4 Strided Memory Access

For strided memory access patterns:

```
STRIDED_LOAD  dst, [base], stride, count
STRIDED_STORE [base], src, stride, count
```

### 5.5 Gather/Scatter Operations

For irregular access patterns:

```
GATHER  dst, base, indices
SCATTER base, indices, values
```

## 6. Memory Synchronization

### 6.1 Memory Fence

COIL provides memory fence operations for synchronization:

```
FENCE [scope, ordering]
```

Where:
- `scope`: Optional synchronization scope (thread, warp, block, device, system)
- `ordering`: Optional memory ordering constraints

### 6.2 Atomic Operations

COIL supports atomic memory operations:

```
ATOMIC_OP dst, [addr], operand
```

Where `OP` can be:
- `ADD`: Atomic add
- `SUB`: Atomic subtract
- `AND`: Atomic bitwise AND
- `OR`: Atomic bitwise OR
- `XOR`: Atomic bitwise XOR
- `MIN`: Atomic minimum
- `MAX`: Atomic maximum
- `EXCH`: Atomic exchange
- `CMPXCHG`: Atomic compare-and-exchange

### 6.3 Memory Barriers

For explicit thread synchronization with memory semantics:

```
BARRIER [scope]
```

## 7. Memory Safety

### 7.1 Bounds Checking

COIL provides explicit bounds checking operations:

```
BOUNDS_CHECK addr, size
```

This verifies that the memory range [addr, addr+size) is valid for access.

### 7.2 Null Checking

```
NULL_CHECK addr
```

This verifies that `addr` is not null before dereferencing.

### 7.3 Lifetime Tracking

COIL provides mechanisms to track object lifetimes:

```
LIFETIME_START addr, size
LIFETIME_END addr, size
```

These operations mark the beginning and end of an object's lifetime for validation and optimization.

## 8. Memory Transfer

### 8.1 Basic Memory Copy

```
MEMCPY dst, src, size
```

Copies `size` bytes from `src` to `dst`.

### 8.2 Memory Set

```
MEMSET dst, value, size
```

Sets `size` bytes at `dst` to `value`.

## 9. Memory Layout Control

### 9.1 Alignment Control

```
ALIGN addr, alignment
```

Ensures that `addr` is aligned to `alignment` (power of 2).

### 9.2 Padding Control

```
PAD addr, size, alignment
```

Adds padding to ensure proper alignment of structures.

### 9.3 Memory Packing

```
PACK_BEGIN
PACK_FIELD field, type, offset
PACK_END dst_type
```

Controls memory layout for packed structures.

## 10. Cache Control

### 10.1 Cache Operations

```
PREFETCH [addr], hint
FLUSH [addr], size
INVALIDATE [addr], size
```

Where:
- `PREFETCH`: Brings data into cache
- `FLUSH`: Writes cache data back to memory
- `INVALIDATE`: Invalidates cache entries

### 10.2 Prefetch Hints

| Hint | Value | Description |
|------|-------|-------------|
| READ | 0 | Data will be read |
| WRITE | 1 | Data will be written |
| TEMPORAL | 2 | Data has temporal locality |
| NONTEMPORAL | 3 | Data lacks temporal locality |

## 11. Memory Models

### 11.1 Memory Ordering Models

COIL supports multiple memory ordering models:

| Model | Description |
|-------|-------------|
| RELAXED | No ordering guarantees |
| ACQUIRE | Acquires synchronization |
| RELEASE | Releases synchronization |
| ACQREL | Both acquire and release semantics |
| SEQ_CST | Sequential consistency |

### 11.2 Memory Coherence Models

COIL supports different coherence models for different memory spaces:

| Model | Description |
|-------|-------------|
| NONE | No coherence guarantees |
| WEAK | Weak coherence (manual synchronization required) |
| STRONG | Strong coherence (hardware-maintained) |
| SEQUENTIAL | Sequential consistency |

## 12. Ownership and Lifetimes

### 12.1 Ownership Tracking

COIL provides explicit ownership tracking:

```
TRANSFER_OWNERSHIP dst, src
```

Transfers ownership of an object from `src` to `dst`.

### 12.2 Lifetime Annotations

```
LIFETIME_START addr, size
LIFETIME_END addr, size
```

Mark the beginning and end of object lifetimes.

## 13. Memory Access Attributes

### 13.1 Volatile Access

```
VOLATILE_LOAD dst, [addr]
VOLATILE_STORE [addr], src
```

Ensures that the compiler does not optimize away these memory accesses.

### 13.2 Atomic Access

```
ATOMIC_LOAD dst, [addr], ordering
ATOMIC_STORE [addr], src, ordering
```

Performs memory access with atomic guarantees.

### 13.3 Relaxed Access

```
RELAXED_LOAD dst, [addr]
RELAXED_STORE [addr], src
```

Indicates that memory access can be reordered for optimization.

## 14. Implementation Guidelines

### 14.1 Memory Space Mapping

When implementing the COIL memory model for a specific target:

1. Map COIL memory spaces to hardware-specific memory types
2. Respect the required attributes of each memory space
3. Implement appropriate fallbacks when direct mapping is not possible

### 14.2 Memory Allocation Strategy

For efficient memory allocation:

1. Use pool allocators for same-sized allocations
2. Align allocations to natural boundaries by default
3. Respect specialized alignment requirements
4. Use memory regions for related allocations

### 14.3 Safety Considerations

To ensure memory safety:

1. Implement bounds checking when requested
2. Validate memory spaces for operations
3. Track object lifetimes for validation
4. Detect and prevent memory leaks

### 14.4 Performance Optimizations

For optimal performance:

1. Use the most efficient memory space for each allocation
2. Minimize transfers between memory spaces
3. Leverage cache prefetching and hints
4. Coalesce small allocations when appropriate

## 15. Mapping to Hardware Memory Models

### 15.1 CPU Memory Model Mapping

| COIL Space | CPU Equivalent |
|------------|----------------|
| GLOBAL | Main memory |
| SHARED | L2/L3 cache |
| LOCAL | Thread-local storage |
| CONSTANT | Constant data section |
| REGISTER | CPU registers |
| STACK | Stack memory |

### 15.2 GPU Memory Model Mapping

| COIL Space | GPU Equivalent |
|------------|----------------|
| GLOBAL | Global memory |
| DEVICE | Device memory |
| SHARED | Shared memory |
| LOCAL | Local memory |
| CONSTANT | Constant memory |
| TEXTURE | Texture memory |
| REGISTER | GPU registers |

### 15.3 FPGA Memory Model Mapping

| COIL Space | FPGA Equivalent |
|------------|-----------------|
| GLOBAL | External DRAM |
| DEVICE | On-chip BRAM |
| SHARED | Distributed RAM |
| CONSTANT | LUT-based ROM |
| REGISTER | Flip-flops/registers |

## 16. Low-Level System Programming

### 16.1 Physical Memory Access

For bootloaders and UEFI applications, COIL provides direct physical memory access:

```
PHYSICAL_LOAD dst, [addr]
PHYSICAL_STORE [addr], src
```

### 16.2 Memory-Mapped I/O

For hardware access:

```
MMIO_LOAD dst, [addr]
MMIO_STORE [addr], src
```

### 16.3 Device Memory

For device-specific memory operations:

```
DEVICE_ALLOC dst, device_id, size, flags
DEVICE_FREE device_id, addr
```

### 16.4 Memory Protection

For controlling memory permissions:

```
SET_PROTECTION addr, size, protection
```

Where `protection` can be combinations of READ, WRITE, EXECUTE flags.

## 17. Memory Error Handling

COIL defines standard memory error types:

| Error | Description |
|-------|-------------|
| OUT_OF_MEMORY | Allocation failed due to insufficient memory |
| ACCESS_VIOLATION | Attempted to access invalid memory |
| ALIGNMENT_ERROR | Improper alignment for memory access |
| DOUBLE_FREE | Attempted to free already freed memory |
| INVALID_FREE | Attempted to free invalid pointer |
| MEMORY_LEAK | Memory was not properly deallocated |