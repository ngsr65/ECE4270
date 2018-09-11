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
	handle_instruction();
	CURRENT_STATE = NEXT_STATE;
	INSTRUCTION_COUNT++;
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
			runAll(); 
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
/* decode and execute instruction                                                                     */ 
/************************************************************/
void handle_instruction()
{
	//Current state/next state
	
	uint64_t mul;
	uint32_t instr, temp, temp2, temp3;
	uint8_t command, flag = 0, rs, rt, rd, sa; 
	uint16_t immed;

	//Get instr value
	instr = mem_read_32(CURRENT_STATE.PC);

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
			if (flag == 0){	//LB
				temp = mem_read_32(rs + immed);	//Base + offset
				NEXT_STATE.REGS[rt] = temp;  //Put memory into rt
			} else {	//Add	Works I think
				temp = CURRENT_STATE.REGS[rs];
				temp2 = CURRENT_STATE.REGS[rt];
				temp3 = temp + temp2;
				
				//Debug					
				//printf("RS = %d\nCSTATE[rs] = %ld\n", rs, CURRENT_STATE.REGS[rs]);
				//printf("Temp = %ld\n", temp);
				//printf("Temp2 = %ld\n", temp2);
				//printf("Temp3 - Sum = %ld\n\n", temp3);

				if ( checkOverflow(temp, temp2) == 1 ){
					printf("Overflow\n");
				} else {
					NEXT_STATE.REGS[rd] = temp3;
				}
			}
			NEXT_STATE.PC += 4;
			break;
			
		//ADDI && JR
		case 8:
			if (flag == 0){ //ADDI
				temp = CURRENT_STATE.REGS[rs];
				temp2 = immed;
				if (temp2 >> 15 == 1){
					temp2 = temp2 | 0xFFFF0000;
				}
				temp3 = temp + temp2;
				if ( checkOverflow(temp, temp2) == 1 ){
					printf("Overflow\n");
				} else {
					NEXT_STATE.REGS[rt] = temp3;
				}
				NEXT_STATE.PC += 4;
			} else {	//JR
				temp = CURRENT_STATE.REGS[rs];
				if ((temp && 0x00000003) == 0){
					NEXT_STATE.PC += temp;
				} else {
					//Exception	
				}
			}			
			break;
		//ADDIU && JALR
		case 9:
			if (flag == 1){//JALR
				temp = CURRENT_STATE.REGS[rs];
				if ( ((temp && 0x00000003) == 0) && (temp != CURRENT_STATE.REGS[rd]) ){
					NEXT_STATE.PC += temp;
					if ((CURRENT_STATE.REGS[rd]) == 0){
						NEXT_STATE.REGS[31] = NEXT_STATE.PC;	
					} else {
						NEXT_STATE.REGS[rd] = NEXT_STATE.PC;	
					}
				} else {
					//Exception	
				}
			} else {	//ADDIU
				temp = CURRENT_STATE.REGS[rs];
				temp2 = immed;
				if (temp2 >> 15 == 1){
					temp2 = temp2 | 0xFFFF0000;
				}
				temp3 = temp + temp2;
				NEXT_STATE.REGS[rt] = temp3;
				NEXT_STATE.PC += 4;
			}
			break;
		//ADDU && LH
		case 33:
			if (flag == 0){//LH
				temp = mem_read_32( rs + immed );
				NEXT_STATE.REGS[ rt ] = temp;				
			} else {	//ADDU
				temp = CURRENT_STATE.REGS[rs];
				temp2 = CURRENT_STATE.REGS[rt];
				temp3 = temp + temp2;
				NEXT_STATE.REGS[rd] = temp3;
			}
			NEXT_STATE.PC += 4;
			break;
		//SUB
		case 34:
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			temp3 = temp - temp2;
			NEXT_STATE.REGS[rd] = temp3;
			NEXT_STATE.PC += 4;
			break;
		//SUBU/LW
		case 35:
			if (flag == 0){//LW
				temp = immed;
                if( temp >> 15 == 1 ){
                    temp = temp | 0xffff0000;
                }
                temp2 = mem_read_32( CURRENT_STATE.REGS[ rs ] + temp );
                NEXT_STATE.REGS[ rt ] = temp2;
			} else {//SUBU
				temp = CURRENT_STATE.REGS[ rs ];
				temp2 = CURRENT_STATE.REGS[rt];
				temp3 = temp - temp2;
				NEXT_STATE.REGS[rd] = temp3;
			}			
			NEXT_STATE.PC += 4;
			break;
		//MULT
		case 24:	
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			mul = temp * temp2;
			NEXT_STATE.HI = mul >> 32;
			NEXT_STATE.LO = mul & 0x00000000FFFFFFFF;
			NEXT_STATE.PC += 4;
			break;
		//MULTU
		case 25:
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			mul = temp * temp2;
			NEXT_STATE.HI = mul >> 32;
			NEXT_STATE.LO = mul & 0x00000000FFFFFFFF;
			NEXT_STATE.PC += 4;
			break;
		//DIV
		case 26:
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			//Check for division by zero
			if( temp2 == 0 ){
				printf( "\nError Cannot Divide by 0\n" );
			} else {
				NEXT_STATE.HI = temp % temp2;
				NEXT_STATE.LO = temp / temp2;
			}
			NEXT_STATE.PC += 4;
			break;
		//DIVU
		case 27:
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			//Check for division by zero
			if( temp2 == 0 ){
				printf( "\nError Cannot Divide by 0\n" );
			} else {
				NEXT_STATE.HI = temp % temp2;
				NEXT_STATE.LO = temp / temp2;
			}
			NEXT_STATE.PC += 4;
			break;
		//AND
		case 36:
			temp = CURRENT_STATE.REGS[ rs ];
			temp2 = CURRENT_STATE.REGS[ rt ];
			temp3 = temp & temp2;
			NEXT_STATE.REGS[ rd ] = temp3;
			NEXT_STATE.PC += 4;
			break;
		//ANDI && SYSCALL
		case 12:
			if (flag == 0){	//ANDI
				temp = CURRENT_STATE.REGS[ rs ];
				temp2 = temp & immed;
				NEXT_STATE.REGS[ rt ] = temp2;	
				NEXT_STATE.PC += 4;				//SHOULD THIS BE HERE OR OUTSIDE OF IF STATEMENT SO ITS INCREMENTED AFTER SYSCALL TOO?
			} else {	//SYSCALL
				NEXT_STATE.REGS[2] = 10;
				RUN_FLAG = FALSE;
			}
			break;
		//OR
		case 37:
			temp = CURRENT_STATE.REGS[ rs ];
			temp2 = CURRENT_STATE.REGS[ rt ];
			temp3 = temp | temp2;
			NEXT_STATE.REGS[ rd ] = temp3;
			NEXT_STATE.PC += 4;	
			break;
		//ORI
		case 13:
			temp = CURRENT_STATE.REGS[ rs ];
			temp2 = temp | immed;
			NEXT_STATE.REGS[ rt ] = temp2;
			NEXT_STATE.PC += 4;
			break;
		//XOR
		case 38:
			temp = CURRENT_STATE.REGS[ rs ];
			temp2 = CURRENT_STATE.REGS[ rt ];
			temp3 = temp ^ temp2;
			NEXT_STATE.REGS[ rd ] = temp3;
			NEXT_STATE.PC += 4;
			break;
		//XORI
		case 14:
			temp = CURRENT_STATE.REGS[ rs ];
			temp2 = temp ^ immed;
			NEXT_STATE.REGS[ rt ] = temp2; 
			NEXT_STATE.PC +=4;	
			break;
		//NOR
		case 39:
			temp = CURRENT_STATE.REGS[rs];
			temp2 = CURRENT_STATE.REGS[rt];
			temp3 = temp | temp2;
			temp3 = ~temp3;
			NEXT_STATE.REGS[rd] = temp3;
			NEXT_STATE.PC += 4;
			break;
		//SLT
		case 42:
			temp = CURRENT_STATE.REGS[rt];
			temp2 = CURRENT_STATE.REGS[rs];	
			if (temp2 < temp){
				NEXT_STATE.REGS[rd] = 1;
			} else {
				NEXT_STATE.REGS[rd] = 0;
			}
			NEXT_STATE.PC += 4;
			break;
		//SLTI
		case 10:
			temp2 = CURRENT_STATE.REGS[rs];	
			temp = immed;
			if (temp >> 15 == 1){
				temp = temp | 0xFFFF0000;
			}
			if (temp < 2147483648){
				if (temp2 < temp){
					NEXT_STATE.REGS[rt] = 1;
				} else {
				NEXT_STATE.REGS[rt] = 0;
				}
			} else {
				if (temp2 > temp){
					NEXT_STATE.REGS[rt] = 1;
				} else {
					NEXT_STATE.REGS[rt] = 0;
				}
			}
			NEXT_STATE.PC += 4;
			break;
		//SLL && BLTZ
		case 0:
			if (flag == 2){//BLTZ
				temp = immed;
				if (temp >> 15 == 1){
					temp = temp | 0xFFFF0000;
				}
				temp = (immed << 2) + 4;
				if ( (CURRENT_STATE.REGS[rs] >> 31) == 1){
					NEXT_STATE.PC += temp;
				} else {
					NEXT_STATE.PC += 4;
				}
			} else {	//SLL		
				temp = CURRENT_STATE.REGS[rt];
				temp2 = temp << sa;
				NEXT_STATE.REGS[rd] = temp2;
				NEXT_STATE.PC += 4;
			}
			break;
		//SRL && J
		case 2:
			if (flag == 0){//J
				temp = instr & 0x3FFFFFF;
				temp = temp << 2;
				temp2 = (NEXT_STATE.PC + 4) & 0xF0000000;
				temp = temp2 | temp;
				NEXT_STATE.PC = temp;
			} else {	//SRL
				temp = CURRENT_STATE.REGS[rt];
				temp2 = temp >> sa;
				NEXT_STATE.REGS[rd] = temp2;
				NEXT_STATE.PC += 4;
			}
			break;
		//SRA && JAL
		case 3:
			if (flag == 0){//JAL
				temp = instr & 0x3FFFFFF;
				temp = temp << 2;
				temp2 = (NEXT_STATE.PC + 4) & 0xF0000000;
				temp = temp2 | temp;
				NEXT_STATE.REGS[31] = NEXT_STATE.PC + 8;
				NEXT_STATE.PC = temp;
			} else {	//SRA
				temp = CURRENT_STATE.REGS[rt];
				temp2 = temp >> sa;
				if (temp >> 31 == 1){
					temp2 = temp2 | 0xC0000000;
				}
				NEXT_STATE.REGS[rd] = temp2;
				NEXT_STATE.PC += 4;
			}
			break;
		//LUI
		case 15:
			temp = immed << 16;
			NEXT_STATE.REGS[ rt ] = temp; 
			NEXT_STATE.PC += 4;
			break;
		//SW
		case 43:
			temp = immed;
			if (temp >> 15 == 1){
				temp = temp | 0xFFFF0000;
			}
			temp2 = CURRENT_STATE.REGS[rs] + temp;
			if ((temp2 & 0x00000003) == 0){
				mem_write_32(temp2, CURRENT_STATE.REGS[rt]);	
			} else {
				//Exception	
			}
			NEXT_STATE.PC += 4;
			break;
		//SB
		case 40:
			temp = immed;
			if (temp >> 15 == 1){
				temp = temp | 0xFFFF0000;
			}
			temp2 = CURRENT_STATE.REGS[28] + temp;
			if ((temp2 && 0x00000003) == 0){
				mem_write_32(temp2, CURRENT_STATE.REGS[rt] & 0x000000FF);	
			} else {
				//Exception	
			}
			NEXT_STATE.PC += 4;
			break;
		//SH
		case 41:
			temp = immed;
			if (temp >> 15 == 1){
				temp = temp | 0xFFFF0000;
			}
			temp2 = CURRENT_STATE.REGS[28] + temp;
			if ((temp2 && 0x00000003) == 0){
				mem_write_32(temp2, CURRENT_STATE.REGS[rt] & 0x0000FFFF);	
			} else {
				//Exception	
			}
			NEXT_STATE.PC += 4;
			break;
		//MFHI
		case 16:
			NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
			break;
		//MFLO
		case 18:
			NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
			break;
		//MTHI
		case 17:
			CURRENT_STATE.HI = NEXT_STATE.REGS[rs];
			break;
		//MTLO
		case 19:
			CURRENT_STATE.LO = NEXT_STATE.REGS[rs];
			break;
		//BEQ
		case 4:
			if ((mem_read_32(rt)) == (mem_read_32(rs))){ //COMPARISON
				if (((immed << 2) & 0x20000) != 0){ // NEGATIVE SIGN BIT
					temp = ((immed << 2) | 0xFFFC0000);
				}
				else{	//POSITIVE SIGN BIT
					temp = ((immed << 2) & 0xFFFC0000);
				}
			NEXT_STATE.PC = ((CURRENT_STATE.PC + 4) + temp);	//DELAY SLOT + TARGET
			}
			break;
		//BNE
		case 5:
			if ((mem_read_32(rt)) != (mem_read_32(rs))){ //COMPARISON
				if (((immed << 2) & 0x20000) != 0){ // NEGATIVE SIGN BIT
					temp = ((immed << 2) | 0xFFFC0000);
				}
				else{	//POSITIVE SIGN BIT
					temp = ((immed << 2) & 0xFFFC0000);
				}
			NEXT_STATE.PC = ((CURRENT_STATE.PC + 4) + temp);	//DELAY SLOT + TARGET
			}
			break;
		//BLEZ
		case 6:
			if ((mem_read_32(rs) & 0x40000000 == 0) && (mem_read_32(rs) <= 0)){ //COMPARISON
				if (((immed << 2) & 0x20000) != 0){ // NEGATIVE SIGN BIT
					temp = ((immed << 2) | 0xFFFC0000);
				}
				else{	//POSITIVE SIGN BIT
					temp = ((immed << 2) & 0xFFFC0000);
				}
			NEXT_STATE.PC = ((CURRENT_STATE.PC + 4) + temp);	//DELAY SLOT + TARGET
			}
			break;
		//BGEZ
		case 1:
			if ((mem_read_32(rs) & 0x40000000 == 0) && (mem_read_32(rs) >= 0)){ //COMPARISON
				if (((immed << 2) & 0x20000) != 0){ // NEGATIVE SIGN BIT
					temp = ((immed << 2) | 0xFFFC0000);
				}
				else{	//POSITIVE SIGN BIT
					temp = ((immed << 2) & 0xFFFC0000);
				}
			NEXT_STATE.PC = ((CURRENT_STATE.PC + 4) + temp);	//DELAY SLOT + TARGET
			}
			
			break;
		//BGTZ
		case 7:
			if ((mem_read_32(rs) & 0x40000000 == 0) && (mem_read_32(rs) > 0)){ //COMPARISON
				if (((immed << 2) & 0x20000) != 0){ // NEGATIVE SIGN BIT
					temp = ((immed << 2) | 0xFFFC0000);
				}
				else{	//POSITIVE SIGN BIT
					temp = ((immed << 2) & 0xFFFC0000);
				}
			NEXT_STATE.PC = ((CURRENT_STATE.PC + 4) + temp);	//DELAY SLOT + TARGET
			}
			
			break;

		default: 
			
			break;
	}
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
	int i;
	uint32_t addr;
	
	for(i=0; i<PROGRAM_SIZE; i++){
		addr = MEM_TEXT_BEGIN + (i*4);
		printf("[0x%x]\t", addr);
		print_instruction(addr);
	}
}

/************************************************************/
/* Print the instruction at given memory address (in MIPS assembly format)    */
/************************************************************/
void print_instruction(uint32_t addr){
	
	uint32_t instr;
	uint8_t command, flag = 0, rs, rt, rd, sa; 
	uint16_t immed;

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

/* Check Overflow
*
*/
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
