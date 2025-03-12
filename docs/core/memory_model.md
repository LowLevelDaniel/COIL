# COIL Memory Model

The COIL Memory Model defines how memory is represented, accessed, and managed. It supports diverse processor architectures and memory hierarchies while requiring **no runtime system** for core functionality.

## Overview

- **Direct Hardware Mapping:** Maps directly to hardware memory
- **Explicit Memory Spaces:** Clear separation of different memory types
- **Static Management:** Memory operations determined at compile time
- **Bare-Metal Support:** Works in environments without an OS
- **Zero Abstraction:** No hidden costs or abstractions

## Memory Spaces

COIL defines abstract memory spaces that map to hardware memory hierarchies:

| Space    | ID  | Description                   | Hardware Examples            |
|----------|-----|-------------------------------|------------------------------|
| GLOBAL   | 0x0 | Global system memory          | System RAM, GPU global memory|
| DEVICE   | 0x1 | Device-specific memory        | GPU VRAM, accelerator memory |
| SHARED   | 0x2 | Memory shared within a group  | GPU shared memory, L2/L3 cache|
| LOCAL    | 0x3 | Thread-local memory           | Thread-local storage, registers|
| CONSTANT | 0x4 | Read-only memory with fast access | Constant caches, ROM     |
| REGISTER | 0x6 | Direct fast storage registers | CPU/GPU registers            |
| STACK    | 0x7 | Stack allocation memory       | CPU stack                    |

This explicit separation allows for direct mapping to different hardware memory types, which is critical for bare-metal environments.

## Memory Addressing

COIL uses a structured address format that includes the memory space:

```c
typedef struct {
  uint8_t  space;      // Memory space ID
  uint8_t  flags;      // Address flags
  uint16_t reserved;   // Reserved for future use
  uint64_t address;    // Actual address within the space
} coil_address_t;
```

Address flags provide additional information:

| Flag       | Value | Description                        |
|------------|-------|------------------------------------|
| ALIGNED    | 0x01  | Address is aligned to natural boundary |
| RESTRICTED | 0x02  | Address doesn't alias with other pointers |
| VOLATILE   | 0x04  | Address accesses are volatile (important for MMIO) |
| READONLY   | 0x08  | Address is read-only              |
| ATOMIC     | 0x20  | Address supports atomic operations |

## Core Memory Operations

### Allocation Operations

```
ALLOC dst, size, [alignment, space, flags]  // Allocate memory
FREE addr                                   // Deallocate memory
ALLOCA dst, size, [alignment]               // Stack allocation
```

All allocation is statically managed at compile time.

### Memory Access Operations

```
LOAD dst, [addr]                            // Load from memory
STORE [addr], src                           // Store to memory
LOAD_I32 dst, [addr]                        // Load 32-bit integer
STORE_F64 [addr], src                       // Store 64-bit float
```

Memory access operations map directly to hardware instructions.

### Vector and Specialized Access

```
VLOAD dst, [addr], length                   // Vector load
VSTORE [addr], src, length                  // Vector store
STRIDED_LOAD dst, [base], stride, count     // Strided memory access
GATHER dst, base, indices                   // Gather from indexed addresses
SCATTER base, indices, values               // Scatter to indexed addresses
```

### Memory Safety Operations

```
BOUNDS_CHECK addr, size                     // Verify memory range is valid
NULL_CHECK addr                             // Verify address is not null
LIFETIME_START addr, size                   // Mark object lifetime beginning
LIFETIME_END addr, size                     // Mark object lifetime end
```

## Bare-Metal Support

COIL provides direct access to hardware memory for bare-metal environments:

```
PHYSICAL_LOAD dst, [addr]                   // Direct physical memory access
PHYSICAL_STORE [addr], src                  // Direct physical memory write
MMIO_LOAD dst, [addr]                       // Memory-mapped I/O access
MMIO_STORE [addr], src                      // Memory-mapped I/O write
SET_PROTECTION addr, size, protection       // Set memory protection
```

These operations enable COIL to work in bootloaders, firmware, and UEFI applications where direct hardware access is required.

## Memory Layout Control

COIL provides explicit control over memory layout:

```
ALIGN addr, alignment                       // Ensure specific alignment
PAD addr, size, alignment                   // Add padding for alignment
```

## Hardware-Specific Memory Examples

### Memory-Mapped I/O for Bare Metal

```
// Define a hardware register at a specific physical address
MMIO_REG uart_data = 0xFF001000;

// Write to the hardware register
MMIO_STORE [uart_data], 'A';

// Read from the hardware register
MMIO_LOAD status_reg, [uart_data + 4];
```

### Direct Physical Memory Access

```
// Access physical memory directly (e.g., in a bootloader)
PHYSICAL_LOAD boot_params, [0x1000];
```

## Differences from Runtime Memory Management

Unlike runtime-dependent memory systems, COIL memory model:

1. Requires no memory allocator or garbage collector
2. Performs all address calculations at compile time
3. Maps directly to hardware memory spaces
4. Supports direct hardware register access
5. Works without any OS memory services