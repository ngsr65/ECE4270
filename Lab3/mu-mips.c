#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "mu-mips.h"

/***************************************************************/
/* Print out a list of commands available                                                                  */
/***************************************************************/
void help() {        
    printf("------------------------------------------------------------------\n\n");
    printf("\t**********MU-MIPS Help MENU**********\n\n");
    printf("sim\t-- simulate program to completion \n");
    printf("run <n>\t-- simulate program for <n> instructions\n");
    printf("rdump\t-- dump register values\n");
    printf("reset\t-- clears all registers/memory and re-loads the program\n");
    printf("input <reg> <val>\t-- set GPR <reg> to <val>\n");
    printf("mdump <start> <stop>\t-- dump memory from <start> to <stop> address\n");
    printf("high <val>\t-- set the HI register to <val>\n");
    printf("low <val>\t-- set the LO register to <val>\n");
    printf("print\t-- print the program loaded into memory\n");
    printf("show\t-- print the current content of the pipeline registers\n");
    printf("?\t-- display help menu\n");
    printf("quit\t-- exit the simulator\n\n");
    printf("------------------------------------------------------------------\n\n");
}

/***************************************************************/
/* Read a 32-bit word from memory                                                                            */
/***************************************************************/
uint32_t mem_read_32(uint32_t address)
{
    int i;
    for (i = 0; i < NUM_MEM_REGION; i++) {
        if ( (address >= MEM_REGIONS[i].begin) &&  ( address <= MEM_REGIONS[i].end) ) {
            uint32_t offset = address - MEM_REGIONS[i].begin;
            return (MEM_REGIONS[i].mem[offset+3] << 24) |
                (MEM_REGIONS[i].mem[offset+2] << 16) |
                (MEM_REGIONS[i].mem[offset+1] <<  8) |
                (MEM_REGIONS[i].mem[offset+0] <<  0);
        }
    }
    return 0;
}

/***************************************************************/
/* Write a 32-bit word to memory                                                                                */
/***************************************************************/
void mem_write_32(uint32_t address, uint32_t value)
{
    int i;
    uint32_t offset;
    for (i = 0; i < NUM_MEM_REGION; i++) {
        if ( (address >= MEM_REGIONS[i].begin) && (address <= MEM_REGIONS[i].end) ) {
            offset = address - MEM_REGIONS[i].begin;

            MEM_REGIONS[i].mem[offset+3] = (value >> 24) & 0xFF;
            MEM_REGIONS[i].mem[offset+2] = (value >> 16) & 0xFF;
            MEM_REGIONS[i].mem[offset+1] = (value >>  8) & 0xFF;
            MEM_REGIONS[i].mem[offset+0] = (value >>  0) & 0xFF;
        }
    }
}

/***************************************************************/
/* Execute one cycle                                                                                                              */
/***************************************************************/
void cycle() {                                                
    handle_pipeline();
    CURRENT_STATE = NEXT_STATE;
    CYCLE_COUNT++;
}

/***************************************************************/
/* Simulate MIPS for n cycles                                                                                       */
/***************************************************************/
void run(int num_cycles) {                                      

    if (RUN_FLAG == FALSE) {
        printf("Simulation Stopped\n\n");
        return;
    }

    printf("Running simulator for %d cycles...\n\n", num_cycles);
    int i;
    for (i = 0; i < num_cycles; i++) {
        if (RUN_FLAG == FALSE) {
            printf("Simulation Stopped.\n\n");
            break;
        }
        cycle();
    }
}

/***************************************************************/
/* simulate to completion                                                                                               */
/***************************************************************/
void runAll() {                                                     
    if (RUN_FLAG == FALSE) {
        printf("Simulation Stopped.\n\n");
        return;
    }

    printf("Simulation Started...\n\n");
    while (RUN_FLAG){
        cycle();
    }
    printf("Simulation Finished.\n\n");
}

/***************************************************************/ 
/* Dump a word-aligned region of memory to the terminal                              */
/***************************************************************/
void mdump(uint32_t start, uint32_t stop) {          
    uint32_t address;

    printf("-------------------------------------------------------------\n");
    printf("Memory content [0x%08x..0x%08x] :\n", start, stop);
    printf("-------------------------------------------------------------\n");
    printf("\t[Address in Hex (Dec) ]\t[Value]\n");
    for (address = start; address <= stop; address += 4){
        printf("\t0x%08x (%d) :\t0x%08x\n", address, address, mem_read_32(address));
    }
    printf("\n");
}

/***************************************************************/
/* Dump current values of registers to the teminal                                              */   
/***************************************************************/
void rdump() {                               
    int i; 
    printf("-------------------------------------\n");
    printf("Dumping Register Content\n");
    printf("-------------------------------------\n");
    printf("# Instructions Executed\t: %u\n", INSTRUCTION_COUNT);
    printf("# Cycles Executed\t: %u\n", CYCLE_COUNT);
    printf("PC\t: 0x%08x\n", CURRENT_STATE.PC);
    printf("-------------------------------------\n");
    printf("[Register]\t[Value]\n");
    printf("-------------------------------------\n");
    for (i = 0; i < MIPS_REGS; i++){
        printf("[R%d]\t: 0x%08x\n", i, CURRENT_STATE.REGS[i]);
    }
    printf("-------------------------------------\n");
    printf("[HI]\t: 0x%08x\n", CURRENT_STATE.HI);
    printf("[LO]\t: 0x%08x\n", CURRENT_STATE.LO);
    printf("-------------------------------------\n");
}

/***************************************************************/
/* Read a command from standard input.                                                               */  
/***************************************************************/
void handle_command() {                         
    char buffer[20];
    uint32_t start, stop, cycles;
    uint32_t register_no;
    int register_value;
    int hi_reg_value, lo_reg_value;

    printf("MU-MIPS SIM:> ");

    if (scanf("%s", buffer) == EOF){
        exit(0);
    }

    switch(buffer[0]) {
        case 'S':
        case 's':
            if (buffer[1] == 'h' || buffer[1] == 'H'){
                show_pipeline();
            }else {
                runAll(); 
            }
            break;
        case 'M':
        case 'm':
            if (scanf("%x %x", &start, &stop) != 2){
                break;
            }
            mdump(start, stop);
            break;
        case '?':
            help();
            break;
        case 'Q':
        case 'q':
            printf("**************************\n");
            printf("Exiting MU-MIPS! Good Bye...\n");
            printf("**************************\n");
            exit(0);
        case 'R':
        case 'r':
            if (buffer[1] == 'd' || buffer[1] == 'D'){
                rdump();
            }else if(buffer[1] == 'e' || buffer[1] == 'E'){
                reset();
            }
            else {
                if (scanf("%d", &cycles) != 1) {
                    break;
                }
                run(cycles);
            }
            break;
        case 'I':
        case 'i':
            if (scanf("%u %i", &register_no, &register_value) != 2){
                break;
            }
            CURRENT_STATE.REGS[register_no] = register_value;
            NEXT_STATE.REGS[register_no] = register_value;
            break;
        case 'H':
        case 'h':
            if (scanf("%i", &hi_reg_value) != 1){
                break;
            }
            CURRENT_STATE.HI = hi_reg_value; 
            NEXT_STATE.HI = hi_reg_value; 
            break;
        case 'L':
        case 'l':
            if (scanf("%i", &lo_reg_value) != 1){
                break;
            }
            CURRENT_STATE.LO = lo_reg_value;
            NEXT_STATE.LO = lo_reg_value;
            break;
        case 'P':
        case 'p':
            print_program(); 
            break;
        default:
            printf("Invalid Command.\n");
            break;
    }
}

/***************************************************************/
/* reset registers/memory and reload program                                                    */
/***************************************************************/
void reset() {   
    int i;
    /*reset registers*/
    for (i = 0; i < MIPS_REGS; i++){
        CURRENT_STATE.REGS[i] = 0;
    }
    CURRENT_STATE.HI = 0;
    CURRENT_STATE.LO = 0;

    for (i = 0; i < NUM_MEM_REGION; i++) {
        uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
        memset(MEM_REGIONS[i].mem, 0, region_size);
    }

    /*load program*/
    load_program();

    /*reset PC*/
    INSTRUCTION_COUNT = 0;
    CURRENT_STATE.PC =  MEM_TEXT_BEGIN;
    NEXT_STATE = CURRENT_STATE;
    RUN_FLAG = TRUE;
}

/***************************************************************/
/* Allocate and set memory to zero                                                                            */
/***************************************************************/
void init_memory() {                                           
    int i;
    for (i = 0; i < NUM_MEM_REGION; i++) {
        uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
        MEM_REGIONS[i].mem = malloc(region_size);
        memset(MEM_REGIONS[i].mem, 0, region_size);
    }
}

/**************************************************************/
/* load program into memory                                                                                      */
/**************************************************************/
void load_program() {                   
    FILE * fp;
    int i, word;
    uint32_t address;

    /* Open program file. */
    fp = fopen(prog_file, "r");
    if (fp == NULL) {
        printf("Error: Can't open program file %s\n", prog_file);
        exit(-1);
    }

    /* Read in the program. */

    i = 0;
    while( fscanf(fp, "%x\n", &word) != EOF ) {
        address = MEM_TEXT_BEGIN + i;
        mem_write_32(address, word);
        printf("writing 0x%08x into address 0x%08x (%d)\n", word, address, address);
        i += 4;
    }
    PROGRAM_SIZE = i/4;
    printf("Program loaded into memory.\n%d words written into memory.\n\n", PROGRAM_SIZE);
    fclose(fp);
}

/************************************************************/
/* maintain the pipeline                                                                                           */ 
/************************************************************/
void handle_pipeline()
{
    /*INSTRUCTION_COUNT should be incremented when instruction is done*/
    /*Since we do not have branch/jump instructions, INSTRUCTION_COUNT should be incremented in WB stage */

    WB();
    MEM();
    EX();
    ID();
    IF();
}

/************************************************************/
/* writeback (WB) pipeline stage:                                                                          */ 
/************************************************************/
void WB()
{
    uint32_t rt = (MEM_WB.IR & 0x001F0000) >> 16, rd = (MEM_WB.IR & 0x0000F800) >> 11;
	uint8_t opCode = ( MEM_WB.IR >> 26 ) & 0x3f, flag = 0;
	
	//if the instruction in special
    if( opCode == 0 ){
        opCode = MEM_WB.IR & 63;
        flag = 1;
    }
    else if( opCode == 1 ){
        opCode = ( MEM_WB.IR >> 16 ) & 31;
        flag = 2;
    }
	
	if (MEM_WB.IR == 0){
		return;
	}
	
	switch( opCode ){
        //ADD && LB
        case 32:
            if (flag == 0){ //LB
                NEXT_STATE.REGS[rt] = MEM_WB.LMD;
            } else {    //Add
                NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            }
            break;
        case 8:
            //ADDI
            if( flag == 0 ){
                NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
            } else { //JR
			
            }
            break;
        case 9:     //JALR
            if( flag == 1 ){
				
            } else { //ADDIU
                NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
                
            }
            break;
        case 33:
            //LH
            if( flag == 0 ){
		NEXT_STATE.REGS[rt] = MEM_WB.LMD;
            } else { //ADDU
                NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            }
            break;
        case 34:
            //SUB
            NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            break;NEXT_STATE.REGS[rd] = MEM_WB.LMD;
        case 35:
            if( flag == 0 ){
                //LW
                NEXT_STATE.REGS[rt] = MEM_WB.LMD;
            } else { //SUBU
                NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            }
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
            if( flag == 0 ){ //ANDI
                NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
            } else { //SYSCALL

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
            if( flag == 2 ){ //BLTZ	

            } else { //SLL
				NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            }
            break;
        case 2: 
            if( flag == 0 ){ //J
			
            } else { //SRL
				NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            }
            break;
        case 3: 
            if( flag == 0 ){ //JAL
			
            } else { //SRA
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

/************************************************************/
/* memory access (MEM) pipeline stage:                                                          */ 
/************************************************************/
void MEM()
{
    uint8_t opCode = ( MEM_WB.IR >> 26 ) & 0x3f;
    uint8_t flag = 0;
	
	printf("In MEM, opCode = %x\n", opCode);
    //if the instruction in special
    if( opCode == 0 ){
        opCode = IF_EX.IR & 63;
        flag = 1;
    }
    else if( opCode == 1 ){
        opCode = ( IF_EX.IR >> 16 ) & 31;
        flag = 2;
    }
	
	MEM_WB.IR = EX_MEM.IR;
	MEM_WB.ALUOutput = EX_MEM.ALUOutput;
	
	//Store
	//SW
	if (opCode == 43){
		mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
		printf("In MEM, Storing at address %x\n", EX_MEM.ALUOutput);
	}
	//SB
	if (opCode == 40){
		mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
	}
	//SH
	if (opCode == 41){
		mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
	}
	
	//Load
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

/************************************************************/
/* execution (EX) pipeline stage:                                                                          */ 
/************************************************************/
void EX()
{
    /*IMPLEMENT THIS*/

    uint8_t opCode = ( IF_EX.IR >> 26 ) & 0x3f;
    uint8_t flag = 0;
	uint64_t mulreg;

    //if the instruction in special
    if( opCode == 0 ){
        opCode = IF_EX.IR & 63;
        flag = 1;
    }
    else if( opCode == 1 ){
        opCode = ( IF_EX.IR >> 16 ) & 31;
        flag = 2;
    }

    //Store the machine code again 
    EX_MEM.IR = IF_EX.IR;

    switch( opCode ){
        //ADD && LB
        case 32:
            if (flag == 0){ //LB
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.B = IF_EX.B;
            } else {    //Add  
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
                if( checkOverflow( IF_EX.A, IF_EX.B ) == 1 ){
                    printf( "Overflow\n" );
                }
            }
            break;
        case 8:
            //ADDI
            if( flag == 0 ){
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                if( checkOverflow( IF_EX.A, IF_EX.imm ) == 1 ){
                    printf( "Overflow\n" );
                }
            } else { //JR
			
            }
            break;
        case 9:     //JALR
            if( flag == 1 ){
				
            } else { //ADDIU
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                if( checkOverflow( IF_EX.A, IF_EX.imm ) == 1 ){
                    printf( "Overflow\n" );
                }
            }
            break;
        case 33:
            //LH
            if( flag == 0 ){
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.B = IF_EX.B;

            } else { //ADDU
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
                if( checkOverflow( IF_EX.A, IF_EX.B ) == 1 ){
                    printf( "Overflow\n" );
                }
            }
            break;
        case 34:
            //SUB
            EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
            break;
        case 35:
            if( flag == 0 ){
                //LW
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.A = IF_EX.B;
            } else { //SUBU
                EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
            }
	    break;
        case 24:
            //MULT
			mulreg = IF_EX.A * IF_EX.B;
            NEXT_STATE.HI = (0xFFFFFFFF00000000 & mulreg) >> 32;
            NEXT_STATE.LO = (0x00000000FFFFFFFF & mulreg);
            break;
        case 25: 
            //multu
            mulreg = IF_EX.A * IF_EX.B;
            NEXT_STATE.HI = (0xFFFFFFFF00000000 & mulreg) >> 32;
            NEXT_STATE.LO = (0x00000000FFFFFFFF & mulreg);
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
            if( flag == 0 ){ //ANDI
                EX_MEM.ALUOutput = IF_EX.A & IF_EX.imm;
            } else { //SYSCALL
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
            //SLT            EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
			if( IF_EX.A < IF_EX.B ){
				EX_MEM.ALUOutput = 1;
			} else {
				EX_MEM.ALUOutput = 0;
			}
            break;
        case 10: 
            //SLTI
            if( IF_EX.A < IF_EX.imm ){
                 EX_MEM.ALUOutput = 1;
             } else {
                 EX_MEM.ALUOutput = 0;
             }
			
			break;
        case 0: 
            if( flag == 2 ){ //BLTZ	

            } else { //SLL
				EX_MEM.ALUOutput = IF_EX.B >> ( ( IF_EX.IR >> 5 ) & 0x001f );
            }
            break;
        case 2: 
            if( flag == 0 ){ //J
			
            } else { //SRL
				EX_MEM.ALUOutput = IF_EX.B >> ( ( IF_EX.IR >> 5 ) & 0x001f );
            }
            break;
        case 3: 
            if( flag == 0 ){ //JAL
			
            } else { //SRA
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
            printf( "\nNot a Valid OpCode: %x", opCode );
            break;
    }

}

/************************************************************/
/* instruction decode (ID) pipeline stage:                                                         */ 
/************************************************************/
void ID()
{
    uint8_t opCode = 0x0, rs, rt, rd;

    opCode = ( ID_IF.IR >> 26 ) & 0x3f;        //get bits 31-26 for the opCode
    rs = ( ID_IF.IR >> 21 ) & 0x1f;     //get the value of RS register 
    rt = ( ID_IF.IR >> 16 ) & 0x1f;     //get the value of RT register 
    rd = ( ID_IF.IR >> 11 ) & 0x1f;     //get the value of RD register 

    IF_EX.IR = ID_IF.IR;
    IF_EX.A = CURRENT_STATE.REGS[ rs ];
    IF_EX.B = CURRENT_STATE.REGS[ rt ];
    IF_EX.imm = ID_IF.IR;

    if( (0x01 & ( ID_IF.IR >> 15 ) ) == 1 ){
        //need to sign extend negative
        IF_EX.imm = IF_EX.imm | 0xFFFF0000;
    } else {
        //positive 
        IF_EX.imm = IF_EX.imm & 0x0000FFFF;
    }
}

/************************************************************/
/* instruction fetch (IF) pipeline stage:                                                              */ 
/************************************************************/
void IF()
{
    ID_IF.IR = mem_read_32( CURRENT_STATE.PC );
    NEXT_STATE.PC += 4;
}


/************************************************************/
/* Initialize Memory                                                                                                    */ 
/************************************************************/
void initialize() { 
    init_memory();
    CURRENT_STATE.PC = MEM_TEXT_BEGIN;
    NEXT_STATE = CURRENT_STATE;
    RUN_FLAG = TRUE;
}

/************************************************************/
/* Print the program loaded into memory (in MIPS assembly format)    */ 
/************************************************************/
void print_program(){
    uint32_t instr, addr;
    uint8_t command, flag = 0, rs, rt, rd, sa; 
    uint16_t immed;
    int i;


    for(i=0; i<PROGRAM_SIZE; i++){
        addr = MEM_TEXT_BEGIN + (i*4);

        //Get instr value
        instr = mem_read_32(addr);

        command = instr >> 26;
        flag = 0;

        if (command == 0){
            command = instr & 63;
            flag = 1;
        }

        if (command == 1){
            command = (instr >> 16) & 31;
            flag = 2;
        }

        rs = (instr >> 21) & 31;
        rt = (instr >> 16) & 31;
        rd = (instr >> 11) & 31;
        sa = (instr >> 6) & 31;
        immed = instr & 0x0FFFF;

        //Parse
        switch (command){
            //ADD && LB
            case 32:
                if (flag == 0){
                    printf("LB \t BASE = %d \t RT = %d\n", rs, rt);
                } else {
                    printf("ADD \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                }
                break;
                //ADDI && JR
            case 8:
                if (flag == 0){
                    printf("ADDI \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                } else {
                    printf("JR \t RS = %d\n", rs);			
                }			
                break;
                //ADDIU && JALR
            case 9:
                if (flag == 1){
                    printf("JALR \t RS = %d \t RD = %d\n", rs, rd);
                } else {
                    printf("ADDIU \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                }
                break;
                //ADDU && LH
            case 33:
                if (flag == 0){
                    printf("LH \t BASE = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                } else {
                    printf("ADDU \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                }
                break;
                //SUB
            case 34:
                printf("SUB \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //SUBU
            case 35:
                if (flag == 0){
                    printf("LW \t BASE = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                } else {
                    printf("SUBU \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                }			
                break;
                //MULT
            case 24:
                printf("MULT \t RS = %d \t RT = %d\n", rs, rt);
                break;
                //MULTU
            case 25:
                printf("MULTU \t RS = %d \t RT = %d\n", rs, rt);
                break;
                //DIV
            case 26:
                printf("DIV \t RS = %d \t RT = %d\n", rs, rt);
                break;
                //DIVU
            case 27:
                printf("DIVU \t RS = %d \t RT = %d\n", rs, rt);
                break;
                //AND
            case 36:
                printf("AND \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //ANDI && SYSCALL
            case 12:
                if (flag == 0){
                    printf("ANDI \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                } else {
                    printf("SYSCALL\n");
                }
                break;
                //OR
            case 37:
                printf("OR \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //ORI
            case 13:
                printf("ORI \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                break;
                //XOR
            case 38:
                printf("XOR \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //XORI
            case 14:
                printf("XORI \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                break;
                //NOR
            case 39:
                printf("NOR \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //SLT
            case 42:
                printf("SLT \t RS = %d \t RT = %d \t RD = %d\n", rs, rt, rd);
                break;
                //SLTI
            case 10:
                printf("SLTI \t RS = %d \t RT = %d \t IMMEDIATE = %d\n", rs, rt, immed);
                break;
                //SLL && BLTZ
            case 0:
                if (flag == 2){
                    printf("BLTZ \t RS = %d \t OFFSET = %d\n", rs, immed);
                } else {			
                    printf("SLL \t RT = %d \t RD = %d \t SA = %d\n", rt, rd, sa);
                }
                break;
                //SRL && J
            case 2:
                if (flag == 0){
                    command = instr & 0x3FFFFFF;
                    printf("J\tTarget: %d\n", command);
                } else {
                    printf("SRL \t RT = %d \t RD = %d \t SA = %d\n", rt, rd, sa);
                }
                break;
                //SRA && JAL
            case 3:
                if (flag == 0){
                    command = instr & 0x3FFFFFF;
                    printf("JAL\tTarget: %d\n", command);
                } else {
                    printf("SRA \t RT = %d \t RD = %d \t SA = %d\n", rt, rd, sa);
                }
                break;
                //LUI
            case 15:
                printf("LUI \t RT = %d \t IMMEDIATE = %d\n", rt, immed);
                break;
                //SW
            case 43:
                printf("SW \t BASE = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                break;
                //SB
            case 40:
                printf("SB \t BASE = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                break;
                //SH
            case 41:
                printf("SH \t BASE = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                break;
                //MFHI
            case 16:
                printf("MFHI \t RD = %d\n", rd);
                break;
                //MFLO
            case 18:
                printf("MFLO \t RD = %d\n", rd);
                break;
                //MTHI
            case 17:
                printf("MTHI \t RS = %d\n", rs);
                break;
                //MTLO
            case 19:
                printf("MTLO \t RS = %d\n", rs);
                break;
                //BEQ
            case 4:
                printf("BEQ \t RS = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                break;
                //BNE
            case 5:
                printf("BNE \t RS = %d \t RT = %d \t OFFSET = %d\n", rs, rt, immed);
                break;
                //BLEZ
            case 6:
                printf("BLEZ \t RS = %d \t OFFSET = %d\n", rs, immed);
                break;
                //BGEZ
            case 1:
                printf("BGEZ \t RS = %d \t OFFSET = %d\n", rs, immed);
                break;
                //BGTZ
            case 7:
                printf("BGTZ \t RS = %d \t OFFSET = %d\n", rs, immed);
                break;
            default: 
                printf("\n");
                break;
        }
    }
}

/************************************************************/
/* Print the current pipeline                                                                                    */ 
/************************************************************/
void show_pipeline(){
    printf("Showing pipelines...\n");

    printf("ID_IF.PC->%x\n", ID_IF.PC);
    printf("ID_IF.IR->%x\n", ID_IF.IR);
    printf("ID_IF.A->%x\n", ID_IF.A);
    printf("ID_IF.B->%x\n", ID_IF.B);
    printf("ID_IF.IMM->%x\n", ID_IF.imm);
    printf("ID_IF.ALUOutput->%x\n", ID_IF.ALUOutput);
    printf("ID_IF.LMD->%x\n\n", ID_IF.LMD);

    printf("IF_EX.PC->%x\n", IF_EX.PC);
    printf("IF_EX.IR->%x\n", IF_EX.IR);
    printf("IF_EX.A->%x\n", IF_EX.A);
    printf("IF_EX.B->%x\n", IF_EX.B);
    printf("IF_EX.IMM->%x\n", IF_EX.imm);
    printf("IF_EX.ALUOutput->%x\n", IF_EX.ALUOutput);
    printf("IF_EX.LMD->%x\n\n", IF_EX.LMD);

    printf("EX_MEM.PC->%x\n", EX_MEM.PC);
    printf("EX_MEM.IR->%x\n", EX_MEM.IR);
    printf("EX_MEM.A->%x\n", EX_MEM.A);
    printf("EX_MEM.B->%x\n", EX_MEM.B);
    printf("EX_MEM.IMM->%x\n", EX_MEM.imm);
    printf("EX_MEM.ALUOutput->%x\n", EX_MEM.ALUOutput);
    printf("EX_MEM.LMD->%x\n\n", EX_MEM.LMD);

    printf("MEM_WB.PC->%x\n", MEM_WB.PC);
    printf("MEM_WB.IR->%x\n", MEM_WB.IR);
    printf("MEM_WB.A->%x\n", MEM_WB.A);
    printf("MEM_WB.B->%x\n", MEM_WB.B);
    printf("MEM_WB.IMM->%x\n", MEM_WB.imm);
    printf("MEM_WB.ALUOutput->%x\n", MEM_WB.ALUOutput);
    printf("MEM_WB.LMD->%x\n\n", MEM_WB.LMD);
}
/**************************************************************
  Check Overflow
 *************************************************************/
int checkOverflow(uint32_t num1, uint32_t num2){
    uint32_t carry1;
    uint64_t carry2;
    char overf1, overf2;

    carry1 = (num1 & 0x7FFFFFFF) + (num2 & 0x7FFFFFFF);
    carry2 = num1 + num2;

    if (carry1 > 2147483647){
        overf1 = 1;
    } else {
        overf1 = 0;
    }

    if (carry2 > 4294967295){
        overf2 = 1;
    } else {
        overf2 = 0;
    }

    if ( overf1 != overf2 ){
        return 1;
    } else {
        return 0;
    }
}
/***************************************************************/
/* main                                                                                                                                   */
/***************************************************************/
int main(int argc, char *argv[]) {                              
    printf("\n**************************\n");
    printf("Welcome to MU-MIPS SIM...\n");
    printf("**************************\n\n");

    if (argc < 2) {
        printf("Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0]);
        exit(1);
    }

    strcpy(prog_file, argv[1]);
    initialize();
    load_program();
    help();
    while (1){
        handle_command();
    }
    return 0;
}
