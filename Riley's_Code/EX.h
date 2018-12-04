#ifndef EX_H
#define EX_H

void EX(){
   EX_MEM.IR = IF_EX.IR;
   uint8_t opCode = ((EX_MEM.IR >> 26 ) & 0x3f);
   uint8_t flag = 0;

   //if the instruction in special
   if(opCode == 0){
       opCode = (IF_EX.IR & 63);
       flag = 1;
   }
   else if( opCode == 1 ){
       opCode = ((IF_EX.IR >> 16) & 31);
       flag = 2;
   }

   switch(opCode){
       case 32:
          if (flag == 0){
          //LB
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
              EX_MEM.B = IF_EX.B;
          } else {
          //ADD
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
              if(checkOverflow(IF_EX.A, IF_EX.B) == 1){ printf( "Overflow\n" ); }
          }
          break;

       case 8:
          if( flag == 0 ){
          //ADDI
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
              if( checkOverflow( IF_EX.A, IF_EX.imm ) == 1 ){ printf( "Overflow\n" ); }
          } else {
          //JR
          }
          break;

       case 9:
          if( flag == 1 ){
          //JALR
          } else {
          //ADDIU
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
              if( checkOverflow( IF_EX.A, IF_EX.imm ) == 1 ){ printf( "Overflow\n" ); }
          }
          break;

       case 33:
          if( flag == 0 ){
          //LH
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
              EX_MEM.B = IF_EX.B;
          } else {
          //ADDU
               EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
               if(checkOverflow(IF_EX.A, IF_EX.B) == 1){ printf("Overflow\n"); }
          }
          break;

       case 34:
          //SUB
          EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
          break;

       case 35:
          if(flag == 0){
          //LW
              EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
              EX_MEM.A = IF_EX.B;
          } else {
          //SUBU
              EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
          }
          break;

       case 24:
          //MULT
          NEXT_STATE.HI = (0xFFFFFFFF00000000 & (IF_EX.A * IF_EX.B)) >> 32;
          NEXT_STATE.LO = (0x00000000FFFFFFFF & (IF_EX.A * IF_EX.B));
          break;

       case 25:
          //MULTU
          NEXT_STATE.HI = (0xFFFFFFFF00000000 & (IF_EX.A * IF_EX.B)) >> 32;
          NEXT_STATE.LO = (0x00000000FFFFFFFF & (IF_EX.A * IF_EX.B));
          break;

       case 26:
          //DIV
          if( IF_EX.B == 0 ){
              printf( "\nCannot Divide by Zero" );
          } else {
              NEXT_STATE.LO = IF_EX.A / IF_EX.B;
              NEXT_STATE.HI = IF_EX.A % IF_EX.B;
          }
          break;

       case 27:
          //DIVU
          if( IF_EX.B == 0 ){
              printf( "\nCannot Divide by Zero" );
          } else {
              NEXT_STATE.LO = IF_EX.A / IF_EX.B;
              NEXT_STATE.HI = IF_EX.A % IF_EX.B;
          }
          break;

       case 36:
          //AND
          EX_MEM.ALUOutput = IF_EX.A & IF_EX.B;
          break;

       case 12:
          if( flag == 0 ){
          //ANDI
              EX_MEM.ALUOutput = IF_EX.A & IF_EX.imm;
          } else {
          //SYSCALL
              RUN_FLAG = FALSE;
          }
          break;

       case 37:
          //OR
          EX_MEM.ALUOutput = IF_EX.A | IF_EX.B;
          break;
       case 13:
          //ORI
          EX_MEM.ALUOutput = IF_EX.A | IF_EX.imm;
          break;

       case 38:
          //XOR
          EX_MEM.ALUOutput = IF_EX.A ^ IF_EX.B;
          break;

       case 14:
          //XORI
          EX_MEM.ALUOutput = IF_EX.A ^ IF_EX.imm;
          break;

       case 39:
          //NOR
          EX_MEM.ALUOutput = ~( IF_EX.A | IF_EX.B );
          break;

       case 42:
          //SLT
          if(IF_EX.A < IF_EX.B){ EX_MEM.ALUOutput = 1;
          } else { EX_MEM.ALUOutput = 0; }
          break;

       case 10:
          //SLTI
          if( IF_EX.A < IF_EX.imm ){ EX_MEM.ALUOutput = 1;
          } else { EX_MEM.ALUOutput = 0; }
          break;

       case 0:
          if(flag == 2){
          //BLTZ
          } else {
          //SLL
              EX_MEM.ALUOutput = IF_EX.B >> ( ( IF_EX.IR >> 5 ) & 0x001f );
          }
          break;

       case 2:
          if(flag == 0){
          //J
          } else {
          //SRL
              EX_MEM.ALUOutput = IF_EX.B >> ( ( IF_EX.IR >> 5 ) & 0x001f );
          }
          break;

       case 3:
          if(flag == 0){
          //JAL
          } else {
          //SRA
              EX_MEM.ALUOutput = IF_EX.B >> ( ( IF_EX.IR >> 5 ) & 0x001f );
          }
          break;

       case 15:
          //LUI
          EX_MEM.ALUOutput = IF_EX.imm << 16;
          EX_MEM.A = IF_EX.A;
          EX_MEM.B = IF_EX.B;
          break;

       case 43:
          //SW
          EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
          EX_MEM.B = IF_EX.B;
          printf("IF_EX.A = %x, IF_EX.B = %x, IF_EX.imm = %x\n", IF_EX.A, IF_EX.B, IF_EX.imm);
          break;

       case 40:
          //SB
          EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
          EX_MEM.B = IF_EX.B;
          break;

       case 41:
          //SH
          EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
          EX_MEM.B = IF_EX.B;
          break;

       case 16:
          //MFHI
          EX_MEM.ALUOutput = CURRENT_STATE.HI;
          break;

       case 18:
          //MFLO
          EX_MEM.ALUOutput = CURRENT_STATE.LO;
          break;

       case 17:
          //MTHI
          NEXT_STATE.HI = IF_EX.A;
          break;

       case 19:
          //MTLO
          NEXT_STATE.LO = IF_EX.A;
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
          printf( "\nNot Valid: %x", opCode );
          break;
   }
 }
#endif
