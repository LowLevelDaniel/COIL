# Version Control and Compatibility (Version 1.0.0)

This document defines the versioning scheme and compatibility guarantees for the COIL project. We follow a modified semantic versioning approach tailored to the needs of a low-level intermediate language specification.

## Versioning Format

COIL uses a `MAJOR.MINOR.PATCH` versioning scheme where:

- **MAJOR**: Significant changes that break backward compatibility
- **MINOR**: New features that maintain backward compatibility
- **PATCH**: Bug fixes and clarifications that maintain backward compatibility

## Current Version and Scope

The current version of COIL is 1.0.0, which establishes a production-ready specification with focus on:

1. **CPU Architecture Support**: Primary focus on CPU architectures (x86-64, ARM, RISC-V), with an architecture-independent design
2. **Primitive Type System**: Support for primitive types (integers, floating-point)
3. **Core Instruction Set**: Complete instruction set for arithmetic, control flow, and memory access
4. **Virtual Register System**: Architecture-independent register access
5. **Configuration Format**: Comprehensive framework for describing processing unit capabilities
6. **ABI Support**: Robust mechanisms for function calls and parameter passing
7. **Object Format**: Complete specification of COF and NCOF binary formats

Features explicitly planned for future versions (and not supported in 1.0.0):
1. **Composite Types**: Structures, unions (planned for v2.0.0)
2. **Array Support**: Native array handling (planned for v2.0.0)
3. **Vector/SIMD Operations**: Dedicated instruction set extensions (planned for v2.0.0)
4. **Standard Library**: Any form of standard library support (planned for v2.0.0)
5. **Non-CPU Processing Units**: Explicit support for GPUs, TPUs, etc. (planned for v3.0.0)
6. **Debugging Information**: Standardized debug information format (planned for v2.0.0)

## Version 1.0.0 Core Guarantees

Version 1.0.0 provides the following guarantees:

1. **Binary Stability**: The binary format for COIL Object Format (COF) is stable and will be compatible with future COIL processors.
2. **Instruction Set Completeness**: The instruction set defined in version 1.0.0 is complete and sufficient for general-purpose computing tasks.
3. **Primitive Type Stability**: The primitive type system is stable and will remain backward compatible in future versions.
4. **ABI Integration**: The ABI mechanisms defined in version 1.0.0 are stable and will be supported in future versions.
5. **Configuration Format Stability**: The configuration format defined in version 1.0.0 is stable and will be compatible with future COIL processors.

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

## Implementation Compatibility

COIL implementations must specify which version of the COIL specification they support. An implementation claiming compatibility with COIL 1.0.0 must:

1. Support all instructions defined in the specification
2. Correctly process all directive formats
3. Handle the configuration format as specified
4. Generate valid NCOF output from valid COF input
5. Maintain binary compatibility guarantees

Implementations may extend COIL with additional features, but must document these extensions and ensure they do not conflict with the standard specification.

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
.version 1.0.0  // Targets COIL version 1.0.0
```

COIL processors should check this directive and:
- Warn if features from later versions are used
- Error if targeting a version they do not support

## Processing Unit Expansion Roadmap

The initial version (1.0.0) of COIL focuses on CPU architectures with common execution models. Future major versions will expand to include:

1. **Version 2.x**: Enhanced CPU support
   - Introduction of standard libraries
   - Introduction of composite types
   - Support for vector/SIMD operations
   - Debugging information format

2. **Version 3.x**: Full multi-processing unit coordination
   - Comprehensive support for GPUs, TPUs, NPUs, etc.
   - Heterogeneous computing frameworks
   - Unified memory model across processing unit types

This gradual expansion ensures that each step maintains backward compatibility while accommodating the fundamentally different execution models of various processing unit types.

## Future Development Process

The path forward for COIL development after 1.0.0 will follow these principles:

1. **User-Driven Enhancement**: Additions to the specification will be driven by real-world use cases and user needs
2. **Implementation Experience**: Changes will be informed by experience with actual implementations
3. **Backward Compatibility**: Changes must respect the compatibility guarantees described in this document
4. **Minimalism**: New features will only be added when there is a clear need that cannot be met by existing mechanisms
5. **Architecture Neutrality**: All additions must maintain COIL's architecture-independent nature

## Version History

- **0.1.0**: Initial pre-release specification with focus on core design
- **1.0.0**: First stable release with complete instruction set and binary format

## Implementation Validation

While no formal certification process exists for COIL implementations, the following validation approaches are recommended:

1. **Self-Testing**: Implementations should include comprehensive test suites
2. **Reference Comparisons**: Results should be compared against reference implementations
3. **Compatibility Testing**: Testing with COF files generated by different tools

## Backward Compatibility Requirements

COIL 1.0.0 establishes the following backward compatibility requirements for future versions:

1. All instructions defined in COIL 1.0.0 must continue to behave as specified
2. The binary format defined in COIL 1.0.0 must continue to be valid
3. Virtual register assignments defined in COIL 1.0.0 must be preserved
4. ABI mechanisms defined in COIL 1.0.0 must continue to work
5. Configurations valid in COIL 1.0.0 must remain valid