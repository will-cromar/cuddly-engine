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
    *instruction = Mem[PC>>2];
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

// Returns the bits of intstruction from begIndex to endidx
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
    //Assume R type
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->RegWrite = 1;
    controls->RegDst = 1;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemtoReg = 0;
    controls->ALUSrc = 0;
    controls->ALUOp = (char)0b111;
    if(op == 0b000000){
        return 0;
    }
    //addi
    if(op == 0b001000){
        controls->ALUOp = 0b000;
        controls->RegDst = 0;
        controls->ALUSrc = 1;
        return 0;
    }
    //lw
    if(op == 0b100011){
        controls->MemRead = 1;
        controls->MemWrite = 0;
        controls->RegWrite = 1;
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemtoReg = 1;
        controls->ALUSrc = 1;
        controls->ALUOp = (char)0b000;
        return 0;
    }
    //sw
    if(op == 0b101011){
        controls->MemRead = 0;
        controls->MemWrite = 1;
        controls->RegWrite = 0;
        controls->RegDst = 0b10;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemtoReg = 0b10;
        controls->ALUSrc = 1;
        controls->ALUOp = (char)0b000;
        return 0;
    }
    //lui
    if(op == 0b001111){
        controls->ALUSrc = 1;
        controls->RegDst = 0;
        controls->ALUOp = (char)0b110;
        return 0;
    }
    //beq
    if(op == 0b000100){
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegWrite = 0;
        controls->RegDst = 0b10;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemtoReg = 0b10;
        controls->ALUSrc = 0;
        controls->ALUOp = (char)0b001;
        return 0;
    }
    //slti
    if(op == 0b001010) {
        controls->ALUSrc = 0;
        controls->RegDst = 0;
        controls->ALUOp = (char) 0b001;
        return 0;
    }
    //sltiu
    if(op == 0b001001){
        controls->ALUSrc = 0;
        controls->RegDst = 0;
        controls->ALUOp = (char)0b011;
        return 0;
    }
    //j
    if(op == 0b000010){
        controls->MemRead = 0b10;
        controls->MemWrite = 0b10;
        controls->RegWrite = 0b10;
        controls->RegDst = 0b10;
        controls->Jump = 1;
        controls->Branch = 0b10;
        controls->MemtoReg = 0b10;
        controls->ALUSrc = 1;
        controls->ALUOp = (char)0b000;
        return 0;
    }

    return 1;
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
    *extended_value = offset & 0x0000FFFF; // Only use the lower half of the value
    int sign = *extended_value >> 15;   // grab just the 16th bit
    if (sign == 1)                      // If signed,
        *extended_value += 0xFFFF0000;  // extend with 1's
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    char ALUControl;
    if (ALUOp != 0b111) {
        ALUControl = ALUOp; // If not an R-type, use ALUOp
    } else {
        // Derive operation from funct
        funct = funct & 0b111111;
        if (funct == 0b100000) ALUControl = 0b000; // add
        else if (funct == 0b100010) ALUControl = 0b001; // sub
        else if (funct == 0b100100) ALUControl = 0b100; // and
        else if (funct == 0b100101) ALUControl = 0b101; // or
        else if (funct == 0b101010) ALUControl = 0b010; // slt
        else if (funct == 0b101011) ALUControl = 0b011; // sltu
        else return 1; // Halt if funct is invalid
    }

    // Decide whether to use extended value or data2
    if (ALUSrc)
        ALU(data1, extended_value, ALUControl, ALUresult, Zero);
    else
        ALU(data1, data2, ALUControl, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // If both are asserted or both are de-asserted, don't do anything
    if (MemWrite == MemRead) {
        return 0;
    }
    // Check that ALUresult is a proper memory index (Halt condition).
    if(ALUresult % 4 != 0 || ALUresult < 0x4000 || ALUresult > 0xFFFF)
        return 1;

    // If 'MemWrite' is asserted, write data from register to 'Mem' (location: 'ALUresult').
    else if (MemWrite) {
        Mem[ALUresult >> 2] = data2;
    }
    // If 'MemRead' is asserted, read data from memory into 'memdata' (location: ALUresult).
    else if (MemRead) {
        *memdata = Mem[ALUresult >> 2];
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    unsigned dataToWrite = MemtoReg == 1 ? memdata : ALUresult;
    unsigned destination = RegDst ==  1 ? r3 : r2;

    // Don't write to register $zero
    if (RegWrite && destination != 0)
        Reg[destination] = dataToWrite;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // Add 4 bytes to program counter to isolate next instruction.
    *PC += 4;

    // If 'Jump' control signal is asserted, execute jump datapath.
    if (Jump) {
        // The new location of 'PC' is the jump address, indicated by 'jsec', shifted by two.
        *PC = jsec << 2;
        return;
    }
    // If 'Jump' is not asserted, we need to look at 'Branch' and 'Zero.' These will tell us if we Branch on Equal.
    else if (Branch && Zero) {
            // Take the sign extension, bitshift it left by 2, then add it to the PC value.
            extended_value = extended_value << 2;
            *PC += extended_value;
    }

    return;
}

