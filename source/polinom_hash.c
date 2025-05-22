#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <commands_hashes.h>

uint32_t PolinHash(const char* st)
{
    uint32_t p = 1, summ = 0, s;
    for (int i = 0; i < 3; i++)
    {
        s = (uint32_t) ((uint8_t) st[i] - 'A' + (uint8_t) p);
        summ += s * p;
        p *= P;
    }
    return summ % REM;
}

int main()
{
    int* hashes = (int*) calloc (REM, sizeof(int));
    int calculated_hash;
    const char* commands[14] = {"MOV", "MVI", "DEC", "INC", "ADD", "ADI", "SUB", "SUI", "ANA", "ANI", "XRA", "XRI", "ORA", "ORI"};
    int amount_of_intersects = 0;
    for (int i = 0; i < 14; i++)
    {
        calculated_hash = PolinHash(commands[i]);
        if (hashes[calculated_hash])
            amount_of_intersects++;
        hashes[calculated_hash] += 1;
    }
    free(hashes);
    printf("Intersects amount %d\n", amount_of_intersects);
    FILE* output = freopen("../IO/hashes.txt", "w", stdout);
    printf("#define MOV_HASH %u\n", PolinHash("MOV"));
    printf("#define MVI_HASH %u\n", PolinHash("MVI"));
    printf("#define DEC_HASH %u\n", PolinHash("DEC"));
    printf("#define INC_HASH %u\n", PolinHash("INC"));
    printf("#define ADD_HASH %u\n", PolinHash("ADD"));
    printf("#define ADI_HASH %u\n", PolinHash("ADI"));
    printf("#define SUB_HASH %u\n", PolinHash("SUB"));
    printf("#define SUI_HASH %u\n", PolinHash("SUI"));
    printf("#define ANA_HASH %u\n", PolinHash("ANA"));
    printf("#define ANI_HASH %u\n", PolinHash("ANI"));
    printf("#define XRA_HASH %u\n", PolinHash("XRA"));
    printf("#define XRI_HASH %u\n", PolinHash("XRI"));
    printf("#define ORA_HASH %u\n", PolinHash("ORA"));
    printf("#define ORI_HASH %u\n", PolinHash("ORI"));
    fclose(output);
    return 0;
}
