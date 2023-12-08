#pragma once

#include "common.h"

#define CURR_OP_TYPE ((instr->opcode & 0xF000) >> 12)
#define CURR_OP_VX ((instr->opcode & 0x0F00) >> 8)
#define CURR_OP_VY ((instr->opcode & 0x00F0) >> 4)
#define CURR_OP_NNN (instr->opcode & 0x0FFF)
#define CURR_OP_KK (instr->opcode & 0x00FF)
#define CURR_OP_N (instr->opcode & 0x000F)

void OP_CLS(chip8_t *c8, instr_t *instr);
void OP_RET(chip8_t *c8, instr_t *instr);
void OP_JP_ADDR(chip8_t *c8, instr_t *instr);
void OP_CALL_ADDR(chip8_t *c8, instr_t *instr);
void OP_SE_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_SNE_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_SE_VX_VY(chip8_t *c8, instr_t *instr);
void OP_LD_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_ADD_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_LD_VX_VY(chip8_t *c8, instr_t *instr);
void OP_OR_VX_VY(chip8_t *c8, instr_t *instr);
void OP_AND_VX_VY(chip8_t *c8, instr_t *instr);
void OP_XOR_VX_VY(chip8_t *c8, instr_t *instr);
void OP_ADD_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SUB_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SHR_VX(chip8_t *c8, instr_t *instr);
void OP_SUBN_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SHL_VX(chip8_t *c8, instr_t *instr);
void OP_SNE_VX_VY(chip8_t *c8, instr_t *instr);
void OP_LD_I_ADDR(chip8_t *c8, instr_t *instr);
void OP_JP_V0_ADDR(chip8_t *c8, instr_t *instr);
void OP_RND_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_DRW_VX_VY_NIB(chip8_t *c8, instr_t *instr);
void OP_SKP_VX(chip8_t *c8, instr_t *instr);
void OP_SKNP_VX(chip8_t *c8, instr_t *instr);
void OP_LD_VX_DT(chip8_t *c8, instr_t *instr);
void OP_LD_VX_K(chip8_t *c8, instr_t *instr);
void OP_LD_DT_VX(chip8_t *c8, instr_t *instr);
void OP_LD_ST_VX(chip8_t *c8, instr_t *instr);
void OP_ADD_I_VX(chip8_t *c8, instr_t *instr);
void OP_LD_F_VX(chip8_t *c8, instr_t *instr);
void OP_LD_B_VX(chip8_t *c8, instr_t *instr);
void OP_LD_I_VX(chip8_t *c8, instr_t *instr);
void OP_LD_VX_I(chip8_t *c8, instr_t *instr);
