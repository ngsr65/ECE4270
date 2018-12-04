#ifndef WB_H
#define WB_H

void WB(){
  uint32_t rt = (MEM_WB.IR & 0x001F0000) >> 16, rd = (MEM_WB.IR & 0x0000F800) >> 11;
  uint8_t opCode = ( MEM_WB.IR >> 26 ) & 0x3f, flag = 0;

  if( opCode == 0 ){
    opCode = MEM_WB.IR & 63;
    flag = 1;
  }
  else if( opCode == 1 ){
    opCode = ( MEM_WB.IR >> 16 ) & 31;
    flag = 2;
  }

  if (MEM_WB.IR == 0){ return; }

  switch(opCode){
    case 32:
      if (flag == 0){
        //LB
        NEXT_STATE.REGS[rt] = MEM_WB.LMD;
      } else {
        //ADD
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 8:
      if( flag == 0 ){
        //ADDI
        NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      } else {
        //JR

      }
      break;

    case 9:
      if( flag == 1 ){
        //JALR
      } else {
        //ADDIU
        NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      }
      break;

    case 33:
      if( flag == 0 ){
        //LH
        NEXT_STATE.REGS[rt] = MEM_WB.LMD;
      } else {
        //ADDU
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 34:
      //SUB
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;//NEXT_STATE.REGS[rd] = MEM_WB.LMD;

    case 35:
      if( flag == 0 ){
      //LW
        NEXT_STATE.REGS[rt] = MEM_WB.LMD;
      } else {
        //SUBU
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 24:
      //MULT

      break;
    case 25:
      //multu

      break;

    case 26:
      //DIV

      break;

    case 27:

      break;

    case 36:
      //AND
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 12:
      if(flag == 0){
        //ANDI
        NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      } else {
        //SYSCALL

      }
      break;

    case 37:
      //OR
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 13:
      //ORI
      NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      break;

    case 38:
      //XOR
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 14:
      //XORI
      NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      break;

    case 39:
      //NOR
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 42:
      //SLT
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 10:
      //SLTI
      NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      break;

    case 0:
      if( flag == 2 ){
        //BLTZ

      } else {
        //SLL
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 2:
      if( flag == 0 ){
        //J

      } else {
        //SRL
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 3:
      if( flag == 0 ){
        //JAL

      } else {
        //SRA
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      }
      break;

    case 15:
      //LUI
      NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
      break;

    case 43:
      //SW

      break;

    case 40:
      //SB

      break;

    case 41:
      //SH

      break;

    case 16:
      //MFHI
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 18:
      //MFLO
      NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
      break;

    case 17:
      //MTHI

      break;

    case 19:
      //MTLO

      break;

    case 4:
      //BEQ

      break;

    case 5:
      //BNE

      break;

    case 6:
      //BLEZ

      break;

    case 1:
      //BGEZ

      break;

    case 7:
      //BGTZ

      break;

    default:
      break;
  }

INSTRUCTION_COUNT++;
}

#endif
