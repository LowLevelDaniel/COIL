# Directives
A COIL directive is a way to talk directly to the COIL assembler. Built to be able to be extended by COIL assemblers if needed this covers section to the final binary program output, inserting bytes, linker sections and much more. A simple macro system is even implemented for better program compactness. These directives are defined in detail below and are similar to c functions having never more then one return and never more then 5 arguments.

They are simple compact and fast ways to get information from the assembler and control compilation. Version Compilation Control, Architecture Compilation Control, Proccessing Unit Compilation Control and more.

