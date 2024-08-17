# Readme

------

This project is for Project 7 of Nand2tetris, which translates VM instructions in the HackCPU chip into assembly code for execution.

------

Development Environment:

- Operating System: Linux
- Programming Language: C++
- Compiler: GCC
  - Compiler Version: gcc version 13.2.0

------

Code Directory:

- main.cpp: Core file for compilation
- config.hpp: For configuring the debug program
- compiler_control.hpp: For managing program errors
- parser.hpp: For parsing statements
- type.hpp: Defines common types
- symbol_table.hpp: Defines types appearing in the program
- CodeWrite.hpp: Generates assembly code based on statements parsed by parser.hpp

------

Usage Instructions:

- After compiling main.cpp
- Enter in the terminal: `./compiled_program_name input_vm_command_file output_assembly_program_directory`

------

VM Instruction Format:

> PUSH Instruction
>
> ```
> PUSH segment value
> ```
>
> Pushes the value from the segment onto the stack

> POP Instruction
>
> ```
> POP segment value
> ```
>
> Pops the top value from the stack into the segment

For example,

`push local 1` is equivalent to `push local[1]`, which pushes the value from the segment onto the stack.

- Memory Segments

| Segment_Name | Description                                                  |
| ------------ | ------------------------------------------------------------ |
| argument     | Stores function arguments                                    |
| local        | Stores function local variables                              |
| static       | Stores static variables                                      |
| constant     | Indicates that the value is a constant to be pushed directly onto the stack |
| this\that    | To be used for future extension                              |
| pointer      | To be used for future extension                              |
| temp         | Fixed field with eight memory units for temporary values     |

------

Memory Access Commands:

| Command | Description                                                  |
| ------- | ------------------------------------------------------------ |
| add     | Pops the top two values from the stack and adds them         |
| sub     | Pops the top two values from the stack and subtracts them    |
| eq      | Pops the top two values from the stack and checks if they are equal |
| gt      | Pops the top two values from the stack and checks if the first is greater than the second |
| lt      | Pops the top two values from the stack and checks if the first is less than the second |
| neg     | Pops the top value from the stack and negates it             |
| not     | Pops the top value from the stack and negates it (logical negation) |
| and     | Pops the top two values from the stack and performs a logical AND |
| or      | Pops the top two values from the stack and performs a logical OR |

------

VM Code Example:

```
push constant 1
push constant 2
add
push constant 1
push constant 2 
gt
```

---

```
Contact me: cyberspace2077@163.com
```