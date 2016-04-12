#include "spimcore.h"
unsigned grabOnly(unsigned instruction, int begIndex, int endidx);
//WILL
/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    if (ALUControl == 0b000) // add
        *ALUresult = A + B;
    else if (ALUControl ==  0b001) // sub
        *ALUresult = A - B;
    else if (ALUControl == 0b010) // slt (signed)
        *ALUresult = (unsigned) ((int)A < (int)B);
    else if (ALUControl == 0b011) // sltu (unsigned)
        *ALUresult = (unsigned) (A < B);
    else if (ALUControl == 0b100) // and
        *ALUresult = A & B;
    else if (ALUControl == 0b101) // or
        *ALUresult = A | B;
    else if (ALUControl == 0b110) // 16-bit shift B
        *ALUresult = B << 16;
    else if(ALUControl == 0b111) // NOT A
        *ALUresult = ~A;

    *Zero = *ALUresult == 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(PC%4 != 0 || PC < 0x4000 || PC > 0xFFFF){
        return 1;
    }
    *instruction = Mem[PC];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
/*
 * unsigned op,	// instruction [31-26]
	r1,	// instruction [25-21]
	r2,	// instruction [20-16]
	r3,	// instruction [15-11]
	funct,	// instruction [5-0]
	offset,	// instruction [15-0]
	jsec;	// instruction [25-0]
 */
    *op = grabOnly(instruction,26,31);
    *r1 = grabOnly(instruction,21,25);
    *r2 = grabOnly(instruction,16,20);
    *r3 = grabOnly(instruction,11,15);
    *funct = grabOnly(instruction,0,5);
    *offset = grabOnly(instruction, 0,15);
    *jsec = grabOnly(instruction,0,25);

}

unsigned grabOnly(unsigned instruction, int begIndex, int endidx){
    unsigned bitmask = 0;
    int i;
    for(i = begIndex; i<=endidx;i++){
        bitmask = bitmask | (1<<i);
    }
    instruction = instruction & bitmask;
    instruction = instruction>>begIndex;
    return instruction;
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
/*
 * typedef struct
{
	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemtoReg;
	char ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;
}struct_controls;
 *//*
    For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0 means that disabled, 2 means “don’t care”.
    For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the selected path of the multiplexer; 2 means “don’t care”.
    The following table shows the meaning of the values of ALUOp.

     */
    controls->MemRead = '2';
    controls->MemWrite = '2';
    controls->RegWrite = '2';
    controls->RegDst = '2';
    controls->Jump = '2';
    controls->Branch = '2';
    controls->MemtoReg = '2';
    controls->ALUSrc = '2';
    controls->ALUOp = (char)0b000;
    unsigned first = grabOnly(op,0,2);
    unsigned last = grabOnly(op,3,5);
    //add
    if(op==100000){

    }
    //sub
    //addi
    //and
    //or
    //lw
    //sw
    //lui
    //beq
    //slt
    //slti
    //sltu
    //sltiu
    //j
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1]; // r1 -> data1
    *data2 = Reg[r2]; // r2 -> data2
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    *extended_value = *extended_value & 0x0000FFFF; // Only use the lower half of the value
    int sign = *extended_value >> 15;   // grab just the 16th bit
    if (sign == 1)                      // If signed,.
        *extended_value += 0xFFFF0000;  // extend with 1's
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

