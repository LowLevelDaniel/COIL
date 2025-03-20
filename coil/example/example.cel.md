# COIL Explanatory Language Examples
Just examples of how a COIL program should be laid out using the [COIL Explanatory language](./../../CEL-GRAMMAR.md)


## Simple Beginnings

```
_start:
  VARSC
    ; int is the integer at the bitwidth of general purpose register width
    VARCR $0, int, 10
    VARCR $1, int, 0

    MOV RQ0, 42

    ADD $1, $0, RQ0 ; three arguments equals to . = . + . 
    SUB $1, RQ0 ; two arguments equals to . -= .

    ; float is the default price
    VARCR $3, float, 10.2

    ; type cast integer type to floating type
    ITOF RS1, $1, float

    ADD RS0, $3, $1
  VAREND

  VARSC
    ; special variables exist like state types
    ; in this case it is a state containing a sort of reference
    ; to registers on the stack, when the variable is freed it 
    ; will pop the registers or you can manually call pop with the variable
    ; identifier
    PUSHA $0
    POP $0

    ; zero can be resued as the last reference was deleted in the same scope
    PUSHFD $0
    VARDL $0
  VAREND 

  VARSC
    ; variables can be deleted before end of the scope
    ; but they have to be deleted in reverse order of creation
    ; this is in case the variable is stored on the stack meaning
    ; variables after it will be deleted to free the other variable
    VARCR $0, int, 0
    VARCR $1, int, 0
    VARCR $2, int, 0

    ; this works
    VARDL $2
    VARDL $1

    ; this doesn't
    VARDL $1
  VAREND

  ; also called hlt on x86
  ; we can't return without operating system support
  ; COIL has no official support for exiting without the ESTD
  WFI
```


## C Compatibility

```
.abi "c-abi" .start
  ; arguments


  ; return argumnets
  .ret 0, RQ0

  ; stack


  ; redscope


.end

```

