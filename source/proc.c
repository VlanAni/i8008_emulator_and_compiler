#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <proc_abstr.h>
#include <index_reg_instr.h>

typedef void (*DecInstr) (i8008_MODEL_s*, uint8_t*, opcode_u*);

DecInstr Fetch(DecInstr* dec_func_arr, i8008_MODEL_s* i8008, uint8_t* RAM, opcode_u* opc)
{
    uint8_t instruction_group = RAM[i8008->ADDR_STACK.PC];
    i8008->ADDR_STACK.PC += 1;
    opc->value = RAM[i8008->ADDR_STACK.PC];
    i8008->ADDR_STACK.PC += 1;
    return dec_func_arr[instruction_group];
}

void DecIndRegInstr (i8008_MODEL_s* i8008, uint8_t* RAM, opcode_u* opc)
{
    uint8_t idx = opc->labels.id;
    uint8_t src = opc->labels.reg2;
    uint8_t dst = opc->labels.reg1;
    if (idx)
    {
        if (src == 0b111)
            MOV_Rd_M(i8008, dst, RAM);
        else if (dst == 0b111)
            MOV_M_Rs(i8008, src, RAM);
        else
            MOV_Rd_Rs(i8008, dst, src);
    }
    else
    {
        switch (src)
        {
            case 0b110:
                uint8_t imm = RAM[i8008->ADDR_STACK.PC];
                i8008->ADDR_STACK.PC += 1;
                if (dst == 0b111)
                    MVI_M_Imm(i8008, imm, RAM);
                else
                    MVI_Rd_Imm(i8008, dst, imm);
                break;
            case 0b000:
                INC_Rd(i8008, dst);
                break;
            case 0b001:
                DEC_Rd(i8008, dst);
                break;
        }
    }
}

int main()
{
    //Initializing...
    i8008_MODEL_s I8008M;
    I8008M.ADDR_STACK.PC = 0;
    uint8_t* RAM = (uint8_t*) malloc (sizeof(uint8_t) * ADDR_AREA_SIZE); // external memory for processor
    DecInstr dec_func_array[4] = {DecIndRegInstr};
    DecInstr DecFunc;
    opcode_u opc;
    printf("The RAM has been enabled successfully\n");
    FILE* program = fopen("../IO/code.bin", "rb");
    if (program == NULL)
    {
        printf("ERROR! Cannot open the file with binary code");
        return 0;
    }

    int prog_size;
    fread(&prog_size, sizeof(int), 1, program);
    fread(RAM, sizeof(uint8_t), prog_size, program);
    printf("PROG SIZE: %d\nThe emulator has started performing...\n", prog_size);
    for (int inst_num = 1; I8008M.ADDR_STACK.PC < prog_size; inst_num++)
    {
        DecFunc = Fetch(dec_func_array, &I8008M, RAM, &opc);
        DecFunc(&I8008M, RAM, &opc);
    }

    printf("The program was performing successfully\n");
    printf("##REGISTERS VALUES##\n");
    printf("A - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.A);
    printf("B - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.B);
    printf("C - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.C);
    printf("D - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.D);
    printf("E - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.E);
    printf("H - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.H);
    printf("L - %" PRIu8 "\n", I8008M.REG_FILE.named_regs.L);

    fclose(program);
    free(RAM);
    printf("The RAM has been disabled successfully");
    return 0;
}

