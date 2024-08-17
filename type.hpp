#ifndef TYPE_HPP
#define TYPE_HPP
 #include "symbol_table.hpp"
 #include <iostream>
 #include "compiler_control.hpp"
 using namespace std;
class VM_command{
public:
   friend ostream &operator<<(ostream &output,VM_command &in){
     output <<"VMcommand==>  type:"  << in.get_type() << ", operation: " <<in.get_operation() << ",stack_operation:" << in.get_stackOperation() << ". stack_segment:" << in.get_stackSegment() << ",stack_value:" << in.get_indexValue() << endl;
    return output;
   }
    VM_TYPE get_type(){
        return this->type;
    }

    STACK get_stackOperation(){
        return this->stack_operation;
    }

    SEGMENT get_stackSegment(){
        return this->stack_segment;
    }

    OPERATION get_operation(){
        return this->operation;
    }

    void set_type(VM_TYPE type){
        this->type = type;
    }

    void set_stackOperation(STACK stoperation){
        this->stack_operation = stoperation;
    }
    void set_stackSegment(SEGMENT stSegment){
        this->stack_segment=stSegment;
    }

    void set_operation(OPERATION op){
        this->operation = op;
    }
    void set_indexValue(int value){
        this->index_value = value;
    }
    void set_indexName(string name){
        this->index_name = name;
    }
    int get_indexValue(){
        return this->index_value;
    }

    VM_command(){
        this->type = vmtype_None;
        this->stack_operation = stackop_None;
        this->stack_segment = stackseg_None;
        this->operation = op_None;
    }

    VM_command(VM_TYPE type){
        this->type = type;
        this->stack_operation = stackop_None;
        this->stack_segment = stackseg_None;
        this->operation = op_None;
    }
    
    private:
     VM_TYPE type;
     STACK stack_operation;
     SEGMENT stack_segment;
     OPERATION operation;
     string index_name;
     int index_value;

};

typedef struct{
    int index;
    VM_TYPE type;
    string keyword;
}ret_AlyWord;


// Asm
class ASM_command{

public:
    ASM_command(ASM_TYPE type,int A_value){
        this->type = type;
        this->A_value = A_value;
    }
    ASM_command(ASM_TYPE type,string text){
        this->type=type;
        if(type==lableCommand){
            this->A_label=text;
        }else if(type == gotoCommand){
            this->A_goto = text;
        }
    }

    ASM_command(ASM_TYPE type,C_Jump jmp,C_Dest dest,C_Comp cmp){
        this->type = type;
        this->jmp = jmp;
        this->dest = dest;
        this->cmp = cmp;
    }

    string to_string_asm(){
        if(this->type == Acommand){
            return "@"+to_string(this->A_value);
        }else if(this->type==gotoCommand){
            return "@"+this->A_goto;
        }else if(this->type == lableCommand){
            return "("+this->A_label+")";
        }else if(this->type == Ccommand){
            string result = "";
            switch (dest){
                case C_desA:
                    result+="A=";
                    break;
                case C_desAD:
                    result+="AD=";
                    break;
                case C_desADM:
                    result+="ADM=";
                    break;
                case C_desM:
                    result+="M=";
                    break;
                case C_desD:
                    result+="D=";
                    break;
                case C_desMD:
                    result+="MD=";
                    break;
                case C_desNone:
                    break;
                default:
                     System_ERROR("Genenrate ASMcommand error: Meet error command  type in DEST !");
                    break;
            }

            switch (cmp) {
                case C_zero:
                  result+="0";
                  break;
                case C_one:
                    result+="1";
                    break;
                case C_neg:
                    result+="-1";
                    break;
                case C_D:
                    result+="D";
                    break;
                case C_A:
                    result+="A";
                    break;
                case C_nD:
                    result+="!D";
                    break;
                case C_nA:
                    result+="!A";
                    break;
                case C_negA:
                    result+="-A";
                    break;
                case C_negD:
                    result+="-D";
                    break;
                case C_negM:
                    result+="-M";
                    break;
                case C_Aadd1:
                    result+="A+1";
                    break;
                case C_Dadd1:
                    result+="D+1";
                    break;
                case C_Dsub1:
                    result+="D-1";
                    break;
                case C_Asub1:
                    result+="A-1";
                    break;
                case C_DaddA:
                    result+="D+A";
                    break;
                case C_AsubD:
                    result+="A-D";
                    break;
                case C_DandA:
                    result+="D&A";
                    break;
                case C_DorA:
                    result+="D|A";
                    break;
                case C_M:
                    result+="M";
                    break;
                case C_nM:
                    result+="!M";
                    break;
                case C_Madd1:
                    result+="M+1";
                    break;
                case C_Msub1:
                    result+="M-1";
                    break;
                case C_DandM:
                    result+="D&M";
                    break;
                case C_MsubD:
                    result+="M-D";
                    break;
                case C_DaddM:
                    result+="D+M";
                    break;
                case C_DorM:
                    result+="D|M";
                    break;
                default:
                    System_ERROR("Generate ASMcommand error in cmp!");
                    break;
            }

            if(jmp!=C_NoJum && dest!=C_desNone) System_ERROR("Generate ASM command error in Jmp command");
            if(jmp!=C_NoJum){
                result+=";";
                switch (jmp){
                    case  C_JGT:
                        result+="JGT";
                        break;
                    case C_JEQ:
                        result+="JEQ";
                        break;
                    case C_JGE:
                        result+="JGE";
                        break;
                    case C_JLE:
                        result+="JLE";
                        break;
                    case C_JLT:
                        result+="JLT";
                        break;
                    case C_JNE:
                        result+="JNE";
                        break;
                    case C_JMP:
                        result+="JMP";
                        break;
                }
            }

            return result;

        }else{
            System_ERROR("Generate ASMcommand error: Meet error command  type!");
        }

    
    }

 private:
    ASM_TYPE type;
    C_Jump jmp;
    C_Dest dest;
    C_Comp cmp;
    int A_value;
    string A_label;
    string A_goto;
};


#endif
