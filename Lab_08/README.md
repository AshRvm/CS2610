# Pipelined-Processor Simulator

Simulates a 5-stage Reduced Instruction Set Computer(RISC) Pipelined Processor, with a given Cache system and Register File, consisting of 16 8-bit registers.

Cache system specifications: 
- 256B Data Cache
- 256B Instruction Cache
- Single level
- Block size of 4B
- Single read and write port for each cache

The 5 stages of the pipeline:
- Instruction Fetch (IF)
- Instruction Decode (ID)
- Execution/Effective Address (EX)
- Memory Access (MEM)
- Write Back (WB)

Performance details:
- Number of stalls due to Read AFter Write (RAW) hazards
- Number of stalls due to Control hazards
- The Clock Cycles per Instruction (CPI) of the program

The various instructions supported by the processor are as follows

Opcode | ADD | SUB | MUL | INC | AND | OR | NOT | XOR | LOAD | STORE | JMP | BEQZ | HLT
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---
4-bit representation | 0000 | 0001 | 0010 | 0011 | 0100 | 0101 | 0110 | 0111 | 1000 | 1001 | 1010 | 1011 | 1111
