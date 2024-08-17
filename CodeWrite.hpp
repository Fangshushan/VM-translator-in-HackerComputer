#ifndef CODE_WRITE_HPP
    #define CODE_WRITE_HPP
    #include <queue>
    #include <iostream>
    #include "type.hpp"
    #include <cstring>
    #include "compiler_control.hpp"
    using namespace std;

    class GenAsm{

        public:

        GenAsm(queue<VM_command*> command){
            vm_code = command;
            // stard mapping
            sp_address = 0;
            sp_value=256;
            lcl_address = 1;
            lcl_value = 0;
            
            arg_address = 2;
            arg_value = 0;

            this_base_address = 3;
            this_value = 0;

            that_base_address = 4;
            that_value = 0;

            temp_base_address = 5; // seven 
            static_base_address = 16;

            label = 0;
            ret_label = 0;

            
            init_command();   

        }
// 
        void set_arg_value(int value){
            this->arg_value = value;
        }
        void set_lcl_value(int value){
            this->lcl_value = value;
        }
        void set_this_value(int value){
            this->this_value = value;
        }
        void set_that_value(int value){
            this->that_value = value;
        }

        
        void Generate_assembly(){
            if(this->vm_code.empty()){
                System_ERROR("vm code parser  of result is null in Generate assmbly file");
            }
            VM_command *vm =NULL; 
            while (!vm_code.empty()){
                vm = vm_code.front();
                switch (vm->get_type()){
                    case Stack:
                    //********************************
                        switch (vm->get_stackOperation()){
                            case Push:
                                this->push(vm->get_stackSegment(),vm->get_indexValue());
                                break;
                            case Pop:
                                this->pop(vm->get_stackSegment(),vm->get_indexValue());
                                break;
                            default:
                                break;
                        }
                    //********************************
                        break;
                    case Operation:
                    //********************************
                        this->operation(vm->get_operation());
                    //**********************************
                        break;
                    default:
                        break;
                }

                delete vm;
                vm_code.pop();
            }
            

            
        }

        void Generate_asm_file(string outfile){
            if(outfile.length() == 0){
                System_ERROR("Generate assmbly file,out file address is empty!");
            } 
            ofstream out_file;
            out_file.open(outfile);
            if(asm_code.empty()){
                cout << "[Out asm file] asmcode Queue is null!" << endl;
            }else{
                cout << "[Out asm file] asm len:" << asm_code.size() << endl;

                while (!asm_code.empty())  {
                        cout << asm_code.front()->to_string_asm() << endl;
                        out_file<<asm_code.front()->to_string_asm() << endl;
                        delete asm_code.front();
                        asm_code.pop();
                }
            }
        }

        void debug_AsmcodeQueue(){
            if(asm_code.empty()){
                cout << "[CodeWrite] asmcode Queue is null!" << endl;
            }else{
                cout << "[CodeWrite] asm len:" << asm_code.size() << endl;
                while (!asm_code.empty())  {
                        cout << asm_code.front()->to_string_asm() << endl;
                        delete asm_code.front();
                        asm_code.pop();
                }
            }
        }

        private:
        queue<VM_command*> vm_code;
        queue <ASM_command* >  asm_code;

        int label;
        int ret_label;
        // 标准映射
        int sp_address;
        int sp_value; // Stack pointer

        int lcl_address;
        int lcl_value; // Local pointer

        int arg_address;
        int arg_value; // Argument pointer

        int this_base_address;
        int this_value;

        int that_base_address;
        int that_value;


        int temp_base_address; // 基址
        int static_base_address;



        ASM_command* ASMtemp  = NULL;

        void push(SEGMENT seg,int index){

            switch (seg)
            {
            case Local:
                if(lcl_value!=0){
                 push_op(lcl_address,index);
                }else{
                    Compiler_Warning("(Push)The current Local segment is not initialized address!");
                }
                break;
            case Constant:
                push_op(0,index,1);
                break;
            case Argument:
                if(arg_value!=0){
                 push_op(arg_address,index);
                }else{
                    Compiler_Warning("(Push)The current Argument segment is not initialized address!");
                }
                break;
            case Temp:
                if(index <=7){
                    push_op(temp_base_address,index);
                }else{
                    Compiler_Warning("(Push)that is visit value > 7 in PUSH command ");
                }
            case Static:
                push_op(static_base_address,index);
                break;
            case Pointer:
                break;
            default:
                break;
            }

        }

        void pop(SEGMENT seg,int index){

            switch (seg)
            {
            case Local:
                if(lcl_value!=0){
                 pop_op(lcl_address,index);
                }else{
                    Compiler_Warning("(Pop)The current Local segment is not initialized address!");
                }
                break;
            case Constant:
                break;
            case Argument:
                if(arg_value!=0){
                 pop_op(arg_address,index);
                }else{
                    Compiler_Warning("(Pop)The current Argument segment is not initialized address!");
                }
                break;
            case Temp:
                if(index <=7){
                    pop_op(temp_base_address,index);
                }else{
                    Compiler_Warning("(Pop)that is visit value > 7 in PUSH command ");
                }
            case Static:
                pop_op(static_base_address,index);
                break;
            case Pointer:
                break;
            default:
                break;
            }
        }

        // real push operation ,argument (base address, value)
        void push_op(int base_address,int value,char isconst = 0){
            if(isconst){
                A_command(value);
                C_command(C_NoJum,C_desD,C_A);
            }else{
                A_command(base_address);
                C_command(C_NoJum,C_desD,C_A); //D=A
                A_command(value);
                C_command(C_NoJum,C_desA,C_DaddA);
                C_command(C_NoJum,C_desD,C_M);
                // D = M
            }

            A_command(sp_address);
            C_command(C_NoJum,C_desA,C_M);// A=M
            C_command(C_NoJum,C_desM,C_D); //M=D
            // SP++
            A_command(sp_address);
            C_command(C_NoJum,C_desM,C_Madd1);
        }


        // real push operation ,argument (base address, index)
        void pop_op(int base_address,int index){
            // step get value
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Msub1);
            C_command(C_NoJum,C_desA,C_M);
            C_command(C_NoJum,C_desD,C_M); //D = value
            // set address 
            A_command(base_address+index);
            C_command(C_NoJum,C_desM,C_D);
        
        }



        void operation(OPERATION op){
            switch (op)
            {
            case Add:
                add_op();
                break;
            case Sub:
                sub_op();
                break;
            case Neg:
                neg_op();
                break;
            case Eq:
                logic_op(Eq);
                break;
            case Gt:
                logic_op(Gt);
                break;
            case Lt:
                logic_op(Lt);
                break;
            case And:
                and_op();
                break;
            case Or:
                or_op();
                break;
            case Not:
                not_op();
                break;

            default:
                break;
            }

        }
        void neg_op(){
            A_command(sp_address);
            C_command(C_NoJum,C_desM,C_Msub1);
            C_command(C_NoJum,C_desA,C_M);
            
            C_command(C_NoJum,C_desM,C_negM);

            A_command(sp_address);
            C_command(C_NoJum,C_desM,C_Madd1);


        }
        void not_op(){
           A_command(sp_address);
            C_command(C_NoJum,C_desM,C_Msub1);
            C_command(C_NoJum,C_desA,C_M);
            
            C_command(C_NoJum,C_desM,C_nM);

            A_command(sp_address);
            C_command(C_NoJum,C_desM,C_Madd1);
        }
        void op_double(){
            // step get value
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Msub1);
            C_command(C_NoJum,C_desA,C_M);
            
            C_command(C_NoJum,C_desD,C_M); //D = value
            
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Msub1);
            C_command(C_NoJum,C_desA,C_M);
        }
        void add_op(){
            op_double();     
            C_command(C_NoJum,C_desM,C_DaddM); // M 
            
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Madd1);
        }
        void and_op(){
            op_double();
            C_command(C_NoJum,C_desM,C_DandM); // M
            
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Madd1);

        }
        void sub_op(){
            op_double();
            C_command(C_NoJum,C_desM,C_MsubD);

            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Madd1);
        }
        void or_op(){
            op_double();
            C_command(C_NoJum,C_desM,C_DorM); // M
            
            A_command(sp_address); // 取出sp
            C_command(C_NoJum,C_desM,C_Madd1);

        }

        string logic_op_double(){
            op_double(); 
            label++;
            return "label"+to_string(label);
        }

        void logic_set_one(string lab,string ret){
            Lable_command(lab);
            push(Constant,1);
            Lable_command(ret);    
        }

        void logic_op(OPERATION op){
        // stack_location M  > D
            string labels = logic_op_double();
            string ret_labels = "ret"+to_string(ret_label++);
            C_command(C_NoJum,C_desD,C_MsubD);
            Goto_command(labels);
            switch (op){
                case Eq:
                C_command(C_JEQ,C_desNone,C_D);
                    break;
                case Gt:
                C_command(C_JGT,C_desNone,C_D);
                    break;
                case Lt:
                C_command(C_JLT,C_desNone,C_D);
                    break;
                default:
                    break;
            }
            push(Constant,0);
            Goto_command(ret_labels);
            C_command(C_JMP,C_desNone,C_zero);
            logic_set_one(labels,ret_labels);

        }
        void gt_op(){
            op_double();
        
        }

        void lt_op(){
            op_double();


        }

        void init_command(){
            // init memeory segment mapping
            set_segment_address(sp_address,sp_value);
            set_segment_address(lcl_address,lcl_value);
            set_segment_address(arg_address,arg_value);
            set_segment_address(this_base_address,this_value);
            set_segment_address(that_base_address,that_value);
        }
    
        void set_segment_address(int address,int value){
            A_command(value);
            C_command(C_NoJum,C_desD,C_A);
            A_command(address);
            C_command(C_NoJum,C_desM,C_D);
        }

        void A_command(int value){
            if(value < 0 || ASMtemp!=NULL){
                cout <<"[A command]: Gen fail!" <<endl;
            }else{
                ASMtemp = new ASM_command(Acommand,value);
                asm_code.push(ASMtemp);
                ASMtemp =NULL;
            }
        }

        void C_command(C_Jump jmp,C_Dest des,C_Comp cmp){
            if( ASMtemp!=NULL){
                cout <<"[C command]: Gen fail!" <<endl;
            }else{
                ASMtemp = new ASM_command(Ccommand,jmp,des,cmp);
                asm_code.push(ASMtemp);
                ASMtemp =NULL;
            }
        }

        void Goto_command(string text){
            if(text.length() == 0){
                cout <<"[Goto command]: Gen fail!" <<endl;
            }else{
                ASMtemp = new ASM_command(gotoCommand,text);
                asm_code.push(ASMtemp);
                ASMtemp =NULL;
            }
        }

        void Lable_command(string name){
            if(name.length() == 0){
                cout <<"[label command]: Gen fail!" <<endl;
            }else{
                ASMtemp = new ASM_command(lableCommand,name);
                asm_code.push(ASMtemp);
                ASMtemp =NULL;
            }
        }
    

    };

#endif
