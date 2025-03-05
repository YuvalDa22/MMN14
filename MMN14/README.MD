
# 🚀 Assembler Project - 2024B 🚀
## Project Grade - 💯

This is the final project for the Laboratory C 20465- 2024B course at the Open University. This project was developed as part of the "20465 - Systems Programming Laboratory" 2024B course at the Open University. It's a comprehensive assembler program for a specific assembly language, written in C. The purpose is to simulate the operation of common system programs and gain experience in writing a large-scale software project.

___

## 📖 Table of Contents
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Usage](#-usage)
- [Testing](#-testing)
- [Documentation](#-documentation)

___

## 🧩 Project Structure

The project consists of multiple C and header files, each serving a specific purpose in the program. The assembler works in three main stages:

1. **Macro Expansion**
2. **Symbol Table Creation**
3. **Machine Code Generation**

These stages form the core algorithm of the assembler.

### Macro Expansion

In this stage, the assembler reads the source file line by line, identifying and expanding any macro definitions. The macro definitions are stored, and each instance of the macro is replaced with the corresponding instructions in the output.

### Symbol Table Creation

The assembler performs a first pass through the code, where it builds a symbol table. This table maps labels (symbols) to their respective memory addresses, which are needed for correct instruction referencing.

### Machine Code Generation

In the second pass, the assembler uses the symbol table and converts the assembly instructions into machine code. It writes the generated machine code to an object file, resolving symbols to their memory addresses.

### 📂 Project Structure

```bash
include/
├── assembler.h
├── defines.h
├── macros.h
├── symbols.h
└── utils.h
src/
├── assembler.c
├── macros.c
├── main.c
├── symbols.c
└── utils.c
test_cases/
├── ps.as
├── sample1.as
├── sample2.as
└── error_sample.as
```

___ 

## ⚙️ Getting Started

### ✔️ Prerequisites

Before you begin, ensure that you have the following prerequisites installed:
> - `GCC compiler`
> - `Ubuntu system 16.04`

### 📦 Installation

1. Navigate to the project directory:
```sh
cd <project-directory>
```

2. Build the project using the Makefile:
```sh
make
```

3. Clean the project build (optional):
```sh
make clean
```

___

## 🔧 Usage

To run the assembler, use the following command:

```sh
./assembler ps
```

> Replace `ps` with the filename (without the extension) of the assembly source file you want to process.

### Output:
- If the program is valid, the following files will be generated: `.ent`, `.ext`, `.ob`, and `.am`.
- If there are errors, the assembler will display error messages and will not generate `.ent`, `.ext`, or `.ob` files.

___

## 🧪 Testing

Several assembly language input files are provided to demonstrate the use of various operations, data types, and the assembler's handling of errors.

### Test Files:
- `ps.as`: Sample program provided in the course.
- `sample1.as`, `sample2.as`: Custom programs created to test the assembler.
- `error_sample.as`: Program with errors to demonstrate the assembler's error handling capabilities.

___ 

## 📚 Documentation

Each function in the source code is accompanied by concise, clear documentation in the form of header comments, explaining its purpose and usage. Detailed comments are present throughout the code for better understanding.

___

## 👏🏻 Created by Tal Liberman and Yuval Dadon
