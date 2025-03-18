# Virtual Registers

In the future the virtual register system will also support other processing units. (this note will be noted to be a theme throughout this early version)

## CPU
A note for the future, as ARM is built out the architectures should all have the same amount of registers. in this case where the register can not be supported it will either be put directly that it is stack space or for example x86_32 systems which don't support r8-r15 will automatically utilize stack space for these reigsters. Directly calling registers should not be seen as a way for optimization but only for needed compatibility utilizing this as a last resort when ABI can not possibly be used.

The virtual register system is always the last to consult.

#### x86
There is no direct mapping for float and vector registers, this may be worked on in the future but float registers seem to operate in vector registers which comes as a slight problem as vector operations may or may not be implemented on the system and if they are they could be xmm0, ymm0, zmm0, etc... They should not be needed as any functions, calls, interrupts that you need to cantact through direct register manipulation will probably only use general purpose registers.

###### 64 bit mapping
General Purpose Quad Register

RQ0  = RAX
RQ1  = RBX
RQ2  = RCX
RQ3  = RDX
RQ4  = RDI
RQ5  = RSI
RQ6  = RSP
RQ7  = RBP
RQ8  = r8
RQ9  = r9
RQ10 = r10
RQ11 = r11
RQ12 = r12
RQ13 = r13
RQ14 = r14
RQ15 = r15

###### 32 bit mapping
General Purpose Long Register

RL0 = EAX
RL1 = EBX
RL2 = ECX
RL3 = EDX
RL4 = EDI
RL5 = ESI
RL6 = ESP
RL7 = ESI
RL8  = r8d
RL9  = r9d
RL10 = r10d
RL11 = r11d
RL12 = r12d
RL13 = r13d
RL14 = r14d
RL15 = r15d

###### 16 bit mapping
RW0 = AX
RW1 = BX
RW2 = CX
RW3 = DX
RW4 = DI
RW5 = SI
RW6 = SP
RW7 = SI
RW8  = r8w
RW9  = r9w
RW10 = r10w
RW11 = r11w
RW12 = r12w
RW13 = r13w
RW14 = r14w
RW15 = r15w

###### 8 bit mapping
RB0 = AXL
RB1 = BXL
RB2 = CXL
RB3 = DXL
RB4 = DIL
RB5 = SIL
RB6 = SPL
RB7 = SIL
RB8  = r8b
RB9  = r9b
RB10 = r10b
RB11 = r11b
RB12 = r12b
RB13 = r13b
RB14 = r14b
RB15 = r15b

###### Segments
S0 = SS
S1 = CS
S2 = DS
S3 = ES
S4 = FS
S5 = GS

#### ARM
...