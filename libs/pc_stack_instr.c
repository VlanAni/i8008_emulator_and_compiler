#include <inttypes.h>
#include <stdlib.h>
#include <proc_abstr.h>

#define ADD_REM ((i8008->ADDR_STACK.SP + 1) % 7 + 7) % 7
#define SUB_REM ((i8008->ADDR_STACK.SP - 1) % 7 + 7) % 7

void JMP(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void JNC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void JNZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void JP(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void JPO(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void JC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.PC = addr;
    }
}

void JZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.PC = addr;
    }
}

void JM(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.PC = addr;
    }
}

void JPE(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.PC = addr;
    }
}



void CALL(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.SP = ADD_REM;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.PC = addr;
}

void CNC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.SP = ADD_REM;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.PC = addr;
}

void CNZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.SP = ADD_REM;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.PC = addr;
}

void CP(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.SP = ADD_REM;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.PC = addr;
}

void CPO(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
        return;
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.SP = ADD_REM;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.PC = addr;
}

void CC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.SP = ADD_REM;
        i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
        i8008->ADDR_STACK.PC = addr;
    }
}

void CZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.SP = ADD_REM;
        i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
        i8008->ADDR_STACK.PC = addr;
    }
}

void CM(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.SP = ADD_REM;
        i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
        i8008->ADDR_STACK.PC = addr;
    }
}

void CPE(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
    {
        uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
        i8008->ADDR_STACK.SP = ADD_REM;
        i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
        i8008->ADDR_STACK.PC = addr;
    }
}




void RET(i8008_MODEL_s* i8008)
{
    uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
    i8008->ADDR_STACK.SP = SUB_REM;
    i8008->ADDR_STACK.PC = addr;
}

void RNC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
        return;
    uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
    i8008->ADDR_STACK.SP = SUB_REM;
    i8008->ADDR_STACK.PC = addr;
}

void RNZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
        return;
    uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
    i8008->ADDR_STACK.SP = SUB_REM;
    i8008->ADDR_STACK.PC = addr;
}

void RP(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
        return;
    uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
    i8008->ADDR_STACK.SP = SUB_REM;
    i8008->ADDR_STACK.PC = addr;
}

void RPO(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
        return;
    uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
    i8008->ADDR_STACK.SP = SUB_REM;
    i8008->ADDR_STACK.PC = addr;
}

void RC(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.CF)
    {
        uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
        i8008->ADDR_STACK.SP = SUB_REM;
        i8008->ADDR_STACK.PC = addr;
    }
}

void RZ(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.ZF)
    {
        uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
        i8008->ADDR_STACK.SP = SUB_REM;
        i8008->ADDR_STACK.PC = addr;
    }
}

void RM(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.SF)
    {
        uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
        i8008->ADDR_STACK.SP = SUB_REM;
        i8008->ADDR_STACK.PC = addr;
    }
}

void RPE(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    if (i8008->FLAGS.PF)
    {
        uint16_t addr = i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP];
        i8008->ADDR_STACK.SP = SUB_REM;
        i8008->ADDR_STACK.PC = addr;
    }
}
