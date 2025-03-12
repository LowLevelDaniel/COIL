# COIL Execution Model Specification

## 1. Introduction

The COIL Execution Model defines how code is executed across processing units. This model is designed as a two-tier system:

1. **Core Execution Model**: The fundamental execution capabilities that operate without any runtime system, suitable for bare-metal environments like bootloaders, firmware, and UEFI applications.

2. **Optional Runtime Extensions**: Advanced capabilities for heterogeneous computing environments that build upon the core model but require additional runtime support.

This separation ensures COIL can serve both low-level system programming needs and high-performance heterogeneous computing requirements.

## 2. Execution Domains

### 2.1 Execution Domain Types

| Domain ID | Name | Description | Core/Optional | 
|-----------|------|-------------|--------------|
| 0x0 | SEQUENTIAL | Sequential execution | Core |
| 0x1 | THREAD | Multi-threaded execution | Core (basic) / Optional (advanced) |
| 0x2 | SIMD | Single Instruction Multiple Data | Core |
| 0x3 | SIMT | Single Instruction Multiple Threads | Optional |
| 0x4 | DATAFLOW | Data-driven execution | Optional |
| 0x5 | VECTOR | Vector processing | Core |
| 0x6 | QUANTUM | Quantum circuit execution | Optional |
| 0x7 | TASK | Task-based parallelism | Optional |
| 0x8 | PIPELINE | Pipeline parallelism | Core (basic) / Optional (advanced) |

### 2.2 Domain Attributes

Each execution domain has attributes that define its characteristics:

```c
/**
 * @struct coil_execution_domain_attrs
 * @brief Attributes defining execution domain characteristics
 */
typedef struct {
  uint32_t domain_id;         /* Execution domain identifier */
  uint32_t parallelism;       /* Degree of parallelism */
  uint32_t sync_model;        /* Synchronization model */
  uint32_t coherence_model;   /* Memory coherence model */
  uint32_t min_work_size;     /* Minimum work item size */
  uint32_t max_work_size;     /* Maximum work item size */
  uint32_t work_group_size;   /* Typical work group size */
  uint32_t features;          /* Special features bitfield */
} coil_execution_domain_attrs_t;
```

## 3. Core Execution Units

### 3.1 Function Definition (Core)

The basic execution unit in COIL is a function, which defines a sequence of instructions to be executed:

```c
/**
 * @struct coil_function
 * @brief Function definition in COIL
 */
typedef struct {
  uint32_t name_offset;       /* Offset to function name in string table */
  uint32_t signature_id;      /* Function signature ID */
  uint32_t domain;            /* Execution domain */
  uint32_t flags;             /* Function flags */
  uint32_t code_offset;       /* Offset to code section */
  uint32_t code_size;         /* Size of code section */
  uint32_t stack_size;        /* Required stack size */
  uint32_t local_size;        /* Required local memory size */
  uint32_t register_count;    /* Number of registers used */
  uint32_t param_count;       /* Number of parameters */
  uint32_t entry_point;       /* 1 if function is entry point, 0 otherwise */
} coil_function_t;
```

### 3.2 Function Flags

| Flag | Value | Description | Core/Optional |
|------|-------|-------------|--------------|
| EXTERNAL | 0x0001 | Function is defined externally | Core |
| INLINE | 0x0002 | Function should be inlined | Core |
| NOINLINE | 0x0004 | Function should not be inlined | Core |
| PURE | 0x0008 | Function has no side effects | Core |
| CONST | 0x0010 | Function always returns same value for same inputs | Core |
| KERNEL | 0x0020 | Function is a compute kernel | Optional |
| DEVICE | 0x0040 | Function runs on device | Optional |
| HOST | 0x0080 | Function runs on host | Optional |
| EXPORTED | 0x0100 | Function is exported from module | Core |
| INTERRUPT | 0x0200 | Function is an interrupt handler | Core |
| NORETURN | 0x0400 | Function does not return | Core |
| NAKED | 0x0800 | Function has no prologue/epilogue | Core |
| CRITICAL | 0x1000 | Function is critical section | Core |

## 4. Core Control Flow

### 4.1 Basic Control Flow (Core)

COIL supports standard control flow operations that work without runtime support:

1. **Unconditional Branch**: Jump to a specified location
2. **Conditional Branch**: Branch based on a condition
3. **Function Call**: Call another function
4. **Function Return**: Return from a function
5. **Switch**: Multi-way branch based on a value

### 4.2 Structured Control Flow (Core)

COIL also supports structured control flow constructs:

1. **Loops**: Iteration constructs with defined entry and exit
2. **Selection**: If-then-else and switch statements
3. **Basic Exceptions**: Simple try-catch for error handling
4. **Handlers**: Interrupt and exception handlers

### 4.3 Control Flow Graph (Core)

Control flow is represented as a graph with basic blocks as nodes and branches as edges:

```c
/**
 * @struct coil_basic_block
 * @brief Basic block in COIL control flow graph
 */
typedef struct {
  uint32_t id;                /* Basic block identifier */
  uint32_t offset;            /* Offset in function code */
  uint32_t size;              /* Size in bytes */
  uint32_t pred_count;        /* Number of predecessors */
  uint32_t pred_offset;       /* Offset to predecessor array */
  uint32_t succ_count;        /* Number of successors */
  uint32_t succ_offset;       /* Offset to successor array */
  uint32_t flags;             /* Basic block flags */
} coil_basic_block_t;
```

## 5. Core Thread Model (Single Device)

### 5.1 Basic Thread Support (Core)

Core thread capabilities for single-device operation:

1. **Hardware Threads**: Utilize hardware threading capabilities
2. **Vector Units**: Leverage SIMD/vector execution units
3. **Simple Synchronization**: Basic synchronization primitives
4. **Interrupts**: Hardware interrupt handling
5. **Exceptions**: Basic exception handling

### 5.2 Thread Identification (Core)

For core functionality, threads are identified simply:

```c
/**
 * @struct coil_core_thread_id
 * @brief Basic thread identifier in COIL core model
 */
typedef struct {
  uint32_t id;                /* Thread ID */
  uint32_t hardware_id;       /* Hardware thread ID (if applicable) */
  uint32_t core_id;           /* Core ID (if applicable) */
  uint32_t flags;             /* Thread flags */
} coil_core_thread_id_t;
```

### 5.3 Thread Synchronization (Core)

COIL provides basic primitives for thread synchronization:

1. **Simple Barrier**: Synchronize hardware threads
2. **Memory Fence**: Basic memory ordering barrier
3. **Atomic Operations**: Core atomic memory access
4. **Critical Sections**: Simple exclusive execution regions
5. **Interrupt Control**: Enable/disable interrupts for synchronization

## 6. Core Parallel Execution (Single Device)

### 6.1 SIMD/Vector Execution (Core)

COIL supports SIMD/vector execution on a single device:

```
SIMD_EXECUTE function, width, data
```

Where:
- `function`: The function to execute
- `width`: Vector width (number of lanes)
- `data`: Vector data

### 6.2 Hardware Threading (Core)

COIL supports hardware threading when available:

```
THREAD_EXECUTE function, core, [args...]
```

Where:
- `function`: The function to execute
- `core`: Target core (if applicable)
- `args`: Function arguments

## 7. Core Execution Context

### 7.1 Context Definition (Core)

The core execution context contains the minimum state necessary for execution:

```c
/**
 * @struct coil_core_execution_context
 * @brief Core execution context in COIL
 */
typedef struct {
  uint32_t function_id;       /* Current function */
  uint32_t pc;                /* Program counter */
  uint32_t stack_ptr;         /* Stack pointer */
  uint32_t frame_ptr;         /* Frame pointer */
  uint32_t flags;             /* Context flags */
} coil_core_execution_context_t;
```

### 7.2 Execution Stack (Core)

The core execution stack contains:
1. Return addresses
2. Local variables
3. Saved registers
4. Function parameters
5. Essential context information

## 8. Optional Heterogeneous Execution Extensions

> **Note**: The following capabilities are provided as optional extensions and require runtime support. They are not available in bare-metal environments without additional infrastructure.

### 8.1 Device Management (Optional)

Optional extensions for managing multiple processing units:

```
DEVICE_QUERY devices, properties       // Optional: Requires runtime
DEVICE_SELECT device, criteria         // Optional: Requires runtime
DEVICE_ALLOCATE device, memory, size   // Optional: Requires runtime
DEVICE_FREE device, memory             // Optional: Requires runtime
DEVICE_COPY dst, src, size             // Optional: Requires runtime
```

### 8.2 Kernel Dispatch (Optional)

Optional extensions for executing code on specific devices:

```
DEVICE_DISPATCH device, kernel, dimensions, global_size, local_size, [args...]  // Optional: Requires runtime
```

### 8.3 Device Synchronization (Optional)

Optional extensions for synchronizing operations across devices:

```
DEVICE_SYNC device, [event]           // Optional: Requires runtime
DEVICE_BARRIER devices, [event]       // Optional: Requires runtime
```

## 9. Optional Asynchronous Execution Extensions

> **Note**: These asynchronous execution features require runtime support and are provided as optional extensions.

### 9.1 Events and Completion Signals (Optional)

Optional event-based synchronization:

```
EVENT_CREATE event              // Optional: Requires runtime
EVENT_DESTROY event             // Optional: Requires runtime
EVENT_SIGNAL event              // Optional: Requires runtime
EVENT_WAIT event, [timeout]     // Optional: Requires runtime
EVENT_STATUS status, event      // Optional: Requires runtime
```

### 9.2 Asynchronous Operations (Optional)

Optional non-blocking operations:

```
ASYNC_BEGIN operation           // Optional: Requires runtime
ASYNC_END [event]               // Optional: Requires runtime
AWAIT event                     // Optional: Requires runtime
```

### 9.3 Futures and Promises (Optional)

Optional asynchronous result handling:

```
FUTURE_CREATE future, type      // Optional: Requires runtime
PROMISE_CREATE promise, future  // Optional: Requires runtime
PROMISE_FULFILL promise, value  // Optional: Requires runtime
FUTURE_GET value, future        // Optional: Requires runtime
```

## 10. Advanced Exception Handling (Optional)

> **Note**: Advanced exception handling features are provided as optional extensions and may require runtime support.

### 10.1 Extended Exception Model (Optional)

Optional extended exception handling:

```
TRY_BEGIN label                // Optional 
CATCH type, handler            // Optional
FINALLY cleanup                // Optional
THROW exception                // Optional
TRY_END                        // Optional
```

### 10.2 Exception Types (Core + Optional)

| Exception ID | Name | Description | Core/Optional |
|--------------|------|-------------|--------------|
| 0x00 | GENERAL | General exception | Core |
| 0x01 | ARITHMETIC | Arithmetic error (divide by zero, overflow) | Core |
| 0x02 | MEMORY | Memory access violation | Core |
| 0x03 | ALIGNMENT | Alignment error | Core |
| 0x04 | STACK | Stack overflow/underflow | Core |
| 0x05 | TYPE | Type error | Core |
| 0x06 | RANGE | Out of range error | Core |
| 0x07 | NULL | Null pointer dereference | Core |
| 0x08 | DOMAIN | Domain error (math functions) | Core |
| 0x09 | IO | Input/output error | Core |
| 0x0A | HARDWARE | Hardware-specific error | Core |
| 0x10-0xFF | ADVANCED | Advanced exception types | Optional |

## 11. Core Execution Models for Specific Domains

### 11.1 SIMD Execution Model (Core)

For SIMD execution domains:

1. **Vector Operations**: Operations apply to entire vectors
2. **Masking**: Conditional execution via masks
3. **Lane Predication**: Per-lane execution control

### 11.2 SIMT Execution Model (Optional)

For SIMT execution domains (e.g., GPUs):

1. **Warp Execution**: Threads within a warp execute in lockstep
2. **Branch Divergence**: Handling of divergent branches
3. **Reconvergence**: Thread reconvergence at join points

### 11.3 Dataflow Execution Model (Optional)

For dataflow execution domains (e.g., FPGAs, TPUs):

1. **Data-Driven**: Operations execute when inputs are available
2. **Dependency Graph**: Execution follows data dependencies
3. **Pipelined**: Operations execute in pipelined fashion

## 12. Core Work Distribution (Single Device)

### 12.1 Work Distribution Patterns (Core)

COIL supports basic patterns for distributing work on a single device:

1. **Block Distribution**: Contiguous chunks to each unit
2. **Cyclic Distribution**: Round-robin assignment
3. **Block-Cyclic Distribution**: Blocks assigned in round-robin fashion

### 12.2 Static Scheduling (Core)

COIL provides static scheduling strategies:

1. **Compile-Time Scheduling**: Fixed assignment at compile time
2. **Static Partitioning**: Predetermined work distribution
3. **Balanced Distribution**: Equal work assignment

## 13. Optional Advanced Scheduling Extensions

> **Note**: Advanced scheduling features are provided as optional extensions and require runtime support.

### 13.1 Dynamic Scheduling (Optional)

Optional dynamic scheduling support:

```
DYNAMIC_SCHEDULE work, workers, strategy     // Optional: Requires runtime
```

### 13.2 Work Stealing (Optional)

Optional dynamic load balancing:

```
WORK_QUEUE_CREATE queue, item_size          // Optional: Requires runtime
WORK_ENQUEUE queue, item                    // Optional: Requires runtime
WORK_DEQUEUE item, queue                    // Optional: Requires runtime
WORK_STEAL item, queue                      // Optional: Requires runtime
```

## 14. Core Execution Annotations

### 14.1 Performance Annotations (Core)

Static annotations for performance optimization:

```
HINT_HOT block           /* Frequently executed code */
HINT_COLD block          /* Rarely executed code */
HINT_UNROLL loop, factor /* Loop unrolling hint */
HINT_VECTORIZE loop      /* Vectorization hint */
HINT_INLINE function     /* Function inlining hint */
```

### 14.2 Hardware-Specific Annotations (Core)

Static annotations for targeting specific hardware features:

```
HINT_FEATURE feature     /* Require specific hardware feature */
HINT_FALLBACK function   /* Fallback implementation */
HINT_ALIGN address, alignment /* Memory alignment hint */
```

## 15. Implementation Guidelines

### 15.1 Core Execution Model Implementation

Guidelines for implementing the core execution model:

1. **No Runtime Dependencies**: Core functionality must work without runtime system
2. **Direct Hardware Mapping**: Map execution directly to hardware capabilities
3. **Static Compilation**: All decisions made at compile/link time
4. **Minimal Abstraction**: Provide direct access to hardware features
5. **Bare-Metal Support**: Support execution in environments with no OS
6. **Position Independence**: Support relocatable code execution
7. **Safety Mechanisms**: Include basic safety checks for robust execution
8. **Predictability**: Ensure deterministic execution behavior
9. **Resource Awareness**: Be conscious of limited resources in embedded environments
10. **Fallback Mechanisms**: Provide fallbacks for unsupported features

### 15.2 Optional Extensions Implementation

Guidelines for implementing optional extensions:

1. **Clear Separation**: Clearly separate core functionality from extensions
2. **Runtime Detection**: Detect availability of runtime support
3. **Graceful Degradation**: Fall back to core functionality when extensions unavailable
4. **Layered Architecture**: Build extensions on top of core functionality
5. **Optional Integration**: Allow selective use of extensions
6. **Performance Awareness**: Minimize overhead of runtime support
7. **Feature Detection**: Detect available extension features at runtime
8. **Versioning**: Support versioning of extensions
9. **Platform Adaptation**: Adapt to different host environments
10. **Documentation**: Clearly document extension dependencies

### 15.3 Bare-Metal Execution Considerations

Special considerations for bare-metal execution:

1. **No Heap Requirement**: Avoid dynamic memory allocation
2. **Static Allocation**: Use compile-time memory allocation
3. **Direct Hardware Access**: Allow direct register and memory access
4. **Interrupt Integration**: Support hardware interrupt mechanisms
5. **System Initialization**: Support early system initialization
6. **Memory Map Awareness**: Honor system memory map constraints
7. **Stack Management**: Careful stack usage tracking
8. **Power Management**: Support power state control
9. **Boot Process**: Support various boot processes
10. **Firmware Integration**: Enable seamless firmware integration

## 16. Cross-Platform Execution (Core)

### 16.1 Platform Abstraction (Core)

COIL provides minimal platform abstraction:

```c
/**
 * @struct coil_platform_info
 * @brief Platform information for core functionality
 */
typedef struct {
  uint32_t platform_id;       /* Platform identifier */
  uint32_t vendor_id;         /* Vendor identifier */
  uint32_t version;           /* Platform version */
  uint32_t features;          /* Platform features */
} coil_platform_info_t;
```

### 16.2 Target-Specific Code Paths (Core)

For handling platform-specific optimizations:

```
TARGET_BEGIN platform
TARGET_FALLBACK fallback_platform
TARGET_END
```

This allows for platform-specific implementations with fallbacks.

### 16.3 Compatibility Layer (Core)

For ensuring cross-platform compatibility:

1. **Feature Detection**: Detect available hardware features
2. **Fallback Implementations**: Provide fallbacks for missing features
3. **Abstraction Layer**: Thin abstraction for platform differences
4. **Compatibility Macros**: Define compatibility macros
5. **Portable Types**: Use portable type definitions

## 17. Future Directions

Areas for future development in the COIL execution model:

### 17.1 Core Model Enhancements

Future enhancements to core functionality:

1. **Static Analysis Integration**: Better integration with static analysis tools
2. **Safety Verification**: Enhanced safety verification
3. **Certification Support**: Features for safety certification
4. **Formal Verification**: Support for formal verification
5. **Timing Analysis**: Enhanced timing analysis and predictability

### 17.2 Optional Extensions

Potential future optional extensions:

1. **Fault Tolerance**: Resilient execution across unreliable hardware
2. **Energy-Aware Execution**: Optimizing for energy efficiency
3. **Dynamic Recompilation**: Runtime code optimization
4. **Adaptive Scheduling**: Machine learning-based work distribution
5. **Distributed Execution**: Execution across networked devices