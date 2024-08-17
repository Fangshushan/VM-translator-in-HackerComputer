#include <iostream>
#include "parser.hpp"
#include "CodeWrite.hpp"
using namespace std;

int main(int argc, char const *argv[]){
    if(argc < 3){
        cout << "argument error. example: read_file_address write_file address!" << endl;
    }else{
        Parser machine(argv[1]);
        machine.Vm_to_Asm();
        // // machine.debug_commandQueue();     
        GenAsm asms(machine.get_ParserResult());
        asms.Generate_assembly();
        asms.Generate_asm_file(argv[2]);
        cout <<"[Complete]:Success!" << endl;
    }

}
