#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <commands_hashes.h>
#include <translator.h>
#include <stdio.h>

void InitExecMemory(machine_code_s* MACHINE_CODE)
{
    MACHINE_CODE->buff_size = 100;
    MACHINE_CODE->img = (uint8_t*) malloc (MACHINE_CODE->buff_size);
    MACHINE_CODE->act_size = 0;
}

uint8_t GetRegIdx(char reg)
{
    switch (reg)
    {
        case 'A':
            return 0;
        case 'H':
            return 5;
        case 'L':
            return 6;
        case 'M':
            return 7;
        default:
            return (uint8_t) (reg - 'A');
    }
}

uint32_t PolinHash(char* st)
{
    uint32_t p = 1, summ = 0, s;
    for (int i = 0; i < 3; i++)
    {
        s = (uint32_t) ((uint8_t) st[i]);
        summ += s * p;
        p *= 6;
    }
    return summ % 1000000;
}

int InstrEncoder(instr_s* dest, char* instr_buff, FILE* src)
{
    char* mnemonic;
    mnemonic = strtok(instr_buff, " ");
    switch (PolinHash(mnemonic))
    {
        case MOV_HASH:
            dest->group = 0b000;
            dest->opcode.labels.id = 0b11;
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case MVI_HASH:
            dest->group = 0b000;
            uint8_t imm;
            dest->opcode.labels.id = 0;
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b110;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case INC_HASH:
            mnemonic = strtok(NULL, " ");
            if (mnemonic[0] == 'A')
            {
                fclose(src);
                printf("ERROR! DEC and INC instructions do not work with accumulator (A)!\n");
                return 1;
            }
            dest->group = 0b000;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b000;
            dest->length = 0b01;
            return 0;
        case DEC_HASH:
            mnemonic = strtok(NULL, " ");
            if (mnemonic[0] == 'A')
            {
                fclose(src);
                printf("ERROR! DEC and INC instructions do not work with accumulator (A)!\n");
                return 1;
            }
            dest->group = 0b000;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b001;
            dest->length = 0b01;
            return 0;
        default:
            fclose(src);
            printf("ERROR! No such command in the Intel-8008 ISA! -> %s \n", mnemonic);
            return 1;
    }
}

void WriteInMemImg(machine_code_s* mem, instr_s* src)
{
    if (mem->act_size + src->length + 1 > mem->buff_size)
    {
        mem->buff_size *= 2;
        mem->img = (uint8_t*) realloc (mem->img, sizeof(uint8_t) * mem->buff_size);
    }
    switch (src->length)
    {
        case 0b01:
            mem->img[mem->act_size] = src->group;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->opcode.value;
            mem->act_size += 1;
            return;
        case 0b10:
            mem->img[mem->act_size] = src->group;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->opcode.value;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->second_byte;
            mem->act_size += 1;
            return;
    }
}

int main()
{

    FILE* program = fopen("../IO/asm_code.txt", "r");
    if (program == NULL)
    {
        printf("ERROR! File opening failed!");
        return 0;
    }

    machine_code_s MACHINE_CODE;
    InitExecMemory(&MACHINE_CODE); // Initializing memory for executable code

    char instr_buff[INSTR_BUFF_SIZE];
    char* check_pointer;
    int encoder_code, line = 0;
    instr_s input_instr;
    while (!feof(program))
    {
        check_pointer = fgets(instr_buff, INSTR_BUFF_SIZE, program);

        if (check_pointer == NULL)
        {
            printf("ERROR! Command reading failed!");
            return 0;
        }

        encoder_code = InstrEncoder(&input_instr, instr_buff, program);

        if (encoder_code != 0)
        {
            DEST_EXEC_MEMORY;
            printf("Location of the error: %d", line);
            return 0;
        }
        line++;
        WriteInMemImg(&MACHINE_CODE, &input_instr);
    }

    FILE* bin_code = fopen("../IO/code.bin", "wb");
    fwrite(&MACHINE_CODE.act_size, sizeof(int), 1, bin_code);
    fwrite(MACHINE_CODE.img, sizeof(uint8_t), MACHINE_CODE.act_size, bin_code);
    fclose(bin_code);
    fclose(program);
    DEST_EXEC_MEMORY;
    printf("COMPILING REPORT\nNumber of bytes: %d\nProgram size (bytes): %d\nAdditional bytes for emulator working: %d\n", MACHINE_CODE.act_size + sizeof(int), MACHINE_CODE.act_size - line, line + sizeof(int));
    return 0;
}
