//
// Created by ReFraction on 2019/7/3.
//
//
// Created by ReFraction on 2019/7/1.
//
#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "register.hpp"
#include "statement.hpp"
#include "five-stage.hpp"

int run_program()
{
    int high = 1;
    while (!finished)
    {
        if (high == 5)
            WB(),--high;
        if (high >= 4)
            MEM();
        if (high >= 3)
            EX();
        if (high >= 2)
            ID();
        if (high >= 1)
            IF(),++high;
    }
    WB();
    MEM();
    EX();
    WB();
    MEM();
    WB();
//    unsigned int nowCommand;
//    while((nowCommand = par.get_command()) != EndProgram)
//    {
//        TypeName tn = judge_type(nowCommand);
//        Statement *st = new_statement(nowCommand,pc,tn);
//        st->running();
//        par.next_command();
//        delete(st);
//    }
    return ((unsigned int)xRegister[10].val) & 255u;
}

int main(int argc,char * argv[])
{
    std::cout << run_program();
    return 0;
}
