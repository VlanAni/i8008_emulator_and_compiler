#ifndef PROC_ABSTR_H_INCLUDED
#define PROC_ABSTR_H_INCLUDED

#define ADDR_AREA_SIZE 16384 // 2^14 bytes of memory!!!

struct named_regs_s
{
    uint8_t A; // Accumulator
    uint8_t B; //|
    uint8_t C; //|
    uint8_t D; //| General-purposes registers
    uint8_t E; //|
    uint8_t H; // high 6 bits of address
    uint8_t L; // low 8 bits of address
};

typedef struct named_regs_s named_regs_s;

typedef union Reg_file_s
{
    uint8_t REGS[7]; // array of registers
    named_regs_s named_regs;
} Reg_file_s;

typedef struct Addr_stack_s
{
    uint16_t PC; // Program counter
    uint16_t stack[7]; // stack for return-addresses
    uint8_t SP; // 3-bit stack-pointer
} Addr_stack_s;

typedef struct Flags_s
{
    uint8_t PF : 1;
    uint8_t ZF : 1;
    uint8_t SF : 1;
    uint8_t CF : 1;
} Flags_s;

typedef struct opcode_s
{
    uint8_t reg2 : 3;
    uint8_t reg1 : 3;
    uint8_t id : 2;
} opcode_s;

typedef union opcode_u
{
    opcode_s labels;
    uint8_t value;
} opcode_u;

typedef struct i8008_MODEL_s // Intel-8008 processor
{
    Flags_s FLAGS; // 0 - parity flip-flop, 1 - zero flip-flop, 2 - sign flip-flop, 3 - carry flip-flop
    Reg_file_s REG_FILE;
    Addr_stack_s ADDR_STACK;
} i8008_MODEL_s;

extern i8008_MODEL_s I8008M; // Intel-8008 model

#endif // PROC_ABSTR_H_INCLUDED
