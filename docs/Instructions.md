| Instruction | Hex        | Binary                | Examples                |
| ----------- | ---------- | --------------------- | ----------------------- |
|             |            |                       |                         |
| LOAD        | 0x08       | 0000 0001             | LOAD s0, 0x1000         |
| STORE       | 0x09       | 0000 0010             | STORE S2, 0x1011        |
| LOADI       | 0x0A       | 0000 1010             | LOADI s0, s2            |
| ADD         | 0x11       | 0001 0001             | ADD S0, S2              |
| ADDI        | 0x10       | 0001 0000             | ADD S0, 5               |
| SUB         | 0x12       | 0001 0010             | SUB S0, S2              |
| MUL         | 0x13       | 0001 0011             | MUL S0, S2              |
| DIV         | 0x14       | 0001 0100             | DIV S0, S2              |
| INC         | 0x15       | 0001 0101             | INC S0                  |
| DEC         | 0x16       | 0001 0110             | DEC S2                  |
| CMP         | 0x20       | 0010 0000             | CMP S0, S1              |
| JMP         | 0x30       | 0011 0000             | JMP 0x1111              |
| JE          | 0x31       | 0011 0001             | JE 0x1111               |
| JNE         | 0x32       | 0011 0010             | JNE 0x1111              |
| JG          | 0x33       | 0011 0011             | JG                      |
| JL          | 0x34       | 0011 0100             | JL                      |
| PUSH        | 0x40       | 0100 0000             | PUSH S0                 |
| POP         | 0x41       | 0100 0001             | POP S1                  |
| CALL        | 0x42       | 0100 0010             | CALL 0x0100             |
| RET         | 0x43       | 0100 0011             | RET                     |
| NOP         | 0xFE       | 1111 1110             | NOP                     |
| HLT         | 0xFF       | 1111 1111             | HLT                     |
