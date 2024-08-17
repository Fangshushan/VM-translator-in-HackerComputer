#ifndef PARSER_HPP
    #define PARSER_HPP
    // #define CONFIG
    #include <iostream>
    #include <cstring>
    #include <fstream>
    #include <queue>
    #include "config.hpp"
    #include "type.hpp"
    #include "compiler_control.hpp"

   using namespace std;

   typedef struct node{
       
       node* left;
       node* rigth;
       VM_TYPE type;
   }Abstract_Tree ;




    class Parser{
     public:
        void Vm_to_Asm(){
            if (this->vm_file_address.length() == 0 ){
                General_ERROR("Not find VM code file !! Please close program, again ~");
            }
            std::ifstream infile(vm_file_address.c_str());
            if (!infile){
                General_ERROR("Failed to open "+vm_file_address+" file!");
            }
            
            std::string line;
            int line_number = 0;

            while (getline(infile,line)){
               #ifdef PARSER_DEBUG
               cout <<"[Parser module DEBUG<read a line>]: " << " context: " <<  line  << " len: " << line.length() << endl;
               #endif
                line_number++;
               if(strcmp(line.c_str(),"\n")==0 && line.length() == 1) continue;
               analyse_statement(line,line_number);
            }
            
            infile.close();
            return ;
        }

        void debug_commandQueue(){
            // this is debug function
            cout << this->VMcommand_Queue.size() << endl;
            if(this->VMcommand_Queue.empty()){
                cout << "[VMcommand Queue] : Queue is empty!" << endl;
            }else{
                int i = 1;
                while (!this->VMcommand_Queue.empty()) {
                    cout << "[" << i << "] " <<*(this->VMcommand_Queue.front())  << endl;
                    delete (this->VMcommand_Queue.front());
                    this->VMcommand_Queue.pop();
                    i++;
                }
                
            }
        }


        Parser(std:: string file_address){
             vm_file_address = file_address;
        }
        Parser(){
        }
        void set_file(string file_address){
            this->vm_file_address= file_address;
        }

        queue<VM_command*> get_ParserResult(){
            return VMcommand_Queue;
        }

        private:
         queue<VM_command* > VMcommand_Queue; 
         
          ret_AlyWord* analyse_word(string line,int index){
            // This method is only for temporary use and has not been optimized. 
            // It will be waited for optimized.

            #ifdef PARSER_DEBUG
             cout << "[PARSER DEBUG <anayse_word>]: line: " << line <<"  index:" << index << endl;
            #endif 
                ret_AlyWord *ret =new ret_AlyWord ;
                ret->keyword = "";
                ret->type = vmtype_None;

           
                line= line+ "'#"; // the end symbol
                int length = line.length(); // don't need peek character
                // char peek = -1; // look up next char !
                #define OUTCOME_ADD ret->keyword+=line[idx]
            //  record state?

                struct{
                    char frist_type; 
                    // if word then word+number
                    // if number then number .....

                    // WORD is zero
                    // NUMBER is one
                    // OTHER is neg
                }word_type; // temp variable


                int idx = index;
                for(idx; idx < length; idx++){
                    if(idx == index){
                        if(line[idx] >='a' && line[idx] <= 'z'  || line[idx]>='A' && line[idx] <='Z'){
                            word_type.frist_type = 0;
                            OUTCOME_ADD;
                        }else if(line[idx]>='0' && line[idx] <='9'){
                            word_type.frist_type = 1;
                            OUTCOME_ADD;
                        }else{
                            ret->type = Illegal_Character;
                            ret->index = idx;
                            OUTCOME_ADD;
                            
                            return ret;
                        }
                    }else{
                        if(line[idx]>='a' && line[idx] <='z' || line[idx]>='A' && line[idx]<='Z'){
                            if(word_type.frist_type == 0){
                                OUTCOME_ADD;
                            }else{
                                // number type exit!! ^ v ^
                                ret->type = Index_value;// Value index
                                break; // It is recognized that the character does not match
                            }
                        }else if(line[idx]>='0' && line[idx]<='9'){
                            OUTCOME_ADD;
                        }else{
                            // number Exit;
                            if(word_type.frist_type == 0){
                                ret->type = Str;
                            }else if(word_type.frist_type == 1){
                                ret->type=Index_value;
                            }
                            break;
                        }
                    }


                }// loop end
                ret->index = idx;
                #ifdef PARSER_DEBUG
                 cout << "[Parser module DEBUG<return anaslyse_word>]: " <<  "index:" << ret->index << " keyword:" << ret->keyword << " VM_type:" << ret->type << endl; 
                #endif
                return ret;

           }

           void analyse_statement(string line,int line_number){
                #define SKIPS while((line[idx] == ' ' || line[idx] == '\t')&& idx < line.size()) idx++;
                VM_command *new_command=NULL;
                for(int idx = 0; idx < line.length();idx++){
                     if(line[idx] == ' ' || line[idx] == '\t' )continue;
                     ret_AlyWord *ret = analyse_word(line,idx);
                    idx = ret->index;
                    // ========================================================
                        SKIPS;
                        // if(idx >=line.length() && ret!=NULL) break;
                    // =========================================================
                    switch (ret->type){
                    case Illegal_Character:
                          Compiler_ERROR("We are meet  Illegal character(\'"+ret->keyword +"\') "+"in VM code file "+to_string(line_number)+"  line");
                        break;
                    case Str:
                    //============================================
                      #define Str_cmp(x) strcmp(ret->keyword.c_str(),x)==0
                      #define Operation_Expression   if(idx !=  line.length()) Compiler_ERROR("Error operation expression ( "+ line+" ) in VM code file "+to_string(line_number) + " line");
                      #define Set_Command(type,op) \
                                    if(new_command!=NULL) General_ERROR("New command memoery is not null!  " ); \
                                    new_command = new VM_command(); \
                                    new_command->set_type(type); \
                                    new_command->set_operation(op);
                      #define Push_Opcommand  \
                                  VMcommand_Queue.push(new_command);\
                                  new_command = NULL; \
                                  delete ret; // release result memory 
                    //============================================

                    #ifdef PARSER_DEBUG
                       cout << "[parser debug]:" <<"<" << line_number << ">" << "find str" << endl;
                    #endif
                      if(Str_cmp("sub")){
                        Operation_Expression;
                        Set_Command(Operation,Sub);
                        Push_Opcommand;
                       }else if(Str_cmp("add")){
                        Operation_Expression;
                        Set_Command(Operation,Add);
                        Push_Opcommand;
                       
                       }else if(Str_cmp("eq")){
                        Operation_Expression;
                        Set_Command(Operation,Eq);
                        Push_Opcommand;
                       }else if (Str_cmp("gt")){
                        Operation_Expression;
                        Set_Command(Operation,Gt);
                        Push_Opcommand;
                       }else if(Str_cmp("lt")){
                        Operation_Expression;
                        Set_Command(Operation,Lt);
                        Push_Opcommand;
                       }else if(Str_cmp("and")){
                        Operation_Expression;
                        Set_Command(Operation,And);
                        Push_Opcommand;
                       }else if(Str_cmp("or")){
                         Operation_Expression;
                        Set_Command(Operation,Or);
                        Push_Opcommand;
                       }else if(Str_cmp("not")){
                        Operation_Expression;
                        Set_Command(Operation,Not);
                        Push_Opcommand;
                       }else if(Str_cmp("neg")){
                        Operation_Expression;
                        Set_Command(Operation,Neg);
                        Push_Opcommand;
                       }else if(Str_cmp("push")){
                         delete ret;
                        Set_Command(Stack,op_None);
                        new_command->set_stackOperation(Push);                       
                        ret = analyse_word(line,idx);
                        idx = ret->index;
                        
                        #ifdef PARSER_DEBUG
                        cout << "[PARSER DEBUG <analyse push >]:  index " << ret->index << " keyword: "<< ret->keyword <<  "  type :  " << ret->type << " Now idx: "<< idx << endl;
                        cout << "[PARSER DEBUG <ret type == Index_value >]:  equ_result===> " << (ret->type ==Index_value) <<" " << ret->type << "   " << Index_value<< endl;
                        #endif
                        #define Set_Segment(segments)  new_command->set_stackSegment(segments);\
                                                                          delete ret;\
                                                                          SKIPS; \
                                                                          if(idx>=line.length()) Compiler_ERROR("Lost value after segment "); \
                                                                          ret = analyse_word(line,idx); \
                                                                          idx = ret->index; \
                                                                          if (ret->type==Index_value){ \
                                                                            new_command->set_indexValue(atoi(ret->keyword.c_str())); \
                                                                            VMcommand_Queue.push(new_command);\
                                                                            new_command = NULL; \
                                                                          }else{ \
                                                                            Compiler_ERROR("Error value type after segment!"); \
                                                                          }

                                                                          
                        if(ret->type == Index_value){
                            Compiler_ERROR("The Push command is behind illegal  in VMcode file  " + to_string(line_number) + " line");
                        }else{
                             if(Str_cmp("local")){
                                Set_Segment(Local);
                             }else if(Str_cmp("argument") || Str_cmp("argu")){
                                Set_Segment(Argument);
                             }else if(Str_cmp("this")){
                                Set_Segment(This);
                             }else if(Str_cmp("that")){
                                Set_Segment(That);
                             }else if(Str_cmp("constant")){
                                Set_Segment(Constant);
                             }else if(Str_cmp("static")){
                                Set_Segment(Static);
                             }else if(Str_cmp("Pointer") || Str_cmp("pointer")){
                                Set_Segment(Pointer);
                             }else if(Str_cmp("temp")){
                                Set_Segment(Temp);
                             }else{
                                Compiler_ERROR("unknown identifier");
                             }
                        }

                       }else if(Str_cmp("pop")){
                        Set_Command(Stack,op_None);
                        new_command->set_stackOperation(Pop);
                          ret = analyse_word(line,idx);
                        idx = ret->index;
                        
                        #ifdef PARSER_DEBUG
                        cout << "[PARSER DEBUG <analyse push >]:  index " << ret->index << " keyword: "<< ret->keyword <<  "  type :  " << ret->type << " Now idx: "<< idx << endl;
                        cout << "[PARSER DEBUG <ret type == Index_value >]:  equ_result===> " << (ret->type ==Index_value) <<" " << ret->type << "   " << Index_value<< endl;
                        #endif
                                                                          
                        if(ret->type == Index_value){
                            Compiler_ERROR("The Push command is behind illegal  in VMcode file  " + to_string(line_number) + " line");
                        }else{
                             if(Str_cmp("local")){
                                Set_Segment(Local);
                             }else if(Str_cmp("argument") || Str_cmp("argu")){
                                Set_Segment(Argument);
                             }else if(Str_cmp("this")){
                                Set_Segment(This);
                             }else if(Str_cmp("that")){
                                Set_Segment(That);
                             }else if(Str_cmp("constant")){
                                Set_Segment(Constant);
                             }else if(Str_cmp("static")){
                                Set_Segment(Static);
                             }else if(Str_cmp("Pointer") || Str_cmp("pointer")){
                                Set_Segment(Pointer);
                             }else{
                                Compiler_ERROR("unknown identifier");
                             }
                        }
                       }else{
                         Compiler_ERROR("Illegal expression \""+line+"\"in VMcode file "+to_string(line_number) +" line");
                       }
                       
                        break;
                    default:
                        General_ERROR("Error analyse in line <" + line + ">" + "line:"+to_string(line_number));
                        break;
                    }

                }

           }
           
           std:: string vm_file_address;
  
    };

#endif