

#ifndef _ALU_H
#define _ALU_H
#include "systemc.h"

//ALU finished
//ALU should be sensitive to clock, for highest priority, if clock enabled
//ALU is also controlled by control signals
//Error exists: Immediate value signed or unsigned
SC_MODULE(ALU){
  sc_in<bool> clk;
  sc_in<sc_uint<16> > DATA1, DATA2;
  sc_in<sc_uint<8> > IMM;
  sc_in<sc_uint<6> > CONTROL;
  sc_in<sc_uint<1> > D1, D2, D3, D4;
  sc_in<sc_uint<2> > WB;
  sc_in<sc_uint<4> > RW;
  sc_in<sc_uint<1> > R_W, D_W, D_R;
  sc_in<sc_uint<1> > pc_alu_mux;
  sc_in<sc_uint<1> > mdr_mux;
  sc_in<sc_uint<1> > mar_mux;
  sc_in<sc_uint<16> > PC;  


  sc_out<sc_uint<4> > PSR;
  sc_out<sc_uint<16> > MAR, MDR, RFF;
  sc_out<sc_uint<2> > wb_signal;
  sc_out<sc_uint<4> > RW_OUT;
  sc_out<sc_uint<1> > R_W_OUT, D_W_OUT, D_R_OUT;
 
///////////// pipeline 信号 ////////////
  sc_signal<sc_uint<8> > imm_reg_in;
  sc_signal<sc_uint<6> > control_reg_in;
  sc_signal<sc_uint<1> > d1_reg, d2_reg, d3_reg, d4_reg;
  sc_signal<sc_uint<2> > wb_signal_reg_1;
  sc_signal<sc_uint<1> > r_w_reg, d_w_reg, d_r_reg;
  sc_signal<sc_uint<1> > r_w_reg_out,d_w_reg_out,d_r_reg_out;
  sc_signal<sc_uint<4> > rw_reg,rw_reg_out;

///////////////////////////////////////



  //data1:Rdest data2:Rsrc,Ramt,Raddr
 
  sc_uint<16> data1;
  sc_uint<16> data2;
  sc_uint<16> result;
  sc_signal<sc_uint<16> > result_reg;

  sc_uint<1> d1, d2, d3, d4;
  sc_uint<16> d1_out, d2_out, d3_out, d4_out;

  sc_uint<8> im;
  sc_uint<6> control;
  sc_uint<4> psr;
  sc_uint<16> imm, imm_ori;
  int i;

  sc_uint<16> data2inv, imminv;
  int lendata1, lendata2, lenresult, lenimm, lenresult_cmp;
  sc_uint<17> mmax;
  sc_uint<16> result_cmp;
  sc_uint<16> right1;
  sc_uint<4> right2;
  sc_uint<16> right3;
  sc_uint<4> right4;
  sc_uint<1> left_most;

  void proc();
  void pipeline();
  //void pipeline_out();
  SC_CTOR(ALU){
    SC_METHOD(proc);
    sensitive << DATA1 << DATA2 << imm_reg_in << control_reg_in << d1_reg << d2_reg << d3_reg << d4_reg;
    SC_METHOD(pipeline);
    sensitive << clk.pos();
  }
};

#endif
	
