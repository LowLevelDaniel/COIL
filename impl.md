# COIL Reference Implementation Guide

## Introduction

This document provides a practical guide for implementing COIL toolchains, including a reference implementation architecture, essential components, and development guidelines. It aims to bridge the gap between the COIL specification and actual implementation, providing concrete guidance for developers working on the first generation of COIL tools.

## Reference Implementation Architecture

The reference implementation of COIL consists of the following key components:

1. **COIL Compiler Infrastructure Library (libcoil)** - A core library implementing the fundamental COIL abstractions
2. **COIL Assembler (coilasm)** - A tool that processes COIL assembly and generates COF binaries
3. **COIL Disassembler (coildis)** - A tool that converts COF binaries back to COIL assembly
4. **COIL Target Assembler (coiltasm)** - A tool that converts COF binaries to target-specific NOF binaries
5. **COIL Linker (coilld)** - A tool that links multiple COF/NOF files into a unified binary
6. **COIL Validator (coilval)** - A tool that validates COIL binary formats

### Implementation Language and Dependencies

The reference implementation is written in C++17 with minimal dependencies to ensure portability and ease of integration. Core dependencies include:

- **C++ Standard Library** - For basic data structures and algorithms
- **LLVM (optional)** - For leveraging existing compiler infrastructure (optional component)
- **Capstone (optional)** - For disassembly capabilities (optional component)

## Core Library Architecture (libcoil)

The COIL library is designed with a modular architecture to enable reuse across different tools:

```
libcoil/
├── core/           # Core abstractions and data structures
│   ├── instruction.h     # Instruction representation
│   ├── operand.h         # Operand representation
│   ├── register.h        # Register system
│   ├── variable.h        # Variable management system
│   ├── abi.h             # ABI definitions
│   └── type.h            # Type system
├── binary/         # Binary format handling
│   ├── cof.h             # COIL Object Format
│   ├── nof.h             # Native Object Format
│   ├── section.h         # Section management
│   ├── symbol.h          # Symbol table handling
│   └── relocation.h      # Relocation handling
├── target/         # Target architecture support
│   ├── target.h          # Target abstraction
│   ├── x86_64.h          # x86-64 implementation
│   ├── arm64.h           # ARM64 implementation
│   └── riscv.h           # RISC-V implementation
├── transform/      # Transformation passes
│   ├── pass.h            # Pass infrastructure
│   ├── abi_lowering.h    # ABI implementation
│   ├── register_alloc.h  # Register allocation
│   ├── instruction_selection.h # Instruction selection
│   └── optimization.h    # Optimization passes
└── util/           # Utility functions
    ├── logger.h          # Logging infrastructure
    ├── options.h         # Option parsing
    └── diagnostics.h     # Error handling
```

### Core Data Structures

The reference implementation defines key data structures that represent the COIL abstractions:

```cpp
// Instruction representation
class Instruction {
private:
    uint8_t category;
    uint8_t operation;
    std::vector<std::unique_ptr<Operand>> operands;
    std::vector<uint8_t> extendedData;

public:
    // Constructors and methods
    Instruction(uint8_t cat, uint8_t op);
    void addOperand(std::unique_ptr<Operand> op);
    void setExtendedData(const std::vector<uint8_t>& data);
    
    // Binary encoding/decoding
    std::vector<uint8_t> encode() const;
    static Instruction decode(const uint8_t* data, size_t& offset);
    
    // Transformation methods
    void transformToTarget(const Target& target);
};

// Variable representation
class Variable {
private:
    uint8_t id;
    Type type;
    uint8_t storageClass;
    uint32_t flags;
    uint32_t lifetimeStart;
    uint32_t lifetimeEnd;
    std::optional<std::vector<uint8_t>> initialValue;
    
public:
    // Constructors and methods
    Variable(uint8_t id, Type type, uint8_t storageClass = COIL_VAR_STORAGE_AUTO);
    void setLifetime(uint32_t start, uint32_t end);
    void setInitialValue(const std::vector<uint8_t>& value);
    
    // Storage allocation
    void allocateStorage(RegisterAllocator& regAlloc);
    
    // Binary encoding/decoding
    std::vector<uint8_t> encode() const;
    static Variable decode(const uint8_t* data, size_t& offset);
};

// Register mapping
class RegisterMapping {
private:
    uint8_t virtualRegId;
    uint8_t physicalRegId;
    uint8_t physicalRegClass;
    uint8_t flags;
    
public:
    // Constructors and methods
    RegisterMapping(uint8_t vId, uint8_t pId, uint8_t pClass, uint8_t flags = 0);
    
    // Binary encoding/decoding
    std::vector<uint8_t> encode() const;
    static RegisterMapping decode(const uint8_t* data, size_t& offset);
};

// ABI representation
class ABI {
private:
    uint32_t id;
    std::string name;
    uint8_t paramPassingModel;
    uint8_t returnValueModel;
    uint8_t stackAlignment;
    std::vector<uint8_t> intParamRegs;
    std::vector<uint8_t> fpParamRegs;
    std::vector<uint8_t> vecParamRegs;
    std::vector<uint8_t> intReturnRegs;
    std::vector<uint8_t> fpReturnRegs;
    std::vector<uint8_t> vecReturnRegs;
    uint32_t callerPreservedRegs;
    uint32_t calleePreservedRegs;
    
public:
    // Constructors and methods
    ABI(uint32_t id, const std::string& name);
    void setParameterRegs(const std::vector<uint8_t>& intRegs, const std::vector<uint8_t>& fpRegs, const std::vector<uint8_t>& vecRegs);
    void setReturnRegs(const std::vector<uint8_t>& intRegs, const std::vector<uint8_t>& fpRegs, const std::vector<uint8_t>& vecRegs);
    void setPreservedRegs(uint32_t callerRegs, uint32_t calleeRegs);
    
    // Parameter passing logic
    void generateParameterPassing(Function& func, const std::vector<OperandPtr>& args);
    void generateReturnValueHandling(Function& func, const OperandPtr& returnValue);
    
    // Binary encoding/decoding
    std::vector<uint8_t> encode() const;
    static ABI decode(const uint8_t* data, size_t& offset);
};
```

### Target Abstraction

The Target class provides a consistent interface for different architectures:

```cpp
class Target {
private:
    uint32_t id;
    uint8_t archClass;
    uint8_t archType;
    uint8_t wordSize;
    uint8_t endianness;
    uint32_t features;
    uint32_t extensions;
    std::vector<RegisterMapping> registerMappings;
    std::shared_ptr<ABI> defaultABI;
    
public:
    // Constructors and methods
    Target(uint32_t id, uint8_t archClass, uint8_t archType, uint8_t wordSize, uint8_t endianness);
    void setFeatures(uint32_t features);
    void setExtensions(uint32_t extensions);
    void addRegisterMapping(const RegisterMapping& mapping);
    void setDefaultABI(std::shared_ptr<ABI> abi);
    
    // Target-specific transformation
    virtual void transformInstruction(Instruction& inst) = 0;
    virtual void allocateRegisters(Function& func) = 0;
    virtual void generatePrologue(Function& func) = 0;
    virtual void generateEpilogue(Function& func) = 0;
    
    // Binary encoding/decoding
    std::vector<uint8_t> encode() const;
    static Target decode(const uint8_t* data, size_t& offset);
};

// Target implementation for x86-64
class X86_64Target : public Target {
public:
    X86_64Target();
    
    void transformInstruction(Instruction& inst) override;
    void allocateRegisters(Function& func) override;
    void generatePrologue(Function& func) override;
    void generateEpilogue(Function& func) override;
};
```

### Binary Format Handling

The binary format classes handle the COIL Object Format (COF) and Native Object Format (NOF):

```cpp
class COFFile {
private:
    COFHeader header;
    std::vector<TargetDef> targets;
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
    std::vector<Relocation> relocations;
    std::vector<VariableInfo> variables;
    std::vector<TypeInfo> types;
    std::string stringTable;
    
public:
    // Constructors and methods
    COFFile();
    
    // Section management
    Section& addSection(const std::string& name, uint32_t type, uint32_t flags);
    void addSymbol(const Symbol& symbol);
    void addRelocation(const Relocation& relocation);
    void addVariable(const VariableInfo& variable);
    void addType(const TypeInfo& type);
    
    // String table management
    uint32_t addString(const std::string& str);
    
    // File I/O
    void write(const std::string& filename) const;
    static COFFile read(const std::string& filename);
};

class NOFFile {
private:
    NOFHeader header;
    std::vector<TargetEntry> targets;
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
    std::vector<Relocation> relocations;
    std::vector<TargetSwitch> targetSwitches;
    std::vector<DataMapping> dataMappings;
    std::string stringTable;
    
public:
    // Constructors and methods
    NOFFile();
    
    // Target management
    void addTarget(const TargetEntry& target);
    void addTargetSwitch(const TargetSwitch& targetSwitch);
    void addDataMapping(const DataMapping& dataMapping);
    
    // File I/O
    void write(const std::string& filename) const;
    static NOFFile read(const std::string& filename);
    
    // Conversion
    static NOFFile convertFromCOF(const COFFile& cof, const std::vector<Target>& targets);
};
```

## Assembler Implementation (coilasm)

The COIL assembler is responsible for converting COIL assembly code to COF binary format. It follows these key implementation steps:

### 1. Parser Implementation

The parser converts COIL assembly text to an abstract syntax tree (AST):

```cpp
class Parser {
private:
    Lexer lexer;
    DiagnosticEngine& diag;
    
public:
    Parser(const std::string& sourceCode, DiagnosticEngine& diag);
    
    // Parsing methods
    std::unique_ptr<Module> parseModule();
    std::unique_ptr<Function> parseFunction();
    std::unique_ptr<Instruction> parseInstruction();
    std::unique_ptr<Operand> parseOperand();
    std::unique_ptr<Variable> parseVariable();
    
    // Error handling
    void emitError(const std::string& message, SourceLocation loc);
};
```

### 2. Code Generation Passes

The code generator applies a series of passes to transform the AST to binary code:

```cpp
class CodeGenerator {
private:
    Module& module;
    DiagnosticEngine& diag;
    TargetManager& targetMgr;
    
public:
    CodeGenerator(Module& module, DiagnosticEngine& diag, TargetManager& targetMgr);
    
    // Code generation passes
    void validateTypes();
    void resolveABIs();
    void trackVariableLifetimes();
    void allocateRegisters();
    void transformInstructions();
    void resolveSymbols();
    void generateRelocations();
    
    // Binary output
    std::unique_ptr<COFFile> generateCOF();
};
```

### 3. Assembly Driver

The main driver coordinates the assembly process:

```cpp
class AssemblerDriver {
private:
    CompilationOptions options;
    DiagnosticEngine diag;
    TargetManager targetMgr;
    
public:
    AssemblerDriver(const CompilationOptions& options);
    
    // Main assembly process
    int assemble(const std::string& inputFile, const std::string& outputFile);
    
    // Helper methods
    std::unique_ptr<Module> parseInput(const std::string& inputFile);
    bool generateOutput(Module& module, const std::string& outputFile);
};
```

## Target Assembler Implementation (coiltasm)

The COIL target assembler converts COF files to NOF files with target-specific machine code:

### 1. Instruction Implementation

For each target architecture, the target assembler implements COIL instructions using native instructions:

```cpp
class InstructionImplementer {
private:
    Target& target;
    InstructionSelector& selector;
    
public:
    InstructionImplementer(Target& target, InstructionSelector& selector);
    
    // Implementation strategies
    NativeInstructionSequence implementDirectly(const Instruction& inst);
    NativeInstructionSequence implementWithSequence(const Instruction& inst);
    NativeInstructionSequence implementWithInlineExpansion(const Instruction& inst);
    NativeInstructionSequence implementWithLibraryCall(const Instruction& inst);
    
    // Main implementation method
    NativeInstructionSequence implement(const Instruction& inst);
};

// Implementation for x86-64
class X86_64InstructionImplementer : public InstructionImplementer {
public:
    X86_64InstructionImplementer(Target& target, InstructionSelector& selector);
    
    // Specialized implementation methods
    NativeInstructionSequence implementMathAdd(const Instruction& inst);
    NativeInstructionSequence implementMemMove(const Instruction& inst);
    NativeInstructionSequence implementCfBranch(const Instruction& inst);
    // ... other instruction implementations
};
```

### 2. Register Allocation

The register allocator maps virtual registers to physical registers:

```cpp
class RegisterAllocator {
private:
    Target& target;
    std::map<uint8_t, uint8_t> virtualToPhysical;
    std::set<uint8_t> allocatedRegisters;
    
public:
    RegisterAllocator(Target& target);
    
    // Allocation methods
    uint8_t allocateRegister(uint8_t virtualReg, uint8_t preferredPhysReg = 0xFF);
    void releaseRegister(uint8_t virtualReg);
    
    // Spilling logic
    void spillRegister(uint8_t virtualReg, MemoryLocation& spillLoc);
    void reloadRegister(uint8_t virtualReg, const MemoryLocation& spillLoc);
    
    // Register mapping
    std::vector<RegisterMapping> generateMappings() const;
};
```

### 3. Target Switching Implementation

The target switcher handles transitions between different architectures:

```cpp
class TargetSwitcher {
private:
    std::vector<Target>& targets;
    
public:
    TargetSwitcher(std::vector<Target>& targets);
    
    // Switch point generation
    TargetSwitch generateSwitchPoint(uint32_t sourceTarget, uint32_t destTarget, 
                                    uint32_t entryPoint, uint32_t returnPoint,
                                    const std::vector<DataMapping>& dataMappings);
    
    // Data mapping
    DataMapping generateDataMapping(uint32_t sourceSymbol, uint32_t destSymbol,
                                  uint32_t mappingType, uint32_t mappingFlags);
};
```

## Linker Implementation (coilld)

The COIL linker combines multiple COF/NOF files into a unified binary:

### 1. Symbol Resolution

The symbol resolver handles symbol references between input files:

```cpp
class SymbolResolver {
private:
    std::vector<COFFile>& inputFiles;
    std::map<std::string, Symbol> symbolTable;
    DiagnosticEngine& diag;
    
public:
    SymbolResolver(std::vector<COFFile>& inputFiles, DiagnosticEngine& diag);
    
    // Symbol resolution
    void collectSymbols();
    void resolveReferences();
    void generateSymbolTable();
    
    // Error handling
    void reportUndefinedSymbols();
};
```

### 2. Section Merging

The section merger combines sections from multiple input files:

```cpp
class SectionMerger {
private:
    std::vector<COFFile>& inputFiles;
    COFFile& outputFile;
    
public:
    SectionMerger(std::vector<COFFile>& inputFiles, COFFile& outputFile);
    
    // Section merging
    void mergeSections();
    void updateRelocations();
    void updateSymbolAddresses();
    
    // Helper methods
    Section mergeSection(const std::vector<Section>& inputSections);
};
```

### 3. Target Switch Handling

The target switch handler manages cross-architecture transitions in the linked binary:

```cpp
class TargetSwitchHandler {
private:
    NOFFile& outputFile;
    std::vector<Target>& targets;
    
public:
    TargetSwitchHandler(NOFFile& outputFile, std::vector<Target>& targets);
    
    // Target switch handling
    void collectTargetSwitches(const std::vector<NOFFile>& inputFiles);
    void updateTargetSwitchAddresses();
    void generateTargetSwitchTable();
    
    // Data mapping
    void collectDataMappings(const std::vector<NOFFile>& inputFiles);
    void updateDataMappingReferences();
    void generateDataMappingTable();
};
```

## Validator Implementation (coilval)

The COIL validator ensures correctness of COIL binary formats:

### 1. Binary Format Validation

The format validator checks the structure of COF/NOF files:

```cpp
class FormatValidator {
private:
    std::vector<uint8_t>& binaryData;
    DiagnosticEngine& diag;
    
public:
    FormatValidator(std::vector<uint8_t>& binaryData, DiagnosticEngine& diag);
    
    // Validation methods
    bool validateCOFFormat();
    bool validateNOFFormat();
    
    // Structure validation
    bool validateHeader();
    bool validateSectionTable();
    bool validateSymbolTable();
    bool validateRelocationTable();
    
    // Error reporting
    void reportFormatError(const std::string& message, size_t offset);
};
```

### 2. Semantic Validation

The semantic validator checks for logical correctness:

```cpp
class SemanticValidator {
private:
    COFFile& cofFile;
    DiagnosticEngine& diag;
    
public:
    SemanticValidator(COFFile& cofFile, DiagnosticEngine& diag);
    
    // Validation methods
    bool validateInstructions();
    bool validateVariableLifetimes();
    bool validateRegisterAllocation();
    bool validateRelocations();
    bool validateTargetSwitches();
    
    // Error reporting
    void reportSemanticError(const std::string& message, const std::string& location);
};
```

## Development Guidelines

### Implementation Strategy

1. **Phased Development**:
   - Phase 1: Implement core library and basic COF binary format support
   - Phase 2: Implement assembler with minimal instruction set and single target
   - Phase 3: Implement target assembler for x86-64
   - Phase 4: Expand to additional targets and implement full instruction set
   - Phase 5: Implement linker and multi-target support

2. **Start with Minimal Implementation**:
   - Begin with a subset of instructions that cover basic functionality
   - Focus on one target architecture (e.g., x86-64) initially
   - Implement basic variable management and register allocation
   - Add optimizations and advanced features incrementally

3. **Testing Strategy**:
   - Develop unit tests for each component
   - Create integration tests using sample COIL programs
   - Implement reference test suite with coverage of all instructions
   - Test cross-architecture functionality with simple examples

### Minimal Working Implementation

A minimal COIL implementation should support:

1. **Instructions**:
   - Control Flow: BR, BRC, CALL, RET, SYSC
   - Memory Operations: MOV, PUSH, POP
   - Arithmetic: ADD, SUB, MUL, DIV
   - Bit Manipulation: AND, OR, XOR, NOT, SHL, SHR
   - Variable Management: DECL, DLT
   - Frame Management: ENTER, LEAVE

2. **Basic Types**:
   - int32, int64
   - fp32, fp64
   - ptr(type)

3. **Single Target Architecture**:
   - x86-64 with System V ABI

4. **Binary Format**:
   - Basic COF structure with sections, symbols, and relocations
   - Simple NOF output with x86-64 machine code

### Test Suite Organization

The reference test suite should be organized as follows:

```
tests/
├── unit/               # Unit tests for library components
│   ├── instruction/    # Instruction encoding/decoding tests
│   ├── binary/         # Binary format tests
│   └── target/         # Target-specific tests
├── integration/        # Integration tests
│   ├── assembler/      # Assembler tests
│   ├── target_asm/     # Target assembler tests
│   └── linker/         # Linker tests
├── feature/            # Feature-specific tests
│   ├── control_flow/   # Control flow instruction tests
│   ├── memory/         # Memory operation tests
│   └── arithmetic/     # Arithmetic operation tests
└── examples/           # Example COIL programs
    ├── hello_world/    # Basic hello world example
    ├── factorial/      # Recursive function example
    └── vector_sum/     # Vector operation example
```

## Conclusion

This reference implementation guide provides a blueprint for developing the first generation of COIL toolchains. By following the architecture and guidelines outlined in this document, developers can create a compliant and efficient implementation of the COIL specification.

The phased development approach allows for incremental progress while ensuring that each component is well-tested and validated. Starting with a minimal implementation and expanding to cover the full specification enables early testing and feedback while managing development complexity.

With this reference implementation guide, the COIL specification is now ready for practical implementation, marking an important step toward the realization of COIL's vision for a unified, cross-architecture intermediate language.