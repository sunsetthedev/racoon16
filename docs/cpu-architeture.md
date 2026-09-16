
# SwanCPU Architecture

> Version: 0.1
> Author: Sunset
> Status: In Development

---

# Overview

SwanCPU is a 16-bit educational CPU architecture designed for learning:

- CPU Design
- Assembly
- Emulators
- Compilers
- Operating Systems

Goals:

- Easy to understand
- Easy to implement
- Fun to extend

Non-goals:

- Performance
- Compatibility
- Real hardware support

---

# CPU Specifications

| Property | Value |
|-----------|--------|
| Registers | 8 |
| Register Width | 16-bit |
| Address Width | 16-bit |
| RAM | 64 KB |
| Endianness | Little Endian |
| Instruction Size | 4 Bytes |
| Stack Growth | Downwards |

---

# Register Layout

| Register | Purpose |
|-----------|----------|
| R0 | General Purpose |
| R1 | General Purpose |
| R2 | General Purpose |
| R3 | General Purpose |
| R4 | General Purpose |
| R5 | General Purpose |
| R6 | General Purpose |
| R7 | General Purpose |

Special Registers:

| Register | Description |
|-----------|-------------|
| PC | Program Counter |
| SP | Stack Pointer |

---

# Flags

| Flag | Name | Description |
|--------|--------|-------------|
| Z | Zero | Result == 0 |
| C | Carry | Overflow occurred |
| N | Negative | Result < 0 |

---

# Memory Map

| Start | End | Description |
|---------|---------|-------------|
| 0x0000 | 0x7FFF | Program Memory |
| 0x8000 | 0xDFFF | Data Memory |
| 0xE000 | 0xEFFF | Stack |
| 0xF000 | 0xFFFF | Reserved / I/O |

---

# Instruction Format

All instructions are 4 bytes.

| Byte | Purpose |
|--------|---------|
| 0 | Opcode |
| 1 | Register |
| 2 | Operand High |
| 3 | Operand Low |

Example:

LOADI R0, 5

Machine Code:

0A 00 00 05

---

# Instruction Set

## LOADI

Load immediate value into register.

Assembly:

LOADI R0, 5

Format:

| Byte | Value |
|--------|--------|
| 0 | Opcode |
| 1 | Register |
| 2 | Value High |
| 3 | Value Low |

Pseudo:

R0 = 5

Opcode:

0x01

---

## ADD

Add register B to register A.

Assembly:

ADD R0, R1

Pseudo:

R0 = R0 + R1

Opcode:

0x10

---

## SUB

Assembly:

SUB R0, R1

Pseudo:

R0 = R0 - R1

Opcode:

0x11

---

## LOAD

Assembly:

LOAD R0, 0x1000

Pseudo:

R0 = MEM[0x1000]

Opcode:

0x20

---

## STORE

Assembly:

STORE R0, 0x1000

Pseudo:

MEM[0x1000] = R0

Opcode:

0x21

---

# Opcode Table

| Opcode | Instruction |
|-----------|-------------|
| 0x01 | LOADI |
| 0x10 | ADD |
| 0x11 | SUB |
| 0x20 | LOAD |
| 0x21 | STORE |
| 0xFF | HLT |

---

# Stack

Stack grows downward.

Example:

SP = 0xEFFF

PUSH

SP--

POP

SP++

---

# Calling Convention

Reserved for future implementation.

---

# Interrupts

Reserved for future implementation.

---

# Future Features

- Filesystem
- Text Display
- System Calls
- Compiler
- SwanOS

---

# Changelog

## v0.1

- Initial architecture
- 8 registers
- 16-bit CPU
- 64 KB RAM
