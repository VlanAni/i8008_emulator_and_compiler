#include <inttypes.h>
#include <proc_abstr.h>
#include <index_reg_instr.h>
#include <stdio.h>


#define GET_MEM_ADDR \
uint16_t H = I8008M->REG_FILE.named_regs.H;\
uint16_t L = I8008M->REG_FILE.named_regs.L;

#define MEM_CELL RAM[H * 0x100 + L]

#define SRC_REG I8008M->REG_FILE.REGS[src]
#define DST_REG I8008M->REG_FILE.REGS[dst]

#define SET_VALUE I8008M->REG_FILE.REGS[dst] = val;

#define SET_FLIP_FLOPS \
I8008M->FLAGS.PF = I8008M->REG_FILE.REGS[dst] & 0b00000001;\
I8008M->FLAGS.ZF = I8008M->REG_FILE.REGS[dst] == 0;\
I8008M->FLAGS.SF = I8008M->REG_FILE.REGS[dst] >> 7;

void MOV_Rd_Rs(i8008_MODEL_s* I8008M, uint8_t dst, uint8_t src) // MOV Rd, Rs
{
    uint8_t val = SRC_REG;
    DST_REG = val;
}

void MOV_Rd_M(i8008_MODEL_s* I8008M, uint8_t dst, uint8_t* RAM) // MOV Rd, M
{
    GET_MEM_ADDR
    uint8_t val = MEM_CELL; // get value from RAM
    DST_REG = val;
}

void MOV_M_Rs(i8008_MODEL_s* I8008M, uint8_t src, uint8_t* RAM) // MOV Rd, M
{
    GET_MEM_ADDR
    uint8_t val = SRC_REG;
    MEM_CELL = val;
}

void MVI_Rd_Imm(i8008_MODEL_s* I8008M, uint8_t dst, uint8_t val)
{
    DST_REG = val;
}

void MVI_M_Imm(i8008_MODEL_s* I8008M, uint8_t val, uint8_t* RAM)
{
    GET_MEM_ADDR
    MEM_CELL = val;
}

void INC_Rd(i8008_MODEL_s* I8008M, uint8_t dst)
{
    DST_REG += 1;
    SET_FLIP_FLOPS
}

void DEC_Rd(i8008_MODEL_s* I8008M, uint8_t dst)
{
    DST_REG -= 1;
    SET_FLIP_FLOPS
}
