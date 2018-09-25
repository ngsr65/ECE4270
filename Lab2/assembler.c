#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int GET_REGISTER_NUM(char*);

int main(int argc, char *argv[]) {                              
	if (argc < 2) {
		printf("Error: Provide input file.\nUsage: %s <input program> \n\n",  argv[0]);
		exit(1);
	}

	FILE *fp;
	FILE *Output;

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Error: Can't open program file %s\n", argv[1]);
		exit(-1);
	}

	Output = fopen("MIPS_Input.in", "w");
	if (Output == NULL) {
		printf("Error: Can't create program output file\n");
		exit(-1);
	}

    unsigned int RS = 0; //MUST OR WITH OXFFE0 + VALUE IN REG
    unsigned int RT = 0; //MUST OR WITH OXFFE0 + VALUE IN REG
    unsigned int RD = 0; //MUST OR WITH OXFFE0 + VALUE IN REG
    unsigned int SA = 0; //MUST OR WITH OXFFE0 + VALUE IN REG
    unsigned int BASE = 0; //MUST OR WITH OXFFE0 + VALUE IN REG
    unsigned long TARGET = 0; //MUST OR WITH 0XFC00000 + VALUE
    unsigned long CODE = 0; //MUST OR WITH 0XFFF00000 + VALUE
    unsigned long IMMEDIATE = 0; //MUST OR WITH 0XFFFF0000
    uint16_t OFFSET = 0;
	
	char *TempOpCode = malloc(10 * sizeof(char));

	size_t buffer_size = 80;
    char *buffer = malloc(buffer_size * sizeof(char));
    char *TEMPbuffer = malloc(buffer_size * sizeof(char));

    size_t ARGUMENT_SIZE = 7;
    char *ARGUMENT1 = malloc(ARGUMENT_SIZE * sizeof(char));
    char *ARGUMENT2 = malloc(ARGUMENT_SIZE * sizeof(char));
    char *ARGUMENT3 = malloc(ARGUMENT_SIZE * sizeof(char));

    //uint32_t *Op_Code_Decoded = malloc(sizeof(uint32_t));
    uint32_t *Op_Code_Decoded;

    int i = 0;
    while(getline(&buffer, &buffer_size, fp) > 0){
    	i = 0;
    	TEMPbuffer = strtok(buffer, " ");
    	TempOpCode = TEMPbuffer;

    if (strcmp(TempOpCode, "add") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
    			RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("ADD RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000020 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "addu") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
    			RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("ADDU RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000021 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "addi") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("ADDI RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x10000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "addiu") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("ADDIU RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x24000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sub") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SUB RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000022 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "subu") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SUBU RS= %x RT= %x RD= %x\n", RS, RT, RD);
				fprintf(Output, "%08x\n", (0x00000023 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "mult") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				printf("MULT RS= %x RT= %x\n", RS, RT);
				fprintf(Output, "%08x\n", (0x00000018 | RS | RT));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "multu") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				printf("MULTU RS= %x RT= %x\n", RS, RT);
				fprintf(Output, "%08x\n", (0x00000019 | RS | RT));
    			TEMPbuffer = NULL;
    		}		
		}
	}
	else if (strcmp(TempOpCode, "div") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				printf("DIV RS= %x RT= %x\n", RS, RT);
				fprintf(Output, "%08x\n", (0x0000001A | RS | RT));
    			TEMPbuffer = NULL;
    		}		
		}
	}
	else if (strcmp(TempOpCode, "divu") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				printf("DIVU RS= %x RT= %x\n", RS, RT);
				fprintf(Output, "%08x\n", (0x0000001B | RS | RT));
    			TEMPbuffer = NULL;
    		}		
		}
	}
	else if (strcmp(TempOpCode, "and") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("AND RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000024 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "andi") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("ADDI RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x18000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "or") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("OR RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000025 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "ori") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("ORI RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x1A000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "xor") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("XOR RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000026 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "xori") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("XORI RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x1C000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "nor") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("NOR RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x00000027 | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "slt") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT3) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SLT RD= %x RS= %x RT= %x\n", RD, RS, RT);
				fprintf(Output, "%08x\n", (0x0000002A | RS | RT | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "slti") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				IMMEDIATE = (unsigned long)strtol(ARGUMENT3, NULL, 16);
				printf("SLTI RS= %x RT= %x IMMEDIATE= %lx\n", RS, RT, IMMEDIATE);
				fprintf(Output, "%08lx\n", (0x14000000 | RS | RT | IMMEDIATE));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sll") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				SA = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 6;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SLL RD= %x RT= %x SA= %x\n", RD, RT, SA);
				fprintf(Output, "%08x\n", (0x00000000 | RT | RD | SA));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "srl") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				SA = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 6;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SRL RD= %x RT= %x SA= %x\n", RD, RT, SA);
				fprintf(Output, "%08x\n", (0x00000002 | RT | RD | SA));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sra") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				SA = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 6;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("SRA RD= %x RT= %x SA= %x\n", RD, RT, SA);
				fprintf(Output, "%08x\n", (0x00000003 | RT | RD | SA));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "lw") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("LW BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0x8C000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "lb") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("LB BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0x80000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "lh") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("LH BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0x84000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "lui") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("LUI RT= %x OFFSET= %x\n", RT, OFFSET);
				fprintf(Output, "%08x\n", (0x1E000000 | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sw") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("SW BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0xAC000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sb") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("SB BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0xA0000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "sh") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				BASE = (unsigned int)strtol(ARGUMENT3, NULL, 16) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT1) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT2, NULL, 16);
				printf("SH BASE= %x RT= %x IMMEDIATE= %x\n", BASE, RT, OFFSET);
				fprintf(Output, "%08x\n", (0xA4000000 | BASE | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "mfhi") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("MFHI RD= %x\n", RD);
				fprintf(Output, "%08x\n", (0x00000010 | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "mflo") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
				printf("MFLO RD= %x\n", RD);
				fprintf(Output, "%08x\n", (0x00000012 | RD));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "mthi") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				printf("MTHI RS= %x\n", RS);
				fprintf(Output, "%08x\n", (0x00000011 | RS));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "mtlo") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				printf("MTLO RS= %x\n", RS);
				fprintf(Output, "%08x\n", (0x00000013 | RS));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "beq") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				OFFSET = GET_REGISTER_NUM(ARGUMENT3);
				printf("BEQ RS= %x\n", RS);
				fprintf(Output, "%08x\n", (0x00000011 | RS | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "bne") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
   			} else if (i == 2){
  				TEMPbuffer = strtok(NULL, " $(),\n\t");
   				ARGUMENT3 = TEMPbuffer;
    			printf ("%s ", ARGUMENT3);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RT = GET_REGISTER_NUM(ARGUMENT2) << 16;
				OFFSET = (uint16_t)strtol(ARGUMENT3, NULL, 16) >> 2;
				printf("BNE RS= %x RT= %x OFFSET= %x\n", RS, RT, OFFSET);
				fprintf(Output, "%08x\n", (0x1400c000 | RS | RT | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "blez") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				OFFSET = (*ARGUMENT2 & 0x0000FFFF);
				printf("BLEZ RS= %x OFFSET= %x\n", RS, OFFSET);
				fprintf(Output, "%08x\n", (0x18000000 | RS | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "bltz") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				OFFSET = (*ARGUMENT2 & 0x0000FFFF);
				printf("BLTZ RS= %x OFFSET= %x\n", RS, OFFSET);
				fprintf(Output, "%08x\n", (0x04000000 | RS | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "bgez") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				OFFSET = (*ARGUMENT2 & 0x0000FFFF);
				printf("BGEZ RS= %x OFFSET= %x\n", RS, OFFSET);
				fprintf(Output, "%08x\n", (0x04010000 | RS | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "bgtz") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				OFFSET = (*ARGUMENT2 & 0x0000FFFF);
				printf("BGTZ RS= %x OFFSET= %x\n", RS, OFFSET);
				fprintf(Output, "%08x\n", (0x1C000000 | RS | OFFSET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "j") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				TARGET = *ARGUMENT1;
				printf("J TARGET= %lx\n", TARGET);
				fprintf(Output, "%08lx\n", (0x08000000 | TARGET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "jr") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				printf("JR RS= %x\n", RS);
				fprintf(Output, "%08x\n", (0x00000008|RS)&(0x03E0003F)); //this and ensures the special and empty values stay 0
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "jal") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else {
				TARGET = (*ARGUMENT1 & 0x0000FFFF);
				printf("JAL TARGET= %lx\n", TARGET);
				fprintf(Output, "%08lx\n", (0x0c000000 | TARGET));
    			TEMPbuffer = NULL;
    		}		
  		}
	}
	else if (strcmp(TempOpCode, "jalr") == 0){
    	printf("%s ", TempOpCode);
    	while(TEMPbuffer != NULL){
  			if(i == 0){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT1 = TEMPbuffer;
    			printf ("%s ", ARGUMENT1);
    			i++;
    		} else if (i == 1){
    			TEMPbuffer = strtok(NULL, " $(),\n\t");
    			ARGUMENT2 = TEMPbuffer;
    			printf ("%s ", ARGUMENT2);
    			i++;
    		} else {
				RS = GET_REGISTER_NUM(ARGUMENT1) << 21;
				RD = GET_REGISTER_NUM(ARGUMENT2) << 11;
				if (RD == 0){
					printf("JALR RS= %x\n", RS);
					fprintf(Output, "%08x\n", (0x00000009 | RS));
				} else {
					RS = GET_REGISTER_NUM(ARGUMENT2) << 21;
					RD = GET_REGISTER_NUM(ARGUMENT1) << 11;
					printf("JALR RD= %x, RS= %x\n", RD, RS);
					fprintf(Output, "%08x\n", (0x00000009 | RS | RD));
				}
    			TEMPbuffer = NULL;
    		}
  		}
	}
	else if (strcmp(TempOpCode, "syscall") == 0){
		fprintf(Output, "%08x\n", (0x0000000C));
	}
	else {
		TEMPbuffer = NULL;
	}

	}

    fclose(fp);
    fclose(Output);
    free(buffer);
    return 0;
}

//FUNCTION TO RETURN REGISTER NUMBER
int GET_REGISTER_NUM(char* ARG){
	if (strcmp(ARG, "zero") == 0)
		return 0;
	else if (strcmp(ARG, "at") == 0)
		return 1;
	else if (strcmp(ARG, "v0") == 0)
		return 2;
	else if (strcmp(ARG, "v1") == 0)
		return 3;
	else if (strcmp(ARG, "a0") == 0)
		return 4;
	else if (strcmp(ARG, "a1") == 0)
		return 5;
	else if (strcmp(ARG, "a2") == 0)
		return 6;
	else if (strcmp(ARG, "a3") == 0)
		return 7;
	else if (strcmp(ARG, "t0") == 0)
		return 8;
	else if (strcmp(ARG, "t1") == 0)
		return 9;
	else if (strcmp(ARG, "t2") == 0)
		return 10;
	else if (strcmp(ARG, "t3") == 0)
		return 11;
	else if (strcmp(ARG, "t4") == 0)
		return 12;
	else if (strcmp(ARG, "t5") == 0)
		return 13;
	else if (strcmp(ARG, "t6") == 0)
		return 14;
	else if (strcmp(ARG, "t7") == 0)
		return 15;
	else if (strcmp(ARG, "s0") == 0)
		return 16;
	else if (strcmp(ARG, "s1") == 0)
		return 17;
	else if (strcmp(ARG, "s2") == 0)
		return 18;
	else if (strcmp(ARG, "s3") == 0)
		return 19;
	else if (strcmp(ARG, "s4") == 0)
		return 20;
	else if (strcmp(ARG, "s5") == 0)
		return 21;
	else if (strcmp(ARG, "s6") == 0)
		return 22;
	else if (strcmp(ARG, "s7") == 0)
		return 23;
	else if (strcmp(ARG, "t8") == 0)
		return 24;
	else if (strcmp(ARG, "t9") == 0)
		return 25;
	else if (strcmp(ARG, "k0") == 0)
		return 26;
	else if (strcmp(ARG, "k1") == 0)
		return 27;
	else if (strcmp(ARG, "gp") == 0)
		return 28;
	else if (strcmp(ARG, "sp") == 0)
		return 29;
	else if (strcmp(ARG, "fp") == 0)
		return 30;
	else if (strcmp(ARG, "ra") == 0)
		return 31;
	else
		return 40;
}

