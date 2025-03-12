# COIL Execution Model Specification

## 1. Introduction

The COIL Execution Model defines how code is executed across diverse processing units. This model is designed to support heterogeneous computing environments while providing deterministic behavior, efficient parallelism, and flexible control flow.

## 2. Execution Domains

COIL defines several execution domains that correspond to different processing unit architectures and programming models.

### 2.1 Execution Domain Types

| Domain ID | Name | Description | Typical Hardware |
|-----------|------|-------------|------------------|
| 0x0 | SEQUENTIAL | Sequential execution | Single-core CPU |
| 0x1 | THREAD | Multi-threaded execution | Multi-core CPU |
| 0x2 | SIMD | Single Instruction Multiple Data | Vector units, AVX, NEON |
| 0x3 | SIMT | Single Instruction Multiple Threads | GPU |
| 0x4 | DATAFLOW | Data-driven execution | FPGA, TPU, neural accelerators |
| 0x5 | VECTOR | Vector processing | Vector processors |
| 0x6 | QUANTUM | Quantum circuit execution | QPU |
| 0x7 | TASK | Task-based parallelism | Task schedulers |
| 0x8 | PIPELINE | Pipeline parallelism | DSPs, media processors |

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

## 3. Execution Units

### 3.1 Core Execution Unit

The core execution unit in COIL is a function, which defines a sequence of instructions to be executed:

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

| Flag | Value | Description |
|------|-------|-------------|
| EXTERNAL | 0x0001 | Function is defined externally |
| INLINE | 0x0002 | Function should be inlined |
| NOINLINE | 0x0004 | Function should not be inlined |
| PURE | 0x0008 | Function has no side effects |
| CONST | 0x0010 | Function always returns same value for same inputs |
| KERNEL | 0x0020 | Function is a compute kernel |
| DEVICE | 0x0040 | Function runs on device |
| HOST | 0x0080 | Function runs on host |
| EXPORTED | 0x0100 | Function is exported from module |
| INTERRUPT | 0x0200 | Function is an interrupt handler |
| NORETURN | 0x0400 | Function does not return |
| NAKED | 0x0800 | Function has no prologue/epilogue |
| CRITICAL | 0x1000 | Function is critical section |

## 4. Control Flow

### 4.1 Basic Control Flow

COIL supports standard control flow operations:

1. **Unconditional Branch**: Jump to a specified location
2. **Conditional Branch**: Branch based on a condition
3. **Function Call**: Call another function
4. **Function Return**: Return from a function
5. **Switch**: Multi-way branch based on a value

### 4.2 Structured Control Flow

COIL also supports structured control flow constructs:

1. **Loops**: Iteration constructs with defined entry and exit
2. **Selection**: If-then-else and switch statements
3. **Exceptions**: Try-catch-finally constructs
4. **Coroutines**: Functions that can suspend and resume

### 4.3 Control Flow Graph

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

## 5. Thread Model

### 5.1 Thread Hierarchy

COIL defines a hierarchical thread model suitable for diverse hardware:

1. **Thread**: Individual execution unit
2. **Warp/Wavefront**: Group of threads executing in lockstep (SIMD/SIMT)
3. **Block/Workgroup**: Group of threads that can synchronize and share memory
4. **Grid/NDRange**: Collection of blocks executing the same kernel

### 5.2 Thread Identification

Threads are identified by their coordinates in the hierarchy:

```c
/**
 * @struct coil_thread_id
 * @brief Thread identifier in COIL
 */
typedef struct {
  uint32_t global[3];         /* Global thread ID (x,y,z) */
  uint32_t local[3];          /* Local thread ID within block (x,y,z) */
  uint32_t block[3];          /* Block ID (x,y,z) */
  uint32_t warp_id;           /* Warp/wavefront ID */
  uint32_t lane_id;           /* Lane ID within warp */
} coil_thread_id_t;
```

### 5.3 Thread Synchronization

COIL provides primitives for thread synchronization:

1. **Barrier**: Synchronize threads in a block
2. **Fence**: Memory ordering barrier
3. **Atomic Operations**: Atomic memory access
4. **Critical Sections**: Exclusive execution regions
5. **Mutex/Semaphore**: Traditional synchronization primitives

## 6. Parallel Execution

### 6.1 Data Parallelism

COIL supports data-parallel execution across multiple processing units:

```
DISPATCH kernel, dimensions, global_size, local_size, [args...]
```

Where:
- `kernel`: The kernel function to execute
- `dimensions`: Number of dimensions (1-3)
- `global_size`: Total work items
- `local_size`: Work items per work group
- `args`: Kernel arguments

### 6.2 Task Parallelism

COIL supports task-parallel execution:

```
SPAWN task, [args...]
TASK_WAIT [task]
```

### 6.3 Pipeline Parallelism

COIL supports pipeline parallelism for stream processing:

```
PIPELINE_STAGE stage, function, [next_stage]
PIPELINE_PUSH pipeline, data
PIPELINE_POP pipeline, result
```

### 6.4 Vector Parallelism

For SIMD-style operations:

```
FOR_SIMD i, start, end, step, body
```

## 7. Execution Context

### 7.1 Context Definition

The execution context contains the state necessary for execution:

```c
/**
 * @struct coil_execution_context
 * @brief Execution context in COIL
 */
typedef struct {
  uint32_t domain;            /* Execution domain */
  uint32_t function_id;       /* Current function */
  uint32_t pc;                /* Program counter */
  uint32_t stack_ptr;         /* Stack pointer */
  uint32_t frame_ptr;         /* Frame pointer */
  uint32_t thread_id_ptr;     /* Pointer to thread ID structure */
  uint32_t memory_base_ptr;   /* Base pointer for memory addressing */
  uint32_t flags;             /* Context flags */
} coil_execution_context_t;
```

### 7.2 Context Switching

COIL supports context switching for cooperative multitasking:

```
CONTEXT_SAVE ctx
CONTEXT_LOAD ctx
CONTEXT_SWITCH old_ctx, new_ctx
```

### 7.3 Execution Stack

The execution stack contains:
1. Return addresses
2. Local variables
3. Saved registers
4. Function parameters
5. Context information

## 8. Heterogeneous Execution

### 8.1 Device Management

COIL provides primitives for managing multiple processing units:

```
DEVICE_QUERY devices, properties
DEVICE_SELECT device, criteria
DEVICE_ALLOCATE device, memory, size
DEVICE_FREE device, memory
DEVICE_COPY dst_device, dst_addr, src_device, src_addr, size
```

### 8.2 Kernel Dispatch

For executing code on specific devices:

```
DEVICE_DISPATCH device, kernel, dimensions, global_size, local_size, [args...]
```

### 8.3 Device Synchronization

For synchronizing operations across devices:

```
DEVICE_SYNC device, [event]
DEVICE_BARRIER devices, [event]
```

## 9. Asynchronous Execution

### 9.1 Events and Completion Signals

COIL supports event-based synchronization:

```
EVENT_CREATE event
EVENT_DESTROY event
EVENT_SIGNAL event
EVENT_WAIT event, [timeout]
EVENT_STATUS status, event
```

### 9.2 Asynchronous Operations

For non-blocking operations:

```
ASYNC_BEGIN operation
ASYNC_END [event]
AWAIT event
```

### 9.3 Futures and Promises

For asynchronous result handling:

```
FUTURE_CREATE future, type
PROMISE_CREATE promise, future
PROMISE_FULFILL promise, value
FUTURE_GET value, future, [timeout]
```

## 10. Exception Handling

### 10.1 Exception Model

COIL provides a structured exception handling model:

```
TRY label
CATCH type, handler
FINALLY cleanup
THROW exception
```

### 10.2 Exception Types

| Exception ID | Name | Description |
|--------------|------|-------------|
| 0x00 | GENERAL | General exception |
| 0x01 | ARITHMETIC | Arithmetic error (divide by zero, overflow) |
| 0x02 | MEMORY | Memory access violation |
| 0x03 | ALIGNMENT | Alignment error |
| 0x04 | STACK | Stack overflow/underflow |
| 0x05 | TYPE | Type error |
| 0x06 | RANGE | Out of range error |
| 0x07 | NULL | Null pointer dereference |
| 0x08 | DOMAIN | Domain error (math functions) |
| 0x09 | IO | Input/output error |
| 0x0A | HARDWARE | Hardware-specific error |

### 10.3 Exception Propagation

Exceptions propagate up the call stack until caught or until the program terminates.

## 11. Execution Models for Specific Domains

### 11.1 SIMD Execution Model

For SIMD execution domains:

1. Operations apply to entire vectors
2. Masking controls which elements are affected
3. Predication enables conditional execution

### 11.2 SIMT Execution Model

For SIMT execution domains (e.g., GPUs):

1. Threads within a warp execute in lockstep
2. Divergent branches cause serialization
3. Convergence points resynchronize execution

### 11.3 Dataflow Execution Model

For dataflow execution domains (e.g., FPGAs, TPUs):

1. Operations execute when inputs are available
2. Data dependencies determine execution order
3. No explicit program counter

### 11.4 Quantum Execution Model

For quantum execution domains (QPUs):

1. Operations are represented as quantum gates
2. Circuits define sequences of operations
3. Measurement collapses quantum state

## 12. Work Distribution and Scheduling

### 12.1 Work Distribution Patterns

COIL supports different patterns for distributing work:

1. **Block Distribution**: Contiguous chunks to each unit
2. **Cyclic Distribution**: Round-robin assignment
3. **Block-Cyclic Distribution**: Blocks assigned in round-robin fashion
4. **Adaptive Distribution**: Runtime load balancing

### 12.2 Work Scheduling

COIL provides different scheduling strategies:

1. **Static Scheduling**: Fixed assignment at compile time
2. **Dynamic Scheduling**: Runtime assignment based on availability
3. **Guided Scheduling**: Decreasing chunk sizes for better load balancing

### 12.3 Work Stealing

For dynamic load balancing:

```
WORK_QUEUE_CREATE queue, item_size
WORK_ENQUEUE queue, item
WORK_DEQUEUE item, queue
WORK_STEAL item, queue
```

## 13. Execution Annotations

### 13.1 Performance Annotations

COIL supports annotations for performance optimization:

```
HINT_HOT block           /* Frequently executed code */
HINT_COLD block          /* Rarely executed code */
HINT_UNROLL loop, factor /* Loop unrolling hint */
HINT_VECTORIZE loop      /* Vectorization hint */
HINT_PARALLEL loop       /* Parallelization hint */
```

### 13.2 Hardware-Specific Annotations

For targeting specific hardware features:

```
HINT_DEVICE type         /* Target specific device type */
HINT_FEATURE feature     /* Require specific hardware feature */
HINT_FALLBACK function   /* Fallback implementation */
```

### 13.3 Memory Behavior Annotations

For memory access optimization:

```
HINT_MEMORY_ACCESS addr, pattern /* Memory access pattern */
HINT_MEMORY_ALIAS a, b, relation /* Memory aliasing information */
HINT_MEMORY_ALIGN addr, alignment /* Memory alignment */
```

## 14. Execution Profiling and Monitoring

### 14.1 Performance Counters

COIL supports performance monitoring:

```
COUNTER_CREATE counter, type
COUNTER_START counter
COUNTER_STOP counter
COUNTER_RESET counter
COUNTER_READ value, counter
```

### 14.2 Execution Trace

For detailed execution analysis:

```
TRACE_BEGIN trace, events
TRACE_END trace
TRACE_EVENT trace, event_type, data
```

### 14.3 Execution Debugging

For debugging support:

```
BREAKPOINT
WATCHPOINT addr, size, condition
SINGLE_STEP
```

## 15. Implementation Guidelines

### 15.1 Execution Environment Mapping

When implementing the COIL execution model for a specific target:

1. Map COIL execution domains to hardware execution units
2. Implement appropriate synchronization primitives
3. Handle thread hierarchy appropriately
4. Support control flow constructs
5. Provide proper exception handling

### 15.2 Heterogeneous Execution Strategy

For heterogeneous computing environments:

1. Determine optimal device for each kernel
2. Manage data transfers efficiently
3. Balance load across processing units
4. Synchronize execution across devices
5. Handle device-specific optimizations

### 15.3 Performance Considerations

For optimal performance:

1. Minimize thread divergence
2. Maximize memory coalescing
3. Optimize work distribution
4. Leverage hardware-specific features
5. Reduce synchronization overhead

## 16. Cross-Platform Execution

### 16.1 Platform Abstraction

COIL provides a platform abstraction layer:

```c
/**
 * @struct coil_platform
 * @brief Platform abstraction in COIL
 */
typedef struct {
  uint32_t platform_id;       /* Platform identifier */
  uint32_t vendor_id;         /* Vendor identifier */
  uint32_t version;           /* Platform version */
  uint32_t device_count;      /* Number of devices */
  uint32_t features;          /* Platform features */
} coil_platform_t;
```

### 16.2 Target-Specific Code Paths

For handling platform-specific optimizations:

```
TARGET_BEGIN platform
TARGET_FALLBACK fallback_platform
TARGET_END
```

### 16.3 Compatibility Layer

For ensuring cross-platform compatibility:

1. Feature detection
2. Fallback implementations
3. Emulation of missing features
4. Portable memory model
5. Consistent thread model

## 17. Future Extensions

Areas for future development in the COIL execution model:

1. **Fault Tolerance**: Resilient execution across unreliable hardware
2. **Energy-Aware Execution**: Optimizing for energy efficiency
3. **Dynamic Recompilation**: Runtime code optimization
4. **Adaptive Scheduling**: Machine learning-based work distribution
5. **Distributed Execution**: Execution across networked devices