# Computer Architecture - Laboratory Exercises

This repository contains laboratory exercises and coursework for a Computer Architecture course, focusing on low-level programming with x86-64 assembly language and performance optimization.

## 📚 Course Reference

Course materials can be found at: http://galaxy.agh.edu.pl/~amrozek/ak.html

## 🗂️ Repository Structure

The repository is organized into numbered directories, each corresponding to a specific laboratory exercise:

### Assembly Language Labs (01-07, 09-10)

- **01/** - Introduction to Assembly
  - Basic program structure
  - System calls (sys_write, sys_exit)
  - Hello World example
  
- **02/** - Control Flow
  - For loops in assembly
  - Different loop implementations
  
- **03/** - Number Conversions
  - Converting integers to hexadecimal strings
  - Working with different data sizes (8, 16, 32, 64 bits)
  
- **04/** - Advanced Conversions
  - Decimal to string conversion
  - Lookup tables (LUT)
  
- **05/** - System Calls and File Operations
  - File I/O operations
  
- **06-07/** - Combined exercises
  
- **09/** - Advanced assembly programming
  
- **10/** - Floating Point Operations
  - Computing Pi
  - FPU operations
  - Performance measurements (GFLOPS)
  - Includes `eval_time.c` for timing measurements

### Performance Optimization Labs (11-12)

- **11/** - Matrix Multiplication (C)
  - Naive implementation
  - Blocked algorithm
  - Performance analysis with Python
  - Includes: `mat_mat.c`, `eval_time.h`, `main.py`
  
- **12/** - Matrix Multiplication Optimization
  - Loop unrolling techniques
  - Performance comparison
  - Visualization with Python (`chart.py`, `chart_new.py`)

### Additional Directories

- **kolos/** - Exam/test exercises
- **kolos2/** - Second exam/test exercises
- **wlasne_cwiczenia/** - Personal exercises

## 🛠️ Prerequisites

To work with this repository, you'll need:

- **Assembler**: GNU Assembler (GAS/as) for x86-64
- **Linker**: GNU linker (ld)
- **C Compiler**: GCC (for labs 11-12)
- **Python**: Python 3.x (for performance analysis scripts)
- **Operating System**: Linux (x86-64 architecture)

## 🚀 Building and Running

### Assembly Programs

To compile and run assembly programs:

```bash
# Navigate to a lab directory
cd 01

# Assemble the source file
as -o lab1.o lab1.s

# Link the object file
ld -o lab1 lab1.o

# Run the executable
./lab1
```

### C Programs (Labs 10-12)

Labs 10-12 include C code for performance measurement and optimization:

```bash
# Navigate to the lab directory (example: lab 11)
cd 11

# First, compile the timing utility (if needed, from lab 10)
gcc -c -O2 ../10/eval_time.c -o eval_time.o

# Compile the main program with optimization
gcc -O2 -o mat_mat mat_mat.c eval_time.o -lm

# Run the program
./mat_mat
```

**Note**: The `eval_time.c` file is located in lab 10 and provides timing utilities used across multiple labs.

## 📊 Performance Analysis

Some labs include Python scripts for performance visualization:

```bash
# Run the Python analysis script
python3 main.py
```

This will generate performance data and visualization charts (e.g., `wykres.png`).

## 📝 Key Topics Covered

- **System Calls**: Direct interaction with the Linux kernel
- **Data Representation**: Working with different data sizes and types
- **Control Structures**: Loops and conditionals in assembly
- **Number Systems**: Binary, hexadecimal, decimal conversions
- **Floating Point Arithmetic**: FPU operations and precision
- **Memory Management**: Static allocation, stack operations
- **Performance Optimization**: Loop unrolling, blocking algorithms
- **Algorithm Analysis**: Measuring and comparing execution times

## 🔧 File Types

- `.s` - Assembly source files (AT&T syntax)
- `.o` - Object files (compiled but not linked)
- `.c` - C source files
- `.h` - C header files
- `.py` - Python scripts for analysis
- `.txt` - Data files and notes
- `.png` - Generated charts and graphs

## 📖 Assembly Syntax

This repository uses **AT&T syntax** for x86-64 assembly, which is the default for GNU Assembler. Key characteristics:

- Source before destination: `mov %rax, %rbx` (move rax to rbx)
- Register names prefixed with `%`: `%rax`, `%rdi`, `%esi`
- Immediate values prefixed with `$`: `$10`, `$sys_write`
- Operand size suffixes: `movb` (byte), `movw` (word), `movl` (long), `movq` (quad)

## 🎓 Learning Objectives

Through these exercises, students will:

1. Understand computer architecture at the instruction level
2. Learn how high-level constructs translate to machine code
3. Optimize code for performance
4. Measure and analyze computational efficiency
5. Work directly with system calls and hardware

## 📄 License

This is educational material for a Computer Architecture course.

## 🤝 Contributing

This repository contains coursework and lab exercises. Feel free to use it as a reference for learning computer architecture concepts.
