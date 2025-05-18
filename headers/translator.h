#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#define INSTR_BUFF_SIZE 50

#define DEST_EXEC_MEMORY free(MACHINE_CODE.img)


typedef struct machine_code_s
{
    uint8_t* img;
    int buff_size;
    int act_size;
} machine_code_s;

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

typedef struct instr_s
{
    opcode_u opcode;
    uint8_t second_byte;
    uint8_t third_byte;
    uint8_t length;
    uint8_t group;
} instr_s;

#endif // TRANSLATOR_H_INCLUDED
