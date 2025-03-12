# COIL Debugging Support Specification

## 1. Introduction

The COIL Debugging Support specification defines the mechanisms for debugging COIL code across diverse processing units. This document describes debugging information formats, debugging operations, and tools integration to facilitate effective development.

## 2. Debugging Information Format

### 2.1 Debug Information Section

COIL object files include a dedicated section for debugging information:

```c
/**
 * @struct coil_debug_section_header
 * @brief Header for debug information section
 */
typedef struct {
  uint32_t version;          /* Debug format version */
  uint32_t flags;            /* Debug section flags */
  uint32_t source_count;     /* Number of source files */
  uint32_t line_map_offset;  /* Offset to line number mapping */
  uint32_t var_map_offset;   /* Offset to variable mapping */
  uint32_t type_info_offset; /* Offset to type debug information */
  uint32_t macro_offset;     /* Offset to macro information */
  uint32_t comp_unit_offset; /* Offset to compilation unit information */
} coil_debug_section_header_t;
```

### 2.2 Source Location Mapping

Source location mapping associates COIL instructions with source code locations:

```c
/**
 * @struct coil_source_location
 * @brief Source code location
 */
typedef struct {
  uint32_t file_index;      /* Source file index */
  uint32_t line;            /* Line number */
  uint32_t column;          /* Column number */
  uint32_t instr_offset;    /* Instruction offset in function */
} coil_source_location_t;
```

### 2.3 Variable Mapping

Variable mapping provides information about variables in the source code:

```c
/**
 * @struct coil_debug_variable
 * @brief Debug information for a variable
 */
typedef struct {
  uint32_t name_offset;     /* Offset to variable name */
  uint32_t type_index;      /* Type index */
  uint32_t location_type;   /* How the variable is located */
  uint32_t location_data;   /* Location data (register, memory, etc.) */
  uint32_t scope_begin;     /* Scope begin offset */
  uint32_t scope_end;       /* Scope end offset */
  uint32_t flags;           /* Variable flags */
} coil_debug_variable_t;
```

### 2.4 Type Information

Rich type information for debugging:

```c
/**
 * @struct coil_debug_type
 * @brief Debug information for a type
 */
typedef struct {
  uint32_t name_offset;     /* Offset to type name */
  uint32_t size;            /* Size in bytes */
  uint32_t alignment;       /* Alignment in bytes */
  uint32_t type_class;      /* Type class (primitive, struct, etc.) */
  uint32_t member_count;    /* Number of members (for composite types) */
  uint32_t members_offset;  /* Offset to member information */
  uint32_t parent_type;     /* Parent type (for derived types) */
  uint32_t flags;           /* Type flags */
} coil_debug_type_t;
```

## 3. Debugging Operations

### 3.1 Breakpoints

COIL provides support for breakpoints:

```
BREAKPOINT [id], [condition]
```

Breakpoint types:
1. **Code Breakpoint**: Stop at a specific instruction
2. **Data Breakpoint**: Stop on memory access
3. **Conditional Breakpoint**: Stop when condition is true
4. **Hardware Breakpoint**: Use hardware breakpoint registers
5. **Software Breakpoint**: Use instruction replacement

### 3.2 Watchpoints

For monitoring memory locations:

```
WATCHPOINT addr, size, [access_type], [condition]
```

Where `access_type` can be:
- READ: Break on read access
- WRITE: Break on write access
- READWRITE: Break on any access

### 3.3 Single Stepping

For instruction-by-instruction execution:

```
SINGLE_STEP [mode]
```

Where `mode` can be:
- INSTRUCTION: Step one instruction
- SOURCE_LINE: Step one source line
- OVER: Step over function calls
- INTO: Step into function calls
- OUT: Step out of current function

### 3.4 Variable Inspection

For examining variable values:

```
INSPECT variable, [format]
MEMORY_DUMP addr, size, [format]
```

### 3.5 Call Stack Manipulation

For examining and manipulating the call stack:

```
BACKTRACE [depth]
FRAME_SELECT frame_index
FRAME_LOCALS frame_index
FRAME_ARGS frame_index
```

## 4. Debug Sections

### 4.1 Line Number Information

Line number tables map between source code and binary instructions:

```c
/**
 * @struct coil_line_table_header
 * @brief Header for line number table
 */
typedef struct {
  uint32_t file_index;      /* Source file index */
  uint32_t entry_count;     /* Number of entries */
  uint32_t address_size;    /* Size of addresses (4 or 8 bytes) */
  uint32_t flags;           /* Table flags */
} coil_line_table_header_t;
```

Each entry maps a range of code to a source location.

### 4.2 Source File Information

Information about source files used in compilation:

```c
/**
 * @struct coil_source_file
 * @brief Source file information
 */
typedef struct {
  uint32_t name_offset;     /* Offset to file name */
  uint32_t path_offset;     /* Offset to full path */
  uint32_t content_hash;    /* Content hash for version check */
  uint32_t timestamp;       /* File timestamp */
  uint32_t line_count;      /* Number of lines */
  uint32_t flags;           /* File flags */
} coil_source_file_t;
```

### 4.3 Symbolic Information

Symbolic information for debugging:

```c
/**
 * @struct coil_debug_symbol
 * @brief Debug symbol information
 */
typedef struct {
  uint32_t name_offset;     /* Offset to symbol name */
  uint32_t address;         /* Symbol address */
  uint32_t size;            /* Symbol size */
  uint32_t type;            /* Symbol type */
  uint32_t binding;         /* Symbol binding */
  uint32_t section;         /* Symbol section */
  uint32_t flags;           /* Symbol flags */
} coil_debug_symbol_t;
```

### 4.4 Scope Information

Information about lexical scopes:

```c
/**
 * @struct coil_debug_scope
 * @brief Debug scope information
 */
typedef struct {
  uint32_t type;            /* Scope type (global, function, block, etc.) */
  uint32_t name_offset;     /* Offset to scope name (if any) */
  uint32_t start_offset;    /* Start code offset */
  uint32_t end_offset;      /* End code offset */
  uint32_t parent_scope;    /* Parent scope index */
  uint32_t sibling_scope;   /* Next sibling scope index */
  uint32_t child_scope;     /* First child scope index */
  uint32_t variable_count;  /* Number of variables in scope */
  uint32_t variables_offset; /* Offset to variable indices */
} coil_debug_scope_t;
```

## 5. Debugging Communication Protocol

### 5.1 Debug Server Interface

COIL defines a standard protocol for debugger-target communication:

```c
/**
 * @struct coil_debug_message
 * @brief Debug protocol message
 */
typedef struct {
  uint32_t message_id;      /* Message identifier */
  uint32_t sequence;        /* Sequence number */
  uint32_t type;            /* Message type */
  uint32_t length;          /* Payload length */
  uint8_t  payload[];       /* Message payload */
} coil_debug_message_t;
```

### 5.2 Message Types

| Message Type | Direction | Description |
|--------------|-----------|-------------|
| CONNECT | Client→Server | Initialize debugging session |
| DISCONNECT | Client→Server | Terminate debugging session |
| SET_BREAKPOINT | Client→Server | Set a breakpoint |
| CLEAR_BREAKPOINT | Client→Server | Clear a breakpoint |
| STEP | Client→Server | Single-step execution |
| CONTINUE | Client→Server | Continue execution |
| PAUSE | Client→Server | Pause execution |
| READ_MEMORY | Client→Server | Read memory content |
| WRITE_MEMORY | Client→Server | Write memory content |
| READ_REGISTER | Client→Server | Read register value |
| WRITE_REGISTER | Client→Server | Write register value |
| EVAL_EXPRESSION | Client→Server | Evaluate expression |
| BREAKPOINT_HIT | Server→Client | Breakpoint hit notification |
| STEP_COMPLETE | Server→Client | Step completed notification |
| ERROR | Server→Client | Error notification |
| EXCEPTION | Server→Client | Exception notification |
| STATE_CHANGE | Server→Client | Execution state change notification |

### 5.3 Execution State

COIL defines execution states for debugging:

| State | Description |
|-------|-------------|
| RUNNING | Program is executing |
| PAUSED | Execution is paused |
| TERMINATED | Execution has terminated |
| ERROR | Execution encountered an error |
| STARTING | Execution is starting |
| STEPPING | Executing a single step |

## 6. Heterogeneous Debugging

### 6.1 Multi-Device Debugging

COIL supports debugging across multiple processing units:

```
DEVICE_SELECT device_id
DEVICE_STATE device_id, state_var
DEVICE_BREAKPOINT device_id, address, [condition]
DEVICE_CONTINUE device_id
```

### 6.2 Device Synchronization

For synchronizing debugging across devices:

```
SYNC_POINT devices, [condition]
SYNC_CONTINUE devices
SYNC_STEP devices, mode
```

### 6.3 Domain-Specific Views

COIL supports domain-specific debugging views:

1. **SIMD View**: Visualize SIMD register state
2. **Thread View**: View multiple threads/lanes
3. **Memory View**: Visualize memory hierarchies
4. **Pipeline View**: Visualize processor pipeline state
5. **Kernel View**: Debug compute kernels
6. **Dataflow View**: Visualize dataflow execution

## 7. Debug Annotations

### 7.1 Source-Level Annotations

COIL supports debug annotations in the source code:

```
DEBUG_ASSERT condition, message
DEBUG_PRINT format, [values...]
DEBUG_TRACE name, [values...]
DEBUG_REGION_BEGIN name
DEBUG_REGION_END name
```

### 7.2 Value Annotations

For annotating values for debugging:

```
DEBUG_VALUE name, value
DEBUG_TYPE_HINT variable, type
DEBUG_ALIAS name, variable
```

### 7.3 Performance Annotations

For debugging performance issues:

```
DEBUG_TIMER name, [precision]
DEBUG_COUNTER name
DEBUG_MEMORY_USAGE
DEBUG_BANDWIDTH
```

## 8. Debug Information Compression

### 8.1 Compression Techniques

COIL employs multiple techniques to reduce debug information size:

1. **String Table Deduplication**: Share common strings
2. **Line Number Compression**: Run-length encoding of line mappings
3. **Type Deduplication**: Share common type information
4. **Incremental Updates**: Store only changes in debug information
5. **Separate Debug Information**: Store debug info separately from code

### 8.2 Compression Header

```c
/**
 * @struct coil_debug_compression_header
 * @brief Header for compressed debug information
 */
typedef struct {
  uint32_t original_size;   /* Original uncompressed size */
  uint32_t compressed_size; /* Compressed size */
  uint32_t algorithm;       /* Compression algorithm */
  uint32_t chunk_count;     /* Number of compressed chunks */
  uint32_t chunk_size;      /* Size of each chunk (except possibly last) */
  uint32_t flags;           /* Compression flags */
} coil_debug_compression_header_t;
```

## 9. Remote Debugging

### 9.1 Remote Protocol

COIL defines a protocol for remote debugging:

```c
/**
 * @struct coil_remote_debug_header
 * @brief Header for remote debugging protocol
 */
typedef struct {
  uint32_t magic;           /* Magic number ('CODR') */
  uint32_t version;         /* Protocol version */
  uint32_t packet_size;     /* Packet size */
  uint32_t sequence;        /* Sequence number */
  uint32_t type;            /* Packet type */
  uint32_t flags;           /* Packet flags */
  uint32_t crc;             /* Error checking */
} coil_remote_debug_header_t;
```

### 9.2 Transport Mechanisms

COIL supports multiple transport mechanisms for remote debugging:

1. **TCP/IP**: Standard network debugging
2. **USB**: Direct USB connection
3. **JTAG**: JTAG debugging interface
4. **Serial**: Serial port connection
5. **Shared Memory**: Local debugging via shared memory
6. **Custom Transports**: Extensible transport mechanism

### 9.3 Security Considerations

For secure remote debugging:

1. **Authentication**: Verify debugger identity
2. **Encryption**: Protect debug communication
3. **Access Control**: Limit debug capabilities
4. **Audit Logging**: Log debug operations
5. **Secure Boot**: Protect against unauthorized debug

## 10. Profiling Support

### 10.1 Performance Counters

COIL provides access to hardware performance counters:

```
PROFILE_COUNTER_CREATE counter, type
PROFILE_COUNTER_START counter
PROFILE_COUNTER_STOP counter
PROFILE_COUNTER_RESET counter
PROFILE_COUNTER_READ value, counter
```

### 10.2 Sampling Profiler

For statistical profiling:

```
PROFILE_SAMPLE_START frequency, [events]
PROFILE_SAMPLE_STOP
PROFILE_REPORT file, format
```

### 10.3 Instrumentation

For explicit code instrumentation:

```
PROFILE_INSTRUMENT_BEGIN function, [options]
PROFILE_INSTRUMENT_END function
PROFILE_FUNCTION_ENTER name
PROFILE_FUNCTION_EXIT name
```

## 11. Memory Debugging

### 11.1 Memory Tracker

For tracking memory allocations and usage:

```
MEM_TRACK_ENABLE [options]
MEM_TRACK_DISABLE
MEM_TRACK_REPORT file, [format]
```

### 11.2 Memory Checks

For validating memory operations:

```
MEM_CHECK_BOUNDS addr, size
MEM_CHECK_LEAK
MEM_CHECK_USE_AFTER_FREE
MEM_CHECK_UNINIT
```

### 11.3 Memory Visualization

For visualizing memory layout and usage:

```
MEM_VISUALIZE region, [format]
MEM_HEATMAP region, metric
MEM_TIMELINE operation, time
```

## 12. JIT Debugging

### 12.1 JIT Compilation Interface

COIL supports debugging of just-in-time compiled code:

```
JIT_DEBUG_REGISTER module, addr, size, source
JIT_DEBUG_UNREGISTER module
JIT_DEBUG_MAP addr, source_location
```

### 12.2 JIT Metadata

For providing debug information for JIT-compiled code:

```c
/**
 * @struct coil_jit_debug_info
 * @brief Debug information for JIT-compiled code
 */
typedef struct {
  uint32_t module_id;       /* JIT module identifier */
  uint32_t code_addr;       /* Code address */
  uint32_t code_size;       /* Code size */
  uint32_t debug_data;      /* Debug data address */
  uint32_t debug_size;      /* Debug data size */
  uint32_t source_map;      /* Source map address */
  uint32_t flags;           /* JIT debug flags */
} coil_jit_debug_info_t;
```

## 13. Core Dump Format

### 13.1 Core Dump Header

COIL defines a standard format for core dumps:

```c
/**
 * @struct coil_core_dump_header
 * @brief Header for COIL core dump
 */
typedef struct {
  uint32_t magic;           /* Magic number ('COCD') */
  uint32_t version;         /* Format version */
  uint32_t platform;        /* Platform identifier */
  uint32_t timestamp;       /* Timestamp */
  uint32_t process_id;      /* Process identifier */
  uint32_t thread_count;    /* Number of threads */
  uint32_t section_count;   /* Number of sections */
  uint32_t sections_offset; /* Offset to section table */
  uint32_t reason;          /* Reason for core dump */
  uint32_t signal;          /* Signal that caused dump (if any) */
} coil_core_dump_header_t;
```

### 13.2 Core Dump Sections

| Section | Description |
|---------|-------------|
| REGISTERS | Register state |
| MEMORY | Memory contents |
| THREADS | Thread information |
| MODULES | Loaded modules |
| SYMBOLS | Symbol information |
| STACK | Stack traces |
| PROCESSOR | Processor state |
| DEVICE | Device-specific state |
| METADATA | Additional metadata |

### 13.3 Core Dump Analysis

COIL provides tools for analyzing core dumps:

```
DUMP_ANALYZE file, [options]
DUMP_EXTRACT section, output, [options]
DUMP_COMPARE file1, file2, [options]
DUMP_SYMBOLIZE file, symbol_path, [options]
```

## 14. Debugger Integration

### 14.1 Debugger Extension Interface

COIL defines an interface for debugger extensions:

```c
/**
 * @struct coil_debug_extension
 * @brief Debugger extension interface
 */
typedef struct {
  uint32_t interface_version; /* Interface version */
  uint32_t extension_id;      /* Extension identifier */
  uint32_t capabilities;      /* Extension capabilities */
  void* (*initialize)(void);   /* Initialization function */
  void (*shutdown)(void);      /* Shutdown function */
  uint32_t (*command)(const char* cmd, char* result, uint32_t size); /* Command handler */
  uint32_t (*event_callback)(uint32_t event_type, void* data); /* Event callback */
} coil_debug_extension_t;
```

### 14.2 Custom Visualizers

COIL supports custom data visualizers:

```
VISUALIZER_REGISTER name, type, handler
VISUALIZER_UNREGISTER name
VISUALIZER_APPLY value, visualizer, [options]
```

### 14.3 Scripting Interface

For debugger scripting support:

```
SCRIPT_LOAD file
SCRIPT_EVAL code
SCRIPT_SET_HOOK event, handler
SCRIPT_CALL function, [args...]
```

## 15. Implementation Guidelines

### 15.1 Debug Information Generation

When generating debug information:

1. Generate debug information in parallel with code generation
2. Use incremental updates for efficiency
3. Compress debug information appropriately
4. Validate source-to-instruction mapping
5. Handle optimized code appropriately
6. Ensure accurate variable location tracking

### 15.2 Debugger Implementation

For implementing a COIL debugger:

1. Support multiple target architectures
2. Handle both source-level and machine-level debugging
3. Implement efficient breakpoint management
4. Support heterogeneous debugging
5. Provide user-friendly visualization
6. Scale to large programs and data sets

### 15.3 Performance Considerations

For optimal debugging performance:

1. Lazy loading of debug information
2. Efficient breakpoint implementation
3. Minimize intrusion in target program
4. Optimize remote debugging protocol
5. Cache frequently used debug information
6. Support for parallel debugging

## 16. Future Extensions

Areas for future development in COIL debugging support:

1. **Time-Travel Debugging**: Record and replay execution
2. **Live Editing**: Update code during debugging
3. **AI-Assisted Debugging**: Intelligent debug suggestions
4. **Hardware-Accelerated Debugging**: Use specialized hardware for debugging
5. **Collaborative Debugging**: Multi-user debugging sessions
6. **Fault Tolerance**: Debug partially-failed systems
7. **Record-and-Replay**: Deterministic replay debugging
8. **Predictive Debugging**: Predict potential failures