# COIL Standard Library

The COIL Standard Library provides a set of essential functions and utilities that wrap system calls and provide optimized implementations of common operations. This library is an optional feature that COIL assemblers can include, but it's not required for bare metal code development.

## 1. Overview

### 1.1 Purpose

The COIL Standard Library serves multiple purposes:

1. **System Call Abstraction**: Providing consistent wrappers around operating system functionality
2. **Cross-Architecture Support**: Implementing features that may not be available natively on all architectures
3. **Device Interaction**: Facilitating communication with different processing units
4. **Standard Algorithms**: Providing optimized implementations of common algorithms
5. **Memory Management**: Offering memory allocation and management utilities

### 1.2 Implementation Approach

The standard library follows these principles:

1. **Minimal Dependencies**: Each component has minimal dependencies on other components
2. **Optimized for Size and Speed**: Implementations are optimized for both binary size and execution speed
3. **Architecture-Aware**: Takes advantage of architecture-specific features when available
4. **Fallback Implementations**: Provides software implementations when hardware features are unavailable

### 1.3 Modularity

The standard library is divided into modules that can be selectively included based on the needs of the application:

- **Core**: Essential functionality required by most applications
- **System**: Operating system interfaces
- **Memory**: Memory allocation and management
- **String**: String manipulation functions
- **Math**: Mathematical functions
- **IO**: Input/output operations
- **Thread**: Threading and synchronization
- **Device**: Device interaction
- **Net**: Networking
- **Crypto**: Cryptography

## 2. Core Module

The Core module provides essential functionality that forms the foundation of the standard library.

### 2.1 Types

```
# Basic type definitions
typedef i8 int8_t
typedef i16 int16_t
typedef i32 int32_t
typedef i64 int64_t

typedef u8 uint8_t
typedef u16 uint16_t
typedef u32 uint32_t
typedef u64 uint64_t

typedef addr.u8 ptr_t
typedef u64 size_t
typedef i64 ssize_t
typedef i32 error_t
```

### 2.2 Constants

```
const i32 SUCCESS = 0
const i32 ERROR = -1
const i32 TRUE = 1
const i32 FALSE = 0
const ptr_t NULL = 0
```

### 2.3 Error Handling

```
procedure error_string(error_t code) -> ptr_t {
    # Returns a string describing the error code
}

procedure set_error(error_t code) {
    # Sets the last error code
}

procedure get_error() -> error_t {
    # Gets the last error code
}
```

### 2.4 Initialization

```
procedure initialize() -> i32 {
    # Initializes the standard library
    # Returns SUCCESS or ERROR
}

procedure cleanup() {
    # Cleans up resources used by the standard library
}
```

## 3. System Module

The System module provides interfaces to operating system functionality.

### 3.1 Process Management

```
procedure process_create(ptr_t path, ptr_t args[]) -> i32 {
    # Creates a new process
    # Returns process ID or ERROR
}

procedure process_exit(i32 status) -> void {
    # Exits the current process with the given status code
}

procedure process_wait(i32 pid, ptr_t status) -> i32 {
    # Waits for a process to terminate
    # Returns SUCCESS or ERROR
}
```

### 3.2 File System

```
procedure fs_open(ptr_t path, i32 flags) -> i32 {
    # Opens a file
    # Returns file descriptor or ERROR
}

procedure fs_close(i32 fd) -> i32 {
    # Closes a file
    # Returns SUCCESS or ERROR
}

procedure fs_read(i32 fd, ptr_t buffer, size_t size) -> ssize_t {
    # Reads from a file
    # Returns number of bytes read or ERROR
}

procedure fs_write(i32 fd, ptr_t buffer, size_t size) -> ssize_t {
    # Writes to a file
    # Returns number of bytes written or ERROR
}

procedure fs_seek(i32 fd, i64 offset, i32 whence) -> i64 {
    # Sets the file position
    # Returns new position or ERROR
}
```

### 3.3 Environment

```
procedure env_get(ptr_t name) -> ptr_t {
    # Gets an environment variable
    # Returns variable value or NULL
}

procedure env_set(ptr_t name, ptr_t value) -> i32 {
    # Sets an environment variable
    # Returns SUCCESS or ERROR
}

procedure env_unset(ptr_t name) -> i32 {
    # Unsets an environment variable
    # Returns SUCCESS or ERROR
}
```

## 4. Memory Module

The Memory module provides memory allocation and management utilities.

### 4.1 Allocation

```
procedure mem_alloc(size_t size) -> ptr_t {
    # Allocates memory
    # Returns pointer to allocated memory or NULL
}

procedure mem_realloc(ptr_t ptr, size_t size) -> ptr_t {
    # Reallocates memory
    # Returns pointer to reallocated memory or NULL
}

procedure mem_free(ptr_t ptr) -> void {
    # Frees allocated memory
}
```

### 4.2 Memory Operations

```
procedure mem_copy(ptr_t dest, ptr_t src, size_t size) -> ptr_t {
    # Copies memory
    # Returns dest
}

procedure mem_move(ptr_t dest, ptr_t src, size_t size) -> ptr_t {
    # Moves memory (handles overlapping regions)
    # Returns dest
}

procedure mem_set(ptr_t dest, i32 value, size_t size) -> ptr_t {
    # Sets memory to a value
    # Returns dest
}

procedure mem_compare(ptr_t a, ptr_t b, size_t size) -> i32 {
    # Compares memory
    # Returns 0 if equal, <0 if a<b, >0 if a>b
}
```

### 4.3 Memory Mapping

```
procedure mem_map(ptr_t addr, size_t length, i32 prot, i32 flags) -> ptr_t {
    # Maps memory
    # Returns mapped address or NULL
}

procedure mem_unmap(ptr_t addr, size_t length) -> i32 {
    # Unmaps memory
    # Returns SUCCESS or ERROR
}

procedure mem_protect(ptr_t addr, size_t length, i32 prot) -> i32 {
    # Changes memory protection
    # Returns SUCCESS or ERROR
}
```

## 5. String Module

The String module provides string manipulation functions.

### 5.1 String Operations

```
procedure str_length(ptr_t str) -> size_t {
    # Returns the length of a string
}

procedure str_copy(ptr_t dest, ptr_t src) -> ptr_t {
    # Copies a string
    # Returns dest
}

procedure str_concat(ptr_t dest, ptr_t src) -> ptr_t {
    # Concatenates strings
    # Returns dest
}

procedure str_compare(ptr_t a, ptr_t b) -> i32 {
    # Compares strings
    # Returns 0 if equal, <0 if a<b, >0 if a>b
}
```

### 5.2 String Search

```
procedure str_find(ptr_t haystack, ptr_t needle) -> ptr_t {
    # Finds a substring
    # Returns pointer to the substring or NULL
}

procedure str_find_char(ptr_t str, i32 c) -> ptr_t {
    # Finds a character
    # Returns pointer to the character or NULL
}

procedure str_find_last_char(ptr_t str, i32 c) -> ptr_t {
    # Finds the last occurrence of a character
    # Returns pointer to the character or NULL
}
```

### 5.3 String Conversion

```
procedure str_to_int(ptr_t str) -> i32 {
    # Converts a string to an integer
}

procedure str_to_long(ptr_t str) -> i64 {
    # Converts a string to a long integer
}

procedure str_to_float(ptr_t str) -> f32 {
    # Converts a string to a float
}

procedure str_to_double(ptr_t str) -> f64 {
    # Converts a string to a double
}

procedure int_to_str(i32 value, ptr_t buffer, size_t size) -> ptr_t {
    # Converts an integer to a string
    # Returns buffer
}
```

## 6. Math Module

The Math module provides mathematical functions.

### 6.1 Basic Functions

```
procedure math_abs(i32 x) -> i32 {
    # Returns the absolute value of x
}

procedure math_min(i32 a, i32 b) -> i32 {
    # Returns the minimum of a and b
}

procedure math_max(i32 a, i32 b) -> i32 {
    # Returns the maximum of a and b
}

procedure math_clamp(i32 x, i32 min, i32 max) -> i32 {
    # Clamps x between min and max
}
```

### 6.2 Floating-Point Functions

```
procedure math_sqrt(f64 x) -> f64 {
    # Returns the square root of x
}

procedure math_pow(f64 x, f64 y) -> f64 {
    # Returns x raised to the power of y
}

procedure math_sin(f64 x) -> f64 {
    # Returns the sine of x
}

procedure math_cos(f64 x) -> f64 {
    # Returns the cosine of x
}

procedure math_tan(f64 x) -> f64 {
    # Returns the tangent of x
}

procedure math_log(f64 x) -> f64 {
    # Returns the natural logarithm of x
}

procedure math_exp(f64 x) -> f64 {
    # Returns e raised to the power of x
}
```

### 6.3 Random Number Generation

```
procedure math_rand_seed(u32 seed) -> void {
    # Seeds the random number generator
}

procedure math_rand() -> i32 {
    # Returns a random integer
}

procedure math_rand_range(i32 min, i32 max) -> i32 {
    # Returns a random integer in the range [min, max]
}

procedure math_rand_float() -> f32 {
    # Returns a random float in the range [0, 1)
}
```

## 7. IO Module

The IO module provides input/output operations.

### 7.1 Standard I/O

```
procedure io_print(ptr_t str) -> void {
    # Prints a string to standard output
}

procedure io_println(ptr_t str) -> void {
    # Prints a string followed by a newline to standard output
}

procedure io_printf(ptr_t format, ...) -> i32 {
    # Prints a formatted string to standard output
    # Returns number of characters printed or ERROR
}

procedure io_getchar() -> i32 {
    # Gets a character from standard input
    # Returns the character or EOF
}

procedure io_gets(ptr_t buffer, size_t size) -> ptr_t {
    # Gets a line from standard input
    # Returns buffer or NULL
}

procedure io_scanf(ptr_t format, ...) -> i32 {
    # Scans formatted input from standard input
    # Returns number of items scanned or EOF
}
```

### 7.2 File I/O

```
procedure io_fopen(ptr_t path, ptr_t mode) -> ptr_t {
    # Opens a file
    # Returns file handle or NULL
}

procedure io_fclose(ptr_t file) -> i32 {
    # Closes a file
    # Returns SUCCESS or ERROR
}

procedure io_fread(ptr_t buffer, size_t size, size_t count, ptr_t file) -> size_t {
    # Reads from a file
    # Returns number of items read
}

procedure io_fwrite(ptr_t buffer, size_t size, size_t count, ptr_t file) -> size_t {
    # Writes to a file
    # Returns number of items written
}

procedure io_fseek(ptr_t file, i64 offset, i32 whence) -> i32 {
    # Sets the file position
    # Returns SUCCESS or ERROR
}

procedure io_ftell(ptr_t file) -> i64 {
    # Gets the file position
    # Returns position or ERROR
}
```

## 8. Thread Module

The Thread module provides threading and synchronization utilities.

### 8.1 Thread Management

```
procedure thread_create(ptr_t function, ptr_t arg) -> ptr_t {
    # Creates a new thread
    # Returns thread handle or NULL
}

procedure thread_join(ptr_t thread) -> i32 {
    # Waits for a thread to terminate
    # Returns SUCCESS or ERROR
}

procedure thread_detach(ptr_t thread) -> i32 {
    # Detaches a thread
    # Returns SUCCESS or ERROR
}

procedure thread_exit(i32 status) -> void {
    # Exits the current thread with the given status code
}

procedure thread_yield() -> void {
    # Yields execution to another thread
}
```

### 8.2 Synchronization

```
procedure mutex_create() -> ptr_t {
    # Creates a mutex
    # Returns mutex handle or NULL
}

procedure mutex_destroy(ptr_t mutex) -> i32 {
    # Destroys a mutex
    # Returns SUCCESS or ERROR
}

procedure mutex_lock(ptr_t mutex) -> i32 {
    # Locks a mutex
    # Returns SUCCESS or ERROR
}

procedure mutex_unlock(ptr_t mutex) -> i32 {
    # Unlocks a mutex
    # Returns SUCCESS or ERROR
}

procedure cond_create() -> ptr_t {
    # Creates a condition variable
    # Returns condition variable handle or NULL
}

procedure cond_destroy(ptr_t cond) -> i32 {
    # Destroys a condition variable
    # Returns SUCCESS or ERROR
}

procedure cond_wait(ptr_t cond, ptr_t mutex) -> i32 {
    # Waits on a condition variable
    # Returns SUCCESS or ERROR
}

procedure cond_signal(ptr_t cond) -> i32 {
    # Signals a condition variable
    # Returns SUCCESS or ERROR
}

procedure cond_broadcast(ptr_t cond) -> i32 {
    # Broadcasts a condition variable
    # Returns SUCCESS or ERROR
}
```

### 8.3 Atomic Operations

```
procedure atomic_load(ptr_t addr) -> i32 {
    # Atomically loads a value
}

procedure atomic_store(ptr_t addr, i32 value) -> void {
    # Atomically stores a value
}

procedure atomic_exchange(ptr_t addr, i32 value) -> i32 {
    # Atomically exchanges values
    # Returns the previous value
}

procedure atomic_compare_exchange(ptr_t addr, i32 expected, i32 desired) -> i32 {
    # Atomically compares and exchanges values
    # Returns SUCCESS if the exchange occurred, otherwise ERROR
}

procedure atomic_add(ptr_t addr, i32 value) -> i32 {
    # Atomically adds a value
    # Returns the previous value
}
```

## 9. Device Module

The Device module facilitates interaction with different processing units.

### 9.1 Device Management

```
procedure device_enum() -> i32 {
    # Enumerates available devices
    # Returns the number of devices or ERROR
}

procedure device_get_info(i32 index, ptr_t info) -> i32 {
    # Gets information about a device
    # Returns SUCCESS or ERROR
}

procedure device_open(i32 index) -> ptr_t {
    # Opens a device
    # Returns device handle or NULL
}

procedure device_close(ptr_t device) -> i32 {
    # Closes a device
    # Returns SUCCESS or ERROR
}
```

### 9.2 Memory Transfer

```
procedure device_mem_alloc(ptr_t device, size_t size) -> ptr_t {
    # Allocates memory on a device
    # Returns device memory pointer or NULL
}

procedure device_mem_free(ptr_t device, ptr_t mem) -> i32 {
    # Frees memory on a device
    # Returns SUCCESS or ERROR
}

procedure device_mem_copy_to(ptr_t device, ptr_t dest, ptr_t src, size_t size) -> i32 {
    # Copies memory from host to device
    # Returns SUCCESS or ERROR
}

procedure device_mem_copy_from(ptr_t device, ptr_t dest, ptr_t src, size_t size) -> i32 {
    # Copies memory from device to host
    # Returns SUCCESS or ERROR
}
```

### 9.3 Code Execution

```
procedure device_load_code(ptr_t device, ptr_t code, size_t size) -> ptr_t {
    # Loads code onto a device
    # Returns code handle or NULL
}

procedure device_unload_code(ptr_t device, ptr_t code) -> i32 {
    # Unloads code from a device
    # Returns SUCCESS or ERROR
}

procedure device_execute(ptr_t device, ptr_t code, ptr_t args, size_t arg_size) -> i32 {
    # Executes code on a device
    # Returns SUCCESS or ERROR
}

procedure device_execute_async(ptr_t device, ptr_t code, ptr_t args, size_t arg_size) -> ptr_t {
    # Asynchronously executes code on a device
    # Returns operation handle or NULL
}

procedure device_wait(ptr_t device, ptr_t operation) -> i32 {
    # Waits for an asynchronous operation to complete
    # Returns SUCCESS or ERROR
}
```

### 9.4 Specialized Device Interfaces

#### 9.4.1 GPU Interface

```
procedure gpu_init(ptr_t device) -> i32 {
    # Initializes the GPU interface
    # Returns SUCCESS or ERROR
}

procedure gpu_shutdown(ptr_t device) -> i32 {
    # Shuts down the GPU interface
    # Returns SUCCESS or ERROR
}

procedure gpu_compile_shader(ptr_t device, ptr_t source, i32 type) -> ptr_t {
    # Compiles a shader
    # Returns shader handle or NULL
}

procedure gpu_create_program(ptr_t device, ptr_t shaders[], i32 count) -> ptr_t {
    # Creates a program from shaders
    # Returns program handle or NULL
}

procedure gpu_delete_program(ptr_t device, ptr_t program) -> i32 {
    # Deletes a program
    # Returns SUCCESS or ERROR
}

procedure gpu_execute_program(ptr_t device, ptr_t program, i32 work_size) -> i32 {
    # Executes a program
    # Returns SUCCESS or ERROR
}
```

#### 9.4.2 FPGA Interface

```
procedure fpga_init(ptr_t device) -> i32 {
    # Initializes the FPGA interface
    # Returns SUCCESS or ERROR
}

procedure fpga_shutdown(ptr_t device) -> i32 {
    # Shuts down the FPGA interface
    # Returns SUCCESS or ERROR
}

procedure fpga_load_bitstream(ptr_t device, ptr_t bitstream, size_t size) -> i32 {
    # Loads a bitstream onto the FPGA
    # Returns SUCCESS or ERROR
}

procedure fpga_execute(ptr_t device, ptr_t input, size_t input_size, ptr_t output, size_t output_size) -> i32 {
    # Executes a function on the FPGA
    # Returns SUCCESS or ERROR
}
```

## 10. Net Module

The Net module provides networking functionality.

### 10.1 Socket Operations

```
procedure socket_create(i32 domain, i32 type, i32 protocol) -> i32 {
    # Creates a socket
    # Returns socket descriptor or ERROR
}

procedure socket_close(i32 sockfd) -> i32 {
    # Closes a socket
    # Returns SUCCESS or ERROR
}

procedure socket_bind(i32 sockfd, ptr_t addr, size_t addrlen) -> i32 {
    # Binds a socket to an address
    # Returns SUCCESS or ERROR
}

procedure socket_listen(i32 sockfd, i32 backlog) -> i32 {
    # Listens for connections
    # Returns SUCCESS or ERROR
}

procedure socket_accept(i32 sockfd, ptr_t addr, ptr_t addrlen) -> i32 {
    # Accepts a connection
    # Returns new socket descriptor or ERROR
}

procedure socket_connect(i32 sockfd, ptr_t addr, size_t addrlen) -> i32 {
    # Connects to an address
    # Returns SUCCESS or ERROR
}

procedure socket_send(i32 sockfd, ptr_t buf, size_t len, i32 flags) -> ssize_t {
    # Sends data
    # Returns number of bytes sent or ERROR
}

procedure socket_recv(i32 sockfd, ptr_t buf, size_t len, i32 flags) -> ssize_t {
    # Receives data
    # Returns number of bytes received or ERROR
}
```

### 10.2 Address Resolution

```
procedure addr_resolve(ptr_t hostname, ptr_t service, ptr_t hints, ptr_t result) -> i32 {
    # Resolves a hostname to an address
    # Returns SUCCESS or ERROR
}

procedure addr_to_string(ptr_t addr, ptr_t buffer, size_t buflen) -> ptr_t {
    # Converts an address to a string
    # Returns buffer or NULL
}

procedure addr_from_string(ptr_t str, ptr_t addr) -> i32 {
    # Converts a string to an address
    # Returns SUCCESS or ERROR
}
```

### 10.3 Higher-Level Protocols

```
procedure http_get(ptr_t url, ptr_t response, size_t response_size) -> ssize_t {
    # Performs an HTTP GET request
    # Returns response size or ERROR
}

procedure http_post(ptr_t url, ptr_t data, size_t data_size, ptr_t response, size_t response_size) -> ssize_t {
    # Performs an HTTP POST request
    # Returns response size or ERROR
}
```

## 11. Crypto Module

The Crypto module provides cryptographic functions.

### 11.1 Hash Functions

```
procedure crypto_sha256(ptr_t data, size_t size, ptr_t hash) -> i32 {
    # Computes the SHA-256 hash of data
    # Returns SUCCESS or ERROR
}

procedure crypto_sha512(ptr_t data, size_t size, ptr_t hash) -> i32 {
    # Computes the SHA-512 hash of data
    # Returns SUCCESS or ERROR
}

procedure crypto_md5(ptr_t data, size_t size, ptr_t hash) -> i32 {
    # Computes the MD5 hash of data
    # Returns SUCCESS or ERROR
}
```

### 11.2 Encryption

```
procedure crypto_aes_encrypt(ptr_t data, size_t size, ptr_t key, ptr_t iv, ptr_t output) -> i32 {
    # Encrypts data using AES
    # Returns SUCCESS or ERROR
}

procedure crypto_aes_decrypt(ptr_t data, size_t size, ptr_t key, ptr_t iv, ptr_t output) -> i32 {
    # Decrypts data using AES
    # Returns SUCCESS or ERROR
}
```

### 11.3 Random Number Generation

```
procedure crypto_random(ptr_t buffer, size_t size) -> i32 {
    # Fills buffer with cryptographically secure random bytes
    # Returns SUCCESS or ERROR
}
```

## 12. Implementation Details

### 12.1 Architecture-Specific Optimization

The standard library detects the capabilities of the target architecture and selects the most efficient implementation for each function:

1. **SIMD Acceleration**: Uses vector instructions for memory and string operations where available
2. **Hardware Cryptography**: Uses hardware crypto acceleration for hash and encryption functions
3. **Memory Model Awareness**: Adapts synchronization primitives to the target memory model
4. **Cache-Aware Algorithms**: Optimizes algorithms based on cache characteristics

### 12.2 Feature Test Macros

The standard library provides feature test macros that allow applications to check for the availability of specific features:

```
# Test if a feature is available
if (HAS_FEATURE_XYZ) {
    # Use the feature
} else {
    # Use an alternative
}
```

### 12.3 Conditional Compilation

Parts of the standard library can be conditionally compiled based on the target environment:

```
# For bare metal environments
#ifdef BARE_METAL
procedure io_print(ptr_t str) {
    # Direct hardware implementation
}
#else
procedure io_print(ptr_t str) {
    # OS-based implementation
}
#endif
```

### 12.4 Thread Safety

The standard library ensures thread safety through several mechanisms:

1. **Thread-Local Storage**: For state that needs to be thread-specific
2. **Atomic Operations**: For lock-free synchronization
3. **Mutex Protection**: For operations that require exclusive access
4. **Reentrant Design**: Functions are designed to be reentrant where possible

## 13. Building and Using the Standard Library

### 13.1 Compilation

The standard library can be compiled in several configurations:

1. **Static Library**: Linked into the application at compile time
2. **Shared Library**: Dynamically linked at runtime
3. **Header-Only**: Included directly in the application source code
4. **Built-In**: Integrated directly into the COIL assembler

### 13.2 Usage in HOIL

```
# Import the entire standard library
import std

# Import specific modules
import std.io
import std.string

procedure main() {
    # Use standard library functions
    std.io.println("Hello, world!")
    
    var ptr_t str = "Hello"
    var size_t len = std.string.str_length(str)
}
```

### 13.3 Disabling the Standard Library

For bare metal applications, the standard library can be completely disabled:

```
# Compile without the standard library
hoil-assembler --no-stdlib -o output.coil input.hoil
```

## 14. Extending the Standard Library

### 14.1 Custom Modules

Developers can create custom modules that integrate with the standard library:

```
module my_module

# Export a procedure
export procedure my_function() {
    # Implementation
}
```

### 14.2 Platform-Specific Extensions

Platform-specific extensions can be provided for different operating systems or hardware:

```
# Windows-specific extension
#ifdef PLATFORM_WINDOWS
procedure windows_specific_function() {
    # Windows-specific implementation
}
#endif

# Linux-specific extension
#ifdef PLATFORM_LINUX
procedure linux_specific_function() {
    # Linux-specific implementation
}
#endif
```

### 14.3 Hardware-Specific Extensions

Extensions can also target specific hardware features:

```
# Extension for a specific GPU
#ifdef GPU_VENDOR_XYZ
procedure xyz_gpu_function() {
    # Vendor-specific implementation
}
#endif
```