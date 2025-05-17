#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

uint32_t PolinHash(const char* st)
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

int main()
{
    printf("MOV_HASH: %u\n", PolinHash("MOV"));
    printf("MVI_HASH: %u\n", PolinHash("MVI"));
    printf("DEC_HASH: %u\n", PolinHash("DEC"));
    printf("INC_HASH: %u\n", PolinHash("INC"));
    return 0;
}
