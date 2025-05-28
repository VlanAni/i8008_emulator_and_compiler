#ifndef SYMBOLS_TABLE_H_INCLUDED
#define SYMBOLS_TABLE_H_INCLUDED

typedef union mem_addr_u
{
    uint16_t value;
    uint8_t bytes[2];
} mem_addr_s;

typedef struct table_node_s
{
    char* symbol;
    mem_addr_s jump_addr;
    struct table_node_s* next;
} table_node_s;

typedef struct stack_node_s
{
    uint16_t instr_addr;
    char* label;
} stack_node_s;

typedef struct stack_s
{
    stack_node_s* arr;
    int cap;
    int length;
} stack_s;

uint32_t PolinHash(char* st);

stack_s* InitStack ();

void JmpPush (stack_s* stack, uint16_t instr_addr, char* label);

table_node_s** InitSymbolTable ();

void AddElem (table_node_s** table, char* symbol, uint16_t value);

table_node_s* GetElement (table_node_s** table, char* symbol);

void DestroyTable (table_node_s** table);

int FillAddr (stack_node_s* addrs, int length, table_node_s** table, uint8_t* RAM);

void DestroyStack (stack_s* addrs);

#endif // SYMBOLS_TABLE_H_INCLUDED
