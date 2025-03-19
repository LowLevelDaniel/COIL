# COIL Standard Library (Version 0.1.0)

The COIL Standard Library provides a set of common functions and modules that can be used across different platforms. The library is organized into levels of increasing complexity and platform dependence, allowing developers to choose the appropriate level for their specific needs.

## Library Structure

The COIL Standard Library is divided into three main levels:

1. **Simple Standard (SSTD)** - Level 1
   - Core functionality for all environments
   - Can be statically linked
   - No operating system dependencies
   - Suitable for embedded and bare-metal environments

2. **Extended Standard (ESTD)** - Level 2
   - Extended functionality for operating system environments
   - Typically dynamically linked
   - Requires basic operating system features
   - Suitable for general application development

3. **Graphics Standard (GSTD)** - Level 3
   - Graphics and user interface functionality
   - Requires display system support
   - Depends on operating system graphics capabilities
   - Suitable for application and game development

## Library Versioning

The COIL Standard Library uses a year-based versioning scheme similar to C/C++ standards:

```
COIL-STD-YYYY[a-z]
```

For example:
- COIL-STD-2023a - Initial standard library release
- COIL-STD-2024a - First major update

Minor updates within a year use letter suffixes (a, b, c, etc.).

## Level 0 - RAW

Level 0 is not technically part of the standard library but represents bare COIL code with no library dependencies. This level is mentioned for completeness and to define the baseline for comparisons with higher levels.

## Level 1 - Simple Standard (SSTD)

The Simple Standard provides core functionality that can operate without an operating system. It is designed to be small, efficient, and suitable for embedded systems or bare-metal environments.

### SSTD Implementation

The SSTD can be implemented in two ways:

1. **Static Library**: Linked directly into the executable
2. **Header-Only**: Implemented as inline functions in headers

## Level 2 - Extended Standard (ESTD)

The Extended Standard builds upon the SSTD and adds functionality that requires an operating system environment. It is designed for general application development on platforms with standard OS services.

### ESTD Implementation

The ESTD is typically implemented as a dynamically linked library, with implementations specific to each supported operating system (Linux, Windows, etc.).

## Level 3 - Graphics Standard (GSTD)

The Graphics Standard builds upon the ESTD and adds graphics and user interface functionality. It requires an operating system with display capabilities.

### GSTD Implementation

The GSTD is implemented as a dynamically linked library, with implementations specific to each supported display system (X11, Wayland, Windows, etc.).

## Standard Library Compilation

The COIL standard library compilation process depends on the level:

## Implementation Requirements

COIL assemblers should:

1. Provide a mechanism to include standard library functions
2. Support both static and dynamic linking of standard library modules
3. Provide platform-specific implementations where needed
4. Maintain consistent behavior across different platforms

## Future Extensions

Future versions of the COIL standard library will include:

1. More comprehensive graphics and user interface capabilities
2. Multimedia support (audio, video)
3. Network protocol implementations
4. Database access
5. Parallel computing support
6. Advanced data structures and algorithms

These extensions will be defined in later specification versions.