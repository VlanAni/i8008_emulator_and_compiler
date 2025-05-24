#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <commands_hashes.h>

uint32_t PolinHash(const char* st)
{
    uint32_t p = 1, summ = 0, s;
    for (int i = 0; i < 3; i++)
    {
        s = (uint32_t) ((uint8_t) st[i] - 'A');
        summ += s * p;
        p *= P;
    }
    return summ % REM;
}

int main()
{
    int* hashes = (int*) calloc (REM, sizeof(int));
    int calculated_hash;
    const char* index_reg_commands[4] = {"MOV", "MVI", "DEC", "INC"};
    const char* acc_commands[20] = {"ADD", "ADI", "SUB", "SUI", "ANA", "ANI", "XRA", "XRI", "ORA", "ORI", "SBB", "SCI", "ADC", "ACI", "CMP", "CPI", "RLC", "RRC", "RAL", "RAR"};
    int amount_of_intersects = 0;
    for (int i = 0; i < 4; i++)
    {
        calculated_hash = PolinHash(index_reg_commands[i]);
        if (hashes[calculated_hash])
            amount_of_intersects++;
        hashes[calculated_hash] += 1;
    }
    for (int i = 0; i < 20; i++)
    {
        calculated_hash = PolinHash(acc_commands[i]);
        if (hashes[calculated_hash])
            amount_of_intersects++;
        hashes[calculated_hash] += 1;
    }
    free(hashes);
    printf("Intersects amount %d\n", amount_of_intersects);
    FILE* output = freopen("../IO/hashes.txt", "w", stdout);
    for (int i = 0; i < 4; i++)
    {
        calculated_hash = PolinHash(index_reg_commands[i]);
        printf("#define %s_HASH %" PRIu32 "\n", index_reg_commands[i], calculated_hash);
    }
    for (int i = 0; i < 20; i++)
    {
        calculated_hash = PolinHash(acc_commands[i]);
        printf("#define %s_HASH %" PRIu32 "\n", acc_commands[i], calculated_hash);
    }
    fclose(output);
    return 0;
}
