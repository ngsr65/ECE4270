#ifndef MEM_H
#define MEM_H

void MEM(){
    MEM_WB.IR = EX_MEM.IR;
    MEM_WB.ALUOutput = EX_MEM.ALUOutput;

    uint8_t opCode = ( MEM_WB.IR >> 26 ) & 0x3f;
    uint8_t flag = 0;

    if( opCode == 0 ){
        opCode = MEM_WB.IR & 63;
        flag = 1;
    }
    else if( opCode == 1 ){
        opCode = ( MEM_WB.IR >> 16 ) & 31;
        flag = 2;
    }

  	//SW
  	if (opCode == 43){
  		  mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
  	}
  	//SB
  	if (opCode == 40){
  		  mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
  	}
  	//SH
  	if (opCode == 41){
  		  mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
  	}

  	//LB
  	if (opCode == 32 && flag == 0){
  		  MEM_WB.LMD = 0x000000FF & mem_read_32(EX_MEM.ALUOutput);
  	}
  	//LH
  	if (opCode == 33 && flag == 0){
  		  MEM_WB.LMD = 0x0000FFFF & mem_read_32(EX_MEM.ALUOutput);
  	}
  	//LW
  	if (opCode == 35 && flag == 0){
  		  MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput);
  	}
}

#endif
