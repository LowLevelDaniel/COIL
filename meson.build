project('coil', 'c',
  version : '0.1.0',
  default_options : [
    'warning_level=3',
    'c_std=c11',
    'optimization=2'
  ]
)

# Global configuration
cc = meson.get_compiler('c')
add_global_arguments('-DCOIL_VERSION="@0@"'.format(meson.project_version()), language : 'c')

# Check for required headers and functions
conf_data = configuration_data()
conf_data.set('HAVE_STDINT_H', cc.has_header('stdint.h'))
conf_data.set('HAVE_STDBOOL_H', cc.has_header('stdbool.h'))
conf_data.set('HAVE_STDDEF_H', cc.has_header('stddef.h'))
conf_data.set('HAVE_MEMMOVE', cc.has_function('memmove'))
configure_file(output : 'config.h', configuration : conf_data)

# Include directories
inc_dirs = include_directories('include', 'src')

# Library dependencies
# (currently no external dependencies required for core)

# Common library
common_sources = [
  'src/common/type_system.c',
  'src/common/instruction.c',
  'src/common/module.c',
  'src/common/error.c',
]

common_lib = static_library('coil_common',
  common_sources,
  include_directories : inc_dirs
)

# HOIL compiler library
hoil_compiler_sources = [
  'src/hoil_compiler/lexer.c',
  'src/hoil_compiler/parser.c',
  'src/hoil_compiler/semantic.c',
  'src/hoil_compiler/hoil_compiler.c',
]

hoil_compiler_lib = static_library('hoil_compiler',
  hoil_compiler_sources,
  include_directories : inc_dirs,
  link_with : common_lib
)

# COIL assembler library
coil_assembler_sources = [
  'src/coil_assembler/binary_parser.c',
  'src/coil_assembler/instruction_decoder.c',
  'src/coil_assembler/target.c',
  'src/coil_assembler/translator.c',
  'src/coil_assembler/optimizer.c',
  'src/coil_assembler/coil_assembler.c',
]

coil_assembler_lib = static_library('coil_assembler',
  coil_assembler_sources,
  include_directories : inc_dirs,
  link_with : common_lib
)

# Tools
executable('hoilc',
  'src/tools/hoilc.c',
  include_directories : inc_dirs,
  link_with : [common_lib, hoil_compiler_lib]
)

executable('coilasm',
  'src/tools/coilasm.c',
  include_directories : inc_dirs,
  link_with : [common_lib, coil_assembler_lib]
)

# Test suite will be added later