//
// Created by ReFraction on 2019/7/2.
//

#ifndef RISC_V_REGISTER_HPP
#define RISC_V_REGISTER_HPP
class reg
{
public:
    int val;
    reg(int val = 0):val(val){};
}xRegister[32];
int pc;
#endif //RISC_V_REGISTER_HPP
