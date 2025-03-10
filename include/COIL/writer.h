/**
* @file writer.h
* @brief Definition of the COIL writer
*
* This file contains definitions specific to the COIL (Computer Oriented Intermediate 
* Language) Format
*/

#ifndef COIL_WRITER_H
#define COIL_WRITER_H

#include <COIL/err.h>
#include <COIL/format.h>
#include <stdio.h>

/**
* @brief a coil device is a modular map of functions that create a Optimize COIL Object
*
* Target specific implemenations exist for each device.
*
*/
typedef struct coil_device {
  // Memory Operations
  coil_err_t (*mem_def_var) (FILE *sout, uint64_t id, coil_type_t type);
  coil_err_t (*mem_def_symb)(FILE *sout, const char *symbol);
  coil_err_t (*mem_prmt_var)(FILE *sout, uint64_t id, coil_type_t type);
  coil_err_t (*mem_demt_var)(FILE *sout, uint64_t id, coil_type_t type);
  coil_err_t (*mem_redef)   (FILE *sout, uint64_t id, coil_type_t type);
  coil_err_t (*mem_dlt)     (FILE *sout, uint64_t id, coil_type_t type);
  coil_err_t (*mem_move)    (FILE *sout, uint64_t destid, uint64_t srcid, coil_type_t desttype, coil_type_t srctype);
  coil_err_t (*mem_load)    (FILE *sout, uint64_t destid, uint64_t srcid, coil_type_t desttype, coil_type_t srctype);
  coil_err_t (*mem_store)   (FILE *sout, uint64_t destid, uint64_t srcid, coil_type_t desttype, coil_type_t srctype);
  coil_err_t (*mem_pusha)   (FILE *sout, uint64_t destid);
  coil_err_t (*mem_pushfd)  (FILE *sout, uint64_t destid);
  coil_err_t (*mem_pushs)   (FILE *sout, uint64_t destid);
  coil_err_t (*mem_popa)    (FILE *sout, uint64_t destid);
  coil_err_t (*mem_popfd)   (FILE *sout, uint64_t destid);
  coil_err_t (*mem_pops)    (FILE *sout, uint64_t destid);

  // Arithmetic Operations
  // TODO

  // Bitwise Operations
  // TODO

  // Control Flow Operations
  // TODO
} coil_device_t;

typedef struct coil_writer {
  const char **symbol_map;
  
  coil_device_t *devices; // static array of all selected target devices
  size_t devices_length;

  FILE *output;

  uint8_t host_device; // the main device to compile for
} coil_wrtier_t;

#endif /* COIL_WRITER_H */