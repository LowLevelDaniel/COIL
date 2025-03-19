# Version Control and Compatibility

This document defines the versioning scheme and compatibility guarantees for the COIL project. We follow a modified semantic versioning approach tailored to the needs of a low-level intermediate language specification.

## Versioning Format

COIL uses a `MAJOR.MINOR.PATCH` versioning scheme where:

- **MAJOR**: Significant changes that break backward compatibility
- **MINOR**: New features that maintain backward compatibility
- **PATCH**: Bug fixes and clarifications that maintain backward compatibility

## Current Version and Scope

The current version of COIL is 0.1.0, which establishes a foundation with focus on:

1. **CPU Architecture Support**: Initial implementation focuses on CPU architectures (primarily x86-64, with consideration for ARM and RISC-V)
2. **Basic Type System**: Only primitive types (integers, floating-point) are supported
3. **Core Instruction Set**: Fundamental operations for arithmetic, control flow, and memory access
4. **Virtual Register System**: Architecture-independent register access
5. **Configuration Format**: Framework for describing processing unit capabilities
6. **ABI Support**: Basic mechanisms for function calls and parameter passing

Features explicitly planned for future versions (and not supported in 0.1.0):
1. **Composite Types**: Structures, unions (planned for v2.0.0)
2. **Array Support**: Native array handling (planned for v2.0.0)
3. **Vector/SIMD Operations**: Dedicated instruction set extensions (planned for v2.0.0)
4. **Standard Library**: Any form of standard library support (planned for v2.0.0)
5. **Non-CPU Processing Units**: Explicit support for GPUs, TPUs, etc. (planned for v3.0.0)

## Version 1.0.0 Core Focus

Version 1.0.0 will focus on providing a complete and stable specification for:

1. Basic assembly-like instruction set
2. Primitive types and operations
3. Virtual register system for common architectures
4. Essential directives for assembly control
5. Stable binary format for both COF and NCOF
6. Configuration format for describing processing unit capabilities
7. ABI integration for major platforms

Version 1.0.0 intentionally excludes:
1. Standard library functionality
2. Composite types (structures, unions, arrays)
3. Vector/SIMD operations
4. Support for non-CPU processing units

## Version Number Assignment

### Major Version (X.0.0)

A major version change indicates substantial changes that break backward compatibility:

- Changes to the binary format that alter how existing COIL binaries are interpreted
- Removal of previously deprecated features
- Significant modifications to core semantics that affect how code behaves
- Introduction of new mandatory features that older processors cannot support

All major releases must be thoroughly validated with the specification and undergo a formal review process by at least three core team members.

#### Transition Period

When a new major version is released:
- The previous major version enters a maintenance mode for at least 12 months
- Critical clarifications and corrections will still be applied to the previous major version
- Clear migration guides must be provided

### Minor Version (0.X.0)

Minor version increases represent added functionality that maintains backward compatibility:

- New instructions that don't affect existing code
- Extended type support that preserves existing type semantics
- Additional directives that don't alter existing directive behavior
- Enhanced configuration options
- New optimization opportunities

All minor releases must preserve binary compatibility with previous minor versions within the same major version. This means:
- Any valid COIL binary from version X.Y.Z will remain valid in version X.(Y+N).Z
- All behaviors guaranteed in X.Y.Z will be preserved in X.(Y+N).Z

### Patch Version (0.0.X)

Patch versions address bugs or clarify specifications without changing functionality:

- Documentation clarification or expansion
- Specification ambiguity resolution
- Correction of errors or inconsistencies
- Improved explanations and examples

Patch releases never change the behavior of correctly implemented code. They may address how edge cases or undefined behaviors are handled.

## Feature Lifecycle

Features in COIL follow a well-defined lifecycle:

1. **Experimental**: Introduced with an explicit opt-in flag
2. **Stable**: Guaranteed to work as documented
3. **Deprecated**: Marked for future removal with alternatives provided
4. **Removed**: Eliminated in a major version change

Features must be stable for at least one minor version before they can be deprecated, and must be deprecated for at least one minor version before they can be removed.

## Compatibility Guarantees

### Binary Compatibility

- Binary compatibility is maintained within the same major version
- COIL binaries produced for version X.Y.Z will be correctly processed by tools for version X.(Y+N).(Z+M)

### Feature Compatibility

- Features will not be removed without at least one minor version of deprecation warnings
- Deprecated features will continue to function until the next major version
- New features will be clearly marked with their minimum version requirement

## Pre-1.0.0 Versioning

Prior to the 1.0.0 release, the API is considered unstable and may change without major version increments. Version 0.x.y indicates development status where:

- **x**: Milestone or significant feature set completion
- **y**: Incremental improvements or fixes

During the 0.x.y phase:
- Minor version increments (0.x.0) may contain breaking changes
- Breaking changes will be documented clearly
- Major architectural decisions may be revisited

## Release Process

### Release Preparation

1. **Documentation Review**: Ensure all features are properly documented
2. **Version Number Assignment**: Following the rules above
3. **Changelog Generation**: Detailed notes on all changes

### Release Publication

1. **Tagged Release**: Git tag with version number
2. **Release Package**: Complete specification bundle
3. **Announcement**: Public notification with highlights

## Version Identification in Code

COIL code can specify its target version using the `.version` directive:

```
.version 1.2.3  // Targets COIL version 1.2.3
```

COIL processors should check this directive and:
- Warn if features from later versions are used
- Error if targeting a version they do not support

## Processing Unit Expansion Roadmap

The initial versions (0.x.0 and 1.0.0) of COIL focus on CPU architectures with common execution models. Future major versions will expand to include:

1. **Version 2.x**: Enhanced CPU support plus initial GPU support
   - Introduction of standard libraries
   - Introduction of composite types
   - Support for vector/SIMD operations

2. **Version 3.x**: Full multi-processing unit coordination
   - Comprehensive support for GPUs, TPUs, NPUs, etc.
   - Heterogeneous computing frameworks
   - Unified memory model across processing unit types

This gradual expansion ensures that each step maintains backward compatibility while accommodating the fundamentally different execution models of various processing unit types.