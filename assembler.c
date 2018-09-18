#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void OutputGenerate(uint32_t);
uint32_t GetOpCode(char*, char*, char*, char*);

int main(int argc, char *argv[]) {                              
	if (argc < 2) {
		printf("Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0]);
		exit(1);
	}

	FILE * fp;

	/* Open program file. */
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Error: Can't open program file %s\n", argv[1]);
		exit(-1);
	}
	
	size_t OpCode_size = 10;
	char *TempOpCode = malloc(10 * sizeof(char));

    size_t buffer_size = 30;
    char *buffer = malloc(buffer_size * sizeof(char));

    size_t ARGUMENT_SIZE = 7;
    char *ARGUMENT1 = malloc(ARGUMENT_SIZE * sizeof(char));
    char *ARGUMENT2 = malloc(ARGUMENT_SIZE * sizeof(char));
    char *ARGUMENT3 = malloc(ARGUMENT_SIZE * sizeof(char));

    uint32_t *Op_Code_Decoded = malloc(sizeof(uint32_t));

    while(-1 != getline(&buffer, &buffer_size, fp)){
        printf("%s", buffer);
		int i = 0;
		int ARG_COUNTER = 0;
		while ((buffer[i] != ' ')&&(buffer[i] != '\n')){ //OPCODE
 			TempOpCode[i] = buffer[i];
			i++;
		}
		if (buffer[i] != '\n'){
			i++;
			while ((buffer[i] != ' ')&&(buffer[i] != '\n')){ //ARGUMENT 1
 				ARGUMENT1[ARG_COUNTER] = buffer[i];
				i++;
				ARG_COUNTER++;
			}
			if (buffer[i] != '\n'){
				i++;
				ARG_COUNTER = 0;
				while ((buffer[i] != ' ')&&(buffer[i] != '\n')){ //ARGUMENT 2
 					ARGUMENT2[ARG_COUNTER] = buffer[i];
					i++;
					ARG_COUNTER++;
				}
				if (buffer[i] != '\n'){
					i++;
					ARG_COUNTER = 0;
					while ((buffer[i] != ' ')&&(buffer[i] != '\n')){ //ARGUMENT 3
 						ARGUMENT3[ARG_COUNTER] = buffer[i];
						i++;
						ARG_COUNTER++;
					}
				}
			}
		}

		*Op_Code_Decoded = GetOpCode(TempOpCode, ARGUMENT1, ARGUMENT2, ARGUMENT3);
		printf("%x\n", *Op_Code_Decoded);
		//OutputGenerate(*Op_Code_Decoded);
		// NEED TO CLEAR TEMPOPCODE
		int clear = 0;
		for (clear = 0; clear < 10; clear++)
			TempOpCode[clear] = '\0';
		clear = 0;
		for (clear = 0; clear < 7; clear++){
			ARGUMENT1[clear] = '\0';
			ARGUMENT2[clear] = '\0';
			ARGUMENT3[clear] = '\0';
		}
	}


    fclose(fp);
    free(buffer);
	free(TempOpCode);
	free(Op_Code_Decoded);
	free(ARGUMENT1);
	free(ARGUMENT2);
	free(ARGUMENT3);
    return 0;
}

//FUNCTION TO RETURN REGISTER NUMBER
int GET_REGISTER_NUM(char* ARG){
	if (strcmp(ARG, "$zero,") == 0)
		return 0;
	else if (strcmp(ARG, "$at,") == 0)
		return 1;
	else if (strcmp(ARG, "$v0,") == 0)
		return 2;
	else if (strcmp(ARG, "$v1,") == 0)
		return 3;
	else if (strcmp(ARG, "$a0,") == 0)
		return 4;
	else if (strcmp(ARG, "$a1,") == 0)
		return 5;
	else if (strcmp(ARG, "$a2,") == 0)
		return 6;
	else if (strcmp(ARG, "$a3,") == 0)
		return 7;
	else if (strcmp(ARG, "$t0,") == 0)
		return 8;
	else if (strcmp(ARG, "$t1,") == 0)
		return 9;
	else if (strcmp(ARG, "$t2,") == 0)
		return 10;
	else if (strcmp(ARG, "$t3,") == 0)
		return 11;
	else if (strcmp(ARG, "$t4,") == 0)
		return 12;
	else if (strcmp(ARG, "$t5,") == 0)
		return 13;
	else if (strcmp(ARG, "$t6,") == 0)
		return 14;
	else if (strcmp(ARG, "$t7,") == 0)
		return 15;
	else if (strcmp(ARG, "$s0,") == 0)
		return 16;
	else if (strcmp(ARG, "$s1,") == 0)
		return 17;
	else if (strcmp(ARG, "$s2,") == 0)
		return 18;
	else if (strcmp(ARG, "$s3,") == 0)
		return 19;
	else if (strcmp(ARG, "$s4,") == 0)
		return 20;
	else if (strcmp(ARG, "$s5,") == 0)
		return 21;
	else if (strcmp(ARG, "$s6,") == 0)
		return 22;
	else if (strcmp(ARG, "$s7,") == 0)
		return 23;
	else if (strcmp(ARG, "$t8,") == 0)
		return 24;
	else if (strcmp(ARG, "$t9,") == 0)
		return 25;
	else if (strcmp(ARG, "$k0,") == 0)
		return 26;
	else if (strcmp(ARG, "$k1,") == 0)
		return 27;
	else if (strcmp(ARG, "$gp,") == 0)
		return 28;
	else if (strcmp(ARG, "$sp,") == 0)
		return 29;
	else if (strcmp(ARG, "$fp,") == 0)
		return 30;
	else if (strcmp(ARG, "$ra,") == 0)
		return 31;
	else
		return 40;
}

uint32_t GetOpCode(char* OpCode, char* ARG1, char* ARG2, char* ARG3){
/*
	printf("ARG1: %s", ARG1);
	printf("ARG2: %s", ARG2);
	printf("ARG3: %s", ARG3);
*/

    unsigned int RS = 0; //MUST OR WITH OXFFE0 + VALUE IN REG

    unsigned int RT = 0; //MUST OR WITH OXFFE0 + VALUE IN REG

    unsigned int RD = 0; //MUST OR WITH OXFFE0 + VALUE IN REG

    unsigned int SA = 0; //MUST OR WITH OXFFE0 + VALUE IN REG

    unsigned int BASE = 0; //MUST OR WITH OXFFE0 + VALUE IN REG

    unsigned long TARGET = 0; //MUST OR WITH 0XFC00000 + VALUE

    unsigned long CODE = 0; //MUST OR WITH 0XFFF00000 + VALUE

    unsigned long IMMEDIATE = 0; //MUST OR WITH 0XFFFF0000

	if (strcmp(OpCode, "add") == 0){
		RD = GET_REGISTER_NUM(ARG3) << 11;
		RS = GET_REGISTER_NUM(ARG1) << 21;
		RT = GET_REGISTER_NUM(ARG2) << 16;
		printf("ADD RD=%d RS=%d RT=%d\n", RD,RS,RT);
		return (0x20 | RS | RT | RD);
	}
	else if (strcmp(OpCode, "addu") == 0){
		RD = GET_REGISTER_NUM(ARG3) << 11;
		RS = GET_REGISTER_NUM(ARG1) << 21;
		RT = GET_REGISTER_NUM(ARG2) << 16;
		printf("ADDU RD=%d RS=%d RT=%d\n", RD,RS,RT);
		return (0x21 | RS | RT | RD);
	}
	else if (strcmp(OpCode, "addi") == 0){
		RD = GET_REGISTER_NUM(ARG3) << 11;
		RS = GET_REGISTER_NUM(ARG1) << 21;
		RT = GET_REGISTER_NUM(ARG2) << 16;
		printf("ADDI RD=%d RS=%d RT=%d\n", RD,RS,RT);
		return 0x10000000;
	}
	else if (strcmp(OpCode, "addiu") == 0){
		RS = GET_REGISTER_NUM(ARG1) << 16;
		RT = GET_REGISTER_NUM(ARG2) << 21;
		IMMEDIATE = atoi(ARG3);
		printf("ADDIU RS=%d RT=%d IMMEDIATE=%d\n", RS,RT, IMMEDIATE);
		return (0x24000000 | RS | RT | IMMEDIATE);
	}
	else if (strcmp(OpCode, "sub") == 0){
		RD = GET_REGISTER_NUM(ARG3) << 11;
		RS = GET_REGISTER_NUM(ARG1) << 21;
		RT = GET_REGISTER_NUM(ARG2) << 16;
		printf("SUB RD=%d RS=%d RT=%d\n", RD,RS,RT);	
		return 0x22;
	}
	else if (strcmp(OpCode, "subu") == 0){
		RD = GET_REGISTER_NUM(ARG3) << 11;
		RS = GET_REGISTER_NUM(ARG1) << 21;
		RT = GET_REGISTER_NUM(ARG2) << 16;
		printf("SUBU RS=%d RT=%d RD=%d\n", RS,RT,RD);	
		return 0x23;
	}
	else if (strcmp(OpCode, "mult") == 0)
		return 0x18;
	else if (strcmp(OpCode, "multu") == 0)
		return 0x19;
	else if (strcmp(OpCode, "div") == 0)
		return 0x1A;
	else if (strcmp(OpCode, "divu") == 0)
		return 0x1B;
	else if (strcmp(OpCode, "and") == 0)
		return 0x24;
	else if (strcmp(OpCode, "andi") == 0)
		return 0x18000000;
	else if (strcmp(OpCode, "or") == 0)
		return 0x25;
	else if (strcmp(OpCode, "ori") == 0)
		return 0x1A000000;
	else if (strcmp(OpCode, "xor") == 0)
		return 0x26;
	else if (strcmp(OpCode, "xori") == 0)
		return 0x1C000000;
	else if (strcmp(OpCode, "nor") == 0)
		return 0x27;
	else if (strcmp(OpCode, "slt") == 0)
		return 0x2A;
	else if (strcmp(OpCode, "slti") == 0)
		return 0x14000000;
	else if (strcmp(OpCode, "sll") == 0)
		return 0x0;
	else if (strcmp(OpCode, "srl") == 0)
		return 0x2;
	else if (strcmp(OpCode, "sra") == 0)
		return 0x3;
	else if (strcmp(OpCode, "lw") == 0)
		return 0x46000000;
	else if (strcmp(OpCode, "lb") == 0)
		return 0x40000000;
	else if (strcmp(OpCode, "lh") == 0)
		return 0x42000000;
	else if (strcmp(OpCode, "lui") == 0)
		return 0x1E000000;
	else if (strcmp(OpCode, "sw") == 0)
		return 0x56000000;
	else if (strcmp(OpCode, "sb") == 0)
		return 0x50000000;
	else if (strcmp(OpCode, "sh") == 0)
		return 0x52000000;
	else if (strcmp(OpCode, "mfhi") == 0)
		return 0x10;
	else if (strcmp(OpCode, "mflo") == 0)
		return 0x12;
	else if (strcmp(OpCode, "mthi") == 0)
		return 0x11;
	else if (strcmp(OpCode, "mtlo") == 0)
		return 0x13;
	else if (strcmp(OpCode, "beq") == 0)
		return 0x8000000;
	else if (strcmp(OpCode, "bne") == 0)
		return 0xA000000;
	else if (strcmp(OpCode, "blez") == 0)
		return 0xC000000;
	else if (strcmp(OpCode, "bltz") == 0)
		return 0x2000000;
	else if (strcmp(OpCode, "bgez") == 0)
		return 0x2008000;
	else if (strcmp(OpCode, "bgtz") == 0)
		return 0xE000000;
	else if (strcmp(OpCode, "j") == 0)
		return 0x4000000;
	else if (strcmp(OpCode, "jr") == 0)
		return 0x8;
	else if (strcmp(OpCode, "jal") == 0)
		return 0x6000000;
	else if (strcmp(OpCode, "jalr") == 0)
		return 0x9;
	else if (strcmp(OpCode, "syscall") == 0)
		return 0xC;
	else
		printf("ERROR! NO OPCODE EXISTS\n");

	printf("%s\n", OpCode);
	//DECODE OPCODE
	return 0xFFFFFFFF;
}


void OutputGenerate(uint32_t LINE){
	//printf("%x\n", LINE);
	//FILE * out;
	//out = fopen("MachineCode.in", "a");
	//if (out == NULL){
	//	printf("Error: Can't create output file\n");
	//	exit(-1);
	//}
	//fprintf(out, "%x\n", LINE);
   
//fclose(out);
}
