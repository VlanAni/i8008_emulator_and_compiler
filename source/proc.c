#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <proc_abstr.h>
#include <index_reg_instr.h>

int main()
{
    i8008_MODEL_s I8008M;
    uint8_t* RAM = (uint8_t*) malloc (sizeof(uint8_t) * ADDR_AREA_SIZE); // external memory for processor
    printf("The RAM has been enabled successfully\n");
    MVI_Rd_Imm(&I8008M, 0b001, 0);
    printf("B: %u\n", I8008M.REG_FILE.named_regs.B);
    DEC_Rd(&I8008M, 0b001);
    printf("B: %u\n", I8008M.REG_FILE.named_regs.B);
    printf("PF - %u\n", I8008M.FLAGS.PF);
    printf("ZF - %u\n", I8008M.FLAGS.ZF);
    printf("SF - %u\n", I8008M.FLAGS.SF);
    free(RAM);
    printf("The RAM has been disabled successfully");
    return 0;
}

