

#ifndef _CTRL_H
#define _CTRL_H
#include "systemc.h"

SC_MODULE(CTRL){
    //////////////////////////
    //PORTS
    /////////////////////////
    sc_in<bool> clock;
    sc_in<sc_uint<16> > Instr;
    sc_in<sc_uint<4> > psr;//ZNCF
    sc_in<sc_uint<16> > Rtar_val;//////////////////////// Rtar
    sc_out<sc_uint<16> > pc;
    sc_out<sc_uint<8> > Imm;//signed or unsigned, 8-bit or 16-bit
    /////////////////////////////
    sc_out<sc_uint<6> > alu_ctrl;
    sc_out<sc_uint<1> >m4;
    sc_out<sc_uint<1> > m3;
    sc_out<sc_uint<1> > m2;
    sc_out<sc_uint<1> > m1;
    sc_out<sc_uint<1> > rf_wr;
    sc_out<sc_uint<1> > rf_rd;
    sc_out<sc_uint<1> > dm_wr;
    sc_out<sc_uint<1> > dm_rd;
    sc_out<sc_uint<4> > R_src, R_dest;
    sc_out<sc_uint<4> > R_w;
    //write back signal
    //00 - choose alu result
    //01 - choose mem
    //10 - choose pc//deactivited at Mar.10th
    sc_out<sc_uint<2> > wb_sig;
    //0 - choose alu result
    //1 - choose pc 
    sc_out<sc_uint<1> > pc_alu_mux;
    //0 - choose mdr
    //1 - choose 000000000
    sc_out<sc_uint<1> > mdr_mux;
    //0 - choose mar
    //1 - choose 000000000
    sc_out<sc_uint<1> > mar_mux;


    ////R_Waddr
    /////////////////////////
    //PORTS
    /////////////////////////


    //local variables
    //displacement is defined locally， and some other decompositions
    sc_uint<16> lcl_instr;//local retrieved instructions
    sc_uint<1> pc_alu_m;
    sc_uint<1> mdr_m;
    sc_uint<1> mar_m;
    //local variables that are decoded from the instructions
    //usage of each variables are indicated as their names
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
    sc_uint<1> imm_sign;
    sc_uint<2> wb_sig_temp;
    sc_uint<6> ctrl_signal;
    sc_uint<1> m1_val,m2_val,m3_val,m4_val,rf_w,rf_r,dm_w,dm_r;
    sc_uint<16> counter_instr;
    sc_uint<4> counter_psr;
    sc_uint<1> counter_z;
    sc_uint<1> counter_n;
    sc_uint<1> counter_c;
    sc_uint<1> counter_f;
    sc_uint<4> counter_cond;
    sc_uint<8> counter_disp;
    sc_uint<1> start;
    sc_signal<sc_uint<16> > temp_instr;
    sc_signal<sc_uint<4> > temp_psr;
    //CTRL_IN* ctrl_in;
 
    void counter();
    void proc();
    void ctrl_in_reg();

    SC_CTOR(CTRL){
      start = 0;
      //ctrl_in = new CTRL_IN("ctrl_in");

      //ctrl_in->instr_in(Instr);
      //ctrl_in->psr_in(psr);
      //ctrl_in->clock(clock);
      //ctrl_in->instr_out(temp_instr);
      //ctrl_in->psr_out(temp_psr);

      inter_pc=0;
      SC_METHOD(counter);
      sensitive << clock.pos();

      SC_METHOD(proc);
      sensitive << temp_instr << temp_psr<< pc;

      SC_METHOD(ctrl_in_reg);
      sensitive << clock.pos();
    }
};

#endif

