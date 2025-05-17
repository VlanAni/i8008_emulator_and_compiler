#ifndef INDEX_REG_INSTR_H_INCLUDED
#define INDEX_REG_INSTR_H_INCLUDED

void MOV_Rd_Rs(i8008_MODEL_s* I8008M, uint8_t dest, uint8_t source);

void MOV_Rd_M(i8008_MODEL_s* I8008M, uint8_t dest, uint8_t* RAM);

void MOV_M_Rs(i8008_MODEL_s* I8008M, uint8_t source, uint8_t* RAM);

void MVI_Rd_Imm(i8008_MODEL_s* I8008M, uint8_t dest, uint8_t val);

void MVI_M_Imm(i8008_MODEL_s* I8008M, uint8_t val, uint8_t* RAM);

void INC_Rd(i8008_MODEL_s* I8008M, uint8_t dest);

void DEC_Rd(i8008_MODEL_s* I8008M, uint8_t dest);

#endif // INDEX_REG_INSTR_H_INCLUDED
