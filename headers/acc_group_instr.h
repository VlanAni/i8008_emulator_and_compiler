#ifndef ACC_GROUP_INSTR_H_INCLUDED
#define ACC_GROUP_INSTR_H_INCLUDED

typedef union temp_reg_u
{
    uint16_t value;
    uint8_t bytes[2];
} temp_reg_u;

void ADD_Rs(i8008_MODEL_s* i8008, uint8_t src);

void ADD_M(i8008_MODEL_s* i8008, uint8_t* RAM);

void ADI(i8008_MODEL_s* i8008, uint8_t imm);

void SUB_Rs(i8008_MODEL_s* i8008, uint8_t src);

void SUB_M(i8008_MODEL_s* i8008, uint8_t* RAM);

void SUI(i8008_MODEL_s* i8008, uint8_t imm);

void ANA_Rs(i8008_MODEL_s* i8008, uint8_t src);

void ANA_M(i8008_MODEL_s* i8008, uint8_t* RAM);

void ANI(i8008_MODEL_s* i8008, uint8_t imm);

void XRA_Rs(i8008_MODEL_s* i8008, uint8_t src);

void XRA_M(i8008_MODEL_s* i8008, uint8_t* RAM);

void XRI(i8008_MODEL_s* i8008, uint8_t imm);

void ORA_Rs(i8008_MODEL_s* i8008, uint8_t src);

void ORA_M(i8008_MODEL_s* i8008, uint8_t* RAM);

void ORI(i8008_MODEL_s* i8008, uint8_t imm);

#endif // ACC_GROUP_INSTR_H_INCLUDED
