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
#define DEBUG

int run_program()
{
    unsigned int nowCommand;
    while((nowCommand = par.get_command()) != EndProgram)
    {
        TypeName tn = judge_type(nowCommand);
        Statement *st = new_statement(nowCommand,pc,tn);
        st->running();
        par.next_command();
        delete(st);
    }
    return ((unsigned int)xRegister[10].val) & 255u;
}

int main(int argc,char * argv[])
{
    std::cout << run_program();
    return 0;
}
