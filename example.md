# COIL Examples


## Basic
```

; As specified in reg.md
; registers are given pseudo names and used in very specific places where they are needed.
; these are always the same (literally they are actually defined as a hexadecimal value).
; (this being the textual represenation would not use these but more is explained in reg.md)
;
; rax = R0, rbx = R1, rcx = R2, rdx = R3, rdi = R4, rsi = R5
;


DIR SECT text READ EXEC

DIR ABI abi-linux-x86_64
{
  args = [ R0, R4, R5, R3]
  rets = [ R0 ]
  ; ...
}

DIR ABI abi-c
{
  ; complete abi creation will be defined later on
}

DIR HINT _start FUNC GLOBAL
DIR LABEL _start
  ; enter stack frame
  FRAME ENTER

  ; print hello world string
  CF SYSC abi-linux-x86_64 (0x01, 0, _hw_str, _hw_str_end - _hw_str) -> ( $0 )

  ; has to be deleted in reverse order from where it was initalized
  VAR DLT $0

  CF CALL void vec_example

  ; leave stack frame
  FRAME LEAVE

  ; linux syscall
  ; exit with 0
  CF SYSC abi-linux-x86_64 (0x3c, 0) > ()
DIR HINT _start ENDFUNC

DIR HINT vec_example FUNC
DIR LABEL vec_example
  ; enter stack scope
  FRAME ENTER

  ; push state
  MEM PUSH STATE $0
  
  ; Alloate a space of 256 bits for vector operations
  VAR DECL $1 : vec256(uint64)

  ; the vector + immediate64 is not directly native in most cases
  ; so this instruction could convert to multiple native code
  MATH ADD $1, 10

  ; most would be something along like this
  ; MATH ADD VEC GET($1, 0), 10
  ; MATH ADD VEC GET($1, 1), 10
  ; MATH ADD VEC GET($1, 2), 10
  ; MATH ADD VEC GET($1, 3), 10

  ; Variables have to be deleted in reverse order
  VAR DLT $1
  MEM POP $0 ; pop the state

  ; leave stack scope
  FRAME LEAVE

  ; Return to address by the call function
  CF RET
DIR HINT vec_example ENDFUNC


DIR SECT data READ
DIR LABEL _hw_str
  DIR INST "Hello, World!"
DIR LABEL _hw_str_end
  DIR PADD 512
```

