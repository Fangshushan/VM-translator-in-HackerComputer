#ifndef COMPILER_CONTROL_HPP
 #define COMPILER_CONTROL_HPP
 #include <iostream>
void General_ERROR(std:: string  messge){
    std::cout << "<0>_[General Error]:  " << messge  << std::endl;
    while (1){
    }
}

void Compiler_Warning(std:: string messgae){
    std::cout << "[Compiler Warning]:  " <<  messgae << std::endl;
}

void Compiler_ERROR(std::string messge){
        std::cout << "<1>_[Compiler Error]:  " << messge << std::endl;
        while (1){
         }
}
void System_ERROR(std:: string message){
        std::cout << "<!!!>_[System Error]:  " << message << std::endl;
        while (1){
         }
}
#endif
