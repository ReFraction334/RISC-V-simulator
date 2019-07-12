//
// Created by ReFraction on 2019/7/2.
//

#ifndef RISC_V_PARSER_HPP
#define RISC_V_PARSER_HPP

#include <iostream>
#include <queue>
#include "register.hpp"

//#define DEBUG
class parser
{
    int mem[0x20000];
    std::istream &is;
    int &pc;
public:
    void initialize_memory();
    parser(std::istream &is,int &pc):is(is),pc(pc)
    {
        initialize_memory();
    }
    unsigned int get_command(int NPC);
    void next_command(){    pc += 4;}
    int & get_memory(int n)
    {
        return mem[n];
    }
}par(std::cin,pc);
void parser::initialize_memory()
{
    char ch;
    while (!is.eof() && (ch = is.get()) != '@');
    while (!is.eof())
    {
        int beg = 0;
        for (int i = 0; i < 8; ++i)
        {
            ch = is.get();
            beg <<= 4;
            beg += (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'A' + 10);
        }
        while (!is.eof() && (ch = is.get()) != '@')
        {
            if ((ch >= 'A' && ch <= 'F')||(ch >= '0' && ch <= '9'))
            {
                mem[beg] = (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'A' + 10);
                if (!is.eof() && (ch = is.get()) != '@' && ((ch >= 'A' && ch <= 'F')||(ch >= '0' && ch <= '9')))
                {
                    mem[beg] <<= 4;
                    mem[beg++] += (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'A' + 10);
                }
                else
                    throw("PE\n");
            }
        }
    }
}
unsigned int parser::get_command(int NPC)
{
    unsigned int ans = 0;
    for (int i = NPC + 3; i >= NPC ; --i)
    {
        ans <<= 8;
        ans += mem[i];
    }
    return ans;
}
#endif //RISC_V_PARSER_HPP
