#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <commands_hashes.h>
#include <symbols_table.h>
#include <translator.h>

void InitExecMemory(machine_code_s* MACHINE_CODE)
{
    MACHINE_CODE->buff_size = 100;
    MACHINE_CODE->img = (uint8_t*) malloc (MACHINE_CODE->buff_size);
    MACHINE_CODE->act_size = 0;
}

uint8_t GetRegIdx(char reg)
{
    switch (reg)
    {
        case 'A':
            return 0;
        case 'H':
            return 5;
        case 'L':
            return 6;
        case 'M':
            return 7;
        default:
            return (uint8_t) (reg - 'A');
    }
}

uint32_t PolinHash(char* st)
{
    uint32_t p = 1, summ = 0, s;
    for (int i = 0; st[i] != '\n' && st[i] != '\0'; i++)
    {
        s = (uint32_t) ((uint8_t) st[i] - 'A' + 1);
        summ += s * p;
        p *= P;
    }
    return summ % REM;
}

int InstrEncoder(instr_s* dest, char* instr_buff, stack_s* stack, int act_size, FILE* src)
{
    char* mnemonic;
    char* symbol;
    uint16_t addr_jmp_instr;
    mnemonic = strtok(instr_buff, " ");
    uint32_t hash = PolinHash(mnemonic);
    switch (hash)
    {
        case MOV_HASH:
            dest->group = 0b000;
            dest->opcode.labels.id = 0b11;
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case MVI_HASH:
            dest->group = 0b000;
            uint8_t imm;
            dest->opcode.labels.id = 0;
            mnemonic = strtok(NULL, " ");
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b110;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case INC_HASH:
            mnemonic = strtok(NULL, " ");
            if (mnemonic[0] == 'A')
            {
                fclose(src);
                printf("ERROR! DEC and INC instructions do not work with accumulator (A)!\n");
                return 1;
            }
            dest->group = 0b000;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b000;
            dest->length = 0b01;
            return 0;
        case DEC_HASH:
            mnemonic = strtok(NULL, " ");
            if (mnemonic[0] == 'A')
            {
                fclose(src);
                printf("ERROR! DEC and INC instructions do not work with accumulator (A)!\n");
                return 1;
            }
            dest->group = 0b000;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = GetRegIdx(mnemonic[0]);
            dest->opcode.labels.reg2 = 0b001;
            dest->length = 0b01;
            return 0;
        case ADD_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b000;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case ADI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b000;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case ADC_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b001;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case ACI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b001;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case SUB_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b010;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case SUI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b010;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case SBB_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b011;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case SCI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b011;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case ANA_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b100;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case ANI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b100;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case XRA_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b101;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case XRI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b101;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case ORA_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b110;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case ORI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b110;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case CMP_HASH:
            mnemonic = strtok(NULL, " ");
            dest->group = 0b001;
            dest->opcode.labels.id = 0b10;
            dest->opcode.labels.reg1 = 0b111;
            dest->opcode.labels.reg2 = GetRegIdx(mnemonic[0]);
            dest->length = 0b01;
            return 0;
        case CPI_HASH:
            dest->group = 0b001;
            dest->opcode.labels.id = 0b00;
            dest->opcode.labels.reg1 = 0b111;
            dest->opcode.labels.reg2 = 0b100;
            mnemonic = strtok(NULL, " ");
            sscanf(mnemonic, "%" SCNu8, &imm);
            dest->second_byte = imm;
            dest->length = 0b10;
            return 0;
        case RLC_HASH:
            dest->group = 0b001;
            dest->opcode.value = 0b00000010;
            dest->length = 0b01;
            return 0;
        case RRC_HASH:
            dest->group = 0b001;
            dest->opcode.value = 0b00001010;
            dest->length = 0b01;
            return 0;
        case RAL_HASH:
            dest->group = 0b001;
            dest->opcode.value = 0b00010010;
            dest->length = 0b01;
            return 0;
        case RAR_HASH:
            dest->group = 0b001;
            dest->opcode.value = 0b00011010;
            dest->length = 0b01;
            return 0;
        case RET_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00000111;
            dest->length = 0b01;
            return 0;
        case RNC_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00000011;
            dest->length = 0b01;
            return 0;
        case RNZ_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00001011;
            dest->length = 0b01;
            return 0;
        case RP_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00010011;
            dest->length = 0b01;
            return 0;
        case RPO_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00011011;
            dest->length = 0b01;
            return 0;
        case RC_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00100011;
            dest->length = 0b01;
            return 0;
        case RZ_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00101011;
            dest->length = 0b01;
            return 0;
        case RM_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00110011;
            dest->length = 0b01;
            return 0;
        case RPE_HASH:
            dest->group = 0b010;
            dest->opcode.value = 0b00111011;
            dest->length = 0b01;
            return 0;
        default:
            switch (hash)
            {
                case JMP_HASH:
                    break;
                case JNC_HASH:
                    break;
                case JNZ_HASH:
                    break;
                case JP_HASH:
                    break;
                case JPO_HASH:
                    break;
                case JC_HASH:
                    break;
                case JZ_HASH:
                    break;
                case JM_HASH:
                    break;
                case JPE_HASH:


                case CALL_HASH:
                    break;
                case CNC_HASH:
                    break;
                case CNZ_HASH:
                    break;
                case CP_HASH:
                    break;
                case CPO_HASH:
                    break;
                case CC_HASH:
                    break;
                case CZ_HASH:
                    break;
                case CM_HASH:
                    break;
                case CPE_HASH:
                    break;
                default:
                    fclose(src);
                    printf("\nERROR! No such command in Intel-8008 ISA!!! -> %s\n", mnemonic);
                    return 1;
            }
            char* name = (char*) malloc (sizeof(char) * (strlen(mnemonic) + 1));
            strcpy(name, mnemonic);
            mnemonic = strtok(NULL, " ");
            if (mnemonic == NULL)
            {
                fclose(src);
                printf("\nERROR! No any label after jump-instruction -> [%s ???] \n", name);
                free(name);
                return 1;
            }
            symbol = (char*) malloc (sizeof(char) * (strlen(mnemonic) + 1));
            if (strstr(mnemonic, "\n") != NULL)
                *(strstr(mnemonic, "\n")) = '\0';
            strcpy(symbol, mnemonic);
            addr_jmp_instr = (uint16_t) act_size;
            JmpPush(stack, addr_jmp_instr, symbol);
            switch (hash)
            {
                case JMP_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01000100;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JNC_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01000000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JNZ_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01001000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JP_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01010000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JPO_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01011000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JC_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01100000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JZ_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01101000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JM_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01110000;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case JPE_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01111000;
                    dest->length = 0b11;
                    free(name);
                    return 0;

                case CALL_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01000110;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CNC_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01000010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CNZ_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01001010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CP_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01010010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CPO_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01011010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CC_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01100010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CZ_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01101010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CM_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01110010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
                case CPE_HASH:
                    dest->group = 0b010;
                    dest->opcode.value = 0b01111010;
                    dest->length = 0b11;
                    free(name);
                    return 0;
        }
    }
}

void WriteInMemImg(machine_code_s* mem, instr_s* src)
{
    if (mem->act_size + src->length + 1 > mem->buff_size)
    {
        mem->buff_size *= 2;
        mem->img = (uint8_t*) realloc (mem->img, sizeof(uint8_t) * mem->buff_size);
    }
    switch (src->length)
    {
        case 0b01:
            mem->img[mem->act_size] = src->group;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->opcode.value;
            mem->act_size += 1;
            return;
        case 0b10:
            mem->img[mem->act_size] = src->group;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->opcode.value;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->second_byte;
            mem->act_size += 1;
            return;
        case 0b11:
            mem->img[mem->act_size] = src->group;
            mem->act_size += 1;
            mem->img[mem->act_size] = src->opcode.value;
            mem->act_size += 3;
            return;
    }
}

int main(int argc, char** argv)
{
    char* input_name;
    char* output_name;
    switch (argc)
    {
        case 1:
            printf("\nNo arguments for compiler\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
            return 0;
        case 2:
            if (strcmp(argv[1], "-h"))
                printf("\nIf you use compiler with one flag, it must be only -h\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
            else
            {
                printf("\ni8008compiler - the application which translates Intel-8008 assembly code from .txt-file into binary file\n Output .bin-file can be used as input-file for i8008emulator\n");
                printf("In code you can use Index-reg instructions, Acc-group instructions and PC and stack instructions except RST\n");
                printf("If you want to use labels in your program, labels must be located in such way:\n#####\nLABEL:\nCOMMAND WITH ARGS\n#####\n");
                printf("If compiling finishes successfully, you will see a report. If compiler finds some errors or cannot read the string, you will see the text with description of error\n");
                printf("How to call the program from command line - you can use three variants: [i8008compiler -h] / [i8008compiler -o output_file -i input_file] / [i8008compiler -i input_file -o output_file]\n");
            }
            return 0;
        case 3:
            printf("\nYou can use only this three combinations: [-h] / [-i input_file -o output_file] / [-o output_file -i input_file]\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
            return 0;
        case 4:
            printf("\nYou can use only this three combinations: [-h] / [-i input_file -o output_file] / [-o output_file -i input_file]\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
            return 0;
        case 5:
            if (strcmp(argv[1], "-i") == 0)
            {
                if (strcmp(argv[3], "-o"))
                {
                    printf("The third argument must be flag -o\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
                    return 0;
                }
                if (strstr(argv[2], ".txt") == NULL || strstr(argv[4], ".bin") == NULL)
                {
                    printf("Input file must be .txt-file and output file must be .bin-file\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
                    return 0;
                }
                input_name = argv[2];
                output_name = argv[4];
            }
            else if (strcmp(argv[1], "-o") == 0)
            {
                if (strcmp(argv[3], "-i"))
                {
                    printf("The third argument must be flag -i\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
                    return 0;
                }
                if (strstr(argv[2], ".bin") == NULL || strstr(argv[4], ".txt") == NULL)
                {
                    printf("Input file must be .txt-file and output file must be .bin-file\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
                    return 0;
                }
                input_name = argv[4];
                output_name = argv[2];
            }
            else
            {
                printf("The first argument must be -o or -i\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
                return 0;
            }
            break;
        default:
            printf("\nYou can use only this three combinations: [-h] / [-i input_file -o output_file] / [-o output_file -i input_file]\nPlease, enter [i8008compiler.exe -h] to learn how to use the compiler correctly\n");
            return 0;
    }

    FILE* program = fopen(input_name, "r");
    if (program == NULL)
    {
        printf("\nERROR! File opening failed! Please, check that the file %s is exists\n", input_name);
        return 0;
    }

    machine_code_s MACHINE_CODE;
    InitExecMemory(&MACHINE_CODE); // Initializing memory for executable code

    char instr_buff[INSTR_BUFF_SIZE];
    char* label;
    char* check_pointer;
    char* double_point;
    uint16_t symbol_addr;
    int encoder_code, command_number = 0, line = 0;
    instr_s input_instr;
    table_node_s** symbol_table = InitSymbolTable();
    stack_s* stack = InitStack();

    while (!feof(program))
    {
        check_pointer = fgets(instr_buff, INSTR_BUFF_SIZE, program);

        if (check_pointer == NULL)
        {
            printf("\nERROR! Command reading failed after line %d\n", line);
            fclose(program);
            DEST_EXEC_MEMORY;
            DestroyTable(symbol_table);
            DestroyStack(stack);
            return 0;
        }

        double_point = strstr(instr_buff, ":");

        if (double_point != NULL)
        {
            double_point = strtok(instr_buff, ":");
            check_pointer = strtok(NULL, " ");
            if (check_pointer != NULL && *(check_pointer) != '\n')
            {
                printf("\nERROR! If you have label on the line, there must be only label and no instructions and anything\nLocation of the error: %d\n", line);
                DEST_EXEC_MEMORY;
                DestroyTable(symbol_table);
                DestroyStack(stack);
                return 0;
            }
            label = (char*) malloc(sizeof(char) * (strlen(double_point) + 1));
            strcpy(label, double_point);
            symbol_addr = (uint16_t) MACHINE_CODE.act_size;
            AddElem(symbol_table, label, symbol_addr);
            line++;
            continue;
        }

        encoder_code = InstrEncoder(&input_instr, instr_buff, stack, MACHINE_CODE.act_size, program);

        if (encoder_code != 0)
        {
            printf("Location of the error: %d\n", line);
            DEST_EXEC_MEMORY;
            DestroyTable(symbol_table);
            DestroyStack(stack);
            return 0;
        }
        command_number++;
        line++;
        WriteInMemImg(&MACHINE_CODE, &input_instr);
    }

    fclose(program);

    int fa_code = FillAddr(stack->arr, stack->length, symbol_table, MACHINE_CODE.img);
    if (fa_code)
    {
        DEST_EXEC_MEMORY;
        DestroyTable(symbol_table);
        DestroyStack(stack);
        return 0;
    }

    printf("\nCompiling finished\n");
    FILE* bin_code = fopen(output_name, "wb");
    if (bin_code == NULL)
    {
        printf("\nCannot write compiled program in output-file %s. Please, check that the name of output-file is correct\n", output_name);
        DEST_EXEC_MEMORY;
        DestroyTable(symbol_table);
        DestroyStack(stack);
        return 0;
    }
    fwrite(&MACHINE_CODE.act_size, sizeof(int), 1, bin_code);
    fwrite(MACHINE_CODE.img, sizeof(uint8_t), MACHINE_CODE.act_size, bin_code);
    DEST_EXEC_MEMORY;
    DestroyTable(symbol_table);
    DestroyStack(stack);
    printf("\nCOMPILING REPORT\nNumber of bytes: %d\nProgram size (bytes): %d\nAdditional bytes for emulator working: %d\n", MACHINE_CODE.act_size + sizeof(int), MACHINE_CODE.act_size - command_number, command_number + sizeof(int));
    return 0;
}
