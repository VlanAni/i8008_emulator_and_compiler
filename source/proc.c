#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <proc_abstr.h>
#include <index_reg_instr.h>
#include <acc_group_instr.h>

typedef void (*DecInstr) (i8008_MODEL_s*, uint8_t*, opcode_u*);

typedef void (*RotInstr) (i8008_MODEL_s*);

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

void DecAccGroupInstr (i8008_MODEL_s* i8008, uint8_t* RAM, opcode_u* opc)
{
    uint8_t idx = opc->labels.id;
    uint8_t src = opc->labels.reg2;
    uint8_t dst = opc->labels.reg1;
    if (!idx)
    {
        if (src == 0b100)
        {
            uint8_t imm = RAM[i8008->ADDR_STACK.PC];
            i8008->ADDR_STACK.PC += 1;
            switch (dst)
            {
            case 0b000:
                ADI(i8008, imm);
                break;
            case 0b010:
                SUI(i8008, imm);
                break;
            case 0b100:
                ANI(i8008, imm);
                break;
            case 0b101:
                XRI(i8008, imm);
                break;
            case 0b110:
                ORI(i8008, imm);
                break;
            case 0b111:
                CPI(i8008, imm);
                break;
            }
        }
        else
        {
            RotInstr rot_instrs[4] = {RLC, RRC, RAL, RAR};
            RotInstr currInstr = rot_instrs[dst];
            currInstr (i8008);
        }
    }
    else
    {
        switch (dst)
        {
            case 0b000:
                if (src == 0b111)
                    ADD_M(i8008, RAM);
                else
                    ADD_Rs(i8008, src);
                break;
            case 0b001:
                if (src == 0b111)
                    ADC_M(i8008, RAM);
                else
                    ADC_Rs(i8008, src);
                break;
            case 0b010:
                if (src == 0b111)
                    SUB_M(i8008, RAM);
                else
                    SUB_Rs(i8008, src);
                break;
            case 0b011:
                if (src == 0b111)
                    SBB_M(i8008, RAM);
                else
                    SBB_Rs(i8008, src);
                break;
            case 0b100:
                if (src == 0b111)
                    ANA_M(i8008, RAM);
                else
                    ANA_Rs(i8008, src);
                break;
            case 0b101:
                if (src == 0b111)
                    XRA_M(i8008, RAM);
                else
                    XRA_Rs(i8008, src);
                break;
            case 0b110:
                if (src == 0b111)
                    ORA_M(i8008, RAM);
                else
                    ORA_Rs(i8008, src);
                break;
            case 0b111:
                if (src == 0b111)
                    CMP_M(i8008, RAM);
                else
                    CMP_Rs(i8008, src);
                break;
        }
    }
}

void Reset(i8008_MODEL_s* i8008)
{
    i8008->ADDR_STACK.PC = 0;
    i8008->ADDR_STACK.SP = STACK_TOP;
    i8008->FLAGS.CF = 0;
    i8008->FLAGS.PF = 0;
    i8008->FLAGS.SF = 0;
    i8008->FLAGS.ZF = 0;
    for (int i = 0;  i < 7; i++)
    {
        i8008->REG_FILE.REGS[i] = 0;
        i8008->ADDR_STACK.stack[i] = 0;
    }
}

int main()
{
    //Initializing...
    i8008_MODEL_s I8008M;
    Reset(&I8008M);
    uint8_t* RAM = (uint8_t*) malloc (sizeof(uint8_t) * ADDR_AREA_SIZE); // external memory for processor
    DecInstr dec_func_array[4] = {DecIndRegInstr, DecAccGroupInstr}; // decoding instructions
    DecInstr DecFunc;
    opcode_u opc;
    printf("\nThe RAM has been enabled successfully\n");
    FILE* program = fopen("../IO/code.bin", "rb");
    if (program == NULL)
    {
        printf("ERROR! Cannot open the file with binary code\n");
        return 0;
    }

    int prog_size;
    fread(&prog_size, sizeof(int), 1, program);
    fread(RAM, sizeof(uint8_t), prog_size, program);
    printf("The emulator has started performing...\n");
    for (int inst_num = 1; I8008M.ADDR_STACK.PC < prog_size; inst_num++)
    {
        DecFunc = Fetch(dec_func_array, &I8008M, RAM, &opc);
        DecFunc(&I8008M, RAM, &opc);
    }

    fclose(program);
    printf("The program has been performed successfully\n");
    printf("##REGISTERS VALUES##\n");
    printf("A unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.A, (int8_t) I8008M.REG_FILE.named_regs.A);
    printf("B unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.B, (int8_t) I8008M.REG_FILE.named_regs.B);
    printf("C unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.C, (int8_t) I8008M.REG_FILE.named_regs.C);
    printf("D unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.D, (int8_t) I8008M.REG_FILE.named_regs.D);
    printf("E unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.E, (int8_t) I8008M.REG_FILE.named_regs.E);
    printf("H unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.H, (int8_t) I8008M.REG_FILE.named_regs.H);
    printf("L unsig = %" PRIu8 "\tsig = %" PRIi8 "\n", I8008M.REG_FILE.named_regs.L, (int8_t) I8008M.REG_FILE.named_regs.L);
    printf("##FLAGS##\n");
    printf("ZF - %" PRIu8 "\n", I8008M.FLAGS.ZF);
    printf("CF - %" PRIu8 "\n", I8008M.FLAGS.CF);
    printf("PF - %" PRIu8 "\n", I8008M.FLAGS.PF);
    printf("SF - %" PRIu8 "\n", I8008M.FLAGS.SF);
    printf("########\n");
    free(RAM);
    printf("The RAM has been disabled successfully\n");
    return 0;
}

