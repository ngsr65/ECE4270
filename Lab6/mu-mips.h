#include <stdint.h>

#define FALSE 0
#define TRUE  1

/******************************************************************************/
/* MIPS memory layout                                                                                                                                      */
/******************************************************************************/
#define MEM_TEXT_BEGIN  0x00400000
#define MEM_TEXT_END      0x0FFFFFFF
/*Memory address 0x10000000 to 0x1000FFFF access by $gp*/
#define MEM_DATA_BEGIN  0x10010000
#define MEM_DATA_END   0x7FFFFFFF

#define MEM_KTEXT_BEGIN 0x80000000
#define MEM_KTEXT_END  0x8FFFFFFF

#define MEM_KDATA_BEGIN 0x90000000
#define MEM_KDATA_END  0xFFFEFFFF

/*stack and data segments occupy the same memory space. Stack grows backward (from higher address to lower address) */
#define MEM_STACK_BEGIN 0x7FFFFFFF
#define MEM_STACK_END  0x10010000

typedef struct {
	uint32_t begin, end;
	uint8_t *mem;
} mem_region_t;

/* memory will be dynamically allocated at initialization */
mem_region_t MEM_REGIONS[] = {
	{ MEM_TEXT_BEGIN, MEM_TEXT_END, NULL },
	{ MEM_DATA_BEGIN, MEM_DATA_END, NULL },
	{ MEM_KDATA_BEGIN, MEM_KDATA_END, NULL },
	{ MEM_KTEXT_BEGIN, MEM_KTEXT_END, NULL }
};

#define NUM_MEM_REGION 4
#define MIPS_REGS 32

//Pipeline flags, when set to one this pipeline should stall 
uint8_t IF_stall = 0, ID_stall = 0, EX_stall = 0, MEM_stall = 0, WB_stall = 0;

//Register Writing flags, if the flag is one rd will be used 
uint8_t EX_MEM_regWrite = 0, MEM_WB_regWrite = 0;

//Forwarding flags and values
int ENABLE_FORWARDING = FALSE, ForwardA, ForwardB;

//Cache flag
uint8_t CacheStall = 0;
uint8_t UseCache = 0;

int SKIPID = 0;

typedef struct CPU_State_Struct {

  uint32_t PC;		                   /* program counter */
  uint32_t REGS[MIPS_REGS]; /* register file. */
  uint32_t HI, LO;                          /* special regs for mult/div. */
} CPU_State;

typedef struct CPU_Pipeline_Reg_Struct{
	uint32_t PC;
	uint32_t IR;
	uint32_t A;
	uint32_t B;
	uint32_t imm;
	uint32_t ALUOutput;
	uint32_t LMD;
	uint32_t regWrite;
} CPU_Pipeline_Reg;

/***************************************************************/
/* CPU State info.                                                                                                               */
/***************************************************************/

CPU_State CURRENT_STATE, NEXT_STATE;
int RUN_FLAG;	/* run flag*/
uint32_t INSTRUCTION_COUNT;
uint32_t CYCLE_COUNT;
uint32_t PROGRAM_SIZE; /*in words*/


/***************************************************************/
/* Pipeline Registers.                                                                                                        */
/***************************************************************/
CPU_Pipeline_Reg ID_IF;
CPU_Pipeline_Reg ID_EX;
CPU_Pipeline_Reg EX_MEM;
CPU_Pipeline_Reg MEM_WB;

char prog_file[32];


/***************************************************************/
/* Function Declerations.                                                                                                */
/***************************************************************/
void help();
uint32_t mem_read_32(uint32_t address);
void mem_write_32(uint32_t address, uint32_t value);
void cycle();
void run(int num_cycles);
void runAll();
void mdump(uint32_t start, uint32_t stop) ;
void rdump();
void handle_command();
void reset();
void init_memory();
void cacheWrite(uint32_t, uint32_t);
uint32_t cacheRead(uint32_t);
void load_program();
void handle_pipeline(); /*IMPLEMENT THIS*/
void WB();/*IMPLEMENT THIS*/
void MEM();/*IMPLEMENT THIS*/
void EX();/*IMPLEMENT THIS*/
void ID();/*IMPLEMENT THIS*/
void IF();/*IMPLEMENT THIS*/
void show_pipeline();/*IMPLEMENT THIS*/
void initialize();
void print_program(); /*IMPLEMENT THIS*/
int checkOverflow(uint32_t num1, uint32_t num2);
