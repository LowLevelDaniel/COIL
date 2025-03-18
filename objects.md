# COIL Object Formats

## Introduction

COIL employs two distinct object formats to balance the needs of optimization, cross-platform compatibility, and integration with existing tools:

1. **COIL Object Format (COF)**: A rich intermediate format that preserves COIL's abstractions, enabling link-time optimization and cross-platform capabilities.

2. **Native Object Format (NOF)**: A specialized object format created specifically for COIL that contains machine code targeting multiple processing units and architectures. This format is designed to work with COIL-compliant linkers and provides superior support for heterogeneous computing environments.

This document specifies the structure and usage of these formats, as well as the processes for converting between them.

## Design Philosophy

The COIL object formats are designed according to several key principles:

1. **Preserving Information**: The COIL Object Format preserves all semantic information necessary for optimization and cross-platform adaptation.

2. **Performance**: Both formats are designed for efficient processing, with minimal overhead for the functionality they provide.

3. **Extensibility**: The formats can be extended to support new features and target architectures without breaking compatibility.
