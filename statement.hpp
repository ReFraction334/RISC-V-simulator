//
// Created by ReFraction on 2019/7/2.
//

#ifndef RISC_V_STATEMENT_HPP
#define RISC_V_STATEMENT_HPP

#include "register.hpp"
#include "parser.hpp"

#define EndProgram 0x00c68223
#define opc_OP_IMM 0b0010011
#define Get_Opcode 127
#define opc_LUI 0b0110111
#define opc_AUIPC 0b0010111
#define opc_OP 0b0110011
#define opc_JAL 0b1101111
#define opc_JALR 0b1100111
#define opc_BRANCH 0b1100011
#define opc_LOAD 0b0000011
#define opc_STORE 0b0100011
#define Get_Funct3 0x7000
#define func3_ADDI 0b000
#define func3_SLTI 0b010
#define func3_SLTIU 0b011
#define func3_ANDI 0b111
#define func3_ORI 0b110
#define func3_XORI 0b100
#define func3_SLLI 0b001
#define func3_SRI 0b101
#define Get_Funct7 0x40000000
#define func3_ADDSUB 0b000
#define func3_SLL 0b001
#define func3_SLT 0b010
#define func3_SLTU 0b011
#define func3_XOR 0b100
#define func3_SR 0b101
#define func3_OR 0b110
#define func3_AND 0b111
#define func3_BEQ 0b000
#define func3_BNE 0b001
#define func3_BLT 0b100
#define func3_BLTU 0b110
#define func3_BGE 0b101
#define func3_BGEU 0b111
#define func3_LB 0b000
#define func3_LH 0b001
#define func3_LW 0b010
#define func3_LBU 0b100
#define func3_LHU 0b101
#define func3_SB 0b000
#define func3_SH 0b001
#define func3_SW 0b010
#define NOP 0x00000013

enum TypeName{ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI,LUI,AUIPC,ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,JAL,JALR,BEQ,BNE,BLT,BLTU,BGE,BGEU,LB,LH,LW,LBU,LHU,SB,SH,SW};

int TurntoLI(int code,int dis)
{
    return NOP | (0xf80 & code) | (dis << 20);
}

TypeName judge_type(unsigned int command)
{
    switch (command & Get_Opcode)
    {
        case opc_OP_IMM:
            switch ((command & Get_Funct3) >> 12)
            {
                case func3_ADDI:
                    return ADDI;
                case func3_SLTI:
                    return SLTI;
                case func3_SLTIU:
                    return SLTIU;
                case func3_ANDI:
                    return ANDI;
                case func3_ORI:
                    return ORI;
                case func3_XORI:
                    return XORI;
                case func3_SLLI:
                    return SLLI;
                case func3_SRI:
                    return (command & Get_Funct7) ? SRAI : SRLI;
            }

        case opc_LUI:
            return LUI;

        case opc_AUIPC:
            return AUIPC;

        case opc_OP:
            switch ((command & Get_Funct3) >> 12)
            {
                case func3_ADDSUB:
                    return (command & Get_Funct7) ? SUB : ADD;
                case func3_SLL:
                    return SLL;
                case func3_SLT:
                    return SLT;
                case func3_SLTU:
                    return SLTU;
                case func3_XOR:
                    return XOR;
                case func3_SR:
                    return (command & Get_Funct7) ? SRA : SRL;
                case func3_OR:
                    return OR;
                case func3_AND:
                    return AND;
            }

        case opc_JAL:
            return JAL;

        case opc_JALR:
            return JALR;

        case opc_BRANCH:
            switch ((command & Get_Funct3) >> 12)
            {
                case func3_BEQ:
                    return BEQ;
                case func3_BNE:
                    return BNE;
                case func3_BLT:
                    return BLT;
                case func3_BLTU:
                    return BLTU;
                case func3_BGE:
                    return BGE;
                case func3_BGEU:
                    return BGEU;
            }

        case opc_LOAD:
            switch ((command & Get_Funct3) >> 12)
            {
                case func3_LB:
                    return LB;
                case func3_LW:
                    return LW;
                case func3_LH:
                    return LH;
                case func3_LBU:
                    return LBU;
                case func3_LHU:
                    return LHU;
            }

        case opc_STORE:
            switch ((command & Get_Funct3) >> 12)
            {
                case func3_SB:
                    return SB;
                case func3_SW:
                    return SW;
                case func3_SH:
                    return SH;
            }
    }
    throw("PE\n");
}
class Statement
{
protected:
    const unsigned int code;
    int & pc;
    TypeName tn;
public:
    Statement(const unsigned int command,int &pc,TypeName tn) :code(command),pc(pc),tn(tn) {}
    virtual void running() = 0;
    virtual ~Statement() = default;
    TypeName find_tn()
    {
        return tn;
    }
    int find_opc()
    {
        switch (tn)
        {
            case ADDI:
            case SLTI:
            case SLTIU:
            case ANDI:
            case ORI:
            case XORI:
            case SLLI:
            case SRLI:
            case SRAI:
                return opc_OP_IMM;
            case LUI:
                return opc_LUI;
            case AUIPC:
                return opc_AUIPC;
            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND:
                return opc_OP;
            case JAL:
                return opc_JAL;
            case JALR:
                return opc_JALR;
            case BEQ:
            case BNE:
            case BLT:
            case BLTU:
            case BGE:
            case BGEU:
                return opc_BRANCH;
            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU:
                return opc_LOAD;
            case SB:
            case SH:
            case SW:
                return opc_STORE;
        }
    }
    int get_code()
    {
        return code;
    }
    virtual void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond) = 0;
    virtual void WB(int ans) = 0;
};
class R_Statement:public Statement
{
public:
    reg &rs1,&rs2,&rd;
public:
    R_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rs1(xRegister[((31 << 15) & command) >> 15]),rs2(xRegister[((31 << 20) & command) >> 20]),rd(xRegister[((31 << 7) & command) >> 7]) {}
    void ID_Setting(reg &A,reg &B)
    {
        A = rs1;
        B = rs2;
    }
};
class I_Statement:public Statement
{
public:
    reg &rs1,&rd;
    const int imm;
public:
    I_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rs1(xRegister[((31 << 15) & command) >> 15]),rd(xRegister[((31 << 7) & command) >> 7]),imm((((0xfff << 20) & command) >> 20) + (((0xfffff) * (((1 << 31) & command) >> 19))) ){}
    void ID_Setting(reg &A,reg &B)
    {
        A = rs1;
        B = rd;
    }
};
class S_Statement:public Statement
{
public:
    reg &rs1,&rs2;
    const int imm;
public:
    S_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rs1(xRegister[((31 << 15) & command) >> 15]),rs2(xRegister[((31 << 20) & command) >> 20]),imm((((0x3f << 25) & command) >> 20) + (((31 << 7) & command) >> 7) + (((0x1fffff) * (((1 << 31) & command) >> 20)))) {}
    void ID_Setting(reg &A,reg &B)
    {
        A = rs1;
        B = rs2;
    }
};
class B_Statement:public Statement
{
public:
    reg &rs1,&rs2;
    const int imm;
public:
    B_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rs1(xRegister[((31 << 15) & command) >> 15]),rs2(xRegister[((31 << 20) & command) >> 20]),imm((((0x3f << 25) & command) >> 20) + (((15 << 8) & command) >> 7) + (((1 << 7) & command) << 4) + (((0xfffff) * (((1 << 31) & command) >> 19)))){}
    void ID_Setting(reg &A,reg &B)
    {
        A = rs1;
        B = rs2;
    }

//    void EX(int &ALUOutput,bool &cond)
//    {
//        ALUOutput = pc + imm;
//        switch
//    }
};
class U_Statement:public Statement
{
public:
    reg &rd;
    const int imm;
public:
    U_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rd(xRegister[((31 << 7) & command) >> 7]),imm(((0xfffff << 12) & command)) {}
    void ID_Setting(reg &A,reg &B)
    {
        A = rd;
    }
};
class J_Statement:public Statement
{
public:
    reg &rd;
    const int imm;
public:
    J_Statement(const unsigned int command,int &pc,TypeName tn):Statement(command,pc,tn),rd(xRegister[((31 << 7) & command) >> 7]),imm(((0xff << 12) & command) + (((1 << 20) & command) >> 9) + (((0x3ff << 21) & command) >> 20) + (((0xfff) * (((1 << 31) & command) >> 11)))) {}
    void ID_Setting(reg &A,reg &B)
    {
        A = rd;
    }
};
class ADDI_Statement:public I_Statement
{
public:
    ADDI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val + imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLTI_Statement:public I_Statement
{
public:
    SLTI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
        {
            if(rs1.val < imm)
                rd.val = 1;
            else
                rd.val = 0;
        }
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = (A.val < imm) ? 1 : 0;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLTIU_Statement:public I_Statement
{
public:
    SLTIU_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
        {
            if((unsigned int)rs1.val < (unsigned int)imm)
                rd.val = 1;
            else
                rd.val = 0;
        }
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = ((unsigned int)A.val < (unsigned int)imm) ? 1 : 0;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class ANDI_Statement:public I_Statement
{
public:
    ANDI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val & imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val & imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class ORI_Statement:public I_Statement
{
public:
    ORI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val | imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val | imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class XORI_Statement:public I_Statement
{
public:
    XORI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val ^ imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val ^ imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLLI_Statement:public I_Statement
{
public:
    SLLI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int shamt = imm & (31);
        if (&rd != &xRegister[0])
            rd.val = rs1.val << shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = imm & (31);
        ALUoutput = A.val << shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SRLI_Statement:public I_Statement
{
public:
    SRLI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int shamt = imm & (31);
        if (&rd != &xRegister[0])
            rd.val = (unsigned int)rs1.val >> shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = imm & (31);
        ALUoutput = (unsigned int)A.val >> shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SRAI_Statement:public I_Statement
{
public:
    SRAI_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int shamt = imm & (31);
        if (&rd != &xRegister[0])
            rd.val = rs1.val >> shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = imm & (31);
        ALUoutput = A.val >> shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class LUI_Statement:public U_Statement
{
public:
    LUI_Statement(const unsigned int command,int &pc,TypeName tn):U_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class AUIPC_Statement:public U_Statement
{
public:
    AUIPC_Statement(const unsigned int command,int &pc,TypeName tn):U_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = pc + imm;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class ADD_Statement:public R_Statement
{
public:
    ADD_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val + rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + B.val;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SUB_Statement:public R_Statement
{
public:
    SUB_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val - rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val - B.val;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLL_Statement:public R_Statement
{
public:
    SLL_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        int shamt = 31 & rs2.val;
        if (&rd != &xRegister[0])
            rd.val = rs1.val << shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = 31 & B.val;
        ALUoutput = A.val << shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLT_Statement:public R_Statement
{
public:
    SLT_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = (rs1.val < rs2.val) ? 1 : 0;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = (A.val < B.val) ? 1 : 0;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SLTU_Statement:public R_Statement
{
public:
    SLTU_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = ((unsigned int)rs1.val < (unsigned int)rs2.val) ? 1 : 0;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = ((unsigned int)A.val < (unsigned int)B.val) ? 1 : 0;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class XOR_Statement:public R_Statement
{
public:
    XOR_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val ^ rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val ^ B.val;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SRL_Statement:public R_Statement
{
public:
    SRL_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        int shamt = 31 & rs2.val;
        if (&rd != &xRegister[0])
            rd.val = (unsigned int)rs1.val >> shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = 31 & B.val;
        ALUoutput = (unsigned int)A.val >> shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SRA_Statement:public R_Statement
{
public:
    SRA_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        int shamt = 31 & rs2.val;
        if (&rd != &xRegister[0])
            rd.val = rs1.val >> shamt;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        int shamt = 31 & B.val;
        ALUoutput = A.val >> shamt;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class OR_Statement:public R_Statement
{
public:
    OR_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val | rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val | B.val;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class AND_Statement:public R_Statement
{
public:
    AND_Statement(const unsigned int command,int &pc,TypeName tn):R_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = rs1.val & rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val & B.val;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class JAL_Statement:public J_Statement
{
public:
    JAL_Statement(const unsigned int command,int &pc,TypeName tn):J_Statement(command,pc,tn){}
    void running()
    {
        if (&rd != &xRegister[0])
            rd.val = pc + 4;
        pc += imm;
        pc -= 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class JALR_Statement:public I_Statement
{
public:
    JALR_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        //qwq
        pc = (imm + rs1.val) & (0xfffffffe);
        if (&rd != &xRegister[0])
            rd.val = pc + 4;
        pc -= 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = (imm + A.val) & (0xfffffffe);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class BEQ_Statement:public B_Statement
{
public:
    BEQ_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if (rs1.val == rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = (A.val == B.val);
    }

    void WB(int ans) {}
};
class BNE_Statement:public B_Statement
{
public:
    BNE_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if (rs1.val != rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = (A.val != B.val);
    }
    void WB(int ans) {}
};
class BLT_Statement:public B_Statement
{
public:
    BLT_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if (rs1.val < rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = (A.val < B.val);
    }
    void WB(int ans) {}
};
class BLTU_Statement:public B_Statement
{
public:
    BLTU_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if ((unsigned int)rs1.val < (unsigned int)rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = ((unsigned int)A.val < (unsigned int)B.val);
    }
    void WB(int ans) {}
};
class BGE_Statement:public B_Statement
{
public:
    BGE_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if (rs1.val >= rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = (A.val >= B.val);
    }
    void WB(int ans) {}
};
class BGEU_Statement:public B_Statement
{
public:
    BGEU_Statement(const unsigned int command,int &pc,TypeName tn):B_Statement(command,pc,tn){}
    void running()
    {
        if ((unsigned int)rs1.val >= (unsigned int)rs2.val)
            pc += imm - 4;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = pc + imm - 4;
        cond = ((unsigned int)A.val >= (unsigned int)B.val);
    }
    void WB(int ans) {}
};
class LW_Statement:public I_Statement
{
public:
    LW_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        if (&rd != &xRegister[0])
            rd.val = par.get_memory(add) + (par.get_memory(add + 1) << 8) + (par.get_memory(add + 2) << 16) + (par.get_memory(add + 3) << 24);
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    int MEM_Setting(int ALUoutput)
    {
        return par.get_memory(ALUoutput) + (par.get_memory(ALUoutput + 1) << 8) + (par.get_memory(ALUoutput + 2) << 16) + (par.get_memory(ALUoutput + 3) << 24);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class LH_Statement:public I_Statement
{
public:
    LH_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        if (&rd != &xRegister[0])
            rd.val = par.get_memory(add) + (par.get_memory(add + 1) << 8),rd.val = (((rd.val & (1 << 15)) * (0xffff)) << 1);
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    int MEM_Setting(int ALUoutput)
    {
        return ((((par.get_memory(ALUoutput) + (par.get_memory(ALUoutput + 1) << 8)) & (1 << 15)) * (0xffff)) << 1);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class LHU_Statement:public I_Statement
{
public:
    LHU_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        if (&rd != &xRegister[0])
            rd.val = par.get_memory(add) + (par.get_memory(add + 1) << 8);
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    int MEM_Setting(int ALUoutput)
    {
        return par.get_memory(ALUoutput) + (par.get_memory(ALUoutput + 1) << 8);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class LB_Statement:public I_Statement
{
public:
    LB_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        if (&rd != &xRegister[0])
            rd.val = par.get_memory(add),rd.val = (((rd.val & (1 << 7)) * (0xffffff)) << 1);
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    int MEM_Setting(int ALUoutput)
    {
        return ((((par.get_memory(ALUoutput)) & (1 << 7)) * (0xffffff)) << 1);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class LBU_Statement:public I_Statement
{
public:
    LBU_Statement(const unsigned int command,int &pc,TypeName tn):I_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        if (&rd != &xRegister[0])
            rd.val = par.get_memory(add);
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    int MEM_Setting(int ALUoutput)
    {
        return par.get_memory(ALUoutput);
    }
    void WB(int ans)
    {
        if (&rd != &xRegister[0])
            rd.val = ans;
    }
};
class SW_Statement:public S_Statement
{
public:
    SW_Statement(const unsigned int command,int &pc,TypeName tn):S_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        par.get_memory(add) = rs2.val & 0xff;
        par.get_memory(add + 1) = (rs2.val & 0xff00) >> 8;
        par.get_memory(add + 2) = (rs2.val & 0xff0000) >> 16;
        par.get_memory(add + 3) = (rs2.val & 0xff000000) >> 24;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    void MEM_Setting(int ALUoutput,reg B)
    {
        par.get_memory(ALUoutput) = B.val & 0xff;
        par.get_memory(ALUoutput + 1) = (B.val & 0xff00) >> 8;
        par.get_memory(ALUoutput + 2) = (B.val & 0xff0000) >> 16;
        par.get_memory(ALUoutput + 3) = (B.val & 0xff000000) >> 24;
    }
    void WB(int ans) {}
};
class SB_Statement:public S_Statement
{
public:
    SB_Statement(const unsigned int command,int &pc,TypeName tn):S_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        par.get_memory(add) = 0xff & rs2.val;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }
    void MEM_Setting(int ALUoutput,reg B)
    {

        par.get_memory(ALUoutput) = B.val & 0xff;
    }
    void WB(int ans) {}
};
class SH_Statement:public S_Statement
{
public:
    SH_Statement(const unsigned int command,int &pc,TypeName tn):S_Statement(command,pc,tn){}
    void running()
    {
        int add = rs1.val + imm;
        par.get_memory(add) = rs2.val & 0xff;
        par.get_memory(add + 1) = (rs2.val & 0xff00) >> 8;
    }
    void EX_Setting(reg A,reg B,int &ALUoutput,bool &cond)
    {
        ALUoutput = A.val + imm;
    }

    void MEM_Setting(int ALUoutput,reg B)
    {

        par.get_memory(ALUoutput) = B.val & 0xff;
        par.get_memory(ALUoutput + 1) = (B.val & 0xff00) >> 8;
    }
    void WB(int ans) {}
};
Statement * new_statement(int code,int &pc,TypeName tn)
{
    switch (tn)
    {
        case ADDI:
            return new ADDI_Statement(code,pc,tn);
        case SLTI:
            return new SLTI_Statement(code,pc,tn);
        case SLTIU:
            return new SLTIU_Statement(code,pc,tn);
        case ANDI:
            return new ANDI_Statement(code,pc,tn);
        case ORI:
            return new ORI_Statement(code,pc,tn);
        case XORI:
            return new XORI_Statement(code,pc,tn);
        case SLLI:
            return new SLLI_Statement(code,pc,tn);
        case SRLI:
            return new SRLI_Statement(code,pc,tn);
        case SRAI:
            return new SRAI_Statement(code,pc,tn);
        case LUI:
            return new LUI_Statement(code,pc,tn);
        case AUIPC:
            return new AUIPC_Statement(code,pc,tn);
        case ADD:
            return new ADD_Statement(code,pc,tn);
        case SUB:
            return new SUB_Statement(code,pc,tn);
        case SLL:
            return new SLL_Statement(code,pc,tn);
        case SLT:
            return new SLT_Statement(code,pc,tn);
        case SLTU:
            return new SLTU_Statement(code,pc,tn);
        case XOR:
            return new XOR_Statement(code,pc,tn);
        case SRL:
            return new SRL_Statement(code,pc,tn);
        case SRA:
            return new SRA_Statement(code,pc,tn);
        case OR:
            return new OR_Statement(code,pc,tn);
        case AND:
            return new AND_Statement(code,pc,tn);
        case JAL:
            return new JAL_Statement(code,pc,tn);
        case JALR:
            return new JALR_Statement(code,pc,tn);
        case BEQ:
            return new BEQ_Statement(code,pc,tn);
        case BNE:
            return new BNE_Statement(code,pc,tn);
        case BLT:
            return new BLT_Statement(code,pc,tn);
        case BLTU:
            return new BLTU_Statement(code,pc,tn);
        case BGE:
            return new BGE_Statement(code,pc,tn);
        case BGEU:
            return new BGEU_Statement(code,pc,tn);
        case LH:
            return new LH_Statement(code,pc,tn);
        case LHU:
            return new LHU_Statement(code,pc,tn);
        case LW:
            return new LW_Statement(code,pc,tn);
        case LB:
            return new LB_Statement(code,pc,tn);
        case LBU:
            return new LBU_Statement(code,pc,tn);
        case SW:
            return new SW_Statement(code,pc,tn);
        case SB:
            return new SB_Statement(code,pc,tn);
        case SH:
            return new SH_Statement(code,pc,tn);
    }
}
#endif //RISC_V_STATEMENT_HPP
