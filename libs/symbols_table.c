#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <symbols_table.h>
#define TABLE_SIZE 5000

uint32_t PolinHash(char* st)
{
    uint32_t summ = 0, p = 1;
    for (int i = 0; st[i] != '\0'; i++)
    {
        summ += (uint32_t) (st[i] - 'A') * p;
        p *= 11;
    }
    return summ % TABLE_SIZE;
}

stack_s* InitStack ()
{
    stack_s* stack = (stack_s*) malloc (sizeof(stack_s));
    stack->arr = (stack_node_s*) malloc (sizeof(stack_node_s) * 30);
    stack->length = 0;
    stack->cap = 30;
}

void JmpPush (stack_s* stack, uint16_t instr_addr, char* label)
{
    if (stack->length + 1 > stack->cap)
    {
        stack->cap *= 2;
        stack->arr = (stack_node_s*) realloc (stack->arr, sizeof(stack_node_s) * stack->cap);
    }
    stack->arr[stack->length].instr_addr = instr_addr;
    stack->arr[stack->length].label = label;
    stack->length += 1;
}

table_node_s** InitSymbolTable ()
{
    table_node_s** table = (table_node_s**) malloc(sizeof(table_node_s*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = NULL;
    return table;
}

void AddElem (table_node_s** table, char* symbol, uint16_t value)
{
    uint32_t hash = PolinHash(symbol);
    if (table[hash] == NULL)
    {
        table[hash] = (table_node_s*) malloc (sizeof(table_node_s) * 1);
        table[hash]->next = NULL;
        table[hash]->symbol = symbol;
        table[hash]->jump_addr.value = value;
    }
    else
    {
        table_node_s* new_node = (table_node_s*) malloc (sizeof(table_node_s) * 1);
        new_node->next = table[hash];
        new_node->symbol = symbol;
        new_node->jump_addr.value = value;
        table[hash] = new_node;
    }
}

table_node_s* GetElement (table_node_s** table, char* symbol)
{
    uint32_t hash = PolinHash(symbol);
    table_node_s* node = table[hash];
    if (node == NULL)
        return NULL;
    while (strcmp(node->symbol, symbol) != 0)
    {
        if (node == NULL)
            return NULL;
        node = node->next;
    }
    return node;
}

void DestroyTable (table_node_s** table)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table_node_s* node = table[i];
        table_node_s* next_node;
        while (node != NULL)
        {
            next_node = node->next;
            free(node->symbol);
            free(node);
            node = next_node;
        }
    }
    free(table);
}

int FillAddr (stack_node_s* addrs, int length, table_node_s** table, uint8_t* RAM)
{
    char* label;
    uint16_t instr_addr;
    table_node_s* table_node;
    for (int i = 0; i < length; i++)
    {
        label = addrs[i].label;
        instr_addr = addrs[i].instr_addr;
        table_node = GetElement(table, label);
        if (table_node == NULL)
        {
            printf("\nERROR! No such symbol in the program!!! -> %s\n", label, strlen(label));
            return 1;
        }
        RAM[instr_addr + 2] = table_node->jump_addr.bytes[0];
        RAM[instr_addr + 3] = table_node->jump_addr.bytes[1];
    }
    return 0;
}

void DestroyStack (stack_s* addrs)
{
    for (int i = 0; i < addrs->length; i++)
        free(addrs->arr[i].label);
    free(addrs->arr);
    free(addrs);
}
