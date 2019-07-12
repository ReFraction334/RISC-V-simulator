//
// Created by ReFraction on 2019/7/8.
//

#ifndef RISC_V_FIVE_STAGE_HPP
#define RISC_V_FIVE_STAGE_HPP
#include "parser.hpp"
#include "statement.hpp"

bool finished = false;
bool flag = true;
struct IF_ID_Register
{
    int nowcommand,NPC;
}IF_ID;
struct ID_EX_Register
{
    int NPC,address,opcode;
    bool cond;
    reg A,B;
    Statement *st;
}ID_EX;
struct EX_MEM_Register
{
    int ALUoutput,opcode,NPC;
    Statement *st;
    bool cond;
    reg A,B;
}EX_MEM;
struct MEM_WB_Register
{
    int NPC,ALUoutput,opcode,LMD;
    Statement *st;
    reg A,B;
}MEM_WB;
void IF()
{
    if (flag == false){
        flag = true;
        return;
    }
    IF_ID.nowcommand = par.get_command(pc);
    if (ID_EX.st != nullptr && ((ID_EX.opcode == opc_BRANCH && ID_EX.cond) || (ID_EX.opcode == opc_JAL) || (ID_EX.opcode == opc_JALR)))
        IF_ID.nowcommand = par.get_command(ID_EX.address),pc = IF_ID.NPC = ID_EX.address + 4;
    else
        pc = IF_ID.NPC = pc + 4;
    if (IF_ID.nowcommand == EndProgram)
        finished = true;
}

void ID()
{
    TypeName tn = judge_type(IF_ID.nowcommand);
    ID_EX.st = new_statement(IF_ID.nowcommand,ID_EX.NPC = IF_ID.NPC,tn);
    switch (ID_EX.opcode = ID_EX.st->find_opc())
    {
        case opc_OP:
            reinterpret_cast<R_Statement *>(ID_EX.st)->ID_Setting(ID_EX.A,ID_EX.B);
            switch(MEM_WB.st->find_opc()) {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_LOAD:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.LMD;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.LMD;
                    break;
            }
            switch(EX_MEM.st->find_opc())
            {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_LOAD:
                    if ((&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1))||(&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2)))
                    {
                        ID_EX.A.val = 0,ID_EX.B.val = 0;
                        Statement *tmp = ID_EX.st;
                        ID_EX.st = new_statement(NOP,ID_EX.NPC,ADDI);
                        flag = false;
                        delete tmp;
                    }
                    break;
            }
            if (&(reinterpret_cast<R_Statement *>(ID_EX.st)->rs1) == &xRegister[0])
                ID_EX.A.val = 0;
            if (&(reinterpret_cast<R_Statement *>(ID_EX.st)->rs2) == &xRegister[0])
                ID_EX.B.val = 0;
            break;
        case opc_OP_IMM:
        case opc_JALR:
        case opc_LOAD:
            reinterpret_cast<I_Statement *>(ID_EX.st)->ID_Setting(ID_EX.A,ID_EX.B);
            switch(MEM_WB.st->find_opc()) {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    break;
                case opc_LOAD:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.LMD;
                    break;
            }
            switch(EX_MEM.st->find_opc())
            {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    break;
                case opc_LOAD:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                    {
                        Statement *tmp = ID_EX.st;
                        ID_EX.A.val = 0,ID_EX.st = new_statement(NOP,ID_EX.NPC,ADDI);
                        flag = false;
                        delete tmp;
                    }
                    break;
            }
            if (ID_EX.st->find_opc() == opc_JALR)
            {
                reinterpret_cast<I_Statement *>(ID_EX.st)->EX_Setting(ID_EX.A,ID_EX.B,ID_EX.address,ID_EX.cond);
            }
            if (&xRegister[0] == &(reinterpret_cast<I_Statement *>(ID_EX.st)->rs1))
                ID_EX.A.val = 0;
            break;
        case opc_STORE:
            reinterpret_cast<S_Statement *>(ID_EX.st)->ID_Setting(ID_EX.A,ID_EX.B);
            switch(MEM_WB.st->find_opc()) {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_LOAD:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.LMD;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.LMD;
                    break;
            }
            switch(EX_MEM.st->find_opc())
            {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_LOAD:
                    if ((&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))||(&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2)))
                    {
                        ID_EX.A.val = 0,ID_EX.B.val = 0;
                        Statement * tmp = ID_EX.st;
                        ID_EX.st = new_statement(NOP,ID_EX.NPC,ADDI);
                        flag = false;
                        delete tmp;
                    }
                    break;
            }
            if (&xRegister[0] == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs1))
                ID_EX.A.val = 0;
            if (&xRegister[0] == &(reinterpret_cast<S_Statement *>(ID_EX.st)->rs2))
                ID_EX.B.val = 0;
            break;
        case opc_BRANCH:
            reinterpret_cast<B_Statement *>(ID_EX.st)->ID_Setting(ID_EX.A,ID_EX.B);
            switch(MEM_WB.st->find_opc()) {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.NPC;
                    if (&(reinterpret_cast<J_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.ALUoutput;
                    break;
                case opc_LOAD:
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = MEM_WB.LMD;
                    if (&(reinterpret_cast<I_Statement *>(MEM_WB.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = MEM_WB.LMD;
                    break;
            }
            switch(EX_MEM.st->find_opc())
            {
                case opc_OP:
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<R_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_OP_IMM:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_JALR:
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_JAL:
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.NPC;
                    if (&(reinterpret_cast<J_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.NPC;
                    break;
                case opc_LUI:
                case opc_AUIPC:
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                        ID_EX.A.val = EX_MEM.ALUoutput;
                    if (&(reinterpret_cast<U_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                        ID_EX.B.val = EX_MEM.ALUoutput;
                    break;
                case opc_LOAD:
                    if ((&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))||(&(reinterpret_cast<I_Statement *>(EX_MEM.st)->rd) == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2)))
                    {
                        Statement * tmp = ID_EX.st;
                        ID_EX.A.val = 0,ID_EX.B.val = 0,ID_EX.st = new_statement(NOP,ID_EX.NPC,ADDI);
                        flag = false;
                        delete tmp;
                    }
                    break;
            }
            if (&xRegister[0] == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs1))
                ID_EX.A.val = 0;
            if (&xRegister[0] == &(reinterpret_cast<B_Statement *>(ID_EX.st)->rs2))
                ID_EX.B.val = 0;
            reinterpret_cast<B_Statement *>(ID_EX.st)->EX_Setting(ID_EX.A,ID_EX.B,ID_EX.address,ID_EX.cond);
            break;
        case opc_JAL:
            reinterpret_cast<J_Statement *>(ID_EX.st)->EX_Setting(ID_EX.A,ID_EX.B,ID_EX.address,ID_EX.cond);
            break;
    }
}

void EX()
{
    EX_MEM.st = ID_EX.st;
    EX_MEM.opcode = EX_MEM.st->find_opc();
    EX_MEM.NPC = ID_EX.NPC;
    EX_MEM.st->EX_Setting(ID_EX.A,ID_EX.B,EX_MEM.ALUoutput,EX_MEM.cond);
    EX_MEM.A = ID_EX.A; EX_MEM.B = ID_EX.B;
}

void MEM()
{
    MEM_WB.st = EX_MEM.st;
    MEM_WB.NPC = EX_MEM.NPC;
    switch (MEM_WB.opcode = EX_MEM.opcode)
    {
        case opc_OP_IMM:
        case opc_JALR:
            MEM_WB.B = EX_MEM.B;
            MEM_WB.ALUoutput = EX_MEM.ALUoutput;
            break;
        case opc_LUI:
        case opc_AUIPC:
        case opc_OP:
        case opc_JAL:
            MEM_WB.ALUoutput = EX_MEM.ALUoutput;
            break;
        case opc_BRANCH:
            break;
        case opc_LOAD:
            switch(MEM_WB.st->find_tn())
            {
                case LW:
                    MEM_WB.LMD = reinterpret_cast<LW_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput);
                    break;
                case LH:
                    MEM_WB.LMD = reinterpret_cast<LH_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput);
                    break;
                case LHU:
                    MEM_WB.LMD = reinterpret_cast<LHU_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput);
                    break;
                case LB:
                    MEM_WB.LMD = reinterpret_cast<LB_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput);
                    break;
                case LBU:
                    MEM_WB.LMD = reinterpret_cast<LBU_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput);
                    break;
                default: break;
            }
            break;
        case opc_STORE:
            switch(MEM_WB.st->find_tn())
            {
                case SW:
                    reinterpret_cast<SW_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput,EX_MEM.B);
                    break;
                case SH:
                    reinterpret_cast<SH_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput,EX_MEM.B);
                    break;
                case SB:
                    reinterpret_cast<SB_Statement *>(MEM_WB.st)->MEM_Setting(EX_MEM.ALUoutput,EX_MEM.B);
                    break;
                default: break;
            }
            break;
    }
}

void WB()
{
    switch(MEM_WB.opcode)
    {
        case opc_OP_IMM:
        case opc_LUI:
        case opc_AUIPC:
        case opc_OP:
            MEM_WB.st->WB(MEM_WB.ALUoutput);
            break;
        case opc_JAL:
        case opc_JALR:
            MEM_WB.st->WB(MEM_WB.NPC);
            break;
        case opc_BRANCH:
        case opc_STORE:
            break;
        case opc_LOAD:
            MEM_WB.st->WB(MEM_WB.LMD);
            break;
    }
    delete MEM_WB.st;


}

#endif //RISC_V_FIVE_STAGE_HPP
