

#ifndef _CTRL_H
#define _CTRL_H
#include "systemc.h"

SC_MODULE(CTRL){
    //////////////////////////
    //PORTS
    /////////////////////////
    sc_in<sc_uint<16> > Instr;
    sc_in<sc_uint<4> > psr;//ZNCF
    sc_in<sc_uint<16> > Rtar_val;//////////////////////// Rtar
    sc_out<sc_uint<16> > pc;
    sc_out<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
    sc_out<sc_uint<10> > ctrl;//RF-2bit, DM-2bit, CTRL-6bit //RF,DM r/w enable port
    //9-RF w; 8-RF r;
    //7-DM w; 6-DM r;
    //5-0 CTRL signal
    sc_out<sc_uint<4> > R_src, R_dest;
    sc_out<sc_uint<4> > R_w;
    ////R_Waddr
    /////////////////////////
    //PORTS
    /////////////////////////

    //local variables
    //displacement is defined locally， and some other decompositions
    sc_uint<16> lcl_instr;//local retrieved instructions
    //local variables that are decoded from the instructions
    //usage of each variables are indicated as their names
    sc_uint<8> Disp;//local displacement
    sc_uint<4> cond_bit;//condition bit, as indicated in Discription page_2
    sc_uint<4> opcode;//
    sc_uint<4> Rdest;//
    sc_uint<4> op_ext;//
    sc_uint<4> Rsrc;
    sc_uint<8> imm;
    sc_uint<4> psr_val;
    sc_uint<1> z;
    sc_uint<1> n;
    sc_uint<1> c;
    sc_uint<1> f;
    sc_uint<16> inter_pc;//internal program counter
    sc_uint<3> shift_op;
    //ctrl_signal is a local variables for ctrl in output ports
    //9-8 Register File Write and Read
    //7-6 Data Memory Write and Read
    //5-0 Control Signals for ALU and etc.
    sc_uint<10> ctrl_signal;
    //local method
    void proc();
    //constructor
    SC_CTOR(CTRL){
      SC_METHOD(proc);
      sensitive<<Instr;
    }
};

#endif