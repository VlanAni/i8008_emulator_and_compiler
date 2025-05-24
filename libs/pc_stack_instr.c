#include <inttypes.h>
#include <stdlib.h>
#include <proc_abstr.h>

void JMP(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.PC = addr;
}

void CALL(i8008_MODEL_s* i8008, uint8_t low, uint8_t high)
{
    uint16_t addr = (uint16_t) high  * 0x100 + (uint16_t) low;
    i8008->ADDR_STACK.stack[i8008->ADDR_STACK.SP] = i8008->ADDR_STACK.PC;
    i8008->ADDR_STACK.SP += 1;
    i8008->ADDR_STACK.PC = addr;
}
