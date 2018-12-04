#ifndef ID_H
#define ID_H

void ID(){
  uint8_t opCode = 0x0, rs, rt, rd;

  opCode = ( ID_IF.IR >> 26 ) & 0x3f; //get bits 31-26 for the opCode
  rs = ( ID_IF.IR >> 21 ) & 0x1f;     //RS register
  rt = ( ID_IF.IR >> 16 ) & 0x1f;     //RT register
  rd = ( ID_IF.IR >> 11 ) & 0x1f;     //RD register

  IF_EX.IR = ID_IF.IR;
  IF_EX.A = CURRENT_STATE.REGS[rs];
  IF_EX.B = CURRENT_STATE.REGS[rt];
  IF_EX.imm = ID_IF.IR;

  if( (0x01 & ( ID_IF.IR >> 15 ) ) == 1 ){
      IF_EX.imm = IF_EX.imm | 0xFFFF0000; //NEGATIVE SIGN EXTEND
  } else {
      IF_EX.imm = IF_EX.imm & 0x0000FFFF; //POSITIVE SIGN EXTEND
  }
}

#endif
