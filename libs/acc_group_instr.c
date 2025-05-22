#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <proc_abstr.h>
#include <acc_group_instr.h>

#define SET_FILP_FLOPS \
i8008->FLAGS.CF = temp_reg.bytes[1] & 0b00000001;\
i8008->FLAGS.ZF = (temp_reg.bytes[0] == 0);\
i8008->FLAGS.PF = temp_reg.bytes[0] & 0b00000001;\
i8008->FLAGS.SF = (temp_reg.bytes[0] & 0b10000000) >> 7;

#define GET_MEM_ADDR \
uint16_t H = i8008->REG_FILE.named_regs.H;\
uint16_t L = i8008->REG_FILE.named_regs.L;

#define MEM_CELL RAM[H * 0x100 + L]

void ADD_Rs(i8008_MODEL_s* i8008, uint8_t src)
{
    temp_reg_u temp_reg;
    uint8_t val = i8008->REG_FILE.REGS[src];
    temp_reg.value = (uint16_t) val + (uint16_t) i8008->REG_FILE.named_regs.A;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ADD_M(i8008_MODEL_s* i8008, uint8_t* RAM)
{
    temp_reg_u temp_reg;
    GET_MEM_ADDR
    uint8_t val = MEM_CELL;
    temp_reg.value = (uint16_t) val + (uint16_t) i8008->REG_FILE.named_regs.A;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ADI(i8008_MODEL_s* i8008, uint8_t imm)
{
    temp_reg_u temp_reg;
    temp_reg.value = (uint16_t) imm + (uint16_t) i8008->REG_FILE.named_regs.A;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void SUB_Rs(i8008_MODEL_s* i8008, uint8_t src)
{
    temp_reg_u temp_reg;
    uint8_t val = (~(i8008->REG_FILE.REGS[src]) + 1);
    temp_reg.value = (uint16_t) i8008->REG_FILE.named_regs.A + (uint16_t) val;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void SUB_M(i8008_MODEL_s* i8008, uint8_t* RAM)
{
    temp_reg_u temp_reg;
    GET_MEM_ADDR
    uint8_t val = (~(MEM_CELL) + 1);
    temp_reg.value = (uint16_t) i8008->REG_FILE.named_regs.A + (uint16_t) val;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void SUI(i8008_MODEL_s* i8008, uint8_t imm)
{
    temp_reg_u temp_reg;
    uint8_t val = (~(imm) + 1);
    temp_reg.value = (uint16_t) i8008->REG_FILE.named_regs.A + (uint16_t) val;
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ANA_Rs(i8008_MODEL_s* i8008, uint8_t src)
{
    temp_reg_u temp_reg;
    uint8_t val = i8008->REG_FILE.REGS[src];
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A & val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ANA_M(i8008_MODEL_s* i8008, uint8_t* RAM)
{
    temp_reg_u temp_reg;
    GET_MEM_ADDR
    uint8_t val = MEM_CELL;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A & val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ANI(i8008_MODEL_s* i8008, uint8_t imm)
{
    temp_reg_u temp_reg;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A & imm);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void XRA_Rs(i8008_MODEL_s* i8008, uint8_t src)
{
    temp_reg_u temp_reg;
    uint8_t val = i8008->REG_FILE.REGS[src];
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A ^ val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void XRA_M(i8008_MODEL_s* i8008, uint8_t* RAM)
{
    temp_reg_u temp_reg;
    GET_MEM_ADDR
    uint8_t val = MEM_CELL;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A ^ val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void XRI(i8008_MODEL_s* i8008, uint8_t imm)
{
    temp_reg_u temp_reg;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A ^ imm);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ORA_Rs(i8008_MODEL_s* i8008, uint8_t src)
{
    temp_reg_u temp_reg;
    uint8_t val = i8008->REG_FILE.REGS[src];
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A | val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ORA_M(i8008_MODEL_s* i8008, uint8_t* RAM)
{
    temp_reg_u temp_reg;
    GET_MEM_ADDR
    uint8_t val = MEM_CELL;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A | val);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

void ORI(i8008_MODEL_s* i8008, uint8_t imm)
{
    temp_reg_u temp_reg;
    temp_reg.value = (uint16_t) (i8008->REG_FILE.named_regs.A | imm);
    SET_FILP_FLOPS
    i8008->REG_FILE.named_regs.A = temp_reg.bytes[0];
}

