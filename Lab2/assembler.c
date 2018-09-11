#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void OutputGenerate(uint32_t LINE);
void GetOpCode(char*);

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
	char *TempOpCode = malloc(OpCode_size * sizeof(char));
    	size_t buffer_size = 30;
    	char *buffer = malloc(buffer_size * sizeof(char));
	
    	while(-1 != getline(&buffer, &buffer_size, fp))
    	{
        	printf("%s", buffer);
		int i = 0;
		while (buffer[i] != ' '){
 			TempOpCode[i] = buffer[i];
			i++;
		}
		GetOpCode(TempOpCode);
		// NEED TO CLEAR TEMPOPCODE
	}


    	fclose(fp);
    	free(buffer);
	free(TempOpCode);
    	return 0;
}

void GetOpCode(char* OpCode){
	printf("%s\n", OpCode);
	//DECODE OPCODE

}

void OutputGenerate(uint32_t LINE){
	FILE * out;
	out = fopen("MachineCode.in", "a");
	if (out == NULL){
		printf("Error: Can't create output file\n");
		exit(-1);
	}
	fprintf(out, "%x\n", LINE);
   
fclose(out);
}

