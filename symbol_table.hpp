#ifndef SYMBOL_TABLE_HPP
 #define SYMOBOL_TABLE_HPP

 typedef enum segment{
stackseg_None = 0,
 Local,
 Argument,
 This,
 That,
 Constant,
 Static,
 Temp, 
 Pointer}SEGMENT;
  
  typedef enum vm_type {
    vmtype_None = 0,
    Illegal_Character,
    Str,
    Index,
    Index_value,
    Operation,
    Stack,
    Stack_Segment,
    END
  }VM_TYPE;

  typedef enum operation{
   op_None=0,
   Add,
   Sub,
   Neg,
   Eq,
   Gt,
   Lt,
   And,
   Or,
   Not
  }OPERATION;

 typedef enum stack{
    stackop_None=0,
    Pop,
    Push
  }STACK;


typedef enum{
Acommand=0,
Ccommand,
lableCommand,
gotoCommand
}ASM_TYPE;

typedef enum{
 C_NoJum=0,
 C_JGT,
 C_JEQ,
 C_JGE,
 C_JLE,
 C_JLT,
 C_JNE,
 C_JMP
}C_Jump;

typedef enum{
 C_desNone=0,
 C_desM,
 C_desD,
 C_desMD,
 C_desA,
 C_AM,
 C_desAD,
 C_desADM
}C_Dest;

typedef enum{
C_zero,
C_one,
C_neg,
C_D,
C_A,
C_nD,
C_nA,
C_negD,
C_negA,
C_Dadd1,
C_Aadd1,
C_Dsub1,
C_Asub1,
C_DaddA,
C_DaddM,
C_DsubA,
C_AsubD,
C_DandA,
C_DorA,
C_M,
C_nM,
C_negM,
C_Madd1,
C_Msub1,
C_DandM,
C_MsubD,
C_DorM

}C_Comp;
#endif