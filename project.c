#include "spimcore.h"


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

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

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
    if (sign == 1)                      // If signed
        *extended_value += 0xFFFF0000;  // extend with 1's
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    char ALUControl = -1;
    if (ALUOp != 0b111) {
        ALUControl = ALUOp;
    } else {
        switch (funct) {
            case 0b0000:
                ALUControl = 0b0010;
                break;

            case 0b0010:
                ALUControl = 0b0110;
                break;

            case 0b0100:
                ALUControl = 0b0000;
                break;

            case 0b1010:
                ALUControl = 0b0111;
                break;

            default:
                return 1;
        }
    }


    if (ALUSrc) ALU(data1, extended_value, ALUControl, ALUresult, Zero);
    else ALU(data1, data2, ALUControl, ALUresult, Zero);

    return 0;
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

